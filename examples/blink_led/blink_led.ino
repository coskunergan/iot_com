/*
    Examples

    Created on: May 12, 2022

    Author: Coskun ERGAN
*/

#ifdef ESP32
#define LED_PIN  3
#elif defined(ESP8266)
#endif

int LedState = 0;
long lastTime = 0;

#define DELAY_PERIOD 1000

/*****************************************************************/
void setup()
{
    pinMode(LED_PIN, OUTPUT);
    Serial.begin(115200);
    Serial.println("Restart!");
}
/*****************************************************************/
void loop()
{
    if(LedState)
    {
        digitalWrite(LED_PIN, LOW);
    }
    else
    {
        digitalWrite(LED_PIN, HIGH);
    }

    if((millis() - lastTime) > DELAY_PERIOD)
    {
        LedState = !LedState;
        lastTime = millis();
    }
}
/*****************************************************************/
/*****************************************************************/
/*****************************************************************/
