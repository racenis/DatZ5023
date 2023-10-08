#ifndef FFT_H
#define FFT_H

#include <complex.h>

void FFT(float complex* input, float complex* output, int N);
void InvFFT(float complex* input, float complex* output, int N);
void PrintComplexArray(float complex* array, int size);

#endif // FFT_H