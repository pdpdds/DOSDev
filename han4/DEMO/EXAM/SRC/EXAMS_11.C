#include "hanio.h"
#include "keydef.h"

void main(void)
{
   char string[MAX_STRLEN+1];

   if(init_han() != OK_HAN) exit(1);

   /* �啡�� �w�b�i �����a�e �b�a�i �a���a. */
   draw_box_han(9, 4, 61, 16, BOX_H1V1);
   /* �啡���i ����e�a. */
   set_window_han(10, 5, 60, 15);
   puts_han("�{���a�e <Esc> ǡ�i ���a�A�a..");
   do
   {
      puts_han("���w�i ���b�a�A�a");
      if(gets_han(string) == ESC) break;
      /* <Esc> ǡ�i ���a�e ���a�i �i�e�a. */
      puts_han(string);
   }
   while(1);

   /* �啡���i Ё�A�e ҁ ���e�i �����a. */
   init_window_han();
   clrscr_han();
   puts_han("�啡���a Ё�A�A���s���a. �a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();

   close_han();
}
