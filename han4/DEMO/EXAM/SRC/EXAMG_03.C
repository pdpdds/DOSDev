#include "hanio.h"

void main(void)
{
   int x, y;

   if(init_han() != OK_HAN) exit(1);

   set_window_han(10, 5, 50, 15);
   printf_han("get_abs_maxx_han() = %2d\n", get_abs_maxx_han());
   printf_han("get_abs_maxy_han() = %2d\n", get_abs_maxy_han());
   printf_han("get_minx_han()     = %2d\n", get_minx_han());
   printf_han("get_miny_han()     = %2d\n", get_miny_han());
   printf_han("get_maxx_han()     = %2d\n", get_maxx_han());
   printf_han("get_maxy_han()     = %2d\n", get_maxy_han());

   /* printf_han() �q���� ���a�i�i �b�a�e �e�� ���a�a */
   /* �a��a�� ���� �a�e �e���A ��wЁ ���ᴡ �e�a.     */
   gotoxy_han(0, 6);
   x = get_curx_han();
   y = get_cury_han();
   printf_han("get_curx_han()     = %2d\n", x);
   printf_han("get_cury_han()     = %2d\n", y);
   puts_han("\n�a�� ǡ�a ���a�A�a.");

   if(!getch()) getch();
   close_han();
}
