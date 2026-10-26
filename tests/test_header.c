
#include "include/nms_types.h"

int main(void)
{
    Device device = {0};

    device.id = 101;
    snprintf(device.name, NAME_LEN, "%s", "CoreRouter");
    snprintf(device.ip, IP_LEN, "%s", "192.168.1.1");
    device.status = STATUS_UNREACHABLE;

    printf("Device ID: %d\n", device.id);
    printf("Device Name: %s\n", device.name);
    printf("IP Address: %s\n", device.ip);
    printf("Device Status: %d\n", device.status);

    return 0;
}
