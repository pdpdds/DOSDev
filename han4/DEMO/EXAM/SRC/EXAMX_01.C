#include "hanio.h"
#include "extendio.h"

void main(void)
{
   int i;

   if(init_han() != OK_HAN) exit(1);

   for(i = 0; i < 100; i++) cputs_han("¯aÇa©· ¯¡Ðñ·³“¡”a.  ");
   puts_han("\n´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();

   /* maxx_han, maxy_han·e ¸åµb ¥e® */
   for(i = 0; i <= get_maxy_han(); i++)
   {
      xscrollup_han(0,            0, maxx_han/2, maxy_han);
      xscrolldn_han(maxx_han/2+1, 0, maxx_han,   maxy_han);
   }

   gotoxy_han(0, 0);
   puts_han("´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();

   close_han();
}
