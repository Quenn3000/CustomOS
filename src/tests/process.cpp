#include <process.hpp>
#include <time.hpp>

#include "test_suite.hpp"

// --- demo processes -------------------------------------------------------

static void dummy_entry(Process* own_addr) {
    (void)own_addr;
}

static void log_process(const char* name, const char* message, Process* own_addr) {
    debug_string("[demo] ");
    debug_string(name);
    debug_string(": ");
    debug_string(message);
    debug_string(" (pid=");
    debug_int(own_addr->get_pid());
    debug_string(")\n");
}

void processA(Process* own_addr);
void processB(Process* own_addr);
void processC(Process* own_addr);
void processD(Process* own_addr);

void processStarter(Process* own_addr) {
    ProcessusManager& manager = ProcessusManager::Instance();

    log_process("Starter", "started", own_addr);
    manager.AddProcess(processA);
    manager.AddProcess(processB);
    manager.AddProcess(processC);

    for (;;) {
        for (volatile unsigned int i = 0; i < 1000000; i++) { }
        log_process("Starter", "alive", own_addr);
    }
}

void processA(Process* own_addr) {
    ProcessusManager& manager = ProcessusManager::Instance();

    log_process("A", "started", own_addr);
    unsigned int iterations = 0;
    bool added_d = false;

    for (;;) {
        // Regression check: adding a process from a real process stack used
        // to overflow that stack when AddProcess built a 4 KiB temporary.
        if (!added_d) {
            if (++iterations == 100000) {
                manager.AddProcess(processD);
                debug_string("[demo] A added process D\n");
                added_d = true;
            }
            continue;
        }
        for (volatile unsigned int i = 0; i < 1000000; i++) { }
        log_process("A", "alive", own_addr);
    }
}

void processB(Process* own_addr) {
    log_process("B", "started", own_addr);
    for (;;) {
        for (volatile unsigned int i = 0; i < 1000000; i++) { }
        log_process("B", "alive", own_addr);
    }
}

void processC(Process* own_addr) {
    log_process("C", "started", own_addr);
    for (;;) {
        for (volatile unsigned int i = 0; i < 1000000; i++) { }
        log_process("C", "alive", own_addr);
    }
}

void processD(Process* own_addr) {
    log_process("D", "started", own_addr);
    for (;;) {
        for (volatile unsigned int i = 0; i < 1000000; i++) { }
        log_process("D", "alive", own_addr);
    }
}

// --- unit tests (they all return) ----------------------------------------

void test_process(InterruptManager* itr_manager) {
    (void)itr_manager;

    test_begin("process/layout");
    CHECK_INT(sizeof(StackFrame), 40);
    CHECK_INT(sizeof(Process), 4112);
    CHECK_INT(sizeof(ProcessNode), 4120);
    test_end();

    test_begin("process/init");
    Process first;
    Process second;

    first.init(dummy_entry, 11);
    second.init(dummy_entry, 22);

    CHECK_INT(first.get_pid(), 11);
    CHECK_INT(second.get_pid(), 22);

    first.init(dummy_entry, 33);
    CHECK_INT(first.get_pid(), 33);
    test_end();
}

// --- integration demo (never returns) ------------------------------------

void run_process_demo(InterruptManager* itr_manager) {
    TimeManager time_manager(itr_manager);
    ProcessusManager& manager = ProcessusManager::Instance();

    time_manager.set_processManager(&manager);
    manager.AddProcess(processStarter);

    debug_string("[demo] timer started, switching to the first process\n");
    for (;;) { }
}
