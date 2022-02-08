#include "hanio.h"
#include "extendio.h"

void main(void)
{
   int x, y;
   int code;

   if(init_han() != OK_HAN)
   {
      puts("Hangul Environment Error..");
      exit(1);
   }

   clrscr_han();
   draw_box_han(18, 4, 60, 18, BOX_H1V1);
   xputs_han(32, 2, "ASCII code table");

   for(code = 0; code < 256; code++)
   {
      x = 20 + (code % 20) * 2;
      y = 5 + code / 20;
      eputch_han(x, y, code);
   }

   aputs_han(20, 20, "Type any key", FONT_SHADE);
   if(!getch()) getch();

   close_han();
}
