#include "obd-dd-tft.h"

#define RA8875_INT 8
#define RA8875_CS 10
#define RA8875_RESET 9

Adafruit_RA8875 tft = Adafruit_RA8875(RA8875_CS, RA8875_RESET);

bool initTFT() {
	if (!tft.begin(RA8875_800x480)) {
		return false;
	}

	tft.displayOn(true);
	tft.GPIOX(true);
	tft.PWM1config(true, RA8875_PWM_CLK_DIV1024);
	tft.PWM1out(255);
	tft.fillScreen(RA8875_BLACK);
	tft.cp437(true);
	tft.setFont()
	tft.textEnlarge(3);
	tft.textMode();
	tft.textTransparent(RA8875_WHITE);
	tft.textSetCursor(10, 10);
	tft.textWrite("Hello, world!");
	tft.textSetCursor(10, 80);
	tft.textWrite("Hello, world 2.0!");

	return true;
}

void updateTFT(const DashData_s& data) {
	tft.fillScreen(RA8875_BLACK);

	//tft.textMode();
	//tft.textTransparent(RA8875_WHITE);

	//tft.setCursor(10, 10);
	tft.print("Engine RPM: ");
	tft.println(data.rpm);

	tft.print("Speed: ");
	tft.println(data.speed.mph);

	tft.print("Fuel: ");
	tft.print(data.fuel);
	tft.println('%');

	tft.print("Water Temp: ");
	tft.print(data.coolant_temp);
	tft.println("\x00B0" "C"); // degree sign

	tft.print("Vac/Boost: ");
	tft.println(data.boost_pressure, 1);
}

void updateTFT_Logo(const char* status) {
}
