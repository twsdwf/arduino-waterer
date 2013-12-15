#ifndef _AT24C512
#define _AT24C512
#include <inttypes.h>
#include <Arduino.h>
#include <Wire.h>

class AT24C512{
	protected:
		int address;
	public:
		AT24C512(int addr = 80);
		~AT24C512();
		void init();
		uint8_t read(uint16_t address);
		void write(uint16_t address, uint8_t byte);
		int writeBuffer(int start, char*buf, int length);
		int readBuffer(int start, char*buf, int length);
};


#endif
