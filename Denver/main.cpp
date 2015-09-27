/*
		Project Denver aka (phibiaNES Next)
		Goals:

			* Accuracy.

*/

#include <stdio.h>
#include <io.h>
#include <iostream>
#include "hardware/bus.h"
#include "hardware/cartridge.h"
#include "mappers/mmc1.h"

// test bus devices.
// first create a new bus device, called HexWriter

class hexwriter : public bus_device {
public:
	hexwriter::hexwriter() {
		mask = 0xFFFF;
		baseaddr = 0x0000;
		busdelay = 0x03;
	}
	void	write(unsigned __int8 data) {
		std::cout << data;
	}
};

int main() {
	// create/initialize the bus.
	bus *MyBus = new bus;
	hexwriter *writer = new hexwriter;
	MyBus->add_device(writer);

	// overruled bus at addr 0x3000 (empty bus)
	bus_device  *empty = new bus_device;
	empty->baseaddr = 0x3000;
	empty->mask = 0xFFF;
	MyBus->add_device(empty);

	MyBus->setaddr(0x4200);

	base_cartridge *cart = new base_cartridge(MyBus, MyBus);
	// vcpu *cpu = new vcpu (cpubus);
	// keyboard_controller *controller1 = new keyboard_controller (ID_PLAYER1, cpubus);
	// dinput_controller *controller2 = new dinput_controller (ID_PLAYER2, cpubus);
	// vppu *ppu = new vppu (cpubus, ppubus);
	// vapu *apu = new vapu (cpubus);

	// Denver *MyEmu = new Denver ();
	// Denver->DisplayInterface (MyDirect2DDisplayHandler);
	// Denver->AudioInterface (MySDLAudioHandler);
	// MyDirect2DDisplayHandler->Filter (XBRZ4);
	// MyDirect2DDisplayHandler->EnableScanLines (0 .. 1.0);
	// or MyOpenGLDisplayHandler or MySDLDisplayHandler;
	// inherited from DefaultDisplayHandler;

	char *text = "This is an BUS device test"; // with device empty @ 0x3000 this should not be written

	int i = 0;
	while (text[i]) {
		MyBus->write(text[i++]);
	}

	// we set the addr to 0x4200, our device resides @ 0x3000 (MASK 0x0FFF)
	// so if we want to get the current address it should say 0x200
	std::cout << "device current addr " << std::hex << "0x" << empty->currentaddr << std::endl;

	return 0;
}