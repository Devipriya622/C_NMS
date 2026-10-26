
#include <stdio.h>
#include "../include/nms_types.h"
#include "../include/device_manager.h"
#include "../include/alarm_manager.h"
#include "../include/storage.h"

int main(void)
{
    Device device = {0};

    device.id = 701;
    snprintf(device.name, NAME_LEN, "AlarmRouter");
    snprintf(device.ip, IP_LEN, "192.168.1.7");
    snprintf(device.type, TYPE_LEN, "Router");
    snprintf(device.location, LOC_LEN, "Lab");
    device.status = STATUS_UP;

    clear_devices();
    clear_alarms();

    if (!add_device(&device)) {
        printf("FAIL: Add device\n");
        return 1;
    }

    if (!create_alarm(701, "High CPU usage", SEV_MAJOR)) {
        printf("FAIL: Create alarm\n");
        return 1;
    }

    if (!save_alarms()) {
        printf("FAIL: Save alarms\n");
        return 1;
    }

    clear_alarms();

    if (get_alarm_count() != 0) {
        printf("FAIL: Clear alarms before loading\n");
        return 1;
    }

    if (!load_alarms()) {
        printf("FAIL: Load alarms\n");
        return 1;
    }

    if (get_alarm_count() != 1) {
        printf("FAIL: Alarm count after loading\n");
        return 1;
    }

    printf("PASS: Alarm persistence\n");
    list_alarms();

    return 0;
}
