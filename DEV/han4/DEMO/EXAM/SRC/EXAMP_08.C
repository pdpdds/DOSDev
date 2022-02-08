#include "hanio.h"
#include "extendio.h"

void main(void)
{
   int count, delay_time, i;

   if(init_han() != OK_HAN) exit(1);

   for(i = 0; i < 100; i++) cputs_han("Ðe‹i œa·¡§aœáŸ¡ <Ðe> 4.1    ");
   /* abs_maxy_han ·e ¸åµb ¥e® */
   center_puts_han(abs_maxy_han, "   ´a¢ Ç¡a ’Ÿa­A¶a   ",
                   DARKGRAY, FONT_INVERSE);
   if(!getch()) getch();

   /* abs_maxx_han·e ¸åµb ¥e® */
   count = abs_maxx_han * abs_maxy_han * 2;
   delay_time = 0;
   rand_clear_text_han(0, 0, abs_maxx_han, abs_maxy_han,
                       FONT_NORMAL, count, delay_time);
   close_han();
}
