#include "hanio.h"
#include "extendio.h"

void main(void)
{
   int i;

   if(init_han() != OK_HAN) exit(1);
   if(is_vga_mode())
   {
      puts_han("�� �a���a���e HGC�A��e ��З�S���a.");
      puts_han("�a�� ǡ�a ���a�A�a..");
      if(!getch()) getch();
      close_han();
      exit(1);
   }

   /* �e�� 0 �A�����a ���A �����a. */
   xputs_han(0, 0, "�e�� �A�����e 0 �A���������a.");
   for(i = 0; i < 200; i += 2) circle(400, 200, i);

   setactivepage_han(1);
   /* �� ���e���� ���e ���a�� �a��Ϣ�e 1�A������ �b�E�a. */
   /* 1 �A�����e ���A ������ �g�e�a.                        */
   xputs_han(0, 0, "�e�� �A�����e 1 �A���������a.");
   for(i = 0; i < 200; i += 2) circle(200, 200, i);

   setactivepage_han(0);
   /* �� ���e���� ���e ���a�� �a��Ϣ�e 0�A������ �b�E�a. */
   xputs_han(0, 1, "�a�� ǡ�a ���a�e 1 �A������ ���ŖS���a.");
   if(!getch()) getch();
   setvisualpage(1);
   /* setvisualpage(1) �� ѡ�A�e ���e 1�A�����a ���A ��  */
   /* ���A �E�a.                                           */

   setactivepage_han(1);
   xputs_han(0, 1, "�a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();
   close_han();
}
