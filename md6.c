#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "image.h"
#include "fwt.h"

int PP(int x) {
	for (int i = 1;; i = i << 1) {
		if (i >= x) return i;
	}
}

int main(int argc, const char** argv) {
	if (argc != 4 && argc != 5) {
		printf("Usage: md6 input output gate [flags]\n");
		printf("  Where 'input' is path to input file and 'output' is path ");
		printf("to output file, and\n");
		printf("  'gate' is a value in range [0.0..1.0].\n");
		printf("  Optional flag '-s', which skips Haar inverse transform.\n");
		printf("  Input file must be a 24-bit .bmp file with no compression.\n");
		return -1;
	}
	
	const char* input = argv[1];
	const char* output = argv[2];
	
	float gate = atof(argv[3]);
	
	char invert = argc == 4 || strcmp(argv[4], "-s") != 0;
	
	Image kakis = LoadImageFromFile(input);
	Image img = ResizeImage(kakis, PP(kakis.width), PP(kakis.height));
	
	// attēls peldošā punkta reprezentācijā
	float* kakis_r = malloc(img.width * img.height * sizeof(float));
	float* kakis_g = malloc(img.width * img.height * sizeof(float));
	float* kakis_b = malloc(img.width * img.height * sizeof(float));
	
	for (int i = 0; i < img.width * img.height; i++) {
		kakis_r[i] = (float)img.data[i].r / 255.0f;
		kakis_g[i] = (float)img.data[i].g / 255.0f;
		kakis_b[i] = (float)img.data[i].b / 255.0f;
	}
	
	// Hāra transformācija
	for (int y = 0; y < img.height; y++){
		FWT(&kakis_r[y * img.width], img.width);
		FWT(&kakis_g[y * img.width], img.width);
		FWT(&kakis_b[y * img.width], img.width);
	}
	
	for (int x = 0; x < img.width; x++) {
		float column_r[img.height];
		float column_g[img.height];
		float column_b[img.height];
		
		for (int y = 0; y < img.height; y++) {
			column_r[y] = kakis_r[img.width * y + x];
			column_g[y] = kakis_g[img.width * y + x];
			column_b[y] = kakis_b[img.width * y + x];
		}
		
		FWT(column_r, img.height);
		FWT(column_g, img.height);
		FWT(column_b, img.height);
		
		for (int y = 0; y < img.height; y++) {
			kakis_r[img.width * y + x] = column_r[y];
			kakis_g[img.width * y + x] = column_g[y];
			kakis_b[img.width * y + x] = column_b[y];
		}
	}
	
	// filtrēšana
	for (int i = 0; i < img.width * img.height; i++) {
		if (fabsf(kakis_r[i]) < gate) kakis_r[i] = 0.0f;
		if (fabsf(kakis_g[i]) < gate) kakis_g[i] = 0.0f;
		if (fabsf(kakis_b[i]) < gate) kakis_b[i] = 0.0f;
	}

	// inversā Hāra transformācija
	if (invert) {
		for (int x = 0; x < img.width; x++) {
			float column_r[img.height];
			float column_g[img.height];
			float column_b[img.height];
			
			for (int y = 0; y < img.height; y++) {
				column_r[y] = kakis_r[img.width * y + x];
				column_g[y] = kakis_g[img.width * y + x];
				column_b[y] = kakis_b[img.width * y + x];
			}
			
			InvFWT(column_r, img.height);
			InvFWT(column_g, img.height);
			InvFWT(column_b, img.height);
			
			for (int y = 0; y < img.height; y++) {
				kakis_r[img.width * y + x] = column_r[y];
				kakis_g[img.width * y + x] = column_g[y];
				kakis_b[img.width * y + x] = column_b[y];
			}
		}
		
		
		for (int y = 0; y < img.height; y++){
			InvFWT(&kakis_r[y * img.width], img.width);
			InvFWT(&kakis_g[y * img.width], img.width);
			InvFWT(&kakis_b[y * img.width], img.width);
		}
	}

	// pārvēršana atpakaļ veselo skaitļu reprezentācijā
	Image egg = NewImage(img.width, img.height);

	for (int i = 0; i < img.width * img.height; i++) {
		if (kakis_r[i] < 0.0f) kakis_r[i] = 0.0f;
		if (kakis_g[i] < 0.0f) kakis_g[i] = 0.0f;
		if (kakis_b[i] < 0.0f) kakis_b[i] = 0.0f;
		
		if (kakis_r[i] > 1.0f) kakis_r[i] = 1.0f;
		if (kakis_g[i] > 1.0f) kakis_g[i] = 1.0f;
		if (kakis_b[i] > 1.0f) kakis_b[i] = 1.0f;
		
		egg.data[i].r = kakis_r[i] * 255.0f;
		egg.data[i].g = kakis_g[i] * 255.0f;
		egg.data[i].b = kakis_b[i] * 255.0f;
	}
	
	if (invert) {
		Image egg_crop = ResizeImage(egg, kakis.width, kakis.height);
		SaveImageToFile(output, egg_crop);
	} else {
		SaveImageToFile(output, egg);
	}
	
	
}