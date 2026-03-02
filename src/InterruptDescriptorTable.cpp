#include "InterruptDescriptorTable.hpp"
#include "utils.hpp"
#include "interrupt_handlers.hpp"
#include "ioport.hpp"

// https://pdos.csail.mit.edu/6.828/2005/readings/i386/s09_05.htm

struct IDTEntry idt[IDT_SIZE];

struct IDTPointer idt_ptr;


void set_idt_gate(int n, uint32_t handler) {
    idt[n].offset_low = handler & 0xFFFF;
    idt[n].selector = 0x08; // Kernel code segment
    idt[n].zero = 0;
    idt[n].type_attr = 0x8E; // 32-bit interrupt gate
    idt[n].offset_high = (handler >> 16) & 0xFFFF;

}

// set all parameters to call correctly load_idt
void idt_install() {
    idt_ptr.limit = (sizeof(IDTEntry) * 256) - 1;
    idt_ptr.base = (uint32_t)&idt;

    //print_int((int)&idt_ptr);

    load_idt(&idt_ptr);


    //print_string("IDT installed\n");
}

// global function for initializing IDT
void init_idt() {
    asm volatile ("cli");

    set_idt_gates();

    idt_install();
    init_pic();

    asm volatile ("sti");

    //print_string("IDT Initialized\n");

    return;

}

// PIC remapping and masking
void init_pic() {
    outb(PIC_MASTER_COMMAND_PORT, 0x11); // initialise the configuration
    outb(PIC_SLAVE_COMMAND_PORT, 0x11);

    outb(PIC_MASTER_DATA_PORT, 0x20); // set start of interrupts number
    outb(PIC_SLAVE_DATA_PORT, 0x28);

    outb(PIC_MASTER_DATA_PORT, 0x04); // set the connection between Slave and Master PICs
    outb(PIC_SLAVE_DATA_PORT, 0x02);

    outb(PIC_MASTER_DATA_PORT, 0x1); // 8086 environment
    outb(PIC_SLAVE_DATA_PORT, 0x1);

    outb(PIC_MASTER_DATA_PORT, 0xFD); // mask all interrupts except IRQ1
    outb(PIC_SLAVE_DATA_PORT, 0xFF); // mask all interrupts

    return;
}