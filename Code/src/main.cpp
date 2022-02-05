#include "sms.h"
#include "battery.h"
#include "sleep.h"
#include "commands.h"
#include "motor.h"

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

	// Publish a good started message here with motor time on and batteries
	printf("Done with setup\n");
}

void loop()
{
	digitalWrite(BLUE_LED, HIGH);
	/* Check for incoming sms */
	char *sms_msg, *sms_number;
	int sms_left;
	printf("There are %d sms left\n", sms_waiting_cnt_get());
	while((sms_left = sms_waiting_cnt_get()) > 0){
		if(false == sms_get(&sms_number, &sms_msg)){
			printf("Failed to get sms\n");
			continue;
		}

		command_handle(sms_msg, sms_number);

		printf("There are %d sms left\n", sms_left);
		// sms_send(sms_number, "received sms");
	}
	delay(500);

	/* Check for low battery levels */

	motor_loop();

	digitalWrite(BLUE_LED, LOW);
	sleep_go();

	// sms_send("+46706628353", "woke up");
}