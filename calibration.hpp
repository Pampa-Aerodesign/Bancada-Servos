#ifndef _CALIBRATION_H_
#define _CALIBRATION_H_

#include "Arduino.h"
#include "Servo.h"
#include "stdint.h"
#include "Adafruit_INA219.h"

#include "sampling.hpp"

void calibrationStart(Servo *pServo, uint16_t servoCenter, Adafruit_INA219 *pIna219);
void calibrationEnd(Servo *pServo, uint8_t servoPin, Adafruit_INA219 *pIna219);

#endif
