#ifndef EVENT_LOGGER_H
#define EVENT_LOGGER_H

#include "nms_types.h"

int log_event(const char *message);
void list_events(void);
int get_event_count(void);

#endif