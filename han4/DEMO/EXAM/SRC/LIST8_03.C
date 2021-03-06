#include <graphics.h>
#include <alloc.h>
#include "hanio.h"
#include "extendio.h"

void main(void)
{
   unsigned size;
   void *screen;

   if(init_han() != OK_HAN) exit(1);

   /* (0, 0) �A �a�w �a�w�A ���w �i���� �q�w���a�i �b�e�a. */
   set_attr_han(FONT_INVERSE);
   vbputs_han(0, 0, "�e�i �a���a�១ <�e> 4.1", LIGHTGRAY, RED);
   /* (0, 1) �A �����a�i �b�e�a. */
   set_attr_han(FONT_VERT_DBL | FONT_HORIZ_DBL);
   vputs_han(0, 1, "�e�i �a���a�១ <�e> 4.1", BLUE);
   /* ��i �u�� ���i �a���a. */
   draw_horiz_line(0, 50, 200, LIGHTMAGENTA);
   draw_vert_line(100, 20, 60, LIGHTGREEN);
   circle(100, 50, 30);

   set_attr_han(FONT_NORMAL);
   nputs_han(0, 10, "�a�� ǡ�a ���a�e ���e�i �������a..", 80);
   if(!getch()) getch();
   /* (0, 0) - (400, 80) �w�b�i ��w�a�e �A ϩ�a�e �A������ ���i size�A ��w */
   size = imagesize(0, 0, 400, 80);
   /* ϩ�a�e �A�����i �i�w�h�e�a. */
   screen = malloc(size);
   if(screen == NULL)
   {
      error_exit_msg = "�A�����a �����s���a";
      error_exit_han("main()", 1);
   }
   /* (0, 0) - (400, 80) �w�b�i screen �A ��w�e�a. */
   getimage(0, 0, 400, 80, screen);
   clrscr_han();

   nputs_han(0, 10, "�a�� ǡ�a ���a�e ���� ���e�i �����s���a..", 80);
   if(!getch()) getch();
   putimage(0, 0, screen, COPY_PUT);

   nputs_han(0, 10, "���� ���e�� �����A���s���a? �a�� ǡ�a ���a�A�a..", 80);
   if(!getch()) getch();
   free(screen);
   close_han();
}
