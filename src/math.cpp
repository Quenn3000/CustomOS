#include <math.hpp>
#include <types.hpp>

// x to the power y
int power_int(int x, int y) {
    int res = 1;
    while (y-- > 0)
        res *= x;
    
    return res;
}

// EN FAIT Y'A PAS BESOIN, LA DIVISION D'INT ARRONDI AUTOMATIQUEMENT
// A L'INFERIEUR :________
// Bon ben j'y laisse en hommage au travail
/*
int round_inf(double x) {
    unsigned long long int_x;
    __builtin_memcpy(&int_x, &x, sizeof(x));
    int16_t E = (int_x>>52)&0x03FF; // 0000 0011  1111 1111 -> virer le signe
    int16_t p = E-1023; // position de la virgule

    int64_t round = int_x>>(52-p);
    return round;
}*/