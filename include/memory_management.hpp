#pragma once

#include <types.hpp>
#define MEMORY_MAP_ENTRIES_COUNTER_ADDRESS 0x8FFE
#define MEMORY_MAP_ENTRIES_ADDRESS 0x9000 // you can find it in bootloader.asm // I should create a way to centralize this value

#define MEMORY_PROCESS_BLOCK_SIZE 4096 // 4Ko

/*
Address Range Descriptor Structure (from the doc)

Offset in Bytes		Name				Description
	0	    			BaseAddrLow			Low 32 Bits of Base Address
	4	    			BaseAddrHigh		High 32 Bits of Base Address
	8	    			LengthLow			Low 32 Bits of Length in Bytes
	12	    			LengthHigh			High 32 Bits of Length in Bytes
	16	    			Type				Address type of  this range.


Type Description
	1 -> Usable RAM
	2 -> Unusuable RAM (reserved by BIOS or hardware)
	3 -> ACPI (should not exist here)
	4 -> ACPI NVS (I even don't know what it is, but same as above)
	5 -> Bad Memory
*/
enum MemoryType {
	USABLE = 1,
	RESERVED = 2,
	ACPI = 3,
	ACPI_NVS = 4,
	BAD_MEMORY = 5 
};


struct MemoryMapEntry {
	uint64_t base_address;
	uint64_t length;
	uint32_t type;
	uint32_t attributes;
} __attribute__((packed));

/*
bitmap spec:
array of 8 blocks indicating if they are free
	lowest bit : first block
	highest bit : last block
*/
class MemoryManager {
	public:
		uint16_t memory_map_counter;
		
		static MemoryManager* Instance();

		// --- getters ---
		MemoryMapEntry* get_block(int i);
		int16_t get_block_number();

		// --- pratical functions ---
		void* malloc();
		bool free(void* addr);
		bool isFree(void* addr);
	
	protected:
		MemoryManager();

	private:
		static MemoryManager* _instance_address;
		static MemoryManager _instance;
	
		MemoryMapEntry* memory_map;
		
		uint8_t* bitmap;
		void* memory_block_adress; // address of the real entrypoint for the block address
		uint64_t bitmap_size;
};