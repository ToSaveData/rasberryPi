#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

#define BYTE unsigned char

#define widthbytes(bits) (((bits)+31)/32*4)

typedef struct tagRGBQUAD{
	BYTE rgbBlue; 
	BYTE rgbGreen; 
	BYTE rgbRed; 
	BYTE rgbReserved;
} RGBQUAD;

int main(int argc, char** argv) {
	FILE* fp; 
	RGBQUAD palrgb[256];
	
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

	char input[128], output[128];
	
	int i, j, size; 
	int xFactor = 2, yFactor = 2; 
	float srcX, srcY;
	int index; 
	float r,g,b,gray;
	int graysize; 
	int index2;
	
	unsigned char *grayimg, *inimg, *outimg;
	
	/* usage a.out in.bmp out.bmp */
	strcpy(input, argv[1]); 
	strcpy(output, argv[2]);
	
	
	if((fp=fopen(input, "rb"))==NULL) { 
		fprintf(stderr, "Error : Failed to open file...₩n"); 
		return -1;
	}
	
	fread(&type, sizeof(unsigned short int), 1, fp); 
	fread(&file_size, sizeof(unsigned int), 1, fp); 
	fread(&reserved1, sizeof(unsigned short int), 1, fp); 
	fread(&reserved2, sizeof(unsigned short int), 1, fp); 
	fread(&offset, sizeof(unsigned int), 1, fp);
	
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
	
	size=widthbytes(bits * width); 
	graysize = widthbytes(8 * width);
	
	if(!imagesize) 
		imagesize=height*size;
	
	inimg=(BYTE*)malloc(sizeof(BYTE)*imagesize); 
	outimg=(BYTE*)malloc(sizeof(BYTE)*imagesize*xFactor*yFactor); 
	fread(inimg, sizeof(BYTE), imagesize, fp); 
	
	fclose(fp);
	
	for(i=0; i<=height*3; i+=3) { 
		for(j=0; j<=width*3; j+=3) {
#if 0
			outimg[(j*xFactor)+(width*xFactor*i*yFactor)]=inimg[j+(i*width)];
			outimg[(j*xFactor)+(width*xFactor*i*yFactor)+1]=inimg[j+(i*width)+1];
			outimg[(j*xFactor)+(width*xFactor*i*yFactor)+2]=inimg[j+(i*width)+2];
			//0,0,r
			outimg[(j*xFactor)+(width*xFactor*i*yFactor)]=inimg[j+(i*width)];
			//0,1,r
			outimg[(j*xFactor+3)+(width*xFactor*i*yFactor)]=(inimg[j+(i*width)]+inimg[j+(i*width)+3])/2;
			//0,0,g
			outimg[(j*xFactor)+(width*xFactor*i*yFactor)+1]=inimg[j+(i*width)+1];
			//0,1,g
			outimg[(j*xFactor+3)+(width*xFactor*i*yFactor)+1]=(inimg[j+(i*width)+1]+inimg[j+(i*width)+1+3])/2;
			//0,0,b
			outimg[(j*xFactor)+(width*xFactor*i*yFactor)+2]=inimg[j+(i*width)+2];
			//0,1,b
			outimg[(j*xFactor+3)+(width*xFactor*i*yFactor)+2]=(inimg[j+(i*width)+2]+inimg[j+(i*width)+2+3])/2;

			//1,0,r
			outimg[(j*xFactor)+(width*xFactor*i*yFactor)+(width*xFactor*yFactor*3)]=(inimg[j+(i*width)]+inimg[j+(i*width)+width])/2;
			//1,0,g
			outimg[(j*xFactor)+(width*xFactor*i*yFactor)+(width*xFactor*yFactor*3)+1]=(inimg[j+(i*width)+1]+inimg[j+(i*width)+width+1])/2;
			//1,0,b
			outimg[(j*xFactor)+(width*xFactor*i*yFactor)+(width*xFactor*yFactor*3)+2]=(inimg[j+(i*width)+2]+inimg[j+(i*width)+width+2])/2;
	
			//1,1,r
			outimg[(j*xFactor+3)+(width*xFactor*i*yFactor)+(width*xFactor*yFactor*3)]=(inimg[j+(i*width)]+inimg[j+(i*width)+3]+inimg[j+(i*width)+width]+inimg[j+(i*width)+3+width])/4;
			//1,1,g
			outimg[(j*xFactor+3)+(width*xFactor*i*yFactor)+(width*xFactor*yFactor*3)+1]=(inimg[j+(i*width)+1]+inimg[j+(i*width)+1+3]+inimg[j+(i*width)+width+1]+inimg[j+(i*width)+3+width+1])/4;
			//1,1,b
			outimg[(j*xFactor+3)+(width*xFactor*i*yFactor)+(width*xFactor*yFactor*3)+2]=(inimg[j+(i*width)+2]+inimg[j+(i*width)+3+2]+inimg[j+(i*width)+width+2]+inimg[j+(i*width)+3+width+2])/4;
#else
			//0,0,r
			outimg[(j*xFactor)+(width*xFactor*i*yFactor)]=inimg[j+(i*width)];
			//0,1,r
			outimg[(j*xFactor+3)+(width*xFactor*i*yFactor)]=(inimg[j+(i*width)]+inimg[j+(i*width)+3])/2;
			//0,0,g
			outimg[(j*xFactor)+(width*xFactor*i*yFactor)+1]=inimg[j+(i*width)+1];
			//0,1,g
			outimg[(j*xFactor+3)+(width*xFactor*i*yFactor)+1]=(inimg[j+(i*width)+1]+inimg[j+(i*width)+1+3])/2;
			//0,0,b
			outimg[(j*xFactor)+(width*xFactor*i*yFactor)+2]=inimg[j+(i*width)+2];
			//0,1,b
			outimg[(j*xFactor+3)+(width*xFactor*i*yFactor)+2]=(inimg[j+(i*width)+2]+inimg[j+(i*width)+2+3])/2;
if(i <height*3){
			//1,0,r
			outimg[(j*xFactor)+(width*xFactor*i*yFactor)+(width*xFactor*yFactor)]=(inimg[j+(i*width)]+inimg[j+(i*width)+width])/2;
			//1,0,g
			outimg[(j*xFactor)+(width*xFactor*i*yFactor)+(width*xFactor*yFactor)+1]=(inimg[j+(i*width)+1]+inimg[j+(i*width)+width+1])/2;
			//1,0,b
			outimg[(j*xFactor)+(width*xFactor*i*yFactor)+(width*xFactor*yFactor)+2]=(inimg[j+(i*width)+2]+inimg[j+(i*width)+width+2])/2;
			//1,1,r
			outimg[(j*xFactor+3)+(width*xFactor*i*yFactor)+(width*xFactor*yFactor)]=(inimg[j+(i*width)]+inimg[j+(i*width)+3]+inimg[j+(i*width)+width]+inimg[j+(i*width)+3+width])/4;
			//1,1,g
			outimg[(j*xFactor+3)+(width*xFactor*i*yFactor)+(width*xFactor*yFactor)+1]=(inimg[j+(i*width)+1]+inimg[j+(i*width)+1+3]+inimg[j+(i*width)+width+1]+inimg[j+(i*width)+3+width+1])/4;
			//1,1,b
			outimg[(j*xFactor+3)+(width*xFactor*i*yFactor)+(width*xFactor*yFactor)+2]=(inimg[j+(i*width)+2]+inimg[j+(i*width)+3+2]+inimg[j+(i*width)+width+2]+inimg[j+(i*width)+3+width+2])/4;
}
#endif
		};
	};	  
	
	width*=xFactor, height*=yFactor; 
	size=widthbytes(bits*width); 
	imagesize=height*size; 
	offset+=256*sizeof(RGBQUAD);
	
	if((fp=fopen(output, "wb"))==NULL) { 
		fprintf(stderr, "Error : Failed to open file...₩n"); 
		return -1;
	}
	
	fwrite(&type, sizeof(unsigned short int), 1, fp); 
	fwrite(&file_size, sizeof(unsigned int), 1, fp); 
	fwrite(&reserved1, sizeof(unsigned short int), 1, fp); 
	fwrite(&reserved2, sizeof(unsigned short int), 1, fp); 
	fwrite(&offset, sizeof(unsigned int), 1, fp); 
	
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
	
	fwrite(outimg, sizeof(unsigned char), imagesize, fp);
	
	free(inimg); 
	free(outimg);
	
	fclose(fp); 
	
	return 0;
}