// == Controlador da bancada de teste de Torque x Corrente de servos ==

/* 
Este programa faz a medição em tempo real da corrente consumida por um servo
e exibe a leitura num display LCD 16x2.
A posição do servo pode ser controlada por um potenciômetro.
Para obter uma média de corrente com um maior número de amostras, basta
pressionar o botão HOLD. O resultado obtido será escrito no display até
que o botão seja pressionado novamente.
*/

// Libraries
#include "config.hpp"
#include <Servo.h>
#include <LiquidCrystal.h>
#include "ACS712.h"
#include "sampling.hpp"
#include "display.hpp"
#include "calibration.hpp"

// Create Servo object to control the servo.
Servo servo;

// Setting up the ACS712 current sensor
// (Analog pin, Voltage, ADC resolution, mV/A)
ACS712 ACS(AMPPIN, 5.0, 1023, 185);	

// LCD Display
LiquidCrystal lcd(LCDPINS);

// Global variables
bool hold = 0;					// HOLD flag (false = running; true = holding)
bool hldstate = 0;			// HOLD state
bool lasthldbtn = 0;		// Previous reading from HOLD button
uint64_t lasthldt = 0;	// Last time the HOLD flag was toggled
bool manual_calib_flag = 0; // Flag indicating if the button has been manually calibrated recently

bool mampflag = 0;			// Flag when current sensor reading is done
float avg;							// Current average
int64_t total = 0;			// Sum of all samples
float manual_calib = 0;


void setup() {
	// Initialize serial monitor
	Serial.begin(BAUD);
	Serial.println("Initializing...");

	// Initialize display (columns, rows)
	lcd.begin(16, 2);
	lcd.print("Initializing   ");

	//Set HOLD button to input
	pinMode(HOLDPIN, INPUT);
	
	// Run ACS calibration
	calibACS(ACS, lcd, servo);

	// Clear display
	lcd.clear();

	Serial.println("Initialization complete.");
}

void loop() {
	// Variables
	bool holdbtn;				// HOLD button reading
	uint16_t potval;		// Potentiometer reading
	float vin;						// Servo voltage divider input
	float volt;					// Servo voltage

	// Read, calculate and print servo voltage
	vin = analogRead(VOLTPIN);
	volt = (vin/1023) * 5 * (32.0/12.0); // divisions in C need to be done with floats (and not integers) for result to be a float
	lcd.setCursor(11,0);
	lcd.print(volt, 2);
	lcd.print("V");
  
	// Reading HOLD button
	holdbtn = digitalRead(HOLDPIN);

	// Debouncing and toggling HOLD state
	if (holdbtn != lasthldbtn) {
    // reset the debouncing timer
    lasthldt = millis();
  }
	if ((millis() - lasthldt) > DEBOUNCE) {
    // whatever the reading is at, it's been there for longer than the
    // debounce delay, so take it as the actual current state

    // if the button state has changed
    if (holdbtn != hldstate) {
      hldstate = holdbtn;

      /*if(hldstate == HIGH) {
        // when the button starts being pressed
      }
			else*/ if(hldstate == LOW){
        // when the button stops being pressed
				if(manual_calib_flag == 0) {
          hold = !hold;
				}
        else manual_calib_flag = 0;
			}
    }
  }
  if(hldstate == HIGH) {
    // when the button is being pressed
    if (millis() - lasthldt > MANUAL_CALIB_DELAY*1000 && manual_calib_flag == 0) {
      manual_calib = manualCalibration(ACS, lcd);
      manual_calib_flag = 1;
    }
  }

  
	// Save button reading for the next loop
	lasthldbtn = holdbtn;

	// If HOLD is off, take readings in real time with fewer samples
	if(!hold){
		// Get reading from sensor
		avg = getma(ACS, DSAMPLES) - manual_calib;

		// Print reading to display
		printReading(lcd, avg, hold);

		// Reset reading done flag 
		mampflag = false;
	}

	// If HOLD is on, take one reading with more samples
	else{
		if(!mampflag){
			// Print tare message on display
			lcd.setCursor(0,0);
			lcd.print("Reading... ");
			Serial.println("Reading...");

			// Get reading from sensor
			avg = getma(ACS, SAMPLES) - manual_calib;

			// Print reading to display
			printReading(lcd, avg, hold);

			// Set reading done flag 
			mampflag = true;
		}

		delay(DELAY);
	}
	
	// Rotate servo according to potentiometer position
	potval = analogRead(POTPIN);						// Read the potentiometer
	potval = map(potval, 0, 1023, 0, 180);	// Map pot between 0 to 180 degrees to use with the servo
	servo.write(potval);										// Set servo position

	// Printing to serial monitor/plotter
	Serial.print(avg);
	Serial.print('\t');
	Serial.print(hold);
	Serial.print('\t');
	Serial.print(vin);
	Serial.print('\t');
	Serial.print(volt, 2);
	Serial.print('\t');
	Serial.println(potval);

	//delay(DELAY);	// Slow down to make reading easier
}
