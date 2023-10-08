#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "image.h"

typedef struct HSIPixel {
	float h;
	float s;
	float i;
} HSIPixel;

HSIPixel PixelToHSIPixel(Pixel pix) {
	int RGB = pix.r + pix.g + pix.b;
	
	float r = (float) pix.r / (float) RGB;
	float g = (float) pix.g / (float) RGB;
	float b = (float) pix.b / (float) RGB;
	
	float h1 = 0.5f * ((r - g) + (r - b));
	float h2 = sqrt(((r - g)*(r - g)) + ((r - b)*(g - b)));
	float h = acos(h1/h2);
	
	if (b > g) {
		h = 2.0f * M_PI - h;
	}
	
	float min_rgb = r < g ? (r < b ? r : b) : (g < b ? g : b);
	
	float s = 1.0f - 3.0f * min_rgb;
	
	float i = (float) RGB / (3.0f * 255.0f);
	
	HSIPixel res = {h, s, i};
	return res;
}

Pixel HSIPixelToPixel(HSIPixel pix) {
	float x = pix.i * (1.0f - pix.s);
	float y = pix.i * (1.0f + ((pix.s * cos(pix.h))/cos(M_PI/3.0f - pix.h)));
	float z = (3.0f * pix.i) - (x + y);
	
	Pixel res;
	
	if (x < 0.0f) x = 0.0f;
	if (y < 0.0f) y = 0.0f;
	if (z < 0.0f) z = 0.0f;
	
	if (x > 1.0f) x = 0.0f;
	if (y > 1.0f) y = 0.0f;
	if (z > 1.0f) z = 0.0f;
	
	if (pix.h < (2.0f * M_PI)/3.0f) {
		res.b = x * 255.0f;
		res.r = y * 255.0f;
		res.g = z * 255.0f;
	} else if (pix.h < (4.0f * M_PI)/3.0f) {
		res.r = x * 255.0f;
		res.g = y * 255.0f;
		res.b = z * 255.0f;
	} else {
		res.g = x * 255.0f;
		res.b = y * 255.0f;
		res.r = z * 255.0f;
	}
	
	return res;
}

int main(int argc, const char** argv) {
	Image img = LoadImageFromFile("kakis.bmp");
	Image res = NewImage(img.width, img.height);
	
	float shift = 50.0f;
	
	for (int i = 0; i < img.width * img.height; i++) {
		HSIPixel pix = PixelToHSIPixel(img.data[i]);
		
		pix.h += shift * (M_PI/180.0f);
		
		res.data[i] = HSIPixelToPixel(pix);
		
		
		/*if (res.data[i].r == 254 && res.data[i].g == 0 && res.data[i].b == 7) {
			printf("%i %i %i\n", img.data[i].r, img.data[i].g, img.data[i].b);
			printf("%f %f %f\n", pix.h, pix.s, pix.i);
		}*/
		
	}
	
	SaveImageToFile("kakisbuba.bmp", res);
	
	return 0;
}