#include "battery.h"

#define BATTERY_GPIO_1 35
#define BATTERY_GPIO_2 34
#define R1 1000000.f
#define R2 220000.f

bool battery_init(){
    pinMode(BATTERY_GPIO_1, INPUT);
    pinMode(BATTERY_GPIO_2, INPUT);

    return true;
}

float ReadVoltage(byte pin){
  double reading = analogRead(pin); // Reference voltage is 3v3 so maximum reading is 3v3 = 4095 in range 0 to 4095
  if(reading < 1 || reading > 4095) return 0;
  // return -0.000000000009824 * pow(reading,3) + 0.000000016557283 * pow(reading,2) + 0.000854596860691 * reading + 0.065440348345433;
  return (-0.000000000000016 * pow(reading,4) + 0.000000000118171 * pow(reading,3)- 0.000000301211691 * pow(reading,2)+ 0.001109019271794 * reading + 0.034143524634089)*1.03;
} // Added an improved polynomial, use either, comment out as required

float map_voltage(float volt){
    return volt * (R1+R2)/R2;
}

float battery_get(int battery_number){
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