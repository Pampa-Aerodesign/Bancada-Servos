#include "calibration.hpp"

void calibrationStart(Servo *pServo, uint16_t servoCenter, Adafruit_INA219 *pIna219)
{
    pServo->write(servoCenter);
    delay(5000);
    pServo->detach();
    pIna219->setCalibration_32V_2A();
}

void calibrationEnd(Servo *pServo, uint8_t servoPin, Sampling *pSampling, uint16_t smpSize, Adafruit_INA219 *pIna219)
{
    pServo->attach(servoPin);
}
