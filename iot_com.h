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

class Iot_Com
{
private:
    uint8_t i2c_addr;
    void key_procces();
    bool key_pop(uint32_t *key, uint8_t *time);
    bool key_push(uint32_t key, uint8_t time);
    void key_send(uint32_t keys);
    void display_procces();
public:
    Iot_Com(uint8_t addr, uint16_t pin_sda, uint16_t pin_clk);
    void init();
    void procces();
    void power_on();
    void power_off();
};
#endif