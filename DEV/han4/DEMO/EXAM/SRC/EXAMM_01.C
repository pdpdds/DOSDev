#include "hanio.h"

void main(void)
{
   char src[HAN_FONT_SIZE], dest[HAN_FONT_SIZE];
   int  i;

   if(init_han() != OK_HAN) exit(1);

   /* Á¡¬÷ Àõ¤å¼ ¤é· –¤å¼ ¢…¸a· Í¥ËaŸi srcµA ¸á¸w */
   for(i = 0; i < HAN_FONT_SIZE; i++) src[i] = han1_font[0][1][i];

   /* ¶¥œ· Í¥ËaŸi Â‰b */
   xputs_han(0, 0, "¶¥œ· Í¥Ëa  : ");
   putimage_16(15, 0, src);

   /* FONT_INVERSE¡ ¥eÑÅÐe Ò Â‰b */
   modify_image(dest, src, HAN_FONT_SIZE, FONT_INVERSE);
   xputs_han(0, 1, "FONT_INVERSE : ");
   putimage_16(15, 1, dest);

   /* FONT_SHADE¡ Â‰b */
   modify_image(dest, src, HAN_FONT_SIZE, FONT_SHADE);
   xputs_han(0, 2, "FONT_SHADE   : ");
   putimage_16(15, 2, dest);

   aputs_han(0, 4, "´a¢ Ç¡a ’Ÿa­A¶a..", FONT_INVERSE);
   if(!getch()) getch();

   close_han();
}
