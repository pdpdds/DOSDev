#include "hanio.h"

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   /* set_color_han(), get_color_han(), set_bkcolor_han(), */
   /* get_bkcolor_han() �q�� �a�w                          */
   set_color_han(CYAN);
   if(get_color_han() == CYAN)
      puts_han("��w���e �a�i�������a.");
   else
      puts_han("��w���e �a�i���� �a�����a.");
   set_bkcolor_han(BROWN);
   if(get_bkcolor_han() == BROWN)
      puts_han("���w���e �i�������a.");
   else
      puts_han("���w���e �i���� �a�����a.");

   /* ��b �e�� color_han, bkcolor_han �a�w */
   color_han = RED;
   if(color_han == RED)
      puts_han("\n��w���e �i�w�����a.");
   else
      puts_han("\n��w���e �i�w�� �a�����a.");
   bkcolor_han = LIGHTMAGENTA;
   if(bkcolor_han == LIGHTMAGENTA)
      puts_han("���w���e ��ѷ�����a.");
   else
      puts_han("���w���e ��ѷ�� �a�����a.");

   /* set_vbcolor_han() �q�� �a�w */
   set_vbcolor_han(DARKGRAY, LIGHTGRAY);
   puts_han("\n�j�e �A�� �a�w�A �ᖁ�� �A�������a.");

   set_vbcolor_han(WHITE, BLACK);
   puts_han("\n�a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();
   close_han();
}
