#include<Arduino.h>
#include "WaterDoser.h"
#include <util/atomic.h>

#define ERR_WFS_NOSIGNAL 	1

const float water_multiplier = 3900./1000.;

//out digitalWrite wrapper function. we can use "software" pin via HC595.

extern void pin_write(uint8_t pin, char value, bool flush);


WaterDoser* cur_doser = NULL;

void water_doser_ticker()
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		if (!cur_doser) {
			return;
		}
		cur_doser->value++;
		if (cur_doser->value >= cur_doser->stop_value) {
			cur_doser->stop();
		}
	}
}

void WaterDoser::setDevId(uint8_t id)
{
	this->dev_id = id;
}

uint8_t WaterDoser::getError()
{
	return (this->dev_id<<4)|this->errc;

}

WaterDoser::WaterDoser(int _pump_pin,int _temp_pin)
{
	state=0;
	pump_pin = _pump_pin;
	value = stop_value = 0;
#ifdef _USE_TEMP_MON
	temp_pin = _temp_pin;
	this->setMaxTemp(48);
#endif
	this->resetError();
}

void WaterDoser::resetError()
{
	this->errc = 0;
}

#ifdef _USE_TEMP_MON
uint8_t WaterDoser::getMaxTemp()
{
	return this->max_temp;
}

void WaterDoser::setMaxTemp(uint8_t val)
{
	this->max_temp = val;
}
#endif

bool WaterDoser::isError()
{
	return this->errc;
}

char WaterDoser::getState()
{
	return state;
}
#ifdef _USE_TEMP_MON
uint8_t WaterDoser::readTemperature()
{
	double temp=0;
	if (this->temp_pin) {
		temp = log(((10240000/analogRead(this->temp_pin)) - 10000));
		temp = 1.0 / (0.001129148 + (0.000234125 * temp) + (0.0000000876741 * temp * temp * temp));
		temp = temp - 273.15;
	}
	return (uint8_t) round(temp);
}

bool WaterDoser::isOverheated() {
	return this->readTemperature() > this->max_temp;
}
#endif
int WaterDoser::run(int ml)
{
	unsigned long int start = millis();
	int prev_val = 0;
#ifdef _USE_TEMP_MON
	if(this->isOverheated()) {
		this->stop();
		return 0;
	}
#endif

	if (this->errc) {
		this->stop();
		return 0;
	}
	this->stop();
	this->value = 0;
	this->stop_value = ml * 4;//i'm a bit lazy and this class is not work in drag factory. let flow sensor sends 4000 pulses, not 3900.
	if (cur_doser) {
// 		Serial.println("FUCK! Doser inited");
		return 0;
	}
	cur_doser = this;

	pin_write(this->pump_pin, HIGH, true);
// 	Serial.print("run");
// 	Serial.println(this->dev_id, HEX);
	state = 1;

	while (this->state) {
		delay(500);
		if ((this->value - prev_val<=2) && (millis() - start > 4000UL)) {
			delay(200);
			if (!this->state) {
				break;
			}
			this->stop();
			errc = ERR_WFS_NOSIGNAL; // WFS is dead or no water left in tank.
// 			Serial.println("dbg");
// 			Serial.println(this->value, DEC);
// 			Serial.println(prev_val, DEC);
// 			Serial.println(millis() - start, DEC);
// 			char buf[80]={0};
// 			this->error_time = ds1302.getTimestamp(ds1302.readtime());
// 				Serial.print("NO WATER"
//  				sprintf(buf, "ERROR! %d - %d < 2, %ul - %ul = %ul > %ul. NO WATER",this->value, prev_val, now, start, now - start, 10000UL);
//  				Serial.println(buf);
			return round(this->value/water_multiplier);//return real value
		}
		prev_val = this->value;
	}

	this->stop();
	delay(500); // timeout for resetting water pressure.
	return round(this->value/water_multiplier);
}

void WaterDoser::stop()
{
	this->state = 0;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE){
		cur_doser = NULL;
		pin_write(this->pump_pin, LOW, true);
	}
// 	Serial.print("stop");
// 	Serial.println(this->dev_id, HEX);
}

void WaterDoser::init(uint8_t interrupt)
{
	//attaching our interrupt handler
	attachInterrupt(interrupt, water_doser_ticker, RISING);
	//turning OFF our pump
	pin_write(this->pump_pin, LOW, true);
}

