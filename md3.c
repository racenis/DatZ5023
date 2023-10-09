#include <complex.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "fft.h"
#include "image.h"

// Attēls kuram ir uztaisīta Furjē transformācija
typedef struct ComplexImage {
	int width;
	int height;
	
	float complex* red_channel;
	float complex* green_channel;
	float complex* blue_channel;
} ComplexImage;

// Veic Furjē transformāciju attēlam
ComplexImage ImageToComplexImage(Image img) {
	ComplexImage comp;
	
	comp.width = img.width;
	comp.height = img.height;
	comp.red_channel = malloc(img.width * img.height * sizeof(float complex));
	comp.green_channel = malloc(img.width * img.height * sizeof(float complex));
	comp.blue_channel = malloc(img.width * img.height * sizeof(float complex));
	
	for (int y = 0; y < img.height; y++) {
		float complex red_line[img.width];
		float complex green_line[img.width];
		float complex blue_line[img.width];
		
		for (int x = 0; x < img.width; x++) {
			Pixel pix = GetPixel(img, x, y);
			
			red_line[x] = (float)pix.r / 255.0f;
			green_line[x] = (float)pix.g / 255.0f;
			blue_line[x] = (float)pix.b / 255.0f;	
		}

		float complex red_line_t[img.width];
		float complex green_line_t[img.width];
		float complex blue_line_t[img.width];
		
		FFT(red_line, red_line_t, img.width);
		FFT(green_line, green_line_t, img.width);
		FFT(blue_line, blue_line_t, img.width);
		
		for (int i = 0; i < img.width; i++) {
			comp.red_channel[img.width*y +i] = red_line_t[i];
			comp.green_channel[img.width*y +i] = green_line_t[i];
			comp.blue_channel[img.width*y +i] = blue_line_t[i];
		}
	}
	
	for (int x = 0; x < img.width; x++) {
		float complex red_column[img.width];
		float complex green_column[img.width];
		float complex blue_column[img.width];

		for (int y = 0; y < img.height; y++) {
			red_column[y] = comp.red_channel[img.width * y + x];
			green_column[y] = comp.green_channel[img.width * y + x];
			blue_column[y] = comp.blue_channel[img.width * y + x];
		}

		float complex red_column_t[img.width];
		float complex green_column_t[img.width];
		float complex blue_column_t[img.width];
		
		FFT(red_column, red_column_t, img.width);
		FFT(green_column, green_column_t, img.width);
		FFT(blue_column, blue_column_t, img.width);
		
		for (int y = 0; y < img.height; y++) {
			comp.red_channel[img.width * y + x] = red_column_t[y];
			comp.green_channel[img.width * y + x] = green_column_t[y];
			comp.blue_channel[img.width * y + x] = blue_column_t[y];
		}
	}
	
	return comp;
}

// Veic inverso Furjē transformāciju attēlam
Image ComplexImageToImage(ComplexImage comp) {
	Image img =	NewImage(comp.width, comp.height);
	
	float complex* r = malloc(comp.width * comp.height * sizeof(float complex));
	float complex* g = malloc(comp.width * comp.height * sizeof(float complex));
	float complex* b = malloc(comp.width * comp.height * sizeof(float complex));
	
	for (int x = 0; x < img.width; x++) {
		float complex red_column[img.height];
		float complex green_column[img.height];
		float complex blue_column[img.height];
		
		for (int y = 0; y < img.height; y++) {
			red_column[y] = comp.red_channel[img.width * y + x];
			green_column[y] = comp.green_channel[img.width * y + x];
			blue_column[y] = comp.blue_channel[img.width * y + x];
		}
		
		float complex red_column_t[img.height];
		float complex green_column_t[img.height];
		float complex blue_column_t[img.height];
		
		InvFFT(red_column, red_column_t, img.height);
		InvFFT(green_column, green_column_t, img.height);
		InvFFT(blue_column, blue_column_t, img.height);
		
		for (int y = 0; y < img.height; y++) {
			r[img.width * y + x] = red_column_t[y];
			g[img.width * y + x] = green_column_t[y];
			b[img.width * y + x] = blue_column_t[y];
		}
		
	}
	
	for (int y = 0; y < img.height; y++) {
		float complex red_line[img.width];
		float complex green_line[img.width];
		float complex blue_line[img.width];
		
		for (int x = 0; x < img.width; x++) {
			red_line[x] = r[img.width*y +x];
			green_line[x] = g[img.width*y +x];
			blue_line[x] = b[img.width*y +x];
		}
		
		float complex red_line_t[img.width];
		float complex green_line_t[img.width];
		float complex blue_line_t[img.width];
		
		InvFFT(red_line, red_line_t, img.width);
		InvFFT(green_line, green_line_t, img.width);
		InvFFT(blue_line, blue_line_t, img.width);
		
		for (int x = 0; x < img.width; x++) {
			Pixel pix;
			
			pix.r = 255.0f * red_line_t[x];
			pix.g = 255.0f * green_line_t[x];
			pix.b = 255.0f * blue_line_t[x];
			
			SetPixel(img, pix, x, y);
		}
		
	}
	
	
	return img;
}

// Vizualizācija Furjē transformētajam attēlam.
Image ComplexImageExtract(ComplexImage comp) {
	Image img =	NewImage(comp.width, comp.height);

	for (int y = 0; y < comp.height; y++) {
	for (int x = 0; x < comp.width; x++) {
		int pix_off = comp.width * y + x;
		Pixel pix;
		
		pix.r = 15.0f * log(cabs(comp.red_channel[pix_off]));
		pix.g = 15.0f * log(cabs(comp.green_channel[pix_off]));
		pix.b = 15.0f * log(cabs(comp.blue_channel[pix_off]));
		
		img.data[pix_off] = pix;
	}}
	
	return img;
}

// Veic konvolūciju ar diviem Furjē transformētajiem attēliem
ComplexImage ComplexImageConvolve(ComplexImage a, ComplexImage b) {
	ComplexImage img;
	
	img.width = a.width;
	img.height = a.height;
	img.red_channel = malloc(a.width * a.height * sizeof(float complex));
	img.green_channel = malloc(a.width * a.height * sizeof(float complex));
	img.blue_channel = malloc(a.width * a.height * sizeof(float complex));
	
	for (int y = 0; y < a.height; y++) {
	for (int x = 0; x < a.width; x++) {
		float complex cr = a.red_channel[a.width * y + x] * b.red_channel[a.width * y + x];
		float complex cg = a.green_channel[a.width * y + x] * b.green_channel[a.width * y + x];
		float complex cb = a.blue_channel[a.width * y + x] * b.blue_channel[a.width * y + x];
		img.red_channel[a.width * y + x] = cr;
		img.green_channel[a.width * y + x] = cg;
		img.blue_channel[a.width * y + x] = cb;
	}
	}
	
	return img;
}

// Atrod nākamo ar 2 dalāmo skaitli
int NextPowerOfTwo(int x) {
	for (int i = 1;; i = i << 1) {
		if (i >= x) return i;
	}
}

int main(int argc, const char** argv) {
	if (argc != 4) {
		printf("Usage: md3 image kernel output [flag]\n");
		printf("  Where 'input' and 'kernel' are paths to images that will be convolved\n");
		printf("  together, and 'output' is path to file to which the resulting image will\n");
		printf("  be written to.\n");
		printf("  Input files must be unindexed 24-bit .bmp files with no compression.\n");
		
		return -1;
	}

	const char* image_path = argv[1];
	const char* kernel_path = argv[2];
	const char* output_path = argv[3];
	
	Image kakis_raw = LoadImageFromFile(image_path);
	Image kakis_img = ResizeImage(kakis_raw, NextPowerOfTwo(kakis_raw.width), NextPowerOfTwo(kakis_raw.height));
	
	printf("Computing image transform...\n");
	ComplexImage kakis_kompl = ImageToComplexImage(kakis_img);
	
	Image kernel_raw = LoadImageFromFile(kernel_path);
	Image kernel_img = ResizeImage(kernel_raw, NextPowerOfTwo(kakis_raw.width), NextPowerOfTwo(kakis_raw.height));
	
	printf("Computing kernel transform...\n");
	int kernel_brightness = 0;
	for (int i = 0; i < kernel_img.width * kernel_img.height; i++) {
		Pixel pix = kernel_img.data[i];
		kernel_brightness += (pix.r + pix.g + pix.b) / 3;
	}

	float kernel_normal = 255.0f / (float)kernel_brightness;
	
	ComplexImage kernel_kompl = ImageToComplexImage(kernel_img);
	
	for (int i = 0; i < kernel_img.width * kernel_img.height; i++) {
		kernel_kompl.red_channel[i] *= kernel_normal;
		kernel_kompl.green_channel[i] *= kernel_normal;
		kernel_kompl.blue_channel[i] *= kernel_normal;
	}
	
	ComplexImage convl_kompl = ComplexImageConvolve(kernel_kompl, kakis_kompl);
	
	printf("Computing inverse transform..\n");
	SaveImageToFile(output_path, ComplexImageToImage(convl_kompl));
	
	printf("Done!\n");
	
	return 0;
}