
#include <stdio.h>
#include <string.h>
#include "../include/alarm_manager.h"
#include "../include/device_manager.h"

static Alarm alarms[MAX_ALARMS];
static int alarm_count = 0;
static int next_alarm_id = 1;

int create_alarm(int device_id, const char *message, Severity severity)
{
    Device device = {0};

    if (!get_device_by_id(device_id, &device)) {
        printf("Error: Cannot create alarm. Device not found.\n");
        return 0;
    }

    if (message == NULL || message[0] == '\0' ||
        strlen(message) >= TEXT_LEN ||
        severity < SEV_WARNING || severity > SEV_CRITICAL) {
        printf("Error: Invalid alarm details.\n");
        return 0;
    }

    if (alarm_count >= MAX_ALARMS) {
        printf("Error: Alarm capacity reached.\n");
        return 0;
    }

    Alarm alarm = {0};
    alarm.id = next_alarm_id++;
    alarm.deviceId = device_id;
    snprintf(alarm.message, TEXT_LEN, "%s", message);
    alarm.severity = severity;
    alarm.state = ALARM_ACTIVE;
    alarm.timestamp = time(NULL);

    alarms[alarm_count++] = alarm;

    printf("Alarm created successfully. Alarm ID: %d\n", alarm.id);
    return alarm.id;
}

int acknowledge_alarm(int alarm_id)
{
    for (int i = 0; i < alarm_count; i++) {
        if (alarms[i].id == alarm_id) {
            if (alarms[i].state == ALARM_CLEARED) {
                printf("Cannot acknowledge a cleared alarm.\n");
                return 0;
            }

            alarms[i].state = ALARM_ACKNOWLEDGED;
            printf("Alarm %d acknowledged.\n", alarm_id);
            return 1;
        }
    }

    printf("Alarm %d not found.\n", alarm_id);
    return 0;
}

int clear_alarm(int alarm_id)
{
    for (int i = 0; i < alarm_count; i++) {
        if (alarms[i].id == alarm_id) {
            alarms[i].state = ALARM_CLEARED;
            printf("Alarm %d cleared.\n", alarm_id);
            return 1;
        }
    }

    printf("Alarm %d not found.\n", alarm_id);
    return 0;
}

void list_alarms(void)
{
    if (alarm_count == 0) {
        printf("No alarms recorded.\n");
        return;
    }

    printf("\n%-8s %-10s %-12s %-15s %-12s %s\n",
           "Alarm ID", "Device ID", "Severity",
           "State", "Timestamp", "Message");

    for (int i = 0; i < alarm_count; i++) {
        const char *severity;
        const char *state;
        char timestamp[20];
        struct tm *time_info = localtime(&alarms[i].timestamp);

        switch (alarms[i].severity) {
            case SEV_WARNING: severity = "WARNING"; break;
            case SEV_MINOR: severity = "MINOR"; break;
            case SEV_MAJOR: severity = "MAJOR"; break;
            default: severity = "CRITICAL"; break;
        }

        switch (alarms[i].state) {
            case ALARM_ACTIVE: state = "ACTIVE"; break;
            case ALARM_ACKNOWLEDGED: state = "ACKNOWLEDGED"; break;
            default: state = "CLEARED"; break;
        }

        if (time_info != NULL) {
            strftime(timestamp, sizeof(timestamp),
                     "%Y-%m-%d %H:%M", time_info);
        } else {
            snprintf(timestamp, sizeof(timestamp), "Unknown");
        }

        printf("%-8d %-10d %-12s %-15s %-12s %s\n",
               alarms[i].id, alarms[i].deviceId, severity,
               state, timestamp, alarms[i].message);
    }
}

int get_alarm_count(void)
{
    return alarm_count;
}


int get_alarm_at(int index, Alarm *result)
{
    if (index < 0 || index >= alarm_count || result == NULL) {
        return 0;
    }

    *result = alarms[index];
    return 1;
}

int restore_alarm(const Alarm *alarm)
{
    if (alarm == NULL || alarm->id <= 0 ||
        alarm->deviceId <= 0 ||
        alarm_count >= MAX_ALARMS ||
        alarm->severity < SEV_WARNING ||
        alarm->severity > SEV_CRITICAL ||
        alarm->state < ALARM_ACTIVE ||
        alarm->state > ALARM_CLEARED ||
        memchr(alarm->message, '\0', TEXT_LEN) == NULL ||
        alarm->message[0] == '\0') {
        return 0;
    }

    for (int i = 0; i < alarm_count; i++) {
        if (alarms[i].id == alarm->id) {
            return 0;
        }
    }

    alarms[alarm_count] = *alarm;
    alarm_count++;

    if (alarm->id >= next_alarm_id) {
        next_alarm_id = alarm->id + 1;
    }

    return 1;
}

int clear_alarms(void)
{
    alarm_count = 0;
    next_alarm_id = 1;
    return 1;
}
