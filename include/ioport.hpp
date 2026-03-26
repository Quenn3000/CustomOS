#pragma once

#include <types.hpp>

// write a value into an output port
void outb(unsigned short port, unsigned char val);
unsigned char inb(unsigned short port);

class Port8Bit {
    public:
        uint16_t port_number;

        Port8Bit(uint16_t port_number);

        void write(uint8_t val);
        uint8_t read();

};


class Port16Bit {
    public:
        uint16_t port_number;

        Port16Bit(uint16_t port_number);

        void write(uint16_t val);
        uint16_t read();

};

class Port32Bit {
    public:
        uint16_t port_number;

        Port32Bit(uint16_t port_number);

        void write(uint32_t val);
        uint32_t read();

};