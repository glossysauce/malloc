# malloc

custom memory allocator written in C, containing `malloc`, `free`, `realloc`, and `calloc` on top of a static 1MB arena.

## how it works

at startup, `heap_init()` stamps a single giant free block across a static `uint8_t heap[1MB]` array. every block carries a small header describing its size, status, and neighbors. `malloc` carves new blocks out of free space; `free` flips a bit and merges adjacent free blocks.

```
[header | data...][header | data...][header | data (free)...]
```

