#include "obd-dd-oled.h"

#include "chloe_logo.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define LOGO_X ((SCREEN_WIDTH - CHLOE_LOGO_WIDTH) / 2)
#define LOGO_Y ((SCREEN_HEIGHT - CHLOE_LOGO_HEIGHT) / 2)

#define MODE_COUNT ((int32_t)2)

static const char* modes[MODE_COUNT] = {
	"STAT",
	"INFO",
};

#define OLED_RESET 9
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

static int32_t dmode = 0;
static byte _obdver = 0;

static char* _vin = nullptr;

const char* debugtitle = "Debug";

static inline uint32_t ralign(const uint32_t& len) {
	return 1 + SCREEN_WIDTH - (6 * len); // characters are 6px wide I think
}

static inline uint32_t calign(const uint32_t& len) {
	return 1 + (SCREEN_WIDTH / 2) - (6 * len / 2);
}

void initOLED(const byte& obdver) {
	_obdver = obdver;

	display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

	// make sure the display is cleared
	display.clearDisplay();
	display.display();

	// set display settings
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setTextWrap(false);
	display.cp437(true);
}

static void drawData(const DashData_s& data) {
	if (_vin == nullptr) {
		_vin = data.VIN;
	}

	display.setTextColor(BLACK, WHITE);
	display.setCursor(0, 0);
	display.print("Stat                              "); // make sure the whole top is white
	display.setCursor(ralign(4), 0);
	display.print("Unit");
	display.setTextColor(WHITE, BLACK);

	// print RPM
	display.setCursor(0, 8);
	display.print("Revs:  ");
	display.print(data.rpm);
	display.setCursor(ralign(3), 8);
	display.print("RPM");

	// print speed
	display.setCursor(0, 16);
	display.print("Spd:   ");
	display.print(data.speed.mph);
	display.setCursor(ralign(3), 16);
	display.print("MPH");

	// print fuel
	display.setCursor(0, 24);
	display.print("Fuel:  ");
	display.print(data.fuel, 0);
	display.setCursor(ralign(1), 24);
	display.print('%');

	// display coolant temp
	display.setCursor(0, 32);
	display.print("WTemp: ");
	display.print(data.coolant_temp);
	display.setCursor(ralign(1), 32);
	display.print('C');

	// display oil temp
	display.setCursor(0, 40);
	display.print("OTemp: ");
	display.print(data.oil_temp);
	display.setCursor(ralign(1), 40);
	display.print('C');

	// display boost pressure
	display.setCursor(0, 48);
	display.print("Boost: ");
	display.print(data.boost_pressure, 1);
	display.setCursor(ralign(3), 48);
	display.print("PSI");
}

static void drawInfo() {
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
}

void updateOLED(const DashData_s& data) {
	display.clearDisplay();

	switch(dmode) {
	case 0: // stats
		drawData(data);
		break;
	case 1: // info
		drawInfo();
		break;
	default:
		break;
	}

	// display mode title
	display.setTextColor(BLACK, WHITE);
	display.setCursor(0,0);
	display.print("                           ");
	display.setCursor(calign(strlen(modes[dmode])), 0);
	display.print(modes[dmode]);
	display.setTextColor(WHITE, BLACK);

	// display modes
	display.setCursor(0, 56);
	for (int i = 0; i < MODE_COUNT; i++) {
		if (dmode == i) {
			display.setTextColor(BLACK, WHITE);
		}
		display.print(modes[i]);
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
	display.setCursor(calign(strlen(debugtitle)), 0);
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

void updateOLED_Startup() {
	display.clearDisplay();
	display.drawBitmap(LOGO_X, LOGO_Y, chloe_logo, CHLOE_LOGO_WIDTH, CHLOE_LOGO_HEIGHT, WHITE);
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
