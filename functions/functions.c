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

void *my_malloc(size_t size){
    block_header_t *current = head;

    while (current != NULL) {
        if (current->is_free == true && size <= current->size) {
            //stamp new_block at current + 1 + size
            block_header_t *new_block = (block_header_t *)((uint8_t *)(current + 1) + size);
            //new block is the new unallocated space, current becomes the allocated block
            new_block->size = current->size - size - sizeof(block_header_t);
            new_block->is_free = true;
            new_block->next = current->next;
            // update current's fields
            current->size = size;
            current->is_free = false;
            current->next = new_block;
            // return pointer to current's data region
            return (void*)(current + 1);
        }
        current = current->next;
    }

    return NULL; // no block found
}

void my_free(void *ptr){
    block_header_t *header = (block_header_t *)ptr - 1;
    header->is_free = true;

    //coalesce 2 consecutive blocks, implement recursive later
    if (header->next != NULL && header->next->is_free){
        header->size += header->next->size + sizeof(block_header_t);
        header->next = header->next->next;
    }
}