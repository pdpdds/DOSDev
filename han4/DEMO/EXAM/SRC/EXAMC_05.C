#include "hanio.h"
#include "extendio.h"

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   puts_han("���e ���A�i FONT_INVERSE�� �����V�s���a. �a�� ǡ�a ���a�A�a.");
   if(!getch()) getch();
   aclrscr_han(FONT_INVERSE);

   puts_han("���e ���A�i �a�� �����V�s���a. �a�� ǡ�a ���a�A�a.");
   if(!getch()) getch();
   clrscr_han();

   puts_han("���e ���A�i FONT_SHADE�� �����V�s���a. �a�� ǡ�a ���a�A�a.");
   if(!getch()) getch();
   aclrscr_han(FONT_SHADE);

   puts_han("���e ���A�i �a�e���a�� �����V�s���a. �a�� ǡ�a ���a�A�a.");
   if(!getch()) getch();
   vclrscr_han(BLUE);

   puts_han("�a�� ǡ�a ���a�A�a");
   if(!getch()) getch();

   close_han();
}
