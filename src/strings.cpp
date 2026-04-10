#include <strings.hpp>
#include <math.hpp>
#include <types.hpp>
#include <utils.hpp>


// get the size of a string (before '\0')
int strlen(const char* s) {
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

// convert an int into a string in the buffer, in the base you want (return the number of digits, 0 if failed)
int itoa(int value, int buffer_size, char* buffer, int base) {
	int nb_digit = 0;

	if (base < 2 || base > 32) {
        return 0;
    }

    if (value < 0) {
		if (buffer_size-- <= 0) {
			return 0;
		}
        buffer[0] = '-';
		buffer+=1;
        value = -value;

		nb_digit += 1;
    }

    int sav_value = value;
    int power_base = 0;

    while (value >= base) {
        value = value / base;
        power_base++;
    }

	nb_digit += power_base+1;

    value = sav_value;

    while (power_base >= 0) {
		if (buffer_size < 0) {
			return 0;
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
		return 0;
	}

	buffer[0] = '\0';

    return nb_digit;
}


// convert an unsigned int into a string in the buffer, in the base you want (return the number of digits, 0 if failed)
int uitoa(unsigned int value, int buffer_size, char* buffer, int base) {
	if (base < 2 || base > 32) {
        return 0;
    }

    unsigned int sav_value = value;
    int power_base = 0;

    while (value >= base) {
        value = value / base;
        power_base++;
    }

	int nb_digit = power_base+1;

    value = sav_value;

    while (power_base >= 0) {
		if (buffer_size < 0) {
			return 0;
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
		return 0;
	}

	buffer[0] = '\0';

    return nb_digit;
}


// compare two strings (basic comparison, only if it is equals)
int strcmp(const char s1[], const char s2[]) {

	while (*s1 != '\0' || *s2 != '\0')
		if (*s1++ != *s2++) {
			s1-=2;
			s2-=2;
			if (s1++ == 0)
				return -1;
			else if (s2++ == 0)
				return 1;
			
			return *s1 < *s2 ? -1 : 1;
		}
	
	return 0;
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


/* return value :
1	: no error
-1	: not enought variables informed
-2	: buffer filled when treating a number
-3	: not a good format after '%'
-4	: buffer filled without '\0' at the end
*/
int out_format_factor(const char* format, char* buffer, int buffer_size, int nb_args, void * arg, ...) {
	int buffer_index = 0;
	int format_index = 0;
	int arg_index = 0;
	int format_length = strlen(format);

	void** args = (void**)(&arg);


	while (format_index <= format_length && buffer_index < buffer_size) {
		if (format[format_index] == '%' && format_index+1 < format_length) {
			if (arg_index >= nb_args)
				return -1;
			int base = 0;
			int power_base = 0;
			int nb_digit = 0;
			int64_t x;

			uint64_t ux;
			uint64_t usav_x;
			switch (format[format_index+1]) {
				// signed int
				case 'd':
					base = 10;

				case 'x':
					if (base == 0)
						base = 16;
				case 'b':
					if (base == 0)
						base = 2;

					x = *(int*)(args[arg_index]);

					nb_digit = itoa(x, buffer_size-buffer_index, &buffer[buffer_index], base);
					if (!nb_digit) {
						print_string("itoa not good\n");
						return -2;
					}
					buffer_index+= nb_digit;
				

				// unsigned int
				case 'u':
				if (format_index+2 < format_length) {
					switch (format[format_index+2]) {
						case 'd':
							base = 10;

						case 'x':
							if (base == 0)
								base = 16;
						case 'b':
							if (base == 0)
								base = 2;

							ux = *(unsigned int*)(args[arg_index]);

							nb_digit = uitoa(ux, buffer_size-buffer_index, &buffer[buffer_index], base);
							if (!nb_digit) {
								print_string("itoa not good\n");
								return -2;
							}
							buffer_index+= nb_digit;
						
							
					}
					
					format_index+=1;

				} else {
					return -5;
				}

				break;
				
				case 'c':
					buffer[buffer_index++] = *(char*)(args[arg_index]);
					break;
				

				default:
					return -3;
			}

			arg_index+=1;
			format_index+=2;
		} else {
			buffer[buffer_index++] = format[format_index++];
		}
	}

	if (buffer_index < buffer_size) {
		buffer[buffer_index] = '\0';
	} else {
		return -4;
	}

	return 1;
}