#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "image.h"

typedef struct Kernel {
	int width;
	int height;
	float* data;
} Kernel;

Kernel MakeGaussianKernel(float scale, int w, int h) {
	Kernel k;
	
	k.width = w;
	k.height = h;
	k.data = malloc(w * h * sizeof(float));
	
	for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) {
		float dscale = 1.0f / (scale * scale);
		
		float fx = ((float)x - (float)w/2.0f) * dscale;
		float fy = ((float)y - (float)h/2.0f) * dscale;
		
		float value = 1.0f / exp(fx * fx + fy * fy);
		
		k.data[w * y + x] = value;
	}
	
	return k;
}

Pixel GetClampedPixel(Image img, int x, int y) {
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	
	if (x >= img.width) x = img.width - 1;
	if (y >= img.height) y = img.height - 1;
	
	return GetPixel(img, x, y);
}

int main(int argc, const char** argv) {
	Image img = LoadImageFromFile("kakis.bmp");
	Image blr = NewImage(img.width, img.height);
	
	Kernel k = MakeGaussianKernel(4.0f, 50, 1);

	float kernel_sum = 0.0f;
	int kernel_size = k.width * k.height;
	for (int i = 0; i < kernel_size; i++) {
		kernel_sum += k.data[i];
	}
	
	//float kernel_weight = kernel_sum / (float)kernel_size;
	float kernel_weight = 1.0f / kernel_sum;
	
	printf("k size %i k sum %f\n", kernel_size, kernel_sum);
	
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
		
		//printf("%f ", r);
		
		Pixel pix = {r, g, b};
		
		SetPixel(blr, pix, x, y);
	}
	}
	
	
	
	
	for (int y = 0; y < k.height; y++) for (int x = 0; x < k.width; x++) {
		blr.data[img.width * y + x].r = 255.0f * k.data[k.width * y + x];
		blr.data[img.width * y + x].g = 255.0f * k.data[k.width * y + x];
		blr.data[img.width * y + x].b = 255.0f * k.data[k.width * y + x];
	}
	
	SaveImageToFile("kakis2.bmp", blr);
}