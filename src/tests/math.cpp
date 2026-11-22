#include <math.hpp>

#include "test_suite.hpp"

void test_math() {
    test_begin("math/power_int");

    CHECK_INT(power_int(2, 10), 1024);
    CHECK_INT(power_int(3, 3), 27);
    CHECK_INT(power_int(7, 0), 1);
    CHECK_INT(power_int(5, 1), 5);
    CHECK_INT(power_int(-2, 3), -8);

    test_end();
}
