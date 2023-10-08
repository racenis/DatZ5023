default: md3
	md3

md1: md1.c image.c image.h
	gcc md1.c image.c -o md1
	
md2: md2.c image.c image.h
	gcc md2.c image.c -o md2
	
md3: md3.c fft.c fft.h
	gcc md3.c fft.c -o md3