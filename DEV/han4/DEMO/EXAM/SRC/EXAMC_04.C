#include "hanio.h"
#include "extendio.h"

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   puts_han("ÑÁ¡e ¸åÁAŸi FONT_INVERSE¡ »¡¶‰V¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
   if(!getch()) getch();
   aclear_text_han(0, 0, get_abs_maxx_han(), get_abs_maxy_han(), FONT_INVERSE);

   puts_han("ÑÁ¡e· ·©¦Ÿi ‹a· »¡¶‰V¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
   if(!getch()) getch();
   clear_text_han(10, 15, 40, 20);

   puts_han("ÑÁ¡e· ·©¦Ÿi FONT_SHADE¡ »¡¶‰V¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
   if(!getch()) getch();
   aclear_text_han(10, 15, 40, 20, FONT_SHADE);

   puts_han("ÑÁ¡e· ·©¦Ÿi Ða“i¬‚·a¡ »¡¶‰V¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
   if(!getch()) getch();
   vclear_text_han(10, 15, 40, 20, LIGHTCYAN);

   puts_han("\n\n´a¢ Ç¡a ’Ÿa­A¶a");
   if(!getch()) getch();

   close_han();
}
