#include <dos.h>
#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

/* �q�� ���w */
int error_handler(int errval, int ax, int bp, int si);

void main(void)
{
   char filename[128], ret_key;

   if(init_han() != OK_HAN) exit(1);

   /* �a�a���� �A�� Ѕ�i��i ����e�a. */
   harderr(error_handler);
   /* �����E �����w�e *.hwp�� �a�� �a���a �e�i�w ���Bɡ�� �b�a�i ���a ���a. */
   ret_key = dir_box_long((get_maxx_han() - 58) / 2, 5, "*.hwp", filename);

   if(ret_key == ENTER) printf_han("\n\n<%s> �����i ��Ȃ�a�v�s���a.\n", filename);
   puts_han("�a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();

   close_han();
}

int error_handler(int errval, int ax, int bp, int si)
/* �a�a����ⷥ �A��a �i���a�v�i �� ��З�A�e �q��. */
/* ���a�����A �w���a 4�� �a���e�A ����Ё�� �E�a.    */
{
   char *screen;
   int  box_x;

   box_x = 40 - (42 + 2) / 2;
   screen = save_text_han(box_x, 10, box_x + 42 + 1, 12);
   draw_box_han(box_x, 10, box_x + 42 + 1, 12, BOX_H1V1);
   xputs_han(box_x + 1, 11, "�A��a �i���a�v�s���a. �a�� ǡ�a ���a�A�a.");
   if(!getch()) getch();
   restore_text_han(box_x, 10, screen);
   hardresume(0);       /* �A��i ignore�e�a. */

   return(0);
}
