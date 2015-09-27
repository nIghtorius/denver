/*

	CPU/PPU memory bus.

	What it does.
	It emulates the memory bus(ses) of the CPU, PPU and cartridge parts.
	It also "connects" the cartridge CHR-R(O)(A)M and PRG-R(O)(A)M to the PPU and CPU respectively.
	It should also enable true bus-conflict emulation.

	Project Denver is about maximum accuracy, not performance.

*/

#include "bus.h"
#include <windows.h>

bus::bus() {
	for (int i = 0; i < MAX_BUS_DEVICES; i++) busses[i] = NULL;
}

void	bus::add_device(bus_device *device) 
{
	/*
		Registers a device (be it a PPU, be it a CPU, be it a (CHR)(PRG)-R(O)(A)M..
		the BUS object should synchro connections.
	*/
	for (int i = 0; i < MAX_BUS_DEVICES; i++) {
		if (busses[i] == NULL) {
			busses[i] = device;
			break;
		}
	}
}

void	bus::remove_device(bus_device *device)
{
	/*
		Unregisters a device.
	*/
	for (int i = 0; i < MAX_BUS_DEVICES; i++) {
		if (busses[i] == device) {
			busses[i] = NULL;
			break;
		}
	}
}

void	bus::remove_device_from_address(unsigned __int16 addr, bool _continue) {
	/*
		Unregisters a device from address and up.
		example: 0x8000 will remove device 0x8000 and up.
	*/
	for (int i = 0; i < MAX_BUS_DEVICES; i++) {
		if (busses[i]) {
			if (busses[i]->baseaddr >= addr) {
				busses[i] = NULL;
				if (!_continue) break;
			}
		}
	}
}

void	bus_device::setaddr(unsigned __int16 addr)
{
	currentaddr = addr & mask;
}

unsigned __int8	bus_device::read() 
{ 
	return 0x00;
}

void			bus_device::write(unsigned __int8 data) { }

void	bus::setaddr(unsigned __int16 addr) {
	/*
		set bus address -- also determines which bus to read/write to.
		for example:
		bus 0x00 - 0x0000 (mask 0x07FF) (RAM)
		bus 0x01 - 0x2000 (mask 0x200F) (PPU)
		bus 0x02 - 0x4000 (mask 0x40FF) (APU/CONTROLLER)
		bus 0x03 - 0x8000 (mask 0x????) (ROM)

		selected addr = 0x8402 which will select bus 0x03, because:
		delta with bus 0x00 = 0x8402 (high delta)
		delta with bus 0x01 = 0x6402 (lower delta)
		delta with bus 0x02 = 0x4402 (even lower delta)
		delta with bus 0x03 = 0x0402 (lowest delta)

		bus with the lowest delta takes precedence.
	*/

	unsigned __int16 lowest_delta = 0xFFFF;

	currentaddr = addr;

	for (int i = 0; i < MAX_BUS_DEVICES; i++) {
		if (busses[i]) {
			if (addr >= busses[i]->baseaddr) {
				if (addr - busses[i]->baseaddr < lowest_delta) {
					bus_select = i;
					lowest_delta = addr - busses[i]->baseaddr;
				}
			}
		}
	}
}

unsigned __int8	bus::read()
{
	busses[bus_select]->setaddr(currentaddr - busses[bus_select]->baseaddr);
	return	busses[bus_select]->read();
}

void			bus::write(unsigned __int8 data)
{
	busses[bus_select]->setaddr(currentaddr - busses[bus_select]->baseaddr);
	busses[bus_select]->write(data);
}

unsigned __int16 bus::getaddr()
{
	return currentaddr;
}
