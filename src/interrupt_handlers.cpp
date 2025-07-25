#include "interrupt_handlers.hpp"
#include "InterruptDescriptorTable.hpp"
#include "ioport.hpp"

//void(*interrupt_handlers[IDT_SIZE])(IDTEntry*);

#define LSHIFT 14
#define RSHIFT 15

bool KEY_PRESSED[256];
char KEYBOARD_BUFFER = 0;

const char scancode_to_ascii[] = {
    0,  27, '&', '~', '\"', '\'', '(', '-',
    '`','_','^','@',')','=', '\b', '\t',
    'a','z','e','r','t','y','u','i',
    'o','p','^','$','\n', 0, 'q','s',
    'd','f','g','h','j','k','l','m',
   '%','\n', LSHIFT, '*','w','x','c','v',
    'b','n',',',';',':','!', RSHIFT,  '*',
    0,  ' '
};

const char shifted_scancode_to_ascii[] = {
    0, 27, '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', ')', '+', '\b', '\t',
    'A','Z','E','R','T','Y','U','I',
    'O','P','^','$','\n', 0, 'Q','S',
    'D','F','G','H','J','K','L','M',
   '%','\n', 0, '*','W','X','C','V',
    'B','N','?','.','/','!', 0,  '*',
    0,  ' '
};


extern "C" __attribute__ ((interrupt)) void isr_test(IDTEntry* entry) {
    //__asm__ volatile ("cli");
    print_string("interruption handled");

    return;
}


extern "C" __attribute__((interrupt)) void keyboard_handler(IDTEntry* entry) {

    uint8_t scancode = inb(0x60);

    bool pressed = !(scancode & 128);
    scancode &= 127; 

    uint8_t ascii = scancode_to_ascii[scancode];

    if (pressed) {
        KEY_PRESSED[ascii] = true;
        KEYBOARD_BUFFER = (KEY_PRESSED[LSHIFT] || KEY_PRESSED[RSHIFT]) ? shifted_scancode_to_ascii[scancode] : ascii;
    } else {
        KEY_PRESSED[ascii] = false;
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