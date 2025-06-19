#include "math.hpp"

uint64_t power_int(uint32_t x, uint32_t y) {
    int res = 1;
    while (y-- > 0)
        res *= x;
    
    return res;
}