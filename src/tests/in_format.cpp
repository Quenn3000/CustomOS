#include <strings.hpp>

#include "test_suite.hpp"

void test_in_format_factor() {
    test_begin("strings/in_format_factor");

    int number = 0;
    char letter = 0;
    char format[] = "%d %c";
    char text[] = "42 Z";

    bool ok = in_format_factor(format, text, 2, &number, &letter);

    CHECK(ok);
    CHECK_INT(number, 42);
    CHECK_INT(letter, 'Z');

    test_end();
}
