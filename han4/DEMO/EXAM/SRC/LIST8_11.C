#include <stdio.h>
#include <dos.h>
#include "hanio.h"
#include "extendio.h"

/* �q�� ���w */
void my_redirect_func(char *string);
void put_msg(char *string);
int  error_handler(int errval, int ax, int bp, int si);

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   /* �a�a���� �A�� Ѕ�i��i ����e�a. */
   harderr(error_handler);
   puts_han("�a�q 5���e �b�� ����з�S���a.");
   puts_han("�a����i �a�� �a�� ǡ�a ���a�A�a..\n");
   /* �b�A�e �A����a my_redirect_func�a�� ��i�A���� �e�a.     */
   /* puts_han(), cputs_han(), printf_han() ���� ����з�� �a�w�a�a.*/
   redirect_func = my_redirect_func;
   printf_han("*****\n");
   puts_han("�b ����з�i ���w�a�e �a���a����");
   puts_han("��З �i���i �a�� �������a �a���ᝡ");
   puts_han("�b�i �� ���s���a.");
   cputs_han("*****\n");
   /* �b�A�e �A����a �a�� �q���A�� ��i�A�� �g���� �e�a. */
   redirect_func = NULL;
   puts_han("\n�a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();

   close_han();
}

void my_redirect_func(char *string)
/* ����з�E �A����i �១�a�e �q�� */
{
   /* ����з�E �A���� string�i �a���ᝡ �b�e�a. */
   fputs(string, stdprn);
}

void put_msg(char *string)
/* �A���� �b �q��(list9_10.c �q��) */
{
   char *screen;
   int  box_x;

   box_x = 40 - (strlen(string) + 2) / 2;
   screen = save_text_han(box_x, 10, box_x + strlen(string) + 1, 12);
   draw_box_han(box_x, 10, box_x + strlen(string) + 1, 12, BOX_H1V1);
   xputs_han(box_x + 1, 11, string);
   if(!getch()) getch();
   restore_text_han(box_x, 10, screen);
}

int error_handler(int errval, int ax, int bp, int si)
/* �a�a����ⷥ �A��a �i���a�v�i �� ��З�A�e �q�� */
/* ���a�����A 4���� �w���a �a���e�A ����Ё�� �E�a. */
{
   put_msg("�A��a �i���a�v�s���a. �a�� ǡ�a ���a�A�a.");
   hardresume(0);       /* �A��i �����e�a. */
   return(0);
}
