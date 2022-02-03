#include "sleep.h"

static uint64_t sleep_time_us = 30000000;

bool sleep_init(){
    sleep_time_set(sleep_time_us);
    return true;
}

void sleep_activate(){
    printf("Going to sleep\n");
    delay(100);
	Serial.flush();
	esp_light_sleep_start();
}


bool sleep_time_set(uint64_t time_us){
    sleep_time_us = time_us;
	esp_sleep_enable_timer_wakeup(sleep_time_us);
	printf("Setup ESP32 to sleep for every %d Seconds\n", sleep_time_us);
    return true;
}

uint64_t sleep_time_get(){
    return sleep_time_us;
}