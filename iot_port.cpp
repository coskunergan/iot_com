/*
    IOT Hob Control

    Created on: May 12, 2022

    Author: Coskun ERGAN

*/

#include "iot_com.h"

/******************************************************/
/******************************************************/
/******************************************************/
void Iot_Com::Crc16_Calc_Byte(uint8_t byte)
{
    uint8_t j;
    crc16 ^= byte;
    for(j = 0; j < 8; j++)
    {
        if(crc16 & 0x01)
        {
            crc16 = (crc16 >> 1) ^ 0xA001;
        }
        else
        {
            crc16 = crc16 >> 1;
        }
    }
}
/******************************************************/
void Iot_Com::procces()
{
    static uint32_t DisplayTimePreviousMillis = 0;
    static uint32_t KeyTimePreviousMillis = 0;
    bool crc_check = false;
    //----------------------
    while(0 < Wire.available())
    {
        uint8_t *temp = (uint8_t *)&TempBuffer;
        temp += RxCount;
        *temp = Wire.read();

        if(RxCount < IOT_GET_BYTE_COUNT - 2)
        {
            Crc16_Calc_Byte(*temp);
        }
        else if(RxCount == IOT_GET_BYTE_COUNT - 1)
        {
            if(crc16 == TempBuffer.crc16)
            {
                crc_check = true;
                ReceivedBuffer = TempBuffer;
            }
        }

        if(crc_check == true)
        {
            crc_check = false;
            if(ReceivedBuffer.api_version != IOT_API_VERSION)
            {
                device_status = UNSUPPORTED_API;
            }
            api_version = (DeviceType_t)ReceivedBuffer.api_version;
            if(ReceivedBuffer.device_type != IOT_DEVICE_TYPE)
            {
                device_status = UNSUPPORTED_DEVICE;
            }
            device_type = (DeviceType_t)ReceivedBuffer.device_type;
            if((device_status != UNSUPPORTED_API) && (device_status != UNSUPPORTED_DEVICE))
            {
                if(ReceivedBuffer.display[0] == 0x9) // PAUSE KEY
                {
                    device_status = DEVICE_PAUSE;
                }
                else if(ReceivedBuffer.display[0] | ReceivedBuffer.display[1] | ReceivedBuffer.display[2] | ReceivedBuffer.display[3])
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
        RxCount++;
    }
    //----------------------
    uint32_t currentMillis = millis();
    if(currentMillis - KeyTimePreviousMillis >= IOT_KEY_TIME_TICK_MS)
    {
        KeyTimePreviousMillis = currentMillis;
        key_procces();
    }
    if(currentMillis - DisplayTimePreviousMillis >= IOT_DISPLAY_TIME_TICK_MS)
    {
        DisplayTimePreviousMillis = currentMillis;
        display_procces();
    }
    //----------------------
}
/******************************************************/
uint8_t Iot_Com::get_zone_error(Zone_t zone)
{
    return ZoneErrors[zone].Value;
}
/******************************************************/
Iot_DeviceStatus_t Iot_Com::get_device_status()
{
    return device_status;
}
/******************************************************/
uint8_t Iot_Com::get_api_version()
{
    return api_version;
}
/******************************************************/
DeviceType_t Iot_Com::get_device_type()
{
    return device_type;
}
/******************************************************/
char Iot_Com::get_zone_display(Zone_t zone)
{
    return ZoneChar[zone];
}
/******************************************************/
bool Iot_Com::get_zone_display_dot(Zone_t zone)
{
    return ZoneDot[zone];
}
/******************************************************/
char Iot_Com::get_timer_display_high()
{
    return TimeZoneChar[0];
}
/******************************************************/
char Iot_Com::get_timer_display_low()
{
    return TimeZoneChar[1];
}
/******************************************************/
Iot_Status_t Iot_Com::power_on()
{
    key_list.push_front(KEY_BITS(KEY_ON_OFF) | KEY_BITS(KEY_LONG));
    return IOT_SUCCES;
}
/******************************************************/
Iot_Status_t Iot_Com::power_off()
{
    key_list.push_front(KEY_BITS(KEY_ON_OFF));
    return IOT_SUCCES;
}
/******************************************************/
Iot_Status_t Iot_Com::set_level(Zone_t zone, Level_t level)
{
    Iot_Status_t result = IOT_SUCCES;
    uint32_t prev_key;

    if(zone >= IOT_NUMBER_OF_ZONE || level > LEVEL_B)
    {
        result =  IOT_FAIL;
        return result;
    }

    prev_key = key_list.front();

    if((SelectZone != zone) || (prev_key == KEY_RELEASE) || ((prev_key & KEY_SLIDER_BAR) != prev_key))// prev key is it select?
    {
        SelectZone = zone;
        key_list.push_front(KEY_BITS(KEY_ZONE1 + zone) | KEY_BITS(KEY_LONG));
    }
    else if((prev_key & KEY_SLIDER_BAR) == prev_key) // prev key is it slider?
    {
        key_list.pop_front();
    }
    switch(level)
    {
        case LEVEL_0:
            key_list.push_front(KEY_BITS(KEY_SA0));
            break;
        case LEVEL_1:
            key_list.push_front(KEY_BITS(KEY_SA1));
            break;
        case LEVEL_2:
            key_list.push_front(KEY_BITS(KEY_SA1) | KEY_BITS(KEY_SA3));
            break;
        case LEVEL_3:
            key_list.push_front(KEY_BITS(KEY_SA3));
            break;
        case LEVEL_4:
            key_list.push_front(KEY_BITS(KEY_SA3) | KEY_BITS(KEY_SA5));
            break;
        case LEVEL_5:
            key_list.push_front(KEY_BITS(KEY_SA5));
            break;
        case LEVEL_6:
            key_list.push_front(KEY_BITS(KEY_SA5) | KEY_BITS(KEY_SA7));
            break;
        case LEVEL_7:
            key_list.push_front(KEY_BITS(KEY_SA7));
            break;
        case LEVEL_8:
            key_list.push_front(KEY_BITS(KEY_SA7) | KEY_BITS(KEY_SA9));
            break;
        case LEVEL_9:
            key_list.push_front(KEY_BITS(KEY_SA9));
            break;
        case LEVEL_B:
            key_list.push_front(KEY_BITS(KEY_BOOST));
            break;
    }

    return result;
}
/******************************************************/
Iot_Status_t Iot_Com::get_level(Zone_t zone, Level_t *level)
{
    if(zone >= IOT_NUMBER_OF_ZONE)
    {
        return IOT_FAIL;
    }
    *level = ZoneLevel[zone];
    return IOT_SUCCES;
}
/******************************************************/
/******************************************************/
/******************************************************/
