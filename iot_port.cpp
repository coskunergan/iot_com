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
                api_version = temp;
                if(api_version != API_VERSION)
                {
                    device_status = UNSUPPORTED_API;
                }
                break;
            case 1:
                device_type = (DeviceType_t)temp;
                if(device_type != DEVICE_TYPE)
                {
                    device_status = UNSUPPORTED_DEVICE;
                }
                break;
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                DisplayBuffer[RxCount - 2] = temp;
                break;
            case 8:
                KeyFeedback = temp;
                break;
            case 9:
                KeyFeedback |= ((uint32_t)temp << 8);
                break;
            case 10:
                KeyFeedback |= ((uint32_t)temp << 16);
                break;
            case 11:
                KeyFeedback |= ((uint32_t)temp << 24);
                break;
            default:
                break;
        }
        RxCount++;
        if((device_status != UNSUPPORTED_API) && (device_status != UNSUPPORTED_DEVICE))
        {
            if(DisplayBuffer[0] == 0x9) // PAUSE KEY
            {
                device_status = DEVICE_PAUSE;
            }
            else if(DisplayBuffer[0] | DisplayBuffer[1] | DisplayBuffer[2] | DisplayBuffer[3])
            {
                device_status = DEVICE_ON;
            }
            else
            {
                device_status = DEVICE_OFF;
            }
        }
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
Iot_CommandStatus_t Iot_Com::get_command_status()
{
    return command_status;
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
}
/******************************************************/
/******************************************************/
/******************************************************/