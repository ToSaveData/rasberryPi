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
	
	if(argc != 2){
		printf("Usage : ./ %s xxx.bmp \n", argv[0]);
		return -1;
		}
	if((fp = fopen(argv[1], "rb")) == NULL){
		perror("ERROR\n");
		return -1;
		}

	fread(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

	elemsize = bmpInfoHeader.biBitCount/8;
	width = bmpInfoHeader.biWidth*elemsize;

	pos = (ubyte*)malloc(sizeof(ubyte*) * bmpInfoHeader.biHeight * bmpInfoHeader.biWidth * elemsize);
	inimg = (ubyte*)malloc(sizeof(ubyte*) * bmpInfoHeader.biHeight * bmpInfoHeader.biWidth * elemsize);

	if(!bmpInfoHeader.SizeImage){
		bmpInfoHeader.SizeImage = bmpInfoHeader.biHeight * bmpInfoHeader.biWidth * elemsize;
		}
	fread(inimg, sizeof(ubyte) * bmpInfoHeader.SizeImage, 1, fp);
	
	for(int i = 0; i < bmpInfoHeader.biHeight; i++){
		for(int j = 0; j < bmpInfoHeader.biWidth-1; j++){
			printf("(");
			for(int k = 0; k < elemsize; k++){
				pos[i*width + j*elemsize + k] = inimg[(bmpInfoHeader.biHeight-i)*width -((j+1)*elemsize) + k];
				printf("%d, ", pos[i*width + j*elemsize + k]);
				}
				printf("\b\b)\n");
			}
		}
	
	fclose(fp);
	free(inimg);
	free(pos);

	return 0;
}
