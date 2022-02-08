#include "hanio.h"

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   /* set_color_han(), get_color_han(), set_bkcolor_han(), */
   /* get_bkcolor_han() Ðq® ¬a¶w                          */
   set_color_han(CYAN);
   if(get_color_han() == CYAN)
      puts_han("¸å‰w¬‚·e Ða“i¬‚·³“¡”a.");
   else
      puts_han("¸å‰w¬‚·e Ða“i¬‚·¡ ´a“³“¡”a.");
   set_bkcolor_han(BROWN);
   if(get_bkcolor_han() == BROWN)
      puts_han("¤‰w¬‚·e ˆi¬‚·³“¡”a.");
   else
      puts_han("¤‰w¬‚·e ˆi¬‚·¡ ´a“³“¡”a.");

   /* ¸åµb ¥e® color_han, bkcolor_han ¬a¶w */
   color_han = RED;
   if(color_han == RED)
      puts_han("\n¸å‰w¬‚·e ¨iˆw·³“¡”a.");
   else
      puts_han("\n¸å‰w¬‚·e ¨iˆw·¡ ´a“³“¡”a.");
   bkcolor_han = LIGHTMAGENTA;
   if(bkcolor_han == LIGHTMAGENTA)
      puts_han("¤‰w¬‚·e ¦…Ñ··³“¡”a.");
   else
      puts_han("¤‰w¬‚·e ¦…Ñ··¡ ´a“³“¡”a.");

   /* set_vbcolor_han() Ðq® ¬a¶w */
   set_vbcolor_han(DARKGRAY, LIGHTGRAY);
   puts_han("\n¤j·e ÒA¬‚ ¤aÈwµA ´á–¶… ÒA¬‚·³“¡”a.");

   set_vbcolor_han(WHITE, BLACK);
   puts_han("\n´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();
   close_han();
}
