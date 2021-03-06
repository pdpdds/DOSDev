#include "hanio.h"
#include "extendio.h"

void main(void)
{
   char *string = "���a�a �����e ��q �a���i ��a �劅 �q���e ���� �w�� "
                  "�a�i��֓e�A, ́�a�a �a�����a�� ���a�e ���e �a�� é��  "
                  "�w�A ���a�� �g�v�a.";

   if(init_han() != OK_HAN) exit(1);

   set_window_han(20, 5, 60, 20);
   draw_box_han(19, 4, 61, 21, BOX_H1V1);

   /* �a�� puts_han() �q���� �b�e�a. */
   set_attr_han(FONT_INVERSE);
   puts_han("puts_han() �q���� �b�a�e �a�q�� �{�e ������ �S���a.");
   set_attr_han(FONT_NORMAL);
   puts_han(string);
   set_attr_han(FONT_INVERSE);

   /* adv_puts_han() �q���� �b�e�a(2�e �i�a �e�a). */
   puts_han("\nadv_puts_han() �q���� �b�a�e �a�q�� �{�e ������ �S���a.");
   set_attr_han(FONT_NORMAL);
   adv_puts_han(string, 2);
   set_attr_han(FONT_INVERSE);
   puts_han("\n�a�� ǡ�a ���a�A�a.");
   if(!getch()) getch();

   close_han();
}
