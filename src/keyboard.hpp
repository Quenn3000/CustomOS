#ifndef __KEYBOARD_H_
#define __KEYBOARD_H_

#include "interrupt_handlers.hpp"

char getch(); // return the first char pressed after the call of the function

void init_keyboard();

bool scan_keyboard(char* buff, int buff_size); // fill the buffer with the string wrote on screen before a '\n'

#endif