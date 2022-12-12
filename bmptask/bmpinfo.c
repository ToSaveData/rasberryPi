#include "bmpHeader.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef unsigned char ubyte;

int main(int argc, char **argv)
{
	FILE *fp;
	int elemsize;
	int width;
	ubyte *pos;
	ubyte *inimg;

	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	RGBQUAD pal;

	if(argc != 2){
		printf("Usage : ./%s xxx.bmp\n", argv[0]);
		return -1;
		}

	fp = fopen(argv[1], "rb");

	if(fp == NULL){
		perror("ERROR\n");
		return -1;
		}

	fread(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	//fread(&pal, sizeof(RGBQUAD), 1, fp);

	elemsize = bmpInfoHeader.biBitCount/8;
	pos = (ubyte*)malloc(sizeof(ubyte*) * bmpInfoHeader.biHeight * bmpInfoHeader.biWidth * elemsize);
	inimg = (ubyte*)malloc(sizeof(ubyte*) * bmpInfoHeader.biHeight * bmpInfoHeader.biWidth * elemsize);
	width = bmpInfoHeader.biWidth*elemsize;

	for(int i = 0; i < bmpInfoHeader.biHeight -1; i++){
		for(int j = 0; j < bmpInfoHeader.biWidth*3; j++){
			for(int k = 0; k < elemsize; k++){
				printf("(%s, %s, %s)\n", pos[, g, b);
				}
			}
		}
				
	printf("(%s, %s, %s)\n", r, g, b);
	
	fclose(fp);
	free(fp);

	return 0;
}
