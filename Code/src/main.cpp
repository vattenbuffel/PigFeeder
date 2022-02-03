#include "sms.h"

void setup()
{
	Serial.begin(115200);
	sms_init();
}


void loop()
{
	char* sms_msg, *sms_number;
	if(sms_received_get(&sms_number, &sms_msg)){
		sms_send(sms_number, "received sms");
	}
	delay(100);
}