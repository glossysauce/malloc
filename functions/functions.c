#include <stdint.h> 
#include <stdbool.h>
#define HEAP_SIZE (1 << 20) //1 MB

uint8_t heap[HEAP_SIZE];

typedef struct block_header{
    size_t size;
    bool is_free;
    struct block_header* next;
}block_header_t;

static block_header_t *head = NULL;

void heap_init(void){
    head = (block_header_t *)heap;
    head->size = HEAP_SIZE - sizeof(block_header_t);
    head->is_free = true;
    head->next = NULL;
}

//malloc - find block where is_free == true and size >= requested size

