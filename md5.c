#include <stdio.h>
#include <stdlib.h>

#include "image.h"

Pixel Sample(Pixel pix1, Pixel pix2, int mod) {
	switch (mod) {
	case 0:
		return pix1;
	case 1:
		Pixel a = {(pix2.r/4)+((pix1.r/4)*3),(pix2.g/4)+((pix1.g/4)*3),(pix2.b/4)+((pix1.b/4)*3)};
		return a;
	case 2:
		Pixel b = {(pix2.r/2)+(pix1.r/2),(pix2.g/2)+(pix1.g/2),(pix2.b/2)+(pix1.b/2)};
		return b;
	case 3:
		Pixel c = {(pix1.r/4)+((pix2.r/4)*3),(pix1.g/4)+((pix2.g/4)*3),(pix1.b/4)+((pix2.b/4)*3)};
		return c;
		default: abort();
	}
	
	
}

int main(int argc, const char** argv) {
	if (argc != 3) {
		printf("Usage: md5 input output\n");
		printf("  Where 'input' is path to input file and 'output' is path ");
		printf("to output file.\n");
		printf("  Input file must be a 24-bit .bmp file with no compression.\n");
		return -1;
	}
	
	const char* input = argv[1];
	const char* output = argv[2];
	
	Image img = LoadImageFromFile(input);
	Image egg = NewImage(img.width * 4, img.height * 4);
	
	for (int y = 0; y < egg.height; y++) {
		for (int x = 0; x < egg.width; x++) {
			int bck_x = x/4;
			int bck_y = y/4;
			int fwd_x = bck_x + 1;
			int fwd_y = bck_y + 1;

			// šādi apcērpjot vērtības attēla kreisā un apakšējā mala neizskatīsies
			// ļoti smuki, bet vismaz pārējais attēls nebūs izplūdis
			if (fwd_x >= img.width) fwd_x = bck_x;
			if (fwd_y >= img.height) fwd_y = bck_y;
			
			// izvēlamies pirmās divas lineārās vērtības
			Pixel x_val_bck = Sample(GetPixel(img, bck_x, bck_y), GetPixel(img, fwd_x, bck_y), x % 4);
			Pixel x_val_fwd = Sample(GetPixel(img, bck_x, fwd_y), GetPixel(img, fwd_x, fwd_y), x % 4);
			
			// aprēķinam gala vērtību no tām
			Pixel val_final = Sample(x_val_bck, x_val_fwd, y % 4);
			
			SetPixel(egg, val_final, x, y);
		}
	}
	
	
	SaveImageToFile(output, egg);
}