#include <dos.h>
#include "hanio.h"
#include "misc.h"

void main(void)
{
   char ch;

   if(init_han() != OK_HAN) exit(1);

   /* delay�a ѡ�A�e �����A ǡ�i ���� ���a. */
   puts_han("delay(5000)�� ѡ�A�� ���e �������a. "
            "<Y> �a <N> ǡ�i ���� ���A�a.");
   delay(5000);

   puts_han("<Y> �a <N> ǡ�i ���a�A�a..");
   /* �១�A�� �g�� ǡ ����A �q�a ���e ǡ�e ���� �����៥�a. */
   clear_key_buff();
   do
   {
      if((ch = getch()) == 0) getch();
      if(ch == 'Y' || ch == 'y')
      {
         puts_han("<Y> ǡ�i ���a�v�s���a.");
         break;
      }
      else if(ch == 'N' || ch == 'n')
      {
         puts_han("<N> ǡ�i ���a�v�s���a.");
         break;
      }
   }
   while(1);

   puts_han("\n�a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();

   close_han();
}
