#include "../utils.hpp"
#include "../strings.hpp"

void test {
	int t = 1;
    int a = 2;
    char b = 'p';
    char m = 'a';
    if (in_format_factor("%d2%c %d %c", "1125 34 m", 4, &t, &m, &a, &b)) {
        print_string("success format factor\n");
    } else {
        print_string("failed format factor\n");
    }

    print_string("factorised :\n");
    print_int(t);
    print_string("\n");
    print_char(m);
    print_string("\n");
    print_int(a);
    print_string("\n");
    print_char(b);
    print_string("\nend factorised\n");
}