#include <stdio.h>
#include <complex.h>
#include <math.h>

void fft(float complex* input, float complex* output, int N) {
	if (N == 1) {
		output[0] = input[0];
	} else {
		float complex even_input[N/2];
		float complex odd_input[N/2];
		
		for (int i = 0; i < N/2; i++) {
			even_input[i] = input[i * 2];
			odd_input[i] = input[i * 2 + 1];
		}
		
		float complex even_output[N/2];
		float complex odd_output[N/2];
		
		fft(even_input, even_output, N/2);
		fft(odd_input, odd_output, N/2);
		
		for (int k = 0; k < N/2; k++) {
			float complex p = even_output[k];
			float complex q = cexpf(-2.0f * M_PI * I * (float)k/(float)N) * odd_output[k];
			
			output[k] = p + q;
			output[k + N/2] = p - q;
		}
	}
}

void invfft(float complex* input, float complex* output, int N) {
	float complex conjugate[N];
	float complex transform[N];
	
	for (int i = 0; i < N; i++) {
		conjugate[i] = conjf(input[i]);
	}
	
	fft(conjugate, transform, N);
	
	for (int i = 0; i < N; i++) {
		output[i] = conjf(transform[i]/(float)N);
	}
}

void PrintComplexArray(float complex* array, int size) {
	printf("[");
	
	for (int i = 0; i < size; i++) {
		float real = creal(array[i]);
		float imag = cimag(array[i]);

		// pārveido negatīvo nulli par pozitīvo nulli
		if (imag == 0.0f) {
			imag = 0.0f;
		}
		
		if (imag >= 0.0f) {
			printf("%.1f+%.1fi", real, imag);
		} else {
			printf("%.1f%.1fi", real, imag);
		}
		
		if (i != size - 1) {
			printf(" ");
		}
	}
	
	printf("]\n");
}

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
	
	fft(series, transformed, 8);
	
	PrintComplexArray(transformed, 8);
	
	invfft(transformed, back_again, 8);
	
	PrintComplexArray(back_again, 8);
	
	return 0;
}