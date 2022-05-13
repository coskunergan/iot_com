/*
    IOT Hob Control

    Created on: May 12, 2022

    Author: Coskun ERGAN

*/

#include "iot_com.h"

Iot_Com::Iot_Com(byte pin_sda, byte pin_clk)
{
    Wire.begin(pin_sda, pin_clk);
    init();
}

void Iot_Com::init()
{


}

void Iot_Com::procces()
{

}

void Iot_Com::power_on()
{

}

void Iot_Com::power_off()
{

}