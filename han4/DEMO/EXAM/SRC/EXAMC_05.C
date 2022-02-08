#include "hanio.h"
#include "extendio.h"

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   puts_han("ÑÁ¡e ¸åÁAŸi FONT_INVERSE¡ »¡¶‰V¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
   if(!getch()) getch();
   aclrscr_han(FONT_INVERSE);

   puts_han("ÑÁ¡e ¸åÁAŸi ‹a· »¡¶‰V¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
   if(!getch()) getch();
   clrscr_han();

   puts_han("ÑÁ¡e ¸åÁAŸi FONT_SHADE¡ »¡¶‰V¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
   if(!getch()) getch();
   aclrscr_han(FONT_SHADE);

   puts_han("ÑÁ¡e ¸åÁAŸi Ìaœe¬‚·a¡ »¡¶‰V¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
   if(!getch()) getch();
   vclrscr_han(BLUE);

   puts_han("´a¢ Ç¡a ’Ÿa­A¶a");
   if(!getch()) getch();

   close_han();
}
