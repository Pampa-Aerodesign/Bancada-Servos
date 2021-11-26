#ifndef _DATALOGGER_H_
#define _DATALOGGER_H_

#include "Arduino.h"
#include "stdint.h"
#include "string.h"

class Datalogger
{
private:
    uint32_t startTime;
    char header[64];

public:
    void begin(char header_[64]);
    void send(float current, float voltage);
};

#endif
