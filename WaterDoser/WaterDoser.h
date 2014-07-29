#ifndef _WATER_DOSER_H
#define _WATER_DOSER_H
/**
 * Licence: GPL, AS IS.
 *
	WaterDoser -- water dosation control.
	used hardware:
		1. pump, controlled by pump_pin(HIGH=on, LOW=off)
		2. Water Flow Sensor like FS100A with 3900 pulses/1L.
	Software requirements:
		1. free hardware interrupt(0, 1, etc).
*/


class WaterDoser
{
protected:
    int value, stop_value, pump_pin, dev_id, errc;
#ifdef _USE_TEMP_MON
	uint8_t temp_pin, max_temp;
#endif
    char state;
public:
    WaterDoser(int _pump_pin=-1,int _temp_pin=0);
    void resetError();
	void setDevId(uint8_t _dev_id);
	uint8_t getError();
#ifdef _USE_TEMP_MON
	// obsolete. pump temp control via thermoresistor.
	uint8_t getMaxTemp();
	void setMaxTemp(uint8_t val);
	uint8_t readTemperature();
	bool isOverheated();
#endif
	bool isError();
    char getState();
    uint16_t run(int ml = 30);
    void stop();
    void init(uint8_t interrupt=0);
	friend void water_doser_ticker(); //interrupt handler
};

#endif
