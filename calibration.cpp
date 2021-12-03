#include "calibration.hpp"

void calibrationStart(Servo *pServo, uint16_t servoCenter, Adafruit_INA219 *pIna219)
{
    pServo->write(servoCenter);       // Moves the servo to the center.
    delay(5000);                      // Wait fot the servo go to the center and disconect the servo if needed.
    pServo->detach();                 // Detach the servo.
    pIna219->setCalibration_32V_2A(); // Reset the calibration configuration of the INA219.
}

void calibrationEnd(Servo *pServo, uint8_t servoPin)
{
    pServo->attach(servoPin); // Ataches the servo back to its pin.
}
