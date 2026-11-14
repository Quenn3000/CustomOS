#pragma once

#include "memory_management.hpp"
#include <interrupt_descriptor_table.hpp>
#include <types.hpp>

// --- IMPLEMENTED CLASSES ---
class MemoryManager;
class Process;
class ProcessusManager;

// --- IMPLEMENTED TYPES ---
typedef void(*func_process)(Process* own_addr);

// --- ASM FUNCTIONS ---
extern "C" void switch_context(uint32_t* current_esp_ptr, uint32_t next_esp); // check switch_context.asm

// --- STACKFRAME ---
// !! Généré par IA
struct StackFrame {
    // Les dépilés par popad (dans l'ordre inverse du pushad)
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp_dummy; // Ignoré par popad, mais prend de la place
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    
    // Le registre dépilé par popfd
    uint32_t eflags;
    
    // L'adresse de retour dépilée par ret
    uint32_t eip;
} __attribute__((packed));


// --- PROCESS ---
enum ProcessState {READY, RUNNING, BLOCKED, TERMINATED};

class Process {
	public:
		uint16_t pid;           // Identifiant unique du processus
	    ProcessState state;     // Etat actuel

		Process(func_process entry_point, uint16_t pid);
		~Process();

        Process& Process::copy(const Process& other);

        uint32_t* get_esp_addr();
        uint32_t get_esp();

	private:
		uint32_t stack_size;
		uint32_t esp;                 // ESP sauvegardé
		uint8_t stack[4096];          // Pile Kernel du processus

};

struct ProcessNode {
    Process process;
    ProcessNode* next;
    ProcessNode* prev;
};


class ProcessusManager {
    public:

        static ProcessusManager& Instance();
    
        uint32_t AddProcess(func_process p); // return pid
        void Schedule();

        MemoryManager& memory_manager;
    
    protected:
        ProcessusManager();
        
    private:

        static ProcessusManager* _instance_address;
    
        ProcessNode* process_table;
        uint16_t last_pid;
        
};