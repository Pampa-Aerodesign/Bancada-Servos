// Configuration file for pinouts

// Settings
#define BAUD 9600				// Baud rate
#define DSAMPLES 200		// Number of samples to capture for calculating "real time" average
#define SAMPLES 5000		// Number of samples to capture for calculating average on button press
#define DELAY	100				// Small delay to slow things down
#define SERVOCTR 90			// Servo center position
#define DEBOUNCE 50			// Debounce delay

// Pinouts
#define POTPIN 0				// Analog input pin for servo potentiometer
#define AMPPIN 2				// Analog input pin for current sensor
#define VOLTPIN 7				// Analog input pin for voltage sensor
#define HOLDPIN 8				// Digital input pin for HOLD button
#define SERVOPIN 9			// Digital output pin for servo PWM

// LCD Pinout (RS, EN, D4, D5, D6, D7)
#define LCDPINS 12, 11, 5, 4, 3, 2