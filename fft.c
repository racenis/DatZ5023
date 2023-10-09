#include <stdio.h>
#include <complex.h>
#include <math.h>
#include <stdlib.h>

// Cooley-Tukey algoritms
static void fft(float complex* input, float complex* output, int N) {
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

// Inversais FFT algoritms
static void invfft(float complex* input, float complex* output, int N) {
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

// FFT funckija ar ievada pārbaudi
void FFT(float complex* input, float complex* output, int N) {
	if ((N & (N - 1)) != 0) {
		printf("N must be power of two, instead is %i.", N);
		abort();
	}
	
	fft(input, output, N);
}

// Inversā FFT funkcija ar ievada pārbaudi
void InvFFT(float complex* input, float complex* output, int N) {
	if ((N & (N - 1)) != 0) {
		printf("N must be power of two, instead is %i.", N);
		abort();
	}
	
	invfft(input, output, N);
}

// Izdrukukā Furjē transformāciju
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

