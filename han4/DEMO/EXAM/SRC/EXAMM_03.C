#include <conio.h>
#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

void main(void)
{
   int color, i;

   if(init_han() != OK_HAN) exit(1);

   /* ���e�i ����Ё ���e�a.*/
   for(i = 0; i < abs_maxy_han; i++)
      xputs_han(i, i, "���e�� ���� ���� �a�ቡ ���s���a.");
   xputs_han(0, abs_maxy_han, "�a�e ���a�e �a�� ǡ�a ���a�A�a..");

   color = 0;
   /* ǡ�a ���a�� ���a�� ���e�� ���i �a���a.*/
   while(!kbhit())
   {
      /* color�e 0���� 15 �a���� �t�i �a���a. */
      /* HGC �A��e �a���� �e���� ������ �g�e�a.*/
      color = (color + 1) % 16;
      modify_text_color(0, 0, abs_maxx_han, abs_maxy_han - 1, color);
   }

   close_han();
}
