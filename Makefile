default: md2
	md2

md1: md1.c image.c image.h
	gcc md1.c image.c -o md1
	
md2: md2.c image.c image.h
	gcc md2.c image.c -o md2