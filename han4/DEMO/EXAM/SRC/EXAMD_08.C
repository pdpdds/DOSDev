#include <time.h>
#include "hanio.h"
#include "extendio.h"

void main(void)
{
   clock_t time1, time2;

   if(init_han() != OK_HAN) exit(1);

   puts_han("���i �q�b���e �a���a�������a. �a�� ǡ�a ���a�A�a..");

   /* ǡ�a ������ �g�e ���e draw_cursor_han �q���i ���� ѡ�e�a. */
   /* �a���� cursor_twinkle_time�e ��b �e�����a.                 */
   /* (1) ��b �e�� draw_cursor�� ����á�e 1���A �e�� �� �t�i 0�a */
   /*     �� �a�����a�e �� �����A�� �a�� 1�� �a���� ���ᴡ ���a */
   /*     �a�a���a.                                               */
   /* (2) cursor_han�e ��ɷ ���e eng_cursor�i �a��ǡ�e�A  �e�i �� */
   /*     ��i �b�a�� ���a�e �� �����A��  cursor_han = han_curs */
   /*     or; �a�� Ё ���ᴡ �e�a.                                */
   time1 = clock();
   while(!kbhit())
      if( ((time2 = clock()) - time1) > cursor_twinkle_time )
      {
         draw_cursor_han(curx_han, cury_han);
         time1 = time2;
      }
   if(!getch()) getch();

   close_han();
}
