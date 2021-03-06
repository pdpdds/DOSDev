#include "hanio.h"
#include "extendio.h"
#include "misc.h"

void my_wait_func(void);

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   /* ���b ���� ���e ���e my_wait_func�� ѡ�A���� �e�a. */
   set_han_wait_func(my_wait_func);

   puts_han("getch_han() �q���a ��З�A�e �������a. �a�� ǡ�a ���a�A�a.\n");
   getch_han();
   puts_han("getch() �q���a ��З�A�e �������a. �a�� ǡ�a ���a�A�a.\n");
   getch();

   close_han();
}

void my_wait_func(void)
/* ���b ���� ���e ���e ѡ�A�e �q�� */
{
   static int i = 0;

   i = (i + 1) % 30000;
   xprintf_han(get_minx_han(), get_maxy_han(), "%5d", i);
}
