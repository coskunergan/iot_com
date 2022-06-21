/*
    IOT Hob Control

    Created on: May 12, 2022

    Author: Coskun ERGAN

*/

#include "iot_com.h"


/******************************************************/
/******************************************************/
/******************************************************/
Iot_Status_t Iot_Com::key_send(uint32_t keys)
{
    uint8_t buffer[IOT_SEND_BYTE_COUNT], i;
    Wire.beginTransmission(0x60);
    buffer[0] = keys;
    buffer[1] = keys >> 8;
    buffer[2] = keys >> 16;
    buffer[3] = keys >> 24;
    crc16 = IOT_I2C_CRC16_INIT;
    for(i = 0; i < IOT_SEND_BYTE_COUNT - 2; i++)
    {
        Crc16_Calc_Byte(buffer[i]);
    }
    buffer[4] = crc16 >> 8;
    buffer[5] = crc16 & 0xFF;
    Wire.write(buffer, IOT_SEND_BYTE_COUNT);
    Wire.endTransmission();
    return IOT_SUCCES;
}
/******************************************************/
Iot_Status_t Iot_Com::key_procces()
{
    uint32_t key;

    if(key_release == true)
    {
        if(receive_buffer.keyfeedback != KEY_RELEASE)
        {
            return key_send(KEY_RELEASE);
        }
        else
        {
            key_release = false;
        }
    }
    else
    {
        key = key_list.back();

        if(key == receive_buffer.keyfeedback)
        {
            //if((key & KEY_SLIDER_BAR) != key)
            {
                key_release = true;
            }
            if(key_list.size())
            {
                key_list.pop_back();
            }
        }
        else
        {
            if(key == 0 && authority_check == false)
            {
                authority_check = true;
                safety_start_timer = IOT_AUTH_TIMEOUT_SEC;
            }
            return key_send(key);
        }
    }
    return IOT_SUCCES;
}
/******************************************************/
/******************************************************/
/******************************************************/
