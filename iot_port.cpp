/*
    IOT Hob Control

    Created on: May 12, 2022

    Author: Coskun ERGAN

*/

#include "iot_com.h"

uint32_t DisplayTimePreviousMillis = 0;
uint32_t KeyTimePreviousMillis = 0;
uint32_t KeyFeedback = 0;
uint8_t DisplayBuffer[6];
uint8_t RxCount = 0;

/******************************************************/
/******************************************************/
/******************************************************/
void Iot_Com::procces()
{
    //----------------------
    while(0 < Wire.available())
    {
        uint8_t temp = Wire.read();
        switch(RxCount)
        {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                DisplayBuffer[RxCount] = temp;
                break;
            case 6:
                KeyFeedback = temp;
                break;
            case 7:
                KeyFeedback |= ((uint32_t)temp << 8);
                break;
            case 8:
                KeyFeedback |= ((uint32_t)temp << 16);
                break;
            case 9:
                KeyFeedback |= ((uint32_t)temp << 24);
                break;
            default:
                break;
        }
        RxCount++;
    }
    //----------------------
    uint32_t currentMillis = millis();
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
Iot_ZoneErrors_t get_zone_error(Zone_t zone)
{
    return ZONE_NO_ERROR;
}
/******************************************************/
Iot_DeviceStatus_t Iot_Com::get_device_status()
{

    return device_status;
}
/******************************************************/
uint8_t Iot_Com::get_command_status()
{
    return key_count();
    //return command_status;
}
/******************************************************/
void Iot_Com::power_on()
{
    key_push(KEY_BITS(KEY_ON_OFF) | KEY_BITS(KEY_LONG));
}
/******************************************************/
void Iot_Com::power_off()
{
    key_push(KEY_BITS(KEY_ON_OFF));
}
/******************************************************/
void Iot_Com::set_level(Zone_t zone, Level_t level)
{
    key_push(KEY_BITS(KEY_ZONE1 + zone) | KEY_BITS(KEY_LONG));
    //key_push(KEY_BITS(KEY_ZONE1 + zone));
    switch(level)
    {
        case LEVEL_0:
            key_push(KEY_BITS(KEY_SA0));
            break;
        case LEVEL_1:
            key_push(KEY_BITS(KEY_SA1));
            break;
        case LEVEL_2:
            key_push(KEY_BITS(KEY_SA1) | KEY_BITS(KEY_SA3));
            break;
        case LEVEL_3:
            key_push(KEY_BITS(KEY_SA3));
            break;
        case LEVEL_4:
            key_push(KEY_BITS(KEY_SA3) | KEY_BITS(KEY_SA5));
            break;
        case LEVEL_5:
            key_push(KEY_BITS(KEY_SA5));
            break;
        case LEVEL_6:
            key_push(KEY_BITS(KEY_SA5) | KEY_BITS(KEY_SA7));
            break;
        case LEVEL_7:
            key_push(KEY_BITS(KEY_SA7));
            break;
        case LEVEL_8:
            key_push(KEY_BITS(KEY_SA7) | KEY_BITS(KEY_SA9));
            break;
        case LEVEL_9:
            key_push(KEY_BITS(KEY_SA9));
            break;
        case LEVEL_B:
            key_push(KEY_BITS(KEY_BOOST));
            break;
    }
    //key_push(KEY_BITS(KEY_ZONE1 + zone));
}
/******************************************************/
/******************************************************/
/******************************************************/