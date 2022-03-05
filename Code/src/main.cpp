#include "Arduino.h"
#include "sms.h"
#include "battery.h"
#include "sleep.h"
#include "commands.h"
#include "motor.h"
#include "config.h"

#define BLUE_LED 13

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
		while (1) {
			delay(-1);
		}
	}

	analogReadResolution(12);

	// Publish started message with motor time on and batteries
	char sms_start[256];
	snprintf(sms_start, sizeof(sms_start), "Started. Battery 1: %f v, battery 2: %f v, motor on time: %f s, battery low: %f v, sleep time: %f s, time between warnings: %f s", battery_get_v(1), battery_get_v(2), motor_time_get_s(), -1.f, sleep_time_get_s(), -1.f);
    sms_send(NUMBER_NOA, sms_start);
    // sms_send(NUMBER_OLOF, sms_start);
    printf("Sent started sms: %s\n", sms_start);
	printf("Done with setup\n");
}

void loop()
{
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

		// sms_send(sms_number, "received sms");
	}

	battery_loop();
	motor_loop();

	digitalWrite(BLUE_LED, LOW);
	sleep_go();

	// sms_send("+46706628353", "woke up");
}