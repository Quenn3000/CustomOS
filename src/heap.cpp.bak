#include <types.hpp>
#include <heap.hpp>
#include <utils.hpp>

char heap[HEAP_SIZE] = {0};
HeapChunk heap_chunks[HEAPCHUNK_NUMBER];


bool init_heap() {
	if ((float)HEAP_SIZE/HEAPCHUNK_NUMBER != HEAPCHUNK_SIZE) {
		return false;
	}

	for (int i=0; i<HEAPCHUNK_NUMBER; i++) {
		heap_chunks[i].used = false;
		heap_chunks[i].linked = false;
	}

	return true;
}


void * malloc(uint32_t size) {

	int scan_nb = 0;

	for (int i=0; i<HEAPCHUNK_NUMBER; i++) {
		if (!heap_chunks[i].used) {
			scan_nb += 1;

			if (scan_nb*HEAPCHUNK_SIZE >= size) {
				for (int j=-scan_nb+1; j<0; j++) {
					heap_chunks[i+j].used = true;
					heap_chunks[i+j].linked = true;
				}
				heap_chunks[i].used = true;
				heap_chunks[i].linked = false;

				return &heap_chunks + (i-scan_nb+1)*HEAPCHUNK_SIZE;
			}
		} else {
			scan_nb = 0;
		}
	}

	return (void*) 0;
}


void free(void * address) {
	int i;
	for (i = ((int) address - (int) &heap)/HEAPCHUNK_SIZE ; i<HEAPCHUNK_NUMBER && heap_chunks[i].linked; i++) {
		heap_chunks[i].used = false;
		heap_chunks[i].linked = false;
	}
	heap_chunks[i].used = false;
}

void print_heapchunk() {
	for (int i=0; i<HEAPCHUNK_NUMBER; i++) {
		print_int(heap_chunks[i].used ? 1 : 0);
	}
}