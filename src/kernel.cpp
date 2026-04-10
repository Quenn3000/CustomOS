#include <types.hpp>
#include <interrupt_descriptor_table.hpp>
#include <math.hpp>
#include <utils.hpp>
#include <strings.hpp>
#include <keyboard.hpp>
#include <PCIController.hpp>
#include <memory_management.hpp>


extern "C" void main() {
    debug_string("Lancement du kernel\n\n");
    set_cursor();

    char title[] = "           _____                        _        ____   _____\n          / ____|                      (_)      / __ \\ / ____|\n         | (___   __ _ _ __   __ _ _ __ _  __ _| |  | | (___\n          \\___ \\ / _` | '_ \\ / _` | '__| |/ _` | |  | |\\___ \\\n          ____) | (_| | | | | (_| | |  | | (_| | |__| |____) |\n         |_____/ \\__,_|_| |_|\\__, |_|  |_|\\__,_|\\____/|_____/\n                              __/ |\n                             |___/\n";


    print_string("Loading IDT Kernel\n");
    
    InterruptManager itrManager = InterruptManager();
    itrManager.init();

    KeyboardDriver keyboardDriver = KeyboardDriver(&itrManager);
    
    MemoryManager* memory_manager = MemoryManager::Instance();

    print_clearall();
    print_string((char*)&title);

    //uint16_t* caca = "Hello World!";

    PeriphericalComponnentInterconnectController PCIController;

    MemoryMapEntry* test_block = memory_manager->get_block(0);


    PCIController.print_devices();

    unsigned int x = 12;
    char buffer_test[1000];
    char c = 'x';
    if (out_format_factor("test de x = %ud, c = %c\n", (char*)&buffer_test, 1000, 2, &x, &c)) {
        print_string("Test reussi\n");
    } else {
        print_string("Test rate\n");
    }
    print_string(buffer_test);


    char buffer[128];
    while (1) {
        print_string(">> ");
        keyboardDriver.scan_keyboard(buffer, 128, true);
        
        if (strcmp(buffer, "exit")) {
            print_clearall();
            return;
        }

        print_string(buffer);
        print_string("\n");
    }
}