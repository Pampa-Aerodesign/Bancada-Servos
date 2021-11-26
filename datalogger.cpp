#include "datalogger.hpp"

void Datalogger::begin(char header_[64])
{
    startTime = 0;
    strcpy(header, header_);
}

void Datalogger::send(float current, float voltage)
{
    if (!startTime)
    {
        Serial.println("header");
        Serial.println(header);

        startTime = millis();
    }

    else
    {
        Serial.print(millis() - startTime);
        Serial.print(",");
        Serial.print(current);
        Serial.print(",");
        Serial.println(voltage);
    }
}
