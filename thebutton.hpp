#ifndef _THEBUTTON_H_
#define _THEBUTTON_H_

#include "Arduino.h"
#include "stdint.h"

class TheButton{
    private:
        bool currentState;
        bool lastState;

        bool lock;

        uint8_t machineState;
        uint8_t pin;

        uint8_t debounce;
        uint16_t longTime;
        uint16_t returnTime;

        uint32_t readTimer;
        uint32_t returnTimer;

    public:
        TheButton(uint8_t pin_);

        void setDebounce(uint8_t debounce_);
        void setLongTime(uint16_t longTime_);
        void setReturnTime(uint16_t doubleTime_);

        void tick();

        bool wasLongPressed();
        bool wasClicked();
        bool wasDoubleClicked();
		
};

#endif
