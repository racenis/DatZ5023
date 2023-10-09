#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image.h"
#include "kernel.h"

enum {
	MODE_LAPLACE,
	MODE_SOBEL
};

int main(int argc, const char** argv) {
	if (argc != 4) {
		printf("Usage: md10 input output mode\n");
		printf("  Where 'input' is path to input file and 'output' is path ");
		printf("to output file, and 'mode' is the mode of the program.\n");
		printf("  Mode must be either '-laplace' or '-sobel'. ");
		printf("  Input file must be a 24-bit .bmp file with no compression.\n");
		return -1;
	}
	
	const char* input = argv[1];
	const char* output = argv[2];
	
	int mode;
	
	if (strcmp(argv[3], "-laplace") == 0) {
		mode = MODE_LAPLACE;
	} else if (strcmp(argv[3], "-sobel") == 0) {
		mode = MODE_SOBEL;
	} else {
		printf("Unrecognized mode: %s\n", argv[3]);
		return -1;
	}

	Image img = LoadImageFromFile(input);
	
	float sobel_vertical_weights[9] = {
		1.0f, 0.0f, -1.0f,
		2.0f, 0.0f, -2.0f,
		1.0f, 0.0f, -1.0f
	};
	
	float sobel_horizontal_weights[9] = {
		1.0f, 2.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		-1.0f, -2.0f, -1.0f
	};
	
	Kernel sobel_vertical = {
		3,
		3,
		sobel_vertical_weights
	};
	
	Kernel sobel_horizontal = {
		3,
		3,
		sobel_horizontal_weights
	};
	
	float laplace_weights[9] = {
		0.0f, 1.0f, 0.0f,
		1.0f, -4.0f, 1.0f,
		0.0f, 1.0f, 0.0f
	};
	
	Kernel laplace = {
		3,
		3,
		sobel_horizontal_weights
	};
	
	
	Image sobel1 = ConvolveKernelNormalized(img, sobel_vertical);
	Image sobel2 = ConvolveKernelNormalized(img, sobel_horizontal);
	
	Image egg = NewImage(img.width, img.height);
	
	for (int i = 0; i < img.width * img.height; i++) {
		int r = sobel1.data[i].r + sobel2.data[i].r;
		int g = sobel1.data[i].g + sobel2.data[i].g;
		int b = sobel1.data[i].b + sobel2.data[i].b;
		
		egg.data[i].r = r > 255 ? 255 : r;
		egg.data[i].g = g > 255 ? 255 : g;
		egg.data[i].b = b > 255 ? 255 : b;
	}
	
	Image geg = ConvolveKernelNormalized(img, laplace);
	
	switch (mode) {
		case MODE_LAPLACE:
			SaveImageToFile(output, geg);
			break;
		case MODE_SOBEL:
			SaveImageToFile(output, egg);
			break;
	}
}