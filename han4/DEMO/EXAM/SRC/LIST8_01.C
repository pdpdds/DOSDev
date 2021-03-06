#include "hanio.h"
#include "extendio.h"

/* �q�� ���w */
void left_scrl(int left, int top, int right, int bottom, int width);
void right_scrl(int left, int top, int right, int bottom, int width);

void main(void)
{
   char string[MAX_STRLEN + 1];
   int msg_x, msg_y, msg_length;     /* �A�����a �b�I ���a�� �A������ ���� */
   int i;

   if(init_han() != OK_HAN) exit(1);

   /* �A�����e �啡���� �����a�A ���e �A�� �a�e�A �a�a�e�a. */
   msg_x = 0;
   msg_y = get_abs_maxy_han();
   /* �A������ �����e ���e�� ͢�� �����a�A �e�a. */
   msg_length = get_abs_maxx_han() + 1;

   /* ���e �A�� ���A �b�w�� �A���� �a���i �e�e�a. */
   naputs_han(msg_x, msg_y, "", msg_length, FONT_INVERSE);

   /* ���e�� �A���� �a�a���A�e�� ��a�a�� ��Ё ���� ���e�i ����Ё ���e�a. */
   /* ���a�i�i �b�e ҁ�A�� �a��ϡ�a�a �A�� �g���� �a�� ��Ё  puts_han() */
   /* �� �a�� cputs_han() �i �a�w�a�v�a.                                  */
   for(i = 0; i < 50; i++) cputs_han("�e�i �a���a�១ <�e> 4.1      ");

   naputs_han(msg_x, msg_y, "�a�� ǡ�a ���a�e ���A ���e�� ���e���a�� �a�a���S���a.",
              msg_length, FONT_INVERSE);
   if(!getch()) getch();
   /* �A���� �a���i �A�A�e �� ���e�i ���e���a�� 5 ���e �a�a����ǥ�a. */
   right_scrl(0, 0, get_maxx_han(), get_maxy_han() - 1, 5);

   naputs_han(msg_x, msg_y, "�a�� ǡ�a ���a�e ���A ���e�� �E���a�� �a�a���S���a.",
              msg_length, FONT_INVERSE);
   if(!getch()) getch();
   /* �A���� �a���i �A�A�e �� ���e�i �E���a�� 5 ���e �a�a����ǥ�a. */
   left_scrl(0, 0, get_maxx_han(), get_maxy_han() - 1, 5);

   naputs_han(msg_x, msg_y, "�a�a���� �A���s���a. �a�� ǡ�a ���a�A�a.",
              msg_length, FONT_INVERSE);
   if(!getch()) getch();
   close_han();
}

void right_scrl(int left, int top, int right, int bottom, int width)
/* (left, top) - (right, bottom) �w�b�i ���e���a�� width ���e�q �a�a���e�a.*/
{
   char *screen;

   /* ���a�a �i�� �����A���a�e error_exit_han()�i ѡ�a�a ��З�i */
   /* ���e�e�a.                                                   */
   /* ���A���e left, top, right, bottom �w�� ���a�� �a  ��aЁ��  */
   /* �a���e �a����e ���w�� �a���a �g�e ���e ��a�a���� Ж�a.    */
   if(left > right + width || width < 1) error_exit_han("scrl_left", 1);

   /* 1. ���e�i ��w�e�a. */
   if((screen = save_text_han(left, top, right - width, bottom)) == NULL)
   {
      /* �A�����a ���a�a�e �A�� �១ �q�� error_exit_han()�i ѡ�a�a */
      /* ��З�i ���e�e�a.                                             */
      error_exit_han("scrl_left", 1);
   }

   /* 2. ���a�i �������a ���e�i �����e�a. */
   restore_text_han(left + width, top, screen);

   /* 3. �a�ỡ �����i �����a. */
   clear_text_han(left, top, left + width - 1, bottom);
}

void left_scrl(int left, int top, int right, int bottom, int width)
/* (left, top) - (right, bottom) �w�b�i �E���a�� width ���e�q �a�a���e�a. */
{
   char *screen;

   /* ���a�a �i�� �����A���a�e error_exit_han()�i ѡ�a�a ��З�i  */
   /* ���e�e�a.                                                    */
   /* ���A���e left, top, right, bottom �w�� ���a�� �a  ��aЁ��   */
   /* �a���e �a����e ���w�� �a���a �g�e ���e ��a�a���� Ж�a.     */
   if(left + width > right || width < 1) error_exit_han("scrl_left", 1);

   /* 1. ���e�i ��w�e�a */
   if((screen = save_text_han(left + width, top, right, bottom)) == NULL)
   {
      /* �A�����a ���a�a�e �A�� �១ �q�� error_exit_han()�i ѡ�a�a  */
      /* ��З�i ���e�e�a.                                              */
      error_exit_han("scrl_right", 1);
   }

   /* 2. ���a�i �������a ���e�i �����e�a. */
   restore_text_han(left, top, screen);

   /* 3. �a�ỡ �����i �����a. */
   clear_text_han(right - width + 1, top, right, bottom);
}
