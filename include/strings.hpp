#pragma once

#include <types.hpp>

int strlen(const char* s); // return the number of char in a string (strictly before '\0')
int atoi(char* s, int base=10); // convert a char* representing an int in a specific base into int type variable
int itoa(int value, int buffer_size, char* buffer, int base);
int uitoa(unsigned int value, int buffer_size, char* buffer, int base); // unsigned itoa
int strcmp(const char s1[], const char s2[]);


bool in_format_factor(char* format, char* text, int nb_args, void * arg, ...);
int out_format_factor(const char* format, char* buffer, int buffer_size, int nb_args, void * arg, ...);
