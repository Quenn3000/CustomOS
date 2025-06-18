#include "strings.hpp"
#include "math.hpp"
#include "types.hpp"

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