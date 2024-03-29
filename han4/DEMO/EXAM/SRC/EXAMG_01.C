#include "hanio.h"

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   /* set_attr_han(), get_attr_han() �q�� �a�w */
   set_attr_han(FONT_SHADE);
   if(get_attr_han() == FONT_SHADE)
      puts_han("�q�w ���a�����a.");
   else
      puts_han("�q�w ���a�a �a�����a.");

   /* ��b �e�� attr_han �a�w */
   attr_han = FONT_OUTLINE | FONT_INVERSE; 
   puts_han("\n�e��E �A�¬� ���a�����a.");
   if(attr_han == FONT_OUTLINE | FONT_INVERSE)
      puts_han("�e��E �A�¬� ���a�����a.");
   else
      puts_han("�e��E �A�¬� ���a�a �a�����a.");

   /* FONT_INVERSE ������ ����A���e�� ���a�e�a.*/
   /* �a�� attr_han == FONT_INVERSE�� �a�e  �e  */
   /* �E�a.                                     */
   if(attr_han & FONT_INVERSE)
      puts_han("\nFONT_INVERSE ������ ����A���s���a.");
   else
      puts_han("\nFONT_INVERSE ������ ����A�� �g�v�s���a.");
   if(attr_han & FONT_OUTLINE)
      puts_han("FONT_OUTLINE ������ ����A���s���a.");
   else
      puts_han("FONT_OUTLINE ������ ����A�� �g�v�s���a.");

   attr_han = FONT_NORMAL;
   puts_han("\n�a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();
   close_han();
}
