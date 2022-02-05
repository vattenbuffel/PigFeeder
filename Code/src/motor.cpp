#include "motor.h"
#include "sleep.h"

static float time_on_s = 5;
static bool on = false;
static uint32_t time_motor_start_ms = 0;

bool motor_init(){
    printf("%s not implemented\n", __func__);
    return true;
}

bool motor_start(){
    printf("%s not implemented\n", __func__);
    sleep_disable();
    return true;
}

bool motor_stop(){
    printf("%s not implemented\n", __func__);
    sleep_enable();
    return true;
}

bool motor_loop(){
    /* Motor timeout */
    if(on && time_on_s*1000 + time_motor_start_ms > millis()){
        printf("Motor's active time is out. Turning off\n");
        return motor_stop();
    }
    return true;
}

bool motor_time_set_s(float time_s){
    printf("%s not implemented\n", __func__);
    return true;
}

float motor_time_get_s(){
    return time_on_s;
}
