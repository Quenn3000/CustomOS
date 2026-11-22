#include <tests.hpp>

#include "test_suite.hpp"

void tests(InterruptManager* itr_manager) {
    debug_string("\n########## CUSTOMOS TEST SUITE ##########\n");

    test_math();
    test_strings();
    test_in_format_factor();
    test_out_format_factor();
    test_printf();
    test_memory();
    test_process(itr_manager);

    test_summary();

    // Everything above returns. The scheduler demo is last on purpose:
    // it enables the PIT and never gives control back.
    debug_string("\n########## PROCESS SCHEDULER DEMO ##########\n");
    debug_string("(takes over the machine, never returns)\n");
    run_process_demo(itr_manager);

    for (;;) { }
}
