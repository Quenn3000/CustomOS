#pragma once

#include <types.hpp>
#define MEMORY_MAP_ENTRIES_COUNTER_ADDRESS 0x8FFE
#define MEMORY_MAP_ENTRIES_ADDRESS 0x9000 // you can find it in bootloader.asm // I should create a way to centralize this value


struct MemoryMapEntry {
	uint64_t base_address;
	uint64_t length;
	uint32_t type;
	uint32_t attributes;
} __attribute__((packed));

extern MemoryMapEntry* memory_map;
extern uint16_t* memory_map_counter;