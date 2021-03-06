#include "hanio.h"

void main(void)
{
   char src[HAN_FONT_SIZE], dest[HAN_FONT_SIZE];
   int  i;

   if(init_han() != OK_HAN) exit(1);

   /* ���� ���弁 �鷁 ���弁 ���a�� ͥ�a�i src�A ��w */
   for(i = 0; i < HAN_FONT_SIZE; i++) src[i] = han1_font[0][1][i];

   /* ������ ͥ�a�i �b */
   xputs_han(0, 0, "������ ͥ�a  : ");
   putimage_16(15, 0, src);

   /* FONT_INVERSE�� �e���e ҁ �b */
   modify_image(dest, src, HAN_FONT_SIZE, FONT_INVERSE);
   xputs_han(0, 1, "FONT_INVERSE : ");
   putimage_16(15, 1, dest);

   /* FONT_SHADE�� �b */
   modify_image(dest, src, HAN_FONT_SIZE, FONT_SHADE);
   xputs_han(0, 2, "FONT_SHADE   : ");
   putimage_16(15, 2, dest);

   aputs_han(0, 4, "�a�� ǡ�a ���a�A�a..", FONT_INVERSE);
   if(!getch()) getch();

   close_han();
}
