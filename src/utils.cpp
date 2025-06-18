#include "utils.hpp"
#include "types.hpp"
#include "math.hpp"

uint16_t* cursor;

void set_cursor() {
    cursor = (uint16_t*) 0xb8000;
}

uint16_t get_screen_width() {
    return *(uint16_t*)0x044A;  // Read the number of columns
}

uint8_t get_screen_height() {
    return *(uint8_t*)0x0484 + 1;   // Read the number of rows
}


void print_clearall() {
    int nb = get_screen_width()*get_screen_height();
    set_cursor();
    for (int i=0; i<nb; i++) {
        *cursor++ = 0x0f00 | 32; // blank space;
    }
    set_cursor();
    return;
}

void print_string(char const* str) {
    for (int i=0; str[i] != '\0'; i++) {
        if (str[i] == '\n') {
            uint16_t screen_width = get_screen_width();
            int current_line_nb = ((int)cursor - 0xb8000) / (screen_width*2); // pk *2 ?? -> prck c'est 2 bytes de long une case
            cursor = (uint16_t*) 0xb8000 + (current_line_nb+1)*screen_width;
        } else {
            cursor[0] = 0x0f00 | str[i];
            cursor+=1;
        }
    }

}

bool print_int(int x, int base) {
    if (base < 2 || base > 32) {
        return false;
    }

    if (x < 0) {
        char n[] = "-";
        print_string(n);
        x = -x;
    }

    int sav_x = x;
    int power_base = 0;

    while (x >= base) {
        x = x / base;
        power_base++;
    }

    char c[] = "printed int";

    x = sav_x;

    while (power_base >= 0) {
        int to_print = x/power_int(base, power_base);
        if (to_print <= 9) {
            cursor[0] = 0x0f00 | 48 + (to_print);
        } else {
            cursor[0] = 0x0f00 | 55 + (to_print);
        }
        x-= (x/power_int(base, power_base)) * power_int(base, power_base);

        cursor+=1;
        power_base-=1;
    }

    return true;
}