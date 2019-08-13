#ifndef OBD_DD_TFT_H
#define OBD_DD_TFT_H

#include <SPI.h>
#include "Adafruit_GFX.h"
#include "Adafruit_RA8875.h"
#include "obdwrapper.h"

extern bool initTFT();

extern void updateTFT(const DashData_s& data);

extern void updateTFT_Logo(const char* status);

#endif
