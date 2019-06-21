#include "obdwrapper.h"
#include <Wire.h>

// On Arduino Leonardo, Micro, MEGA or DUE, hardware serial can be used for output as the adapter occupies Serial1
// On Arduino UNO and those have no Serial1, we use software serial for output as the adapter uses Serial
#ifdef ARDUINO_AVR_UNO
#include <SoftwareSerial.h>
SoftwareSerial mySerial(A2, A3);
#else
#define mySerial Serial
#endif

#if defined(ESP32) && !defined(Serial1)
HardwareSerial Serial1(1);
#endif

void setup() {
	mySerial.begin(115200);
	while (!mySerial);

	byte version;
	for (bool success = false; !success || !version; DDInitOBD(version));

	mySerial.print("Found OBD-II adapter. Firmware version: ");

	// weird version number packing
	mySerial.print(version / 10);
	mySerial.print('.');
	mySerial.println(version % 10);

	// TODO: get error codes on startup and do something with them
}

unsigned long prevtime = 0UL;
const unsigned long INTERVAL = 160; // 160ms is roughly 60Hz
DashData_s d_data;
bool status = true;

void loop() {
	if (millis() - prevtime >= INTERVAL) {
		// 1. get data
		status = DDGetDashData(d_data);

		// 2. refresh/draw display
	}
}
