default: md1
	md1 kakis.bmp kakis2.bmp

md1: md1.c image.c image.h
	gcc md1.c image.c -o md1