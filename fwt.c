#include <stdio.h>
#include <math.h>

void FWT(float* values, int n) {
	float sums[n/2];
	float difs[n/2];
	
	for (int i = 0; i < n/2; i++) {
		sums[i] = (values[i * 2] + values[i * 2 + 1]) / sqrtf(2.0f);
		difs[i] = (values[i * 2] - values[i * 2 + 1]) / sqrtf(2.0f);
	}
	
	for (int i = 0; i < n/2; i++) {
		values[i] = sums[i];
		values[n/2 + i] = difs[i];
	}
	
	if (n == 2) return;
	
	FWT(values, n/2);
}

void InvFWT(float* values, int n) {
	float sums[n/2];
	float difs[n/2];
	
	if (n != 2) {
		InvFWT(values, n/2);
	}
	
	for (int i = 0; i < n/2; i++) {
		sums[i] = (values[i] + values[i+n/2] ) / sqrtf(2.0f);
		difs[i] = (values[i] - values[i+n/2] ) / sqrtf(2.0f);
	} 
	
	for (int i = 0; i < n/2; i++) {
		values[2 * i] = sums[i];
		values[2 * i + 1] = difs[i];
	}
	
}
