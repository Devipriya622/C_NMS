
#include <stdio.h>
#include <string.h>
#include "../include/storage.h"
#include "../include/nms_types.h"
#include "../include/device_manager.h"
#include "../include/alarm_manager.h"

int save_devices(void)
{
    FILE *file = fopen("devices.csv", "w");

    if (file == NULL) {
        printf("Error: Could not open devices.csv for writing.\n");
        return 0;
    }

    fprintf(file,
        "id,name,ip,type,location,status,cpu,memory,bandwidth,packetLoss,responseTime\n");

    int count = get_device_count();

    for (int i = 0; i < count; i++) {
        Device device = {0};

        if (!get_device_at(i, &device)) {
            fclose(file);
            return 0;
        }

        if (fprintf(file,
            "%d,%s,%s,%s,%s,%d,%.2f,%.2f,%.2f,%.2f,%.2f\n",
            device.id,
            device.name,
            device.ip,
            device.type,
            device.location,
            (int)device.status,
            device.cpu,
            device.memory,
            device.bandwidth,
            device.packetLoss,
            device.responseTime) < 0) {
            fclose(file);
            return 0;
        }
    }

    if (fclose(file) != 0) {
        printf("Error: Could not finish writing devices.csv.\n");
        return 0;
    }

    printf("Devices saved successfully.\n");
    return 1;
}

int load_devices(void)
{
    FILE *file = fopen("devices.csv", "r");

    if (file == NULL) {
        printf("No saved device file found.\n");
        return 0;
    }

    char line[512];

    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        printf("Error: Device file is empty or unreadable.\n");
        return 0;
    }

    int loaded = 0;
    int failed = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        Device device = {0};
        int status;

        int fields = sscanf(
            line,
            "%d,%49[^,],%15[^,],%29[^,],%49[^,],%d,%f,%f,%f,%f,%f",
            &device.id,
            device.name,
            device.ip,
            device.type,
            device.location,
            &status,
            &device.cpu,
            &device.memory,
            &device.bandwidth,
            &device.packetLoss,
            &device.responseTime
        );

        if (fields != 11 ||
            device.id <= 0 ||
            status < STATUS_UP ||
            status > STATUS_UNREACHABLE) {
            failed++;
            continue;
        }

        device.status = (DeviceStatus)status;

        if (!add_device(&device)) {
            failed++;
            continue;
        }

        loaded++;
    }

    if (ferror(file)) {
        fclose(file);
        printf("Error while reading devices.csv.\n");
        return 0;
    }

    fclose(file);

    printf("Loaded %d device(s); skipped %d invalid record(s).\n",
           loaded, failed);

    return 1;
}


int save_alarms(void)
{
    FILE *file = fopen("alarms.csv", "w");

    if (file == NULL) {
        printf("Error: Could not open alarms.csv for writing.\n");
        return 0;
    }

    fprintf(file, "id,deviceId,message,severity,state,timestamp\n");

    int count = get_alarm_count();

    for (int i = 0; i < count; i++) {
        Alarm alarm = {0};

        if (!get_alarm_at(i, &alarm)) {
            fclose(file);
            return 0;
        }

        if (strchr(alarm.message, ',') != NULL) {
            printf("Error: Alarm message cannot contain commas.\n");
            fclose(file);
            return 0;
        }

        if (fprintf(file, "%d,%d,%s,%d,%d,%lld\n",
                    alarm.id,
                    alarm.deviceId,
                    alarm.message,
                    (int)alarm.severity,
                    (int)alarm.state,
                    (long long)alarm.timestamp) < 0) {
            fclose(file);
            return 0;
        }
    }

    if (fclose(file) != 0) {
        printf("Error: Could not finish writing alarms.csv.\n");
        return 0;
    }

    printf("Alarms saved successfully.\n");
    return 1;
}

int load_alarms(void)
{
    FILE *file = fopen("alarms.csv", "r");

    if (file == NULL) {
        printf("No saved alarm file found.\n");
        return 0;
    }

    char line[512];

    if (fgets(line, sizeof(line), file) == NULL) {
        fclose(file);
        printf("Error: Alarm file is empty or unreadable.\n");
        return 0;
    }

    int loaded = 0;
    int failed = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        Alarm alarm = {0};
        int severity;
        int state;
        long long timestamp;

        int fields = sscanf(line, "%d,%d,%99[^,],%d,%d,%lld",
                            &alarm.id,
                            &alarm.deviceId,
                            alarm.message,
                            &severity,
                            &state,
                            &timestamp);

        if (fields != 6 ||
            alarm.id <= 0 ||
            alarm.deviceId <= 0 ||
            alarm.message[0] == '\0' ||
            severity < SEV_WARNING ||
            severity > SEV_CRITICAL ||
            state < ALARM_ACTIVE ||
            state > ALARM_CLEARED) {
            failed++;
            continue;
        }

        alarm.severity = (Severity)severity;
        alarm.state = (AlarmState)state;
        alarm.timestamp = (time_t)timestamp;

        if (!restore_alarm(&alarm)) {
            failed++;
            continue;
        }

        loaded++;
    }

    if (ferror(file)) {
        fclose(file);
        printf("Error while reading alarms.csv.\n");
        return 0;
    }

    fclose(file);

    printf("Loaded %d alarm(s); skipped %d invalid record(s).\n",
           loaded, failed);

    return 1;
}
