#ifndef _WATER_SERVO_CLASS_INCLUDED_
#define _WATER_SERVO_CLASS_INCLUDED_

#include <Servo.h>
/**
 * WaterServo - class for controlling water-dispencer servo.
 * Hardware:
 * 	1.Servo like TowerPro SG-90 on phisical pin.
 * 2. reed switch between +Vcc and pin.
 * 3. 10K or more pull-down resistor between reed pin and GND.
 * 4. cups.
 * 5. CD/DVD disk and strong magnet for every cup.
 *
 * Licence: GPL, AS IS.
 *
 */
class WaterServoEx
{
	Servo srv;
	uint8_t pin, state, reed,dev_id, errc;
	uint8_t* positions;
    int _t0,_t1, n_pos;
protected:
	/**
	 * calibration. using reed to find out all available cups.storing all positions in this->positions for future use.
	 */

public:
	void calibrate();
	WaterServoEx();
	virtual ~WaterServoEx();
	void setDevId(uint8_t _dev_id);
	uint8_t getError();
	bool isError();
	/*
	 * pin for Servo, pin with reed switch, number of cups, t0 and t1 -- from Servo library.
	 */
	void init(char pin, char reed, uint8_t _n_pos, int t0=540, int t1=2400);
	/*
	 * moving to cup number #N(N=1 -- first cup, N=0 -- invalid!).
	 */
	void moveTo(int n);
};



#endif
