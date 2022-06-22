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

Iot_Com Iot_Com(0x60, D1, D2);

#define DELAY_PERIOD 1000
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

        for(uint8_t zone = 0; zone < IOT_NUMBER_OF_ZONE; zone++)
        {
            Serial.printf("Zone_%d:", zone);
            switch(Iot_Com.get_zone_error((Zone_t)zone))
            {
                case 0x01:
                    Serial.println("Device driver comminication error!");
                    break;
                case 0x02:
                    Serial.println("Plate extreme overheat error!");
                    break;
                case 0x04:
                    Serial.println("IGBT overheat error!");
                    break;
                case 0x08:
                    Serial.println("High input voltage!");
                    break;
                case 0x10:
                    Serial.println("Low input voltage!");
                    break;
                case 0x20:
                    Serial.println("Water contact the touch plate!");
                    break;
                default:
                    Serial.println("No error.");
                    break;
            }
        }
        Serial.println();
    }
//-----------------------
    if((millis() - debugTime) > DEBUG_PERIOD)
    {
        debugTime = millis();
        // Serial.printf("API:V%d DEV:%d (%d)(%d)(%d)(%d)\r\n", Iot_Com.get_api_version(), Iot_Com.get_device_type(),
        //               Iot_Com.get_zone_level(ZONE_1),
        //               Iot_Com.get_zone_level(ZONE_2),
        //               Iot_Com.get_zone_level(ZONE_3),
        //               Iot_Com.get_zone_level(ZONE_4));
    }
}
/*****************************************************************/
/*****************************************************************/
/*****************************************************************/