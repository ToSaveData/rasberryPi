#ifndef __BMP_FILE_H__
#define __BMP_FILE_H__

typedef struct __attribute__((__packed__)){
	unsigned short type;
	unsigned int file_size;
	unsigned short int reserved1;
	unsigned short int reserved2;
	unsigned int offset;
} BITMAPFILEHEADER;

typedef struct{
	unsigned int header_size;
	unsigned int width;
	unsigned int height;
	unsigned short planes;
	unsigned short bits;
	unsigned int compression;
	unsigned int imagesize;
	unsigned int biXPelsPerMeter;
	unsigned int biYPelsPerMeter; 	
	unsigned int biClrUsed; 
	unsigned int biClrImportant;
	int hresolution, vresolution;
	unsigned int ncolors, importantcolors;
} BITMAPINFOHEADER; 

typedef struct{
	unsigned char rgbBlue;
	unsigned char rgbGreen;
	unsigned char rgbRed;
	unsigned char rgbReserved;
} RGBQUAD;

#endif /* __BMP_FILE_H__*/
