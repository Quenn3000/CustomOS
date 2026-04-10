#include <interrupt_handlers.hpp>
#include <interrupt_descriptor_table.hpp>
#include <ioport.hpp>
#include <types.hpp>
#include <utils.hpp>

extern "C" __attribute__((interrupt)) void default_handler(uint64_t* entry) {
    // You can print something or halt
    Port8Bit master_port(PIC_MASTER_COMMAND_PORT);
    master_port.write(0x20);
}