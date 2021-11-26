#include "sampling.hpp"

void Sampling::begin(uint16_t smpSize_)
{
	offsetCurrent = 0.0;
	totalCurrent = 0.0;
	totalVoltage = 0.0;
	smpSize = smpSize_;
	smpCnt = smpSize_;
}

void Sampling::addToAvg(float valueCurrent, float valueVoltage)
{
	if (!smpCnt)
	{
		smpCnt = smpSize;
	}
	
	totalCurrent += valueCurrent;
	totalVoltage += valueVoltage;
	smpCnt--;

	if (!smpCnt)
	{
		avgCurrent = (totalCurrent / smpSize) - offsetCurrent;
		avgVoltage = (totalVoltage / smpSize);
		totalCurrent = 0.0;
		totalVoltage = 0.0;
	}
}

void Sampling::avgIsOffset()
{
	offsetCurrent = avgCurrent;
}

void Sampling::changeSize(uint16_t newSize_)
{
	smpSize = newSize_;
	smpCnt = newSize_;
	totalCurrent = 0.0;
	totalVoltage = 0.0;
}
