#include "hanio.h"
#include "extendio.h"
#include "misc.h"

void my_wait_func(void);

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   /* ·³b ”‹¡ ¯¡ˆe •·´e my_wait_func·¡ Ñ¡Â‰–A•¡¢ Ðe”a. */
   set_han_wait_func(my_wait_func);

   puts_han("getch_han() Ðq®ˆa ¯©Ð—–A“e º—·³“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.\n");
   getch_han();
   puts_han("getch() Ðq®ˆa ¯©Ð—–A“e º—·³“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.\n");
   getch();

   close_han();
}

void my_wait_func(void)
/* ·³b ”‹¡ ¯¡ˆe •·´e Ñ¡Â‰–A“e Ðq® */
{
   static int i = 0;

   i = (i + 1) % 30000;
   xprintf_han(get_minx_han(), get_maxy_han(), "%5d", i);
}
