#include "hanio.h"
#include "extendio.h"

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   puts_han("�a�� ǡ�a ���a�e ���e�� �e��S���a..");
   if(!getch()) getch();

   /* ���A ���e�i �e�寡ǥ�a. */
   modify_text_attr(0, 0, abs_maxx_han, abs_maxy_han, FONT_INVERSE);

   /* ���A ���e�� ������ FONT_INVERSE�� �a�����a��  */
   /* �|�a�� �b�A�e ���a�A�e �wз�i ���� �g�e�a.    */
   puts_han("�a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();

   close_han();
}
