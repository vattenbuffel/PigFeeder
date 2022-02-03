#ifndef BATTERY_H_
#define BATTERY_H_

#include "Arduino.h"


bool battery_init();

float battery_read(int batter_number);

#endif /* BATTERY_H_ */