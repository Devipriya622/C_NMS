
#include <stdio.h>
#include "../include/event_logger.h"

int main(void)
{
    if (log_event("Device 401 registered")) {
        printf("PASS: Log event\n");
    } else {
        printf("FAIL: Log event\n");
        return 1;
    }

    if (log_event("High CPU usage detected")) {
        printf("PASS: Log second event\n");
    } else {
        printf("FAIL: Log second event\n");
        return 1;
    }

    if (get_event_count() == 2) {
        printf("PASS: Event count\n");
    } else {
        printf("FAIL: Event count\n");
        return 1;
    }

    if (!log_event("")) {
        printf("PASS: Empty event rejected\n");
    } else {
        printf("FAIL: Empty event test\n");
        return 1;
    }

    printf("\nDisplaying recorded events:\n");
    list_events();

    return 0;
}
