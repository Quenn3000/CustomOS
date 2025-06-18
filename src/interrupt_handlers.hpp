#ifndef __INTERRUPT_HANDLERS_H_
#define __INTERRUPT_HANDLERS_H_

#include "InterruptDescriptorTable.hpp"

//void(*)(IDTEntry* entry) * get_interrupt_handlers()
void set_idt_gates();

#endif