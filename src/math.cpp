#include "math.hpp"

// x to the power y
int power_int(int x, int y) {
    int res = 1;
    while (y-- > 0)
        res *= x;
    
    return res;
}
