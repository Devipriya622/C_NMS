
#include <stdio.h>
#include "../include/nms_types.h"
#include "../include/device_manager.h"
#include "../include/alarm_manager.h"

int main(void)
{
    Device device = {0};

    device.id = 301;
    snprintf(device.name, NAME_LEN, "%s", "DistributionRouter");
    snprintf(device.ip, IP_LEN, "%s", "192.168.1.3");
    snprintf(device.type, TYPE_LEN, "%s", "Router");
    snprintf(device.location, LOC_LEN, "%s", "Lab");
    device.status = STATUS_UP;

    if (!add_device(&device)) {
        printf("FAIL: Could not add test device.\n");
        return 1;
    }

    int alarm_id = create_alarm(
        301, "High CPU usage detected", SEV_MAJOR
    );

    if (alarm_id > 0 && get_alarm_count() == 1) {
        printf("PASS: Create alarm\n");
    } else {
        printf("FAIL: Create alarm\n");
        return 1;
    }

    if (acknowledge_alarm(alarm_id)) {
        printf("PASS: Acknowledge alarm\n");
    } else {
        printf("FAIL: Acknowledge alarm\n");
        return 1;
    }

    if (clear_alarm(alarm_id)) {
        printf("PASS: Clear alarm\n");
    } else {
        printf("FAIL: Clear alarm\n");
        return 1;
    }

    printf("\nDisplaying alarms:\n");
    list_alarms();

    if (!create_alarm(999, "Invalid device alarm", SEV_MINOR)) {
        printf("\nPASS: Invalid device handled\n");
    } else {
        printf("\nFAIL: Invalid device test\n");
        return 1;
    }

    return 0;
}
