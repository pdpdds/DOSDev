#include "hanio.h"
#include "extendio.h"

void main(void)
{
   int i, j;

   if(init_han() != OK_HAN)
   {
      puts("Hangul Environment Error..");
      exit(1);
   }

   aputs_han(0, 0, "<Á¡¬÷ Í¥Ëa>", FONT_SHADE);
   /* Á¡¬÷ Í¥Ëa */
   for(i = 0; i < NO_BUL1; i++)
      for(j = 0; j < NO_HAN1 + 1; j++)
         putimage_16(j*2, 1+i, han1_font[i][j]);

   aputs_han(0, 10,"<º—¬÷ Í¥Ëa>", FONT_SHADE);
   /* º—¬÷ Í¥Ëa */
   for(i = 0; i < NO_BUL2; i++)
      for(j = 0; j < NO_HAN2 + 1; j++)
         putimage_16(j*2, 11 + i, han2_font[i][j]);

   aputs_han(0, 16,"<¹·¬÷ Í¥Ëa>", FONT_SHADE);
   /* ¹·¬÷ Í¥Ëa */
   for(i = 0; i < NO_BUL3; i++)
      for(j = 0; j < NO_HAN3 + 1; j++)
         putimage_16(j*2, 17+ i, han3_font[i][j]);

   if(!getch()) getch();

   close_han();
}
