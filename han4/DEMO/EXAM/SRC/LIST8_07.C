#include <alloc.h>
#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

#define PAD_WIDTH  40
#define PAD_HEIGHT 10

/* �q�� ���w */
int note_pad(int box_x, int box_y, char **text, int height, int width);

void main(void)
{
   /* ���b�h�e ���w�i�e char ͡���� ���i�� text�A ��w�E�a. */
   char *(text[PAD_HEIGHT]);
   int  i, key;

   if(init_han() != OK_HAN) exit(1);
   status_on_han();

   /* text �A ϩ�a�e �A�����i �i�w�h�e�a. */
   for(i = 0; i < PAD_HEIGHT; i++)
   {
      /* �e ���� ͡����A �e �� ������ �A�����i �i�w�h�e�a. */
      text[i] = (char *) malloc(PAD_WIDTH + 1);
      if(text[i] == NULL)
      {
         error_exit_msg = "�A�����a �����s���a";
         error_exit_han("main()", 1);
      }
      strcpy(text[i], "");
   }

   xputs_han(15, 14, "-------------- �a�w �a�w�e ǡ ���� --------------");
   xputs_han(15, 15, "1. <Left>, <Right>, <Ctrl>+<Left>, <Ctrl>+<Right>");
   xputs_han(15, 16, "   <Home>, <End>, <Up>, <Down>");
   xputs_han(15, 17, "   <Ctrl>+<PgUp>, <Ctrl>+<PgDn>");
   xputs_han(15, 18, "2. <Ins>, <BS>, <Del>, <Ctrl>+<T>");
   xputs_han(15, 19, "   <Ctrl>+<Y>, <Ctrl>+<Q>+<Y>");
   xputs_han(15, 20, "3. <Esc>, <Ctrl>+<End>");

   /* �e �A���� ������ ���w�i ���b�h�e�a. */
   key = note_pad(18, 0, text, PAD_HEIGHT, PAD_WIDTH);

   /* ���b �i���i �b�e�a. */
   clrscr_han();
   cputs_han("�a���b�A ���e ǡ�e ");
   switch(key)
   {
      case ENTER   : cputs_han("<Enter> ���a ");     break;
      case ESC     : cputs_han("<Esc> ���a ");       break;
      case CTRLEND : cputs_han("<Ctrl>+<End>���a "); break;
   }
   puts_han("���b�h�e ���w�i�e �a�q�� �{�s���a.\n");
   for(i = 0; i < PAD_HEIGHT; i++)
   {
      puts_han(text[i]);
      free(text[i]);
   }
   puts_han("\n�a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();

   close_han();
}

int note_pad(int box_x, int box_y, char **text, int height, int width)
/* �����a width �� ���a�i�i height �� ���b�h�a text �A ��w�e�a. */
/* ����A�e �t�e ���a�i �� �a�w�E ǡ�� š�a���A ENTER, ESC, CTR  */
/* LEND �� �a�a���a.                                             */
{
   unsigned char exit_key[] = { UP, DOWN, CTRLPGUP, CTRLPGDN, CTRLEND, 0 };
   int cur_line, key, index;

   draw_box_han(box_x, box_y, box_x+width+1, box_y+height+1, BOX_H1V1);
   cur_line = 0;
   while(1)
   {
      key = adv_gets_han(box_x+1, box_y+1+cur_line, text[cur_line],
                         width, FONT_NORMAL, exit_key);
      /* adv_gets_han()�� ѡ�E ��ҁ�A�e adv_gets_han_index�e */
      /* �e���� ��� ��á�A ���e �����i �x�e�a.                */

      if(key == ESC || key == CTRLEND)
         break;
      else if(key == ENTER)
      {
         /* �A�� �{ ���A�� <Enter> ǡ�i ���a�e ���b�i ���a�e�a. */
         if(++cur_line == height) break;
      }
      else if(key == UP && cur_line > 0)
      /* �e �� �ᝡ ���� */
         cur_line--;
      else if(key == DOWN && cur_line < height - 1)
      /* �e �� �a���� ���� */
         cur_line++;
      else if(key == CTRLPGUP)
      /* �A�� �� ���� ���� */
         cur_line = 0;
      else if(key == CTRLPGDN)
      /* �A�� �{ ���� ���� */
         cur_line = height - 1;

      /* �a�� adv_gets_han()�� ѡ�A���i ���� �A�� ��� ��á�i ��  */
      /* ���e�a.                                                    */
      /* <Enter>, <Ctrl>+<PgUp>, <Ctrl>+<PgDn> -> �A�� �� �i        */
      /* <Up>, <Down>                          -> �e�� ��� ��á    */
      /* �e�� �����e ���� ���a�i �����a �A�� ��� ��á���a �l�i  �� */
      /* �e ���a�i�� �A�� �{�A ��á�a���� �����e�a.                 */
      /* ���� : adv_gets_han_index = ���a�e ��� ��á * (-1) - 1    */
      if(key == ENTER || key == CTRLPGUP || key == CTRLPGDN)
         adv_gets_han_index = 0 * (-1) - 1;
      else
      {
         if(strlen(text[cur_line]) < adv_gets_han_index)
         /* ���� �����e ���� ���a�i �����a �� �l�a�e ���a�i�� �A�� �{�A */
            adv_gets_han_index = strlen(text[cur_line]) * (-1) - 1;
         else
         /* ��ӡ ���a�e �e�� ��� ��á�A */
            adv_gets_han_index = adv_gets_han_index * (-1) - 1;
      }
   }
   return(key);
}
