#include "hanio.h"

void main(void)
{
   char string[51];

   if(init_han() != OK_HAN) exit(1);
   status_on_han();

   puts_han("���q�e ���� ���a�����a. �a�� �i�a�a ���bЁ ���A�a..");

   /* �b�a�i �a���� string�i �� ���a�i�� �e�e ҁ adv_gets_han() �q���i ѡ */
   draw_box_han(0, 18, 51, 20, BOX_H1V1);
   string[0] = 0;
   adv_gets_han(1, 19, string, 50, FONT_NORMAL, "");

   puts_han("\n���� ���a���� �·�Ж�s���a? ���A �s�� ���a�� ����Ё ���V�s���a."
            "default_ins_change_func() �q���e �a�� ѡ�a�� �g�s���a.");
   puts_han("�a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();

   /* �s�� ���a�� �����e�a. */
   ins_mode = 1;

   puts_han("\n���q�e �s�� ���a�����a. �a�� �i�a�a ���bЁ ���A�a..");
   adv_gets_han(1, 19, string, 50, FONT_NORMAL, "");

   puts_han("�s�� ���a���� �·�Ж�s���a? �a�� �wȁ �a���A�e ���� ���a�a�� "
            "�a���A�� ���s���a. ���A default_ins_change_func() �q���i ѡ"
            "Ё ���V�s���a.");
   puts_han("�a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();

   /* default_ins_change_func() �q���i ѡ�a�a �wȁ �a���� �s��/���� */
   /* �a�����i �����e�a. default_han_change_func() �q���� �i�� �I���� */
   /* �e ���� �g�e�a.                                                 */
   default_ins_change_func();

   puts_han("\n�wȁ �a���� �s��/���� �a�����a �a�a�A �����A���s���a?");
   puts_han("�a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();

   close_han();
}
