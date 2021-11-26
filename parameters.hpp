// Configuration file for pinouts

// Settings
#define ADDRESS 0x41         // INA219 I²C address
#define BAUD 9600            // Baud rate
#define DSAMPLES 100         // Number of samples to capture for calculating "real time" average
#define SAMPLES 10000        // Number of samples to capture for calculating average on button press
#define SERVOCTR 90          // Servo center position
#define DEBOUNCE 50          // Debounce delay
#define RETURN 250           // Return delay
#define MANUAL_CALIB_DELAY 2 // Seconds the button needs to be pressed to enter manual calibration
#define DISPLAY_DELAY 1      // Delay for displaying information

// Pinouts
#define POTPIN 0   // Analog input pin for servo potentiometer
#define HOLDPIN 8  // Digital input pin for HOLD button
#define SERVOPIN 9 // Digital output pin for servo PWM

// LCD Pinout (RS, EN, D4, D5, D6, D7)
#define LCDPINS 11, 12, 2, 3, 4, 5

//Header for the datalogger
#define DATALOGGER_HEADER "Time (ms),Current (mA),Voltage (V)"
