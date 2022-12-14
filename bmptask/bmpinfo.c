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

//	printf("%d\n", bmpInfoHeader.biBitCount);
	elemsize = bmpInfoHeader.biBitCount/8;
//	printf("%d\n", elemsize);
	width = bmpInfoHeader.biWidth*elemsize;

	pos = (ubyte*)malloc(sizeof(ubyte*) * bmpInfoHeader.biHeight * bmpInfoHeader.biWidth * elemsize);
	inimg = (ubyte*)malloc(sizeof(ubyte*) * bmpInfoHeader.biHeight * bmpInfoHeader.biWidth * elemsize);

	if(!bmpInfoHeader.SizeImage){
		bmpInfoHeader.SizeImage = bmpInfoHeader.biHeight * bmpInfoHeader.biWidth * elemsize;
		}
//	printf("%d %d %d %d \n", bmpInfoHeader.SizeImage, bmpInfoHeader.biHeight, bmpInfoHeader.biWidth, (bmpInfoHeader.biHeight)*width -((1)*elemsize) );
	fread(inimg, sizeof(ubyte) * bmpInfoHeader.SizeImage, 1, fp);
	
	for(int i = 0; i < bmpInfoHeader.biHeight -1; i++){
		for(int j = 0; j < bmpInfoHeader.biWidth-1; j++){
			for(int k = 0; k < elemsize; k++){
//				printf("%d\n", i*width + j*elemsize + k);
//				printf("%d\n", (bmpInfoHeader.biHeight-i)*width -((j+1)*elemsize) + k);
				pos[i*width + j*elemsize + k] = inimg[(bmpInfoHeader.biHeight-i-1)*width -((j+1)*elemsize) + k];
				printf("%d ", pos[i*width + j*elemsize + k]);
				}
				printf("\n");
			}
		}
	
	fclose(fp);
	free(inimg);
	free(pos);

	return 0;
}
