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
Iot_Status_t Iot_Com::key_procces()
{
    if(releaseKey == true)
    {
        if(KeyFeedback != 0)
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

        if(SendKey == KeyFeedback)
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