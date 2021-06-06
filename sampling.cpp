// This file contains functions related to reading, sampling and averaging
// the data coming from the ACS712 current sensor

// Libraries
#include <stdint.h>
#include "sampling.hpp"
#include "ACS712.h"

// Captures samples from the ACS712 and averages them
// Pass the ACS712 object and how many samples to take as parameters
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
  // and returns the absolute value
  avg = total / samples;
  if(avg >= 0) {
    return avg;
  }
  else return -avg;
}
