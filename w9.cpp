/**
	Atmega328 pot plant waterer.
	Hardware:
	1x atmega328
	1-2x pumps and aimilar count of water flow sensors like FS100A
	3x HC595 (or 2 if no LCD)
	1x LCD (not required)
	0-2x DS18b20+
	1-2 servos for dosing water
	1x bluetooth HC-0x for setup and monitoring
	1x AT24C128 or similar.
*/

// #define DEBUG 1

// #define TONE_DBG 1
#include <Servo.h>
#include <avr/wdt.h>
// #include <EEPROM.h>
#include <ShiftOut.h>
#include <OneWire.h>
#include <LiquidCrystal595Rus.h>
#include <WaterDoser.h>
#include <WaterServoEx.h>

#include <DS1302.h>
// #include <DHT.h>

#include <Wire.h>
/*
ATCxxx is compatible between different sizes.
 */
#include <AT24Cxxx.h>

#define CONFIG_MAGIC 0x42


#define BIG_ROOM 1

// #define MULTIDOSER 1
// #define _DEBUG	1

#define MINIMUN_VALID_VALUE		5

#define VIO_BASE 30
#define DS1302_CLK			5	//*
#define DS1302_IO			6	//*
#define DS1302_CE			7	//*
#define SHIFT_DATA			A0	//*
#define SHIFT_LATCH 		A1	//*
#define SHIFT_CLOCK			A2	//*
#define AIN_PIN				A3	//*

/*
	we use 1 HC595 to control two CD4067.
	so, we have 8 outputs and 2*5 control inputs.
	OK, use some outputs twice. we never use CD4067 simultaneously, so we can
	re-use any pins of 595, that not connected to INH inputs of CD4067.
*/
#define VIO_4067_1_A		(VIO_BASE+12)
#define VIO_4067_1_B		(VIO_BASE+11)
#define VIO_4067_1_C		(VIO_BASE+9)
#define VIO_4067_1_D		(VIO_BASE+8)
#define VIO_4067_1_INH		(VIO_BASE+10)

#define VIO_4067_2_A		(VIO_BASE+14)
#define VIO_4067_2_B		(VIO_BASE+15)
#define VIO_4067_2_C		(VIO_BASE+12)
#define VIO_4067_2_D		(VIO_BASE+11)
#define VIO_4067_2_INH		(VIO_BASE+13)


#define VIO_AIN_RELAY	 ((VIO_BASE) + 1) //*

#define SENSOR_RELAY_PIN	VIO_AIN_RELAY

#ifdef BIG_ROOM
	#define MULTIDOSER 1

	#define DOSERS	2
	#define PUMPS	2

	#define VIO_PUMP1	 	 ((VIO_BASE) + 2)	//*
	#define PUMP_PIN			VIO_PUMP1

	#define USE_1WIRE	1
	#define USE_LCD		1
	// #define _USE_LIGHT_SENSOR

	#define PUMP1_COUNTER_PIN	2 //interrupt 0
	#define PUMP2_COUNTER_PIN	3 //interrupt 1

	#ifdef USE_1WIRE
		#define ONE_WIRE_PIN 		4	//*
	#endif


	#define WATER_SERVO1_PIN	8	//*
	#define WATER_SERVO2_PIN	9	//*
	#define REED_2				12 	//*

	// #define DHT_PIN				10 	//

	#define REED_1				13	//*



	// #define LOG_SERIAL_PIN_RX	A4
	// #define LOG_SERIAL_PIN_TX	A0	//bad
	#ifdef _USE_LIGHT_SENSOR
		#define LUX_METER_PIN		A0 //bad
	#endif

// 	#define SENSOR_RELAY_PIN	VIO_AIN_RELAY

	// #define PUMP_TEMP_PIN		3 // analog!!

	#define SHIFT_OUTS	3

	/*	HC595:
	*
		first 0..7
		second 8..15
		third 16..23
	*/

	#define VIO_PUMP2	 	 ((VIO_BASE) + 3) //*

// 	#define PUMP_PIN			VIO_PUMP1
	#define PUMP_PIN2			VIO_PUMP2

	//without VIO_BASE because it uses via shiftOut
	#define VIO_LCD_RS_PIN (16)
	#define VIO_LCD_RW_PIN (17)
	#define VIO_LCD_E_PIN (18)
	#define VIO_LCD_D4_PIN (19)
	#define VIO_LCD_D5_PIN (20)
	#define VIO_LCD_D6_PIN (21)
	#define VIO_LCD_D7_PIN (22)

	//direct use -- with VIO_BASE
	#define VIO_ERROR_LED_PIN ((VIO_BASE)+23)

// light bulb relay pin
#define VIO_LIGHT_BULB_PIN	(VIO_BASE+7)

	#define LCD_TIME_LINE		0
	#define LCD_OUT_TEMP_LINE	1
	#define LCD_STATE_LINE		2
	#define LCD_ERROR_LINE		3
	// #define LCD_INDOOR_STATE_LINE	3
	#define LCD_THERMO_LINE			1

	#define CD4067_COUNT		2


#else
// 	#define MULTIDOSER 1

	#define DOSERS	1
	#define PUMPS	1
	#define SHIFT_OUTS	2
	#define VIO_PUMP1	 	 ((VIO_BASE) + 3)	//*
	#define PUMP_PIN		VIO_PUMP1
// 	#define USE_1WIRE	1
// 	#define USE_LCD		1
	// #define _USE_LIGHT_SENSOR


	#define PUMP1_COUNTER_PIN	2 //interrupt 0
// 	#define PUMP2_COUNTER_PIN	3 //interrupt 1

	#define DS1302_CLK			5	//*
	#define DS1302_IO			6	//*
	#define DS1302_CE			7	//*
	#define WATER_SERVO1_PIN	8	//*
	#define REED_2				12 	//*

	#define REED_1				10	//*



	/*	HC595:
	*
		first 0..7
		second 8..15
		third 16..23
	*/

// 	#define PUMP_PIN2			VIO_PUMP2

	#define CD4067_COUNT		2

#endif

#define MAX_POTS	(CD4067_COUNT*16)

#define BT_BAUD		38400



#define PULL_DOWNS 0
// ShiftOut(uint8_t latchPin, uint8_t clockPin, uint8_t dataPin, uint8_t registersCount);
#define FASTADC 1

// defines for setting and clearing register bits
#ifndef cbi
	#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
	#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define MODE_RUN	1
#define IS_RUN(x) ( (x) & MODE_RUN)
#define USE_CLOCK(x) ( (x) & 0x02)
#define SET_USE_CLOCK(x) ( (x) | 0x02)
#define SET_USE_TIMEOUT(x) ( (x) & 0xFD)

#define SPRAY_ON config.flags |= 0x04;
#define SPRAY_OFF config.flags &= ~0x04;

#define IS_SPRAY_ON		(config.flags & 0x04)
#define SET_ERRORS		(config.flags | 0x08)
#define UNSET_ERRORS	(config.flags & (~0x08))
#define GET_ERRORS		(config.flags & 0x08)
#define PIN(x) ( (x) & 0x0F)  // 4 bits (vals 0..15)
#define CHIP(x) ( ((x)>>4) & 0x03) //2 bits, 0..3
#define DOSER(x) ( ((x)>>6) & 0x03) //2 bits, 0..3
#define PACK_SENSOR(chip, pin, doser) ( ( ((doser) & 0x03)<<6) | (( (chip)&0x03)<<4)| ((pin) & 0x0F ) )

#define NEEDS_WATERING(x) (pot.flags & 0x01)

#define SET_WATERED(x) pot.flags &= 0xFE;

#define START_WATERING(x) pot.flags |= 0x01;

#define SET_WATERING(x, y) if(y) {START_WATERING(x);} else {SET_WATERED(x);}

#define SET_INTERVAL_DRYING(x) 	pot.flags |= 0x02;
#define SET_INTERVAL_WATERING(x)   pot.flags &= 0xFD;
#define IS_DRYING_INTERVAL(x) ( (pot.flags & 0x02) ? 1 : 0 )


#define AIN_PULLDOWN(x) ( ( (x) >> 1) & 0x03)

#undef PROGMEM

#define PROGMEM __attribute__((section(".progmem.data")))



/*
	особо экономить эту память бессмысленно, т.к. у нас 16КБ на конфиг максимум в 32 элемента по меньше, чем 64 байта.
	структура конфига:
	страница							данные
	0									magic
	1									global config
	2									daily watering data
	POTS_CONFIG_START+PAGE_ALIGN*i  	pot[i]
*/

#define PAGE_ALIGN	64
#define POTS_CONFIG_START	(PAGE_ALIGN * 6)
#define DAILY_WATER_DATA_START (PAGE_ALIGN * 2)

AT24Cxxx mem(80);



const char *VER[3] = {__DATE__, __TIME__, "w9r16.cpp"};

ShiftOut extPins(SHIFT_LATCH, SHIFT_CLOCK, SHIFT_DATA, SHIFT_OUTS);

#ifdef USE_LCD
	LiquidCrystal595Rus lcd(&extPins, VIO_LCD_RS_PIN, VIO_LCD_RW_PIN, VIO_LCD_E_PIN, VIO_LCD_D4_PIN, VIO_LCD_D5_PIN, VIO_LCD_D6_PIN, VIO_LCD_D7_PIN);
#endif

DS1302 ds1302(DS1302_CE, DS1302_CLK, DS1302_IO);

char buf[60]={0}, str[40]={0};

// uint8_t WFS[ PUMPS ] = {VIO_WF_SENSOR_1, VIO_WF_SENSOR_2};

#ifdef BIG_ROOM

#ifdef USE_1WIRE

#define MAX_DS18B20_SENSORS 2
#define INDOOR_TERMO	1
#define OUTDOOR_TERMO	0

uint8_t thermo_addrs[MAX_DS18B20_SENSORS][8] = {{40,89,110,160,4,0,0,34},{40,181,213,159,4,0,0,155}};
#endif

#ifdef USE_1WIRE
	OneWire ds(ONE_WIRE_PIN);
#endif

#endif

#define EXTRACT_STRING(x) strcpy_P(str, (char*)(x));

timedata last_analize, last_spray={0};

void _digitalWrite(uint8_t pin, uint8_t state)
{
  if (state) {
	if(pin < 2) {
		return;
	} else if (pin <= 7) {
		PORTD |= (1<<pin);
	} else if (pin <= 13) {
		PORTB |=  1 << (pin - 8);
	} else if ( pin >=A0 && pin <=A5) {
		PORTC |= 1<<(pin - A0);
	}
  } else {
	if ( pin < 2) {
		return;
	} else if (pin <= 7) {
		PORTD &= ~(1<<pin);
	} else if (pin <= 13) {
		PORTB &=  ~(1 << (pin - 8));
	} else if ( pin >=A0 && pin <=A5) {
		PORTC &= ~(1<<(pin - A0));
	}
  }
}

struct GlobalWaterConfig
{
	uint8_t flags;
	uint8_t pause_in_minutes;
	uint8_t night_from_wd;
	uint8_t night_to_wd;
	uint8_t night_from_we;
	uint8_t night_to_we;
	uint8_t sensors_count;
	uint8_t spray_length;//obsolete
	uint8_t light_sensor_pin;
	uint8_t light_sensor_min;
	uint8_t light_sensor_timeout;
};

struct plantdata{
  uint8_t sensor;
  uint8_t flags;
//   char needs_watering;
  uint8_t bowl_index; // номер стаканчика на раздаточном уровне
  uint8_t portion;//кол-во воды для разового полива 255 max!!
  uint8_t lbound;// нижний порог срабатывания сенсора
  uint8_t rbound;//верхний порог срабатывания сенсора
  uint16_t day_max;
  unsigned long int last_time;
  uint16_t watered;
  char name[16];
};

plantdata pot = {0};

GlobalWaterConfig config;

void printTime2buf(timedata& td, char with_seconds = 1,char lang = 0)
{
	static const char*days[2][7] = {{"Mo","Tu","We","Th", "Fr","Sa","Su"}, {"Пн","Вт","Ср","Чт","Пт","Сб","Вс"}};
	static const char time_with_seconds_fmt[] PROGMEM = "%02d:%02d:%02d %s %02d.%02d";
	static const char time_for_lcd_fmt[] PROGMEM = "%02d%s%02d %s %02d.%02d";
	if (with_seconds) {
		EXTRACT_STRING(time_with_seconds_fmt);
		sprintf(buf, str,
			td.hours,td.minutes,td.seconds,
			days[ lang ][max(0, td.dow - 1)],
			td.day,td.month
		);
	} else {
		EXTRACT_STRING(time_for_lcd_fmt);
		sprintf(buf, str,
			td.hours,(td.seconds & 1)?":":" ",td.minutes,
			days[ lang ][max(0, td.dow - 1)],
			td.day,td.month
		);
	}
}

void pin_write(uint8_t pin, char value, bool flush=true)
{
	if(pin <= 3) return;//0 & 1 --serial, 2 -- wf sensor, 3 -- wf sensor or stm like.
	if (pin < VIO_BASE) {
		_digitalWrite(pin, value);
	} else {
		extPins.setData(pin - VIO_BASE, value);
		if (flush) {
			extPins.sendData();
		}
	}
}

class CD4067
{
	uint8_t addr[4], ain;
	uint8_t en;
public:
	CD4067(uint8_t EN, uint8_t S0, uint8_t S1, uint8_t S2,uint8_t S3, uint8_t ain)
	{
		addr[0]=S0;
		addr[1]=S1;
		addr[2]=S2;
		addr[3]=S3;
		this->en = EN;
		this->ain = ain;
	}

	void init()
	{
		for (char i=0;i < 4;++i) {
			if (addr[i]< VIO_BASE) {
				pinMode(addr[i], OUTPUT);
			}
		}
		pinMode(ain, INPUT);
		pin_write(this->en, HIGH, true);
	}

	void on(uint8_t pin)
	{
		pin = pin & 0x0F;
		pin_write(this->en, HIGH, true);
		for (char i =0; i < 4; ++i) {
			pin_write(addr[ i ], (pin & (1<<i))?HIGH:LOW, true);
		}
		pin_write(this->en, LOW, true);
	}
	void off()
	{
		pin_write(this->en, HIGH, true);
	}
	int readFull(uint8_t pin)
	{
		int ret = 0;
		pin = pin & 0x0F;
		for (char i =0; i < 4; ++i) {
			pin_write(addr[ i ], (pin & (1<<i))?HIGH:LOW, true);
		}
		pin_write(this->en, LOW, true);
			delayMicroseconds(20);
			ret = analogRead(this->ain);
			delayMicroseconds(20);
			ret +=analogRead(this->ain);
		pin_write(this->en, HIGH, true);
		return (ret>>1);
	}
	uint8_t read(uint8_t pin)
	{
		return (this->readFull(pin)>>2);
	}
};


CD4067 cd4067[ CD4067_COUNT ]= {
#if CD4067_COUNT > 0
	CD4067(VIO_4067_1_INH, VIO_4067_1_A, VIO_4067_1_B, VIO_4067_1_C, VIO_4067_1_D, AIN_PIN)
#endif
#if CD4067_COUNT > 1
	, CD4067(VIO_4067_2_INH, VIO_4067_2_A, VIO_4067_2_B, VIO_4067_2_C, VIO_4067_2_D, AIN_PIN)
#endif
};

#if DEBUG
extern unsigned int __heap_start;
extern void *__brkval;
/* The head of the free list structure */
extern struct __freelist *__flp;

	/*
	* The free list structure as maintained by the
	* avr-libc memory allocation routines.
	*/
struct __freelist {
	size_t sz;
	struct __freelist *nx;
};

	/* Calculates the size of the free list */
int freeListSize() {
	struct __freelist* current;
	int total = 0;

	for (current = __flp; current; current = current->nx) {
		total += 2; /* Add two bytes for the memory block's header  */
		total += (int) current->sz;
	}

	return total;
}

int freeMemory() {
	int free_memory;
	if ((int)__brkval == 0) {
		free_memory = ((int)&free_memory) - ((int)&__heap_start);
	} else {
		free_memory = ((int)&free_memory) - ((int)__brkval);
		free_memory += freeListSize();
	}
	return free_memory;
}

#endif


WaterDoser wd[ DOSERS ] = {
	WaterDoser(PUMP_PIN, 0)
#if DOSERS > 1
	, WaterDoser(PUMP_PIN2, 0)
#endif
};

WaterServoEx ws[ DOSERS ] = {WaterServoEx()
#if DOSERS > 1
,WaterServoEx()
#endif
};


class PotsHolder{
protected:
	AT24Cxxx*memory;
public:
	PotsHolder(AT24Cxxx*mem){
		this->memory = mem;
	}
	void readPotName(uint8_t index, char*buf)
	{
		memset(buf, 0, 16);
		this->memory->readBuffer(POTS_CONFIG_START + PAGE_ALIGN * index + sizeof(plantdata), buf, 16 );
	}

	void writePotName(uint8_t index, char*name)
	{
		this->memory->writeBuffer(POTS_CONFIG_START + PAGE_ALIGN * index + sizeof(plantdata), name, 16 );
	}

	void readPot(int8_t index, plantdata*pot)
	{
		memset(pot, 0, sizeof(plantdata));
		this->memory->readBuffer(POTS_CONFIG_START + PAGE_ALIGN * index, (char*)pot, sizeof(plantdata));
	}

	void writePot(int8_t index, plantdata*pot)
	{
		this->memory->writeBuffer(POTS_CONFIG_START + PAGE_ALIGN * index, (char*)pot, sizeof(plantdata));
	}
};

PotsHolder pots_holder(&mem);


void saveConfig()
{
	char i = 0;

	mem.write(0, CONFIG_MAGIC);
	mem.writeBuffer(PAGE_ALIGN, (char*)&config, sizeof(config));

	i = 0;
}


void readConfig()
{
	char i = 0;
static const char bad_cfg[] = "bad cfg";
	if (mem.read(0) != CONFIG_MAGIC) {
		config.sensors_count = 0;
		config.flags &= ~MODE_RUN;
		Serial.println(bad_cfg);
		return;
	}
	mem.readBuffer(PAGE_ALIGN, (char*)&config, sizeof(config));

// 	mem.readBuffer(PAGE_ALIGN * 2, (char*)&positions, sizeof(positions));
// 	mem.readBuffer(PAGE_ALIGN * 3, (char*)&positions2, sizeof(positions2));

	if(config.sensors_count > MAX_POTS) {
		config.sensors_count = 0;
		config.flags &= ~MODE_RUN;
		Serial.println(bad_cfg);
		return;
	}

	if (config.night_from_wd > 23) config.night_from_wd = 22;
		if (config.night_from_we > 23) config.night_from_we = 22;
		if (config.night_to_wd > 12) config.night_to_wd = 8;
		if (config.night_to_we > 12) config.night_to_we = 9;
		if (config.spray_length > 20) {
			config.spray_length = 0;
			SPRAY_OFF;
	}

	while (i < config.sensors_count) {
// 		mem.readBuffer(POTS_CONFIG_START + PAGE_ALIGN * i, (char*)&pot, sizeof(plantdata));
		pots_holder.readPot(i, &pot);
		SET_INTERVAL_WATERING(i);
		pots_holder.writePot(i, &pot);
		++i;
	}
}

void printPotConfig(char i)
{
	// strcpy_P(str, (char*)pgm_read_word(time_with_second_fmt))
	static const char fmt1[] PROGMEM = "%d: %d/%d doser %d bowl %d";
	static const char fmt2[] PROGMEM = " dose %u [%d..%d] daymax %d";
	pots_holder.readPot(i, &pot);
	EXTRACT_STRING(fmt1);
	sprintf(buf, str,
			i,
			PIN(pot.sensor),
			CHIP(pot.sensor)+1,
			DOSER(pot.sensor),
			pot.bowl_index
	);
	Serial.print(buf);
	EXTRACT_STRING(fmt2);
	sprintf(buf, str,
			pot.portion,
			pot.lbound,
			pot.rbound,
			pot.day_max
	);
	Serial.print(buf);
	Serial.print(" ");
// 	readPotName(i, buf);
	Serial.println(pot.name);
	Serial.flush();
	delay(200);
}

void printConfig()
{
	char i = 0;
	printGlobalConfig();
	while (i < config.sensors_count) {
		printPotConfig(i);
		++i;
	}
}

void printGlobalConfig()
{
	static const char fmt_1[] PROGMEM = "pause: %d\nRun: %X";
	static const char nights_fmt[] PROGMEM = "Night: wd: %d-%d we: %d-%d";
	static const char sensors_fmt[] PROGMEM = "sensors: %d";
	EXTRACT_STRING(fmt_1);
	sprintf(buf, str, config.pause_in_minutes, config.flags);
	Serial.println(buf);

	EXTRACT_STRING(nights_fmt);
	sprintf(buf, str,
				config.night_from_wd, config.night_to_wd,
				config.night_from_we, config.night_to_we
		   );
	Serial.println(buf);
	EXTRACT_STRING(sensors_fmt);
	sprintf(buf, str, config.sensors_count);
	Serial.flush();
	delay(500);
}

void statistic()
{
	char i = 0;
	static const char  stat_p1[] PROGMEM = "%d %d/%d watered at ";
	static const char  stat_p2[] PROGMEM = " bowl #%d/%d dose:%d / %d state %s";
	static const char  stat_pump[] PROGMEM = "pump[%d]: %d";
	static const char  last_test[] PROGMEM = "Last test:";
#ifdef _USE_LIGHT_SENSOR
	static const char  light[] PROGMEM = "Light sensor value: %d";
#endif
    timedata td;
// 	Serial.println("== statistic ==");
	td = ds1302.readtime();
	printTime2buf(td);
	Serial.println(buf);
	sprintf(buf, "state:%x", config.flags);
	Serial.println(buf);
	Serial.flush();
	for(i=0;i<DOSERS; ++i) {
		Serial.print(ws[i].getError(), HEX);
		Serial.print(",");
		Serial.print(wd[i].getError(), HEX);
		Serial.print(" ");
	}

#ifdef _USE_LIGHT_SENSOR
	EXTRACT_STRING(light);
	sprintf(buf, str, analogRead(LUX_METER_PIN)>>2);
	Serial.println(buf);
	Serial.flush();
#endif
	for(char i=0;i<2;++i) {
#ifdef _USE_TEMP_MON
		sprintf(buf, "pump: %d, temp=%d",wd[i].isError(), wd[i].readTemperature());
#else
		EXTRACT_STRING(stat_pump);
		sprintf(buf, str, i, wd[i].isError());
#endif
		Serial.println(buf);
	}
	Serial.flush();
	EXTRACT_STRING(last_test);
	Serial.print(str);
	printTime2buf(last_analize);
	Serial.println(buf);
	Serial.flush();

	while (i < config.sensors_count) {
		EXTRACT_STRING(stat_p1);
		pots_holder.readPot(i, &pot);
		sprintf(buf, str,
			i,
			PIN(pot.sensor),
			CHIP(pot.sensor)+1
		);
		Serial.print(buf);
		td = ds1302.fromTimestamp(pot.last_time);
		printTime2buf(td);
		Serial.print(buf);
		EXTRACT_STRING(stat_p2);
		sprintf(buf, str,
					pot.bowl_index,
					DOSER(pot.sensor),
					pot.watered,
					pot.day_max,
					IS_DRYING_INTERVAL(i)?"dry ":"water "
			   );
		Serial.print(buf);
// 		readPotName(i, buf);
		Serial.println(pot.name);
		Serial.flush();
		++i;
	}//while
	Serial.println();
}

int last_sensors_fails = 0;

int readSensor(int i, uint8_t debug=0)
{
	static const char  bad_value_debug[] PROGMEM = "BAD VALUE: %d ";
	static const char debug_fmt[] PROGMEM = "%d %d/%d bowl %d/%d val %d/%d..%d should:%d(%d) ";
	int val;
	pots_holder.readPot(i, &pot);
	val = cd4067[ CHIP(pot.sensor) ].read(PIN(pot.sensor) );
	//датчик убился -- не слушаем его
	if (val < MINIMUN_VALID_VALUE && debug!=2) {
		++last_sensors_fails;
		EXTRACT_STRING(bad_value_debug);
		sprintf(buf, str, val);
		Serial.print(buf);
// 		readPotName(i, buf);
		Serial.println(pot.name);
		SET_WATERED(i);
		pots_holder.writePot(i, &pot);
// 		pot.needs_watering = false;
		return 0;
	}

	if (val >= pot.rbound) {
		SET_INTERVAL_DRYING(i);
		SET_WATERING(i, 0);
	} else if (val <= pot.lbound) {
		SET_INTERVAL_WATERING(i);
		SET_WATERING(i, 1);
	} else {
		/**
		* если суше, чем минимально допустимый порог, то поливаем всегда.
		* иначе же, если мы на стадии "проливаем" и не долили до допустимого максимума, то тоже надо поливать.
		*/
		SET_WATERING(i, ( ( IS_DRYING_INTERVAL(i)) ? 0 :1));
	}
	pots_holder.writePot(i, &pot);
	if (debug==1) {
		EXTRACT_STRING(debug_fmt);
		sprintf(buf, str,
					i,
					PIN(pot.sensor),
					CHIP(pot.sensor) + 1,
					pot.bowl_index,
					DOSER(pot.sensor),
					val,
					pot.lbound,
					pot.rbound,
					NEEDS_WATERING(i),
					1 - IS_DRYING_INTERVAL(i)
		);
		Serial.print(buf);
// 		readPotName(i, buf);
		Serial.println(pot.name);
	} else if (debug == 2) {
		sprintf(buf, "%d %d %d %d %d;",
					PIN(pot.sensor),
					CHIP(pot.sensor) + 1,
					val,
					NEEDS_WATERING(i),
					IS_DRYING_INTERVAL(i)
		);
 		Serial.println(buf);
	}
	return val;
}

void analize(char index=-1, uint8_t debug=0)
{
	int8_t i = 0;
// 	static uint8_t vals[MAX_POTS] = {0};
    pin_write(SENSOR_RELAY_PIN, HIGH);
    delay(600);
	last_sensors_fails = 0;
	if(index == -1) {
		if (!debug) { /* тесты и донастройки не должны сбивать таймер, 12мая*/
			last_analize = ds1302.readtime();
		}

		while (i < config.sensors_count) {
			readSensor(i, debug);
			++i;
		}//while

		pin_write(SENSOR_RELAY_PIN, LOW);
	} else {
		readSensor(i, true);
		pin_write(SENSOR_RELAY_PIN, LOW);
	}
	if (debug == 2) {
		Serial.println(";;");
	}
}

void water2Bowl(int i)
{
	timedata td = ds1302.readtime();
	pots_holder.readPot(i, &pot);
	if (td.day != ds1302.fromTimestamp(pot.last_time).day) {
		pot.watered = 0;
	}
	if (pot.watered > pot.day_max) {
//  		sprintf(buf, "%i max=(", i);
//  		Serial.println(buf);
		SET_WATERED(i);
		pots_holder.writePot(i, &pot);
		return;
	}
	uint8_t doser = DOSER(pot.sensor);
// 	pin_write( WFS[ doser ], 1, 1);
	Serial.print("move to:");
	Serial.println(pot.bowl_index, DEC);
	ws[ doser ].moveTo(pot.bowl_index);
	if (ws[ doser ].isError()) {
		showErrors();
		pots_holder.writePot(i, &pot);
		return;
	}
	pot.watered += wd[ doser ].run(pot.portion);
// 	pin_write( WFS[ doser ], 0, 1);
	SET_WATERED(i);
	pot.last_time = ds1302.getTimestamp(td);
	pots_holder.writePot(i, &pot);
}

void water()
{
// 	timedata td;
	char i = 0;

	while ( i < config.sensors_count) {
		pots_holder.readPot(i, &pot);
		if ( !wd[ DOSER(pot.sensor) ].isError() && NEEDS_WATERING(i) ) {
			water2Bowl(i);
		} else {
			if (wd[ DOSER(pot.sensor) ].isError()) {
// 				sprintf(buf, "%d pump error", i);
			} else if (!NEEDS_WATERING(i)) {
// 				sprintf(buf, "%d: no watering flag", i);
			}
			Serial.println(buf);
		}//if needs_watering
		++i;
		showErrors();
	}//while watering
// 	ws[0].standby();
// 	ws[1].standby();
	saveDayData();
	showErrors();
// 	Serial.println(829, DEC);
#ifdef USE_LCD
	lcd.clear();
#endif
// 	Serial.println("eow");
}


bool isNight(timedata td)
{
	if (td.dow < 6) {
		return (td.hours >= config.night_from_wd || td.hours <= config.night_to_wd);
	} else
		return (td.hours >= config.night_from_we || td.hours <= config.night_to_we);
}

int minutes_diff(timedata& t0, timedata& t1)
{
	char dh = t1.hours - t0.hours;
	if (dh < 0) dh += 24;
	return dh * 60 + (t1.minutes - t0.minutes);
}

void saveDayData()
{
	mem.writeBuffer(DAILY_WATER_DATA_START, (char*)&last_analize, sizeof(last_analize));
#if 0
	//obsolete. we can write all in flash memory.
	timedata td = ds1302.readtime();
	ds1302.writeRAMbyte(0, td.day);
	ds1302.writeRAMbyte(1, config.sensors_count);
	for (char i = 0; i < config.sensors_count; ++i) {
		pots_holder.readPot(i, &pot);
		ds1302.writeRAMbyte( 2 + i, map(pot.watered, 0, 1024, 0, 255));
	}
#endif
}

void readDayData()
{
	mem.readBuffer(DAILY_WATER_DATA_START, (char*)&last_analize, sizeof(last_analize));
#if 0
//obsolete. we store all data in flash.
	timedata td = ds1302.readtime();
	for(char i = 0;i < config.sensors_count; ++i) {
		pots_holder.readPot(i, &pot);
		pot.watered = 0;
		pots_holder.writePot(i, &pot);
	}
	if (td.day == ds1302.readRAMbyte(0)) {
		for(char i=0;i<config.sensors_count; ++i) {
			pots_holder.readPot(i, &pot);
			pot.watered = map(ds1302.readRAMbyte( 2 + i), 0, 255, 0, 1024);
			pots_holder.writePot(i, &pot);
		}
	}
#endif
}

void setOption(char* cmd)
{
	timedata td;
#ifdef EXT_COMMANDS
	static const char TWO_INTS_FMT_STR[] PROGMEM="%d %d";
#endif
// 	static const char THREE_INTS_FMT_STR[] PROGMEM="%d %d %d";
	static const char time_read_fmt[] PROGMEM = "%d:%d:%d %d.%d %d";
	static const char data_fmt[] PROGMEM = "%d %d %d %d %d %d %d %d %d";
// 	static const char nights[] PROGMEM = "nights: work day %d-%d, weekend %d-%d";
// 	static const char stop_msg[] PROGMEM = "Stopped.\nSensors count=";
	static const char FOUR_INTS_FMT_STR[] PROGMEM = "%d %d %d %d";
	if (!strncmp(cmd, "light", 5)) {
		int  a,b,c,d;
		EXTRACT_STRING(FOUR_INTS_FMT_STR);
		sscanf(cmd+6, str, &a, &b, &c, &d);
		config.light_sensor_pin = (a<<4) | (b & 0x0F);
		config.light_sensor_min = c;
		config.light_sensor_timeout = d;
	} else if (!strncmp("name", cmd, 4)) {
		uint8_t index = (cmd[5]-'0') * 10 + (cmd[6]-'0');
		Serial.print("index:");
		Serial.println(index, DEC);
		pots_holder.readPot(index, &pot);
		strncpy(pot.name, cmd+8, 16);
		pots_holder.writePot(index, &pot);
// 		readPotName(index, str);
		Serial.println("OK");
// 		Serial.println(str);
	} else if (!strncmp("err", cmd, 3)) {
		Serial.println(config.flags, HEX);
		if(cmd[4]=='1'){
			Serial.println("set");
			config.flags = SET_ERRORS;
		} else {
			Serial.println("unset");
			config.flags = UNSET_ERRORS;
		}
		Serial.println(config.flags, HEX);
	} /*else if (!strncmp("spray", cmd, 5)) {
		config.spray_length = atoi(cmd+6);
		if (config.spray_length) {
			SPRAY_ON;
		} else {
			SPRAY_OFF;
		}
	} */else if (!strncmp("aw", cmd, 2)) {
		char i=0;
		while(i < config.sensors_count) {
			pots_holder.readPot(i, &pot);
			SET_INTERVAL_WATERING(i);
			pots_holder.writePot(i, &pot);
			++i;
		}
	} else if (!strncmp("time", cmd, 4)) {
		int dow=1;
		EXTRACT_STRING(time_read_fmt);
		sscanf(cmd + 5, str, &td.hours, &td.minutes, &td.seconds, &td.day, &td.month, &dow);
		td.dow = dow;
		td.year = 2014;
		ds1302.writetime(td);
// 		printTime2buf(td);
// 		Serial.println(buf);
// 		td = ds1302.readtime();
// 		Serial.print("Set time: ");
// 		printTime2buf(td);
// 		Serial.println(buf);
	} else if (!strncmp("data ", cmd, 5)) {
		int index=0, a=0,b=0,c=0,d=0,e=0, f=0, g=0, h=0;
		EXTRACT_STRING(data_fmt);
		sscanf(cmd+5, str, &index, &a, &b, &c, &d, &e, &f, &g, &h);
		pot.sensor = PACK_SENSOR(a, b, c);//.chip_index = a;
		pot.bowl_index = d;
		pot.portion = e;
		pot.lbound = f;
		pot.rbound = g;
		pot.day_max = h;
// 		pot.name[0] = 0;
		pots_holder.writePot(index, &pot);
		Serial.println("OK");
		printPotConfig(index);
	}
#ifdef EXT_COMMANDS
	else if (!strncmp(cmd, "sensor ", 7)) {
		int index, tmp;
		sscanf(cmd + 8, TWO_INTS_FMT_STR, &index, &tmp);
		pots[ index ].sensor = PACK_SENSOR(0, CHIP(pots[ index ].sensor), tmp);
		printPotConfig(index);
	} else
#endif
	if (!strncmp("night ",cmd, 6)) {
		int a,b,c,d;
		EXTRACT_STRING(FOUR_INTS_FMT_STR);
		sscanf(cmd+6, str, &a, &b, &c, &d);
		config.night_from_wd=a;
		config.night_to_wd=b;
		config.night_from_we=c;
		config.night_to_we=d;
		EXTRACT_STRING(FOUR_INTS_FMT_STR);
		sprintf(buf, str,
				config.night_from_wd,
				config.night_to_wd,
				config.night_from_we,
				config.night_to_we
		);
		Serial.println(buf);
	} else if (!strncmp("sensors", cmd, 7)) {
		config.flags &= ~MODE_RUN;
		config.sensors_count = atoi(cmd+8);
		Serial.println("SET;");
// 		Serial.println(config.sensors_count, DEC);
	}
	Serial.flush();
}

void parseWaterCommand(char* cmd)
{
	if (!strncmp("reset", cmd, 5)) {
		resetError();
	} else if (!strncmp(cmd, "all", 3)) {
		water();
	}
}

void dumpConfig()
{
	char index = 0;
	const char*space=" ";
	static const char*fmt = "%d %d %d %d %d %d %d %d %d %s;";
	sprintf(buf,"set sensors %d\n", config.sensors_count);
	Serial.println(buf);
	delay(100);
	while (index < config.sensors_count) {
		pots_holder.readPot(index, &pot);
		sprintf(buf, fmt,
					index,
					CHIP(pot.sensor),
					PIN(pot.sensor),
					DOSER(pot.sensor),
					pot.bowl_index,
					pot.portion,
					pot.lbound,
					pot.rbound,
					pot.day_max,
					pot.name
			   );
 		Serial.println(buf);
		delay(250);
		++index;
	}
}

void dumpState()
{
	uint8_t index = 0;
	const char*space=" ";
	while (index < config.sensors_count) {
		pots_holder.readPot(index, &pot);
		Serial.print(index, DEC);
		Serial.print(space);
		Serial.print(CHIP(pot.sensor), DEC);
		Serial.print(space);
		Serial.print(PIN(pot.sensor), DEC);
		Serial.print(space);
		Serial.print(pot.watered, DEC);
		Serial.print(space);
		Serial.print(pot.last_time, DEC);
		Serial.print(space);
		Serial.print(pot.flags, DEC);
		Serial.print(space);
		Serial.print(pot.name);
		Serial.println();
		Serial.flush();
		delay(100);
		++index;
	}
}
/*
	set *
	get*
	show config
	read config
	erase config
	dump config
	save config
	start
	stop
	stat
	water *
	analize

 */
bool doCommand(char*cmd)
{
	timedata td;
/*
	if(!strncmp("spray", cmd, 5)) {
		pin_write(VIO_SPRAY_EN, 1, 1);
		delay(4000);
		pin_write(VIO_SPRAY_EN, 0, 1);
	}
	else */
	if (!strncmp("getlight", cmd, 8)) {
		Serial.print(config.light_sensor_pin, DEC);
		Serial.print(" ");
		Serial.print(config.light_sensor_min, DEC);
		Serial.print(" ");
		Serial.print(config.light_sensor_timeout, DEC);
		Serial.print(" ");
		Serial.println(cd4067[ config.light_sensor_pin>>4 ].read(config.light_sensor_pin & 0x0F));
		Serial.println(";");
	} else if (!strncmp("memclr",cmd,6)) {
		for(uint8_t i =0;i<31;++i) {
			ds1302.writeRAMbyte(i, 0);
		}
	} else if (!strncmp("rc",cmd,2)) {
		int8_t index =  cmd[2]-'0';
		if(index >=0 && index < DOSERS) {
			ws[index].calibrate(1);
		}
	} else if (!strncmp(cmd, "getstate", 8)) {
		td = ds1302.readtime();
		char fmt[] = "%d %d %d %d %d %d;";
		sprintf(buf, fmt, td.hours, td.minutes, td.seconds, td.day, td.month, td.dow);
		Serial.println(buf);
		sprintf(buf, fmt,
					IS_RUN(config.flags)?1:0,
					isNight(td)?1:0,
					config.night_from_wd,
					config.night_to_wd,
					config.night_from_we,
					config.night_to_we
			   );
		Serial.println(buf);
		sendErrorState();
		Serial.print(";;");
	} else if (!strncmp("geterrc", cmd, 7)) {
		sendErrorState();
		return true;
	} else if (!strncmp("getall", cmd, 6)) {
		pin_write(SENSOR_RELAY_PIN, HIGH);
		Serial.print(CD4067_COUNT * 16, DEC);
		Serial.print(" ");
		delay(200);
		for (char n = 0; n < CD4067_COUNT; ++n ) {
			for(char p = 0; p < 16; ++p) {
				Serial.print(cd4067[n].read(p), DEC);
				Serial.print(" ");
			}
			Serial.flush();
// 			Serial.println();
		}
		Serial.println(";");
		pin_write(SENSOR_RELAY_PIN, LOW);
		return true;
	} else if (!strncmp(cmd, "ping", 4)) {
		Serial.println("pong");
		for(char i=0;i<3;++i) {
			Serial.println(VER[i]);
		}
	} else if (!strncmp(cmd, "set ", 4)) {
		setOption(cmd + 4);
	} else if (!strncmp("pipi", cmd, 4))  {
		char index = atoi(cmd + 5);
		wd[ index ].run(30);
	} else if (!strncmp("water ", cmd, 6)) {
		parseWaterCommand(cmd+6);
	} else if (!strncmp(cmd,"stat",4)) {
		statistic();
	} else if (strlen(cmd) > 5 && !strncmp(cmd+5, "cfg",3)) {
		if (strncmp(cmd, "show", 4)==0) {
			printConfig();
		} else if (!strncmp(cmd, "dump", 4)) {
			dumpConfig();
		} else if (strncmp(cmd, "save", 4) == 0) {
			saveConfig();
			readConfig();
			Serial.println("OK");
// 			printConfig();
		} else if (strncmp(cmd, "read", 8) == 0) {
			readConfig();
			printConfig();
		}
	} else if(!strncmp(cmd, "!test", 5)) {
		unsigned long start = millis();
		analize(-1, 0);
		unsigned long end = millis();
		Serial.print("time:");
		Serial.println(end-start, DEC);
	} else if (!strncmp(cmd, "test", 4)) {
		analize(-1, 1);
	} else if (!strncmp(cmd, "atest", 5)) {
		analize(-1, 2);
	} else if (strncmp(cmd, "start", 5)==0) {
		config.flags |= MODE_RUN;
	} else if (strncmp(cmd, "stop", 4)==0) {
		config.flags &= ~MODE_RUN;
	}else if (strncmp(cmd, "dump stat", 9)==0) {
		dumpState();
	} else {
		return false;
	}
	Serial.flush();
	return true;
}

void checkCommand()
{
	if (Serial.available() < 4) return;
	static int i = 0;
	static char buf[42]={0}, ch;
	while (Serial.available()) {
		ch = Serial.read();
		if (ch ==10 || ch ==13) {
			if ( i == 0 ) continue;
			buf[ i ] = 0;
			doCommand(buf);
			showErrors();
			i = 0;
		} else {
			buf[i++] = ch;
		}
	}//while

}//sub

void resetError()
{
	wd[0].resetError();
	ws[0].resetError();
#if DOSERS > 1
	wd[1].resetError();
	ws[1].resetError();
#endif
#ifdef VIO_ERROR_LED_PIN
	pin_write(VIO_ERROR_LED_PIN, LOW, true);
#endif
}

void checkWaterPumpState()
{
#ifdef VIO_ERROR_LED_PIN
	pin_write(VIO_ERROR_LED_PIN,
			  wd[0].isError()
#if DOSERS > 1
			  || wd[1].isError()
#endif
			, true);
#endif
	return;
}

#ifdef USE_1WIRE
int16_t readOWThermo(int index)
{
    ds.reset();
    ds.select(thermo_addrs[ index ]);

    ds.write(0x44, 1);         // start conversion, with parasite power on at the end

    delay(800);     // maybe 750ms is enough, maybe not
    // we might do a ds.depower() here, but the reset will take care of it.
    ds.reset();

    ds.select(thermo_addrs[ index ]);
    ds.write(0xBE);         // Read Scratchpad

    uint8_t data[9];
    ds.read_bytes(data, 9);
	int16_t result = round((data[1]<<8|data[0])/16.0);
	return result;
}
#endif

#if defined(USE_1WIRE) && defined(USE_LCD)

void showTemps()
{
	lcd.setCursor(0, LCD_THERMO_LINE);
	static const char* thermo_fmt = "Там:%2dC Тут:%2dC";
	sprintf(buf, thermo_fmt, readOWThermo(OUTDOOR_TERMO), readOWThermo(INDOOR_TERMO));
	lcd.print(buf);
}

#endif

void showErrors()
{
	uint8_t i;
#ifdef USE_LCD
	lcd.setCursor(0, LCD_ERROR_LINE);
	for (i=0;i<DOSERS;++i) {
		lcd.print(ws[i].getError(), HEX);
		lcd.print(" ");
	}
	for (i=0;i<DOSERS;++i) {
		lcd.print(wd[i].getError(), HEX);
		lcd.print(" ");
	}
	lcd.print(last_sensors_fails, DEC);
#else
// 	sendErrorState();
#endif
}

void sendErrorState()
{
	int8_t i;
	for (i=0;i<DOSERS;++i) {
		Serial.print("0x");// Qt QTextStream can not read hexadecimals without prefix.*facepalm*
		Serial.print(ws[i].getError(), HEX);
		Serial.print(" ");
	}
	for (i=0;i<DOSERS;++i) {
		Serial.print("0x");
		Serial.print(wd[i].getError(), HEX);
		Serial.print(" ");
	}
	Serial.print(last_sensors_fails, DEC);
	Serial.println(";");
}

#ifdef VIO_LIGHT_BULB_PIN

#define FRAM_LAST_STATE			2
#define FRAM_HOUR				0
#define FRAM_MINUTE				1
#define FRAM_DARK_START_HOUR	3
#define FRAM_DARK_START_MINUTE	4
#define LAMP_SWITCH_MIN_DELTA	10

uint8_t light_bulb_state = 0;
void lightSwitch(bool state)
{
		timedata td = ds1302.readtime();
		uint8_t h = ds1302.readRAMbyte(FRAM_HOUR), m = ds1302.readRAMbyte(FRAM_MINUTE), delta = abs(td.hours - h) * 60 + abs(td.minutes - m),
			last_state = ds1302.readRAMbyte(FRAM_LAST_STATE);
		if (delta < LAMP_SWITCH_MIN_DELTA && last_state != state) {
			return;
		}

		if (state) {
			pin_write(VIO_LIGHT_BULB_PIN, 1, 1);
			light_bulb_state = 1;
			ds1302.writeRAMbyte(FRAM_HOUR, td.hours);
			ds1302.writeRAMbyte(FRAM_MINUTE, td.minutes);
		} else {
			pin_write(VIO_LIGHT_BULB_PIN, 0, 1);
			light_bulb_state = 0;
			ds1302.writeRAMbyte(FRAM_HOUR, td.hours);
			ds1302.writeRAMbyte(FRAM_MINUTE, td.minutes);
			ds1302.writeRAMbyte(FRAM_LAST_STATE, 0);
		}
}
#endif

void loop()
{
	int i, val;
  	static timedata td;
#ifdef USE_LCD
	static const char night[] PROGMEM = "Ночь            ";
	static const char stop[]  PROGMEM = "СТОП            ";
	static const char watering[] PROGMEM = "полив: ";
	static const char ago[] PROGMEM = "м назад ";
#endif

	checkCommand();
	td = ds1302.readtime();
#ifdef VIO_LIGHT_BULB_PIN
	/**
		we use metal-halogen lamp. it should not change state more than one time in 5 minutes.
	 */
	uint8_t h = ds1302.readRAMbyte(FRAM_HOUR), m = ds1302.readRAMbyte(FRAM_MINUTE), delta = abs(td.hours - h) * 60 + abs(td.minutes - m),
			last_state = ds1302.readRAMbyte(FRAM_LAST_STATE);
	if (light_bulb_state == 0 && td.hours >=10 && td.hours < 22) {
			if (config.light_sensor_pin > 0) {
				int lsv = cd4067[ config.light_sensor_pin>>4 ].read(config.light_sensor_pin & 0x0F);
				int delta2 = (td.hours - ds1302.readRAMbyte(FRAM_DARK_START_HOUR))*60 + (td.minutes - ds1302.readRAMbyte(FRAM_DARK_START_MINUTE));
				if (lsv <= config.light_sensor_min) {
					if (delta2 >= config.light_sensor_timeout) {
						lightSwitch(true);
					}
				} else {
					lightSwitch(false);
					ds1302.writeRAMbyte(FRAM_DARK_START_HOUR, td.hours);
					ds1302.writeRAMbyte(FRAM_DARK_START_MINUTE, td.minutes);
				}
			} else {
// 				if (delta > LAMP_SWITCH_MIN_DELTA) {
					lightSwitch(true);
// 				}
			}
	} else if (light_bulb_state == 1 && (td.hours >=22 || td.hours < 10) ) {
//  		if ( delta > LAMP_SWITCH_MIN_DELTA) {
			lightSwitch(false);
// 			pin_write(VIO_LIGHT_BULB_PIN, 0, 1);
// 			light_bulb_state = 0;
// 			ds1302.writeRAMbyte(0, td.hours);
// 			ds1302.writeRAMbyte(1, td.minutes);
// 			ds1302.writeRAMbyte(2, 0);
//  		}
	}
	if (light_bulb_state == 1) {
		ds1302.writeRAMbyte(FRAM_HOUR, td.hours);
		ds1302.writeRAMbyte(FRAM_MINUTE, td.minutes);
		ds1302.writeRAMbyte(FRAM_LAST_STATE, 1);
	}
/*
save last check time in non-volative RAM
 */
#endif

#ifdef USE_1WIRE
	showTemps();
#endif

#ifdef USE_LCD
		lcd.setCursor(0, LCD_TIME_LINE);
		printTime2buf(td, 0, 1);
		lcd.print(buf);
		showErrors();
#endif
		if ( (isNight(td) || !IS_RUN(config.flags)) ) {
#ifdef USE_LCD
			lcd.setCursor(0, LCD_STATE_LINE);
			if (isNight(td)) {
				EXTRACT_STRING(night);
			} else {
				EXTRACT_STRING(stop);
			}
			lcd.print(str);
#endif
			checkWaterPumpState();
			return;
		}
		val = minutes_diff(last_spray, td);
/*		int barrier  = 100 - 15 * (global_state.data.pe.temp_inner - 25);
		if ( val > barrier && IS_SPRAY_ON) {
			pin_write(VIO_SPRAY_EN, HIGH, 1);
			delay(5000);
			pin_write(VIO_SPRAY_EN, LOW, 1);
			last_spray = td;
		}
*/
		val = minutes_diff(last_analize, td);
#ifdef USE_LCD
		lcd.setCursor(0, LCD_STATE_LINE);
		EXTRACT_STRING(watering);
		lcd.print(str);
		lcd.print(val);
		EXTRACT_STRING(ago);
		lcd.print(str);
		showErrors();
#endif
		if (val < config.pause_in_minutes) {
			return;
		}
// 		Serial.println("analize..");
	analize();
	ws[0].resetError();
#if DOSERS > 1
	ws[1].resetError();
#endif
// 	Serial.println("start water");
	water();
// 	Serial.println("end water");

#ifndef USE_1WIRE // if we dont use 1-wire thermo, make little pause between refreshes.
	delay(1000);
#endif
}


void setup()
{
#if DEBUG
//watchdog setup
	cli();
		/* Clear the reset flag. */
		MCUSR &= ~(1<<WDRF);

		/* In order to change WDE or the prescaler, we need to
		* set WDCE (This will allow updates for 4 clock cycles).
		*/
		WDTCSR |= (1<<WDCE) | (1<<WDE);

		/* set new watchdog timeout prescaler value */
		WDTCSR = /*(1<<WDP0) |*/ (1<<WDP0) | (1<<WDP2); /* 1.0 second */

		/* Enable the WD interrupt (note no reset). */
		WDTCSR |= _BV(WDIE);
	sei();
#endif

	pinMode(2, INPUT);
	pinMode(3, INPUT);
	//pull-down them!
	digitalWrite(2, LOW);
	digitalWrite(3, LOW);
#if FASTADC
  // set prescale to 16
  cbi(ADCSRA,ADPS2);
  sbi(ADCSRA,ADPS1);
  cbi(ADCSRA,ADPS0);
#endif


// static const char hello[] PROGMEM = "HELLO!";
// static const char init[] PROGMEM = "Запуск...";
// Serial.println(freeMemory(), DEC);

Serial.begin(BT_BAUD);
// 	EXTRACT_STRING(hello);
// 	Serial.println(str);

extPins.init();

#ifdef USE_LCD
	lcd.begin(16, 4);
	lcd.clear();
#endif
mem.init();

	ds1302.setTrickleMode(0);
	last_spray = ds1302.readtime();
	printTime2buf(last_spray);
	Serial.println(buf);
	pin_write(SENSOR_RELAY_PIN, LOW, true);

	for(char i = 0; i < CD4067_COUNT; ++i) {
		cd4067[ i ].init();
	}
#ifdef USE_LCD
//  	EXTRACT_STRING(hello);
// 	lcd.print(str);
#endif

	for (char i = 0; i < DOSERS; ++i) {
		wd[i].init(i);
		wd[i].setDevId(10+i);
	}
#ifdef BIG_ROOM
	ws[0].init(WATER_SERVO1_PIN, REED_1, 10);
#else
	ws[0].init(WATER_SERVO1_PIN, REED_1, 16, 500, 2500/*3200*/);
#endif
	ws[0].setDevId(1);

#if DOSERS > 1
	ws[1].init(WATER_SERVO2_PIN, REED_2, 9);
	ws[1].setDevId(2);
#endif
	config.flags = USE_CLOCK(config.flags);
	config.flags &= ~MODE_RUN;
	config.pause_in_minutes = 30;
	config.night_from_wd = config.night_from_we = 22;
	config.night_to_we = config.night_to_wd = 9;
	config.sensors_count = 0;
	readConfig();
	readDayData();
}


#if DEBUG
ISR(WDT_vect)
{
// lcd.setCursor(2,
// showErrors();
Serial.print("WDT ");
Serial.println(freeMemory(), DEC);
}

#endif
