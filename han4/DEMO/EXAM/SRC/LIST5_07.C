#include "hanio.h"
#include "extendio.h"

void main(void)
{
   char *screen;                 /* ���e �A����i ��w�i �e�� */
   char string[MAX_STRLEN + 1];
   int msg_x, msg_y, msg_length; /* �A�����a �b�I ���a�� �A������ ���� */
   int i;

   if(init_han() != OK_HAN)
   {
      puts("Hangul Environment Error..");
      exit(1);
   }

   /* �A�����e �啡���� �����a�A ���e �A�� �a�e�A �a�a�e�a. */
   msg_x = 0;
   msg_y = get_abs_maxy_han();
   /* �A������ �����e ���e�� ͢�� �����a�A �e�a. */
   msg_length = get_abs_maxx_han() + 1;

   /* ���e �A�� ���A �b�w�� �A���� �a���i �e�e�a. */
   naputs_han(msg_x, msg_y, "", msg_length, FONT_INVERSE);

   /* ���e�� �A���� �����A�e�� ��a�a�� ��Ё ���� ���e�i ����Ё ���e�a. */
   /* ���a�i�i �b�e ҁ�A�� �a��ϡ�a�a �A�� �g���� �a�� ��Ё puts_han  */
   /* () �q���a �a�� cputs_han() �q���i �a�w�a�v�a.                     */
   for(i = 0; i < 50; i++) cputs_han("�e�i �a���a�១ <�e> 4.1      ");
   naputs_han(msg_x, msg_y, "�a�� ǡ�a ���a�e �s�� �啡���a �a�a�s���a.",
              msg_length, FONT_INVERSE);
   if(!getch()) getch();

   /* 1. ���e�i ��w�e�a. */
   if((screen = save_text_han(20, 5, 60, 15)) == NULL)
   {
      /* �A�����a ���a�a�e �A�����i �b�e ҁ ��З�i ���e�e�a. */
      naputs_han(msg_x, msg_y, "�A�����a ���a�s���a. �a�� ǡ�a ���a�A�a.",
                 msg_length, FONT_INVERSE);
      if(!getch()) getch();
      close_han();
      exit(1);
   }

   /* 2. �b�a�i �a���� �a���i�i �b�e�a. */
   draw_box_han(20, 5, 60, 15, BOX_H1V1);
   aputs_han(34, 5, " �s�� �啡�� ", FONT_INVERSE);

   /* 3. ϩ�a�e ���e �w�b�i �����a. */
   clear_text_han(21, 6, 59, 14);

   /* 4. ��w�e �១�i �e�a.                                     */
   /* �a����e �啡���i ����e ҁ �啡�����A�� ���a�i�i ���b�h�� */
   /* �bЁ ���a. �a�� <Enter>�i á�e �{���a.                  */
   set_window_han(21, 6, 59, 14);
   naputs_han(msg_x, msg_y, "���a�i�i ���bЁ ���A�a.",
              msg_length, FONT_INVERSE);

   do
   {
      gets_han(string);
      puts_han(string);
   }
   while(strlen(string) != 0);          /* while(!string[0]); �a�� Ё�� �E�a. */
   set_attr_han(FONT_SHADE);
   naputs_han(msg_x, msg_y, "�a�� ǡ�a ���a�e �s�� �啡���a �a�a�����a.",
              msg_length, FONT_INVERSE);
   if(!getch()) getch();

   /* 5. ���e�i �����e�a. */
   restore_text_han(20, 5, screen);

   naputs_han(msg_x, msg_y, "���e�� �A���� �����A���s���a? �a�� ǡ�a ���a�A�a.",
              msg_length, FONT_INVERSE);
   if(!getch()) getch();

   close_han();
}
