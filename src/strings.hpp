#ifndef __STRINGS_H_
#define __STRINGS_H_

#include "types.hpp"

int strlen(char* s); // return the number of char in a string (strictly before '\0')
int atoi(char* s, int base=10); // convert a char* representing an int in a specific base into int type variable
bool itoa(int value, int buffer_size, char* buffer, int base);
bool strcmp(char* const s1, char* const s2);


bool in_format_factor(char* format, char* text, int nb_args, void * arg, ...);
bool out_format_factor(char format, int buffer_size, char* buffer, int nb_args, void * arg, ...);


#endif