#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "image.h"
#include "kernel.h"

// Uztaisa kerneli ar Gausa sadalījumu
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

int main(int argc, const char** argv) {
	if (argc != 6 && argc != 7) {
		printf("Usage: md2 input output width height blur [flag]\n");
		printf("  Where 'input' is path to input file and 'output' is path ");
		printf("to output file,\n");
		printf("  'width' and 'height' are the width and height of the ");
		printf("Gaussian kernel (in\n");
		printf("  pixels) and 'blur' is the ammount of blur (1.0 to 5.0 works ");
		printf("best).\n");
		printf("  Input file must be a 24-bit .bmp file with no compression.\n");
		printf("  Flag -k will overlay kernel image on the output image.\n");
		
		return -1;
	}

	const char* input_path = argv[1];
	const char* output_path = argv[2];
	
	int kernel_width = atoi(argv[3]);
	int kernel_height = atoi(argv[4]);
	float kernel_blur = atof(argv[5]);
	
	char overlay_kernel = argc != 6 && strcmp(argv[6], "-k") == 0;
	
	Image img = LoadImageFromFile(input_path);
	
	Kernel k = MakeGaussianKernel(kernel_blur, kernel_width, kernel_height);

	Image blr = ConvolveKernel(img, k);
	
	// kerneļa uzkopēšana uz izvada attēla (ja nepieciešams)
	if (overlay_kernel) {
		for (int y = 0; y < k.height; y++) for (int x = 0; x < k.width; x++) {
			blr.data[img.width * y + x].r = 255.0f * k.data[k.width * y + x];
			blr.data[img.width * y + x].g = 255.0f * k.data[k.width * y + x];
			blr.data[img.width * y + x].b = 255.0f * k.data[k.width * y + x];
		}
	}

	SaveImageToFile(output_path, blr);
}