#include "obdwrapper.h"
#include <Wire.h>

// set this to either DD_OLED or DD_TFT to set which display backend to use
#define DD_OLED

#ifdef DD_OLED
#include "obd-dd-oled.h"
#endif

// On Arduino Leonardo, Micro, MEGA or DUE, hardware serial can be used for output as the adapter occupies Serial1
// On Arduino UNO and those have no Serial1, we use software serial for output as the adapter uses Serial
#ifdef ARDUINO_AVR_UNO
#include <SoftwareSerial.h>
//SoftwareSerial mySerial(A2, A3);
#else
//#define mySerial Serial
#endif

#if defined(ESP32) && !defined(Serial1)
//HardwareSerial Serial1(1);
#endif

byte obdver;

void setup() {
	//mySerial.begin(115200);
	//while (!mySerial);

	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(3, INPUT);
	pinMode(4, INPUT);

#ifdef DD_OLED
	initOLED(obdver);
	dimOLED(true); // why not
	updateOLED_Startup();
#endif

	bool success = DDInitOBD(obdver);

/*
	mySerial.print("Found OBD-II adapter. Firmware version: ");

	// weird version number packing
	mySerial.print(version / 10);
	mySerial.print('.');
	mySerial.println(version % 10);
*/

	// TODO: get error codes on startup and do something with them

	digitalWrite(LED_BUILTIN, HIGH); // turn on the user LED to show we finished init
}

uint8_t pstateprev = 0, sstateprev = 0;
uint8_t pstatenext = 0, sstatenext = 0;

void checkButtons() {
	pstateprev = sstateprev;
	pstatenext = sstatenext;

	sstateprev = digitalRead(3);
	sstatenext = digitalRead(4);

	if (sstateprev > pstateprev) {
		// rising edge of switch
#ifdef DD_OLED
		prevModeOLED();
#endif
	}

	if (sstatenext > pstatenext) {
		// rising edge of switch
#ifdef DD_OLED
		nextModeOLED();
#endif
	}
}

DashData_s d_data;
bool status = false;

void loop() {
	//checkButtons();

	// 1. get data
	status = DDGetDashData(d_data);
	if (!status) {
		d_data.rpm = millis() % 10000;
		d_data.fuel = 0.95f;
		d_data.coolant_temp = millis() % 99;
		d_data.voltage = 12.1f;
		d_data.boost_pressure = 10.4;
		d_data.speed.mph = millis() % 120;
	}

	// 2. refresh/draw display
#ifdef DD_OLED
	//updateOLED(d_data);
	updateOLED_Debug();
#endif
}
