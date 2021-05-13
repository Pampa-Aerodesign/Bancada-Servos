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
#include <Servo.h>
#include "ACS712.h"

// Configuration
#define BAUD 9600				// Baud rate
#define DSAMPLES 200		// Number of samples to capture for calculating "real time" average
#define SAMPLES 5000		// Number of samples to capture for calculating average on button press
#define DELAY	100				// Small delay to slow things down
#define SERVOCTR 90			// Servo center position
#define DEBOUNCE 50			// Debounce delay

// Pinouts
#define POTPIN 0				// Analog input pin for potentiometer
#define AMPPIN 2				// Analog input pin for current sensor
#define HOLDPIN 4				// Digital input pin for HOLD button
#define SERVOPIN 9			// Digital output pin for servo PWM

Servo servo;						// Create Servo object to control the servo.

ACS712  ACS(AMPPIN, 5.0, 1023, 185);	// Setting up the ACS712 module (current sensor)
// (Analog pin, Voltage, ADC resolution, mV/A)

// Global variables
bool hold = 0;					// HOLD flag (false = running; true = holding)
bool hldstate = 0;			// HOLD state
bool lasthldbtn = 0;		// Previous reading from HOLD button
uint64_t lasthldt = 0;	// Last time the HOLD flag was toggled

bool mampflag = 0;			// Flag when current sensor reading is done
float mamp;							// Current reading in miliamps
float avg;							// Current average
int64_t total = 0;			// Sum of all samples


void setup() {
	Serial.begin(BAUD);				// Initialize serial monitor
	//Serial.println("Initializing...");

	//Set HOLD button to input
	pinMode(HOLDPIN, INPUT);

	// Move servo to center position and detach it to calibrate current sensor
	//Serial.println("Calibrating");
	servo.attach(SERVOPIN);		// Attach servo to output pin
	servo.write(SERVOCTR);		// Move servo to center
	delay(1000);							// Wait for servo to get in position
	servo.detach();						// Detach servo to cut power

	ACS.autoMidPoint();				// Run auto calibration
	delay(100);
	//Serial.print("Midpoint set to ");
	//Serial.println(ACS.getMidPoint());

	servo.attach(SERVOPIN);		// Attach servo to output pin
	//Serial.println("Calibration complete.");
}

void loop() {
	// Variables
	int holdbtn;						// HOLD button reading
	uint16_t potval;				// Potentiometer reading
  
	// Reading HOLD button
	holdbtn = digitalRead(HOLDPIN);

	if (holdbtn != lasthldbtn) {
    // reset the debouncing timer
    lasthldt = millis();
  }

	if ((millis() - lasthldt) > DEBOUNCE) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (holdbtn != hldstate) {
      hldstate = holdbtn;

			if(hldstate == HIGH){
				hold = !hold;
			}
    }
  }
	
	// Save button reading
	lasthldbtn = holdbtn;


	// If HOLD is disabled, take readings in real time with fewer samples
	if(!hold){
		// Reset total and avg variables for next reading
		avg = 0;
		total = 0;

		// Reset reading done flag
		mampflag = false;

		// Capturing all samples
		for(int i = 0; i < DSAMPLES; i++){
			mamp = ACS.mA_DC();		// Get data from sensor (in miliamps)
			total += mamp;				// Add current reading to total
		}

		// Divides sum of all readings by the number of samples (average)
		avg = total / DSAMPLES;	
	}

	// If HOLD is enabled, show average with more samples and don't update it
	else{
		// Check if the average was already taken
		if(!mampflag){
			// Reset total and avg variables for next reading
			avg = 0;
			total = 0;

			// Capturing all samples
			for(int i = 0; i < SAMPLES; i++){
				mamp = ACS.mA_DC();		// Get data from sensor (in miliamps)
				total += mamp;				// Add current reading to total
			}

			// Divides sum of all readings by the number of samples (average)
			avg = total / SAMPLES;

			mampflag = true;	// Indicate that the reading is done
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
	Serial.println(potval);

	//delay(DELAY);	// Slow down to make reading easier
}