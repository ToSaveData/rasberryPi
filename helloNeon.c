#include <stdio.h>
#include <arm_neon.h>

void printData(uint8x16_t data)
{
   printf("%02d ", vgetq_lane_u8(data, 0));
   printf("%02d ", vgetq_lane_u8(data, 1));
   printf("%02d ", vgetq_lane_u8(data, 2));
   printf("%02d ", vgetq_lane_u8(data, 3));
   printf("%02d ", vgetq_lane_u8(data, 4));
   printf("%02d ", vgetq_lane_u8(data, 5));
   printf("%02d ", vgetq_lane_u8(data, 6));
   printf("%02d ", vgetq_lane_u8(data, 7));
   printf("%02d ", vgetq_lane_u8(data, 8));
   printf("%02d ", vgetq_lane_u8(data, 9));
   printf("%02d ", vgetq_lane_u8(data, 10));
   printf("%02d ", vgetq_lane_u8(data, 11));
   printf("%02d ", vgetq_lane_u8(data, 12));
   printf("%02d ", vgetq_lane_u8(data, 13));
   printf("%02d ", vgetq_lane_u8(data, 14));
   printf("%02d ", vgetq_lane_u8(data, 15));
   printf("\n");
}

int main(int argc, char **argv)
{
   const uint8_t source[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
   uint8x16_t data, ret, ret3, ret2;
   uint8x16_t fill5 = vmovq_n_u8(5);
   data = vld1q_u8(source);
   
   uint8x16_t fill3 = vmovq_n_u8(3);
   uint8x16_t fill2 = vmovq_n_u8(2);

   printf("data  : ");
   printData(data);
   
   ret = vaddq_u8(data, fill5);
   
   
   printf("result : ");
   printData(ret);
   printf("\n");

   ret3 = vmulq_u8(ret, fill3);

   printf("result : ");
   printData(ret3);
   printf("\n");

   ret2 = vsubq_u8(ret3, fill2);
   printf("result : ");
   printData(ret2);
   printf("\n");
   
   return 0;
}

