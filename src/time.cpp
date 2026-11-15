#include <types.hpp>
#include <interrupt_descriptor_table.hpp>
#include <time.hpp>
#include <ioport.hpp>

TimeManager* TimeManager::_instance_address = NULL;

TimeManager::TimeManager(InterruptManager* itr) : itrManager(itr) {

	this->_instance_address = this;
	this->process_manager = NULL;
	init_timer(TIMER_INTERRUPT_FREQUENCY);

	asm volatile("cli"); // on fait de la chirurgie ici (surtout pour éviter que le PIT se réveille pendant l'opération)
	itrManager->reset_interrupt(0x20);
	itrManager->set_interrupt(0x20, (uint32_t)TimeManager::PIT_handler);
	asm volatile("sti");

}

bool TimeManager::set_processManager(ProcessusManager* p) {
	if (p == NULL)
		return false;
	_instance_address->process_manager = p;
	return true;
}


void TimeManager::init_timer(uint32_t frequency) {
    uint32_t divisor = PIT_FREQUENCY / frequency;

    // set le PIT en Mode 3 (ondes carrées)
    outb(0x43, 0x36);

    // envoyer le diviseur (octet faible puis octet fort)
    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));

}
