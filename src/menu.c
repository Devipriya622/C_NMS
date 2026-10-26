
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/menu.h"
#include "../include/nms_types.h"
#include "../include/device_manager.h"
#include "../include/monitoring.h"
#include "../include/fault_detection.h"
#include "../include/alarm_manager.h"
#include "../include/event_logger.h"
#include "../include/storage.h"

static void read_line(const char *prompt, char *buffer, size_t size)
{
    int ch;

    printf("%s", prompt);

    if (fgets(buffer, (int)size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }

    size_t length = strlen(buffer);

    if (length > 0 && buffer[length - 1] == '\n') {
        buffer[length - 1] = '\0';
    } else {
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
    }
}

static int read_int(const char *prompt, int *value)
{
    char line[100];
    char *end;
    long number;

    read_line(prompt, line, sizeof(line));

    if (line[0] == '\0') {
        return 0;
    }

    number = strtol(line, &end, 10);

    while (*end == ' ' || *end == '\t') {
        end++;
    }

    if (*end != '\0' || end == line ||
        number < -2147483647L - 1 || number > 2147483647L) {
        return 0;
    }

    *value = (int)number;
    return 1;
}

static void read_device_status(DeviceStatus *status)
{
    int choice;

    printf("1. UP\n");
    printf("2. DOWN\n");
    printf("3. UNREACHABLE\n");

    if (!read_int("Select device status: ", &choice)) {
        printf("Invalid input. Status set to UP.\n");
        *status = STATUS_UP;
        return;
    }

    switch (choice) {
        case 1:
            *status = STATUS_UP;
            break;
        case 2:
            *status = STATUS_DOWN;
            break;
        case 3:
            *status = STATUS_UNREACHABLE;
            break;
        default:
            printf("Invalid status. Status set to UP.\n");
            *status = STATUS_UP;
    }
}

static void add_device_menu(void)
{
    Device device = {0};

    if (!read_int("Enter device ID: ", &device.id) ||
        device.id <= 0) {
        printf("Invalid device ID.\n");
        return;
    }

    read_line("Enter device name: ", device.name, sizeof(device.name));
    read_line("Enter IP address: ", device.ip, sizeof(device.ip));
    read_line("Enter device type: ", device.type, sizeof(device.type));
    read_line("Enter location: ", device.location, sizeof(device.location));

    if (device.name[0] == '\0' ||
        device.ip[0] == '\0' ||
        device.type[0] == '\0' ||
        device.location[0] == '\0') {
        printf("Device fields cannot be empty.\n");
        return;
    }

    read_device_status(&device.status);

    device.cpu = 0.0f;
    device.memory = 0.0f;
    device.bandwidth = 0.0f;
    device.packetLoss = 0.0f;
    device.responseTime = 0.0f;

    if (add_device(&device)) {
        log_event("Device added");
    }
}

static void search_device_menu(void)
{
    int id;
    Device device;

    if (!read_int("Enter device ID to search: ", &id)) {
        printf("Invalid device ID.\n");
        return;
    }

    if (!get_device_by_id(id, &device)) {
        printf("Device not found.\n");
        return;
    }

    printf("\nDevice Details\n");
    printf("ID: %d\n", device.id);
    printf("Name: %s\n", device.name);
    printf("IP Address: %s\n", device.ip);
    printf("Type: %s\n", device.type);
    printf("Location: %s\n", device.location);
    printf("Status: %s\n",
           device.status == STATUS_UP ? "UP" :
           device.status == STATUS_DOWN ? "DOWN" : "UNREACHABLE");
    printf("CPU: %.2f%%\n", device.cpu);
    printf("Memory: %.2f%%\n", device.memory);
    printf("Bandwidth: %.2f Mbps\n", device.bandwidth);
    printf("Packet Loss: %.2f%%\n", device.packetLoss);
    printf("Response Time: %.2f ms\n", device.responseTime);
}

static void update_device_menu(void)
{
    int id;
    Device device;

    if (!read_int("Enter device ID to update: ", &id)) {
        printf("Invalid device ID.\n");
        return;
    }

    if (!get_device_by_id(id, &device)) {
        printf("Device not found.\n");
        return;
    }

    printf("Enter the updated details.\n");

    read_line("New device name: ", device.name, sizeof(device.name));
    read_line("New IP address: ", device.ip, sizeof(device.ip));
    read_line("New device type: ", device.type, sizeof(device.type));
    read_line("New location: ", device.location, sizeof(device.location));

    if (device.name[0] == '\0' ||
        device.ip[0] == '\0' ||
        device.type[0] == '\0' ||
        device.location[0] == '\0') {
        printf("Device fields cannot be empty.\n");
        return;
    }

    read_device_status(&device.status);

    if (update_device(id, &device)) {
        log_event("Device updated");
    }
}

static void delete_device_menu(void)
{
    int id;

    if (!read_int("Enter device ID to delete: ", &id)) {
        printf("Invalid device ID.\n");
        return;
    }

    if (delete_device(id)) {
        log_event("Device deleted");
    }
}

void display_menu(void)
{
    printf("\n========================================\n");
    printf(" ENTERPRISE TELECOM NETWORK MANAGEMENT\n");
    printf("========================================\n");
    printf("1. Add Device\n");
    printf("2. List All Devices\n");
    printf("3. Search Device\n");
    printf("4. Update Device\n");
    printf("5. Delete Device\n");
    printf("6. Monitor a Device\n");
    printf("7. Detect Faults\n");
    printf("8. List Alarms\n");
    printf("9. Acknowledge an Alarm\n");
    printf("10. Clear an Alarm\n");
    printf("11. View Event Log\n");
    printf("12. Save Data\n");
    printf("13. Load Data\n");
    printf("0. Exit\n");
    printf("========================================\n");
}

void run_menu(void)
{
    int choice;
    int id;

    for (;;) {
        display_menu();

        if (!read_int("Enter your choice: ", &choice)) {
            printf("Invalid input. Enter a menu number.\n");
            continue;
        }

        switch (choice) {
            case 1:
                add_device_menu();
                break;

            case 2:
                list_devices();
                break;

            case 3:
                search_device_menu();
                break;

            case 4:
                update_device_menu();
                break;

            case 5:
                delete_device_menu();
                break;

            case 6:
                if (read_int("Enter device ID: ", &id)) {
                    monitor_device(id);
                } else {
                    printf("Invalid device ID.\n");
                }
                break;

            case 7:
                detect_all_faults();
                break;

            case 8:
                list_alarms();
                break;

            case 9:
                if (read_int("Enter alarm ID to acknowledge: ", &id)) {
                    acknowledge_alarm(id);
                } else {
                    printf("Invalid alarm ID.\n");
                }
                break;

            case 10:
                if (read_int("Enter alarm ID to clear: ", &id)) {
                    clear_alarm(id);
                } else {
                    printf("Invalid alarm ID.\n");
                }
                break;

            case 11:
                list_events();
                break;

            case 12:
                if (save_devices() && save_alarms()) {
                    log_event("Device and alarm data saved");
                }
                break;

            case 13:
                if (load_devices()) {
                    load_alarms();
                    log_event("Data loading attempted");
                }
                break;

            case 0:
                printf("Exiting NMS. Goodbye!\n");
                return;

            default:
                printf("Invalid choice. Please select 0-13.\n");
        }
    }
}
