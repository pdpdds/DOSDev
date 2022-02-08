#include "hanio.h"
#include "extendio.h"

void main(void)
{
   int x, y, code;

   if(init_han() != OK_HAN) exit(1);

   clrscr_han();
   draw_box_han(18, 4, 60, 18, BOX_H1V1);
   xputs_han(33, 2, "´a¯aÇ¡ Å¡—aÎa");

   /* ´a¯aÇ¡ ¢…¸aŸi Â‰bÐe”a. */
   for(code = 0; code < 256; code++)
   {
      x = 20 + (code % 20) * 2;
      y = 5 + code / 20;
      switch(code % 3)
      {
         case 0 : put_eng_font(x, y, code);
                  break;
         case 1 : aput_eng_font(x, y, code, FONT_BOLD);
                  break;
         case 2 : vput_eng_font(x, y, code, GREEN);
                  break;
      }
   }

   aputs_han(30, 20, "´a¢ Ç¡a ’Ÿa­A¶a..", FONT_SHADE);
   if(!getch()) getch();

   close_han();
}
