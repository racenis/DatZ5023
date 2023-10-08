#include <complex.h>

#include "fft.h"

int main() {
	float complex series[8];
	float complex transformed[8];
	float complex back_again[8];

	series[0] = 420.0f;
	series[1] = 69.0f;
	series[2] = 489.0f;
	series[3] = 0.0f;
	series[4] = 0.0f;
	series[5] = 0.0f;
	series[6] = 0.0f;
	series[7] = 0.0f;
	
	PrintComplexArray(series, 8);
	
	FFT(series, transformed, 8);
	
	PrintComplexArray(transformed, 8);
	
	InvFFT(transformed, back_again, 8);
	
	PrintComplexArray(back_again, 8);
	
	return 0;
}