#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "functions.h"
#include <string.h>

#define PASS(name) printf("[PASS] %s\n", name)
#define FAIL(name) printf("[FAIL] %s\n", name)
#define CHECK(name, cond) do { if (cond) PASS(name); else FAIL(name); } while(0)

void test_basic(void){
    void *a = my_malloc(32);
    void *b = my_malloc(64);
    void *c = my_malloc(128);
    heap_dump();
    my_free(b);
    heap_dump();
    void *d = my_malloc(64);
    heap_dump();
    (void)a; (void)c; (void)d;
}

void test_malloc_exact_fit(void){
    heap_init();
    void *a = my_malloc(64);
    my_free(a);
    void *b = my_malloc(64);
    CHECK("malloc exact fit returns non-null", b != NULL);
}

void test_malloc_size_zero(void){
    heap_init();
    void *a = my_malloc(0);
    CHECK("malloc size 0 does not crash", 1);
    (void)a;
    heap_dump();
}

void test_malloc_no_space(void){
    heap_init();
    void *a = my_malloc(1 << 20);
    CHECK("malloc returns null when no space", a == NULL);
}

void test_double_free(void){
    heap_init();
    void *a = my_malloc(64);
    my_free(a);
    my_free(a);
    CHECK("double free does not crash", 1);
}

void test_coalesce(void){
    heap_init();
    void *a = my_malloc(64);
    void *b = my_malloc(64);
    my_free(a);
    my_free(b);
    void *c = my_malloc(128);
    CHECK("coalesce adjacent free blocks", c != NULL);
    (void)c;
}

void test_realloc_null(void){
    heap_init();
    void *a = my_realloc(NULL, 64);
    CHECK("realloc null behaves like malloc", a != NULL);
}

void test_realloc_size_zero(void){
    heap_init();
    void *a = my_malloc(64);
    void *b = my_realloc(a, 0);
    CHECK("realloc size 0 frees and returns null", b == NULL);
}

void test_realloc_smaller(void){
    heap_init();
    void *a = my_malloc(128);
    void *b = my_realloc(a, 32);
    CHECK("realloc smaller returns same pointer", b == a);
}

void test_realloc_inplace(void){
    heap_init();
    void *a = my_malloc(64);
    void *b = my_malloc(64);
    my_free(b);
    void *c = my_realloc(a, 96);
    CHECK("realloc expands in place when next block is free", c == a);
    (void)c;
}

void test_realloc_next_too_small(void){
    heap_init();
    void *a = my_malloc(64);
    void *b = my_malloc(16);
    my_free(b);
    void *c = my_realloc(a, 128);
    CHECK("realloc falls back when next block too small", c != NULL);
    (void)c;
}

void test_realloc_fallback(void){
    heap_init();
    void *a = my_malloc(64);
    void *b = my_malloc(64);
    void *c = my_realloc(a, 128);
    CHECK("realloc fallback malloc+copy+free", c != NULL && c != a);
    (void)b; (void)c;
}

void test_calloc_normal(void){
    heap_init();
    int *a = (int *)my_calloc(4, sizeof(int));
    int all_zero = 1;
    for (int i = 0; i < 4; i++) if (a[i] != 0) all_zero = 0;
    CHECK("calloc zeroes memory", all_zero);
}

void test_calloc_no_space(void){
    heap_init();
    void *a = my_calloc(1 << 20, 1);
    CHECK("calloc returns null when no space", a == NULL);
}

void test_calloc_zero(void){
    heap_init();
    void *a = my_calloc(0, sizeof(int));
    CHECK("calloc with num=0 does not crash", 1);
    (void)a;
}

//fail
void test_fragmentation(void){
    heap_init();
    void *blocks[10];
    for (int i = 0; i < 10; i++) blocks[i] = my_malloc(64);
    for (int i = 0; i < 10; i += 2) my_free(blocks[i]);
    void *big = my_malloc(256);
    CHECK("fragmented heap cannot service large alloc without coalescing", big == NULL);
}

int main(void){
    printf("\n--- basic ---\n");
    heap_init();
    test_basic();

    printf("\n--- malloc ---\n");
    test_malloc_exact_fit();
    test_malloc_size_zero();
    test_malloc_no_space();

    printf("\n--- free ---\n");
    test_double_free();
    test_coalesce();

    printf("\n--- realloc ---\n");
    test_realloc_null();
    test_realloc_size_zero();
    test_realloc_smaller();
    test_realloc_inplace();
    test_realloc_next_too_small();
    test_realloc_fallback();

    printf("\n--- calloc ---\n");
    test_calloc_normal();
    test_calloc_no_space();
    test_calloc_zero();

    printf("\n--- fragmentation ---\n");
    test_fragmentation();

    return 0;
}
