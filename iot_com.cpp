/*
    IOT Hob Control

    Created on: May 12, 2022

    Author: Coskun ERGAN

*/

#include "iot_com.h"

/******************************************************/
/******************************************************/
/******************************************************/
Iot_Com::Iot_Com(uint8_t addr, uint16_t pin_sda, uint16_t pin_clk)
{
    this->i2c_addr = addr;
    Wire.begin(pin_sda, pin_clk);
    init();
}
/******************************************************/
void Iot_Com::init()
{

}
/******************************************************/
void Iot_Com::key_send(uint32_t keys)
{
    uint8_t buffer[4];
    Wire.beginTransmission(0x60);
    buffer[0] = keys;
    buffer[1] = keys >> 8;
    buffer[2] = keys >> 16;
    buffer[3] = keys >> 24;
    Wire.write(buffer, 4);
    Wire.endTransmission();
}
/******************************************************/
/******************************************************/
/******************************************************/
