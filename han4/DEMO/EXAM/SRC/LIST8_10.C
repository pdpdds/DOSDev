#include <stdio.h>
#include "hanio.h"
#include "extendio.h"

/* 1. ����з�I �a�a���i �����e�a. */
FILE *redirect_stream;

/* �q�� ���w */
void my_redirect_func(char *string);

void main(int argc, char **argv)
{
   int redirect;

   /* 2. �w�wЗ ���a�i ���a�a�a "list9_10 to <filename>"��  �{�e */
   /*    �w���a�� list9_10�i ��З���v�a�e <filename>�i ���e�e�a. */
   /*    �� �� ����з ��ѡ�e "to"���a.                           */
   if(argc == 3 && !strcmp(argv[1], "to"))
   {
      if((redirect_stream = fopen(argv[2], "wt")) == NULL)
      {
         printf("Can't write to %s", argv[2]);
         exit(1);
      }
      else
         redirect = 1;
   }
   else
      redirect = 0;
   if(init_han() != OK_HAN) exit(1);

   puts_han("�a�q 5���e �b�� ����з�S���a.\n");
   /* 3. ��b �e��  redirect_func�� ����з�I �a�a���i �១�a�e */
   /*    my_redirect_func()�i �a��ǡ���� �e�a. ���q���� �b�A */
   /*    �e �A����e ���� my_redirect_func�a�� ��i�E�a.       */
   if(redirect) redirect_func = my_redirect_func;
   /* printf_han(), puts_han(), cputs_han() ���� ����з�� �a�w */
   /* �q�i ���a ���a.                                          */
   printf_han("*****\n");
   puts_han("�b ����з�i ���w�a�e �a���a����");
   puts_han("��З �i���i �a�� �������a �a���ᝡ");
   puts_han("�b�i �� ���s���a.");
   cputs_han("*****\n");
   /* �b�A�e �A����a �a�� �q���A�� ��i�A�� �g���� �e�a. */
   if(redirect) redirect_func = NULL;
   puts_han("\n�a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();

   close_han();
}

void my_redirect_func(char *string)
/* 4. ����з�E �A����i �១�a�e �q���i �e�e�a. */
{
   /* ����з�E �A���� string�i redirect_stream�a�� �b�e�a. */
   fputs(string, redirect_stream);
}
