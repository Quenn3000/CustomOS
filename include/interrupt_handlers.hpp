#pragma once

#include <interrupt_descriptor_table.hpp>

// scancode numbers I am considering on the keyboard

// array with value representing if a specific key is pressed
extern bool KEY_PRESSED[256]; // ascii keys
extern char KEYBOARD_BUFFER; // last key pressed on the keyboard

extern "C" __attribute__((interrupt)) void keyboard_handler(uint64_t* entry);
extern "C" __attribute__((interrupt)) void default_handler(uint64_t* entry);