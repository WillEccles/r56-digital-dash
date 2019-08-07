#include "obd-dd-oled.h"

//#include "chloe_logo.h"
#include "cactus_logo.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define LOGO_X (((SCREEN_WIDTH - CACTUS_LOGO_WIDTH) / 2) + (CACTUS_LOGO_PADDING / 2))
#define LOGO_Y ((SCREEN_HEIGHT - CACTUS_LOGO_HEIGHT) / 2)

#define MODE_COUNT ((uint8_t)2)

const PROGMEM char modes[] = "STAT\0INFO";

#define OLED_RESET 9
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

uint8_t dmode = 0;

const PROGMEM char debugtitle[] = "Debug";

#define RALIGN(len) (1 + SCREEN_WIDTH - (6 * (len)))
#define CALIGN(len) (1 + (SCREEN_WIDTH / 2) - (6 * (len) / 2))

bool initOLED() {
	if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) return false;

	// make sure the display is cleared
	display.clearDisplay();
	display.display();

	// set display settings
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setTextWrap(false);
	display.cp437(true);

	return true;
}

void drawData(const DashData_s& data) {
	//if (_vin == nullptr) {
		//_vin = data.VIN;
	//}

	// print RPM
	display.setCursor(0, 8);
	display.print("Revs:  ");
	display.print(data.rpm);
	display.setCursor(RALIGN(3), 8);
	display.print("RPM");

	// print speed
	display.setCursor(0, 16);
	display.print("Spd:   ");
	display.print(data.speed.mph);
	display.setCursor(RALIGN(3), 16);
	display.print("MPH");

	// print fuel
	display.setCursor(0, 24);
	display.print("Fuel:  ");
	display.print(data.fuel, 0);
	display.setCursor(RALIGN(1), 24);
	display.print('%');

	// display coolant temp
	display.setCursor(0, 32);
	display.print("WTemp: ");
	display.print(data.coolant_temp);
	display.setCursor(RALIGN(1), 32);
	display.print('C');

	// display oil temp
	display.setCursor(0, 40);
	display.print("OTemp: ");
	display.print(data.oil_temp);
	display.setCursor(RALIGN(1), 40);
	display.print('C');

	// display boost pressure
	display.setCursor(0, 48);
	display.print("Boost: ");
	display.print(data.boost_pressure, 1);
	display.setCursor(RALIGN(3), 48);
	display.print("PSI");
}

static void drawInfo() {
#if 0
	display.setCursor(0, 0);
	display.setTextColor(BLACK, WHITE);
	display.print("                      ");
	display.setTextColor(WHITE, BLACK);

	display.setCursor(0, 8);
	if (_obdver) {
		display.print("Adapter ver. ");
		display.print(_obdver / 10);
		display.print('.');
		display.print(_obdver % 10);
	} else {
		display.print("No adapter found!");
	}

	if (_vin) {
		display.setCursor(0, 16);
		display.print(_vin);
	}
#endif
}

void updateOLED(const DashData_s& data) {
	display.clearDisplay();

	switch(dmode) {
	case 0: // stats
		drawData(data);
		break;
	case 1: // info
		//drawInfo();
		break;
	default:
		break;
	}

	// display mode title
	display.setTextColor(BLACK, WHITE);
	display.setCursor(0,0);
	display.print("                           ");
	display.setCursor(CALIGN(4), 0);
	display.print(modes[dmode*4]);
	display.setTextColor(WHITE, BLACK);

	// display modes
	display.setCursor(0, 56);
	for (int i = 0; i < MODE_COUNT; i++) {
		if (dmode == i) {
			display.setTextColor(BLACK, WHITE);
		}
		display.print(modes[i*4]);
		display.setTextColor(WHITE, BLACK);
		display.print(' ');
	}

	// write buffer to hardware
	display.display();
}

void updateOLED_Debug() {
	display.clearDisplay();

	// display mode title
	display.setTextColor(BLACK, WHITE);
	display.setCursor(0,0);
	display.print("                           ");
	display.setCursor(CALIGN(strlen(debugtitle)), 0);
	display.print(debugtitle);
	display.setTextColor(WHITE, BLACK);

// convenience macro because this will have to change often, most likely
#define __DISPPID__(pidname) {\
		display.setCursor(0, 8*(__COUNTER__+1));\
		display.print(#pidname ": ");\
		display.print(SupportedCodes.pidname);\
	}

	__DISPPID__(oil_temp);
	__DISPPID__(turbo_rpm);
	__DISPPID__(turbo_temp);

#undef __DISPPID__

	display.display();
}

void updateOLED_Logo() {
	display.clearDisplay();
	display.drawBitmap(LOGO_X, LOGO_Y, cactus_logo, CACTUS_LOGO_WIDTH, CACTUS_LOGO_HEIGHT, WHITE);
	display.display();
}

void nextModeOLED() {
	dmode = (dmode + 1) % MODE_COUNT;
}

void prevModeOLED() {
	dmode = (dmode ? dmode : MODE_COUNT) - 1;
}

void dimOLED(bool dim) {
	display.dim(dim);
}
