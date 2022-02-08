#include <string.h>

#include "KORFONT.c"
#include "ENGFONT.C"
#include "SPCFONT.C"
#include "GRPFONT.C"
//#include "HNJFONT.C"

typedef unsigned char byte;

/*-------------------- µb¬w ‹i¸a  ---------------------*/
static void IMG_1(byte *dest, const int bytes)
{
   register int i;

   for(i=0; i != bytes ; i++) dest[i] = ~dest[i];
}
/*-------------------- ·qµw ‹i¸a  ---------------------*/
static void IMG_2(byte *dest, const int bytes)
{
   register int i;

   for(i=0; i != bytes; )
   {
      dest[i++] |= 0xaa; if (bytes==32) dest[i++] |= 0xaa;
      dest[i++] |= 0x55; if (bytes==32) dest[i++] |= 0x55;
   }
}
/*-------------------- ¶A‰Â¬å ‹i¸a  ---------------------*/
static void IMG_3(byte *dest, const int bytes)
{
   register int i;
   char temp[32];

   memcpy(temp, dest, bytes);

   for(i=0;i<bytes;i++)
   {
      dest[i] |= temp[i] >> 1; dest[i] |= temp[i] << 1;
   }
   if(bytes == 16)
   {
      for(i=0; i<= 14; i++) dest[i+1] |= temp[i];
      for(i=0; i<= 15; i++) dest[i-1] |= temp[i];
      for(i=0; i<= 14; i++) dest[i]   ^= temp[i];
   } else
   {
      for(i=0; i<= 29 ; i++) dest[i+2] |= temp[i];
      for(i=0; i<= 31 ; i++) dest[i-2] |= temp[i];
      for(i=0; i<= 31 ; i++) dest[i]   ^= temp[i];
   }
}
/*-------------------- ‹aŸ±¸a ‹i¸a  ---------------------*/
static void IMG_4(byte *dest, const int bytes)
{
   register int i;
   char temp[32];

   memcpy(temp , dest, bytes);

   if(bytes == 16)
  {
     for(i=0; i< 16; i++) dest[i]   |= temp[i] >> 1;
     for(i=0; i< 15; i++) dest[i+1] |= temp[i];
     for(i=0; i< 16; i++) dest[i]   ^= temp[i];
  }
   else
  {
     for(i=0; i< 32; i++) dest[i]   |= temp[i] >> 1;
     for(i=0; i< 30; i++) dest[i+2] |= temp[i];
     for(i=0; i< 32; i++) dest[i]   ^= temp[i];
  }
}
/*-------------------- »¥Ðe ‹i¸a  ---------------------*/
static void IMG_5(byte *dest, const int bytes)
{
  register int i;

  for(i = 0; i < bytes ; i++) dest[i] |= dest[i] >>1;
}
/*-------------------- £»º‰ ‹i¸a  ---------------------*/
static void IMG_6(byte *dest, const int bytes)
{
  if(bytes == 16) dest[bytes-1] = 0xff;
  else
  {
    dest[bytes-2] = 0xff; dest[bytes-1] = 0xff;
  }
}

#if 0
static void fp_NULL_POINTER (byte* dest, const int bytes) { ;}
void (*DoFontImage[7]) (byte *dest, const int bytes) =
	{ fp_NULL_POINTER, IMG_1, IMG_2, IMG_3, IMG_4, IMG_5, IMG_6};
#else
////  ??? BC Can Not Function Pointer ????  ////
void DoFontImage (byte *dest, const int bytes, const int ftype)
{
 switch (ftype)
  {
   default:  IMG_1(dest, bytes); break;
   case 1 :  IMG_1(dest, bytes); break;
   case 2 :  IMG_2(dest, bytes); break;
   case 3 :  IMG_3(dest, bytes); break;
   case 4 :  IMG_4(dest, bytes); break;
   case 5 :  IMG_5(dest, bytes); break;
   case 6 :  IMG_6(dest, bytes); break;
  }
}
#endif