#include "sms.h"
#include "battery.h"
#include "sleep.h"

void setup()
{
	Serial.begin(115200);

	bool res = battery_init();
	res &= sms_init();
	res &= sleep_init();
	if (!res) {
		printf("Failed with init\n");
		while (1) {
			delay(-1);
		}
	}

	analogReadResolution(12);
	printf("Done with setup\n");
}

void loop()
{
	char *sms_msg, *sms_number;
	int sms_left;
	printf("There are %d sms left\n", sms_waiting_cnt_get());
	while((sms_left = sms_waiting_cnt_get()) > 0){
		sms_get(&sms_number, &sms_msg);
		printf("There are %d sms left\n", sms_left);
		// sms_send(sms_number, "received sms");
	}
	delay(500);

	// sleep_activate();

	// sms_send("+46706628353", "woke up");
}