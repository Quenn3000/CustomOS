#include "types.hpp"
#include "InterruptDescriptorTable.hpp"
#include "math.hpp"
#include "utils.hpp"
#include "strings.hpp"
#include "keyboard.hpp"
#include "heap.hpp"



extern "C" void main() {
    set_cursor();

    char title[] = "           _____                        _        ____   _____\n          / ____|                      (_)      / __ \\ / ____|\n         | (___   __ _ _ __   __ _ _ __ _  __ _| |  | | (___\n          \\___ \\ / _` | '_ \\ / _` | '__| |/ _` | |  | |\\___ \\\n          ____) | (_| | | | | (_| | |  | | (_| | |__| |____) |\n         |_____/ \\__,_|_| |_|\\__, |_|  |_|\\__,_|\\____/|_____/\n                              __/ |\n                             |___/\n";


    print_string("Loading IDT Kernel\n");
    init_idt();
    init_heap();

    __asm__ volatile ("int $0x80"); // Manually trigger interrupt
    __asm__ volatile ("int $0x21");

    print_clearall();
    print_string((char*)&title);


    int t = 1;
    int a = 2;
    char b = 'p';
    in_format_factor("%d %d %c", "1125 34 k", 3, &t, &a, &b);

    //print_string("How old are you ? ");
    

    
    char buffer[128];
    while (1) {
        print_string(">> ");
        scan_keyboard(buffer, 128);
        print_string(buffer);
        print_string("\n");
    }
}