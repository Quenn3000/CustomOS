#include <strings.hpp>

#include "test_suite.hpp"

void test_strings() {
    test_begin("strings/strlen");
    CHECK_INT(strlen(""), 0);
    CHECK_INT(strlen("hello"), 5);
    test_end();

    test_begin("strings/atoi");
    char decimal[] = "42";
    char negative[] = "-42";
    char hexadecimal[] = "ff";
    CHECK_INT(atoi(decimal), 42);
    CHECK_INT(atoi(negative), -42);
    CHECK_INT(atoi(hexadecimal, 16), 255);
    test_end();

    test_begin("strings/itoa + uitoa");
    char buffer[32];
    CHECK_INT(itoa(42, 32, buffer, 10), 2);
    CHECK_STR(buffer, "42");
    CHECK_INT(itoa(-42, 32, buffer, 10), 3);
    CHECK_STR(buffer, "-42");
    CHECK_INT(uitoa(255, 32, buffer, 16), 2);
    CHECK_STR(buffer, "FF");
    CHECK_INT(uitoa(5, 32, buffer, 2), 3);
    CHECK_STR(buffer, "101");
    test_end();

    test_begin("strings/strcmp");
    CHECK_INT(strcmp("abc", "abc"), 0);
    CHECK(strcmp("abc", "abd") < 0);
    CHECK(strcmp("abd", "abc") > 0);
    test_end();
}
