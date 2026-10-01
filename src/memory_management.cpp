#include <cstddef>
#include <memory_management.hpp>
#include <types.hpp>
#include <utils.hpp>

MemoryManager* MemoryManager::_instance_address = NULL;
MemoryManager MemoryManager::_instance;


MemoryManager* MemoryManager::Instance() {
	if (MemoryManager::_instance_address == NULL) {
		MemoryManager::_instance = MemoryManager();
		MemoryManager::_instance_address = &MemoryManager::_instance;
	}

	return MemoryManager::_instance_address;
}


MemoryManager::MemoryManager() : memory_map_counter(*(uint16_t*)MEMORY_MAP_ENTRIES_COUNTER_ADDRESS), memory_map((MemoryMapEntry*)MEMORY_MAP_ENTRIES_ADDRESS) {
	// --- GET CORRECT MEMORY BLOCK ---
	char count = 2;

	int16_t block_nb = get_block_number();
	MemoryMapEntry* correct_block = NULL;
	int index = 0;
	while (index<block_nb) {
		MemoryMapEntry* block = get_block(index++);
		if (block->type == USABLE) {
			if(--count == 0) {
				correct_block = block;
				break;
			}

		}
	}

	bitmap = correct_block->base_address; // set address for bitmap


	// I assume here correct_block contains the correct block (lol) of memory

	// --- SET BITMAP ---
	bitmap_size = (uint64_t)((correct_block->length) / (uint64_t) ((MEMORY_PROCESS_BLOCK_SIZE+1) * 8)); // *8 bcs bitmat is an int8_t, so 8 blocks per int
	
	print_string("Bitmap size : ");
	print_int(&bitmap);
	print_string("\n\n");
	for (int i=0; i<bitmap_size; bitmap[i++] = 0x00); // initialize all to 0 (not used)

	memory_block_adress = bitmap + bitmap_size*sizeof(uint8_t);

}


MemoryMapEntry* MemoryManager::get_block(int i) {
	if (0 <= i && i < this->memory_map_counter) {
		return &this->memory_map[i];
	}
	return NULL;
}

int16_t MemoryManager::get_block_number() {
	int16_t* counter_address = (int16_t*)MEMORY_MAP_ENTRIES_COUNTER_ADDRESS;
	return *(int16_t*)(counter_address);
}


void* MemoryManager::malloc() { // return 4Ko free address
	int index;
	for (index=0; index < bitmap_size && bitmap[index] == 0xFF; index+=1);
	if (index == bitmap_size)
		return NULL;


	int offset = 0;
	if (!bitmap[index] == 0) {
		int mask;
		for (mask=1; (bitmap[index] & mask) && offset<8; offset+=1) {
			mask = 1<<offset;
		}
		offset-=1;
	}

	bitmap[index] |= 1<<offset; // reserve block in the bitmap

	return (void*) this->memory_block_adress + (index*8 + offset) *MEMORY_PROCESS_BLOCK_SIZE;
}


bool MemoryManager::free(void* addr) {
	int pos = ((int)addr - (int)(this->memory_block_adress)) / MEMORY_PROCESS_BLOCK_SIZE;
	int index = ((int)addr - (int)(this->memory_block_adress)) / (MEMORY_PROCESS_BLOCK_SIZE * 8);
	int offset = (((int)addr - (int)(this->memory_block_adress)) / MEMORY_PROCESS_BLOCK_SIZE) % 8;

	bitmap[index] &= ~(1<<offset);

	return true;
}