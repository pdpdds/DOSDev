#include "hanio.h"
#include "extendio.h"

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   puts_han("�e�i ͥ�a�e �����A�� ASCII FONT�e italic�A�� �a���V�s���a.");
   puts_han("�a�� ǡ�a ���a�A�a..\n");
   if(!getch()) getch();
   /* hangod1.fnt : �e�i�� ͥ�a ����(�����A)          */
   /* engital.fnt : �a�aǡ ���a�� ͥ�a ����(��ȉ���A) */
   if(replace_font("hangod1.fnt", "engital.fnt", "") != OK_HAN)
      puts_han("\7ͥ�a�i �a�� �� ���s���a..");

   puts_han("�a�� ������ FONT�� ���a ���V�s���a.");
   puts_han("�a�� key�a ���a�A�a..\n");
   if(!getch()) getch();
   if(replace_font("hangul.fnt", "english.fnt", "") != OK_HAN)
      puts_han("\7ͥ�a�i �a�� �� ���s���a..");

   puts_han("�a�� ������ FONT�� ���a �֯s���a.");
   puts_han("�a�� key�a ���a�A�a..\n");
   if(!getch()) getch();
   close_han();
}
