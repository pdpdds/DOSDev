#include "hanio.h"

void main(void)
{
   char string[MAX_STRLEN+1];

   if(init_han() != OK_HAN) exit(1);

   /* �啡�� �w�b�i �����a�e �b�a�i �a���a. */
   draw_box_han(9, 4, 70, 21, BOX_H1V1);
   /* �啡���i ����e�a. */
   set_window_han(10, 5, 69, 20);

   do
   {
      puts_han("���w�i ���b�a�A�a");
      gets_han(string);
      puts_han(string);
   }
   while(strlen(string) != 0);     /* �a�� <Enter> ǡ�i é ���a�� �e���e�a. */

   puts_han("�a�� ǡ�a ���a�A�a");
   getch();
   close_han();
}
