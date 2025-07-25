#include "keyboard.hpp"
#include "types.hpp"
#include "interrupt_handlers.hpp"
#include "utils.hpp"


void init_keyboard() {
	for (int i=0; i<256; i++) {
		KEY_PRESSED[i] = false;
	}
}

char getch() {
    if (KEYBOARD_BUFFER != 0) {
		char c = KEYBOARD_BUFFER;
		KEYBOARD_BUFFER = 0;
		return c;
	}

    return 0;
}


bool scan_keyboard(char* buff, int buff_size, bool graphic) {
	
	int index=-1;
	do {
		index+=1;

		KEYBOARD_BUFFER = 0;
		while (KEYBOARD_BUFFER == 0 || (KEYBOARD_BUFFER == '\b' && index<=0));

		if (KEYBOARD_BUFFER == '\b') {
			buff[index-1] = '\0';
			index -=2;
			if (graphic) {
				set_char(-1, ' ', true);
				set_cursor(-1, true);
			}
		} else {
			buff[index] = KEYBOARD_BUFFER;
			
			if (graphic)
				print_char(buff[index]);
		}
	} while (buff[index] != '\n' && index<buff_size);

	buff[index] = '\0';
	return index<buff_size;
}
