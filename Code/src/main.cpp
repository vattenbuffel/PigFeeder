#include "Adafruit_FONA.h"

#define SIM800L_RX 27
#define SIM800L_TX 26
#define SIM800L_PWRKEY 4
#define SIM800L_RST 5
#define SIM800L_POWER 23

char replybuffer[255];

HardwareSerial *sim800lSerial = &Serial1;
Adafruit_FONA sim800l = Adafruit_FONA(SIM800L_PWRKEY);

uint8_t readline(char *buff, uint8_t maxbuff, uint16_t timeout = 0);

String smsString = "";

void setup()
{
	pinMode(SIM800L_POWER, OUTPUT);
	digitalWrite(SIM800L_POWER, HIGH);

	Serial.begin(115200);


	// Make sure sim800l library isn't printing a lot of garbage
	#ifdef ADAFRUIT_FONA_DEBUG
	printf("Undefine ADAFRUIT_FONA_DEBUG to avoid garbage prints\n");
	while(1){delay(100000);}
	#endif

	printf("ESP32 with GSM SIM800L\n");
	printf("Initializing....\n");

	delay(10000);

	// Make it slow so its easy to read!
	sim800lSerial->begin(115200, SERIAL_8N1, SIM800L_TX, SIM800L_RX);
	if (!sim800l.begin(*sim800lSerial))
	{
		printf("Couldn't find GSM SIM800L\n");
		while (1)
			;
	}
	printf("GSM SIM800L is OK\n");

	char imei[16] = {0}; // MUST use a 16 character buffer for IMEI!
	uint8_t imeiLen = sim800l.getIMEI(imei);
	if (imeiLen > 0)
	{
		printf("SIM card IMEI: %s", imei);
	}

	// Set up the FONA to send a +CMTI notification when an SMS is received
	sim800lSerial->print("AT+CNMI=2,1\r\n");

	printf("GSM SIM800L Ready\n");

	sim800l.sendSMS("+46706628353", "Started");
	printf("Sent sms\n");
}

char sim800lNotificationBuffer[64]; //for notifications from the FONA
char smsBuffer[250];

void loop()
{
	char *bufPtr = sim800lNotificationBuffer; //handy buffer pointer

	if (sim800l.available())
	{
		int slot = 0; // this will be the slot number of the SMS
		int charCount = 0;

		// Read the notification into fonaInBuffer
		do
		{
			*bufPtr = sim800l.read();
			Serial.write(*bufPtr);
			delay(1);
		} while ((*bufPtr++ != '\n') && (sim800l.available()) && (++charCount < (sizeof(sim800lNotificationBuffer) - 1)));

		//Add a terminal NULL to the notification string
		*bufPtr = 0;

		//Scan the notification string for an SMS received notification.
		//  If it's an SMS message, we'll get the slot number in 'slot'
		if (1 == sscanf(sim800lNotificationBuffer, "+CMTI: \"SM\",%d", &slot))
		{
			char callerIDbuffer[32]; //we'll store the SMS sender number in here

			// Retrieve SMS sender address/phone number.
			if (!sim800l.getSMSSender(slot, callerIDbuffer, 31))
			{
				printf("Didn't find SMS message in slot!\n");
				
			}
			printf("Received sms FROM: %s\n", callerIDbuffer);

			// Retrieve SMS value.
			uint16_t smslen;
			// Pass in buffer and max len!
			if (sim800l.readSMS(slot, smsBuffer, 250, &smslen))
			{
				smsString = String(smsBuffer);
				printf("%s\n", smsString);
			}

			// if (!sim800l.sendSMS(callerIDbuffer, "Relay is activated."))
			if (!sim800l.sendSMS("+46706628353", "Relay is activated."))
			{
				printf("failed to send response\n");
			}

			while (1)
			{
				if (sim800l.deleteSMS(slot))
				{
					printf("deleted sms\n");
					break;
				}
				else
				{
					sim800l.print(F("AT+CMGD=?\r\n"));
				}
			}
		}
	}
}