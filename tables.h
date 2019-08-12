#ifndef TABLES_H
#define TABLES_H

#include <Arduino.h>

// 0...255 -> 0...100
extern const uint8_t PERCENT_TABLE[256] PROGMEM;

// 0...255KMH -> MPH
extern const uint8_t KMH_MPH_TABLE[256] PROGMEM;

// 0...255kPa -> PSI
extern const float KPA_PSI_TABLE[256] PROGMEM;

#endif
