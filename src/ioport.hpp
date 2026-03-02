#ifndef __IOPORT_H_
#define __IOPORT_H_

// write a value into an output port
inline void outb(unsigned short port, unsigned char val) {
    asm volatile (
        "outb %0, %1"
        :
        : "a"(val), "Nd"(port)
    );
}


// get the value from an input port
inline unsigned char inb(unsigned short port) {
    unsigned char ret;
    asm volatile (
        "inb %1, %0"
        : "=a"(ret)
        : "Nd"(port)
    );
    return ret;
}

#endif