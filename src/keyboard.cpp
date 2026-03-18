#include <keyboard.hpp>
#include <types.hpp>
#include <interrupt_handlers.hpp>
#include <interrupt_descriptor_table.hpp>
#include <utils.hpp>
#include <ioport.hpp>

KeyboardDriver* KeyboardDriver::instance = nullptr;

// init all the keyboard related variables
KeyboardDriver::KeyboardDriver(InterruptManager* itrManager) {

	itrManager->setInterrupt(0x21, (uint32_t)KeyboardDriver::interrupt_handler);

	KeyboardDriver::instance = this;
	for (int i=0; i<256; i++) {
		this->KEY_PRESSED[i] = false;
	}

	this->KEYBOARD_BUFFER = 0;

}

// get a character from the keyboard
char KeyboardDriver::getch() {
    if (this->KEYBOARD_BUFFER != 0) {
		char c = this->KEYBOARD_BUFFER;
		this->KEYBOARD_BUFFER = 0;
		return c;
	}

    return 0;
}

// get all keys from the keyboard into the buffer, until an '\n' occur
bool KeyboardDriver::scan_keyboard(char* buff, int buff_size, bool graphic) {
	
	int index=-1;
	do {
		index+=1;

		this->KEYBOARD_BUFFER = 0;
		while (this->KEYBOARD_BUFFER == 0 || (this->KEYBOARD_BUFFER == '\b' && index<=0));

		if (this->KEYBOARD_BUFFER == '\b') {
			buff[index-1] = '\0';
			index -=2;
			if (graphic) {
				set_char(-1, ' ', true);
				set_cursor(-1, true);
			}
		} else {
			buff[index] = this->KEYBOARD_BUFFER;
			
			if (graphic)
				print_char(buff[index]);
		}
	} while (buff[index] != '\n' && index<buff_size);

	buff[index] = '\0';
	return index<buff_size;
}
