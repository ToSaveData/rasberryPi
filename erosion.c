#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <limits.h>                     /* USHRT_MAX 상수를 위해서 사용한다. */
#include <unistd.h>
#include <time.h>

#include "bmpHeader.h"

/* 이미지 데이터의 경계 검사를 위한 매크로 */
#define LIMIT_UBYTE(n) ((n)>UCHAR_MAX)?UCHAR_MAX:((n)<0)?0:(n)

typedef unsigned char ubyte;

unsigned char clip(int value, int min, int max)
{
    return(value > max? max : value < min? min : value);
}

void quickSort(int arr[], int L, int R) {
      int left = L, right = R;
      int pivot = arr[(L + R) / 2];    // pivot 설정 (가운데) 
      int temp;
      do
      {
        while (arr[left] < pivot)    // left가 pivot보다 큰 값을 만나거나 pivot을 만날 때까지 
            left++;
        while (arr[right] > pivot)    // right가 pivot보다 작은 값을 만나거나 pivot을 만날 때까지 
            right--;
        if (left<= right)    // left가 right보다 왼쪽에 있다면 교환 
        {
            temp = arr[left];
            arr[left] = arr[right];
            arr[right] = temp;
            /*left 오른쪽으로 한칸, right 왼쪽으로 한칸 이동*/
            left++;
            right--;
        }
      } while (left<= right);    // left가 right 보다 오른쪽에 있을 때까지 반복 
 
    /* recursion */
    if (L < right)
        quickSort(arr, L, right);    // 왼쪽 배열 재귀적으로 반복 
 
    if (left < R)
        quickSort(arr, left, R);    // 오른쪽 배열 재귀적으로 반복 
}

int main(int argc, char** argv) 
{
    FILE* fp; 
    BITMAPFILEHEADER bmpHeader;             /* BMP FILE INFO */
    BITMAPINFOHEADER bmpInfoHeader;     /* BMP IMAGE INFO */
    RGBQUAD *palrgb;
    ubyte *inimg, *outimg;
    int x, y, z, imageSize;
    ubyte* paddingSize;

    if(argc != 3) {
        fprintf(stderr, "usage : %s input.bmp output.bmp\n", argv[0]);
        return -1;
    }
    
    /***** read bmp *****/ 
    if((fp=fopen(argv[1], "rb")) == NULL) { 
        fprintf(stderr, "Error : Failed to open file...₩n"); 
        return -1;
    }

    /* BITMAPFILEHEADER 구조체의 데이터 */
    fread(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);

    /* BITMAPINFOHEADER 구조체의 데이터 */
    fread(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

    /* 트루 컬러를 지원하면 변환할 수 없다. */
    if(bmpInfoHeader.biBitCount != 24) {
        perror("This image file doesn't supports 24bit color\n");
        fclose(fp);
        return -1;
    }
    
    int elemSize = bmpInfoHeader.biBitCount/8;
    int size = bmpInfoHeader.biWidth*elemSize;
    imageSize = size * bmpInfoHeader.biHeight; 

    /* 이미지의 해상도(넓이 × 깊이) */
    printf("Resolution : %d x %d\n", bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);
    printf("Bit Count : %d\n", bmpInfoHeader.biBitCount);     /* 픽셀당 비트 수(색상) */
    printf("Image Size : %d\n", imageSize);

    inimg = (ubyte*)malloc(sizeof(ubyte)*imageSize);
    paddingSize = (ubyte*)malloc(sizeof(ubyte)*imageSize + (bmpInfoHeader.biWidth + bmpInfoHeader.biHeight + 2) * 2); 
    outimg = (ubyte*)malloc(sizeof(ubyte)*imageSize);
    fread(inimg, sizeof(ubyte), imageSize, fp);

    memset(paddingSize, 0, sizeof(ubyte)*imageSize + (bmpInfoHeader.biWidth + bmpInfoHeader.biHeight + 2) * 2);
    for(int a = 0; a < bmpInfoHeader.biHeight + 1; a++){
	for(int b = 0; b < bmpInfoHeader.biWidth + 2; b++){
	    if(a == 0){
#if 1
	    paddingSize[elemSize + size*a + elemSize*b] = inimg[elemSize+(a+1)*size+b*elemSize];
	    paddingSize[elemSize + size*a + elemSize*b + 1] = inimg[elemSize+(a+1)*size+b*elemSize + 1];
	    paddingSize[elemSize + size*a + elemSize*b + 2] = inimg[elemSize+(a+1)*size+b*elemSize + 2];
#else
	    paddingSize[elemSize + size*a + elemSize*b] = 255;
	    paddingSize[elemSize + size*a + elemSize*b + 1] = 255;
	    paddingSize[elemSize + size*a + elemSize*b + 2] = 255;
#endif 
	    }
	    if(a == bmpInfoHeader.biHeight){
#if 1
	    paddingSize[elemSize + size*a + elemSize*b] = inimg[elemSize+(a-1)*size+b*elemSize];
	    paddingSize[elemSize + size*a + elemSize*b + 1] = inimg[elemSize+(a-1)*size+b*elemSize + 1];
	    paddingSize[elemSize + size*a + elemSize*b + 2] = inimg[elemSize+(a-1)*size+b*elemSize + 2];
#else
	    paddingSize[elemSize + size*a + elemSize*b] = 255;
	    paddingSize[elemSize + size*a + elemSize*b + 1] = 255;
	    paddingSize[elemSize + size*a + elemSize*b + 2] = 255;
#endif	    
	    }
	    else if(b == 0){
#if 1
	    paddingSize[elemSize + size*a + elemSize*b] = inimg[elemSize+(a-1)*size+(b+1)*elemSize];
	    paddingSize[elemSize + size*a + elemSize*b + 1] = inimg[elemSize+(a-1)*size+(b+1)*elemSize + 1];
	    paddingSize[elemSize + size*a + elemSize*b + 2] = inimg[elemSize+(a-1)*size+(b+1)*elemSize + 2];
#else
	    paddingSize[elemSize + size*a + elemSize*b] = 255;
	    paddingSize[elemSize + size*a + elemSize*b + 1] = 255;
	    paddingSize[elemSize + size*a + elemSize*b + 2] = 255;
#endif	    
	    }
	    else if(b == bmpInfoHeader.biWidth + 1){
#if 1
	    paddingSize[elemSize + size*a + elemSize*b] = inimg[elemSize+(a-1)*size+(b-1)*elemSize];
	    paddingSize[elemSize + size*a + elemSize*b + 1] = inimg[elemSize+(a-1)*size+(b-1)*elemSize + 1];
	    paddingSize[elemSize + size*a + elemSize*b + 2] = inimg[elemSize+(a-1)*size+(b-1)*elemSize + 2];
#else
	    paddingSize[elemSize + size*a + elemSize*b] = 255;
	    paddingSize[elemSize + size*a + elemSize*b + 1] = 255;
	    paddingSize[elemSize + size*a + elemSize*b + 2] = 255;
#endif	    
	    }
	    else{
	    paddingSize[elemSize + size*a + elemSize*b] = inimg[elemSize+a*size+b*elemSize];
	    paddingSize[elemSize + size*a + elemSize*b + 1] = inimg[elemSize+a*size+b*elemSize + 1];
	    paddingSize[elemSize + size*a + elemSize*b + 2] = inimg[elemSize+a*size+b*elemSize + 2];
  	    }
	}
    }
    
    fclose(fp);

    // define the kernel
    float kernel[3][3] = { {0.0, -1.0, 0.0},
                           {-1.0, 5.0, -1.0},
                           {0.0, -1.0, 0.0} };
    memset(outimg, 0, sizeof(ubyte)*imageSize);
#if 0
    for(y = 1; y < bmpInfoHeader.biHeight - 1; y++) {
        for(x = 1; x < (bmpInfoHeader.biWidth - 1) * elemSize; x+=elemSize) {
            for(z = 0; z < elemSize; z++) {
                int arr[9], cnt = 0; 
		float sum = 0.0;
                for(int i = -1; i < 2; i++) {
                    for(int j = -1; j < 2; j++) {
//                        sum += kernel[i+1][j+1]*paddingSize[(x-i*elemSize)+(y-j)*size+z];
			arr[cnt++] = paddingSize[(x-i*elemSize)+(y-j)*size+z];
                    }
                }
		
                outimg[x+y*size+z] = clip(sum, 0, 255);
            }
        }
    }
#else
    for(y = 1; y < bmpInfoHeader.biHeight - 1; y++) {
        for(x = elemSize; x < bmpInfoHeader.biWidth * elemSize; x+=elemSize) {
            for(z = 0; z < elemSize; z++) {
                int arr[9], cnt = 0; 
                float sum = 0.0;
                for(int i = -1; i < 2; i++) {
                    for(int j = -1; j < 2; j++) {
                        //sum += kernel[i+1][j+1]*paddingSize[(x+i*elemSize)+(y+j)*size+z];
			arr[cnt++] = paddingSize[(x-i*elemSize)+(y-j)*size+z];
                    }
                }
		quickSort(arr,0,8); 
                outimg[x+y*size+z] = arr[0];
            }
        }
    }
#endif
#if 0
    for(y = 0; y < bmpInfoHeader.biHeight - 1; y++) {
        for(x = 0; x < bmpInfoHeader.biWidth * elemSize; x+=elemSize) {
            for(z = 0; z < elemSize; z++) {
                outimg[(rand()+z)%imageSize] = clip((outimg[(rand()%imageSize-3)+z]+rand()%255), 0, 255);
		}
	    }
    }
#else
    for(x = 0; x < 500 ; x++) {
        srand((unsigned int)time(NULL));
	int pox = rand()%bmpInfoHeader.biWidth;
	int poy = rand()%bmpInfoHeader.biHeight;
	int y = rand()%255;
        for(z = 0; z < elemSize; z++) {
        outimg[pox*elemSize + poy*size + z] = clip((outimg[pox*elemSize + poy*size + z]+y), 0, 255);
    	}
    }
#endif

/*
    for(int x = 0; x < bmpInfoHeader.SizeImage; x++){
    srand((unsigned int)time(NULL));
    outimg[x] = rand();
    outimg[x+1] = rand();
    outimg[x+2] = rand();
    }*/
/*    for(int b = 0; b < bmpInfoHeader.biWidth; b++){
    //맨 아랫줄 처리
    outimg[elemSize*b] = inimg[size + elemSize*b];
    outimg[elemSize*b + 1] = inimg[size + elemSize*b+1];
    outimg[elemSize*b + 2] = inimg[size + elemSize*b+2];

    //맨 윗줄 처리
    outimg[size*(bmpInfoHeader.biHeight -1) + elemSize*b] = inimg[(bmpInfoHeader.biHeight-2)*size+b*elemSize];
    outimg[size*(bmpInfoHeader.biHeight -1) + elemSize*b + 1] = inimg[(bmpInfoHeader.biHeight-2)*size+b*elemSize + 1];
    outimg[size*(bmpInfoHeader.biHeight -1) + elemSize*b + 2] = inimg[(bmpInfoHeader.biHeight-2)*size+b*elemSize + 2];
    }    */ 
    /***** write bmp *****/ 
    if((fp=fopen(argv[2], "wb"))==NULL) { 
        fprintf(stderr, "Error : Failed to open file...₩n"); 
        return -1;
    }

    /* BITMAPFILEHEADER 구조체의 데이터 */
    fwrite(&bmpHeader, sizeof(BITMAPFILEHEADER), 1, fp);

    /* BITMAPINFOHEADER 구조체의 데이터 */
    fwrite(&bmpInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);

    //fwrite(inimg, sizeof(ubyte), imageSize, fp); 
    fwrite(outimg, sizeof(ubyte), imageSize, fp);

    fclose(fp); 
    
    free(inimg);
    free(paddingSize); 
    free(outimg);
    
    return 0;
}

