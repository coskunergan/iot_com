/*
    IOT Hob Control

    Created on: May 12, 2022

    Author: Coskun ERGAN

*/

#include "iot_com.h"

unsigned long DisplayTimePreviousMillis = 0;
unsigned long KeyTimePreviousMillis = 0;

/******************************************************/
/******************************************************/
/******************************************************/
void Iot_Com::procces()
{
    //----------------------
    // while(0 < Wire.available())
    // {
    //     uint8_t temp = Wire.read();
    //      Serial.print("(");
    //      Serial.print(Asci_Contert_Of_Char(Char_Contert_Of_Display(Display_Buffer[Rx_Count])));
    //      Serial.print(")");
    //      Rx_Count++;
    //      if(Rx_Count >= 6)
    //      {
    //          Serial.println();
    //      }
    // }
    //----------------------
    unsigned long currentMillis = millis();
    if(currentMillis - KeyTimePreviousMillis >= KEY_TIME_TICK_MS)
    {
        KeyTimePreviousMillis = currentMillis;
        key_procces();
    }
    if(currentMillis - DisplayTimePreviousMillis >= DISPLAY_TIME_TICK_MS)
    {
        DisplayTimePreviousMillis = currentMillis;
        display_procces();
    }
    //----------------------
}
/******************************************************/
void Iot_Com::power_on()
{
    key_push(KEY_BITS(KEY_ON_OFF) | KEY_BITS(KEY_LONG), KEY_TIME_MS(300));
}
/******************************************************/
void Iot_Com::power_off()
{
    key_push(KEY_BITS(KEY_ON_OFF), KEY_TIME_MS(150));
}
/******************************************************/
/******************************************************/
/******************************************************/