#include <utils.hpp>
#include <types.hpp>
#include <math.hpp>
#include <ioport.hpp>
#include <strings.hpp>

uint16_t* cursor;


// set the global variable cursor
bool set_cursor(int n, bool relative) {
    if (relative) {
        if ((int)cursor + n >= TEXT_SCREEN_START_ADDRESS && (int)cursor + n <= TEXT_SCREEN_START_ADDRESS+get_screen_width()*get_screen_height()) {
            cursor+=n;
            return true;
        }
        return false;
    } else {
        cursor = (uint16_t*) TEXT_SCREEN_START_ADDRESS + n;
        return true;
    }
}

// quit simple
uint16_t get_screen_width() {
    return *(uint16_t*)0x044A;  // Read the number of columns
}

// same
uint8_t get_screen_height() {
    return *(uint8_t*)0x0484 + 1;   // Read the number of rows
}

// clear all the screen values
void print_clearall() {
    int nb = get_screen_width()*get_screen_height();
    set_cursor();
    for (int i=0; i<nb; i++) {
        *cursor++ = 0x0f00 | ' ';
    }
    set_cursor();
    return;
}

// set the char c, n cases after the current position (if in relative mod) or at the n-th case (if not in relative mod)
bool set_char(int n, char c, bool relative) {
    if (relative) {

        if ((int)cursor + n >= TEXT_SCREEN_START_ADDRESS && (int)cursor + n <= TEXT_SCREEN_START_ADDRESS+get_screen_width()*get_screen_height()) {
            *(cursor+n) = 0x0f00 | c;
            return true;
        }

        return false;

    } else {

        if (n >= 0 && n < get_screen_width()*get_screen_height()) {
            *((uint16_t*)TEXT_SCREEN_START_ADDRESS+n) = 0x0f00 | c;
            return true;
        }
        
        return false;
    }
}

// print a char on the current cursor position
void print_char(char c) {
    uint16_t screen_width = get_screen_width()*2;
    if (c == '\n') {
        int current_line_nb = ((int)cursor - TEXT_SCREEN_START_ADDRESS) / screen_width;
        cursor = (uint16_t*) TEXT_SCREEN_START_ADDRESS + (current_line_nb+1)*(screen_width/2); // pk /2 ?? -> prck c'est 2 bytes de long une case
    } else {
        cursor[0] = 0x0f00 | c;
        cursor+=1;
    }
}


// print a string on the current cursor position
void print_string(const char* str) {
    uint16_t screen_width = get_screen_width()*2;
    int screen_size = screen_width*get_screen_height();
    for (int i=0; str[i] != '\0' && (int) cursor < TEXT_SCREEN_START_ADDRESS+screen_size; i++) {
        if (str[i] == '\n') {
            int current_line_nb = ((int)cursor - TEXT_SCREEN_START_ADDRESS) / screen_width;
            cursor = (uint16_t*) TEXT_SCREEN_START_ADDRESS + (current_line_nb+1)*(screen_width/2); // pk /2 ?? -> prck c'est 2 bytes de long une case
        } else {
            cursor[0] = 0x0f00 | str[i];
            cursor+=1;
        }
    }

}


// print an int on the current cursor position
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

    int screen_size = get_screen_width()*2*get_screen_height();

    x = sav_x;

    while (power_base >= 0 && (int) cursor < TEXT_SCREEN_START_ADDRESS+screen_size) {
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

// --- DEBUG ---

void debug_string(const char* str) {
    Port8Bit debug_out(OUTPUT_SERIAL_PORT); // port serial de sortie pour qemu

    for (int i=0; str[i] != '\0'; i++) {
        debug_out.write(str[i]);
    }

}

void debug_char(const char c) {
    Port8Bit debug_out(OUTPUT_SERIAL_PORT); // port serial de sortie pour qemu

    debug_out.write(c);
}


// debug an int on the current cursor position
bool debug_int(int x, int base) {
    if (base < 2 || base > 32) {
        return false;
    }

    if (x < 0) {
        char n[] = "-";
        debug_string(n);
        x = -x;
    }

    int sav_x = x;
    int power_base = 0;

    while (x >= base) {
        x = x / base;
        power_base++;
    }

    x = sav_x;

    Port8Bit debug_out(OUTPUT_SERIAL_PORT);

    while (power_base >= 0) {
        int to_print = x/power_int(base, power_base);
        if (to_print <= 9) {
            debug_out.write((char) (48 + to_print));
        } else {
            debug_out.write((char) (55 + to_print));
        }
        x-= (x/power_int(base, power_base)) * power_int(base, power_base);

        power_base-=1;
    }

    return true;
}