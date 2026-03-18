#include <types.hpp>
#include <interrupt_descriptor_table.hpp>
#include <math.hpp>
#include <utils.hpp>
#include <strings.hpp>
#include <keyboard.hpp>
#include <heap.hpp>
#include <PCIController.hpp>



extern "C" void main() {
    set_cursor();

    char title[] = "           _____                        _        ____   _____\n          / ____|                      (_)      / __ \\ / ____|\n         | (___   __ _ _ __   __ _ _ __ _  __ _| |  | | (___\n          \\___ \\ / _` | '_ \\ / _` | '__| |/ _` | |  | |\\___ \\\n          ____) | (_| | | | | (_| | |  | | (_| | |__| |____) |\n         |_____/ \\__,_|_| |_|\\__, |_|  |_|\\__,_|\\____/|_____/\n                              __/ |\n                             |___/\n";


    print_string("Loading IDT Kernel\n");
    
    InterruptManager itrManager = InterruptManager();
    itrManager.init();

    KeyboardDriver keyboardDriver = KeyboardDriver(&itrManager);
    
    init_heap();

    print_clearall();
    print_string((char*)&title);

    PeriphericalComponnentInterconnectController PCIController;

    PCIController.print_devices();


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