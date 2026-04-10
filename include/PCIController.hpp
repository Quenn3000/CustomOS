#pragma once

#include <types.hpp>
#include <ioport.hpp>

#define PCI_DATAPORT 0xCFC
#define PCI_COMMANDPORT 0xCF8


enum BaseAdressRegisterType {
	MemoryMapping = 0, // transfert des données via des bandes RAM
	InputOutput = 1 // transfert des données via les ports
};

class BaseAdressRegister {
	public:
		bool prefetchable;
		uint8_t address;
		uint32_t size;
		BaseAdressRegisterType type;
};

class PeriphericalComponnentInterconnectedDeviceDescriptor {
	public:
		uint32_t portBase;
		uint32_t interrupt;

		uint8_t bus;
		uint16_t device;
		uint8_t function;

		uint16_t vendor_id;
		uint16_t device_id;

		uint8_t class_id;
		uint8_t subclass_id;
		uint8_t interface_id;

		uint8_t revision;

};

class PeriphericalComponnentInterconnectController {
	public:
		PeriphericalComponnentInterconnectController();
		void print_devices();

	private:
		uint32_t read(uint8_t bus, uint16_t device, uint8_t function, uint32_t registerOffset);
		void write(uint8_t bus, uint16_t device, uint8_t function, uint32_t registerOffset, uint32_t value);

		bool device_has_function(uint8_t bus, uint16_t device);

		uint32_t get_id(uint8_t bus, uint16_t device, uint8_t function, uint32_t registerOffset);

		PeriphericalComponnentInterconnectedDeviceDescriptor get_device_descriptor(uint8_t bus, uint16_t device, uint8_t function);

		Port32Bit dataPort;
		Port32Bit commandPort;
};