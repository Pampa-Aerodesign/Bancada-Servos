#include "datalogger.hpp"

void Datalogger::begin(char header_[64])
{
    startTime = 0;           // Set the start time to 0 on the initial configuration.
    strcpy(header, header_); // Copy the header from the input to the internal variable.
}

void Datalogger::send(float current, float voltage)
{
    // Verify if the start time is zero.
    if (!startTime)
    {
        Serial.println("header"); // Print the word header so the python logger knows that the next line is the actual header.
        Serial.println(header);   // Send the header.

        startTime = millis(); // Store the current time from the millis() function.
    }

    // In case of the start time is not zero, send the lines of data to the logger.
    else
    {
        Serial.print(millis() - startTime); // First element is the difference between the current time and the start time.
        Serial.print(",");                  // Print a comma to separate the information on the lines.
        Serial.print(current);              // Print the current.
        Serial.print(",");                  // Another separator.
        Serial.println(voltage);            // Print the voltage and the line ending.
    }
}
