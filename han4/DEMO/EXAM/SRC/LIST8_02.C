#include <stdio.h>
#include "hanio.h"
#include "extendio.h"

/* �q�� ���w */
int save_to_file(int left, int top, int right, int bottom, char *filename);

void main(void)
{
   char string[MAX_STRLEN + 1];
   int msg_x, msg_y, msg_length; /* �A�����a �b�I ���a�� �A������ ���� */
   int i;

   if(init_han() != OK_HAN) exit(1);

   /* �A�����e �啡���� �����a�A ���e �A�� �a�e�A �a�a�e�a. */
   msg_x = 0;
   msg_y = get_abs_maxy_han();
   /* �A������ �����e ���e�� ͢�� �����a�A �e�a. */
   msg_length = get_abs_maxx_han() + 1;

   /* ���e �A�� ���A �b�w�� �A���� �a���i �e�e�a. */
   naputs_han(msg_x, msg_y, "", msg_length, FONT_INVERSE);

   /* ���e�� �A���� ��w�A�e�� ��a�a�� ��Ё ���� ���e�i ����Ё ���e�a. */
   /* ���a�i�i �b�e ҁ�A�� �a��ϡ�a�a �A�� �g���� �a�� ��Ё puts_han  */
   /* ()�� �a�� cputs_han() �i �a�w�a�v�a.                              */
   for(i = 0; i < 50; i++) cputs_han("�e�i �a���a�១ <�e> 4.1      ");
   modify_text_attr(10, 5, 70, 15, FONT_INVERSE);

   naputs_han(msg_x, msg_y, "�a�� ǡ�a ���a�e �b�w�a�� �a���E ������ ��w�S���a",
              msg_length, FONT_INVERSE);
   if(!getch()) getch();

   if(save_to_file(10, 5, 70, 15, "screen.dta") != OK_HAN)
      naputs_han(msg_x, msg_y, "���e�i ��w�i �� ���s���a. �a�� ǡ�a ���a�A�a.",
                 msg_length, FONT_INVERSE);
   else
      naputs_han(msg_x, msg_y, "���e�� ��w�A���s���a. �a�� ǡ�a ���a�A�a.",
                 msg_length, FONT_INVERSE);
   if(!getch()) getch();
   close_han();
}

int save_to_file(int left, int top, int right, int bottom, char *filename)
/* (left, top) - (right, bottom) �w�b�i file_name �A ��w�e�a.     */
/* �A���� ��w�A�e OK_HAN �i, �A��a �i���a�e �A��š�a�i �����e�a. */
{
   FILE *stream;
   int  x, y;

   if((stream = fopen(filename, "wt")) == NULL) return(1);
   for(y = top; y <= bottom; y++)
   {
      for(x = left; x <= right; x++) fputc(scr_buff[y][x][0], stream);
      /* �w�� �����A�� �e�i�� �i���� �g���� �e �a���a �� ��a�e�a.   */
      /* �a���b �i�a�a �e�i ���弁 �a���a���e �e �a���a �� ��w�e�a. */
      if(scr_buff[y][x - 1][1] == HAN1) fputc(scr_buff[y][x][0], stream);
      fputc('\n', stream);           /* ��З ���a �a */
   }
   fclose(stream);
   return(OK_HAN);
}
