#include <types.hpp>

extern "C" {
    void* memcpy(void* dest, const void* src, uint32_t count) {
        char* d = (char*)dest;
        const char* s = (const char*)src;
        while (count--) {
            *d++ = *s++;
        }
        return dest;
    }

    void* memset(void* dest, int val, uint32_t count) {
        unsigned char* ptr = (unsigned char*)dest;
        while (count--) {
            *ptr++ = (unsigned char)val;
        }
        return dest;
    }
}