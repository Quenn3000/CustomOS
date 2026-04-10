#pragma once

#include <interrupt_descriptor_table.hpp>

extern "C" __attribute__((interrupt)) void default_handler(uint64_t* entry);