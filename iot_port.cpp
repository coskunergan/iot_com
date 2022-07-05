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
    static uint8_t second_count = 0;
    bool crc_check = false;
    //----------------------
    while(0 < Wire.available())
    {
        uint8_t *temp = (uint8_t *)&temp_buffer;
        temp += rx_count;
        *temp = Wire.read();

        if(rx_count < IOT_GET_BYTE_COUNT - 2)
        {
            Crc16_Calc_Byte(*temp);
        }
        else if(rx_count == IOT_GET_BYTE_COUNT - 1)
        {
            if(crc16 == temp_buffer.crc16)
            {
                crc_check = true;
                receive_buffer = temp_buffer;
            }
        }

        if(crc_check == true)
        {
            crc_check = false;
            if(receive_buffer.api_version != IOT_API_VERSION)
            {
                device_status = UNSUPPORTED_API;
            }
            api_version = (DeviceType_t)receive_buffer.api_version;
            if(receive_buffer.device_type != IOT_DEVICE_TYPE)
            {
                device_status = UNSUPPORTED_DEVICE;
            }
            device_type = (DeviceType_t)receive_buffer.device_type;
            if((device_status != UNSUPPORTED_API) && (device_status != UNSUPPORTED_DEVICE))
            {
                if(receive_buffer.display[0] == 0x9) // PAUSE KEY
                {
                    device_status = DEVICE_PAUSE;
                }
                else if(receive_buffer.display[0] && zone_status[0].bits.heat == 0)
                {
                    device_status = DEVICE_ON;
                }
                else if(receive_buffer.display[1] && zone_status[1].bits.heat == 0)
                {
                    device_status = DEVICE_ON;
                }
                else if(receive_buffer.display[2] && zone_status[2].bits.heat == 0)
                {
                    device_status = DEVICE_ON;
                }
                else if(receive_buffer.display[3] && zone_status[3].bits.heat == 0)
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
        rx_count++;
    }
    //----------------------
    uint32_t currentMillis = IOT_TIME_MS;
    if(currentMillis - KeyTimePreviousMillis >= IOT_KEY_SEND_TIME_MS)
    {
        KeyTimePreviousMillis = currentMillis;
        key_procces();
    }
    if(currentMillis - DisplayTimePreviousMillis >= IOT_DISPLAY_GET_TIME_MS)
    {
        DisplayTimePreviousMillis = currentMillis;
        if(rx_count == 0)
        {
            if(++lost_connection_count > LOST_CONN_LIMIT)
            {
                device_status = DEVICE_LOST;
            }
        }
        else
        {
            lost_connection_count = 0;
        }
        display_procces();
        if(++second_count > (1000 / IOT_DISPLAY_GET_TIME_MS))
        {
            second_count = 0;
            if(safety_start_timer)
            {
                if(--safety_start_timer == 0)
                {
                    authority_check = false;
                }
            }
        }
    }
    //----------------------
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
uint8_t Iot_Com::get_zone_status(Zone_t zone)
{
    return zone_status[zone].value;
}
/******************************************************/
uint8_t Iot_Com::get_zone_error(Zone_t zone)
{
    return zone_errors[zone].value;
}
/******************************************************/
bool Iot_Com::get_zone_dot_status(Zone_t zone)
{
    return dot_status[zone];
}
/******************************************************/
uint8_t Iot_Com::get_zone_timer_value(Zone_t zone)
{
    return timer_value[zone];
}
/******************************************************/
Iot_Status_t Iot_Com::power_on()
{
    if(device_status != DEVICE_OFF)
    {
        return IOT_FAIL;
    }
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
Iot_Status_t Iot_Com::set_zone_level(Zone_t zone, Iot_Level_t level)
{
    Iot_Status_t result = IOT_SUCCES;
    uint32_t prev_key;

    if(zone >= IOT_NUMBER_OF_ZONE || level > LEVEL_B)
    {
        result =  IOT_FAIL;
        return result;
    }
    if(authority_check == false)
    {
        if(level > zone_level[zone])
        {
            result =  IOT_FAIL;
            return result;
        }
    }

    prev_key = key_list.front();

    if((select_zone != zone) || (prev_key == KEY_RELEASE) || ((prev_key & KEY_SLIDER_BAR) != prev_key))// prev key is it select?
    {
        select_zone = zone;
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
Iot_Level_t Iot_Com::get_zone_level(Zone_t zone)
{
    return zone_level[zone];
}
/******************************************************/
/******************************************************/
/******************************************************/
