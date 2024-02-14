#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "image.h"

typedef struct HSIPixel {
	float h;
	float s;
	float i;
} HSIPixel;

const float PI = 3.14159265358979;

HSIPixel PixelToHSIPixel(Pixel pix) {
	int RGB = pix.r + pix.g + pix.b;
	
	float r = (float) pix.r / (float) RGB;
	float g = (float) pix.g / (float) RGB;
	float b = (float) pix.b / (float) RGB;
	
	float h1 = 0.5f * ((r - g) + (r - b));
	float h2 = sqrtf(((r - g)*(r - g)) + ((r - b)*(g - b)));
	float h = acosf(h1/h2);
	
	if (b > g) {
		h = 2.0f * PI - h;
	}
	
	float min_rgb = r < g ? (r < b ? r : b) : (g < b ? g : b);
	
	float s = 1.0f - 3.0f * min_rgb;
	
	float i = (float) RGB / (3.0f * 255.0f);
	
	HSIPixel res = {h, s, i};
	return res;
}

Pixel HSIPixelToPixel(HSIPixel pix) {
	float h = (pix.h * (180.0f/PI)) / 60.0f;
	float z = 1.0f - fabsf(fmodf(h, 2.0f) - 1.0f);
	float c = (3.0f * pix.i * pix.s) / (1.0f + z);
	float x = c * z;
	
	Pixel res;
	
	if (h < 1.0f) {
		res.r = c * 255.0f;
		res.g = x * 255.0f;
		res.b = 0;
	} else if (h < 2.0f) {
		res.r = x * 255.0f;
		res.g = c * 255.0f;
		res.b = 0;
	} else if (h < 3.0f) {
		res.r = 0;
		res.g = c * 255.0f;
		res.b = x * 255.0f;
	} else if (h < 4.0f) {
		res.r = 0;
		res.g = x * 255.0f;
		res.b = c * 255.0f;
	} else if (h < 5.0f) {
		res.r = x * 255.0f;
		res.g = 0;
		res.b = c * 255.0f;
	} else if (h < 6.0f) {
		res.r = c * 255.0f;
		res.g = 0;
		res.b = x * 255.0f;
	} else {
		res.r = 0;
		res.g = 0;
		res.b = 0;
	}
	
	float m = pix.i * (1.0f - pix.s);
	
	res.r += m * 255.0f;
	res.g += m * 255.0f;
	res.b += m * 255.0f;
	
	return res;
}

int main(int argc, const char** argv) {
	if (argc != 4) {
		printf("Usage: md8 input output shift\n");
		printf("  Where 'input' is path to input file and 'output' is path ");
		printf("to output file, and\n");
		printf("  'shift' is the hue shift in degrees.\n");
		printf("  Input file must be a 24-bit .bmp file with no compression.\n");
		return -1;
	}
	
	const char* input = argv[1];
	const char* output = argv[2];
	
	float shift = atof(argv[3]);
	
	Image img = LoadImageFromFile(input);
	Image res = NewImage(img.width, img.height);
	
	for (int i = 0; i < img.width * img.height; i++) {
		HSIPixel pix = PixelToHSIPixel(img.data[i]);
		
		pix.h += shift * (PI/180.0f);
		
		pix.h = fmodf(pix.h, 360.0f * (PI/180.0f));

		res.data[i] = HSIPixelToPixel(pix);
	}
	
	SaveImageToFile(output, res);
	
	return 0;
}