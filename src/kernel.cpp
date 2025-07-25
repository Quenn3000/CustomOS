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


    char s[6];
    itoa(123, 6, s, 10);
    //print_int(&s);

    //print_string(s);

    //print_string("How old are you ? ");
    /*int x = 1;
    x = x+3;
    x = x+7;*/

    // quand j'ai trop de code, je crash
    char buffer[128];
    while (1) {
        print_string(">> ");
        scan_keyboard(buffer, 128, true);
        print_string(buffer);
        print_string("\n"); // crash ? plus de mémoire ou qque chose du style
    }
}