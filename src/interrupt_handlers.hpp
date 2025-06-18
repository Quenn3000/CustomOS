#ifndef __INTERRUPT_HANDLERS_H_
#define __INTERRUPT_HANDLERS_H_

#include "InterruptDescriptorTable.hpp"

#define SCANCODE_NUMBER 58
#define KEYBOARD_BUFFER_SIZE 128

extern bool SCANCODE_PRESSED[SCANCODE_NUMBER];
extern char KEYBOARD_BUFFER;

void set_idt_gates();

#endif