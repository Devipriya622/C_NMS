
#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include "nms_types.h"

int add_device(const Device *device);
int update_device(int id, const Device *updated_device);
int delete_device(int id);
int find_device_index(int id);
int get_device_by_id(int id, Device *result);
void list_devices(void);
int get_device_count(void);
int get_device_at(int index, Device *result);
int get_device_at(int index, Device *result);
int clear_devices(void);
int get_device_at(int index, Device *result);
int clear_devices(void);
#endif
