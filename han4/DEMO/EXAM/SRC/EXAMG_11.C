#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   draw_box_han(20, 9, 49, 11, BOX_H1V1);
   xputs_han(22, 10, "�����i ��w�a���V�s���a?");
   switch(get_yesno_han(47, 10, FONT_INVERSE))
   {
      case YES : xputs_han(22, 13, "��wЖ�s���a.");
                 break;
      case NO  : xputs_han(22, 13, "��w�a�� �g�v�s���a.");
                 break;
      case ESC : xputs_han(22, 13, "��w�� �᭡�A���s���a.");
                 break;
      default  : xputs_han(22, 13, "���i �� ���e �w�������a.");
                 break;
   }

   xputs_han(22, 14, "�a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();
   close_han();
}
