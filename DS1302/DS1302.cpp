#include <avr/interrupt.h>
#include <Arduino.h>

#include "DS1302.h"

void DS1302::start( void)
{
	digitalWrite( this->rst_pin, LOW); // default, not enabled
	pinMode( this->rst_pin, OUTPUT);

	digitalWrite( this->sclk_pin, LOW); // default, clock low
	pinMode( this->sclk_pin, OUTPUT);

	pinMode( this->io_pin, OUTPUT);

	digitalWrite( this->rst_pin, HIGH); // start the session
	delayMicroseconds(4);           // tCC = 4us
}

// --------------------------------------------------------
// _DS1302_stop
//
// A helper function to finish the communication.
//
void DS1302::stop(void)
{
	// Set CE low
	digitalWrite( this->rst_pin, LOW);
	delayMicroseconds( 4);           // tCWH = 4us
}
// --------------------------------------------------------
// DS1302_clock_burst_read
//
// This function reads 8 bytes clock data in burst mode
// from the DS1302.
//
// This function may be called as the first function,
// also the pinMode is set.
//

DS1302::DS1302(int _rst, int _sclk, int _io){
	rst_pin = _rst;
	sclk_pin = _sclk;
	io_pin = _io;
}

void DS1302::init()
{
	// Start by clearing the Write Protect bit
	// Otherwise the clock data cannot be written
	// The whole register is written,
	// but the WP-bit is the only bit in that register.
	this->write (DS1302_ENABLE, 0);

	// Disable Trickle Charger.
	this->write (DS1302_TRICKLE, 0x00);
}
// --------------------------------------------------------
// DS1302_write
//
// This function writes a byte to the DS1302 (clock or ram).
//
// The address could be like "0x80" or "0x81",
// the lowest bit is cleared anyway.
//
// This function may be called as the first function,
// also the pinMode is set.
//

void DS1302::write( int address, uint8_t data)
{
	// clear lowest bit (read bit) in address
	bitClear( address, DS1302_READBIT);

	this->start();
	// don't release the I/O-line
	this->togglewrite( address, false);
	// don't release the I/O-line
	this->togglewrite( data, false);
	this->stop();
}

// --------------------------------------------------------
// DS1302_read
//
// This function reads a byte from the DS1302
// (clock or ram).
//
// The address could be like "0x80" or "0x81",
// the lowest bit is set anyway.
//
// This function may be called as the first function,
// also the pinMode is set.
//
uint8_t DS1302::read(int address)
{
	uint8_t data;
	// set lowest bit (read bit) in address
	bitSet( address, DS1302_READBIT);

	this->start();
	// the I/O-line is released for the data
	this->togglewrite( address, true);
	data = this->toggleread();
	this->stop();
	return (data);
}

void DS1302::burst_read( uint8_t *p)
{
	int i;
	this->start();
	// Instead of the address,
	// the CLOCK_BURST_READ command is issued
	// the I/O-line is released for the data
	this->togglewrite( DS1302_CLOCK_BURST_READ, true);
	for ( i=0; i<8; i++) {
		*p++ = this->toggleread();
	}
	this->stop();
}


// --------------------------------------------------------
// DS1302_clock_burst_write
//
// This function writes 8 bytes clock data in burst mode
// to the DS1302.
//
// This function may be called as the first function,
// also the pinMode is set.
//
void DS1302::burst_write( uint8_t *p)
{
	int i;

	this->start();
	// Instead of the address,
	// the CLOCK_BURST_WRITE command is issued.
	// the I/O-line is not released
	this->togglewrite( DS1302_CLOCK_BURST_WRITE, false);
	for ( i=0; i<8; i++) {
		// the I/O-line is not released
		this->togglewrite( *p++, false);
	}
	this->stop();
}

// --------------------------------------------------------
// _DS1302_togglewrite
//
// A helper function for writing a byte with bit toggle
//
// The 'release' parameter is for a read after this write.
// It will release the I/O-line and will keep the SCLK high.
//
void DS1302::togglewrite( uint8_t data, uint8_t release)
{
	int i;

	for( i = 0; i <= 7; i++)
	{
		// set a bit of the data on the I/O-line
		digitalWrite(  this->io_pin, bitRead(data, i));
		delayMicroseconds( 1);     // tDC = 200ns

		// clock up, data is read by DS1302
		digitalWrite( this->sclk_pin, HIGH);
		delayMicroseconds(1);     // tCH = 1000ns, tCDH = 800ns

		if ( release && i == 7) {
			// If this write is followed by a read,
			// the I/O-line should be released after
			// the last bit, before the clock line is made low.
			// This is according the datasheet.
			// I have seen other programs that don't release
			// the I/O-line at this moment,
			// and that could cause a shortcut spike
			// on the I/O-line.
			pinMode( this->io_pin, INPUT);

			// For Arduino 1.0.3, removing the pull-up is no longer needed.
			// Setting the pin as 'INPUT' will already remove the pull-up.
			// digitalWrite (DS1302_IO, LOW); // remove any pull-up
		} else {
			digitalWrite( this->sclk_pin, LOW);
			delayMicroseconds( 1);       // tCL=1000ns, tCDD=800ns
		}
	}
}

// --------------------------------------------------------
// _DS1302_toggleread
//
// A helper function for reading a byte with bit toggle
//
// This function assumes that the SCLK is still high.
//

uint8_t DS1302::toggleread( void)
{
	uint8_t i, data;
	data = 0;
	for ( i = 0; i <= 7; i++) {
		// Issue a clock pulse for the next databit.
		// If the 'togglewrite' function was used before
		// this function, the SCLK is already high.
		digitalWrite( this->sclk_pin, HIGH);
		delayMicroseconds( 1);

		// Clock down, data is ready after some time.
		digitalWrite( this->sclk_pin, LOW);
		delayMicroseconds( 1);        // tCL=1000ns, tCDD=800ns

		// read bit, and set it in place in 'data' variable
		bitWrite( data, i, digitalRead( this->io_pin));
	}
	return( data);
}

timedata DS1302::readtime()
{
	ds1302_data rtc;
	timedata result;
	this->burst_read( (uint8_t *) &rtc);
	result.hours = bcd2bin( rtc.h24.Hour10, rtc.h24.Hour);
	result.minutes = bcd2bin( rtc.Minutes10, rtc.Minutes);
	result.seconds = bcd2bin( rtc.Seconds10, rtc.Seconds);
	result.day = bcd2bin( rtc.Date10, rtc.Date);
	result.dow = rtc.Day;
	result.month = bcd2bin( rtc.Month10, rtc.Month);
	result.year = 2000 + bcd2bin( rtc.Year10, rtc.Year);
	return result;
}

void DS1302::writetime(timedata&td)
{
	ds1302_data rtc;
	memset ((char *) &rtc, 0, sizeof(rtc));

	rtc.Seconds    = bin2bcd_l( td.seconds);
	rtc.Seconds10  = bin2bcd_h( td.seconds);
	rtc.CH         = 0;      // 1 for Clock Halt, 0 to run;
	rtc.Minutes    = bin2bcd_l( td.minutes);
	rtc.Minutes10  = bin2bcd_h( td.minutes);
	// To use the 12 hour format,
	// use it like these four lines:
	//    rtc.h12.Hour   = bin2bcd_l( hours);
	//    rtc.h12.Hour10 = bin2bcd_h( hours);
	//    rtc.h12.AM_PM  = 0;     // AM = 0
	//    rtc.h12.hour_12_24 = 1; // 1 for 24 hour format
	rtc.h24.Hour   = bin2bcd_l( td.hours);
	rtc.h24.Hour10 = bin2bcd_h( td.hours);
	rtc.h24.hour_12_24 = 0; // 0 for 24 hour format
	rtc.Date       = bin2bcd_l( td.day);
	rtc.Date10     = bin2bcd_h( td.day);
	rtc.Month      = bin2bcd_l( td.month);
	rtc.Month10    = bin2bcd_h( td.month);
	rtc.Day        = td.dow;
	rtc.Year       = bin2bcd_l( td.year - 2000);
	rtc.Year10     = bin2bcd_h( td.year - 2000);
	rtc.WP = 0;

	// Write all clock data at once (burst mode).
	this->burst_write( (uint8_t *) &rtc);

}

static const unsigned int monthes[12] = {31,28, 31, 30,31,30,31,31,30,31,30,31};

unsigned long DS1302::getTimestamp(timedata td)
{
	unsigned long int ret = 0;
	int y = td.year - 2000;
	if (y < 0) return 0;
	ret += (unsigned long int)( y * 31536000UL);
	unsigned int days = 0;
	while(y > -1) {
		if ( y % 4 == 0) {
			++days;
		}
		--y;
	}

	y = td.month - 2;

	while (y > -1) {
		days += monthes[ y-- ];
	}
	days += (td.day - 1);

	ret += days * 86400UL;

	ret += (unsigned long int) ((td.hours * 60UL + td.minutes) * 60UL + td.seconds);
	return ret;
}

timedata DS1302::fromTimestamp(unsigned long int ts)
{
	timedata td;
	char i = 0;
	td.year = ts / 31536000UL;
	td.dow = 0;
	for(int i = td.year; i >-1; --i) {
		if(i%4==0) {
			ts -= 86400UL;
		}
	}
	ts -= (unsigned long int)td.year * 31536000UL;

	td.year += 2000;
	td.month = 0;
	while (ts > (unsigned long int)(monthes[ td.month ] * 24 * 3600UL)) {
		ts -= (unsigned long int)(monthes[ td.month ] * 24 * 3600UL);
		++td.month;
	}//while
	++td.month;
	td.day = ts / (24 * 3600UL);
	ts -= td.day * (24 * 3600UL);
	++td.day;
	td.hours = ts / 3600UL;
	ts -= td.hours * 3600UL;
	td.minutes = ts / 60;
	td.seconds = ts % 60;
	return td;
}

void DS1302::writeRAMbyte(uint8_t address, uint8_t data)
{
	if (address <0 || address > 31)
		return;
	this->write(0xC0|(address<<1), data);
}


uint8_t DS1302::readRAMbyte(uint8_t address)
{
	if (address <0 || address > 31)
		return 0;
	return this->read(0xC1|(address<<1));
}

void DS1302::setTrickleMode(uint8_t mode)
{
	this->write(DS1302_TRICKLE, mode);
}
uint8_t DS1302::getTrickleMode()
{
	return this->read(DS1302_TRICKLE_READ);
}
