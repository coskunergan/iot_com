/*
    IOT Hob Control

    Created on: May 12, 2022

    Author: Coskun ERGAN

*/

#include "iot_com.h"

uint8_t KeyListStart = 0;
uint8_t KeyListEnd = 0;
uint32_t KeyListBuffer[KEY_LIST_SIZE];

extern uint32_t KeyFeedback;
uint32_t SendKey;
bool releaseKey = false;

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
bool Iot_Com::key_pop()
{
    if(KeyListEnd - KeyListStart == 0)
    {
        return false;
    }
    if(++KeyListStart >= KEY_LIST_SIZE)
    {
        KeyListStart = 0;
    }
    return true;
}
/******************************************************/
bool Iot_Com::key_push(uint32_t key)
{
    KeyListBuffer[KeyListEnd] = key;
    if(++KeyListEnd >= KEY_LIST_SIZE)
    {
        KeyListEnd = 0;
    }
    return true;
}
/******************************************************/
void Iot_Com::key_procces()
{
    if(releaseKey == true)
    {
        if(KeyFeedback != 0)
        {
            key_send(SendKey);
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
            key_pop();
            SendKey = 0;
            releaseKey = true;
        }
        else
        {
            key_send(SendKey);
        }
    }
}
/******************************************************/
/******************************************************/
/******************************************************/