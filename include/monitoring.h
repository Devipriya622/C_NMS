
#ifndef MONITORING_H
#define MONITORING_H

#include "nms_types.h"

int monitor_device(int device_id);
void monitor_all_devices(void);
void display_device_metrics(int device_id);

#endif
