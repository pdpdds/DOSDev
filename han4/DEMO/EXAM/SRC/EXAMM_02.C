#include "hanio.h"
#include "extendio.h"

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   puts_han("´a¢ Ç¡a ’Ÿa¡e ÑÁ¡e·¡ ¤e¸å–S“¡”a..");
   if(!getch()) getch();

   /* ¸åÁA ÑÁ¡e·i ¤e¸å¯¡Ç¥”a. */
   modify_text_attr(0, 0, abs_maxx_han, abs_maxy_han, FONT_INVERSE);

   /* ¸åÁA ÑÁ¡e· ­¢¬÷·¡ FONT_INVERSE¡ ¤aŽá´ö”áœa•¡  */
   /* ´|·a¡ Â‰b–A“e ¢…¸aµA“e µwÐ··i º»¡ ´g“e”a.    */
   puts_han("´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();

   close_han();
}
