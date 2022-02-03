#ifndef MOTOR_H_
#define MOTOR_H_
#include "Arduino.h"

bool motor_init();

bool motor_start();

bool motor_stop();

bool motor_time_set_s(float time_s);

float motor_time_get_s();

#endif /* MOTOR_H_ */