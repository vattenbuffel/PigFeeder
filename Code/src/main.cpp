#include "sms.h"
#include "battery.h"

void sleep()
{
	/*
  First we configure the wake up source
  We set our ESP32 to wake up every 5 seconds
  */
	esp_sleep_enable_timer_wakeup(5 * 100000);
	printf("Setup ESP32 to sleep for every %d Seconds\n", 5000000);

	/*
  Next we decide what all peripherals to shut down/keep on
  By default, ESP32 will automatically power down the peripherals
  not needed by the wakeup source, but if you want to be a poweruser
  this is for you. Read in detail at the API docs
  http://esp-idf.readthedocs.io/en/latest/api-reference/system/deep_sleep.html
  Left the line commented as an example of how to configure peripherals.
  The line below turns off all RTC peripherals in deep sleep.
  */
	//esp_deep_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
	//Serial.println("Configured all RTC Peripherals to be powered down in sleep");

	/*
  Now that we have setup a wake cause and if needed setup the
  peripherals state in deep sleep, we can now start going to
  deep sleep.
  In the case that no wake up sources were provided but deep
  sleep was started, it will sleep forever unless hardware
  reset occurs.
  */
}

void setup()
{
	Serial.begin(115200);
	bool res = battery_init();
	// res &= sms_init();
	if (!res) {
		printf("Failed with init\n");
		while (1) {
			delay(-1);
		}
	}
	sleep();

	analogReadResolution(12);
	printf("Done with init\n");
}

void loop()
{

	// char *sms_msg, *sms_number;
	// if (sms_received_get(&sms_number, &sms_msg))
	// {
	// 	sms_send(sms_number, "received sms");
	// }
	delay(100);

	printf("Going to sleep now\n");
	  delay(1000);
	  Serial.flush();
	esp_deep_sleep_start();
	// printf("This will never be printed\n");
}