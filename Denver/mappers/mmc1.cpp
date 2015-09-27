/*

	cartridge mmc1 class.

	What it does.
	It emulates the mmc1 cartridge. Assigns PRG-RAM, PRG-ROM, CHR-RAM and CHR_ROM
	The data of it will be provided on the BUS.

*/

#include "mmc1.h"

mmc1_cartridge::mmc1_cartridge(bus *cpu_bus, bus *ppu_bus)
{
	this->cpu_bus = cpu_bus;
	this->ppu_bus = ppu_bus;
	prg = new mmc1_prg;
	chr = new mmc1_chr;
	cpu_bus->add_device(prg);
	ppu_bus->add_device(chr);
}