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
    uint8_t zone, temp = 0;

    for(zone = 0; zone < IOT_NUMBER_OF_ZONE; zone++)
    {
        //----------------- DOT CASE -------------------
        if(receive_buffer.display[zone] & dbit(seg_db))
        {
            zone_dot[zone] = true;
        }
        else
        {
            zone_dot[zone] = false;
        }
        if(dot_memory[zone] != zone_dot[zone])
        {
            dot_blink_count[zone] = 0;
            dot_memory[zone] = zone_dot[zone];
            if(zone_dot[zone])
            {
                dot_status[zone] = true;
                if(IS_IT_NUMBER(timezone_char[0]) && IS_IT_NUMBER(timezone_char[1]))
                {
                    timer_value[zone] = (timezone_char[0] - '0') * 10;
                    timer_value[zone] += (timezone_char[1] - '0');
                }
            }
        }
        else
        {
            if(++dot_blink_count[zone] > IOT_BLINK_TIMEOUT_COUNT)
            {
                dot_blink_count[zone] = 0;
                timer_value[zone] = 0;
                if(zone_dot[zone])
                {
                    dot_status[zone] = true;
                }
                else
                {
                    dot_status[zone] = false;
                }
            }
        }
        //-------------- CONVERT CHAR ------------------
        receive_buffer.display[zone] &= ~dbit(seg_db);
        zone_char[zone] = Asci_Contert_Of_Char(CharacterCheck(receive_buffer.display[zone]));
        //--------------- LEVEL CASE -------------------
        if(device_status != DEVICE_ON)
        {
            zone_level[zone] = LEVEL_0;
        }
        else if(IS_IT_NUMBER(zone_char[zone]))
        {
            zone_level[zone] = (Iot_Level_t)(zone_char[zone] - '0');
        }
        else if(zone_char[zone] == 'P')
        {
            zone_level[zone] = LEVEL_B;
        }
        //-------------- STATUS CASE -------------------
        if(display_memory[zone] != zone_char[zone])
        {
            display_blink_count[zone] = 0;
            display_memory[zone] = zone_char[zone];
            zone_status[zone].bits.burning = 0;
            zone_status[zone].bits.stby = 0;
            if(zone_char[zone] == 'U')
            {
                zone_status[zone].bits.no_pan = 1;
            }
            if(zone_char[zone] == 'H')
            {
                zone_status[zone].bits.heat = 1;
            }
        }
        else
        {
            if(++display_blink_count[zone] > IOT_BLINK_TIMEOUT_COUNT)
            {
                display_blink_count[zone] = 0;
                if(zone_char[zone] == 'U')
                {
                    zone_status[zone].bits.no_pan = 1;
                }
                else
                {
                    zone_status[zone].bits.no_pan = 0;
                }
                if(zone_char[zone] == 'H')
                {
                    zone_status[zone].bits.heat = 1;
                }
                else
                {
                    zone_status[zone].bits.heat = 0;
                }
                if(zone_char[zone] == '-')
                {
                    zone_status[zone].bits.stby = 1;
                }
                else
                {
                    zone_status[zone].bits.stby = 0;
                }
                if((zone_char[zone] >= '1' && zone_char[zone] <= '9') || (zone_char[zone] == 'P'))
                {
                    zone_status[zone].bits.burning = 1;
                }
                else
                {
                    zone_status[zone].bits.burning = 0;
                }
            }
        }
        //-------------- ERROR CASE---------------------
        if(zone_errors[zone].bits.F1_error == 1)
        {
            if(zone_char[zone] != 'F' && zone_char[zone] != '1')
            {
                zone_errors[zone].bits.F1_error = 0;
            }
        }
        else
        {
            if(zone_char[zone] == 'F')
            {
                zone_errors[zone].bits.F1_error = 1;
            }
        }
        //-----------------------------
        if(error_flag == true)
        {
            switch(zone_char[zone])
            {
                case '1':
                    zone_errors[zone].bits.E1_error = 1;
                    break;
                case '2':
                    zone_errors[zone].bits.E2_error = 1;
                    break;
                case '3':
                    zone_errors[zone].bits.E3_error = 1;
                    break;
                case '4':
                    zone_errors[zone].bits.E4_error = 1;
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
            if(zone_char[zone] == 'E')
            {
                error_flag = true;
            }
            else
            {
                zone_errors[zone].bits.E1_error = 0;
                zone_errors[zone].bits.E2_error = 0;
                zone_errors[zone].bits.E3_error = 0;
                zone_errors[zone].bits.E4_error = 0;
            }
        }
        if(timezone_char[0] == 'E' && timezone_char[1] == 'R')
        {
            zone_errors[zone].bits.touch_error = 1;
        }
        else if(zone_errors[zone].bits.touch_error == 1)
        {
            if(timezone_char[0] != '0' && timezone_char[1] != '3')
            {
                zone_errors[zone].bits.touch_error = 0;
            }
        }
    }
    receive_buffer.display[IOT_NUMBER_OF_ZONE] &= ~dbit(seg_db);
    receive_buffer.display[IOT_NUMBER_OF_ZONE + 1] &= ~dbit(seg_db);
    timezone_char[0] = Asci_Contert_Of_Char(CharacterCheck(receive_buffer.display[IOT_NUMBER_OF_ZONE + 1]));
    timezone_char[1] = Asci_Contert_Of_Char(CharacterCheck(receive_buffer.display[IOT_NUMBER_OF_ZONE]));
}
/******************************************************/
void Iot_Com::display_procces()
{
    rx_count = 0;
    crc16 = IOT_I2C_CRC16_INIT;
    Wire.requestFrom(IOT_I2C_SLAVE_ADR >> 1, IOT_GET_BYTE_COUNT);
}
/******************************************************/
/******************************************************/
/******************************************************/
