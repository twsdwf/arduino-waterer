
#include "AT24Cxxx.h"

AT24Cxxx::AT24Cxxx(int addr)
{
	this->address = addr;
}

AT24Cxxx::~AT24Cxxx()
{

}

void AT24Cxxx::init()
{
	Wire.begin();
}

uint8_t AT24Cxxx::read(uint16_t address)
{
    Wire.beginTransmission(this->address);
		Wire.write((uint8_t)(address >> 8));
		Wire.write((uint8_t)(address >> 0));
    Wire.endTransmission();
    delay(5);
    Wire.requestFrom(this->address, sizeof(uint8_t));
    return Wire.read();
}

void AT24Cxxx::write(uint16_t address, uint8_t byte)
{
    Wire.beginTransmission(this->address);
		Wire.write((uint8_t)((address) >> 8));
		Wire.write((uint8_t)((address) >> 0));
		Wire.write(byte);
    Wire.endTransmission();
    delay(5);
}

int AT24Cxxx::writeBuffer(int start, char*buf, int length)
{
	char*ptr= buf;
	Wire.beginTransmission(this->address);
	if (!(start & 0x3F)) {
		Wire.write((uint8_t)((start) >> 8));
		Wire.write((uint8_t)((start) >> 0));
		for (int i = 0; i < length; ++i, ++start) {
			this->write(start, *ptr++);
		}
		Wire.endTransmission();
	} else {
		for (int i = 0; i < length; ++i, ++start) {
				Wire.beginTransmission(this->address);
				Wire.write((uint8_t)((start) >> 8));
				Wire.write((uint8_t)((start) >> 0));
				this->write(start, *ptr++);
				Wire.endTransmission();
			}
	}
	delay(5);
	return start;
}

int AT24Cxxx::readBuffer(int start, char*buf, int length)
{
	char *ptr = buf;
    Wire.beginTransmission(this->address);
		Wire.write((uint8_t)(start >> 8));
		Wire.write((uint8_t)(start >> 0));
    Wire.endTransmission();
    delay(5);
	Wire.requestFrom(this->address, length, 1);
	while (length > 0 && Wire.available() > 0 ) {
		*ptr++ = Wire.read();
		--length;
	}
// 	for (int i = 0; i < length; ++i, ++start) {
// 		*ptr++ = Wire.read();
// 	}

	return (int)(ptr-buf);
}
