#include "Arduino.h"
#include "sms.h"
#include "battery.h"
#include "sleep.h"
#include "commands.h"
#include "motor.h"
#include "config.h"

#define BLUE_LED 13

/**
 * @brief Blink blue led 5 timer then sleep, then repeat blink and sleep forever
 * 
 */
void error(){
	while (1) {
		/* Blink to signal error */
		for(int i = 0; i < 5; i++){
			delay(100);
			digitalWrite(BLUE_LED, HIGH);
			delay(100);
			digitalWrite(BLUE_LED, LOW);
		}
		sleep_go();
	}
}

void setup()
{
	Serial.begin(115200);
	pinMode(BLUE_LED, OUTPUT);
	digitalWrite(BLUE_LED, HIGH);

	bool res = battery_init();
	res &= sms_init();
	res &= sleep_init();
	res &= motor_init();
	if (!res) {
		printf("Failed with init\n");
		error();
	}

	analogReadResolution(12);

	// Publish started message with motor time on and batteries
	char sms_start[256];
	snprintf(sms_start, sizeof(sms_start), "Started. Battery 1: %f v, battery 2: %f v, motor on time: %f s, battery low: %f v, sleep time: %f s, time between warnings: %f s", battery_get_v(1), battery_get_v(2), motor_time_get_s(), battery_low_get_v(), sleep_time_get_s(), battery_time_get_s());
	sms_all(sms_start);
    printf("Sent started sms: %s\n", sms_start);
	printf("Done with setup\n");
}

void loop()
{
	/* Signal that esp is awake */
	digitalWrite(BLUE_LED, HIGH);

	/* Check for incoming sms */
	char *sms_msg, *sms_number;
	int sms_left;
	// printf("There are %d sms left\n", sms_waiting_cnt_get());
	while((sms_left = sms_waiting_cnt_get()) > 0){
		if(false == sms_get(&sms_number, &sms_msg)){
			printf("Failed to get sms\n");
			continue;
		}

		command_handle(sms_msg, sms_number);
	}

	battery_loop();
	motor_loop();

	/* Signal that esp is asleep */
	digitalWrite(BLUE_LED, LOW);
	sleep_go();
}