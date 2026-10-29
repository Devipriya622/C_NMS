
#include <stdio.h>
#include <pthread.h>
#include "../include/monitoring.h"
#include "../include/device_manager.h"

typedef struct {
    Device device;
} MonitoringTask;

static pthread_mutex_t output_mutex = PTHREAD_MUTEX_INITIALIZER;

static void print_device_metrics(const Device *device)
{
    const char *status;

    switch (device->status) {
        case STATUS_UP:
            status = "UP";
            break;
        case STATUS_DOWN:
            status = "DOWN";
            break;
        default:
            status = "UNREACHABLE";
            break;
    }

    printf("\nMonitoring device: %s (ID: %d)\n",
           device->name, device->id);
    printf("Device Status: %s\n", status);
    printf("CPU Usage: %.2f%%\n", device->cpu);
    printf("Memory Usage: %.2f%%\n", device->memory);
    printf("Bandwidth: %.2f Mbps\n", device->bandwidth);
    printf("Packet Loss: %.2f%%\n", device->packetLoss);
    printf("Response Time: %.2f ms\n", device->responseTime);
    printf("----------------------------------------\n");
}

static void *monitoring_worker(void *argument)
{
    MonitoringTask *task = (MonitoringTask *)argument;

    if (task == NULL) {
        return NULL;
    }

    if (pthread_mutex_lock(&output_mutex) != 0) {
        return NULL;
    }

    printf("[Thread %lu] Monitoring started.\n",
           (unsigned long)pthread_self());

    print_device_metrics(&task->device);

    printf("[Thread %lu] Monitoring completed.\n",
           (unsigned long)pthread_self());

    if (pthread_mutex_unlock(&output_mutex) != 0) {
        return NULL;
    }

    return NULL;
}

int monitor_device(int device_id)
{
    Device device = {0};

    if (!get_device_by_id(device_id, &device)) {
        printf("Error: Device ID %d not found.\n", device_id);
        return 0;
    }

    print_device_metrics(&device);
    return 1;
}

void monitor_all_devices(void)
{
    pthread_t threads[MAX_DEVICES];
    MonitoringTask tasks[MAX_DEVICES];
    int device_count = get_device_count();
    int task_count = 0;
    int started = 0;
    int creation_failed = 0;
    int i;

    if (device_count == 0) {
        printf("No devices available for monitoring.\n");
        return;
    }

    if (device_count > MAX_DEVICES) {
        device_count = MAX_DEVICES;
    }

    for (i = 0; i < device_count; i++) {
        if (get_device_at(i, &tasks[task_count].device)) {
            task_count++;
        }
    }

    if (task_count == 0) {
        printf("No devices could be prepared for monitoring.\n");
        return;
    }

    printf("\nStarting multithreaded device monitoring...\n");
    printf("Devices to monitor: %d\n", task_count);

    for (i = 0; i < task_count; i++) {
        if (pthread_create(&threads[started], NULL,
                           monitoring_worker, &tasks[i]) != 0) {
            creation_failed = 1;
            break;
        }

        started++;
    }

    for (i = 0; i < started; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            printf("Warning: Failed to join a monitoring thread.\n");
        }
    }

    if (creation_failed) {
        printf("Warning: Some monitoring threads could not be created.\n");
    }

    printf("Multithreaded monitoring finished.\n");
}

void display_device_metrics(int device_id)
{
    monitor_device(device_id);
}
