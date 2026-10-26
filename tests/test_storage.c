
#include <stdio.h>
#include <string.h>
#include "../include/nms_types.h"
#include "../include/device_manager.h"
#include "../include/storage.h"

int main(void)
{
    Device device = {0};

    device.id = 501;
    snprintf(device.name, NAME_LEN, "%s", "StorageRouter");
    snprintf(device.ip, IP_LEN, "%s", "192.168.1.5");
    snprintf(device.type, TYPE_LEN, "%s", "Router");
    snprintf(device.location, LOC_LEN, "%s", "Lab");

    device.status = STATUS_UP;
    device.cpu = 35.0f;
    device.memory = 50.0f;
    device.bandwidth = 200.0f;
    device.packetLoss = 0.0f;
    device.responseTime = 8.0f;

    if (!add_device(&device)) {
        printf("FAIL: Add test device\n");
        return 1;
    }

    if (!save_devices()) {
        printf("FAIL: Save devices\n");
        return 1;
    }

    clear_devices();

    if (get_device_count() != 0) {
        printf("FAIL: Clear devices\n");
        return 1;
    }

    if (!load_devices()) {
        printf("FAIL: Load devices\n");
        return 1;
    }

    Device loaded = {0};

    if (get_device_by_id(501, &loaded) &&
        strcmp(loaded.name, "StorageRouter") == 0 &&
        loaded.cpu == 35.0f) {
        printf("PASS: Device persistence\n");
    } else {
        printf("FAIL: Device persistence\n");
        return 1;
    }

    list_devices();

    return 0;
}
