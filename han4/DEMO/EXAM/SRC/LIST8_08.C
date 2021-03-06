#include <stdio.h>
#include "hanio.h"
#include "extendio.h"

/* �q�� ���w */
void put_msg(char *string);

void main(void)
{
   char string[4], quit;

   if(init_han() != OK_HAN) exit(1);

   set_max_strlen_han(3);
   quit = 0;
   do
   {
      cputs_han("�w���� �a���e? ");
      gets_han(string);
      if(!is_number(string))
      /* ���a ���A�� ���a�i ���bЖ�a�e �A�����i �b�e�a. */
         put_msg("���a�e ���b�a�A�a");
      else
         quit = 1;
   }
   while(!quit);
   put_msg("�a�� ǡ�a ���a�A�a");

   close_han();
}

void put_msg(char *string)
/* �A���� �b �q�� */
{
   char *screen;
   int  box_x;

   box_x = 40 - (strlen(string) + 2) / 2;
   /* 1. �A�����a �b�I �w�b�i ��w�e�a. */
   screen = save_text_han(box_x, 10, box_x + strlen(string) + 1, 12);
   /* 2. �A���� �b�a�i �a���a. */
   draw_box_han(box_x, 10, box_x + strlen(string) + 1, 12, BOX_H1V1);
   /* 3. �A�����i �b�e�a. */
   xputs_han(box_x + 1, 11, string);
   /* 4. �e ���� ǡ�i ���b�h�e�a. */
   if(!getch()) getch();
   /* 5. ���� ���e�i �����e�a. */
   restore_text_han(box_x, 10, screen);
}
