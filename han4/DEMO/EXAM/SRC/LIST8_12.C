#include <dos.h>
#include "hanio.h"
#include "extendio.h"

/* �q�� ���w */
void my_redirect_func(char *string);

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   /* �b�A�e �A����a my_redirect_func�a�� ��i�A���� �e�a.      */
   /* puts_han(), cputs_han(), printf_han() ���� ����з�� �a�w�a�a.*/
   redirect_func = my_redirect_func;
   printf_han("*****\n");
   puts_han("�� �a���a���e �b ����з�i  ���w�a�a");
   puts_han("�b �����i �����e �w��i ���a �����a.");
   cputs_han("*****\n");
   /* �b�A�e �A����a �a�� �q���A�� ��i�A�� �g���� �e�a. */
   redirect_func = NULL;
   puts_han("\n�a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();

   close_han();
}

void my_redirect_func(char *string)
/* ����з�E �A����i �១�a�e �q�� */
{
   /* ���� ���a �A�� ���a�a �b�I ���a�a 'ˢ' �����i ���a. */
   if(strcmp(string, " "))
   {
      sound(300);
      delay(10);
      nosound();
      delay(190);
   }
}
