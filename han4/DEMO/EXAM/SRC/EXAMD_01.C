#include "hanio.h"

void main(void)
{
   char string[MAX_STRLEN+1];
   if(init_han() != OK_HAN) exit(1);
   status_on_han();

   puts_han("���q�e �w�� ���b �wȁ�����a. �e�i ���b �wȁ�� �a���V�s���a.");
   puts_han("�a�� �i�a�a ���bЁ ���A�a..");
   gets_han(string);

   /* �a�w�a�a �a�� ǡ�a ���a�e �e�i ���b �wȁ�� �����e �a�q, */
   /* default_han_change_func()�i ѡ�e�a.                  */
   set_to_han_mode();
   default_han_change_func();

   puts_han("\n���q�e �e�i ���b �wȁ�����a. "
            "�a�� �w�� ���b �wȁ�� �a���V�s���a.");
   puts_han("�a�� �i�a�a ���bЁ ���A�a..");
   gets_han(string);

   /* �a�w�a�a �a�� ǡ�a ���a�e �w�� ���b �wȁ�� �����e �a�q, */
   /* default_han_change_func()�i ѡ�e�a.                   */
   set_to_eng_mode();
   default_han_change_func();

   puts_han("\n\n�a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();

   close_han();
}
