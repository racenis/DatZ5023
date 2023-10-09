default: md1 md2 md3 md5 md6 md8 md9 md10

clean:
	del md1.exe
	del md2.exe
	del md3.exe
	del md5.exe
	del md6.exe
	del md8.exe
	del md9.exe
	del md10.exe

md1: md1.c image.c image.h
	gcc md1.c image.c -o md1 -static -static-libgcc
	
md2: md2.c image.c image.h kernel.c kernel.h
	gcc md2.c image.c kernel.c -o md2 -static -static-libgcc
	
md3: md3.c image.c image.h fft.c fft.h
	gcc md3.c image.c fft.c -o md3 -static -static-libgcc
	
md5: md5.c image.c image.h
	gcc md5.c image.c -o md5 -static -static-libgcc

md6: md6.c image.c image.h fwt.c fwt.h
	gcc md6.c image.c fwt.c -o md6 -static -static-libgcc
	
md8: md8.c image.c image.h 
	gcc md8.c image.c -o md8 -static -static-libgcc
	
md9: md9.c image.c image.h 
	gcc md9.c image.c -o md9 -static -static-libgcc

md10: md10.c image.c image.h kernel.c kernel.h
	gcc md10.c image.c kernel.c -o md10 -static -static-libgcc