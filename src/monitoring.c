
#include <stdio.h>
#include "../include/monitoring.h"
#include "../include/device_manager.h"

int monitor_device(int device_id)
{
    Device device = {0};

    if (!get_device_by_id(device_id, &device)) {
        printf("Error: Device ID %d not found.\n", device_id);
        return 0;
    }

    printf("\nMonitoring device: %s\n", device.name);

    if (device.status == STATUS_UP) {
        printf("Device Status: UP\n");
    } else if (device.status == STATUS_DOWN) {
        printf("Device Status: DOWN\n");
    } else {
        printf("Device Status: UNREACHABLE\n");
    }

    printf("CPU Usage: %.2f%%\n", device.cpu);
    printf("Memory Usage: %.2f%%\n", device.memory);
    printf("Bandwidth: %.2f Mbps\n", device.bandwidth);
    printf("Packet Loss: %.2f%%\n", device.packetLoss);
    printf("Response Time: %.2f ms\n", device.responseTime);

    return 1;
}

void monitor_all_devices(void)
{
    int count = get_device_count();

    if (count == 0) {
        printf("No devices available for monitoring.\n");
        return;
    }

    printf("\nMonitoring Summary\n");
    printf("------------------\n");

    list_devices();
}

void display_device_metrics(int device_id)
{
    monitor_device(device_id);
}
