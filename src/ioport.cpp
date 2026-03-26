#include <ioport.hpp>
#include <types.hpp>
#include <utils.hpp>


/* --- GESTION MANUELLE --- */


void outb(unsigned short port, unsigned char val) {
    asm volatile (
        "outb %0, %1"
        :
        : "a"(val), "Nd"(port)
    );
}


// get the value from an input port
unsigned char inb(unsigned short port) {
    unsigned char ret;
    asm volatile (
        "inb %1, %0"
        : "=a"(ret)
        : "Nd"(port)
    );
    return ret;
}

/* --- PORT 8 BITS --- */

Port8Bit::Port8Bit(uint16_t port_number) : port_number(port_number) {}


void Port8Bit::write(uint8_t val) {
    __asm__ volatile (
        "outb %0, %1"
        :
        : "a"(val), "Nd"(this->port_number)
    );
}


uint8_t Port8Bit::read() {
    uint8_t ret;
    asm volatile (
        "inb %1, %0"
        : "=a"(ret)
        : "Nd"(this->port_number)
    );
    return ret;
}


/* --- PORT 16 BITS --- */



Port16Bit::Port16Bit(uint16_t port_number) : port_number(port_number) {}


void Port16Bit::write(uint16_t val) {
    __asm__ volatile (
        "outw %0, %1"
        :
        : "a"(val), "Nd"(this->port_number)
    );
}


// get the value from an input port
uint16_t Port16Bit::read() {
    uint16_t ret;
    asm volatile (
        "inw %1, %0"
        : "=a"(ret)
        : "Nd"(this->port_number)
    );
    return ret;
}


/* --- PORT 32 BITS --- */



Port32Bit::Port32Bit(uint16_t port_number) : port_number(port_number) {}


void Port32Bit::write(uint32_t val) {
    __asm__ volatile (
        "outl %0, %1"
        :
        : "a"(val), "Nd"(this->port_number)
    );
}


// get the value from an input port
uint32_t Port32Bit::read() {
    uint32_t ret;
    asm volatile (
        "inl %1, %0"
        : "=a"(ret)
        : "Nd"(this->port_number)
    );
    return ret;
}