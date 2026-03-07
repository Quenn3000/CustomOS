#include <strings.hpp>
#include <math.hpp>
#include <types.hpp>


// get the size of a string (before '\0')
int strlen(char* s) {
	int i;
	for (i=0; s[i] != '\0'; i++);
	return i;
}

// convert string, from base 2 to 16... NO, 36 !! LOL
int atoi(char* s, int base) {

	int res = 0;
	int nb_digit = strlen(s);
	bool neg = false;

	if (nb_digit > 0 && s[0] == '-') {
		neg = true;
		s+=1;
		nb_digit-=1;
	}

	for (int i=0; i<nb_digit; i++) {
		if (s[i] >= 48 && s[i] <= 57) { // 0-9
			res += (s[i]-48)*power_int(base, nb_digit-i-1);
		} else if (s[i] >= 97 && s[i] <=122) { // a-z
			res += (s[i]-87)*power_int(base, nb_digit-i-1);
		}
	}

	if (neg) {
		return -res;
	}

	return res;
}

// convert an int into a string in the buffer, in the base you want (return if the operation was a success or it failed)
bool itoa(int value, int buffer_size, char* buffer, int base) {
	if (base < 2 || base > 32) {
        return false;
    }

    if (value < 0) {
		if (buffer_size-- <= 0) {
			return false;
		}
        buffer[0] = '-';
		buffer+=1;
        value = -value;
    }

    int sav_value = value;
    int power_base = 0;

    while (value >= base) {
        value = value / base;
        power_base++;
    }

    value = sav_value;

    while (power_base >= 0) {
		if (buffer_size < 0) {
			return false;
		}
		
        int to_print = value/power_int(base, power_base);
        if (to_print <= 9) {
            buffer[0] = 48 + (to_print);
        } else {
            buffer[0] = 55 + (to_print);
        }
        value-= (value/power_int(base, power_base)) * power_int(base, power_base);

		buffer_size--;
		buffer+=1;

        power_base-=1;
    }

	if (buffer_size <= 0) {
		return false;
	}

	buffer[0] = '\0';

    return true;
}


// compare two strings (basic comparison, only if it is equals)
bool strcmp(char s1[], char s2[]) {
	if (strlen(s1) != strlen(s2)) return false;

	while (*s1 != '\0')
		if (*s1++ != *s2++) return false;
	
	return true;
}


// store into all args the values of text recovered from format
bool in_format_factor(char* format, char* text, int nb_args, void * arg, ...) { // arg is basically the start of an array of void*
	
	int format_length = strlen(format); // size of the format string
	int text_size = strlen(text);		// size of the text string

	int format_index = 0;				// index of the format process
	int text_index = 0;					// index of the text process
	int arg_index = 0;					// index for wich argument is treating

	void** args = (void**)(&arg); // i traduce arg as what it is really, an array of void* (=> void**)

	while (format_index < format_length) {
		if (format[format_index] == text[text_index]) {
			//print_char(text[text_index]);
			format_index+=1;
			text_index+=1;
		} else if (format[format_index] == '%' && format_index<format_length-1) { // manage %*
			
			switch (format[format_index+1]) {

				case 'd': // int
					
					
					char buff[21]; // 21 is the max size for an unsigned long long int in digit
					int i;
					
					for (i=0; text[i+text_index] >= 48 && text[i+text_index] <= 57 && i+text_index<text_size && i<20; i++) { // leave space for '\0' char
						if (text[i+text_index] == format[format_index+2]) { // si on a un chiffre qui correspond normalement au caractère suivant
							break;	// alors on a atteint la suite du format, donc on met fin à cette analyse
						}
						buff[i] = text[i+text_index];
					}
					buff[i] = '\0';

					*(int*)(args[arg_index]) = atoi(buff);
					

					arg_index += 1;
					format_index += 2;
					text_index += i;

					break;

				case 'c': // char

					*(char*)(args[arg_index]) = text[text_index];

					arg_index += 1;
					format_index+=2;
					text_index += 1;

					break;



				default:
					break;
			}

		} else {
			return false;
		}

	}

	return true;

}


/*
bool out_format_factor(char format, int buffer_size, char* buffer, int nb_args, void * arg, ...) {
	int format_index = 0;
	int format_length = strlen(format);

	while (format_index < format_length) {
		if (format[format_index] == '%' && format_index+1 < format_length) {
			switch (format[format_index+1]) {
				case 'd':
					break;
				
				case 'c':
					break;
			}
		}
	}

	return false;
}*/