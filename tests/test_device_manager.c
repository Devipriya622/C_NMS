
#include <stdio.h>
#include <string.h>
#include "../include/device_manager.h"

int main(void)
{
    Device device = {0};
    Device result = {0};

    device.id = 101;
    snprintf(device.name, NAME_LEN, "%s", "CoreRouter");
    snprintf(device.ip, IP_LEN, "%s", "192.168.1.1");
    snprintf(device.type, TYPE_LEN, "%s", "Router");
    snprintf(device.location, LOC_LEN, "%s", "Lab");
    device.status = STATUS_UNREACHABLE;

    if (add_device(&device)) {
        printf("PASS: Add device\n");
    }

    if (find_device_index(101) == 0) {
        printf("PASS: Search device\n");
    }

    snprintf(device.name, NAME_LEN, "%s", "MainRouter");

    if (update_device(101, &device) &&
        get_device_by_id(101, &result) &&
        strcmp(result.name, "MainRouter") == 0) {
        printf("PASS: Update device\n");
    }

    if (delete_device(101) && find_device_index(101) == -1) {
        printf("PASS: Delete device\n");
    }

    return 0;
}
