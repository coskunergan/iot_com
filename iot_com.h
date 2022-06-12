/*
    IOT Hob Control

    Created on: May 12, 2022

    Author: Coskun ERGAN

*/

#ifndef IOT_COM_H
#define IOT_COM_H

#include <Wire.h>

#define API_VERSION            100 // V1.0.0
#define DEVICE_TYPE            I6S02B
#define KEY_TIME_TICK_MS       150
#define DISPLAY_TIME_TICK_MS   50
#define KEY_LIST_SIZE          32
#define GET_BYTE_COUNT         14
#define SEND_BYTE_COUNT        6
#define NUMBER_OF_ZONE         4
#define I2C_CRC16_INIT         0xFFFF


#define   KEY_RELEASE  0
#define   KEY_ZONE1    3
#define   KEY_ZONE2    4
#define   KEY_ZONE3    5
#define   KEY_ZONE4    6
#define   KEY_SA3      10
#define   KEY_SA5      11
#define   KEY_SA7       13
#define   KEY_SA9       14
#define   KEY_SA1       15
#define   KEY_SA0       16
#define   KEY_LOCK      17
#define   KEY_ON_OFF    18
#define   KEY_BOOST     19
#define   KEY_TIMER     20

#define   KEY_LONG      32

#define seg_a  7
#define seg_b 6
#define seg_c 5
#define seg_d 0
#define seg_e 1
#define seg_f 2
#define seg_g 3
#define seg_db  4

#define KEY_BITS(x)  (1 << (x - 1))
#define KEY_TIME_MS(x)  (x / 30)
#define dbit(x)  ((uint8_t)1<<x)

typedef enum
{
    I6S02B = 1,
    I9S02B = 2
} DeviceType_t;

typedef enum
{
    ZONE_1 = 0,
    ZONE_2,
    ZONE_3,
    ZONE_4,
    ZONE_5,
    ZONE_6,
    ZONE_7,
    ZONE_8,
} Zone_t;

typedef enum
{
    LEVEL_0 = 0,
    LEVEL_1,
    LEVEL_2,
    LEVEL_3,
    LEVEL_4,
    LEVEL_5,
    LEVEL_6,
    LEVEL_7,
    LEVEL_8,
    LEVEL_9,
    LEVEL_B,
    LEVEL_DB
} Level_t;

typedef enum
{
    ZONE_NO_ERROR = 0x00,
    ZONE_ERROR_F1 = 0x01, //  Power driver Data Communication fail
    ZONE_ERROR_E3 = 0x02, //  High voltahe
    ZONE_ERROR_E4 = 0x04, //  Low voltage
    ZONE_ERROR_E2 = 0x08, //  IGBT overheat
    ZONE_ERROR_E1 = 0x10, //  Plate Extreme overheat
    ZONE_ERROR_EE = 0x20  //  NTC Communication fail
} Iot_ZoneErrors_t;

typedef enum
{
    DEVICE_LOST = 0,
    DEVICE_OFF,
    DEVICE_ON,
    DEVICE_LOCK,
    DEVICE_PAUSE,
    UNSUPPORTED_DEVICE,
    UNSUPPORTED_API
} Iot_DeviceStatus_t;

typedef enum
{
    IOT_SUCCES = 0,
    IOT_FAIL
} Iot_Status_t;

const uint8_t Display_Char_Table[] =
{
    (dbit(seg_a) | dbit(seg_b) | dbit(seg_c) | dbit(seg_d) | dbit(seg_e) | dbit(seg_f)), // 0
    (dbit(seg_b) | dbit(seg_c)), // 1
    (dbit(seg_a) | dbit(seg_b) | dbit(seg_d) | dbit(seg_e) | dbit(seg_g)), // 2
    (dbit(seg_a) | dbit(seg_b) | dbit(seg_c) | dbit(seg_d) | dbit(seg_g)), // 3
    (dbit(seg_b) | dbit(seg_c) | dbit(seg_f) | dbit(seg_g)), // 4
    (dbit(seg_a) | dbit(seg_c) | dbit(seg_d) | dbit(seg_f) | dbit(seg_g)), // 5
    (dbit(seg_a) | dbit(seg_c) | dbit(seg_d) | dbit(seg_e) | dbit(seg_f) | dbit(seg_g)), // 6
    (dbit(seg_a) | dbit(seg_b) | dbit(seg_c) | dbit(seg_f)), // 7
    (dbit(seg_a) | dbit(seg_b) | dbit(seg_c) | dbit(seg_d) | dbit(seg_e) | dbit(seg_f) | dbit(seg_g)), // 8
    (dbit(seg_a) | dbit(seg_b) | dbit(seg_c) | dbit(seg_d) | dbit(seg_f) | dbit(seg_g)), // 9
    (dbit(seg_a) | dbit(seg_b) | dbit(seg_c) | dbit(seg_e) | dbit(seg_f) | dbit(seg_g)), // A
    (dbit(seg_c) | dbit(seg_d) | dbit(seg_e) | dbit(seg_f) | dbit(seg_g)), // B
    (dbit(seg_a) | dbit(seg_d) | dbit(seg_e) | dbit(seg_f)), // C
    (dbit(seg_b) | dbit(seg_c) | dbit(seg_d) | dbit(seg_e) | dbit(seg_g)), // D
    (dbit(seg_a) | dbit(seg_d) | dbit(seg_e) | dbit(seg_f) | dbit(seg_g)), // E
    (dbit(seg_a) | dbit(seg_e) | dbit(seg_f) | dbit(seg_g)), // F
    (dbit(seg_g)),// -
    (dbit(seg_d) | dbit(seg_g)), // =
    (dbit(seg_d) | dbit(seg_e) | dbit(seg_f)), // L
    (dbit(seg_c) | dbit(seg_d) | dbit(seg_e) | dbit(seg_g)), // o
    (dbit(seg_a) | dbit(seg_b) | dbit(seg_e) | dbit(seg_f) | dbit(seg_g)), // P
    (dbit(seg_e) | dbit(seg_g)), // R
    (dbit(seg_b) | dbit(seg_d) | dbit(seg_f) | dbit(seg_g)), // U
    (dbit(seg_d)),// _
    (dbit(seg_b) | dbit(seg_c) | dbit(seg_e) | dbit(seg_f) | dbit(seg_g)), // H
    (dbit(seg_a) | dbit(seg_b) | dbit(seg_c) | dbit(seg_e) | dbit(seg_f)), // Bridge
    (dbit(seg_c) | dbit(seg_d) | dbit(seg_e)), // WARM u
    (dbit(seg_a) | dbit(seg_d) | dbit(seg_g)), // = _DUAL
    0  // ' ' off
};

typedef enum
{
    CHAR_0 = 0,
    CHAR_1,
    CHAR_2,
    CHAR_3,
    CHAR_4,
    CHAR_5,
    CHAR_6,
    CHAR_7,
    CHAR_8,
    CHAR_9,
    CHAR_A,
    CHAR_B,
    CHAR_C,
    CHAR_D,
    CHAR_E,
    CHAR_F,
    CHAR_MINUS,
    CHAR_EQUAL,
    CHAR_L,
    CHAR_o,
    CHAR_P,
    CHAR_R,
    CHAR_U,
    CHAR__,
    CHAR_H,
    CHAR_BRIDGE,
    CHAR_WARM,
    CHAR_DUAL,
    CHAR_NULL
} Character_t;

typedef struct
{
    uint8_t api_version;
    uint8_t device_type;
    uint8_t display[NUMBER_OF_ZONE + 2];
    uint32_t keyfeedback;
    uint16_t crc16;
} ReceiveBuffer_t;

class Iot_Com
{
private:
    uint8_t i2c_addr;
    uint8_t RxCount;
    bool key_release = false;
    uint8_t KeyListStart = 0;
    uint8_t KeyListEnd = 0;
    uint32_t KeyListBuffer[KEY_LIST_SIZE];
    uint16_t crc16;
    uint8_t api_version;
    DeviceType_t device_type;
    char ZoneChar[NUMBER_OF_ZONE];
    char TimeZoneChar[2];
    Level_t ZoneLevel[NUMBER_OF_ZONE];
    ReceiveBuffer_t TempBuffer;
    ReceiveBuffer_t ReceivedBuffer;
    Iot_DeviceStatus_t device_status;
    Iot_Status_t key_procces();
    uint8_t key_count();
    bool key_get(uint32_t *key);
    void Crc16_Calc_Byte(uint8_t byte);
    Iot_Status_t key_pop();
    Iot_Status_t key_push(uint32_t key);
    Iot_Status_t key_send(uint32_t keys);
    Character_t CharacterCheck(uint8_t byte);
    char Asci_Contert_Of_Char(Character_t character);
    void display_procces();
    void character_handler();
public:
    Iot_Com(uint8_t addr, uint16_t pin_sda, uint16_t pin_clk);
    void init();
    void procces();
    Iot_ZoneErrors_t get_zone_error(Zone_t zone);//not yet!
    Iot_DeviceStatus_t get_device_status();
    Iot_Status_t power_on();
    Iot_Status_t power_off();
    Iot_Status_t set_level(Zone_t zone, Level_t level);
    Iot_Status_t get_level(Zone_t zone, Level_t *level);
    uint8_t get_api_version();
    DeviceType_t get_device_type();
    char get_zone_display(Zone_t zone);
    char get_timer_display_high();
    char get_timer_display_low();
};
#endif
