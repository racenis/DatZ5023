#include "kernel.h"

#include <stdio.h>
#include <math.h>

// Uztaisa attēla-kerneļa konvolūciju
Image ConvolveKernel(Image img, Kernel k) {
	Image blr = NewImage(img.width, img.height);

	// saskaitām kopā visu kerneli
	float kernel_sum = 0.0f;
	int kernel_size = k.width * k.height;
	for (int i = 0; i < kernel_size; i++) {
		kernel_sum += k.data[i];
	}
	
	// kerneļa normalizācijas skaitlis
	float kernel_weight = 1.0f / kernel_sum;
	
	// kerneļa konvolūcija ar attēlu
	for (int y = 0; y < img.height; y++) {
	for (int x = 0; x < img.width; x++) {
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
		
		for (int ky = 0; ky < k.height; ky++) {
		for (int kx = 0; kx < k.width; kx++) {
			int sx = x + kx - (k.width/2);
			int sy = y + ky - (k.height/2);
			
			Pixel pix = GetClampedPixel(img, sx, sy);
			
			float kval = k.data[k.width * ky + kx];
			
			r += (float)pix.r * kval;
			g += (float)pix.g * kval;
			b += (float)pix.b * kval;
		}}
		
		r *= kernel_weight;
		g *= kernel_weight;
		b *= kernel_weight;
		
		if (r < 0.0f) r = 0.0f;
		if (g < 0.0f) g = 0.0f;
		if (b < 0.0f) b = 0.0f;
		
		if (r > 255.0f) r = 255.0f;
		if (g > 255.0f) g = 255.0f;
		if (b > 255.0f) b = 255.0f;
		
		Pixel pix = {r, g, b};
		
		SetPixel(blr, pix, x, y);
	}
	}
	
	return blr;
}

// Uztaisa kerneļa-attēla konvolūciju, bet nenormalizē kerneli
Image ConvolveKernelNormalized(Image img, Kernel k) {
	Image blr = NewImage(img.width, img.height);

	for (int y = 0; y < img.height; y++) {
	for (int x = 0; x < img.width; x++) {
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;
		
		for (int ky = 0; ky < k.height; ky++) {
		for (int kx = 0; kx < k.width; kx++) {
			int sx = x + kx - (k.width/2);
			int sy = y + ky - (k.height/2);
			
			Pixel pix = GetClampedPixel(img, sx, sy);
			
			float val = k.data[k.width * ky + kx];
			
			r += (float)pix.r * val;
			g += (float)pix.g * val;
			b += (float)pix.b * val;
		}}
		
		r = fabsf(r);
		g = fabsf(g);
		b = fabsf(b);
		
		if (r > 255.0f) r = 255.0f;
		if (g > 255.0f) g = 255.0f;
		if (b > 255.0f) b = 255.0f;
		
		Pixel pix = {r, g, b};
		
		SetPixel(blr, pix, x, y);
	}
	}
	
	return blr;
}