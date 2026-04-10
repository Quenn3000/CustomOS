#include <types.hpp>
#include <ioport.hpp>
#include <PCIController.hpp>
#include <utils.hpp>

PeriphericalComponnentInterconnectController::PeriphericalComponnentInterconnectController():
	dataPort(PCI_DATAPORT), commandPort(PCI_COMMANDPORT) {}

uint32_t read(uint8_t bus, uint16_t device, uint8_t function, uint32_t registerOffset);
void write(uint8_t bus, uint16_t device, uint8_t function, uint32_t registerOffset, uint32_t value);


uint32_t PeriphericalComponnentInterconnectController::get_id(uint8_t bus, uint16_t device, uint8_t function, uint32_t registerOffset) {
	
	uint32_t id = 0x1 << 31
		| ((bus && 0xFF) << 16)
		| ((device && 0x1F) << 11)
		| ((function & 0x07) << 8)
		| ((registerOffset) & 0xFC);
	return id;
}

uint32_t PeriphericalComponnentInterconnectController::read(uint8_t bus, uint16_t device, uint8_t function, uint32_t registerOffset) {
	uint32_t id = this->get_id(bus, device, function, registerOffset);
	commandPort.write(id);
	uint32_t result = dataPort.read();

	return result >> (8* (registerOffset % 4));
}
void PeriphericalComponnentInterconnectController::write(uint8_t bus, uint16_t device, uint8_t function, uint32_t registerOffset, uint32_t value) {
	uint32_t id = this->get_id(bus, device, function, registerOffset);
	commandPort.write(id);
	dataPort.write(value);
}

bool PeriphericalComponnentInterconnectController::device_has_function(uint8_t bus, uint16_t device) {
	return this->read(bus, device, 0, 0x0E) & (1<<7);
}


PeriphericalComponnentInterconnectedDeviceDescriptor PeriphericalComponnentInterconnectController::get_device_descriptor(uint8_t bus, uint16_t device, uint8_t function) {
	PeriphericalComponnentInterconnectedDeviceDescriptor res;

	res.interrupt = this->read(bus, device, function, 0x3C);

	res.bus = bus;
	res.device = device;
	res.function = function;

	res.vendor_id = this->read(bus, device, function, 0x00);
	res.device_id = this->read(bus, device, function, 0x02);

	res.class_id = this->read(bus, device, function, 0x0b);
	res.subclass_id = this->read(bus, device, function, 0x0a);
	res.interface_id = this->read(bus, device, function, 0x09);

	res.revision = this->read(bus, device, function, 0x08);

	return res;
}



void PeriphericalComponnentInterconnectController::print_devices() {
	for (int bus=0; bus<8; bus++) {
		for (int device=0; device<32; device++) {
			int function_nb = this->device_has_function(bus, device) ? 8 : 1;
			for (int function=0; function<function_nb; function++) {
				PeriphericalComponnentInterconnectedDeviceDescriptor dev = get_device_descriptor(bus, device, function);

				if (dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF) {
					continue;
				}

				debug_string("PCI BUS");
				debug_int(dev.bus, 16);

				debug_string(", DEVICE ");
				debug_int(dev.device, 16);

				debug_string(", FUNCTION ");
				debug_int(dev.function, 16);

				debug_string(" = VENDOR ");
				debug_int(dev.device, 16);

				debug_string(", DEVICE_ID");
				debug_int(dev.device_id, 16);

				debug_string("\n");
				
			}
		}
	}
}