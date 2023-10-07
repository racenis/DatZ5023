#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Pixel {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} Pixel;

typedef struct Image {
	int width;
	int height;
	
	Pixel* data;
} Image;

typedef struct BMPHeader {
	int file_size;
	int reserved;
	int data_offset;
	
	int header_size;
	int width;
	int height;
	short planes;
	short bit_count;
	int compression;
	int image_size;
	int unimportant[4];
} BMPHeader;

static const Pixel COLOR_WHITE = {255, 255, 255};
static const Pixel COLOR_BLACK = {0, 0, 0};

typedef struct BMPPixel {
	unsigned char b;
	unsigned char g;
	unsigned char r;
} BMPPixel;

Pixel GetPixel(Image img, int x, int y) {
	return img.data[img.width * y + x];
}

void SetPixel(Image img, Pixel pix, int x, int y) {
	img.data[img.width * y + x] = pix;
}

void FillImage(Image img, Pixel pix) {
	for (int y = 0; y < img.height; y++) {
		for (int x = 0; x < img.width; x++) {
			img.data[img.width * y + x] = pix;
		}
	}
}

Image NewImage(int width, int height) {
	Image img;
	
	img.width = width;
	img.height = height;
	img.data = malloc(width * height * sizeof(Pixel));
	
	return img;
}

Image LoadImageFromFile(const char* path) {
	BMPHeader header;
	Image image;
	
	FILE* file = fopen(path, "rb");
	
	if (!file) {
		printf("Can't open file: %s\n", path);
		abort();
	}
	
	char sig[2];
	fread(sig, sizeof(char), 2, file);
	
	if (sig[0] != 'B' || sig[1] != 'M') {
		printf("Incorrect file header: %c%c", sig[0], sig[1]);
	}
	
	fread(&header, sizeof(BMPHeader), 1, file);
	
	if (header.bit_count != 24) {
		printf("Expected 24 bpp, instead got %i\n", header.bit_count);
	}
	
	if (header.compression != 0) {
		printf("Expected 0 compression, instead got %i\n", header.compression);
	}
	
	printf("Image size: %ix%i\n", header.width, header.height);
	
	image.width = header.width;
	image.height = header.height;
	
	char* bmp_data = malloc(header.image_size);
	fread(bmp_data, sizeof(char), header.image_size, file);
	
	printf("uimage size: %i\n", header.image_size);
	
	int image_data_size = image.width * image.height * sizeof(Pixel);
	image.data = malloc(image_data_size);
	
	fclose(file);
	
	int scln_mod = (image.width * 3) % 4;
	int scln_padding = scln_mod == 0 ? 0 : 4 - scln_mod;
	int scln_size = (image.width * 3) + scln_padding;
	
	for (int y = 0; y < image.height; y++) {
		int scln_inv = image.height - y - 1;
		//int scln_inv = y;
		BMPPixel* scln = (BMPPixel*)&bmp_data[scln_inv * scln_size];
		Pixel* imgln = &image.data[y * image.width];
		
		for (int x = 0; x < image.width; x++) {
			imgln[x].r = scln[x].r;
			imgln[x].g = scln[x].g;
			imgln[x].b = scln[x].b;
			
			
			
		}
	}
	
	printf("scanline padding: %i\n", scln_padding);
	
	
	
	free(bmp_data);
	
	printf("A4!!\n");
	
	return image;
}


void SaveImageToFile(const char* path, Image image) {
	FILE* file = fopen(path,"wb");
	
	if (!file) {
		printf("Can't open file: %s\n", path);
		abort();
	}

	int scln_mod = (image.width * 3) % 4;
	int scln_padding = scln_mod == 0 ? 0 : 4 - scln_mod;
	int scln_size = (image.width * 3) + scln_padding;
	
	int bmp_size = image.height * scln_size;
	char* bmp_data = malloc(bmp_size);
	//memset(bmp_data, 0, bmp_size);
	
	for (int y = 0; y < image.height; y++) {
		int scln_inv = image.height - y - 1;
		BMPPixel* scln = (BMPPixel*)&bmp_data[scln_inv * scln_size];
		Pixel* imgln = &image.data[y * image.width];
		
		for (int x = 0; x < image.width; x++) {
			scln[x].r = imgln[x].r;
			scln[x].g = imgln[x].g;
			scln[x].b = imgln[x].b;
		}
	}
	
	char sig[2] = {'B', 'M'};
	fwrite(sig, sizeof(sig), 1, file);
	
	BMPHeader header = {
		scln_size * image.height + 54,
		0,
		54,
		40,
		image.width,
		image.height,
		1,
		24,
		0,
		scln_size * image.height,
		{0, 0, 0, 0}
	};
	
	fwrite(&header, sizeof(BMPHeader), 1, file);
	fwrite(bmp_data, sizeof(char), bmp_size, file);
	
	free(bmp_data);
	
	fclose(file);
}


int main(int argc, const char** argv) {
	if (argc != 3 && argc != 4) {
		printf("Usage: md1 input output [flag]\n");
		printf("  Where 'input' is path to input file and 'output' is path ");
		printf("to output file.\n");
		printf("  Input file must be a 24-bit .bmp file with no compression.\n");
		printf("  Flag -g will output image as grayscale.");
		
		return -1;
	}

	const char* input_path = argv[1];
	const char* output_path = argv[2];
	
	char grayscale = argc != 3 && strcmp(argv[3], "-g") == 0;
	
	Image img = LoadImageFromFile(input_path);

	int histogram[256];
	memset(histogram, 0, sizeof(histogram));
	
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

	float cdf_normalized[256];
	for (int i = 1; i < 256; i++) {
		cdf_normalized[i] = (((float)(cdf[i] - cdf_min)) / (float)(cdf_max - cdf_min)) * 255.0f;
	}
	
	for (int y = 0; y < img.height; y++) {
		for (int x = 0; x < img.width; x++) {
			Pixel pix = GetPixel(img, x, y);
			
			unsigned char value = (pix.r + pix.g + pix.b) / 3;
			
			if (!grayscale) {
				// šis ir lai nebūtu dalīšana ar nulli
				float float_value = (float)value;
				if (float_value < 0.0001f) float_value = 0.0001f;
				
				// attiecība starp pikseļa spilgtumu pirms un pēc normalizācijas
				float value_adj = (float)cdf_normalized[value] / float_value; 
				
				float r = (float)pix.r * value_adj;
				float g = (float)pix.g * value_adj;
				float b = (float)pix.b * value_adj;
				
				//if (r < 0.0f) r = 0.0f;
				//if (g < 0.0f) g = 0.0f;
				//if (b < 0.0f) b = 0.0f;
				
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
	//SaveImageToFile("oo2.bmp", img);
	//SaveImageToFile("yestsav.bmp", img);
	
	return 0;
}