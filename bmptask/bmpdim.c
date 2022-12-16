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
	ubyte *inimg, *outimg;

	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader;
	RGBQUAD *pal;
	
	if(argc != 3){
		printf("Usage : ./%s %s out.bmp \n", argv[0], argv[1]);
		return -1;
		}

	if((fp = fopen(argv[1], "rb")) == NULL){
		perror("ERROR 1\n");
		return -1;
		}

	fread(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

	elemsize = bmpInfoHeader.biBitCount/8;
	width = bmpInfoHeader.biWidth*elemsize;

	inimg = (ubyte*)malloc(sizeof(ubyte) * bmpInfoHeader.biHeight * width);
	outimg = (ubyte*)malloc(sizeof(ubyte) * bmpInfoHeader.biHeight * width);

	pal = (RGBQUAD*)malloc(sizeof(RGBQUAD)*256);
	for(int i = 0; i < 256; i++){
		pal[i].rgbBlue = i;
		pal[i].rgbGreen = i;
		pal[i].rgbRed = i;
	}

	if(!bmpInfoHeader.SizeImage){
		bmpInfoHeader.SizeImage = bmpInfoHeader.biHeight * bmpInfoHeader.biWidth * elemsize;
		}
		
	fread(inimg, sizeof(ubyte) * bmpInfoHeader.SizeImage, 1, fp);
#if 0
	for(int i = 0; i < bmpInfoHeader.biHeight; i++){
		for(int j = 0; j < bmpInfoHeader.biWidth; j++){
			for(int k = 0; k < elemsize; k++){
				outimg[(bmpInfoHeader.biHeight-i)*width -((j+1)*elemsize) + k] = inimg[(bmpInfoHeader.biHeight-i-1)*width -((j+1)*elemsize) + k];
				}
			}
		}
#else
	for(int i = 0; i < bmpInfoHeader.biHeight*bmpInfoHeader.biWidth*elemsize ; i++){
			//outimg[i] = inimg[(bmpInfoHeader.biHeight)*width-i-1];
			outimg[i] = inimg[i];
		}

#endif
	
	fclose(fp);

	if((fp = fopen(argv[2], "wb")) == NULL){
		perror("ERROR 2\n");
		return -1;
		}

	bmpFileHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256 + bmpInfoHeader.SizeImage;
	bmpFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD)*256;

	fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(pal, sizeof(RGBQUAD)*256, 1, fp);
	fwrite(outimg, sizeof(ubyte)*bmpInfoHeader.SizeImage, 1, fp);

	free(inimg);
	free(outimg);
	free(pal);
	fclose(fp);

	return 0;
}
