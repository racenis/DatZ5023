#include <stdio.h>
#include <stdlib.h>

#include "image.h"

Pixel Color2400KTo6500K(Pixel pix) {
	Pixel res;
	
	float red_val = (float)pix.r * (255.0f / 255.0f);
	float green_val = (float)pix.g * (249.0f / 157.0f);
	float blue_val = (float)pix.b * (251.0f / 63.0);
	
	res.r = red_val > 255.0f ? 255.0f : red_val;
	res.g = green_val > 255.0f ? 255.0f : green_val;
	res.b = blue_val > 255.0f ? 255.0f : blue_val;
	
	return res;
}

int main(int argc, const char** argv) {
	if (argc != 3) {
		printf("Usage: md9 input output\n");
		printf("  Where 'input' is path to input file and 'output' is path ");
		printf("to output file.\n");
		printf("  Input file must be a 24-bit .bmp file with no compression.\n");
		return -1;
	}
	
	const char* input = argv[1];
	const char* output = argv[2];
	
	Image img = LoadImageFromFile(input);
	Image res = NewImage(img.width, img.height);
	
	for (int i = 0; i < img.width * img.height; i++) {
		res.data[i] = Color2400KTo6500K(img.data[i]);
	}
	
	SaveImageToFile(output, res);
}