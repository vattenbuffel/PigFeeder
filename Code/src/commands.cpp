#include "commands.h"
#include "Arduino.h"
#include "sms.h"
#include "battery.h"
#include "motor.h"
#include "sleep.h"

static bool extract_number(char* str, double *number){

    int i;
    for(i = 0; i < strlen(str); i++){
        if(isdigit(str[i])){
            break;
        }
    }

    if(i == strlen(str)){
        printf("Found no number in str\n");
        return false;
    }

    /* -1 to remove the trailing space */
    str[i-1] = 0;
    printf("after removing number from string, string is: %s\n", str);

    *number = atof(str+i);
    return true;
}

static bool cmd_battery_get(char* sms_number){
    float b1 = battery_get(1);
    float b2 = battery_get(2);
    char msg[256];
    snprintf(msg, sizeof(msg), "Battery 1: %f, battery 2: %f", b1, b2);
    printf("%s %s\n", __func__, msg);
    return sms_send(sms_number, msg);
}

static bool cmd_motor_start(char* sms_number){
    char msg[256];
    snprintf(msg, sizeof(msg), "Motor start. Success: %d", motor_start());
    printf("%s %s\n", __func__, msg);
    return sms_send(sms_number, msg);
}

static bool cmd_motor_stop(char* sms_number){
    char msg[256];
    snprintf(msg, sizeof(msg), "Motor stop. Success: %d", motor_stop());
    printf("%s %s\n", __func__, msg);
    return sms_send(sms_number, msg);
}

static bool cmd_motor_time_set(char* sms_number, double number){
    char msg[256];
    snprintf(msg, sizeof(msg), "Motor on time set. Success: %d", motor_time_set_s((float) number));
    printf("%s %s\n", __func__, msg);
    return sms_send(sms_number, msg);
}

static bool cmd_motor_time_get(char* sms_number){
    char msg[256];
    snprintf(msg, sizeof(msg), "Motor on time get: %f s.", motor_time_get_s());
    printf("%s %s\n", __func__, msg);
    return sms_send(sms_number, msg);
}

static bool cmd_sleep_time_get(char* sms_number){
    char msg[256];
    snprintf(msg, sizeof(msg), "Sleep time get: %f s.", sleep_time_get_s());
    printf("%s %s\n", __func__, msg);
    return sms_send(sms_number, msg);
}

static bool cmd_sleep_time_set(char* sms_number, double number){
    char msg[256];
    snprintf(msg, sizeof(msg), "Sleep time set. Success: %d", sleep_time_set_s((float) number));
    printf("%s %s\n", __func__, msg);
    return sms_send(sms_number, msg);
}

static bool cmd_battery_low_get(char* sms_number){
    char msg[256];
    snprintf(msg, sizeof(msg), "Battery low get: %f v.", battery_low_get_v());
    printf("%s %s\n", __func__, msg);
    return sms_send(sms_number, msg);
}

static bool cmd_battery_low_set(char* sms_number, double number){
    char msg[256];
    snprintf(msg, sizeof(msg), "Battery low set. Success: %d", battery_low_set_v((float) number));
    printf("%s %s\n", __func__, msg);
    return sms_send(sms_number, msg);
}

static bool cmd_saldo_get(){
    printf("Not implemented\n");
    return true;

}

static bool cmd_invalid(char* sms_number, char* cmd_str){
    char msg[256];
    snprintf(msg, sizeof(msg), "Invalid command: \"%s\"", cmd_str);
    printf("%s %s\n", __func__, msg);
    return sms_send(sms_number, msg);
}

bool command_handle(char* cmd_str, char* sms_number){
    bool res = true;
    double number = 0;

    bool number_exist = extract_number(cmd_str, &number);

	if(0 == strcmp(cmd_str, "Motor start")){
        res = cmd_motor_start(sms_number);
	} else if(0 == strcmp(cmd_str, "Motor stop")){
        res = cmd_motor_stop(sms_number);
	} else if(0 == strcmp(cmd_str, "Motor time set")){
        if(number_exist){
            res = cmd_motor_time_set(sms_number, number);
        } else {
            res = cmd_invalid(sms_number, cmd_str);
        }
	} else if(0 == strcmp(cmd_str, "Motor time get")){
        res = cmd_motor_time_get(sms_number );
	} else if(0 == strcmp(cmd_str, "Battery")){
        res = cmd_battery_get(sms_number);
	} else if(0 == strcmp(cmd_str, "Battery low set")){
        if(number_exist){ res = cmd_battery_low_set(sms_number, number); }
        else{ res = cmd_invalid(sms_number, cmd_str); }
    } else if(0 == strcmp(cmd_str, "Battery low get")){ 
        res = cmd_battery_low_get(sms_number);
    } else if(0 == strcmp(cmd_str, "Sleep time set")){
        if(number_exist){
            res = cmd_sleep_time_set(sms_number, number);
        } else {
            res = cmd_invalid(sms_number, cmd_str);
        }
	} else if(0 == strcmp(cmd_str, "Sleep time get")){
        res = cmd_sleep_time_get(sms_number);
	} else if(0 == strcmp(cmd_str, "Saldo check")){
        res = cmd_saldo_get();
    } else{
        res = cmd_invalid(sms_number, cmd_str);
    }

return res;


}