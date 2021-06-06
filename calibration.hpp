// This is the header file for calibration.cpp

#ifndef _CALIBRATION_
#define _CALIBRATION_

#include "config.hpp"
#include "calibration.hpp"
#include <LiquidCrystal.h>
#include <ACS712.h>
#include <Servo.h>

// Calibrating ACS712 module
void calibACS(ACS712, LiquidCrystal, Servo);
// Manual calibration by current value
float manualCalibration(ACS712 ACS, LiquidCrystal lcd);

#endif
