#ifndef KERNEL_H
#define KERNEL_H

#include "image.h"

typedef struct Kernel {
	int width;
	int height;
	float* data;
} Kernel;

Image ConvolveKernel(Image img, Kernel k);
Image ConvolveKernelNormalized(Image img, Kernel k);

#endif // KERNEL_H