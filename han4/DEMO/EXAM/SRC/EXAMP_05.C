#include "hanio.h"
#include "extendio.h"

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   /* '�e'���a�e �e�i�i �a���e �w��a�� �b�e�a.  */
   /* eputch_han() �q���e �a�w�i �� ���a.          */

   putch_han("�e");
   xputch_han(  2, 0, "�e");
   aputch_han(  4, 0, "�e", FONT_SHADE);
   vputch_han(  6, 0, "�e", GREEN);
   vaputch_han( 8, 0, "�e", BLUE, FONT_OUTLINE);
   vbputch_han(10, 0, "�e", DARKGRAY, LIGHTGRAY);

   /* 'E'�a�e �w���i �a���e �w��a�� �b�e�a. */

   gotoxy_han(0, 1);
   putch_han("E");
   xputch_han(  2, 1, "E");
   aputch_han(  4, 1, "E", FONT_SHADE);
   vputch_han(  6, 1, "E", GREEN);
   vaputch_han( 8, 1, "E", BLUE, FONT_OUTLINE);
   vbputch_han(10, 1, "E", DARKGRAY, LIGHTGRAY);
   eputch_han( 12, 1, 'E');

   xputs_han(0, 3, "�a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();

   close_han();
}
