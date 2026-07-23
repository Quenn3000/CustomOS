#pragma once

#include <types.hpp>
#define MEMORY_MAP_ENTRIES_COUNTER_ADDRESS 0x8FFE
#define MEMORY_MAP_ENTRIES_ADDRESS 0x9000 // you can find it in bootloader.asm // I should create a way to centralize this value

/*
Address Range Descriptor Structure (from the doc)

Offset in Bytes		Name		Description
	0	    BaseAddrLow		Low 32 Bits of Base Address
	4	    BaseAddrHigh	High 32 Bits of Base Address
	8	    LengthLow		Low 32 Bits of Length in Bytes
	12	    LengthHigh		High 32 Bits of Length in Bytes
	16	    Type		Address type of  this range.

*/
struct MemoryMapEntry {
	uint64_t base_address;
	uint64_t length;
	uint32_t type;
	uint32_t attributes;
} __attribute__((packed));

class MemoryManager {
	public:
		uint16_t memory_map_counter;
		
		static MemoryManager* Instance();

		MemoryMapEntry* get_block(int i);

		int16_t get_block_number();
	
	protected:
		MemoryManager();

	private:
		static MemoryManager* _instance_address;
		static MemoryManager _instance;
	
		MemoryMapEntry* memory_map;
};