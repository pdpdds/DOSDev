#include <stdio.h>
#include <alloc.h>
#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

/* �q�� ���w */
void show_help(void);
void put_msg(char *string);

void main(void)
{
   unsigned char exit_key[] = { F1, 0 }, key;
   char string[41];
   if(init_han() != OK_HAN) exit(1);

   draw_box_han(19, 7, 60, 9, BOX_H1V1);
   xputs_han(19, 10, "�����i ���e�i ���a�e <F1>�i ���a�A�a");
   strcpy(string, "");
   do
   {
      key = adv_gets_han(20, 8, string, 40, FONT_NORMAL, exit_key);
      if(key == F1)
      /* ���a�i�i ���b�h�a�a <F1> ǡ�a ���a���e �����i ���e�i ���a ���a. */
      {
         show_help();
         /* �e�� ��� ��á�A�� ���� ���b�i �h���� �e�a. */
         adv_gets_han_index = adv_gets_han_index * (-1) - 1;
      }
   }
   /* <Esc>�a <Enter> ǡ�i ���a�e ���a */
   while(key != ESC && key != ENTER);

   close_han();
}

/* �����i�� ������ ���e �a���A �ŝe�E ���a�� �w�� */
#define HELP_LINE       14                   /* ���e�A �a���A�e �� ��    */
#define MAX_PAGE         2                   /* �a���i �� ���e �A���� �� */
#define MAX_HELP        HELP_LINE * MAX_PAGE /* �a���i �� ���e �� ��     */

void show_help(void)
/* �����i �����i ����� �����i�i ���a ���a. */
{
   FILE *stream;
   char (*help)[68+2], *screen;
   int  cur_page, re_draw, line_no, key, i;

   /* �����i�A ϩ�a�e �A�����i �i�w�e�a. */
   if((help = malloc((68+2)*MAX_HELP)) == NULL)
   {
      put_msg("�A�����a �����s���a. �a�� ǡ�a ���a�A�a.");
      return;
   }

   /* �����i ���� list8_09.hlp�� ���w�i ���� help�A ��w�e�a. */
   if((stream = fopen("list8_09.hlp", "rt")) == NULL)
   {
      put_msg("�����i �����i ���i �� ���s���a. �a�� ǡ�a ���a�A�a.");
      return;
   }
   line_no = 0;
   while(!feof(stream))
      if(line_no >= MAX_HELP || fgets(help[line_no], 70, stream) == NULL)
         break;
      else
      {
         /* ���a�i�� �{�A ���e '\n' ���a�e �b�A�e�a. */
         i = strlen(help[line_no]);
         if(help[line_no][i - 1] == '\n') help[line_no][i - 1] = 0;
         line_no++;
      }
   fclose(stream);

   /* ���e�i ��w�e�a. */
   if((screen = save_text_han(4, 2, abs_maxx_han-4, 2+2+HELP_LINE+3)) == NULL)
   {
      put_msg("�A�����a �����s���a. �a�� ǡ�a ���a�A�a.");
      return;
   }

   /* ���e�i �����e�a. */
   draw_box_han(4, 2, abs_maxx_han-4, 2+2+HELP_LINE+3, BOX_H1V1);
   aputs_han((abs_maxx_han-8)/2, 2+1, " �����i ", FONT_SHADE);
   draw_box_han(4, 2+2, abs_maxx_han-4, 2+2, BOX_H1V1);
   put_eng_font(4,              2+2, 195);
   put_eng_font(abs_maxx_han-4, 2+2, 180);
   draw_box_han(4, 2+2+HELP_LINE+1, abs_maxx_han-4, 2+2+HELP_LINE+1, BOX_H1V1);
   put_eng_font(4,              2+2+HELP_LINE+1, 195);
   put_eng_font(abs_maxx_han-4, 2+2+HELP_LINE+1, 180);
   xputs_han(8, 2+2+HELP_LINE+2, "Up/PgUp : �| �A����   Down/PgDn : "
                                 "�a�q �A����   Esc : �a�e ����");

   /* help�A ��w�E �����i�i ���a ���a. */
   cur_page = 0;
   re_draw  = 1;
   while(1)
   {
      if(re_draw)
      /* �A�����i �a�����a�e �����i�i ���� �a���e�a. */
      {
         for(i = 0; i < HELP_LINE; i++)
            if(cur_page*HELP_LINE+i >= line_no)
               nputs_han(6, 2+3+i, "", 68);
            else
               nputs_han(6, 2+3+i, help[cur_page*HELP_LINE+i], 68);
      }

      /* �a�w�a�� ǡ ���b�i �១�e�a. */
      key = getch_han();
      if(key == _ESC)
         break;
      else if(key == 0)
      {
         switch(getch_han())
         {
            case UP   :
            case PGUP : if(cur_page > 0) cur_page--;
                        re_draw = 1;
                        break;
            case DOWN :
            case PGDN : if(cur_page < MAX_PAGE - 1) cur_page++;
                        re_draw = 1;
                        break;
         }
      }
   }

   /* ���e�i �����a�� help�A �i�w�E �A�����i ���ᯡǥ�a. */
   restore_text_han(4, 2, screen);
   free(help);
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
   /* 3. �A�����i �b�e�a */
   xputs_han(box_x + 1, 11, string);
   /* 4. �e ���� ǡ�i ���b�h�e�a. */
   if(!getch()) getch();
   /* 5. ���� ���e�i �����e�a. */
   restore_text_han(box_x, 10, screen);
}
