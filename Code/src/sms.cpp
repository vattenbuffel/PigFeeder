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
    // Make sure sim800l library isn't printing a lot of garbage
    #ifdef ADAFRUIT_FONA_DEBUG
        printf("Undefine ADAFRUIT_FONA_DEBUG to avoid garbage prints\n");
        while (1)
        {
            delay(100000);
        }
    #endif

    pinMode(SIM800L_POWER, OUTPUT);
    digitalWrite(SIM800L_POWER, HIGH);

    printf("SMS initializing....\n");

    delay(10000);

    // Make it slow so its easy to read!
    sim800lSerial->begin(115200, SERIAL_8N1, SIM800L_TX, SIM800L_RX);
    if (!sim800l.begin(*sim800lSerial))
    {
        printf("Couldn't find GSM SIM800L\n");
        while (1)
            delay(100000);
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

    delay(1000);
    printf("GSM SIM800L Ready\n");

    sim800l.sendSMS(NUMBER_NOA, "Started");
    // sim800l.sendSMS(NUMBER_OLOF, "Started");
    printf("Sent started sms\n");

    // Cleased received sms
    int8_t i = 0;
    while(-1 == sim800l.getNumSMS()){delay(10);}
    while(0 != sim800l.getNumSMS()){
        sim800l.deleteSMS(i);
        printf("cleasning idx %u\n", i);
        i++;
    }

    return true;
}

bool sms_send(char *number, char *msg)
{
    printf("sending sms with msg %s to number %s\n", msg, number);
    return sim800l.sendSMS(number, msg);
}

bool sms_get(char **number_sending, char **msg)
{
    uint16_t dummy;
    int num_sms_left = sim800l.getNumSMS(); 
    // int slot = num_sms_left-1;
    int slot = num_sms_left;
    /* Try to read sms and sender number */
    if(sim800l.readSMS(slot, smsBuffer, 250, &dummy) && sim800l.getSMSSender(slot, callerIDbuffer, 32)){
        printf("Received sms from %s with msg: %s\n", callerIDbuffer, smsBuffer);
        while (1){
            if (sim800l.deleteSMS(slot)){
                printf("deleted sms\n");
                break;
            }
            else{
                sim800l.print(F("AT+CMGD=?\r\n"));
            }
        }
    } else{
        printf("Failed to read sms\n");
        return false;
    }

    *number_sending = callerIDbuffer;
    *msg = smsBuffer;
    return true;
}

bool sms_waiting_cnt_get()
{
    return sim800l.getNumSMS() > 0;
}
