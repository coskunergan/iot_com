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

        digitalWrite(LED_BUILTIN, LOW);

        switch(Iot_Com.get_device_status())
        {
            case DEVICE_LOST:
                Serial.println("Device not connected!");
                break;
            case DEVICE_OFF:
                Serial.println("Device OFF");
                break;
            case DEVICE_ON:
                Serial.println("Device ON");
                digitalWrite(LED_BUILTIN, HIGH);
                break;
            case DEVICE_PAUSE:
                Serial.println("Device PAUSE");
                break;
            case UNSUPPORTED_API:
                Serial.println("Device has a unsupported API vesion!");
                break;
            case UNSUPPORTED_DEVICE:
                Serial.println("Device has a unsupported Type!");
                break;
            default:
                break;
        }
    }
//-----------------------
    if((millis() - debugTime) > DEBUG_PERIOD)
    {
        debugTime = millis();
        Serial.printf("API:V%d DEV:%d (%d)(%d)(%d)(%d)\r\n", Iot_Com.get_api_version(), Iot_Com.get_device_type(),
                      Iot_Com.get_zone_level(ZONE_1),
                      Iot_Com.get_zone_level(ZONE_2),
                      Iot_Com.get_zone_level(ZONE_3),
                      Iot_Com.get_zone_level(ZONE_4));
    }
}
/*****************************************************************/
/*****************************************************************/
/*****************************************************************/
