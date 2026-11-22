#include <strings.hpp>
#include <utils.hpp>

#include "test_suite.hpp"

void test_out_format_factor() {
    test_begin("strings/out_format_factor");

    int number = 300;
    char letter = 'c';
    char buffer[128];
    void* args[] = { &number, &letter };

    int result = out_format_factor("INT=%d CHAR=%c", buffer, 128, 2, args);

    CHECK_INT(result, 1);
    CHECK_STR(buffer, "INT=300 CHAR=c");

    test_end();
}

void test_printf() {
    test_begin("utils/printf");

    int number = 3000;
    char format[] = "printf value=%d\n";

    CHECK_INT(printf(format, 1, &number), 1);

    test_end();
}
