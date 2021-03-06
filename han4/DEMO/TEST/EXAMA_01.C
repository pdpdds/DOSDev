#include <string.h>
#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

void main(void)
{
   char string[51];
   char exit_key[] = { UP, DOWN, 0 };
   int  ret_code;

   if((ret_code = init_han()) != OK_HAN)
   {
     printf("%d", ret_code);
    exit(1);
   }

   status_on_han();     /* ���b �wȁ�i ��A �i �� ������ �wȁ �a���i �a�� */
   draw_box_han(9, 9, 10+50, 11, BOX_H2V2);
   strcpy(string, "�e�w�a�A�a?");

   ret_code = adv_gets_han(10, 10, string, 50, FONT_NORMAL, exit_key);

   switch(ret_code)
   {
      case ENTER : xputs_han(10, 13, "<Enter> ǡ�a ���v�s���a.");
                   break;
      case ESC   : xputs_han(10, 13, "<Esc> ǡ�a ���v�s���a.");
                   break;
      case UP    : xputs_han(10, 13, "<Up> ǡ�a ���v�s���a.");
                   break;
      case DOWN  : xputs_han(10, 13, "<Down> ǡ�a ���v�s���a.");
                   break;
   }
   xprintf_han(10, 15, "���b�E ���a�i�e <%s>�����a.", string);
   xputs_han(10, 16, "�a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();

   close_han();
}
