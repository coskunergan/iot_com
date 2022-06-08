/*
    Examples

    Created on: May 17, 2022

    Author: Coskun ERGAN
*/

#include "iot_com.h"

#ifdef ESP32
#define LED_BUILTIN  3
#define D1 1
#define D2 2
#endif

long lastTime = 0;
long debugTime = 0;
bool state = false;

extern uint32_t KeyFeedback;
extern uint8_t DisplayBuffer[];
extern uint32_t SendKey;

Iot_Com Iot_Com(0x60, D1, D2);

#define DELAY_PERIOD 10000
#define DEBUG_PERIOD 100

/*****************************************************************/
void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
    Serial.println("Restart!");
}
/*****************************************************************/
void loop()
{
    //-----------------------
    Iot_Com.procces();
    //-----------------------
    if((millis() - lastTime) > DELAY_PERIOD)
    {
        lastTime = millis();

        if(state == false)
        {
            state = true;
            Iot_Com.power_on();
            Iot_Com.set_level(ZONE_1, LEVEL_2);
            Iot_Com.set_level(ZONE_2, LEVEL_5);
            Iot_Com.set_level(ZONE_3, LEVEL_8);
            Iot_Com.set_level(ZONE_4, LEVEL_P);
            digitalWrite(LED_BUILTIN, HIGH);
            Serial.println("Sent Power On!");
        }
        else
        {
            state = false;
            Iot_Com.power_off();
            digitalWrite(LED_BUILTIN, LOW);
            Serial.println("Sent Power Off!");
        }
    }
    //-----------------------
    if((millis() - debugTime) > DEBUG_PERIOD)
    {
        debugTime = millis();
        Serial.printf("API:V%d DEV:%d (%X)(%X)(%X)(%X)(%X)(%X)\r\n", Iot_Com.api_version, Iot_Com.device_type, DisplayBuffer[0], DisplayBuffer[1], DisplayBuffer[2], DisplayBuffer[3], DisplayBuffer[4], DisplayBuffer[5]);
        Serial.printf("Count:%d FB: %X Send: %X \r\n", Iot_Com.get_command_status(), KeyFeedback, SendKey);
    }
}
/*****************************************************************/
/*****************************************************************/
/*****************************************************************/
