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

    for(zone = 0; zone < IOT_NUMBER_OF_ZONE; zone++)
    {
        if(ReceivedBuffer.display[zone] & dbit(seg_db))
        {
            ZoneDot[zone] = true;
        }
        else
        {
            ZoneDot[zone] = false;
        }
        ReceivedBuffer.display[zone] &= ~dbit(seg_db);
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
        //------------------------------
        if(ZoneErrors[zone].bits.F1_Error == 1)
        {
            if(ZoneChar[zone] != 'F' && ZoneChar[zone] != '1')
            {
                ZoneErrors[zone].bits.F1_Error = 0;
            }
        }
        else
        {
            if(ZoneChar[zone] == 'F')
            {
                ZoneErrors[zone].bits.F1_Error = 1;
            }
        }
        //-----------------------------
        if(error_flag == true)
        {
            switch(ZoneChar[zone])
            {
                case '1':
                    ZoneErrors[zone].bits.E1_Error = 1;
                    break;
                case '2':
                    ZoneErrors[zone].bits.E2_Error = 1;
                    break;
                case '3':
                    ZoneErrors[zone].bits.E3_Error = 1;
                    break;
                case '4':
                    ZoneErrors[zone].bits.E4_Error = 1;
                    break;
                case 'E':
                    break;
                default:
                    error_flag = false;
                    break;
            }
        }
        else
        {
            if(ZoneChar[zone] == 'E')
            {
                error_flag = true;
            }
            else
            {
                ZoneErrors[zone].bits.E1_Error = 0;
                ZoneErrors[zone].bits.E2_Error = 0;
                ZoneErrors[zone].bits.E3_Error = 0;
                ZoneErrors[zone].bits.E4_Error = 0;
            }
        }
        if(TimeZoneChar[0] == 'E' && TimeZoneChar[1] == 'R')
        {
            ZoneErrors[zone].bits.Touch_Error = 1;
        }
        else if(ZoneErrors[zone].bits.Touch_Error == 1)
        {
            if(TimeZoneChar[0] != '0' && TimeZoneChar[1] != '3')
            {
                ZoneErrors[zone].bits.Touch_Error = 0;
            }
        }
    }
    ReceivedBuffer.display[IOT_NUMBER_OF_ZONE] &= ~dbit(seg_db);
    ReceivedBuffer.display[IOT_NUMBER_OF_ZONE + 1] &= ~dbit(seg_db);
    TimeZoneChar[0] = Asci_Contert_Of_Char(CharacterCheck(ReceivedBuffer.display[IOT_NUMBER_OF_ZONE + 1]));
    TimeZoneChar[1] = Asci_Contert_Of_Char(CharacterCheck(ReceivedBuffer.display[IOT_NUMBER_OF_ZONE]));
}
/******************************************************/
void Iot_Com::display_procces()
{
    RxCount = 0;
    crc16 = IOT_I2C_CRC16_INIT;
    Wire.requestFrom(0x60, IOT_GET_BYTE_COUNT);
}
/******************************************************/
/******************************************************/
/******************************************************/
