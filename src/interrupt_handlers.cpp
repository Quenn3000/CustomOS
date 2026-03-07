#include <interrupt_handlers.hpp>
#include <interrupt_descriptor_table.hpp>
#include <ioport.hpp>
#include <types.hpp>
#include <utils.hpp>


#define LSHIFT 14
#define RSHIFT 15

bool KEY_PRESSED[256]; // global variable actualized to know if a specific key is pressed (by ascii value)
char KEYBOARD_BUFFER = 0; // keyboard key buffer; the last pressed key

// correspondance for azerty classic french keyboard
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



// keyboard interrupts handler
/*extern "C" __attribute__((interrupt)) void keyboard_handler(uint64_t* entry) {

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
    
    this->master_port.write(0x20);

    return;
}*/

extern "C" __attribute__((interrupt)) void default_handler(uint64_t* entry) {
    // You can print something or halt
    Port8Bit master_port(PIC_MASTER_COMMAND_PORT);
    master_port.write(0x20);
}