#include "hanio.h"

void main(void)
{
   char string[MAX_STRLEN+1];

   if(init_han() != OK_HAN) exit(1);

   /* �啡���i ����e�a. */
   set_window_han(10, 5, 69, 20);
   gotoxy_han(20, 5);
   puts_han("�e");
   /* �啡���i �a���� ���� �锁 ���a�� �b�a�e �q�� xputs_han() �i �a�w�a�a */
   /* �a�� (75, 10)�A �b�e�a.                                             */
   xputs_han(75, 10, "�a�{");

   getch();
   close_han();
}
