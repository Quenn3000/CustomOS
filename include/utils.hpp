#pragma once

#include <types.hpp>

#define TEXT_SCREEN_START_ADDRESS 0xb8000
#define OUTPUT_SERIAL_PORT 0x3F8

bool set_cursor(int n=0, bool relative=false); // set cursor to its initial value
uint16_t get_screen_width();
uint8_t get_screen_height();
void print_clearall(); // clear the textual screen

bool set_char(int n, char c, bool relative=false);
void print_char(char c);
void print_string(char const* str);
bool print_int(int x, int base=10);

// --- DEBUGGING ---
void debug_string(char const* str);
bool debug_int(int x, int base=10);