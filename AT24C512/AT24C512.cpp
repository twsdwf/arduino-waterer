
#include "AT24C512.h"

AT24C512::AT24C512(int addr)
{
	this->address = addr;
}

AT24C512::~AT24C512()
{

}

void AT24C512::init()
{
	Wire.begin();
}

uint8_t AT24C512::read(uint16_t address)
{
    Wire.beginTransmission(this->address);
		Wire.write((uint8_t)(address >> 8));
		Wire.write((uint8_t)(address >> 0));
    Wire.endTransmission();
    delay(5);
    Wire.requestFrom(this->address, sizeof(uint8_t));
    return Wire.read();
}

void AT24C512::write(uint16_t address, uint8_t byte)
{
    Wire.beginTransmission(this->address);
		Wire.write((uint8_t)((address) >> 8));
		Wire.write((uint8_t)((address) >> 0));
		Wire.write(byte);
    Wire.endTransmission();
    delay(5);
}

int AT24C512::writeBuffer(int start, char*buf, int length)
{
	char*ptr= buf;
	for (int i = 0; i < length; ++i, ++start) {
		this->write(start, *ptr++);
	}
	return start;
}

int AT24C512::readBuffer(int start, char*buf, int length)
{
	char *ptr = buf;
	for (int i = 0; i < length; ++i, ++start) {
		*ptr++ = this->read(start);
	}
	return length;
}
