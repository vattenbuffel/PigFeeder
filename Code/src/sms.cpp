#include "sms.h"
#include "Adafruit_FONA.h"

#define NUMBER_OLOF ""
#define NUMBER_NOA "+46706628353"

#define SIM800L_RX 27
#define SIM800L_TX 26
#define SIM800L_PWRKEY 4
#define SIM800L_RST 5
#define SIM800L_POWER 23

char replybuffer[255];

HardwareSerial *sim800lSerial = &Serial1;
Adafruit_FONA sim800l = Adafruit_FONA(SIM800L_PWRKEY);

char sim800lNotificationBuffer[64]; //for notifications from the FONA
char smsBuffer[250];
char callerIDbuffer[32]; //we'll store the SMS sender number in here

bool sms_init()
{
    pinMode(SIM800L_POWER, OUTPUT);
    digitalWrite(SIM800L_POWER, HIGH);

// Make sure sim800l library isn't printing a lot of garbage
#ifdef ADAFRUIT_FONA_DEBUG
    printf("Undefine ADAFRUIT_FONA_DEBUG to avoid garbage prints\n");
    while (1)
    {
        delay(100000);
    }
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

    // sim800l.sendSMS("NUMBER_NOA", "Started");
    // sim800l.sendSMS("NUMBER_OLOF", "Started");
    // printf("Sent started sms\n");

    return true;
}

bool sms_send(char *number, char *msg)
{
    printf("sending sms with msg %s to number %s\n", msg, number);
    return sim800l.sendSMS(number, msg);
}

bool sms_received_get(char **number_sending, char **msg)
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

        // Add a terminal NULL to the notification string
        *bufPtr = 0;

        // Scan the notification string for an SMS received notification.
        // If it's an SMS message, we'll get the slot number in 'slot'
        if (1 == sscanf(sim800lNotificationBuffer, "+CMTI: \"SM\",%d", &slot))
        {

            // Retrieve SMS sender address/phone number.
            if (!sim800l.getSMSSender(slot, callerIDbuffer, 31))
            {
                printf("Received sms but couldn't find SMS message in slot!\n");
            }

            // Retrieve SMS value.
            uint16_t smslen;
            // Pass in buffer and max len!
            if (sim800l.readSMS(slot, smsBuffer, 250, &smslen))
            {
                printf("Received sms from %s with msg: %s\n", callerIDbuffer, smsBuffer);
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

            *number_sending = callerIDbuffer;
            *msg = smsBuffer;
            return true;
        }
    }
    return false;
}

bool sms_received_exists()
{
    return sim800l.available() > 0;
}
