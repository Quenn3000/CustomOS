#include <process.hpp>
#include <utils.hpp>
#include <memory_management.hpp>
#include <time.hpp>

void processB(Process* own_addr);
void processA(Process* own_addr);

void processStarter(Process own_addr) {
	ProcessusManager& process_manager = ProcessusManager::Instance();
	process_manager.AddProcess(processA);
	process_manager.AddProcess(processB);
	while(1);
}

void processA(Process* own_addr) {

	while(1) {

		debug_string("AAAAA\n");
	}
	return;
}

void processB(Process* own_addr) {

	while(1) {
		debug_string("BBBBB\n");
	}
	return;
}

void test_process(InterruptManager* itr_manager) {
	MemoryManager& memory_manager = MemoryManager::Instance();

	void* m1 = memory_manager.malloc();
	debug_string("\n\nPointeur m1 : 0x");
	debug_int((int64_t)m1, 16);
	debug_string("\n");

	void* m2 = memory_manager.malloc();
	debug_string("Pointeur m2 : 0x");
	debug_int((int64_t)m2, 16);
	debug_string("\n");

	//memory_manager->free(m1);

	void* m3 = memory_manager.malloc();
	debug_string("Pointeur m3 : 0x");
	debug_int((int64_t)m3, 16);
	debug_string("\n");

	memory_manager.free(m2);

	void* m4 = memory_manager.malloc();
	debug_string("Pointeur m4 : 0x");
	debug_int((int64_t)m4, 16);
	debug_string("\n");

	TimeManager time_manager = TimeManager(itr_manager);
	ProcessusManager& process_manager = ProcessusManager::Instance();

	time_manager.set_processManager(&process_manager);

	process_manager.AddProcess(processStarter);
	return;
}