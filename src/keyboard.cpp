#include "keyboard.hpp"
#include "types.hpp"
#include "interrupt_handlers.hpp"
#include "utils.hpp"


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


bool scan_keyboard(char* buff, int buff_size) {
	
	int index=-1;
	do {
		index+=1;

		KEYBOARD_BUFFER = 0;
		while (KEYBOARD_BUFFER == 0);
		buff[index] = scancode_to_ascii[KEYBOARD_BUFFER];
		print_char(buff[index]);
	} while (buff[index] != '\n' && index<buff_size);

	buff[index] = '\0';
	return index<buff_size;
}