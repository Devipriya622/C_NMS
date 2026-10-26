
#include <stdio.h>
#include "../include/nms_types.h"
#include "../include/device_manager.h"
#include "../include/fault_detection.h"

int main(void)
{
    Device device = {0};

    device.id = 201;
    snprintf(device.name, NAME_LEN, "%s", "EdgeRouter");
    snprintf(device.ip, IP_LEN, "%s", "192.168.1.2");
    snprintf(device.type, TYPE_LEN, "%s", "Router");
    snprintf(device.location, LOC_LEN, "%s", "Lab");

    device.status = STATUS_UP;
    device.cpu = 92.0f;
    device.memory = 90.0f;
    device.packetLoss = 3.5f;
    device.responseTime = 250.0f;

    if (!add_device(&device)) {
        printf("FAIL: Could not add test device.\n");
        return 1;
    }

    int faults = detect_device_faults(201);

    if (faults == 4) {
        printf("\nPASS: Fault detection\n");
    } else {
        printf("\nFAIL: Expected 4 faults, detected %d\n", faults);
        return 1;
    }

    if (detect_device_faults(999) == -1) {
        printf("PASS: Invalid device ID handled\n");
    } else {
        printf("FAIL: Invalid device ID test\n");
        return 1;
    }

    detect_all_faults();

    return 0;
}
