#ifndef IMAGE_H
#define IMAGE_H 

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

Pixel GetPixel(Image img, int x, int y);
void SetPixel(Image img, Pixel pix, int x, int y);
void FillImage(Image img, Pixel pix);
Image ResizeImage(Image img, int width, int height);
Image NewImage(int width, int height);
Image LoadImageFromFile(const char* path);
void SaveImageToFile(const char* path, Image image);

static const Pixel COLOR_WHITE = {255, 255, 255};
static const Pixel COLOR_BLACK = {0, 0, 0};

#endif // IMAGE_H