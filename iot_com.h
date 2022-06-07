/*
    IOT Hob Control

    Created on: May 12, 2022

    Author: Coskun ERGAN

*/

#ifndef IOT_COM_H
#define IOT_COM_H

#include <Wire.h>

#define LIB_VERSION           0x100
#define KEY_TIME_TICK_MS      50
#define DISPLAY_TIME_TICK_MS  100
#define KEY_LIST_SIZE         32

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
    DEVICE_OFF = 0,
    DEVICE_ON,
    DEVICE_LOCK,
    DEVICE_PAUSE
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
    bool key_pop(uint32_t *key, uint8_t *time);
    bool key_push(uint32_t key, uint8_t time);
    void key_send(uint32_t keys);
    void display_procces();
public:
    Iot_Com(uint8_t addr, uint16_t pin_sda, uint16_t pin_clk);
    void init();
    void procces();
    Iot_ZoneErrors_t get_zone_error(Zone_t zone);
    Iot_DeviceStatus_t get_device_status();
    Iot_CommandStatus_t get_command_status();
    void power_on();
    void power_off();
};
#endif