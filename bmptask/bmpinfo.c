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
		printf("Usage : ./ %s xxx.bmp \n", argv[0]);
		return -1;
		}
	if((fp = fopen(argv[1], "rb")) == NULL){
		perror("ERROR\n");
		return -1;
		}
	pos = (ubyte*)malloc(sizeof(ubyte*) * bmpInfoHeader.biHeight * bmpInfoHeader.biWidth * elemsize);
	inimg = (ubyte*)malloc(sizeof(ubyte*) * bmpInfoHeader.biHeight * bmpInfoHeader.biWidth * elemsize);
	fread(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(&inimg, sizeof(ubyte) * bmpInfoHeader.SizeImage, 1, fp);

	elemsize = bmpInfoHeader.biBitCount/8;
	width = bmpInfoHeader.biWidth*elemsize;

	for(int i = 0; i < bmpInfoHeader.biHeight -1; i++){
		for(int j = 0; j < bmpInfoHeader.biWidth*elemsize; j++){
			for(int k = 0; k < elemsize; k++){
				pos[i*width + j*bmpInfoHeader.biHeight + k] = inimg[(bmpInfoHeader.biHeight-i)*width -((bmpInfoHeader.biWidth-j-1)*elemsize) + k];
				//printf("(%s ", pos[i*width + j*bmpInfoHeader.biHeight + k]);
				return 0;
				}
				printf("\n");
				return 0;
			}
		}
	
	free(fp);
	free(inimg);

	fclose(fp);
	return 0;
}
