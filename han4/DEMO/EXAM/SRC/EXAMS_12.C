#include "hanio.h"
#include "extendio.h"

void main(void)
{
   int i;

   if(init_han() != OK_HAN) exit(1);
   if(is_vga_mode())
   {
      puts_han("·¡ Ïa¡‹aœ‘·e HGCµA¬á e ¯©Ð—–S“¡”a.");
      puts_han("´a¢ Ç¡a ’Ÿa­A¶a..");
      if(!getch()) getch();
      close_han();
      exit(1);
   }

   /* Ñe¸ 0 ÍA·¡»¡ˆa ’…µA ¥¡·¥”a. */
   xputs_han(0, 0, "Ñe¸ ÍA·¡»¡“e 0 ÍA·¡»¡·³“¡”a.");
   for(i = 0; i < 200; i += 2) circle(400, 200, i);

   setactivepage_han(1);
   /* ·¡ ®…ˆe¦Èá ¡¡—e ¢…¸aµÁ ‹aœÏ¢·e 1ÍA·¡»¡¡ Â‰b–E”a. */
   /* 1 ÍA·¡»¡“e ’…µA ¥¡·¡»¡ ´g“e”a.                        */
   xputs_han(0, 0, "Ñe¸ ÍA·¡»¡“e 1 ÍA·¡»¡·³“¡”a.");
   for(i = 0; i < 200; i += 2) circle(200, 200, i);

   setactivepage_han(0);
   /* ·¡ ®…ˆe¦Èá ¡¡—e ¢…¸aµÁ ‹aœÏ¢·e 0ÍA·¡»¡¡ Â‰b–E”a. */
   xputs_han(0, 1, "´a¢ Ç¡a ’Ÿa¡e 1 ÍA·¡»¡¡ ¸åÑÅ–S“¡”a.");
   if(!getch()) getch();
   setvisualpage(1);
   /* setvisualpage(1) ·¡ Ñ¡Â‰–A“e ®…ˆe 1ÍA·¡»¡ˆa ’…µA ¥¡  */
   /* ·¡‰A –E”a.                                           */

   setactivepage_han(1);
   xputs_han(0, 1, "´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();
   close_han();
}
