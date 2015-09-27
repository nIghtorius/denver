/*

	cartridge.h
	(c) 2015 Peter Santing.

	base class of the cartridge -> NROM
	
*/

#pragma once;
#include "bus.h"	// we are reliant of the BUS device, as a cartridge connects to 2 busses.

class cart_prg : public bus_device {
private:
public:
	unsigned __int8	*data;
	cart_prg::cart_prg();
	void	set32k();
	void	set16k();
	unsigned __int8 virtual read();
};

class cart_chr : public bus_device {
private:
	bool	chr_ram_enable;
public:
	unsigned __int8	*data;
	cart_chr::cart_chr();
	cart_chr::~cart_chr();
	void	set4k();
	void	set8k();
	void	create_chr_ram();
	void	destroy_chr_ram();
	unsigned __int8 virtual read();
	void			virtual write(unsigned __int8 data);
};

class base_cartridge {
protected:
	bus *cpu_bus;
	bus *ppu_bus;
	cart_prg *prg;
	cart_chr *chr;
	unsigned __int16 prgbanks;
	unsigned __int16 chrbanks;

public:
	base_cartridge::base_cartridge(){};
	base_cartridge::base_cartridge(bus *cpu_bus, bus *ppu_bus);
	base_cartridge::~base_cartridge();
	void	assign_rom_data(void *prgrom, unsigned __int16 prgbanks, void *chrrom, unsigned __int16 chrbanks);
};

