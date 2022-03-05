#include "battery.h"
#include "sms.h"
#include "config.h"


#define BATTERY_GPIO_1 32
#define BATTERY_GPIO_2 34
#define R1 1000000.f
#define R2 220000.f

static float battery_low_v =  BATTERY_LOW_V;
static float sms_time_s = BATTERY_SMS_TIME_S;

bool battery_init(){
    pinMode(BATTERY_GPIO_1, INPUT);
    pinMode(BATTERY_GPIO_2, INPUT);

    return true;
}

bool battery_time_set_s(float time_s){
    sms_time_s = time_s;
    return true;
}

float battery_time_get_s(){
    return sms_time_s;
}

bool battery_low_set_v(float low_voltage_v){
    battery_low_v = low_voltage_v;
    return true;
}

float battery_low_get_v(){
    return battery_low_v;
}


static float ReadVoltage(byte pin){
  double reading = analogRead(pin); // Reference voltage is 3v3 so maximum reading is 3v3 = 4095 in range 0 to 4095
  reading = analogRead(pin);  // Read twice to "prepare" the pins
  if(reading < 1 || reading > 4095) return 0;
  reading =  -0.000000000009824 * pow(reading,3) + 0.000000016557283 * pow(reading,2) + 0.000854596860691 * reading + 0.065440348345433;
  reading += 0.05; // compensate for constant error
  return reading;
} // Added an improved polynomial, use either, comment out as required

float map_voltage(float volt){
    return volt * (R1+R2)/R2;
}

float battery_get_v(int battery_number){
    byte pin;
    if (battery_number == 1){
        pin = BATTERY_GPIO_1;
    }
    else if(battery_number == 2){
        pin = BATTERY_GPIO_2;
    }
    else{
        printf("Invalid battery number\n");
        return false;
    }

    float volt = ReadVoltage(pin);
    float volt_map = map_voltage(volt);
    return volt_map;
}

void battery_loop(){
    static bool sent_sms = false;
    static unsigned long sms_prev_ms = 0;

    if(sent_sms && millis() - sms_prev_ms  < (uint32_t)(sms_time_s * 1000)){
        /* Too soon to send sms again */
        return;
    }

    char sms_text[256];
    bool send = false;
    if (battery_get_v(1) < battery_low_v){
        send = true;
        snprintf(sms_text+strlen(sms_text), sizeof(sms_text) - strlen(sms_text), "Battry 1 low level: %f v. ", battery_get_v(1));
    }
    if (battery_get_v(2) < battery_low_v){
        send = true;
        snprintf(sms_text+strlen(sms_text), sizeof(sms_text) - strlen(sms_text), "Battry 2 low level: %f v. ", battery_get_v(2));
    }

    if(send){
        printf("battery too low. Sending warning sms\n");
        sms_send(NUMBER_NOA, sms_text);
        // sms_send(NUMBER_OLOF, sms_text);
        sent_sms = true;
        sms_prev_ms = millis();
    }


    
}