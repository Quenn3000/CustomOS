#ifndef __IDT_H_
#define __IDT_H_

#include "types.hpp"
#include "utils.hpp"

#define IDT_SIZE 256 // 256 possible interrupts


#define PIC_MASTER_COMMAND_PORT 0x20
#define PIC_SLAVE_COMMAND_PORT 0xA0

#define PIC_MASTER_DATA_PORT 0x21
#define PIC_SLAVE_DATA_PORT 0xA1


struct IDTEntry {
    uint16_t offset_low;   // Lower 16 bits of ISR address
    uint16_t selector;     // Code segment selector (usually 0x08)
    uint8_t zero;     // Type and attributes (0x8E for 32-bit interrupt gate)
    uint8_t type_attr;          // Always zero
    uint16_t offset_high;  // Upper 16 bits of ISR address
} __attribute__((packed));

struct IDTPointer {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));


typedef struct IDTEntry IDTEntry;
typedef struct IDTPointer IDTPointer;

extern "C" void load_idt(IDTPointer* idt_ptr);


void set_idt_gate(int n, uint32_t handler);
extern "C" void isr_test(IDTEntry* entry);
void idt_install();
void init_idt();
void init_pic();


#endif