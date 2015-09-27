/*

	mmc1.h
	(c) 2015 Peter Santing.

	MMC1 cartridge.

*/

#pragma once;
#include "../hardware/cartridge.h"

class mmc1_prg : public cart_prg {
private:
public:
	unsigned __int8 virtual read();
	void			virtual write(unsigned __int8 data);
};

class mmc1_chr : public cart_chr {
private:
public:
	unsigned __int8 virtual read();
	void			virtual write(unsigned __int8 data);
};

class mmc1_cartridge : public base_cartridge {
public:
	mmc1_cartridge::mmc1_cartridge(bus *cpu_bus, bus *ppu_bus);
};
