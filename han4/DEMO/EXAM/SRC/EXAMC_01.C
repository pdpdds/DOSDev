#include "hanio.h"
#include "extendio.h"

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   puts_han("´e‘wÐa­A¶a? center_puts_han() Ðq®· ¯¡Ðñ·³“¡”a..");

   /* ÑÁ¡e· ¹A·© Ða”eµA ¤e¸å  b”Ÿi ‹aŸ¥”a.          */
   /* get_abs_maxx_han + 1 = ÑÁ¡e· Í¢(¶å•¡¶µÁ ¢‰Å) */
   naputs_han(0, get_abs_maxy_han(), "", get_abs_maxx_han() + 1, FONT_INVERSE);

   /* ¡A¯¡»¡Ÿi Ñe¸ ¬‚·a¡ Â‰bÐe”a. */
   center_puts_han(get_abs_maxy_han(), "´a¢ Ç¡a ’Ÿa­A¶a",
                   get_color_han(), FONT_INVERSE);
   if(!getch()) getch();

   close_han();
}
