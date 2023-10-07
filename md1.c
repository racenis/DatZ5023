#include <stdio.h>
#include <limits.h>
#include <string.h>

#include "image.h"

int main(int argc, const char** argv) {
	if (argc != 3 && argc != 4) {
		printf("Usage: md1 input output [flag]\n");
		printf("  Where 'input' is path to input file and 'output' is path ");
		printf("to output file.\n");
		printf("  Input file must be a 24-bit .bmp file with no compression.\n");
		printf("  Flag -g will output image as grayscale.\n");
		
		return -1;
	}

	const char* input_path = argv[1];
	const char* output_path = argv[2];
	
	char grayscale = argc != 3 && strcmp(argv[3], "-g") == 0;
	
	Image img = LoadImageFromFile(input_path);

	// es algoritmu atradu vikipēdijā
	int histogram[256];
	memset(histogram, 0, sizeof(histogram));
	
	// histogrammas atrašana no attēla
	for (int y = 0; y < img.height; y++) {
		for (int x = 0; x < img.width; x++) {
			Pixel pix = GetPixel(img, x, y);
			
			unsigned char value = (pix.r + pix.g + pix.b) / 3;
			
			histogram[value]++;
		}
	}
	
	
	int cdf[256] = {histogram[0]};
	for (int i = 1; i < 256; i++) {
		cdf[i] = cdf[i - 1] + histogram[i]; 
	}
	
	int cdf_min = INT_MAX;
	int cdf_max = 0;
	for (int i = 1; i < 256; i++) {
		if (cdf[i] < cdf_min) cdf_min = cdf[i];
		if (cdf[i] > cdf_max) cdf_max = cdf[i];
	}

	// histogrammas normalizācijas tabula
	float cdf_normalized[256];
	for (int i = 1; i < 256; i++) {
		cdf_normalized[i] = (((float)(cdf[i] - cdf_min)) / (float)(cdf_max - cdf_min)) * 255.0f;
	}
	
	// attēla histogrammatiskā normalizācija
	for (int y = 0; y < img.height; y++) {
		for (int x = 0; x < img.width; x++) {
			Pixel pix = GetPixel(img, x, y);
			
			// pikseļa spilgtums
			unsigned char value = (pix.r + pix.g + pix.b) / 3;
			
			if (!grayscale) {
				// šis ir lai nebūtu dalīšana ar nulli
				float float_value = (float)value;
				if (float_value < 0.0001f) float_value = 0.0001f;
				
				// attiecība starp pikseļa spilgtumu pirms un pēc normalizācijas
				float value_adj = (float)cdf_normalized[value] / float_value; 

				// pabīda krāsas lai tās izkliedējās pa histogrammu
				float r = (float)pix.r * value_adj;
				float g = (float)pix.g * value_adj;
				float b = (float)pix.b * value_adj;

				// lai neiet pāri malām (unsigned char)
				if (r > 255.0f) r = 255.0f;
				if (g > 255.0f) g = 255.0f;
				if (b > 255.0f) b = 255.0f;
				
				pix.r = r;
				pix.g = g;
				pix.b = b;
			} else {
				pix.r = cdf_normalized[value];
				pix.g = cdf_normalized[value];
				pix.b = cdf_normalized[value];
			}
			
			SetPixel(img, pix, x, y);
		}
	}
	
	SaveImageToFile(output_path, img);
	
	return 0;
}