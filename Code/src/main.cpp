#include <Arduino.h>

// SIM card PIN (leave empty, if not defined)
const char simPIN[] = "";

// Your phone number to send SMS: + (plus sign) and country code, for Portugal +351, followed by phone number
// SMS_TARGET Example for Portugal +351XXXXXXXXX

/* Noa vimla */
#define SMS_TARGET "+46706628353"

/* Noas kontantkort */
// #define SMS_TARGET "+46738079358"

/* Hanna */
// #define SMS_TARGET "+46763107286"


// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM800	// Modem is SIM800
#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb

#include <Wire.h>
#include <TinyGsmClient.h>

// TTGO T-Call pins
#define MODEM_RST 5
#define MODEM_PWKEY 4
#define MODEM_POWER_ON 23
#define MODEM_TX 27
#define MODEM_RX 26
#define I2C_SDA 21
#define I2C_SCL 22

// Set serial for debug console (to Serial Monitor, default speed 115200)
#define SerialMon Serial
// Set serial for AT commands (to SIM800 module)
#define SerialAT Serial1

TinyGsm modem(SerialAT);

#define IP5306_ADDR 0x75
#define IP5306_REG_SYS_CTL0 0x00

bool setPowerBoostKeepOn(int en)
{
	Wire.beginTransmission(IP5306_ADDR);
	Wire.write(IP5306_REG_SYS_CTL0);
	if (en)
	{
		Wire.write(0x37); // Set bit1: 1 enable 0 disable boost keep on
	}
	else
	{
		Wire.write(0x35); // 0x37 is default reg value
	}
	return Wire.endTransmission() == 0;
}

void setup()
{
	// Set console baud rate
	SerialMon.begin(115200);

	// Keep power when running from battery
	Wire.begin(I2C_SDA, I2C_SCL);
	bool isOk = setPowerBoostKeepOn(1);
	SerialMon.println(String("IP5306 KeepOn ") + (isOk ? "OK" : "FAIL"));

	// Set modem reset, enable, power pins
	pinMode(MODEM_PWKEY, OUTPUT);
	pinMode(MODEM_RST, OUTPUT);
	pinMode(MODEM_POWER_ON, OUTPUT);
	digitalWrite(MODEM_PWKEY, LOW);
	digitalWrite(MODEM_RST, HIGH);
	digitalWrite(MODEM_POWER_ON, HIGH);

	// Set GSM module baud rate and UART pins
	SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
	delay(3000);

	// Restart SIM800 module, it takes quite some time
	// To skip it, call init() instead of restart()
	SerialMon.println("Initializing modem...");
	modem.restart();
	// use modem.init() if you don't need the complete restart

	// Unlock your SIM card with a PIN if needed
	if (strlen(simPIN) && modem.getSimStatus() != 3)
	{
		bool res = modem.simUnlock(simPIN);
		printf("unlock sim returned: %d\n", res);
	}

	// To send an SMS, call modem.sendSMS(SMS_TARGET, smsMessage)
	String smsMessage = "Hello from ESP32!";
	if (modem.sendSMS(SMS_TARGET, smsMessage))
	{
		SerialMon.println(smsMessage);
	}
	else
	{
		SerialMon.println("SMS failed to send");
	}
}

void loop()
{
	delay(1);
}