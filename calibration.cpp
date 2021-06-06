// This file contains functions related to calibration of sensors

#include "calibration.hpp"
#include "sampling.hpp"
#include "config.hpp"
#include <LiquidCrystal.h>
#include <ACS712.h>
#include <Servo.h>

// Calibrating ACS712 module
void calibACS(ACS712 ACS, LiquidCrystal lcd, Servo servo){
	// Print debug messages on display and serial monitor
	Serial.println("Calibrating...");
	lcd.setCursor(0,0);
	lcd.print("Calibrating... ");

	// Move servo to center position and detach it to calibrate current sensor
	servo.attach(SERVOPIN);		// Attach servo to output pin
	servo.write(SERVOCTR);		// Move servo to center
	delay(1000);							// Wait for servo to get in position
	servo.detach();						// Detach servo to cut power

	ACS.autoMidPoint();				// Run auto calibration
	delay(1000);

	// Get midpoint
	uint16_t midpoint = ACS.getMidPoint();

	servo.attach(SERVOPIN);		// Attach servo to output pin

	// Print debug message on display
	lcd.setCursor(0,0);				// Return cursor 
	lcd.print("Complete       ");
	lcd.setCursor(0,1);				// Set cursor to next line
	lcd.print("Midpoint: ");
	lcd.print(midpoint);

	// Print debug messages to serial monitor
	Serial.print("Midpoint set to ");
	Serial.println(midpoint);
	delay(1000);
}

float manualCalibration(ACS712 ACS, LiquidCrystal lcd) {
  lcd.setCursor(0,0);
  lcd.print("Manual          ");
  lcd.setCursor(0,1);
  lcd.print("calibration...  ");
  delay(1000);
  float manual_calib = getma(ACS, SAMPLES);
  lcd.clear();
  lcd.print("Value: ");
  lcd.print(manual_calib);
  lcd.print("mA");
  delay(2000);
  return manual_calib;
}
