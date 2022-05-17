/*
    IOT Hob Control

    Created on: May 12, 2022

    Author: Coskun ERGAN

*/

#include "iot_com.h"

uint32_t SendKey = 0;
uint8_t KeyRetry = 0;

uint8_t KeyListStart = 0;
uint8_t KeyListEnd = 0;
uint32_t KeyListBuffer[KEY_LIST_SIZE];
uint8_t KeyTimeListBuffer[KEY_LIST_SIZE];

/******************************************************/
/******************************************************/
/******************************************************/
bool Iot_Com::key_pop(uint32_t *key, uint8_t *time)
{
    if(KeyListEnd - KeyListStart == 0)
    {
        return false;
    }
    *key = KeyListBuffer[KeyListStart];
    *time = KeyTimeListBuffer[KeyListStart];
    if(++KeyListStart >= KEY_LIST_SIZE)
    {
        KeyListStart = 0;
    }
    return true;
}
/******************************************************/
bool Iot_Com::key_push(uint32_t key, uint8_t time)
{
    KeyListBuffer[KeyListEnd] = key;
    KeyTimeListBuffer[KeyListEnd] = time;
    if(++KeyListEnd >= KEY_LIST_SIZE)
    {
        KeyListEnd = 0;
    }
    return true;
}
/******************************************************/
void Iot_Com::key_procces()
{
    if(KeyRetry)
    {
        key_send(SendKey);
        if(--KeyRetry == 0)
        {
            KeyRetry = 0;
            SendKey = 0;
        }
        else if(KeyRetry == 3)
        {
            SendKey = 0;
        }
    }
    else
    {
        key_pop(&SendKey, &KeyRetry);
    }
}
/******************************************************/
/******************************************************/
/******************************************************/