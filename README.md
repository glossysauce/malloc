# malloc

custom memory allocator written in C, containing `malloc`, `free`, `realloc`, and `calloc` on top of a static 1MB arena.

## how it works

at startup, `heap_init()` stamps a single giant free block across a static `uint8_t heap[1MB]` array. every block carries a small header describing its size, status, and neighbors. `malloc` carves new blocks out of free space; `free` flips a bit and merges adjacent free blocks.

```
[header | data...][header | data...][header | data (free)...]
```

## API

```c
void  heap_init(void);                       // must call before anything else
void *my_malloc(size_t size);                // allocate size bytes
void  my_free(void *ptr);                    // free a previously allocated block
void *my_realloc(void *ptr, size_t new_size); // resize an allocation
void *my_calloc(size_t num, size_t size);    // allocate and zero num*size bytes
void  heap_dump(void);                       // print all blocks (for debugging)
```

## features

- **first-fit allocation** — walks the block list and takes the first block that fits
- **block splitting** — carves only what's needed, leaving the rest as a new free block
- **forward + backward coalescing** — merges adjacent free blocks on `free()` to counter mem fragmentation
- **in-place realloc** — expands into the next block if it's free and big enough, avoiding copies
- **doubly linked block list** — `prev` pointer enables O(1) backward coalescing

## build & run

```bash
gcc -Wall -Wextra -I functions -o allocator functions/functions.c tests/tests.c && ./allocator
```
