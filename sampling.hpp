#ifndef _SAMPLING_H_
#define _SAMPLING_H_

#include "stdint.h"

class Sampling
{
private:
    float offsetCurrent;
    double totalCurrent;
    double totalVoltage;
    uint16_t smpSize;

public:
    float avgCurrent;
    float avgVoltage;
    uint16_t smpCnt;

    void begin(uint16_t smpSize_);

    void addToAvg(float valueCurrent, float valueVoltage);

    void avgIsOffset();

    void changeSize(uint16_t newSize_);
};

#endif
