/*
    IOT Hob Control

    Created on: May 12, 2022

    Author: Coskun ERGAN

*/

#include "iot_com.h"

/******************************************************/
/******************************************************/
/******************************************************/
Character_t Iot_Com::CharacterCheck(uint8_t byte)
{
    for(uint8_t i = 0; i < sizeof(Display_Char_Table); i++)
    {
        if(Display_Char_Table[i] == byte)
        {
            return (Character_t)i;
        }
    }
    return (Character_t)sizeof(Display_Char_Table);
}
/******************************************************/
char Iot_Com:: Asci_Contert_Of_Char(Character_t character)
{
    char result;
    switch(character)
    {
        case CHAR_0:
        case CHAR_1:
        case CHAR_2:
        case CHAR_3:
        case CHAR_4:
        case CHAR_5:
        case CHAR_6:
        case CHAR_7:
        case CHAR_8:
        case CHAR_9:
            result = '0' + character;
            break;
        case CHAR_A:
        case CHAR_B:
        case CHAR_C:
        case CHAR_D:
        case CHAR_E:
        case CHAR_F:
            result = 'A' + character - CHAR_A;
            break;
        case CHAR_MINUS:
            result = '-';
            break;
        case  CHAR_EQUAL:
            result = '=';
            break;
        case  CHAR_L:
            result = 'L';
            break;
        case  CHAR_o:
            result = 'o';
            break;
        case  CHAR_P:
            result = 'P';
            break;
        case  CHAR_R:
            result = 'R';
            break;
        case  CHAR_U:
            result = 'U';
            break;
        case  CHAR__:
            result = '_';
            break;
        case  CHAR_H:
            result = 'H';
            break;
        case  CHAR_BRIDGE:
            result = ']';
            break;
        case  CHAR_WARM:
            result = '#';
            break;
        case  CHAR_DUAL:
            result = 'd';
            break;
        default:
            result = ' ';
            break;
    }
    return result;
}
/******************************************************/
void Iot_Com::character_handler()
{
    uint8_t zone;

    for(zone = 0; zone < NUMBER_OF_ZONE; zone++)
    {
        ZoneChar[zone] = Asci_Contert_Of_Char(CharacterCheck(ReceivedBuffer.display[zone]));
        if(ZoneChar[zone] >= '0' && ZoneChar[zone] <= '9')
        {
            ZoneLevel[zone] = (Level_t)(ZoneChar[zone] - '0');
        }
        else if(ZoneChar[zone] == 'P')
        {
            ZoneLevel[zone] = LEVEL_B;
        }
        else if(ZoneChar[zone] == '-')
        {
            ZoneLevel[zone] = LEVEL_0;
        }
    }
    TimeZoneChar[1] = Asci_Contert_Of_Char(CharacterCheck(ReceivedBuffer.display[NUMBER_OF_ZONE]));
    TimeZoneChar[0] = Asci_Contert_Of_Char(CharacterCheck(ReceivedBuffer.display[NUMBER_OF_ZONE + 1]));
}
/******************************************************/
void Iot_Com::display_procces()
{
    RxCount = 0;
    crc16 = I2C_CRC16_INIT;
    Wire.requestFrom(0x60, GET_BYTE_COUNT);
}
/******************************************************/
/******************************************************/
/******************************************************/