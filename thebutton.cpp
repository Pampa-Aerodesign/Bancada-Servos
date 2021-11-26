#include "thebutton.hpp"

TheButton::TheButton(uint8_t pin_)
{
    pin = pin_;

    debounce = 50;
    returnTime = 250;
    longTime = 2000;

    lastState = 0;
    machineState = 0;

    readTimer = 0;
    returnTimer = 0;

    pinMode(pin, INPUT_PULLUP);
}

void TheButton::setDebounce(uint8_t debounce_)
{
    debounce = debounce_;
}

void TheButton::setLongTime(uint16_t longTime_)
{
    longTime = longTime_;
}

void TheButton::setReturnTime(uint16_t returnTime_)
{
    returnTime = returnTime_;
}

void TheButton::tick()
{
    currentState = !digitalRead(pin);

    if (currentState && !lastState)
    {
        readTimer = millis();
    }

    else if (!currentState && lock)
    {
        lock = 0;
    }

    else if (!currentState && lastState && millis() - readTimer >= debounce && machineState < 2)
    {
        returnTimer = millis();
        machineState += 1;
    }

    else if (currentState && millis() - readTimer >= longTime && !lock)
    {
        machineState = 3;
    }

    lastState = currentState;
}

bool TheButton::wasClicked()
{
    if (millis() - returnTimer >= returnTime && machineState == 1)
    {
        delayMicroseconds(10);
        bool answer = machineState == 1;
        machineState = 0;
        return answer;
    }

    else
    {
        return 0;
    }
}

bool TheButton::wasDoubleClicked()
{
    if (millis() - returnTimer >= returnTime && machineState == 2)
    {
        delayMicroseconds(10);
        bool answer = machineState == 2;
        machineState = 0;
        return answer;
    }

    else
    {
        return 0;
    }
}

bool TheButton::wasLongPressed()
{
    if (millis() - returnTimer >= returnTime && machineState == 3)
    {
        delayMicroseconds(10);
        bool answer = machineState == 3;
        machineState = 0;
        lock = 1;
        return answer;
    }

    else
    {
        return 0;
    }
}
