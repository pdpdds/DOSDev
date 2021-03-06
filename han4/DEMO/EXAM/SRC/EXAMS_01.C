#include <stdlib.h>
#include "hanio.h"

void draw_dummy(void);

void main(void)
{
   char *screen;

   if(init_han() != OK_HAN) exit(1);

   draw_dummy();
   puts_han("���e�i �����V�s���a. �a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();
   /* ���e ���A�i ��w�e ҁ �����a. */
   screen = save_text_han(0, 0, abs_maxx_han, abs_maxy_han);
   if(screen == NULL)
   {
      close_han();
      puts("Memory is not enough..");
      exit(2);
   }
   clrscr_han();

   puts_han("���e�i �����a�V�s���a. �a�� ǡ�a ���a�A�a..");
   puts_han("���e�� �����E ҁ�A �a�� �a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();

   restore_text_han(0, 0, screen);
   if(!getch()) getch();
   close_han();
}

void draw_dummy(void)
/* ������ ���e�i �����a�e �q�� */
{
   int i, color, bkcolor, attr, rand_attr;

   /* �e�� ���� �����i ��w�e�a. */
   color   = get_color_han();
   bkcolor = get_bkcolor_han();
   attr    = get_attr_han();

   for(i = 0; i < 50; i++)
   {
      /* ���� �����i �i�� �a�a ���a�i�i �b�e�a. */
      set_color_han(random(16));
      set_bkcolor_han(random(16));
      /* �� ���a�e ϡ�e�a. */
      rand_attr = 1 << random(8);
      if((rand_attr & FONT_VERT_DBL) == 0 && (rand_attr & FONT_HORIZ_DBL) == 0)
         set_attr_han(rand_attr);
      cputs_han("�e�i �a���a�១ <�e> 4.1");
      set_color_han(WHITE);
      set_bkcolor_han(BLACK);
      set_attr_han(FONT_NORMAL);
      cputs_han("     ");
   }
   putch_han("\n");

   /* ���� ���� �����i �����e�a. */
   set_color_han(color);
   set_bkcolor_han(bkcolor);
   set_attr_han(attr);
}
