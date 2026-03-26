#pragma once

#include <interrupt_handlers.hpp>
#include "interrupt_descriptor_table.hpp"
#include <ioport.hpp>
#include <utils.hpp>

#define LSHIFT 14
#define RSHIFT 15

class KeyboardDriver {

	public:
		KeyboardDriver(InterruptManager* itrManager);
		char getch();
		bool scan_keyboard(char* buff, int buff_size, bool graphic);

		const char scancode_to_ascii[58] = {
			0,  27, '&', '~', '\"', '\'', '(', '-',
			'`','_','^','@',')','=', '\b', '\t',
			'a','z','e','r','t','y','u','i',
			'o','p','^','$','\n', 0, 'q','s',
			'd','f','g','h','j','k','l','m',
		   '%','\n', LSHIFT, '*','w','x','c','v',
			'b','n',',',';',':','!', RSHIFT,  '*',
			0,  ' '
		};
		
		const char shifted_scancode_to_ascii[58] = {
			0, 27, '1', '2', '3', '4', '5', '6',
			'7', '8', '9', '0', ')', '+', '\b', '\t',
			'A','Z','E','R','T','Y','U','I',
			'O','P','^','$','\n', 0, 'Q','S',
			'D','F','G','H','J','K','L','M',
		   '%','\n', 0, '*','W','X','C','V',
			'B','N','?','.','/','!', 0,  '*',
			0,  ' '
		};

	private:
		static KeyboardDriver* instance;
		bool KEY_PRESSED[256];
		char KEYBOARD_BUFFER;

		static __attribute__((interrupt)) void interrupt_handler(IDTEntry* entry) {
			Port8Bit master_port(PIC_MASTER_COMMAND_PORT);
			Port8Bit input_port(0x60);

			uint8_t scancode = input_port.read();

		    bool pressed = !(scancode & 128);
		    scancode &= 127;

		    uint8_t ascii = instance->scancode_to_ascii[scancode];

		    if (pressed) {
		        instance->KEY_PRESSED[ascii] = true;
		        instance->KEYBOARD_BUFFER = (instance->KEY_PRESSED[LSHIFT] || instance->KEY_PRESSED[RSHIFT]) ? instance->shifted_scancode_to_ascii[scancode] : ascii;
		    } else {
		        instance->KEY_PRESSED[ascii] = false;
			}
		    master_port.write(0x20);

		    return;
		}
	
};