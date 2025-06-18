#ifndef __UTILS_H_
#define __UTILS_H_

#include "types.hpp"

void set_cursor();
uint16_t get_screen_width();
uint8_t get_screen_height();
void print_clearall();
void print_string(char const* str);
bool print_int(int x, int base=10);

#endif