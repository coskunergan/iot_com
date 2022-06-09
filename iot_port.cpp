/*
    IOT Hob Control

    Created on: May 12, 2022

    Author: Coskun ERGAN

*/

#include "iot_com.h"

/******************************************************/
/******************************************************/
/******************************************************/
void Iot_Com::procces()
{
    static uint32_t DisplayTimePreviousMillis = 0;
    static uint32_t KeyTimePreviousMillis = 0;
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
        character_handler();
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
Iot_Status_t Iot_Com::power_on()
{
    return key_push(KEY_BITS(KEY_ON_OFF) | KEY_BITS(KEY_LONG));
}
/******************************************************/
Iot_Status_t Iot_Com::power_off()
{
    return key_push(KEY_BITS(KEY_ON_OFF));
}
/******************************************************/
Iot_Status_t Iot_Com::set_level(Zone_t zone, Level_t level)
{
    Iot_Status_t result = IOT_SUCCES;

    if(zone >= NUMBER_OF_ZONE || level > LEVEL_DB)
    {
        result =  IOT_FAIL;
        return result;
    }
    result = key_push(KEY_BITS(KEY_ZONE1 + zone) | KEY_BITS(KEY_LONG));
    switch(level)
    {
        case LEVEL_0:
            result = key_push(KEY_BITS(KEY_SA0));
            break;
        case LEVEL_1:
            result = key_push(KEY_BITS(KEY_SA1));
            break;
        case LEVEL_2:
            result = key_push(KEY_BITS(KEY_SA1) | KEY_BITS(KEY_SA3));
            break;
        case LEVEL_3:
            result = key_push(KEY_BITS(KEY_SA3));
            break;
        case LEVEL_4:
            result = key_push(KEY_BITS(KEY_SA3) | KEY_BITS(KEY_SA5));
            break;
        case LEVEL_5:
            result = key_push(KEY_BITS(KEY_SA5));
            break;
        case LEVEL_6:
            result = key_push(KEY_BITS(KEY_SA5) | KEY_BITS(KEY_SA7));
            break;
        case LEVEL_7:
            result = key_push(KEY_BITS(KEY_SA7));
            break;
        case LEVEL_8:
            result = key_push(KEY_BITS(KEY_SA7) | KEY_BITS(KEY_SA9));
            break;
        case LEVEL_9:
            result = key_push(KEY_BITS(KEY_SA9));
            break;
        case LEVEL_B:
            result = key_push(KEY_BITS(KEY_BOOST));
            break;
    }
    return result;
}
/******************************************************/
Iot_Status_t Iot_Com::get_level(Zone_t zone, Level_t *level)
{
    if(zone >= NUMBER_OF_ZONE)
    {
        return IOT_FAIL;
    }
    *level = ZoneLevel[zone];
    return IOT_SUCCES;
}
/******************************************************/
/******************************************************/
/******************************************************/