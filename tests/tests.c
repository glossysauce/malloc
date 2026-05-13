#include <stdint.h> 
#include <stdbool.h>
#include <stdio.h>
#include "functions.h"
#include <string.h>

void test_basic(void){
    void *a = my_malloc(32);
    void *b = my_malloc(64);
    void *c = my_malloc(128);
    heap_dump();
    my_free(b);
    heap_dump();
    void *d = my_malloc(64);
    heap_dump();
}

int main(void){
    heap_init();
    test_basic();
}