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
    if(releaseKey == true)
    {
        if(ReceivedBuffer.keyfeedback != 0)
        {
            return key_send(SendKey);
        }
        else
        {
            releaseKey = false;
        }
    }
    else
    {
        key_get(&SendKey);

        if(SendKey == ReceivedBuffer.keyfeedback)
        {
            SendKey = 0;
            releaseKey = true;
            return key_pop();
        }
        else
        {
            return key_send(SendKey);
        }
    }
    return IOT_SUCCES;
}
/******************************************************/
/******************************************************/
/******************************************************/