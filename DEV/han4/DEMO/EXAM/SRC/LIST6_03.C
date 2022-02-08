#include "hanio.h"

void main(void)
{
   int x, y;

   if(init_han() != OK_HAN)
   {
      puts("Hangul Environment Error..");
      exit(1);
   }

   set_window_han(10, 5, 50, 15);
   printf_han("abs_maxx_han = %2d\n", abs_maxx_han);
   printf_han("abs_maxy_han = %2d\n", abs_maxy_han);
   printf_han("minx_han     = %2d\n", minx_han);
   printf_han("miny_han     = %2d\n", miny_han);
   printf_han("maxx_han     = %2d\n", maxx_han);
   printf_han("maxy_han     = %2d\n", maxy_han);

   /* printf_han() Ðq®¡ ¢…¸aµi·i Â‰bÐa¡e Ñe¸ ¹ÁÎaˆa */
   /* ¤aŽá£a¡ £¡Ÿ¡ ”aŸe ¥e®µA ¸á¸wÐ –´á´¡ Ðe”a.     */
   gotoxy_han(0, 6);
   x = curx_han;
   y = cury_han;
   printf_han("curx_han     = %2d\n", x);
   printf_han("cury_han     = %2d\n", y);
   puts_han("\n´a¢ Ç¡a ’Ÿa­A¶a.");

   if(!getch()) getch();
   close_han();
}
