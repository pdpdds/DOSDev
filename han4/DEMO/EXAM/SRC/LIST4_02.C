#include "hanio.h"

void main(void)
{
   char string[MAX_STRLEN+1];

   if(init_han() != OK_HAN) exit(1);

   /* �啡���i ����e�a. */
   set_window_han(10, 5, 69, 20);
   gotoxy_han(20, 5);
   puts_han("�e");
   /* �啡���i �a�� ����e�a. �� �� �啡�� �w�b�e (75, 10)�A  "�e" ���a�e */
   /* ���a�i�i �b�i �� ������ �s�a ���ᴡ �e�a. �a����e (75, 10) - (79,*/
   /* 12)�� �a�v�a.                                                       */
   set_window_han(75, 10, 79, 12);
   gotoxy_han(0, 0);
   puts_han("�a�{");
   /* �啡���i �������� �����e�a. */
   set_window_han(10, 5, 69, 20);

   getch();
   close_han();
}
