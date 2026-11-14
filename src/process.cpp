#include <types.hpp>
#include <process.hpp>



Process::Process(func_process entry_point, uint16_t pid) :
	pid(pid), stack_size(4096) { // static stack size for now
	
	// initialize stack (with zeros)
	uint32_t stack_top = (uint32_t)(this->stack) + 4096; // 4096 -> size of the stack hardly defined in Process
	stack_top -= sizeof(StackFrame);
	StackFrame* stackFrame = (StackFrame*)stack_top;

	stackFrame->edi = 0;
	stackFrame->esi = 0;
	stackFrame->ebp = 0;
	stackFrame->esp_dummy = 0;
	stackFrame->ebx = 0;
	stackFrame->edx = 0;
	stackFrame->ecx = 0;
	stackFrame->eax = 0;

	stackFrame->eflags = 0x202;

	stackFrame->eip = (uint32_t)entry_point;

	this->esp = stack_top;

	this->state = READY;

	debug_string("\n\nFIRST SET stack_size : ");
	debug_int(this->stack_size);
	debug_string("\n\n");
}


Process::~Process() {}


Process& Process::copy(const Process& other) {
	if (this == &other)
		return *this;


	uint32_t stack_offset = other.esp - (uint32_t)other.stack;

	*this = other;

	this->esp = (uint32_t)this->stack + stack_offset;

	return *this;
}


ProcessusManager* ProcessusManager::_instance_address = NULL;


ProcessusManager& ProcessusManager::Instance() {
	if (_instance_address != NULL) {
		return (ProcessusManager&) *_instance_address;
	}
	static ProcessusManager _instance = ProcessusManager();
	ProcessusManager::_instance_address = (ProcessusManager*) &_instance;

	return (ProcessusManager&) *_instance_address;
}



ProcessusManager::ProcessusManager() :
	process_table(NULL),
	memory_manager(MemoryManager::Instance()),
	last_pid(0) {}

void ProcessusManager::Schedule() {
	if (process_table == NULL)
		return;

	ProcessNode* current = process_table;

	
	if (current->process.state == READY) {
		current->process.state = RUNNING;
		switch_context(NULL, current->process.esp);
		return;
	}

	// Find another READY process (skip the current one).
	ProcessNode* next = current->next;
	while (next != current && next->process.state != READY)
		next = next->next;

	if (next == current) // no other ready process
		return;

	current->process.state = READY;
	next->process.state = RUNNING;
	process_table = next;
	switch_context(&(current->process.esp), next->process.esp);
}

uint32_t ProcessusManager::AddProcess(func_process p) {

	ProcessNode* process_node_addr = (ProcessNode*) this->memory_manager.malloc();

	if (process_table == NULL) {
		process_node_addr->prev = process_node_addr;
		process_node_addr->next = process_node_addr;
		process_table = process_node_addr;
	} else {
		process_node_addr->prev = process_table->prev;
		process_node_addr->next = process_table;

		process_table->prev->next = process_node_addr;
		process_table->prev = process_node_addr;
	}


	process_node_addr->process.copy(Process(p, last_pid++));

	process_node_addr->process.state = READY;

	return process_node_addr->process.pid;

}