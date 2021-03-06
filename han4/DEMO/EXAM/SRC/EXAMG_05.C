#include "hanio.h"
#include "misc.h"
#include "keydef.h"

void main(void)
{
   char string[MAX_STRLEN+1];
   int ret_code;

   if(init_han() != OK_HAN) exit(1);

   /* �wȁ �a���i �a�a�a�A �e�a. */
   status_on_han();

   /* �A�鯢�a�� �a�� ҁ �wȁ �a���i �a�� �a���a. */
   set_keyboard_type(BUL3_TYPE);
   draw_status_han();

   if(get_keyboard_type() == BUL2_TYPE)
      puts_han("���鯢 �a�e�i �a�w�a�e �������a.");
   else
      puts_han("�A�鯢 �a�e�i �a�w�a�e �������a.");

   puts_han("�a�� �i�a�a ���bЁ ���A�a.(���� = <Esc>)\n");

   while(1)
   {
      ret_code = gets_han(string);
      if(ret_code == ESC) break;
      puts_han(string);
   }

   close_han();
}
