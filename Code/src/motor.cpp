#include "motor.h"
#include "sleep.h"

#define MOTOR_GPIO 14
#define MOTOR_LED_GPIO 33

static float time_on_s = 1;
static bool on = false;
static uint64_t time_motor_start_ms = 0;

bool motor_init(){
    pinMode(MOTOR_GPIO, OUTPUT);
    digitalWrite(MOTOR_GPIO, LOW);
    pinMode(MOTOR_LED_GPIO, OUTPUT);
    digitalWrite(MOTOR_LED_GPIO, LOW);
    return true;
}

bool motor_start(){
    printf("Motor start\n");
    digitalWrite(MOTOR_GPIO, HIGH);
    digitalWrite(MOTOR_LED_GPIO, HIGH);
    on = true;
    time_motor_start_ms = millis();
    sleep_disable();
    return true;
}

bool motor_stop(){
    printf("Motor stop\n");
    digitalWrite(MOTOR_GPIO, LOW);
    digitalWrite(MOTOR_LED_GPIO, LOW);
    on = false;
    sleep_enable();
    return true;
}

bool motor_loop(){
    /* Motor timeout */
    bool done = time_on_s*1000 + time_motor_start_ms < millis();
    if(on && done){
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
