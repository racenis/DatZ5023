#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "image.h"

Pixel GetPixel(Image img, int x, int y) {
	return img.data[img.width * y + x];
}

Pixel GetClampedPixel(Image img, int x, int y) {
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	
	if (x >= img.width) x = img.width - 1;
	if (y >= img.height) y = img.height - 1;
	
	return GetPixel(img, x, y);
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

Image ResizeImage(Image img, int width, int height) {
	Image res = NewImage(width, height);
	
	for (int y = 0; y < res.height; y++) {
		for (int x = 0; x < res.width; x++) {
			if (x < img.width && y < img.height) {
				res.data[res.width * y + x] = img.data[img.width * y + x];
			} else {
				res.data[res.width * y + x] = COLOR_BLACK;
			}
		}
	}
	
	return res;
}

Image NewImage(int width, int height) {
	Image img;
	
	img.width = width;
	img.height = height;
	img.data = malloc(width * height * sizeof(Pixel));
	
	return img;
}

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

typedef struct BMPPixel {
	unsigned char b;
	unsigned char g;
	unsigned char r;
} BMPPixel;

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
	
	image.width = header.width;
	image.height = header.height;
	
	char* bmp_data = malloc(header.image_size);
	fread(bmp_data, sizeof(char), header.image_size, file);
	
	int image_data_size = image.width * image.height * sizeof(Pixel);
	image.data = malloc(image_data_size);
	
	fclose(file);
	
	// katras pikseļu rindiņas beigās ir nulles lai nolīdzinātu datus ar 4 baitu
	// robežu. es pieņemu ka tas ir saistīts kaut kādā veidā ar analogā signāla
	// ģenerēšanu monitoram.
	int scln_mod = (image.width * 3) % 4;
	int scln_padding = scln_mod == 0 ? 0 : 4 - scln_mod;
	int scln_size = (image.width * 3) + scln_padding;
	
	for (int y = 0; y < image.height; y++) {
		int scln_inv = image.height - y - 1;
		BMPPixel* scln = (BMPPixel*)&bmp_data[scln_inv * scln_size];
		Pixel* imgln = &image.data[y * image.width];
		
		// BMP formāts krāsas glabā kā BGR, nevis RGB
		for (int x = 0; x < image.width; x++) {
			imgln[x].r = scln[x].r;
			imgln[x].g = scln[x].g;
			imgln[x].b = scln[x].b;
		}
	}
	
	free(bmp_data);

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