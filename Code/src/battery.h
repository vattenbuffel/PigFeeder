#ifndef BATTERY_H_
#define BATTERY_H_

#include "Arduino.h"


bool battery_init();

/**
 * @brief Measures battery voltage and returns it. 
 * 
 * @param battery_number 1 or 2
 * @return float 
 */
float battery_get(int battery_number);

void battery_loop();

#endif /* BATTERY_H_ */