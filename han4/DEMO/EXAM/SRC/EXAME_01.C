#include "hanio.h"
#include "extendio.h"

void func1(int y);

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   /* �A�� �A�����i ���� ��Ё ������ �i���E ���a�i ��i�e�a. */
   func1(40);
   puts_han("�a�� ǡ�a ���a�A�a");
   if(!getch()) getch();

   close_han();
}

void func1(int y)
{
   if(y > get_abs_maxy_han())
   {
      set_attr_han(FONT_INVERSE);
      /* error_exit_msg�e ��b �e�����a. */
      error_exit_msg = "y ���a�a ���i ����s���a!!!";
      /* �A��a �i���e �q���e main()���� ���a š�a�e 0���a. */
      error_exit_han("main()", 0);
   }

   gotoxy_han(y, 0);
   puts_han("y ���a�a �a�a�A �����A���s���a.");
}
