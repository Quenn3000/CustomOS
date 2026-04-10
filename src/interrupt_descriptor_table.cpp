#include <interrupt_descriptor_table.hpp>
#include <utils.hpp>
#include <interrupt_handlers.hpp>
#include <ioport.hpp>

// https://pdos.csail.mit.edu/6.828/2005/readings/i386/s09_05.htm


bool InterruptManager::set_interrupt(int n, uint32_t handler) {
    if (this->idt[n].offset_low || this->idt[n].offset_high || n >= this->nb_interrupt) { // check si un handler avait déjà été renseigné pour la fonction
        return false;
    }
    this->idt[n].offset_low = handler & 0xFFFF;
    this->idt[n].selector = 0x08; // Kernel code segment
    this->idt[n].zero = 0;
    this->idt[n].type_attr = 0x8E; // 32-bit interrupt gate
    this->idt[n].offset_high = (handler >> 16) & 0xFFFF;

    return true;
}

bool InterruptManager::reset_interrupt(int n) {
    if (n >= this->nb_interrupt) {
        return false;
    }
    this->idt[n].offset_low = 0;
    this->idt[n].selector = 0; // Kernel code segment
    this->idt[n].zero = 0;
    this->idt[n].type_attr = 0; // 32-bit interrupt gate
    this->idt[n].offset_high = 0;

    return true;
}

// set all parameters to call correctly load_idt
void InterruptManager::idt_install() {
    this->idt_ptr.limit = (sizeof(IDTEntry) * IDT_SIZE) - 1;
    this->idt_ptr.base = (uint32_t)&idt;

    load_idt(&this->idt_ptr);
}

// global function for initializing IDT
void InterruptManager::init() {
    asm volatile ("cli");

    this->set_interrupt(0x20, (uint32_t)default_handler);

    this->idt_install();

    this->init_pic();

    asm volatile ("sti");
}

// PIC remapping and masking
void InterruptManager::init_pic() {
    Port8Bit master_command_port(PIC_MASTER_COMMAND_PORT);
    Port8Bit slave_command_port(PIC_SLAVE_COMMAND_PORT);

    Port8Bit master_data_port(PIC_MASTER_DATA_PORT);
    Port8Bit slave_data_port(PIC_SLAVE_DATA_PORT);
    

    master_command_port.write(0x11); // initialise the configuration
    slave_command_port.write(0x11);

    master_data_port.write(0x20); // set start of interrupts number
    slave_data_port.write(0x28);

    master_data_port.write(0x04); // set the connection between Slave and Master PICs
    slave_data_port.write(0x02);

    master_data_port.write(0x1); // 8086 environment
    slave_data_port.write(0x1);

    master_data_port.write(0xFD); // mask all interrupts except IRQ1 and IRQ0 : mask 11111100
    slave_data_port.write(0xFF); // mask all interrupts
}


InterruptManager::InterruptManager() {
    for (int i=0; i<this->nb_interrupt; this->reset_interrupt(i++)); // reset all interruptions catchings
}