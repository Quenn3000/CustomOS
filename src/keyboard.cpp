#include "keyboard.hpp"
#include "types.hpp"
#include "interrupt_handlers.hpp"


void init_keyboard() {
	for (int i=0; i<SCANCODE_NUMBER; i++) {
		SCANCODE_PRESSED[i] = false;
	}
}


const char scancode_to_ascii[] = {
    0,  27, '1', '2', '3', '4', '5', '6',
    '7','8','9','0',')','=', '\b', '\t',
    'a','z','e','r','t','y','u','i',
    'o','p','^','$','\n', 0, 'q','s',
    'd','f','g','h','j','k','l','m',
   '%','\n', 14,  '*','w','x','c','v',
    'b','n',',',';',':','!', 15,  '*',
    0,  ' '
};

char getch() {
    if (KEYBOARD_BUFFER != 0) {
		char c = KEYBOARD_BUFFER;
		KEYBOARD_BUFFER = 0;
		return scancode_to_ascii[c];
	}

    return 0;
}