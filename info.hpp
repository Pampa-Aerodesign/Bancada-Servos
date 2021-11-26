#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "Arduino.h"
#include "stdint.h"
#include "LiquidCrystal.h"

class Info
{
private:
    uint8_t lastMode;

    uint32_t tmr;
    uint16_t updateDelay;

    void show(float current, float voltage, LiquidCrystal *lcd);

public:
    void begin(uint16_t updateDelay_);
    void update(float current, float voltage, uint8_t mode, LiquidCrystal *lcd);
};

#endif
