/*

	bus.h
	(c) 2015 Peter Santing

*/

#pragma once;
#define	MAX_BUS_DEVICES			0x20

class bus_device {
private:
public:
	unsigned __int16	currentaddr;
	unsigned __int16	baseaddr;
	unsigned __int16	mask;	
	unsigned __int16	busdelay;
	unsigned __int8		virtual	read();
	void				virtual	write(unsigned __int8 data);
	void				setaddr(unsigned __int16 addr);
};

class bus {
private:
	bus_device *busses[MAX_BUS_DEVICES];
	unsigned __int16	currentaddr;
	unsigned __int8		bus_select;
public:
	bus::bus();
	unsigned __int8		read();
	void				write(unsigned __int8 data);
	void				setaddr(unsigned __int16 addr);
	unsigned __int16	getaddr();
	void				add_device(bus_device *device);
	void				remove_device(bus_device *device);
	void				remove_device_from_address(unsigned __int16 addr, bool _continue);
};
