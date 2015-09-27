/*

	cartridge base class.

	What it does.
	It emulates the cartridge. Assigns PRG-RAM, PRG-ROM, CHR-RAM and CHR_ROM
	The data of it will be provided on the BUS.

*/

#include "cartridge.h"
#include <stdlib.h>
#include <malloc.h>

base_cartridge::base_cartridge(bus *cpu_bus, bus *ppu_bus) {
	// we create a new instance of the cartridge object.
	this->cpu_bus = cpu_bus;
	this->ppu_bus = ppu_bus;
	// create 2 bus devices (CHR, PRG)
	prg = new cart_prg;
	chr = new cart_chr;
	cpu_bus->add_device(prg);
	ppu_bus->add_device(chr);
}

base_cartridge::~base_cartridge() {
	// destroys the cartridge.
	// first unregister bus devices.
	cpu_bus->remove_device(prg);
	cpu_bus->remove_device_from_address(0x8000, true);
	ppu_bus->remove_device(chr);
	ppu_bus->remove_device_from_address(0x0000, false);
	delete prg;
	delete chr;
}

cart_prg::cart_prg() {
	baseaddr = 0x8000;	// NROM carts always have 0x8000-0xFFFF
	mask = 0x3FFF;		// We default @ 16kB ROMs
}

void cart_prg::set32k() {
	mask = 0x7FFF;		// full 32kB ROM cart.
}

void cart_prg::set16k() {
	mask = 0x3FFF;		// half 16kB ROM cart.
}

cart_chr::cart_chr() {
	baseaddr = 0x0000;
	mask = 0x1FFF;		// CHR is @ default 8kB
	chr_ram_enable = false;
}

cart_chr::~cart_chr() {
	destroy_chr_ram();
}

void cart_chr::set4k() {
	mask = 0xFFF;	// CHR is 4kB (CHR-ROM 4kB will be mirrored)
}

void cart_chr::set8k() {
	mask = 0x1FFF;	// CHR is 8kB
}

void cart_chr::create_chr_ram() {
	chr_ram_enable = true;
	data = (unsigned __int8 *) malloc(8192);
	mask = 0x1FFF;	// create 8kB CHR-RAM and adjust size accordingly.
}

void cart_chr::destroy_chr_ram() {
	if (chr_ram_enable) free(data);
	chr_ram_enable = false;
}

unsigned __int8 cart_prg::read() {
	return data[currentaddr];
}

unsigned __int8 cart_chr::read() {
	return data[currentaddr];
}

void cart_chr::write(unsigned __int8 data) {
	if (chr_ram_enable) this->data[currentaddr] = data;
}

void base_cartridge::assign_rom_data(void *prgrom, unsigned __int16 prgbanks, void *chrrom, unsigned __int16 chrbanks)
{
	prg->data = (unsigned __int8 *)prgrom;
	this->prgbanks = prgbanks;
	this->chrbanks = chrbanks;
	if (chrbanks = 0) chr->create_chr_ram();
	if (prgbanks > 1) prg->set32k(); else prg->set16k();
}