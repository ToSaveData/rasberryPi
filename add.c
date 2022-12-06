#include "add.h"

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>

#define BYTE unsigned char

#define widthbytes(bits) (((bits)+31)/32*4)
/*
typedef struct tagRGBQUAD{
	BYTE rgbBlue; 
	BYTE rgbGreen; 
	BYTE rgbRed; 
	BYTE rgbReserved;
};
*/
unsigned char clip(int value, int min, int max);

inline unsigned char clip(int value, int min, int max)
{
    return(value > max? max : value < min? min : value);
}

int main(int argc, char** argv) {
	FILE* fp; 
//	struct tagRGBQUAD palrgb[256];
#if 1
	BITMAPFILEHEADER bmpFileHeader;
	BITMAPINFOHEADER bmpInfoHeader; 
#else
	/* BMP FILE INFO */
	unsigned short int type; 
	unsigned int file_size; 
	unsigned short int reserved1; 
	unsigned short int reserved2; 
	unsigned int offset;
	
	/* BMP IMAGE INFO */
	unsigned int header_size; 
	int width, height; 
	unsigned short int planes; 
	unsigned short int bits; 
	unsigned int compression; 
	unsigned int imagesize; 
	int hresolution, vresolution; 
	unsigned int ncolors, importantcolors; 
#endif
	char input[128], output[128];
	
	int i, j, size; 
	float srcX, srcY;
	int index; 
	float r,g,b,gray;
	int graysize; 
	int index2;
	
	unsigned char *grayimg, *inimg, *outimg;
	
	/* usage a.out in.bmp out.bmp */
	strcpy(input, argv[1]); 
	strcpy(output, argv[2]);
	
	if((fp=fopen(input, "rb")) == NULL) { 
		fprintf(stderr, "Error : Failed to open file...₩n"); 
		return -1;
	}

#if 1
	printf("%s, %s, %s", __FILE__, __LINE__, __FUNCTION__);	
	fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp); 
#else	
	fread(&header_size, sizeof(unsigned int), 1, fp); 
	fread(&width, sizeof(int), 1, fp);
	fread(&height, sizeof(int), 1, fp);	
	fread(&planes, sizeof(unsigned short int), 1, fp); 
	fread(&bits, sizeof(unsigned short int), 1, fp); 
	fread(&compression, sizeof(unsigned int), 1, fp); 
	fread(&imagesize, sizeof(unsigned int), 1, fp); 
	fread(&hresolution, sizeof(int), 1, fp); 
	fread(&vresolution, sizeof(int), 1, fp); 
	fread(&ncolors, sizeof(unsigned int), 1, fp); 
	fread(&importantcolors, sizeof(unsigned int), 1, fp);
#endif	

	fread(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);

	size=widthbytes(bmpInfoHeader.bits * bmpInfoHeader.width); 
	graysize = widthbytes(8 * bmpInfoHeader.width);
	
	if(!bmpInfoHeader.imagesize) 
		bmpInfoHeader.imagesize=bmpInfoHeader.height*size;
	
	inimg=(BYTE*)malloc(sizeof(BYTE)*bmpInfoHeader.imagesize); 
	outimg=(BYTE*)malloc(sizeof(BYTE)*bmpInfoHeader.imagesize); 
	fread(inimg, sizeof(BYTE), bmpInfoHeader.imagesize, fp); 
	
	fclose(fp);
	
	if((fp=fopen(output, "wb"))==NULL) { 
		fprintf(stderr, "Error : Failed to open file...₩n"); 
		return -1;
	}

	for(i=0; i<bmpInfoHeader.height*3; i+=3) { 
		for(j=0; j<bmpInfoHeader.width*3; j+=3) {
			int b = inimg[j+(i*bmpInfoHeader.width+0)]; 
			int g = inimg[j+(i*bmpInfoHeader.width+1)]; 
			int r = inimg[j+(i*bmpInfoHeader.width+2)]; 
			
			b += 50;
			g += 50;
			r += 50; 
			
			outimg[j+bmpInfoHeader.width*i+0]= clip(b, 0, 255);
			outimg[j+bmpInfoHeader.width*i+1]= clip(g, 0, 255);
			outimg[j+bmpInfoHeader.width*i+2]= clip(r, 0, 255);
		};
	 };	   
	 
//	size=widthbytes(bits*bmpInfoHeader.width); 
//	imagesize=bmpInfoHeader.height*size; 
//	offset+=256*sizeof(RGBQUAD);
	
#if 1
	fwrite(&bmpFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
#else 
	fwrite(&type, sizeof(unsigned short int), 1, fp); 
	fwrite(&file_size, sizeof(unsigned int), 1, fp); 
	fwrite(&reserved1, sizeof(unsigned short int), 1, fp); 
	fwrite(&reserved2, sizeof(unsigned short int), 1, fp); 
	fwrite(&offset, sizeof(unsigned int), 1, fp); 
#endif	

#if 1
	fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
#else 
	fwrite(&header_size, sizeof(unsigned int), 1, fp); 
	fwrite(&width, sizeof(int), 1, fp);
	fwrite(&height, sizeof(int), 1, fp);
	fwrite(&planes, sizeof(unsigned short int), 1, fp); 
	fwrite(&bits, sizeof(unsigned short int), 1, fp);		
	fwrite(&compression, sizeof(unsigned int), 1, fp); 
	fwrite(&imagesize, sizeof(unsigned int), 1, fp); 
	fwrite(&hresolution, sizeof(int), 1, fp); 
	fwrite(&vresolution, sizeof(int), 1, fp); 
	fwrite(&ncolors, sizeof(unsigned int), 1, fp); 
	fwrite(&importantcolors, sizeof(unsigned int), 1, fp);
	fwrite(palrgb, sizeof(unsigned int), 256, fp); 
#endif
	
	fwrite(outimg, sizeof(unsigned char), bmpInfoHeader.imagesize, fp);
	
	free(inimg); 
	free(outimg);
	
	fclose(fp); 
	
	return 0;
}
