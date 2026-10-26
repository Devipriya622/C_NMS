
#include <stdio.h>
#include <string.h>
#include "../include/event_logger.h"

static Event events[MAX_EVENTS];
static int event_count = 0;

int log_event(const char *message)
{
    if (message == NULL || message[0] == '\0' ||
        strlen(message) >= TEXT_LEN) {
        printf("Error: Invalid event message.\n");
        return 0;
    }

    if (event_count >= MAX_EVENTS) {
        printf("Error: Event capacity reached.\n");
        return 0;
    }

    Event event = {0};
    event.timestamp = time(NULL);
    snprintf(event.message, TEXT_LEN, "%s", message);

    events[event_count++] = event;

    printf("Event logged successfully.\n");
    return 1;
}

void list_events(void)
{
    if (event_count == 0) {
        printf("No events recorded.\n");
        return;
    }

    printf("\n%-20s %s\n", "Timestamp", "Event Message");

    for (int i = 0; i < event_count; i++) {
        char timestamp[20];
        struct tm *time_info = localtime(&events[i].timestamp);

        if (time_info != NULL) {
            strftime(timestamp, sizeof(timestamp),
                     "%Y-%m-%d %H:%M:%S", time_info);
        } else {
            snprintf(timestamp, sizeof(timestamp), "Unknown");
        }

        printf("%-20s %s\n", timestamp, events[i].message);
    }
}

int get_event_count(void)
{
    return event_count;
}
