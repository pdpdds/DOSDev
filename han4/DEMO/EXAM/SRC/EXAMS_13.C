#include "hanio.h"
#include "keydef.h"

void main(void)
{
   char string[MAX_STRLEN+1];

   if(init_han() != OK_HAN) exit(1);

   /* �wȁ �a���� �a�a�a�A �e�a. */
   puts_han("�a�� ǡ�a ���a�e �wȁ �a���� �a�a�s���a.");
   if(!getch()) getch();
   status_on_han();

   puts_han("�wȁ �a���� �a�a�v�s���a.\n");
   /* ���a�i�i �e��Ё�� ���b�h�e�a. */
   while(1)
   {
      puts_han("���a�i�i ���bЁ ���A�a.(�{ = <Esc>)");
      if(gets_han(string) == ESC) break;
   }

   /* �wȁ �a���i �a�a���A �e�a. */
   puts_han("\n�a�� ǡ�a ���a�e �wȁ �a���� �a�a�����a.");
   if(!getch()) getch();
   status_off_han();

   puts_han("�wȁ �a���� �a�a�v�s���a.\n");
   puts_han("�a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();
   close_han();
}
