#include "hanio.h"
#include "extendio.h"

void main(void)
{
   char *(han_font[6]);         /* �a���a 6�� ͡���� ���i(ͥ�a�w) */
   int error, i;

   /* 6�a�� �e�i ͥ�a�i �e���Bɡ���A�� ���� �i���a. */
   error = 0;
   if(read_han_font(&han_font[0], "hangod1.fnt")  != OK_HAN) error = 1;
   if(read_han_font(&han_font[1], "hangod2.fnt")  != OK_HAN) error = 1;
   if(read_han_font(&han_font[2], "hanmyung.fnt") != OK_HAN) error = 1;
   if(read_han_font(&han_font[3], "hanson1.fnt")  != OK_HAN) error = 1;
   if(read_han_font(&han_font[4], "hanson2.fnt")  != OK_HAN) error = 1;
   if(read_han_font(&han_font[5], "hansam.fnt")   != OK_HAN) error = 1;
   /* �a�a�a�� �A���� ���� ���e ͥ�a�a ���a�e ��З�i �����e�a. */
   if(error)
   {
      puts("font reading error..");
      exit(1);
   }
   if(init_han() != OK_HAN) exit(1);

   puts_han("�a�� ǡ�a ���a�e ͥ�a�i �a���� �b�s���a.\n");
   if(!getch()) getch();
   /* 6�a�� ͥ�a�i �a�w�a�a ���a�i�i �b�e�a. */
   for(i = 0; i < 6; i++)
   {
      /* �w��ͥ�a�� �b�����a ͥ�a�e �a���� ���a. */
      set_font(han_font[i], NULL, NULL);
      puts_han("�e�i �a���a�១ <�e> 4.1");
   }

   /* ͥ�a�A �i�w�E �A�����i free��ǥ ҁ �e�i ͥ�a�e */
   /* �a�� ������ ���a�� �A���a ���e�a.              */
   for(i = 0; i < 6; i++) free(han_font[i]);
   return_to_org_font(1, 0, 0);

   puts_han("\n�a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();
   close_han();
}
