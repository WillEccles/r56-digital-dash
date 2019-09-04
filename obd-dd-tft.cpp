#include "obd-dd-tft.h"

#ifdef __AVR_ATmega328P__
// SPI pins for the UNO
#define RA8875_INT 8
#define RA8875_CS 10
#define RA8875_RESET 9
#elif defined(__AVR_ATmega2560__)
// SPI pins for the Mega
#define RA8875_INT 45
#define RA8875_CS 53
#define RA8875_RESET 43
#endif

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
	tft.textEnlarge(3);
	tft.textMode();
	tft.textTransparent(RA8875_WHITE);
#if 0
	tft.textSetCursor(10, 10);
	tft.textWrite("Hello, world!");
	tft.textSetCursor(10, 80);
	tft.textWrite("Hello, world 2.0!");
#endif

	return true;
}

#define FLOAT_F "%01d.%01d"
#define FLOAT_CONV(f) (int)(f), (int)((f)*10.f)%10

void updateTFT(const DashData_s& data) {
    static char rpmbuffer[30];
    static char speedbuffer[30];
    static char fuelbuffer[30];
    static char tempbuffer[30];
    static char boostbuffer[30];
	//tft.fillScreen(RA8875_BLACK);

	tft.textMode();
	tft.textColor(RA8875_WHITE, RA8875_BLACK);

    snprintf(rpmbuffer, 30, "Engine RPM: %d   ", data.rpm);
    snprintf(speedbuffer, 30, "Speed: %d   ", data.speed.mph);
    snprintf(fuelbuffer, 30, "Fuel: " FLOAT_F "%%   ", FLOAT_CONV(data.fuel*100.f));
    snprintf(tempbuffer, 30, "Water Temp: %d\x00B0""C    ", data.coolant_temp);
    snprintf(boostbuffer, 30, "Vac/Boost: " FLOAT_F "   ", FLOAT_CONV(data.boost_pressure));

    tft.textSetCursor(10, 10);
    tft.textWrite(rpmbuffer);
    tft.textSetCursor(10, 75);
    tft.textWrite(speedbuffer);
    tft.textSetCursor(10, 150);
    tft.textWrite(fuelbuffer);
    tft.textSetCursor(10, 225);
    tft.textWrite(tempbuffer);
    tft.textSetCursor(10, 300);
    tft.textWrite(boostbuffer);

    /*
	tft.textSetCursor(10, 10);
	tft.print("Engine RPM: ");
	tft.print(data.rpm);

	tft.print("Speed: ");
	tft.print(data.speed.mph);

	tft.print("Fuel: ");
	tft.print(data.fuel);
	tft.print('%');

	tft.print("Water Temp: ");
	tft.print(data.coolant_temp);
	tft.print("\x00B0" "C"); // degree sign

	tft.print("Vac/Boost: ");
	tft.print(data.boost_pressure, 1);
    */
}

void updateTFT_Logo(const char* status) {
}
