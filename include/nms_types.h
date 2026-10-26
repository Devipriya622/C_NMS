
#ifndef NMS_TYPES_H
#define NMS_TYPES_H

#include <stdio.h>
#include <time.h>

#define MAX_DEVICES 100
#define MAX_ALARMS 200
#define MAX_EVENTS 500

#define NAME_LEN 50
#define IP_LEN 16
#define TYPE_LEN 30
#define LOC_LEN 50
#define TEXT_LEN 100

typedef enum {
    STATUS_UP,
    STATUS_DOWN,
    STATUS_UNREACHABLE
} DeviceStatus;

typedef enum {
    SEV_WARNING,
    SEV_MINOR,
    SEV_MAJOR,
    SEV_CRITICAL
} Severity;

typedef enum {
    ALARM_ACTIVE,
    ALARM_ACKNOWLEDGED,
    ALARM_CLEARED
} AlarmState;

typedef struct {
    int id;
    char name[NAME_LEN];
    char ip[IP_LEN];
    char type[TYPE_LEN];
    char location[LOC_LEN];
    DeviceStatus status;
    float cpu;
    float memory;
    float bandwidth;
    float packetLoss;
    float responseTime;
} Device;

typedef struct {
    int id;
    int deviceId;
    char message[TEXT_LEN];
    Severity severity;
    AlarmState state;
    time_t timestamp;
} Alarm;

typedef struct {
    time_t timestamp;
    char message[TEXT_LEN];
} Event;

#endif
