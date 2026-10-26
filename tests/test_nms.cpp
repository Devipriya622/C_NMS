
#include <gtest/gtest.h>
#include <cstring>

extern "C" {
#include "nms_types.h"
#include "device_manager.h"
#include "monitoring.h"
#include "fault_detection.h"
#include "alarm_manager.h"
#include "event_logger.h"
#include "storage.h"
}

static Device makeDevice(int id, const char *name)
{
    Device d = {};
    d.id = id;
    std::strncpy(d.name, name, NAME_LEN - 1);
    std::strncpy(d.ip, "192.168.1.10", IP_LEN - 1);
    std::strncpy(d.type, "Router", TYPE_LEN - 1);
    std::strncpy(d.location, "Office", LOC_LEN - 1);
    d.status = STATUS_UP;
    d.cpu = 30.0f;
    d.memory = 40.0f;
    d.bandwidth = 50.0f;
    d.packetLoss = 0.5f;
    d.responseTime = 25.0f;
    return d;
}

TEST(DeviceManager, AddDevice)
{
    clear_devices();
    Device d = makeDevice(101, "Router1");
    EXPECT_EQ(add_device(&d), 0);
    EXPECT_EQ(get_device_count(), 1);
}

TEST(DeviceManager, UpdateDevice)
{
    clear_devices();
    Device d = makeDevice(102, "RouterOld");
    ASSERT_EQ(add_device(&d), 0);
    std::strncpy(d.name, "RouterNew", NAME_LEN - 1);
    EXPECT_EQ(update_device(102, &d), 0);

    Device result = {};
    ASSERT_EQ(get_device_by_id(102, &result), 0);
    EXPECT_STREQ(result.name, "RouterNew");
}

TEST(DeviceManager, DeleteDevice)
{
    clear_devices();
    Device d = makeDevice(103, "Router3");
    ASSERT_EQ(add_device(&d), 0);
    EXPECT_EQ(delete_device(103), 0);
    EXPECT_EQ(find_device_index(103), -1);
}

TEST(DeviceManager, FindDeviceIndex)
{
    clear_devices();
    Device d = makeDevice(104, "Router4");
    ASSERT_EQ(add_device(&d), 0);
    EXPECT_GE(find_device_index(104), 0);
    EXPECT_EQ(find_device_index(999), -1);
}

TEST(DeviceManager, GetDeviceById)
{
    clear_devices();
    Device d = makeDevice(105, "Router5");
    ASSERT_EQ(add_device(&d), 0);
    Device result = {};
    EXPECT_EQ(get_device_by_id(105, &result), 0);
    EXPECT_STREQ(result.name, "Router5");
    EXPECT_NE(get_device_by_id(999, &result), 0);
}

TEST(DeviceManager, ListDevices)
{
    clear_devices();
    Device d = makeDevice(106, "Router6");
    ASSERT_EQ(add_device(&d), 0);
    EXPECT_NO_THROW(list_devices());
}

TEST(DeviceManager, GetDeviceCount)
{
    clear_devices();
    EXPECT_EQ(get_device_count(), 0);
    Device d = makeDevice(107, "Router7");
    ASSERT_EQ(add_device(&d), 0);
    EXPECT_EQ(get_device_count(), 1);
}

TEST(DeviceManager, GetDeviceAt)
{
    clear_devices();
    Device d = makeDevice(108, "Router8");
    ASSERT_EQ(add_device(&d), 0);
    Device result = {};
    ASSERT_EQ(get_device_at(0, &result), 0);
    EXPECT_EQ(result.id, 108);
    EXPECT_NE(get_device_at(99, &result), 0);
}

TEST(DeviceManager, ClearDevices)
{
    clear_devices();
    Device d = makeDevice(109, "Router9");
    ASSERT_EQ(add_device(&d), 0);
    EXPECT_EQ(clear_devices(), 0);
    EXPECT_EQ(get_device_count(), 0);
}

TEST(Monitoring, MonitorDevice)
{
    clear_devices();
    Device d = makeDevice(201, "MonitorRouter");
    ASSERT_EQ(add_device(&d), 0);
    EXPECT_NO_THROW(monitor_device(201));
    EXPECT_NO_THROW(monitor_device(999));
}

TEST(Monitoring, MonitorAllDevices)
{
    clear_devices();
    Device d = makeDevice(202, "MonitorAllRouter");
    ASSERT_EQ(add_device(&d), 0);
    EXPECT_NO_THROW(monitor_all_devices());
}

TEST(Monitoring, DisplayDeviceMetrics)
{
    clear_devices();
    Device d = makeDevice(203, "MetricsRouter");
    ASSERT_EQ(add_device(&d), 0);
    EXPECT_NO_THROW(display_device_metrics(203));
    EXPECT_NO_THROW(display_device_metrics(999));
}

TEST(FaultDetection, DetectDeviceFaults)
{
    clear_devices();
    Device healthy = makeDevice(301, "Healthy");
    ASSERT_EQ(add_device(&healthy), 0);
    EXPECT_EQ(detect_device_faults(301), 0);
    EXPECT_EQ(detect_device_faults(999), -1);
}

TEST(FaultDetection, DetectAllFaults)
{
    clear_devices();
    Device d = makeDevice(302, "FaultRouter");
    d.status = STATUS_DOWN;
    d.cpu = 95.0f;
    d.memory = 95.0f;
    d.packetLoss = 5.0f;
    d.responseTime = 300.0f;
    ASSERT_EQ(add_device(&d), 0);
    EXPECT_NO_THROW(detect_all_faults());
}

TEST(AlarmManager, CreateAlarm)
{
    clear_devices();
    Device d = makeDevice(401, "AlarmRouter");
    ASSERT_EQ(add_device(&d), 0);
    int before = get_alarm_count();
    create_alarm(401, "High CPU usage", SEV_MAJOR);
    EXPECT_EQ(get_alarm_count(), before + 1);
}

TEST(AlarmManager, AcknowledgeAlarm)
{
    clear_devices();
    Device d = makeDevice(402, "AckRouter");
    ASSERT_EQ(add_device(&d), 0);
    int before = get_alarm_count();
    create_alarm(402, "Acknowledgement test", SEV_MINOR);
    ASSERT_EQ(get_alarm_count(), before + 1);
    EXPECT_NO_THROW(acknowledge_alarm(1));
    EXPECT_NO_THROW(acknowledge_alarm(999999));
}

TEST(AlarmManager, ClearAlarm)
{
    clear_devices();
    Device d = makeDevice(403, "ClearRouter");
    ASSERT_EQ(add_device(&d), 0);
    int before = get_alarm_count();
    create_alarm(403, "Clear test", SEV_WARNING);
    ASSERT_EQ(get_alarm_count(), before + 1);
    EXPECT_NO_THROW(clear_alarm(1));
    EXPECT_NO_THROW(clear_alarm(999999));
}

TEST(AlarmManager, ListAlarms)
{
    EXPECT_NO_THROW(list_alarms());
}

TEST(AlarmManager, GetAlarmCount)
{
    int before = get_alarm_count();
    EXPECT_GE(before, 0);
    Device d = makeDevice(404, "CountRouter");
    ASSERT_EQ(add_device(&d), 0);
    create_alarm(404, "Count test", SEV_MAJOR);
    EXPECT_EQ(get_alarm_count(), before + 1);
}

TEST(EventLogger, LogEvent)
{
    int before = get_event_count();
    log_event("NMS test event");
    EXPECT_EQ(get_event_count(), before + 1);
}

TEST(EventLogger, ListEvents)
{
    log_event("Event list test");
    EXPECT_NO_THROW(list_events());
}

TEST(EventLogger, GetEventCount)
{
    int before = get_event_count();
    log_event("Event count test");
    EXPECT_EQ(get_event_count(), before + 1);
}

TEST(Storage, SaveDevices)
{
    clear_devices();
    Device d = makeDevice(601, "SaveRouter");
    ASSERT_EQ(add_device(&d), 0);
    EXPECT_EQ(save_devices(), 0);
}

TEST(Storage, LoadDevices)
{
    clear_devices();
    Device d = makeDevice(602, "LoadRouter");
    ASSERT_EQ(add_device(&d), 0);
    ASSERT_EQ(save_devices(), 0);
    clear_devices();
    EXPECT_EQ(load_devices(), 0);
    Device result = {};
    EXPECT_EQ(get_device_by_id(602, &result), 0);
}

TEST(Storage, SaveAlarms)
{
    clear_devices();
    Device d = makeDevice(603, "SaveAlarmRouter");
    ASSERT_EQ(add_device(&d), 0);
    create_alarm(603, "Storage alarm test", SEV_MAJOR);
    EXPECT_EQ(save_alarms(), 0);
}

TEST(Storage, LoadAlarms)
{
    clear_devices();
    Device d = makeDevice(604, "LoadAlarmRouter");
    ASSERT_EQ(add_device(&d), 0);
    create_alarm(604, "Alarm load test", SEV_MINOR);
    ASSERT_EQ(save_alarms(), 0);
    EXPECT_EQ(load_alarms(), 0);
}
