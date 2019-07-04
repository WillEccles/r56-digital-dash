#ifndef OBD_DD_OLED_H
#define OBD_DD_OLED_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "obdwrapper.h"

/* Initializes the OLED and anything related to it. */
extern void initOLED();

/* Put this inside of loop() */
extern void updateOLED(const DashData_s& data);

/* To be called when the "previous mode" button is pressed. */
extern void prevModeOLED();

/* To be called when the "next mode" button is pressed. */
extern void nextModeOLED();

#endif
