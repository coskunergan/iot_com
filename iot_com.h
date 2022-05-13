/*
    IOT Hob Control

    Created on: May 12, 2022

    Author: Coskun ERGAN
	
*/

#ifndef IOT_COM_H
#define IOT_COM_H

#include <Wire.h>

#define LIB_VERSION 0x100

class Iot_Com {
  
  private:
    byte pin;
    
  public:
    
    Iot_Com(byte pin_sda,byte pin_clk);


    void init();

    void procces();
    
    void power_on();

    void power_off();
};
#endif