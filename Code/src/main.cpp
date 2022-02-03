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
	if (sms_received_get(&sms_number, &sms_msg))
	{
		sms_send(sms_number, "received sms");
	}

	sleep_activate();

	// sms_send("+46706628353", "woke up");
}