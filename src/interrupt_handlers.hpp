#ifndef __INTERRUPT_HANDLERS_H_
#define __INTERRUPT_HANDLERS_H_

#include "InterruptDescriptorTable.hpp"

// scancode numbers I am considering on the keyboard
#define SCANCODE_NUMBER 58

// array with value representing if a specific key is pressed
extern bool SCANCODE_PRESSED[SCANCODE_NUMBER];
extern char KEYBOARD_BUFFER; // last key pressed on the keyboard

void set_idt_gates();

#endif