#ifndef MOTOR_H_
#define MOTOR_H_
#include "Arduino.h"

bool motor_init();


/**
 * @brief Sets motor gpio high and disables sleep
 * 
 * @return true 
 * @return false 
 */
bool motor_start();

/**
 * @brief Sets motor gpio to low and enables sleep
 * 
 * @return true 
 * @return false 
 */
bool motor_stop();

bool motor_loop();

bool motor_time_set_s(float time_s);

float motor_time_get_s();

#endif /* MOTOR_H_ */