#include <stdio.h>
#include <stdlib.h>

#include "image.h"
#include "kernel.h"

int main(int argc, const char** argv) {
	/*if (argc != 3) {
		printf("Usage: md6 input output\n");
		printf("  Where 'input' is path to input file and 'output' is path ");
		printf("to output file.\n");
		printf("  Input file must be a 24-bit .bmp file with no compression.\n");
		return -1;
	}
	
	const char* input = argv[1];
	const char* output = argv[2];*/
	
	const char* input = "kakis.bmp";
	const char* output = "kakisdenoiz.bmp";
	
	Image img = LoadImageFromFile(input);
	
	float kakis_r[img.width * img.height];
	float kakis_g[img.width * img.height];
	float kakis_b[img.width * img.height];
	
	for (int i = 0; i < img.width * img.height; i++) {
		kakis_r[i] = (float)img.data[i].r / 255.0f;
		kakis_g[i] = (float)img.data[i].g / 255.0f;
		kakis_b[i] = (float)img.data[i].b / 255.0f;
	}
	
	/*for (int y = 0; y < img.height; y++){
	for (int x = 0; x < img.width; x++) {
		FWT(kakis_r[y*image_width])
	}}*/
	
	for (int i = 0; i < img.width * img.height; i++) {
		img.data[i].r = kakis_r[i] * 255.0f;
		img.data[i].g = kakis_g[i] * 255.0f;
		img.data[i].b = kakis_b[i] * 255.0f;
	}

	SaveImageToFile(output, egg);
}