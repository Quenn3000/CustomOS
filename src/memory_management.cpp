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


MemoryManager::MemoryManager() : memory_map_counter(*(uint16_t*)MEMORY_MAP_ENTRIES_COUNTER_ADDRESS), memory_map((MemoryMapEntry*)MEMORY_MAP_ENTRIES_ADDRESS) {}

MemoryMapEntry* MemoryManager::get_block(int i) {
	if (0 <= i && i < this->memory_map_counter) {
		return &this->memory_map[i];
	}
	return NULL;
}