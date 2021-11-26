// Inclusion of the libraries installed in the Arduino standard directory.
#include <Adafruit_INA219.h>
#include <LiquidCrystal.h>
#include <Servo.h>

// Inclusion of the libraries installed in the folder of the program.
#include "calibration.hpp"
#include "datalogger.hpp"
#include "info.hpp"
#include "sampling.hpp"
#include "thebutton.hpp"
#include "parameters.hpp"

// Declaration of the objects used in the program.
Adafruit_INA219 ina219(ADDRESS); // Current and voltage sensor.

LiquidCrystal lcd(LCDPINS); // Liquid crystal display.

Servo servo; // Servomotor from which the current and voltage will be measured.

Datalogger datalogger; // Responsible for sending data through serial.

Info info; // Responsible for driving the LCD display and showing info on serial monitor.

Sampling sampling; // Responsible for storing and counting the samples.

TheButton button(HOLDPIN); // Responsible for driving the button used to control the system.

// Global Variable.
uint8_t mode = 0; // Responsible for storing the current mode in the system.
/*
	Mode Table:
	[0] Running.
	[1] Reading.
	[2] Holding.
	[3] Calibrating.
	[4] Datalogging.
*/

// Arduino Setup (Code that run when the program gets restarted).
void setup()
{
	Serial.begin(BAUD); // Begins the Serial communication.

	ina219.begin();					// Begins the communication with the INA219 board.
	ina219.setCalibration_32V_2A(); // Sets up the calibration for the INA219.

	lcd.begin(16, 2); // Begins the LCD object with the correct number of rows and columns.

	servo.attach(SERVOPIN); // Begins the servomotor object in the selected pin.

	info.begin(DISPLAY_DELAY * 1000); // Begins the info object setting up the update delay.

	sampling.begin(DSAMPLES); // Begins the sampling object with the number of samples.

	button.setDebounce(DEBOUNCE);				   // Sets up the debounce time for the button.
	button.setLongTime(MANUAL_CALIB_DELAY * 1000); // Sets up the long press time for the button.
	button.setReturnTime(RETURN);				   // Sets up the time the button will take to make the state available.
}

void loop()
{
	button.tick(); // Check for changes in the button

	// Verifies if the button was clicked in the last button tick.
	if (button.wasClicked())
	{
		// If clicked and the mode is "Running", the mode changes to "Reading".
		if (!mode)
		{
			mode = 1;					  // Mode 1 is Reading.
			sampling.changeSize(SAMPLES); // Changes the size of the samples to the bigger one and reset the counter.
		}

		// If clicked and the mode is other than "Running", it will be set to "Running".
		else
		{
			mode = 0;					   // Mode 0 is Running.
			sampling.changeSize(DSAMPLES); // Changes the size of the samples to the smaller one and reset the counter.
		}
	}

	// Verifies if the button was long pressed.
	else if (button.wasLongPressed())
	{
		// If long pressed, sets the mode to "Calibrating".
		mode = 3;									 // Mode 3 is Calibrating.
		sampling.changeSize(SAMPLES);				 // Changes the size of the samples to the bigger one and reset the counter.
		calibrationStart(&servo, SERVOCTR, &ina219); // Start the calibration process.
	}

	// Verifies if the button was double clicked.
	else if (button.wasDoubleClicked())
	{
		// If double clickd, sets the mode to "Dataloggig".
		mode = 4;							 // Mode 4 is Calibrating.
		sampling.changeSize(DSAMPLES);		 // Changes the size of the samples to the smaller one and reset the counter.
		datalogger.begin(DATALOGGER_HEADER); // Begins the datalogging proccess by sending the header to the object.
	}

	// If the mode is not "Holding", collect a sample from INA219 board.
	if (mode != 2)
	{
		sampling.addToAvg(ina219.getCurrent_mA(), ina219.getBusVoltage_V()); // Sample current and voltage.
	}

	// If the sampling counter is zero (sampling is ready) do an action depending on the mode.
	if (!sampling.smpCnt)
	{
		// If the mode is "Reading", will be set to "Holding".
		if (mode == 1)
		{
			mode = 2; // Mode 2 is "Holding".
		}

		// If the mode is "Calibrating", will be set back "Running", and the sampling will be reset.
		else if (mode == 3)
		{
			mode = 0; // Mode 0 is "Running".
			sampling.avgIsOffset(); // Set the current average value as the offset.
			sampling.changeSize(DSAMPLES); // Changes the size of the samples to the smaller one and reset the counter.
			calibrationEnd(&servo, SERVOPIN, &ina219); // End the calibration process.
		}

		// If the mode is "Datalogging", send the data through serial with a specific formar.
		else if (mode == 4)
		{
			datalogger.send(sampling.avgCurrent, sampling.avgVoltage); // Send the data through serial.
		}
	}

	servo.write(map(analogRead(POTPIN), 1023, 0, 0, 180)); // Update the servomotor position depending on the potentiometer position.

	info.update(sampling.avgCurrent, sampling.avgVoltage, mode, &lcd); // Update the info shown in the serial monitor and LCD.
}
