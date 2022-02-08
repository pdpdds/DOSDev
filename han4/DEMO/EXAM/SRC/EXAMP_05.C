#include "hanio.h"
#include "extendio.h"

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   /* 'Ðe'·¡œa“e Ðe‹i·i ”a´·Ðe ¤w¤ó·a¡ Â‰bÐe”a.  */
   /* eputch_han() Ðq®“e ¬a¶wÐi ® ´ô”a.          */

   putch_han("Ðe");
   xputch_han(  2, 0, "Ðe");
   aputch_han(  4, 0, "Ðe", FONT_SHADE);
   vputch_han(  6, 0, "Ðe", GREEN);
   vaputch_han( 8, 0, "Ðe", BLUE, FONT_OUTLINE);
   vbputch_han(10, 0, "Ðe", DARKGRAY, LIGHTGRAY);

   /* 'E'œa“e µw¢…·i ”a´·Ðe ¤w¤ó·a¡ Â‰bÐe”a. */

   gotoxy_han(0, 1);
   putch_han("E");
   xputch_han(  2, 1, "E");
   aputch_han(  4, 1, "E", FONT_SHADE);
   vputch_han(  6, 1, "E", GREEN);
   vaputch_han( 8, 1, "E", BLUE, FONT_OUTLINE);
   vbputch_han(10, 1, "E", DARKGRAY, LIGHTGRAY);
   eputch_han( 12, 1, 'E');

   xputs_han(0, 3, "´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();

   close_han();
}
