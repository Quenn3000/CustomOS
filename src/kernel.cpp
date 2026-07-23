#include <types.hpp>
#include <interrupt_descriptor_table.hpp>
#include <math.hpp>
#include <utils.hpp>
#include <strings.hpp>
#include <keyboard.hpp>
#include <PCIController.hpp>
#include <memory_management.hpp>

#ifndef TEST

extern "C" void main() {
    debug_string("Lancement du kernel\n\n");
    set_cursor();

    char title[] = "           _____                        _        ____   _____\n          / ____|                      (_)      / __ \\ / ____|\n         | (___   __ _ _ __   __ _ _ __ _  __ _| |  | | (___\n          \\___ \\ / _` | '_ \\ / _` | '__| |/ _` | |  | |\\___ \\\n          ____) | (_| | | | | (_| | |  | | (_| | |__| |____) |\n         |_____/ \\__,_|_| |_|\\__, |_|  |_|\\__,_|\\____/|_____/\n                              __/ |\n                             |___/\n";


    print_string("Loading IDT Kernel\n");
    
    InterruptManager itrManager = InterruptManager();
    itrManager.init();

    KeyboardDriver& keyboardDriver = KeyboardDriver::Instance(&itrManager);
    
    MemoryManager* memory_manager = MemoryManager::Instance();

    print_clearall();
    print_string((char*)&title);

    PeriphericalComponnentInterconnectController PCIController;

    PCIController.debug_devices();


    // memory manager
    MemoryMapEntry* test_block = memory_manager->get_block(0);

    printf("Block (address 0x%x): {%ud, %ud, %ud, %ud}\n", 5,
        (int)&test_block,
        &test_block->base_address,
        &test_block->length,
        &test_block->type,
        &test_block->attributes);
    test_block = memory_manager->get_block(1);

    printf("Block (address 0x%x): {%ud, %ud, %ud, %ud}\n", 5,
        (int)&test_block,
        &test_block->base_address,
        &test_block->length,
        &test_block->type,
        &test_block->attributes);
    int nb_block = memory_manager->get_block_number();
    printf("Block number : %d\n", 1, &nb_block);


    char buffer[128];
    while (1) {
        print_string(">> ");
        keyboardDriver.scan_keyboard(buffer, 128, true);
        
        if (strcmp(buffer, "exit") == 0) {
            print_clearall();
            return;
        }

        print_string(buffer);
        print_string("\n");
    }
}

#else

#include <tests.hpp>
extern "C" void main() {
    debug_string("DEBUG MOD");
    set_cursor();

    print_string("DEBUG MOD");

    InterruptManager itrManager = InterruptManager();
    itrManager.init();


    tests();

    while(1);
}

#endif