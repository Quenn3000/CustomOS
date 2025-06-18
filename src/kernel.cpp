#include "types.hpp"
#include "InterruptDescriptorTable.hpp"
#include "math.hpp"
#include "utils.hpp"
#include "strings.hpp"
#include "keyboard.hpp"



extern "C" void main() {
    set_cursor();

    char title[] = "           _____                        _        ____   _____\n          / ____|                      (_)      / __ \\ / ____|\n         | (___   __ _ _ __   __ _ _ __ _  __ _| |  | | (___\n          \\___ \\ / _` | '_ \\ / _` | '__| |/ _` | |  | |\\___ \\\n          ____) | (_| | | | | (_| | |  | | (_| | |__| |____) |\n         |_____/ \\__,_|_| |_|\\__, |_|  |_|\\__,_|\\____/|_____/\n                              __/ |\n                             |___/\n";


    print_string("Loading IDT Kernel\n");
    init_idt();

    __asm__ volatile ("int $0x80"); // Manually trigger interrupt
    __asm__ volatile ("int $0x21");

    print_clearall();
    print_string((char*)&title);
    

    
    while (1) {
        char c[] = {getch(), '\0'};
        print_string((char*)&c);
    }
}