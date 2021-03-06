#include "hanio.h"

void main(void)
{
   char string[MAX_STRLEN+1];

   if(init_han() != OK_HAN) exit(1);
   status_on_han();

   puts_han("���q�e �e/�w ���� ǡ�a �b���S���a. <Left Shift>+<Space> ǡ�i "
            "���a�e �e/�w ���ŷ� �A�e�� ����Ё ���A�a..");
   gets_han(string);

   puts_han("\n����A�e �w���e ���b �a�w�a���� �e�i�� ���V�s���a.");
   puts_han("�a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();

   /* �w�� ���b �wȁ�� �e�e ҁ �e/�w ���� ǡ�� �b���i �q����ǥ�a. */
   set_to_eng_mode();
   disable_han_change();
   default_han_change_func();

   puts_han("\n�w���e ���b�i �� ���e �wȁ�����a. �e/�w ���� ǡ�a �b���A�e�� "
            "����Ё ���A�a..");
   gets_han(string);

   puts_han("\n����A�e �e�i�e ���b �a�w�a���� �e�i�� ���V�s���a.");
   puts_han("�a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();

   /* �e�i ���b �wȁ�� �e�e�a. �a��ӡ �e/�w ���� ǡ�e �b���A�� �g�e�a. */
   set_to_han_mode();
   default_han_change_func();

   puts_han("\n�e�i�e ���b�i �� ���e �wȁ�����a. �e/�w ���� ǡ�a �b���A�e�� "
            "����Ё ���A�a..");
   gets_han(string);

   puts_han("\n����A�e �a�� �e�i�� �w�� �����i ���b�i �� ������ �e�i�� "
            "���V�s���a.");
   puts_han("�a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();

   /* �e/�w ���� ǡ�� �b���i ��w�e ҁ �w�� ���b �wȁ�� �a���a. */
   enable_han_change();
   set_to_eng_mode();
   default_han_change_func();

   puts_han("\n�e�i�� �w�� ���� ���b�i �� ���e �wȁ�����a. "
            "��q�A�e �w�� ���b �wȁ�a �S���a. �e/�w ���� ǡ�a "
            "�b���A�e�� ����Ё ���A�a..");
   gets_han(string);

   close_han();
}
