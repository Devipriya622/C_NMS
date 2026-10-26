
#ifndef ALARM_MANAGER_H
#define ALARM_MANAGER_H

#include "nms_types.h"

int create_alarm(int device_id, const char *message, Severity severity);
int acknowledge_alarm(int alarm_id);
int clear_alarm(int alarm_id);
void list_alarms(void);
int get_alarm_count(void);

int get_alarm_at(int index, Alarm *result);
int restore_alarm(const Alarm *alarm);
int clear_alarms(void);
#endif
