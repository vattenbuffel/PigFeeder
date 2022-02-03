#ifndef SMS_H_
#define SMS_H_

#include "Arduino.h"

bool sms_init();

bool sms_send( char *number, char *msg);

bool sms_received_get(char** number_sending, char** msg);

bool sms_received_exists();



#endif /* SMS_H_ */