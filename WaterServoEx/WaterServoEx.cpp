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

#ifndef WSEX_DELTA
	#define WSEX_DELTA 8
#endif
// #define USE_PRECALIBRATE 	1
WaterServoEx::WaterServoEx()
{
	state = WSST_ZERO;
	n_pos = 0;
	this->last_pos = -1;
// 	this->positions = NULL;
}

void WaterServoEx::resetError()
{
	this->errc = 0;
	this->last_pos = -1;
	srv.attach(this->pin);
	srv.write(0);
	delay(500);
	srv.detach();
	this->state = WSST_CALIBRATED;
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
	__moveTo(180);
	delay(2000);
	__moveTo(0);
	delay(2000);

	last_pos = digitalRead(this->reed) == HIGH ? 0 : -1;
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
#ifdef USE_PRECALIBRATE
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
#else
	this->__moveTo(0);
	Serial.println("Zero");
	delay(1000);
	for (uint8_t i =1; i <= this->n_pos; ++i) {
		Serial.print("moving to ");
		Serial.println(i+1, DEC);
		this->moveTo(i);
		Serial.println("done");
		delay(1000);
	}
#endif
}//sub
//

void WaterServoEx::__moveTo(int angle)
{
	srv.attach(this->pin);
	srv.write(angle);
	delay(400); //turning
	srv.detach();
	delay(300);//waiting reed
// 	Serial.print(angle, DEC);
// 	Serial.print(" : ");
// 	Serial.println(digitalRead(this->reed), DEC);
}

void WaterServoEx::moveTo(int n)
{
	if (n < 1 || n > this->n_pos) {
		this->errc = ERR_OUT_OF_RANGE;
		return;
	}

#ifdef USE_PRECALIBRATE
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
			this->calibrate(1);
			this->moveTo(n);

		} while (digitalRead(this->reed) != HIGH && errs++ < 3);
		if (errs >=3) {
			this->errc = ERR_UNRECOV_MISS;
			this->state = WSST_STOPPED;
			return;
		}
		this->state |= WSST_RUN;

	}
#else
	int da=0, dn=0, a;
// 	if (n == this->n_pos) {
// 		__moveTo(180);
// 		this->last_pos = digitalRead(this->reed)==HIGH ? this->n_pos : this->n_pos+1;
// 	}
	--n;
 	Serial.print("mov to ");
 	Serial.println(n, DEC);
 	Serial.print("last pos:");
 	Serial.println(this->last_pos, DEC);

	if(n == this->last_pos) {
		if (digitalRead(this->reed) == HIGH) {
// 			Serial.println("hit!");
			return;
		}
		da = srv.read();
		for (dn = da-WSEX_DELTA; dn < da+WSEX_DELTA;dn+=2) {
			__moveTo(dn);
// 			srv.attach(this->pin);
// 			srv.write(dn);
// 			delay(200);
// 			srv.detach();
// 			delay(300);
			if (digitalRead(this->reed) == HIGH) {
// 				Serial.println("2:hit!");
				this->errc = 0;
				return;
			}
		}
// 		Serial.println("1:miss!");
		this->resetError();
		this->moveTo(n + 1);
	} else if (n < this->last_pos) {
		da = -2;
		dn = -1;
	} else if (n > this->last_pos) {
		da = 2;
		dn = 1;
	}

// 	Serial.print("searching ");
// 	Serial.print(da,DEC);
// 	Serial.print(" ");
// 	Serial.println(dn,DEC);

	a = srv.read();

	while (digitalRead(this->reed) == HIGH) {
		__moveTo(a);
		a += da;
	}//while

	while (a >=0 && a <= 200 && n != this->last_pos) {
// 		Serial.print("a=");
// 		Serial.println(a, DEC);
// 		Serial.print("current:");
// 		Serial.print(last_pos, DEC);
// 		Serial.print(" mov to:");
// 		Serial.println(n, DEC);
			__moveTo(a);
			if ((digitalRead(this->reed)) == HIGH) {
					this->last_pos += dn;
					if (this->last_pos == n) {
// 						Serial.println("FOUND!");
						return;
					}
// 					Serial.print("found ");
// 					Serial.print(last_pos, DEC);
					//move to next LOW reed pos
					while ((digitalRead(this->reed)==HIGH) && a >=0 && a<=200) {
						a+=da;
						__moveTo(a);
					}//while high
			}//if high
		a+=da;
	}//while
	this->errc = ERR_MISS;
	return;
#endif
}

bool WaterServoEx::isError()
{
	return this->errc;
}

uint8_t WaterServoEx::getError()
{
	return (this->dev_id<<4)|this->errc;
}
