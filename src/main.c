
#include <stdio.h>
#include "../include/menu.h"
#include "../include/storage.h"
#include "../include/event_logger.h"

int main(void)
{
    printf("Starting Enterprise Telecom Network Management System...\n");

    if (load_devices()) {
        load_alarms();
    }

    log_event("NMS application started");

    run_menu();

    log_event("NMS application stopped");

    return 0;
}
