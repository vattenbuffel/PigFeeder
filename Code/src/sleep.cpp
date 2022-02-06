#include "sleep.h"
#include "config.h"

static uint64_t sleep_time_us = SLEEP_TIME_US;
static bool enabled;

bool sleep_init(){
    sleep_time_set_us(sleep_time_us);
    enabled = true;
    return true;
}

void sleep_go(){
    if(enabled){
        printf("Going to sleep\n");
        delay(100);
        Serial.flush();
        esp_light_sleep_start();
        printf("woke up\n");
    } else{
        printf("%s: sleep is disabled\n", __func__);
    }
}


bool sleep_time_set_us(uint64_t time_us){
    sleep_time_us = time_us;
	esp_sleep_enable_timer_wakeup(sleep_time_us);
	printf("Setup ESP32 to sleep for %llu Seconds\n", sleep_time_us/1000000);
    return true;
}

bool sleep_time_set_s(float time_s){
    return sleep_time_set_us((uint64_t) (time_s*1000000));
}

uint64_t sleep_time_get_us(){
    return sleep_time_us;
}

float sleep_time_get_s(){
    return sleep_time_us/1000000.f;
}


bool sleep_disable(){
    enabled = false;
    return true;
}

bool sleep_enable(){
    enabled = true;
    return true;
}