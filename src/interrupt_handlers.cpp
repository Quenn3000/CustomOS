#include "interrupt_handlers.hpp"
#include "InterruptDescriptorTable.hpp"
#include "ioport.hpp"

//void(*interrupt_handlers[IDT_SIZE])(IDTEntry*);

bool SCANCODE_PRESSED[SCANCODE_NUMBER];
char KEYBOARD_BUFFER = 0;


extern "C" __attribute__ ((interrupt)) void isr_test(IDTEntry* entry) {
    //__asm__ volatile ("cli");
    print_string("interruption handled");

    return;
}


extern "C" __attribute__((interrupt)) void keyboard_handler(IDTEntry* entry) {

    uint8_t scancode = inb(0x60);

    bool pressed = !(scancode & 128);
    scancode &= 127;

    if (pressed) {
        SCANCODE_PRESSED[scancode] = true;
        KEYBOARD_BUFFER = scancode;
    } else {
        SCANCODE_PRESSED[scancode] = false;
    }
    
    outb(PIC_MASTER_COMMAND_PORT, 0x20);

    return;
}

extern "C" __attribute__((interrupt)) void default_handler(IDTEntry* entry) {
    // You can print something or halt
    outb(PIC_MASTER_COMMAND_PORT, 0x20);
}

void set_idt_gates() {
	set_idt_gate(0x80, (uint32_t)isr_test);  // Set IRQ1 (keyboard) for testing
    set_idt_gate(0x20, (uint32_t)default_handler);
    set_idt_gate(0x21, (uint32_t)keyboard_handler);
}