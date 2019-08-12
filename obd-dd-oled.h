#ifndef OBD_DD_OLED_H
#define OBD_DD_OLED_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "obdwrapper.h"

/* Initializes the OLED and anything related to it. */
extern bool initOLED();

/* Put this inside of loop() */
extern void updateOLED(const DashData_s& data);

/* Updates the display to show just the set of supported PIDs and anything else debug-related. */
extern void updateOLED_Debug();

/* Use this to display the logo and an optional status (NULL if not required). */
extern void updateOLED_Logo(const char* status);
#if 0
/* To be called when the "previous mode" button is pressed. */
extern void prevModeOLED();

/* To be called when the "next mode" button is pressed. */
extern void nextModeOLED();
#endif
/* Enable or disable display dimming. */
extern void dimOLED(bool dim);

#endif
