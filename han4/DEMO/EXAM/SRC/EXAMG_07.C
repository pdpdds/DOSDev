#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

void main(void)
{
   /* set_max_strlen_han(40)�i ѡ�a�� �g�� string�� �a���i 41��    */
   /* �s�a�e ���a�Q�� �����i ���� ���a. �� �a�廡 ���A�i ���a�e gets */
   /* _han�i ���� ���a.                                              */
   char string[40 + 1];
   int ret_code;

   if(init_han() != OK_HAN) exit(1);

   /* ���b �a�w�e ���a�� ���i 40 �a�� �����a. */
   set_max_strlen_han(40);
   printf_han("���b �a�w�e ���a�� ���e %d�������a.\n", get_max_strlen_han());
   puts_han("�a�� �i�a�a ���bЁ ���A�a.(���� = <Esc>)\n");
   puts_han("+---------+---------+---------+---------+");
   puts_han("12345678901234567890123456789012345678901");

   while(1)
   {
      ret_code = gets_han(string);
      if(ret_code == ESC) break;
      puts_han(string);
   }

   close_han();
}
