#include <avr/interrupt.h>
#include <Arduino.h>
#include <WProgram.h>
#include "WaterServoEx.h"

#define 	ERR_NUMPOS_MISMATCH		0x01
#define 	ERR_MEMORY				0x02
#define		ERR_OUT_OF_RANGE		0x03
#define 	ERR_MISS				0x04
#define 	ERR_UNRECOV_MISS		0x05

#define WSST_ZERO			0
#define WSST_CALIBRATED		1
#define WSST_STOPPED		2
#define WSST_RUN			5
// extern HardwareSerial Serial;

WaterServoEx::WaterServoEx()
{
	state = WSST_ZERO;
	n_pos = 0;
// 	this->positions = NULL;
}

void WaterServoEx::resetError()
{
	this->errc = 0;
	this->state = WSST_ZERO;
}

void WaterServoEx::init(char pin, char reed, uint8_t _n_pos, int t0, int t1)
{
	this->n_pos = _n_pos;
	this->pin = pin;
	this->_t0=t0;
	this->_t1=t1;

// 	this->positions = (uint8_t*)malloc( _n_pos * sizeof(uint8_t));

	this->reed = reed;
	this->dev_id = 0;
	this->errc = 0;
	pinMode(this->reed, INPUT);
	digitalWrite(this->reed, LOW);
	srv.attach(this->pin);
	srv.write(0);
	delay(2000);
	srv.write(180);
	delay(2000);
	srv.detach();
//  	calibrate();
}

WaterServoEx::~WaterServoEx()
{
// 	free(this->positions);
}

void WaterServoEx::setDevId(uint8_t _dev_id)
{
	this->dev_id =_dev_id;

}
void WaterServoEx::calibrate(bool dbg)
{
	int  i=0, _state=0, min_delta = 6, n_angles = 0, n=0;
// 	dbg=true;
// 	Serial.flush();
 	dbg=true;
	srv.attach(this->pin);
	srv.write(0);
	delay(2000);
	srv.detach();
// 	dbg=true;
	for (i = 0; i <= 190; i+=2) {
		srv.attach(this->pin);
		srv.write(i);
		delay(200);
		srv.detach();//for debouncing servo.
		delay(300); // 200ms -- reed delay
// dbg=true;
// Serial.flush();
		if (digitalRead(this->reed) == HIGH) {
			if(_state == 0) {
				_state = 1;
				n = 1;
			} else {
				++n;
			}
		} else {
			if (_state == 1) {

				if (dbg) {
					Serial.print(n_angles, DEC);
					Serial.print(":");
					Serial.println(i, DEC);
				}
				this->positions[ n_angles++ ] = i;
				/*if (n_angles >=2 && (positions[ n_angles - 1 ] - positions[n_angles - 2]) < min_delta) {
					min_delta = (positions[ n_angles - 1 ] - positions[n_angles - 2]);
				}*/
				_state = 0;
				n = 0;
			}
		}//else
		if (this->n_pos < n_angles) {
			this->errc = ERR_NUMPOS_MISMATCH;
			this->state = WSST_STOPPED;
			/*if (dbg) {
				Serial.print("want ");
				Serial.print(this->n_pos, DEC);
				Serial.print(" found: ");
				Serial.println(n_angles, DEC);
			}*/
			delay(1000);
// 			Serial.println("overpos");
			return;
		}
	}//for i
// Serial.println("END FOR");
	if (_state == 1 && n > 0) {//cup at 180.
			if (dbg) {
				Serial.print(n_angles, DEC);
				Serial.print(" : ");
				Serial.println(i, DEC);
			}
		this->positions[ n_angles++ ] = i;
	}
// dbg=true;
// 	--n_angles;
// Serial.flush();
// Serial.print("min delta:");
// Serial.println(min_delta, DEC);
	if (this->n_pos != n_angles) {
		this->errc = ERR_NUMPOS_MISMATCH;
		this->state = WSST_STOPPED;
		if (dbg) {
			Serial.print("2:found ");
			Serial.print(n_angles, DEC);
			Serial.print(" want: ");
			Serial.println(this->n_pos, DEC);
		}
//		Serial.println("overpos");
		return;
	}
// 	dbg=true;
// 	Serial.flush();
// Serial.print("n=");
// Serial.println(n_angles, DEC);
	srv.attach(this->pin);
	srv.write(0);
	delay(1000);

	min_delta >>=1;
	if(min_delta <= 0) {
		min_delta = 5;
	}
// 	n = this->n_pos;
// 	if(dbg) {
// 		Serial.print("d:");
// 		Serial.println(min_delta, DEC);
// 	}
// 	Serial.flush();
	for (int tests = 0; tests < 3; ++tests) {
		n = 0;
// 		dbg=true;
		for (i = 0; i < n_angles; ++i) {
			srv.attach(this->pin);
			srv.write(positions[i]);
			delay(500);
			srv.detach();
			delay(300);
			if (digitalRead(this->reed) == LOW) {
				state = 1;
				for (int j = -min_delta; j <=min_delta; j+=2) {
					srv.attach(this->pin);
					srv.write(0/*positions[i] + min_delta*/);
					delay(500);
					srv.write(positions[i] + j);
					delay(500);
					srv.detach();
					delay(300);
					if ( digitalRead(this->reed) == HIGH) {
						positions[i] += j;
						if (dbg) {
							Serial.print(i, DEC);
							Serial.println(":=)");
						}
						_state = 0;
						break;
					} else {
						if (dbg) {
							Serial.print(i, DEC);
							Serial.println(":=(");
						}
						_state = 1;
					}
				}//for j
				n += _state;
			} else {//if LOW
				if (dbg) {
					Serial.print(i, DEC);
					Serial.println(":=)");
				}
			}
// 			Serial.flush();
			delay(1000);
		}//for i
		if (0 == n) {
			break;
		}
	}//for tests
	if (dbg) {
		Serial.print("n:");
		Serial.println(n, DEC);
	}
	if(n==0) {
		this->state = WSST_CALIBRATED|WSST_RUN;
		this->errc = 0;
		if (dbg) {
			Serial.println("ok");
		}
	} else {
		this->state = WSST_STOPPED;
		this->errc = ERR_UNRECOV_MISS;
		if (dbg) {
			Serial.println("fail");
		}
	}
Serial.flush();
}//sub

void WaterServoEx::moveTo(int n)
{
// 	Serial.print("Mov ");
// 	Serial.println(n, DEC);
// 	Serial.print("move ");
// 	Serial.print(n, DEC);
// 	Serial.print(" ");
// 	Serial.println(this->n_pos);
	if (n < 1 || n > this->n_pos/* || this->positions==NULL*/) {
// 		Serial.println("oor");
		this->errc = ERR_OUT_OF_RANGE;
		return;
	}
	if ((this->state & WSST_CALIBRATED) == 0) {
// 		Serial.println(__LINE__);
		this->calibrate(1);
	}
	if (this->state != (WSST_CALIBRATED|WSST_RUN)) {
// 		Serial.println(__LINE__);
		return;
	}
	srv.attach(this->pin);
	srv.write(this->positions[ n - 1 ]);
	delay(400); //turning
	srv.detach();
	delay(300);//waiting reed

	if (digitalRead(this->reed) != HIGH) {
		//Ooops! cup missed.recalibrating.
// 		Serial.println(__LINE__);
		//будем тупо полагать, что промазали только немножко...
		for (int i=-8;i < 8; i+=1) {
			srv.attach(this->pin);
			srv.write(this->positions[ n - 1 ] + i);
			delay(500); //turning
			srv.detach();
			delay(500);//waiting reed
			if (digitalRead(this->reed) == HIGH) {
				return;
			}
		}

		this->errc = ERR_MISS;
		int _old_state = this->state, errs = 0;
		this->state = WSST_ZERO;
		do {
// 			Serial.println(__LINE__);
			this->calibrate(1);
// 			Serial.println(__LINE__);
			this->moveTo(n);
// 			Serial.println(__LINE__);
		} while (digitalRead(this->reed) != HIGH && errs++ < 3);
		if (errs >=3) {
			this->errc = ERR_UNRECOV_MISS;
			this->state = WSST_STOPPED;
// 			Serial.println(__LINE__);
			return;
		}
		this->state |= WSST_RUN;

	}/* else {
		srv.attach(this->pin);
		srv.write(0);
		delay(500);
		srv.write(this->positions[ n-1 ]);
		delay(500);// pi/3 => 0.1s 0.5s=>5pi/3.
		srv.detach();
	}*/
}

bool WaterServoEx::isError()
{
	return this->errc;
}

uint8_t WaterServoEx::getError()
{
	return (this->dev_id<<4)|this->errc;
}
