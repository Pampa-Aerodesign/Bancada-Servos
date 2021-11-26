#include "info.hpp"

void Info::begin(uint16_t updateDelay_)
{
	updateDelay = updateDelay_;
	lastMode = 0;
}

void Info::show(float current, float voltage, LiquidCrystal *lcd)
{
	Serial.print("Current: ");
	Serial.print(current);
	Serial.println("mA");
	Serial.print("Voltage: ");
	Serial.print(voltage);
	Serial.println("V");

	lcd->setCursor(9, 0);
	lcd->print("V=");
	lcd->print(voltage, 2);
	lcd->print("V");

	lcd->setCursor(0, 1);
	lcd->print("I=");
	lcd->print(current, 1);
	lcd->print("mA");
}

void Info::update(float current, float voltage, uint8_t mode, LiquidCrystal *lcd)
{
	if (millis() - tmr > updateDelay)
	{
		tmr = millis();
		
		if (mode && mode == lastMode)
		{
			return;
		}

		else
		{
			lastMode = mode;
		}
	}

	else if (mode != lastMode)
	{
		lastMode = mode;
	}

	else
	{
		return;
	}

	Serial.println("--------------------");
	Serial.print("Mode: ");
	
	lcd->clear();

	switch (mode)
	{
	case 0:
		Serial.println("Running...");
		lcd->print("Running");
		show(current, voltage, lcd);
		break;

	case 1:
		Serial.println("Reading...");
		lcd->print("Reading");
		break;

	case 2:
		Serial.println("Holding...");
		lcd->print("Holding");
		show(current, voltage, lcd);
		break;

	case 3:
		Serial.println("Calibrating...");
		lcd->print("Calibrating");
		break;

	case 4:
		Serial.println("Datalogging...");
		lcd->print("Datalogging");
		break;

	default:
		break;
	}
}
