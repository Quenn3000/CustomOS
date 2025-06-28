#ifndef __HEAP_H_
#define __HEAP_H_

#define HEAP_SIZE 64000
#define HEAPCHUNK_NUMBER 1000
#define HEAPCHUNK_SIZE HEAP_SIZE/HEAPCHUNK_NUMBER

typedef struct HeapChunk {
	bool linked;
	bool used;
} HeapChunk;


bool init_heap();
void * malloc(uint32_t size);
void free(void * address);

void print_heapchunk();

#endif