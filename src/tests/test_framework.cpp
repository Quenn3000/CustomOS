#include "test_suite.hpp"
#include <utils.hpp>

static int g_total = 0;
static int g_passed = 0;
static int g_failed = 0;

static int g_group_start_total = 0;
static int g_group_start_passed = 0;
static int g_group_start_failed = 0;

// Deliberately not using strcmp(): it is one of the functions under test.
static bool same_string(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i])
            return false;
        i++;
    }
    return a[i] == b[i];
}

void test_begin(const char* name) {
    debug_string("\n[TEST] ");
    debug_string(name);
    debug_string("\n");

    g_group_start_total = g_total;
    g_group_start_passed = g_passed;
    g_group_start_failed = g_failed;
}

void test_end() {
    debug_string("  => ");
    debug_int(g_passed - g_group_start_passed);
    debug_string("/");
    debug_int(g_total - g_group_start_total);
    debug_string(" passed");
    if (g_failed != g_group_start_failed) {
        debug_string(", ");
        debug_int(g_failed - g_group_start_failed);
        debug_string(" FAILED");
    }
    debug_string("\n");
}

void test_check(bool condition, const char* description) {
    g_total++;
    if (condition) {
        g_passed++;
        debug_string("  [PASS] ");
    } else {
        g_failed++;
        debug_string("  [FAIL] ");
    }
    debug_string(description);
    debug_string("\n");
}

void test_check_int(int actual, int expected, const char* description) {
    test_check(actual == expected, description);
    if (actual != expected) {
        debug_string("         got ");
        debug_int(actual);
        debug_string(", expected ");
        debug_int(expected);
        debug_string("\n");
    }
}

void test_check_str(const char* actual, const char* expected, const char* description) {
    bool ok = same_string(actual, expected);
    test_check(ok, description);
    if (!ok) {
        debug_string("         got \"");
        debug_string(actual);
        debug_string("\", expected \"");
        debug_string(expected);
        debug_string("\"\n");
    }
}

int test_summary() {
    debug_string("\n================================\n");
    debug_string("TEST SUMMARY: ");
    debug_int(g_passed);
    debug_string(" passed, ");
    debug_int(g_failed);
    debug_string(" failed (");
    debug_int(g_total);
    debug_string(" total)\n");
    debug_string("================================\n");
    return g_failed;
}
