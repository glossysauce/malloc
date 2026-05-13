#pragma once

#include <stddef.h>

void  heap_init(void);
void *my_malloc(size_t size);
void  my_free(void *ptr);
void *my_realloc(void *ptr, size_t new_size);
void *my_calloc(size_t num, size_t size);
void  heap_dump(void);
