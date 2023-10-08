#include <stdio.h>
#include <stdlib.h>

#include "image.h"
#include "kernel.h"

int main(int argc, const char** argv) {
	if (argc != 3) {
		printf("Usage: md6 input output\n");
		printf("  Where 'input' is path to input file and 'output' is path ");
		printf("to output file.\n");
		printf("  Input file must be a 24-bit .bmp file with no compression.\n");
		return -1;
	}
	
	const char* input = argv[1];
	const char* output = argv[2];
	
	Image img = LoadImageFromFile(input);
	
	float kernel_weights[4] = {
		1.0f, 1.0f,
		1.0f, -1.0f
	};
	
	Kernel k = {
		2,
		2,
		kernel_weights
	};
	
	Image egg = ConvolveKernel(img, k);
	
	SaveImageToFile(output, egg);
}