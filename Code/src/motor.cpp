#include "motor.h"
#include "sleep.h"

#define MOTOR_GPIO 14
#define MOTOR_LED_GPIO 33

static float time_on_s = 5;
static bool on = false;
static uint32_t time_motor_start_ms = 0;

bool motor_init(){
    pinMode(MOTOR_GPIO, OUTPUT);
    digitalWrite(MOTOR_GPIO, LOW);
    pinMode(MOTOR_LED_GPIO, OUTPUT);
    digitalWrite(MOTOR_LED_GPIO, LOW);
    return true;
}

/**
 * @brief Sets motor gpio high and disables sleep
 * 
 * @return true 
 * @return false 
 */
bool motor_start(){
    digitalWrite(MOTOR_GPIO, HIGH);
    digitalWrite(MOTOR_LED_GPIO, HIGH);
    sleep_disable();
    return true;
}

bool motor_stop(){
    digitalWrite(MOTOR_GPIO, LOW);
    digitalWrite(MOTOR_LED_GPIO, LOW);
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
    time_on_s = time_s;
    return true;
}

float motor_time_get_s(){
    return time_on_s;
}
