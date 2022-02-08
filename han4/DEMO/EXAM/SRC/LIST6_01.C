#include "hanio.h"

void main(void)
{
   union hangul han;

   han.code = 0xd065;
   printf("code = 0x%x\n", han.code);
   printf("ishan = %1d   cho = %2d   jung = %2d   jong = %2d\n",
	  han.bit.ishan, han.bit.cho, han.bit.jung, han.bit.jong);
   printf("byte[1] = 0x%2x   byte[0] = 0x%2x\n", han.bytes[1], han.bytes[0]);
}
