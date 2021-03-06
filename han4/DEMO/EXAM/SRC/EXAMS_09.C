#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

void my_wait_func(void);

void main(void)
{
   char string[MAX_STRLEN+1];
   int ret_code;

   if(init_han() != OK_HAN) exit(1);

   /* �A�����a �b�A�e �a���i ñ���a�� �g�� ��Ё �啡���i ��� */
   set_window_han(0, 0, abs_maxx_han, abs_maxy_han - 1);
   /* ���b ���� ���e ���e my_wait_func�� ѡ�A���� �e�a. */
   set_han_wait_func(my_wait_func);

   puts_han("�a�� �i�a�a ���bЁ ���A�a.");
   puts_han("�{�i ���a�e <Esc> ǡ�i ���a�A�a..\n");
   while(1)
   {
      ret_code = gets_han(string);
      /* <Esc> ǡ�a ���a�v�a�e ���a�i �i�e�a. */
      if(ret_code == ESC) break;
   }

   close_han();
}

void my_wait_func(void)
/* ���b ���� ���e ���e ѡ�A�e �q�� */
{
   aputs_han(0, abs_maxy_han, "�i�� ���b�a�A�a", FONT_NORMAL);
   delay(100);
   aputs_han(0, abs_maxy_han, "�i�� ���b�a�A�a", FONT_INVERSE);
   delay(100);
}
