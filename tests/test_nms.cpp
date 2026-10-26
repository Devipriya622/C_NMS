
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

class DeviceManagerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        clear_devices();
    }

    void TearDown() override
    {
        clear_devices();
    }

    void addTestDevice(int id, const char *name)
    {
        Device d = makeDevice(id, name);
        ASSERT_EQ(add_device(&d), 1);
    }
};

TEST_F(DeviceManagerTest, AddDevice)
{
    Device d = makeDevice(101, "Router1");

    EXPECT_EQ(add_device(&d), 1);
    EXPECT_EQ(get_device_count(), 1);
}

TEST_F(DeviceManagerTest, AddDeviceRejectsDuplicateId)
{
    Device d = makeDevice(102, "Router1");
    ASSERT_EQ(add_device(&d), 1);

    Device duplicate = makeDevice(102, "Router2");
    EXPECT_EQ(add_device(&duplicate), 0);
    EXPECT_EQ(get_device_count(), 1);
}

TEST_F(DeviceManagerTest, AddDeviceRejectsNullPointer)
{
    EXPECT_EQ(add_device(nullptr), 0);
}

TEST_F(DeviceManagerTest, UpdateDevice)
{
    addTestDevice(103, "RouterOld");

    Device updated = makeDevice(103, "RouterNew");

    EXPECT_EQ(update_device(103, &updated), 1);

    Device result = {};
    ASSERT_EQ(get_device_by_id(103, &result), 1);
    EXPECT_STREQ(result.name, "RouterNew");
}

TEST_F(DeviceManagerTest, DeleteDevice)
{
    addTestDevice(104, "RouterToDelete");

    EXPECT_EQ(delete_device(104), 1);
    EXPECT_EQ(find_device_index(104), -1);
    EXPECT_EQ(get_device_count(), 0);
}

TEST_F(DeviceManagerTest, FindDeviceIndex)
{
    addTestDevice(105, "Router5");

    EXPECT_GE(find_device_index(105), 0);
    EXPECT_EQ(find_device_index(999), -1);
}

TEST_F(DeviceManagerTest, GetDeviceById)
{
    addTestDevice(106, "Router6");

    Device result = {};
    EXPECT_EQ(get_device_by_id(106, &result), 1);
    EXPECT_STREQ(result.name, "Router6");
    EXPECT_EQ(get_device_by_id(999, &result), 0);
}

TEST_F(DeviceManagerTest, ListDevices)
{
    addTestDevice(107, "Router7");
    EXPECT_NO_THROW(list_devices());
}

TEST_F(DeviceManagerTest, GetDeviceCount)
{
    EXPECT_EQ(get_device_count(), 0);

    addTestDevice(108, "Router8");
    EXPECT_EQ(get_device_count(), 1);
}

TEST_F(DeviceManagerTest, GetDeviceAt)
{
    addTestDevice(109, "Router9");

    Device result = {};
    EXPECT_EQ(get_device_at(0, &result), 1);
    EXPECT_EQ(result.id, 109);
    EXPECT_EQ(get_device_at(99, &result), 0);
}

TEST_F(DeviceManagerTest, ClearDevices)
{
    addTestDevice(110, "Router10");

    clear_devices();

    EXPECT_EQ(get_device_count(), 0);
}

TEST(MonitoringTest, MonitorDevice)
{
    clear_devices();

    Device d = makeDevice(201, "MonitorRouter");
    ASSERT_EQ(add_device(&d), 1);

    EXPECT_NO_THROW(monitor_device(201));
    EXPECT_NO_THROW(monitor_device(999));
}

TEST(MonitoringTest, MonitorAllDevices)
{
    clear_devices();

    Device d = makeDevice(202, "MonitorAllRouter");
    ASSERT_EQ(add_device(&d), 1);

    EXPECT_NO_THROW(monitor_all_devices());
}

TEST(MonitoringTest, DisplayDeviceMetrics)
{
    clear_devices();

    Device d = makeDevice(203, "MetricsRouter");
    ASSERT_EQ(add_device(&d), 1);

    EXPECT_NO_THROW(display_device_metrics(203));
    EXPECT_NO_THROW(display_device_metrics(999));
}

TEST(FaultDetectionTest, DetectDeviceFaults)
{
    clear_devices();

    Device healthy = makeDevice(301, "HealthyRouter");
    ASSERT_EQ(add_device(&healthy), 1);

    EXPECT_EQ(detect_device_faults(301), 0);
    EXPECT_EQ(detect_device_faults(999), -1);
}

TEST(FaultDetectionTest, DetectAllFaults)
{
    clear_devices();

    Device faulty = makeDevice(302, "FaultyRouter");
    faulty.status = STATUS_DOWN;
    faulty.cpu = 95.0f;
    faulty.memory = 95.0f;
    faulty.packetLoss = 5.0f;
    faulty.responseTime = 300.0f;

    ASSERT_EQ(add_device(&faulty), 1);
    EXPECT_NO_THROW(detect_all_faults());
}

TEST(AlarmManagerTest, CreateAlarm)
{
    clear_devices();

    Device d = makeDevice(401, "AlarmRouter");
    ASSERT_EQ(add_device(&d), 1);

    int before = get_alarm_count();

    create_alarm(401, "High CPU usage", SEV_MAJOR);

    EXPECT_EQ(get_alarm_count(), before + 1);
}

TEST(AlarmManagerTest, AcknowledgeAlarm)
{
    clear_devices();

    Device d = makeDevice(402, "AckRouter");
    ASSERT_EQ(add_device(&d), 1);

    int before = get_alarm_count();
    create_alarm(402, "Acknowledgement test", SEV_MINOR);

    ASSERT_EQ(get_alarm_count(), before + 1);

    EXPECT_NO_THROW(acknowledge_alarm(999999));
}

TEST(AlarmManagerTest, ClearAlarm)
{
    clear_devices();

    Device d = makeDevice(403, "ClearRouter");
    ASSERT_EQ(add_device(&d), 1);

    int before = get_alarm_count();
    create_alarm(403, "Clear test", SEV_WARNING);

    ASSERT_EQ(get_alarm_count(), before + 1);

    EXPECT_NO_THROW(clear_alarm(999999));
}

TEST(AlarmManagerTest, ListAlarms)
{
    EXPECT_NO_THROW(list_alarms());
}

TEST(AlarmManagerTest, GetAlarmCount)
{
    int before = get_alarm_count();

    EXPECT_GE(before, 0);
}

TEST(EventLoggerTest, LogEvent)
{
    int before = get_event_count();

    log_event("NMS test event");

    EXPECT_EQ(get_event_count(), before + 1);
}

TEST(EventLoggerTest, ListEvents)
{
    log_event("Event list test");

    EXPECT_NO_THROW(list_events());
}

TEST(EventLoggerTest, GetEventCount)
{
    int before = get_event_count();

    log_event("Event count test");

    EXPECT_EQ(get_event_count(), before + 1);
}

TEST(StorageTest, SaveDevices)
{
    clear_devices();

    Device d = makeDevice(601, "SaveRouter");
    ASSERT_EQ(add_device(&d), 1);

    EXPECT_EQ(save_devices(), 0);
}

TEST(StorageTest, LoadDevices)
{
    clear_devices();

    Device d = makeDevice(602, "LoadRouter");
    ASSERT_EQ(add_device(&d), 1);
    ASSERT_EQ(save_devices(), 0);

    clear_devices();

    EXPECT_EQ(load_devices(), 0);

    Device result = {};
    EXPECT_EQ(get_device_by_id(602, &result), 1);
    EXPECT_STREQ(result.name, "LoadRouter");

    clear_devices();
}

TEST(StorageTest, SaveAlarms)
{
    clear_devices();

    Device d = makeDevice(603, "SaveAlarmRouter");
    ASSERT_EQ(add_device(&d), 1);

    int before = get_alarm_count();
    create_alarm(603, "Storage alarm test", SEV_MAJOR);

    ASSERT_EQ(get_alarm_count(), before + 1);
    EXPECT_EQ(save_alarms(), 0);
}

TEST(StorageTest, LoadAlarms)
{
    clear_devices();

    Device d = makeDevice(604, "LoadAlarmRouter");
    ASSERT_EQ(add_device(&d), 1);

    create_alarm(604, "Alarm load test", SEV_MINOR);
    ASSERT_EQ(save_alarms(), 0);

    EXPECT_EQ(load_alarms(), 0);
}
