/**
 * debug 29 652 bytes
 * release 28 830bytes
	Atmega328 pot plant waterer.
	Hardware:
	1x atmega328
	2 pumps and 2 water flow sensors FS100A
	3x HC595
	1x LCD
	1x DS18b20+
	1x DHT11
	2 servos for dosing water
	1x bluetooth HC-06 for setup and monitoring
	1x beeper (may be disabled)
*/
#define DEBUG 1
// #define TONE_DBG 1
#include <Servo.h>
#include <avr/wdt.h>
// #include <EEPROM.h>
#include <ShiftOut.h>
#include <OneWire.h>
#include <avr/eeprom.h>
#include <LiquidCrystal595Rus.h>
#include <WaterDoser.h>
#include <WaterServoEx.h>

#include <DS1302.h>
// #include <DHT.h>
#include <Wire.h>

#define CONFIG_MAGIC 0x42


#define BIG_ROOM 1

#define MULTIDOSER 1
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
#define VIO_PUMP1	 	 ((VIO_BASE) + 2)	//*
#define SENSOR_RELAY_PIN	VIO_AIN_RELAY

#define PUMP_PIN			VIO_PUMP1

#ifdef BIG_ROOM
	#define MULTIDOSER 1

	#define DOSERS	2
	#define PUMPS	2

	#define USE_1WIRE	1
	#define USE_LCD		1
	// #define _USE_LIGHT_SENSOR

	#define PUMP_STATE_RESET_PIN	3 //interrupt 1

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

// 	#define VIO_AIN_RELAY	 ((VIO_BASE) + 1) //*
// 	#define VIO_PUMP1	 	 ((VIO_BASE) + 2)	//*
	#define VIO_PUMP2	 	 ((VIO_BASE) + 3) //*
	// #define VIO_WF_SENSOR_1	 ((VIO_BASE)+9)
	// #define VIO_WF_SENSOR_2 ((VIO_BASE)+10)
	// #define VIO_SPRAY_EN 	 ((VIO_BASE)+11)

// 	#define PUMP_PIN			VIO_PUMP1
	#define PUMP_PIN2			VIO_PUMP2

	// #define VIO_PULL_DOWN_1	 ((VIO_BASE)+9)
	// #define VIO_PULL_DOWN_2 ((VIO_BASE)+10)
	// #define VIO_PULL_DOWN_3	 ((VIO_BASE)+11)

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

	#define LCD_TIME_LINE		0
	#define LCD_OUT_TEMP_LINE	1
	#define LCD_STATE_LINE		2
	#define LCD_ERROR_LINE		3
	// #define LCD_INDOOR_STATE_LINE	3
	#define LCD_THERMO_LINE			1

	#define CD4067_COUNT		2

	#define POSITIONS	12

#else
// 	#define MULTIDOSER 1

	#define DOSERS	1
	#define PUMPS	1
	#define SHIFT_OUTS	2

// 	#define USE_1WIRE	1
// 	#define USE_LCD		1
	// #define _USE_LIGHT_SENSOR

	#define PUMP_STATE_RESET_PIN	3 //interrupt 1

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
	#define POSITIONS	12

#endif

#define MAX_POTS	20//(CD4067_COUNT*16)

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
#define PIN(x) ( (x) & 0x07)  // 3 bits (vals 0..7)
#define CHIP(x) ( ((x)>>3) & 0x03) //2 bits, 0..3
#define DOSER(x) ( ((x)>>5) & 0x03) //2 bits, 0..3
#define PACK_SENSOR(chip, pin, doser) ( ( ((doser) & 0x03)<<5) | (( (chip)&0x03)<<3)| ((pin) & 0x07 ) )

#define NEEDS_WATERING(x) (pots[x].flags & 0x01)

#define SET_WATERED(x) pots[x].flags &= 0xFE;

#define START_WATERING(x) pots[x].flags |= 0x01;

#define SET_WATERING(x, y) if(y) {START_WATERING(x);} else {SET_WATERED(x);}

#define SET_INTERVAL_DRYING(x) 	pots[x].flags |= 0x02;
#define SET_INTERVAL_WATERING(x)   pots[x].flags &= 0xFD;
#define IS_DRYING_INTERVAL(x) (pots[x].flags & 0x02)


#define AIN_PULLDOWN(x) ( ( (x) >> 1) & 0x03)

#undef PROGMEM

#define PROGMEM __attribute__((section(".progmem.data")))

#include <AT24C512.h>

/*
	особо экономить эту память бессмысленно, т.к. у нас 16КБ на конфиг максимум в 32 элемента по меньше, чем 64 байта.
	структура конфига:
	страница							данные
	0									magic
	1									global config
	2									positions
	3									positions2
	POTS_CONFIG_START+PAGE_ALIGN*i		pots[i]
*/

#define PAGE_ALIGN	64
#define POTS_CONFIG_START	(PAGE_ALIGN * 6)

AT24C512 mem(80);



const char *VER[3] = {__DATE__, __TIME__, "w10.cpp"};

ShiftOut extPins(SHIFT_LATCH, SHIFT_CLOCK, SHIFT_DATA, SHIFT_OUTS);

#ifdef USE_LCD
	LiquidCrystal595Rus lcd(&extPins, VIO_LCD_RS_PIN, VIO_LCD_RW_PIN, VIO_LCD_E_PIN, VIO_LCD_D4_PIN, VIO_LCD_D5_PIN, VIO_LCD_D6_PIN, VIO_LCD_D7_PIN);
#endif

DS1302 ds1302(DS1302_CE, DS1302_CLK, DS1302_IO);

char buf[60]={0}, str[40]={0};

// uint8_t WFS[ PUMPS ] = {VIO_WF_SENSOR_1, VIO_WF_SENSOR_2};

#ifdef BIG_ROOM

#ifdef USE_1WIRE
#define MAX_DS1820_SENSORS 2
#define INDOOR_TERMO	1
#define OUTDOOR_TERMO	0

uint8_t thermo_addrs[MAX_DS1820_SENSORS][8] = {{40,89,110,160,4,0,0,34},{40,181,213,159,4,0,0,155}};
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
	uint8_t spray_length;
};

struct plantdata{
  uint8_t sensor;
  uint8_t flags;
//   char needs_watering;
  uint8_t bowl_index; // номер стаканчика на раздаточном уровне
  uint8_t portion;//кол-во воды для разового полива 255 max!!
  uint8_t lbound;// нижний порог срабатывания сенсора
  uint8_t rbound;//верхний порог срабатывания сенсора
  uint16_t day_max;//2b
  unsigned long int last_time;//4b
  uint16_t watered;//2b
};//16bytes

struct potdata{
	uint8_t flags;//1b
	unsigned long int last_time;//4b
	uint16_t watered;//2b
};

plantdata pots[MAX_POTS] = {-1};

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
	if(pin <= 3) return;//0 & 1 --serial, 2 -- wf sensor, 3 -- water err reset btn
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
	uint8_t read(uint8_t pin)
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
		return (ret >> 3);
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


// bool run = false;

// uint16_t elapsed = 0;

#if 0
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

void readPotName(uint8_t index, char*buf)
{
	memset(buf, 0, 16);
	mem.readBuffer(POTS_CONFIG_START + PAGE_ALIGN * index + sizeof(plantdata), buf, 16 );

}

void writePotName(uint8_t index, char*name)
{
	mem.writeBuffer(POTS_CONFIG_START + PAGE_ALIGN * index + sizeof(plantdata), name, 16 );
}

void saveConfig()
{
	char i = 0;

	mem.write(0, CONFIG_MAGIC);
	mem.writeBuffer(PAGE_ALIGN, (char*)&config, sizeof(config));

	i = 0;
// 	mem.writeBuffer(PAGE_ALIGN * 2, (char*)&positions, sizeof(positions));
// 	mem.writeBuffer(PAGE_ALIGN * 3, (char*)&positions2, sizeof(positions2));

	while (i < config.sensors_count) {
		mem.writeBuffer(POTS_CONFIG_START + PAGE_ALIGN * i, (char*)&pots[i], sizeof(plantdata));
		++i;
	}
}
void eraseConfig()
{
// 	for(uint16_t i=0; i < 512; ++i) {
// 		eeprom_write_byte( (uint8_t*)i, 0);
// 	}
}

void readConfig()
{
	char i = 0;

	if (mem.read(0) != CONFIG_MAGIC) {
		config.sensors_count = 0;
		config.flags &= ~MODE_RUN;
		Serial.println("bad cfg");
		return;
	}
	mem.readBuffer(PAGE_ALIGN, (char*)&config, sizeof(config));

// 	mem.readBuffer(PAGE_ALIGN * 2, (char*)&positions, sizeof(positions));
// 	mem.readBuffer(PAGE_ALIGN * 3, (char*)&positions2, sizeof(positions2));

	if(config.sensors_count > MAX_POTS) {
		config.sensors_count = 0;
		config.flags &= ~MODE_RUN;
		Serial.println("bad cfg");
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
		mem.readBuffer(POTS_CONFIG_START + PAGE_ALIGN * i, (char*)&pots[i], sizeof(plantdata));
		SET_INTERVAL_WATERING(i);
		++i;
	}
}

void printPotConfig(char i)
{
	// strcpy_P(str, (char*)pgm_read_word(time_with_second_fmt))
	static const char fmt1[] PROGMEM = "%d: %d/%d doser %d bowl %d";
	static const char fmt2[] PROGMEM = " dose %u [%d..%d] daymax %d";

	EXTRACT_STRING(fmt1);
	sprintf(buf, str,
			i,
			PIN(pots[i].sensor),
			CHIP(pots[i].sensor)+1,
			DOSER(pots[i].sensor),
			pots[i].bowl_index
	);
	Serial.print(buf);
	EXTRACT_STRING(fmt2);
	sprintf(buf, str,
			pots[i].portion,
			pots[i].lbound,
			pots[i].rbound,
			pots[i].day_max
	);
	Serial.print(buf);
	Serial.print(" ");
	readPotName(i, buf);
	Serial.println(buf);
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
		sprintf(buf, str,
			i,
			PIN(pots[i].sensor),
			CHIP(pots[i].sensor)+1
		);
		Serial.print(buf);
		td = ds1302.fromTimestamp(pots[i].last_time);
		printTime2buf(td);
		Serial.print(buf);
		EXTRACT_STRING(stat_p2);
		sprintf(buf, str,
					pots[i].bowl_index,
					DOSER(pots[i].sensor),
					pots[i].watered,
					pots[i].day_max,
					IS_DRYING_INTERVAL(i)?"dry ":"water "
			   );
		Serial.print(buf);
		readPotName(i, buf);
		Serial.println(buf);
		Serial.flush();
		++i;
	}//while
	Serial.println();
}


int readSensor(int i, bool debug=false)
{
	int val = cd4067[CHIP(pots[i].sensor)].read(PIN(pots[i].sensor) );
	static const char  bad_value_debug[] PROGMEM = "BAD VALUE: %d ";
	static const char debug_fmt[] PROGMEM = "%d %d/%d bowl %d/%d val %d/%d..%d should:%d(%d) ";
	//датчик убился -- не слушаем его
	if (val < MINIMUN_VALID_VALUE) {
		EXTRACT_STRING(bad_value_debug);
		sprintf(buf, str, val);
		Serial.print(buf);
		readPotName(i, buf);
		Serial.println(buf);
		SET_WATERED(i);
// 		pots[i].needs_watering = false;
		return 0;
	}
/*
	Serial.print(val, DEC);
	Serial.print(" ");
	Serial.print(pots[i].lbound, DEC);
	Serial.print(" ");
	Serial.print(pots[i].rbound, DEC);
	Serial.print(" ");
	Serial.print(( (val <= pots[i].rbound) && !(IS_DRYING_INTERVAL(i))), DEC);
	Serial.print(" ");
	Serial.println(( ( (val <= pots[i].rbound) && !(IS_DRYING_INTERVAL(i))) || (val <= pots[i].lbound))?1:0, DEC);
*/
	if (val >= pots[i].rbound) {
		SET_INTERVAL_DRYING(i);
		SET_WATERING(i, 0);
	} else if (val <= pots[i].lbound) {
		SET_INTERVAL_WATERING(i);
		SET_WATERING(i, 1);
	} else {
		/**
		* если суше, чем минимально допустимый порог, то поливаем всегда.
		* иначе же, если мы на стадии "проливаем" и не долили до допустимого максимума, то тоже надо поливать.
		*/
		SET_WATERING(i, ( ( IS_DRYING_INTERVAL(i)) ? 0 :1));
	}
	if (debug) {
		EXTRACT_STRING(debug_fmt);
		sprintf(buf, str,
					i,
					PIN(pots[i].sensor),
					CHIP(pots[i].sensor) + 1,
					pots[i].bowl_index,
					DOSER(pots[i].sensor),
					val,
					pots[i].lbound,
					pots[i].rbound,
					NEEDS_WATERING(i),
					1 - IS_DRYING_INTERVAL(i)
		);
		Serial.print(buf);
		readPotName(i, buf);
		Serial.println(buf);
	}
	return val;
}

void analize(char index=-1, bool debug=false)
{
	int i = 0, val;
	static uint8_t vals[MAX_POTS] = {0};
    pin_write(SENSOR_RELAY_PIN, HIGH);
    delay(1000);

	if(index == -1) {
		if (!debug) { /* тесты и донастройки не должны сбивать таймер, 12мая*/
			last_analize = ds1302.readtime();
		}

		while (i < config.sensors_count) {
			vals[ i ] = readSensor(i, debug);
			++i;
		}//while

		pin_write(SENSOR_RELAY_PIN, LOW);
	} else {
		val = readSensor(i, true);
		pin_write(SENSOR_RELAY_PIN, LOW);
	}
}

void water2Bowl(int i)
{
	timedata td = ds1302.readtime();
	if (td.day != ds1302.fromTimestamp(pots[i].last_time).day) {
		pots[i].watered = 0;
	}
	if (pots[i].watered > pots[i].day_max) {
 		sprintf(buf, "%i max=(", i);
 		Serial.println(buf);
		SET_WATERED(i);
		return;
	}
	uint8_t doser = DOSER(pots[i].sensor);
// 	pin_write( WFS[ doser ], 1, 1);
	ws[ doser ].moveTo(pots[i].bowl_index);
	if (ws[ doser ].isError()) {
		showErrors();
		return;
	}
	pots[i].watered += wd[ doser ].run(pots[i].portion);
// 	pin_write( WFS[ doser ], 0, 1);
	SET_WATERED(i);
	pots[i].last_time = ds1302.getTimestamp(td);
}

void water()
{
// 	timedata td;
	char i = 0;
	while ( i < config.sensors_count) {
		if ( !wd[ DOSER(pots[i].sensor) ].isError() && NEEDS_WATERING(i) ) {
			water2Bowl(i);
		} else {
			if (wd[ DOSER(pots[i].sensor) ].isError()) {
				sprintf(buf, "%d pump error", i);
			} else if (!NEEDS_WATERING(i)) {
				sprintf(buf, "%d: no watering flag", i);
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
	Serial.println("eow");
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
	timedata td = ds1302.readtime();
	ds1302.writeRAMbyte(0, td.day);
	ds1302.writeRAMbyte(1, config.sensors_count);
	for(char i=0;i<config.sensors_count; ++i) {
		ds1302.writeRAMbyte( 2 + i, map(pots[i].watered, 0, 1024, 0, 255));
	}
}

void readDayData()
{
	timedata td = ds1302.readtime();
	for(char i=0;i<config.sensors_count; ++i) {
		pots[i].watered = 0;
	}
	if (td.day == ds1302.readRAMbyte(0)) {
		for(char i=0;i<config.sensors_count; ++i) {
			pots[i].watered = map(ds1302.readRAMbyte( 2 + i), 0, 255, 0, 1024);
		}
	}
}

void setOption(char* cmd)
{
	timedata td;
	static const char TWO_INTS_FMT_STR[] PROGMEM="%d %d";
	static const char THREE_INTS_FMT_STR[] PROGMEM="%d %d %d";
	static const char time_read_fmt[] PROGMEM = "%d:%d:%d %d.%d %d";
	static const char data_fmt[] PROGMEM = "%d %d %d %d %d %d %d %d %d";
	static const char nights[] PROGMEM = "nights: work day %d-%d, weekend %d-%d";
	static const char stop_msg[] PROGMEM = "Stopped.\nSensors count=";
	static const char FOUR_INTS_FMT_STR[] PROGMEM = "%d %d %d %d";
	if (!strncmp("name", cmd, 4)) {
		uint8_t index = (cmd[5]-'0') * 10 + (cmd[6]-'0');
		writePotName(index, cmd + 8);
		readPotName(index, str);
		Serial.println("OK");
		Serial.println(str);
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
			SET_INTERVAL_WATERING(i);
			++i;
		}
	} else if (!strncmp("time", cmd, 4)) {
		int dow=1;
		EXTRACT_STRING(time_read_fmt);
		sscanf(cmd + 5, str, &td.hours, &td.minutes, &td.seconds, &td.day, &td.month, &dow);
		td.dow = dow;
		td.year = 2013;
		printTime2buf(td);
		Serial.println(buf);
		ds1302.writetime(td);
		td = ds1302.readtime();
		Serial.print("Set time: ");
		printTime2buf(td);
		Serial.println(buf);
	} else if (!strncmp("data ", cmd, 5)) {
		int index=0, a=0,b=0,c=0,d=0,e=0, f=0, g=0, h=0;
		EXTRACT_STRING(data_fmt);
		sscanf(cmd+5, str, &index, &a, &b, &c, &d, &e, &f, &g, &h);
		pots[ index ].sensor = PACK_SENSOR(a, b, c);//.chip_index = a;
		pots[ index ].bowl_index = d;
		pots[ index ].portion = e;
		pots[ index ].lbound = f;
		pots[ index ].rbound = g;
		pots[ index ].day_max = h;
		Serial.println("OK");
// 		printPotConfig(index);
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
		wd[0].resetError();
		wd[1].resetError();
	} else if (!strncmp(cmd, "all", 3)) {
		water();
	}
}

void dumpConfig()
{
	char index = 0;
	const char*space=" ";
	static const char*fmt = "set data %d %d %d %d %d %d %d %d %d";
	sprintf(buf,"set sensors %d\n", config.sensors_count);
	Serial.println(buf);
	delay(100);
	while (index < config.sensors_count) {
		sprintf(buf, fmt,
					index,
					CHIP(pots[ index ].sensor),
					PIN(pots[ index ].sensor),
					DOSER(pots[ index ].sensor),
					pots[ index ].bowl_index,
					pots[ index ].portion,
					pots[ index ].lbound,
					pots[ index ].rbound,
					pots[ index ].day_max
			   );
 		Serial.println(buf);
		delay(250);
		++index;
	}
}

void dumpState()
{
	uint8_t index = 0;
	char*space=" ";
	while (index < config.sensors_count) {
		Serial.print(index, DEC);
		Serial.print(space);
		Serial.print(CHIP(pots[ index ].sensor), DEC);
		Serial.print(space);
		Serial.print(PIN(pots[ index ].sensor), DEC);
		Serial.print(space);
		Serial.print(pots[ index ].watered, DEC);
		Serial.print(space);
		Serial.print(pots[ index ].last_time, DEC);
		Serial.print(space);
		Serial.print(pots[ index ].flags, DEC);
		Serial.print(space);
		Serial.println();
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
#ifdef _DEBUG
	if (!strncmp(cmd, "calibrate", 9)) {
		pin_write(SENSOR_RELAY_PIN, HIGH);
		for (char i=0;i < 5; ++i) {
// 			sprintf(buf,"chip %5d %5d %5d %5d %5d %5d %5d %5d",0,1,2,3,4,5,6,7);
			for (char n = 0; n < CD4067_COUNT; ++n ) {
				sprintf(buf,"%4d ", n);
				Serial.print(buf);
				for(char p=0;p<8;++p) {
					sprintf(buf, "%5d:%4d",p, cd4067[n].read(p));
					Serial.print(buf);
				}
				Serial.println();
			}
			Serial.println();
			delay(3000);
		}
		pin_write(SENSOR_RELAY_PIN, LOW);
	} else
#endif
/*
	if(!strncmp("spray", cmd, 5)) {
		pin_write(VIO_SPRAY_EN, 1, 1);
		delay(4000);
		pin_write(VIO_SPRAY_EN, 0, 1);
	}
	else */
	if (!strncmp("rc",cmd,2)) {
		int8_t index =  cmd[2]-'0';
		if(index >=0 && index < DOSERS) {
			ws[index].calibrate(1);
		}
	} else if (!strncmp("getall", cmd, 6)) {
		pin_write(SENSOR_RELAY_PIN, HIGH);
		Serial.print(CD4067_COUNT * 16);
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
	} else if (!strncmp("rm cfg", cmd, strlen("rm cfg"))) {
		eraseConfig();
	} else if (!strncmp(cmd, "ping", 4)) {
		Serial.println("pong");
		for(char i=0;i<3;++i) {
			Serial.println(VER[i]);
		}
	}/* else if (!strncmp("alive ", cmd, 6)) {
		config.alive_ping = cmd[6] - '0';
	} */else if (!strncmp(cmd, "set ", 4)) {
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
	} else if (!strncmp(cmd, "test", 7)) {
		analize(-1, true);
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
	wd[1].resetError();
#ifdef VIO_ERROR_LED_PIN
	pin_write(VIO_ERROR_LED_PIN, LOW, true);
#endif
}

void checkWaterPumpState()
{
#ifdef VIO_ERROR_LED_PIN
	pin_write(VIO_ERROR_LED_PIN, wd[0].isError() || wd[1].isError(), true);
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
#else
	Serial.print("State: ");
	for (i=0;i<DOSERS;++i) {
		Serial.print(ws[i].getError(), HEX);
		Serial.print(" ");
	}
	for (i=0;i<DOSERS;++i) {
		Serial.print(wd[i].getError(), HEX);
		Serial.print(" ");
	}
#endif
}

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
// 	return;
// 	Serial.println("loop()");
// 	showErrors();
	checkCommand();
	td = ds1302.readtime();
#ifdef USE_1WIRE
	showTemps();
#endif

#ifdef USE_LCD
		lcd.setCursor(0, LCD_TIME_LINE);
		printTime2buf(td, 0, 1);
		lcd.print(buf);
#endif
		if ( /*USE_CLOCK(config.flags) &&*/ (isNight(td) || !IS_RUN(config.flags)) ) {
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
#endif
		if (val < config.pause_in_minutes) {
			return;
		}
		Serial.println("analize..");
	analize();
	Serial.println("start water");
	water();
	Serial.println("end water");
	showErrors();
#ifdef USE_LCD
	lcd.begin(16, 4);
	lcd.clear();
#endif
#ifndef BIG_ROOM
	delay(1000);
#endif
}


void setup()
{
#if 0
	/* Clear the reset flag. */
	MCUSR &= ~(1<<WDRF);

	/* In order to change WDE or the prescaler, we need to
	* set WDCE (This will allow updates for 4 clock cycles).
	*/
	WDTCSR |= (1<<WDCE) | (1<<WDE);

	/* set new watchdog timeout prescaler value */
	WDTCSR = /*(1<<WDP0) |*/ (1<<WDP1) | (1<<WDP2); /* 8.0 seconds */

	/* Enable the WD interrupt (note no reset). */
	WDTCSR |= _BV(WDIE);
#endif
	pinMode(2, INPUT);
	pinMode(3, INPUT);
	digitalWrite(2, LOW);
	digitalWrite(3, LOW);
#if FASTADC
  // set prescale to 16
  cbi(ADCSRA,ADPS2);
  sbi(ADCSRA,ADPS1);
  cbi(ADCSRA,ADPS0);
#endif


static const char hello[] PROGMEM = "HELLO!";
static const char init[] PROGMEM = "Запуск...";
// Serial.println(freeMemory(), DEC);

Serial.begin(BT_BAUD);


	EXTRACT_STRING(hello);
	Serial.println(str);


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
 	EXTRACT_STRING(hello);
	lcd.print(str);
#endif

	for (char i = 0; i < DOSERS; ++i) {
		wd[i].init(i);
		wd[i].setDevId(10+i);
	}
#ifdef BIG_ROOM
	ws[0].init(WATER_SERVO1_PIN, REED_1, 10);
#else
	ws[0].init(WATER_SERVO1_PIN, REED_1, 16, 500, 3200);
#endif
	ws[0].setDevId(1);
// 	ws[0].calibrate();
// 	pinMode(4, OUTPUT);
// 	digitalWrite(4, 1);
// 	delay(1000);
// 	digitalWrite(4, 0);


#if DOSERS > 1
	ws[1].init(WATER_SERVO2_PIN, REED_2, 8);
	ws[1].setDevId(2);
// 	ws[1].calibrate();
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

#if 0
ISR(WDT_vect)
{
	Serial.print("WDT ");
	Serial.println(freeMemory(), DEC);
}
#endif
