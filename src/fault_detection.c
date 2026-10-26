
#include <stdio.h>
#include "../include/fault_detection.h"
#include "../include/device_manager.h"

int detect_device_faults(int device_id)
{
    Device device = {0};
    int faults = 0;

    if (!get_device_by_id(device_id, &device)) {
        printf("Error: Device ID %d not found.\n", device_id);
        return -1;
    }

    printf("\nFault Detection: %s (ID: %d)\n",
           device.name, device.id);

    if (device.status != STATUS_UP) {
        printf("FAULT: Device is not UP.\n");
        faults++;
    }

    if (device.cpu > 80.0f) {
        printf("FAULT: High CPU usage: %.2f%%\n", device.cpu);
        faults++;
    }

    if (device.memory > 85.0f) {
        printf("FAULT: High memory usage: %.2f%%\n", device.memory);
        faults++;
    }

    if (device.packetLoss > 2.0f) {
        printf("FAULT: High packet loss: %.2f%%\n",
               device.packetLoss);
        faults++;
    }

    if (device.responseTime > 200.0f) {
        printf("FAULT: High response time: %.2f ms\n",
               device.responseTime);
        faults++;
    }

    if (faults == 0) {
        printf("No faults detected.\n");
    } else {
        printf("Total faults detected: %d\n", faults);
    }

    return faults;
}


void detect_all_faults(void)
{
    int count = get_device_count();
    int total_faults = 0;

    if (count == 0) {
        printf("No devices available for fault detection.\n");
        return;
    }

    printf("\n========== Fault Detection Summary ==========\n");

    for (int i = 0; i < count; i++) {
        Device device = {0};

        if (get_device_at(i, &device)) {
            int faults = detect_device_faults(device.id);

            if (faults > 0) {
                total_faults += faults;
            }
        }
    }

    printf("\nTotal faults across all devices: %d\n",
           total_faults);
}

