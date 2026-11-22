#pragma once

#include <types.hpp>
#include <interrupt_descriptor_table.hpp>

// --- minimal assertion framework (serial output) ---
void test_begin(const char* name);
void test_end();
void test_check(bool condition, const char* description);
void test_check_int(int actual, int expected, const char* description);
void test_check_str(const char* actual, const char* expected, const char* description);
int  test_summary(); // returns the number of failures

#define CHECK(cond)             test_check((cond), #cond)
#define CHECK_INT(actual, exp)  test_check_int((int)(actual), (int)(exp), #actual " == " #exp)
#define CHECK_STR(actual, exp)  test_check_str((actual), (exp), #actual " == " #exp)

// --- extra test entry points (not declared in include/tests.hpp) ---
void test_strings();
void test_memory();

// The scheduler demo takes over the machine and never returns. It is always
// launched last, after the summary has been printed.
void run_process_demo(InterruptManager* itr_manager);
