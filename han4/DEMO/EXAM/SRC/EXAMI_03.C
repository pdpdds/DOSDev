#include "hanio.h"

void main(void)
{
   char string[MAX_STRLEN+1];

   if(init_han() != OK_HAN) exit(1);

   if(is_han_mode())
      puts_han("���q�e �e�i ���b �wȁ�����a.");
   else
      puts_han("���q�e �w�� ���b �wȁ�����a.");
   puts_han("�a�� �i�a�a ���bЁ ���A�a..");
   gets_han(string);

   puts_han("���A �e�i ���b �wȁ�� �a���V�s���a. �a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();
   /* �a�w�a�a �a�� ǡ�a ���a�e �e�i ���b �wȁ�� �����e�a. */
   set_to_han_mode();

   puts_han("���q�e �e�i ���b �wȁ�����a.");
   puts_han("�a�� �i�a�a ���bЁ ���A�a..");
   gets_han(string);
   puts_han("���A �w�� ���b �wȁ�� �a���V�s���a. �a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();
   /* �a�w�a�a �a�� ǡ�a ���a�e �w�� ���b �wȁ�� �����e�a. */
   set_to_eng_mode();

   puts_han("���q�e �w�� ���b �wȁ�����a.");
   puts_han("�a�� �i�a�a ���bЁ ���A�a..");
   gets_han(string);

   close_han();
}
