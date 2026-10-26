
#include <stdio.h>
#include <string.h>
#include "../include/device_manager.h"

static Device devices[MAX_DEVICES];
static int device_count = 0;

int find_device_index(int id)
{
    for (int i = 0; i < device_count; i++) {
        if (devices[i].id == id) {
            return i;
        }
    }

    return -1;
}

int add_device(const Device *device)
{
    if (device == NULL || device->id <= 0) {
        return 0;
    }

    if (device_count >= MAX_DEVICES) {
        printf("Device capacity reached.\n");
        return 0;
    }

    if (find_device_index(device->id) != -1) {
        printf("A device with this ID already exists.\n");
        return 0;
    }

    if (memchr(device->name, '\0', NAME_LEN) == NULL ||
        memchr(device->ip, '\0', IP_LEN) == NULL ||
        memchr(device->type, '\0', TYPE_LEN) == NULL ||
        memchr(device->location, '\0', LOC_LEN) == NULL) {
        printf("Invalid device fields.\n");
        return 0;
    }

    devices[device_count] = *device;
    device_count++;

    printf("Device added successfully.\n");
    return 1;
}

int update_device(int id, const Device *updated_device)
{
    int index = find_device_index(id);

    if (index == -1 || updated_device == NULL) {
        printf("Device not found or invalid update data.\n");
        return 0;
    }

    if (updated_device->id != id ||
        memchr(updated_device->name, '\0', NAME_LEN) == NULL ||
        memchr(updated_device->ip, '\0', IP_LEN) == NULL ||
        memchr(updated_device->type, '\0', TYPE_LEN) == NULL ||
        memchr(updated_device->location, '\0', LOC_LEN) == NULL) {
        printf("Invalid update data.\n");
        return 0;
    }

    devices[index] = *updated_device;

    printf("Device updated successfully.\n");
    return 1;
}

int delete_device(int id)
{
    int index = find_device_index(id);

    if (index == -1) {
        printf("Device not found.\n");
        return 0;
    }

    for (int i = index; i < device_count - 1; i++) {
        devices[i] = devices[i + 1];
    }

    device_count--;

    printf("Device deleted successfully.\n");
    return 1;
}

int get_device_by_id(int id, Device *result)
{
    int index = find_device_index(id);

    if (index == -1 || result == NULL) {
        return 0;
    }

    *result = devices[index];
    return 1;
}

void list_devices(void)
{
    if (device_count == 0) {
        printf("No devices registered.\n");
        return;
    }

    printf("\n%-8s %-20s %-16s %-12s %-15s %-15s\n",
           "ID", "Name", "IP Address", "Type", "Location", "Status");

    for (int i = 0; i < device_count; i++) {
        const char *status;

        switch (devices[i].status) {
            case STATUS_UP: status = "UP"; break;
            case STATUS_DOWN: status = "DOWN"; break;
            default: status = "UNREACHABLE"; break;
        }

        printf("%-8d %-20s %-16s %-12s %-15s %-15s\n",
               devices[i].id,
               devices[i].name,
               devices[i].ip,
               devices[i].type,
               devices[i].location,
               status);
    }
}

int get_device_count(void)
{
    return device_count;
}

int get_device_at(int index, Device *result)
{
    if (index < 0 || index >= device_count || result == NULL) {
        return 0;
    }

    *result = devices[index];
    return 1;
}

int clear_devices(void)
{
    device_count = 0;
    return 1;
}

