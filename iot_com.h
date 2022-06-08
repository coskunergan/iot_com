/*
    IOT Hob Control

    Created on: May 12, 2022

    Author: Coskun ERGAN

*/

#ifndef IOT_COM_H
#define IOT_COM_H

#include <Wire.h>

#define API_VERSION           100 // V1.0.0
#define DEVICE_TYPE           I6S02B
#define KEY_TIME_TICK_MS      150
#define DISPLAY_TIME_TICK_MS   50
#define KEY_LIST_SIZE          32
#define GET_BYTE_COUNT         12

#define   KEY_ZONE1    3
#define   KEY_ZONE2    4
#define   KEY_ZONE3    5
#define   KEY_ZONE4    6
#define   KEY_SA3      10
#define   KEY_SA5      11
#define   KEY_SA7       13
#define   KEY_SA9       14
#define   KEY_SA1       15
#define   KEY_SA0       16
#define   KEY_LOCK      17
#define   KEY_ON_OFF    18
#define   KEY_BOOST     19
#define   KEY_TIMER     20

#define   KEY_LONG      32

#define KEY_BITS(x)  (1 << (x - 1))
#define KEY_TIME_MS(x)  (x / 30)

typedef enum
{
    I6S02B = 1,
    I9S02B = 2
} DeviceType_t;

typedef enum
{
    ZONE_1 = 0,
    ZONE_2,
    ZONE_3,
    ZONE_4,
    ZONE_5,
    ZONE_6,
    ZONE_7,
    ZONE_8,
} Zone_t;

typedef enum
{
    LEVEL_0 = 0,
    LEVEL_1,
    LEVEL_2,
    LEVEL_3,
    LEVEL_4,
    LEVEL_5,
    LEVEL_6,
    LEVEL_7,
    LEVEL_8,
    LEVEL_9,
    LEVEL_B,
    LEVEL_DB
} Level_t;

typedef enum
{
    ZONE_NO_ERROR = 0x00,
    ZONE_ERROR_F1 = 0x01, //  Data Communication fail
    ZONE_ERROR_E3 = 0x02, //  High voltahe
    ZONE_ERROR_E4 = 0x04, //  Low voltage
    ZONE_ERROR_E2 = 0x08, //  IGBT overheat
    ZONE_ERROR_E1 = 0x10, //  Extreme overheat
    ZONE_ERROR_EE = 0x20  //  NTC Communication fail
} Iot_ZoneErrors_t;

typedef enum
{
    DEVICE_LOST = 0,
    DEVICE_OFF,
    DEVICE_ON,
    DEVICE_LOCK,
    DEVICE_PAUSE,
    UNSUPPORTED_DEVICE,
    UNSUPPORTED_API
} Iot_DeviceStatus_t;

typedef enum
{
    COMMAND_READY = 0,
    COMMAND_BUSY,
    COMMAND_FAIL
} Iot_CommandStatus_t;

class Iot_Com
{
private:
    uint8_t i2c_addr;
    Iot_DeviceStatus_t device_status;
    Iot_CommandStatus_t command_status;
    Iot_ZoneErrors_t zone_errors;
    void key_procces();
    uint8_t key_count();
    bool key_get(uint32_t *key);
    bool key_pop();
    bool key_push(uint32_t key);
    void key_send(uint32_t keys);
    void display_procces();
public:
    uint8_t api_version;
    DeviceType_t device_type;
    Iot_Com(uint8_t addr, uint16_t pin_sda, uint16_t pin_clk);
    void init();
    void procces();
    Iot_ZoneErrors_t get_zone_error(Zone_t zone);
    Iot_DeviceStatus_t get_device_status();
    Iot_CommandStatus_t get_command_status();
    void power_on();
    void power_off();
    void set_level(Zone_t zone, Level_t level);
};
#endif