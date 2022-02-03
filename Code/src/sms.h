#ifndef SMS_H_
#define SMS_H_

#include "Arduino.h"

bool sms_init();

bool sms_send( char *number, char *msg);

int sms_get(char** number_sending, char** msg);

bool sms_waiting_cnt_get();


#endif /* SMS_H_ */