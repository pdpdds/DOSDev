#include <stdio.h>

void decode_line_pcx(char far *dest, int bytes, FILE *stream)
{
   unsigned char ch;
   int index = 0, rep_num;

   while(index < bytes)
   {
      ch = fgetc(stream);
      if((ch & 0xc0) == 0xc0)
      {
	 rep_num = ch & 0x3f;
	 ch = fgetc(stream);
	 while(rep_num--) dest[index++] = ch;
      }
      else
	 dest[index++] = ch;
   }
}
