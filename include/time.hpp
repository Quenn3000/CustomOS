#pragma once

#include <utils.hpp>
#include <types.hpp>
#include <interrupt_descriptor_table.hpp>
#include <process.hpp>
#include <ioport.hpp>

#define PIT_FREQUENCY 1193182
#define TIMER_INTERRUPT_FREQUENCY 100 // Hz

class TimeManager {
	public:
		TimeManager(InterruptManager* itrManager);
		bool set_processManager(ProcessusManager* process_manager);
	
	private:
		ProcessusManager* process_manager;

		static TimeManager* _instance_address;
		void init_timer(uint32_t frequency);

		InterruptManager* itrManager;
		static __attribute__((interrupt)) void PIT_handler(uint64_t* entry) { // PIT : Programmable Interval Timer
			outb(PIC_MASTER_COMMAND_PORT, 0x20); // dès que Schedule va s'exec, on va basculer sur le nouveau process. On doit donc acquitter le PIC avant
			
			if (_instance_address != nullptr) {
				if (_instance_address->process_manager != NULL) {
					_instance_address->process_manager->Schedule();
				}
			}
			return;
		}

};