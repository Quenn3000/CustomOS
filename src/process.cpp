#include <types.hpp>
#include <process.hpp>


Process::~Process() {}


/*
Argument : Process other
Side effect : init all the content of a Process. Change the esp and the argument value
*/
void Process::init(func_process entry_point, uint16_t pid) {
	this->pid = pid;
	this->stack_size = 4096;
		
	// initialize stack (with zeros)
	uint32_t stack_top = (uint32_t)(this->stack) + this->stack_size;
	stack_top -= sizeof(StackFrame) + sizeof(uint32_t) + sizeof(Process*); // return address slot + arguments of processes
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

	Process** own_address = stack_top + sizeof(StackFrame) + sizeof(uint32_t);
	*own_address = this;

	return;
}

/*
Return value : return the PID of the current Process
*/
uint16_t Process::get_pid() {
	return this->pid;
}

/*
Side effect : terminate the current Process, switch to another one
Return value : if the Process terminate correctly
*/
bool Process::terminate() {
	this->state = ProcessState::TERMINATED;
	ProcessusManager& process_manager = ProcessusManager::Instance();
	process_manager.Schedule();
	return false;
}


ProcessusManager* ProcessusManager::_instance_address = NULL;

/*
Get the current ProcessusManager instance, create one if it does not exist
*/
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

/*
Side effect : switch context to the next ready or running Process
*/
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
	while (next != current && next->process.state != READY) {
		if (next->process.state == ProcessState::TERMINATED) {
			MemoryManager& m = MemoryManager::Instance();
			m.free(next);
		}
		next = next->next;
	}

	if (next == current) // no other ready process
		return;

	current->process.state = READY;
	next->process.state = RUNNING;
	process_table = next;
	switch_context(&(current->process.esp), next->process.esp);
}

/*
Argument : new Process' function
Side effect : create a new Process with the given function into the Processus Manager
Return value : PID of the Process created
*/
uint32_t ProcessusManager::AddProcess(func_process f_p) {

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


	//process_node_addr->process.copy(Process(f_p, last_pid++));

	process_node_addr->process.init(f_p, last_pid++);

	process_node_addr->process.state = READY;


	return process_node_addr->process.pid;

}