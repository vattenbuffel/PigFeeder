#ifndef SLEEP_H_
#define SLEEP_H_

#include "Arduino.h"

bool sleep_init();

void sleep_activate();

bool sleep_time_set(uint64_t time_us);

uint64_t sleep_time_get();

#endif