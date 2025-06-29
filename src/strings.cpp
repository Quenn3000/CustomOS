#include "strings.hpp"
#include "math.hpp"
#include "types.hpp"
#include "utils.hpp"

int length(char* s) {
	int i;
	for (i=0; s[i] != '\0'; i++);
	return i;
}

int atoi(char* s, int base) {

	int res = 0;
	int nb_digit = length(s);
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



bool in_format_factor(char* format, char* text, int nb_args, void * args, ...) { // here args point to the end of

	/*for (int i=0; i<nb_args; i++) {
		print_int(*((int*)args-i));
		print_string("\n");
	}*/
	
	int format_length = length(format);
	int text_size = length(text);

	int format_index = 0;
	int text_index = 0;

	bool first_match = true;

	while (format_index < format_length) {
		if (format[format_index] == '%' && format_index<format_length-1) { // manage %*
			
			switch (format[format_index+1]) {

				case 'd': // int
					if (first_match) {
						first_match = false;
					} else {
						args-=sizeof(int);
					}
					
					
					char buff[21]; // max size for an unsigned long long int in digit
					int i;
					
					for (i=0; text[i+text_index] >= 48 && text[i+text_index] <= 57 && i+text_index<text_size && i<20; i++) { // let space for '\0' char
						buff[i] = text[i+text_index];
					}
					buff[i] = '\0';

					*(int*)(args) = atoi(buff);
					

					format_index+=2;
					text_index += i;

					break;

				case 'c': // char

					if (first_match) {
						first_match = false;
					} else {
						args-=sizeof(char);
					}

					*(char*)(args) = text[text_index];

					format_index+=2;
					text_index += 1;

					break;



				default:
					break;
			}

		} else if (format[format_index] != text[text_index]) {
			return false;
		} else {
			format_index+=1;
			text_index+=1;
		}

	}

	return true;

}