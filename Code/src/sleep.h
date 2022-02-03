#ifndef SLEEP_H_
#define SLEEP_H_

#include "Arduino.h"

bool sleep_init();

void sleep_activate();

bool sleep_time_set_us(uint64_t time_us);

bool sleep_time_set_s(float time_s);

uint64_t sleep_time_get_us();

float sleep_time_get_s();

bool sleep_disable();

bool sleep_enable();

#endif