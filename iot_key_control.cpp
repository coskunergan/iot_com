/*
    IOT Hob Control

    Created on: May 12, 2022

    Author: Coskun ERGAN

*/

#include "iot_com.h"

/******************************************************/
/******************************************************/
/******************************************************/
uint8_t Iot_Com::key_count()
{
    return KeyListEnd - KeyListStart;
}
/******************************************************/
bool Iot_Com::key_get(uint32_t *key)
{
    if(KeyListEnd - KeyListStart == 0)
    {
        *key = 0;
        return false;
    }
    *key = KeyListBuffer[KeyListStart];
    return true;
}
/******************************************************/
Iot_Status_t Iot_Com::key_pop()
{
    if(KeyListEnd - KeyListStart == 0)
    {
        return IOT_FAIL;
    }
    if(++KeyListStart >= KEY_LIST_SIZE)
    {
        KeyListStart = 0;
    }
    return IOT_SUCCES;
}
/******************************************************/
Iot_Status_t Iot_Com::key_push(uint32_t key)
{
    if((device_status == UNSUPPORTED_API) || (device_status == UNSUPPORTED_DEVICE) || (device_status == DEVICE_LOST))
    {
        return IOT_FAIL;
    }
    if(key_count() >= KEY_LIST_SIZE)
    {
        return IOT_FAIL;
    }
    KeyListBuffer[KeyListEnd] = key;
    if(++KeyListEnd >= KEY_LIST_SIZE)
    {
        KeyListEnd = 0;
    }
    return IOT_SUCCES;
}
/******************************************************/
Iot_Status_t Iot_Com::key_send(uint32_t keys)
{
    uint8_t buffer[SEND_BYTE_COUNT], i;
    Wire.beginTransmission(0x60);
    buffer[0] = keys;
    buffer[1] = keys >> 8;
    buffer[2] = keys >> 16;
    buffer[3] = keys >> 24;
    crc16 = I2C_CRC16_INIT;
    for(i = 0; i < SEND_BYTE_COUNT - 2; i++)
    {
        Crc16_Calc_Byte(buffer[i]);
    }
    buffer[4] = crc16 >> 8;
    buffer[5] = crc16 & 0xFF;
    Wire.write(buffer, SEND_BYTE_COUNT);
    Wire.endTransmission();
    return IOT_SUCCES;
}
/******************************************************/
Iot_Status_t Iot_Com::key_procces()
{
    uint32_t key;   

    if(key_release == true)
    {
        if(ReceivedBuffer.keyfeedback != KEY_RELEASE)
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
        key_get(&key);

        if(key == ReceivedBuffer.keyfeedback)
        {
            key_release = true;
            key_pop();
        }
        else
        {
            return key_send(key);
        }
    }
    return IOT_SUCCES;
}
/******************************************************/
/******************************************************/
/******************************************************/