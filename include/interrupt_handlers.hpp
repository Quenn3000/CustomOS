#pragma once

#include <types.hpp>
#include <interrupt_descriptor_table.hpp>

extern "C" __attribute__((interrupt)) void default_handler(uint64_t* entry);