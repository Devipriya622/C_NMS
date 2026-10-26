
#include <stdio.h>
#include "../include/nms_types.h"
#include "../include/device_manager.h"
#include "../include/monitoring.h"

int main(void)
{
    Device device = {0};

    device.id = 101;
    snprintf(device.name, NAME_LEN, "%s", "CoreRouter");
    snprintf(device.ip, IP_LEN, "%s", "192.168.1.1");
    snprintf(device.type, TYPE_LEN, "%s", "Router");
    snprintf(device.location, LOC_LEN, "%s", "Lab");

    device.status = STATUS_UP;
    device.cpu = 45.5f;
    device.memory = 62.0f;
    device.bandwidth = 100.0f;
    device.packetLoss = 0.5f;
    device.responseTime = 12.5f;

    if (!add_device(&device)) {
        printf("FAIL: Could not add test device.\n");
        return 1;
    }

    if (monitor_device(101)) {
        printf("\nPASS: Monitor device\n");
    } else {
        printf("\nFAIL: Monitor device\n");
        return 1;
    }

    printf("\nTesting invalid device ID:\n");
    if (!monitor_device(999)) {
        printf("PASS: Invalid device ID handled\n");
    }

    printf("\nTesting monitoring summary:\n");
    monitor_all_devices();

    return 0;
}
