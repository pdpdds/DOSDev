#include <conio.h>
#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

void main(void)
{
   int color, i;

   if(init_han() != OK_HAN) exit(1);

   /* ÑÁ¡e·i Š¬÷Ð ‘½“e”a.*/
   for(i = 0; i < abs_maxy_han; i++)
      xputs_han(i, i, "ÑÁ¡e· ¬‚·¡ ‰­¢ ¤aŽá‰¡ ·¶¯s“¡”a.");
   xputs_han(0, abs_maxy_han, "‹a e –a¡e ´a¢ Ç¡a ’Ÿa­A¶a..");

   color = 0;
   /* Ç¡ˆa ’‰a»© ˜Œa»¡ ÑÁ¡e· ¬‚·i ¤aŽ…”a.*/
   while(!kbhit())
   {
      /* color“e 0¦Èá 15 ¬a·¡· ˆt·i ˆa»¥”a. */
      /* HGC µA¬á“e ´a¢œå ¥eÑÁ•¡ ¬—‹¡»¡ ´g“e”a.*/
      color = (color + 1) % 16;
      modify_text_color(0, 0, abs_maxx_han, abs_maxy_han - 1, color);
   }

   close_han();
}
