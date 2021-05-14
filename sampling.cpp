// Sampling and averaging functions

#include <stdint.h>
#include "sampling.h"
#include "ACS712.h"

// If HOLD is disabled, take readings in real time with fewer samples
int16_t getma(ACS712 acs, uint16_t samples){
	// Reset total and avg variables for next reading
	int16_t avg = 0;
	int32_t total = 0;
	int16_t mamp;

	// Capturing all samples
	for(int i = 0; i < samples; i++){
		mamp = acs.mA_DC();		// Get data from sensor (in miliamps)
		total += mamp;				// Add current reading to total
	}

	// Divides sum of all readings by the number of samples (average)
	return avg = total / samples;
}