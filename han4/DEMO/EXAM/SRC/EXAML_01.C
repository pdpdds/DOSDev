#include "hanio.h"
#include "misc.h"
#include "keydef.h"

void main(void)
{
   char string[MAX_STRLEN+1], backup[MAX_STRLEN+1];
   if(init_han() != OK_HAN) exit(1);

   while(1)
   {
      puts_han("���a�i�i ���b�a�A�a.(���a = <Esc>)");
      if(gets_han(string) == ESC) break;
      /* lefttrim�� righttrim�i ѡ�a�e ���a�i�� ���w�� */
      /* �a��a�� ���� ���a�i Ё ���a�� �e�a.            */
      strcpy(backup, string);

      printf_han("���b�h�e  ���a�i  = [%s]\n", string);
      printf_han("���b ���� �b�A ҁ = [%s]\n", lefttrim(string));
      strcpy(string, backup);
      printf_han("���b ���� �b�A ҁ = [%s]\n", righttrim(string));
      /* ���� right trim �e �E �wȁ���a�� left trim �e */
      /* Ё ���e ���� ������ ���� �b�A�E�a.            */
      printf_han("���� ���� �b�A ҁ = [%s]\n\n", lefttrim(string));
   }

   close_han();
}
