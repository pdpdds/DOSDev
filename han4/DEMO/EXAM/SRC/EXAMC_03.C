#include <dos.h>
#include "hanio.h"
#include "misc.h"

void main(void)
{
   char ch;

   if(init_han() != OK_HAN) exit(1);

   /* delayˆa Ñ¡Â‰–A“e •¡º—µA Ç¡Ÿi ’‰œá ¥¡œa. */
   puts_han("delay(5000)·¡ Ñ¡Â‰–A‰¡ ·¶“e º—·³“¡”a. "
            "<Y> a <N> Ç¡Ÿi ’‰œá ¥¡­A¶a.");
   delay(5000);

   puts_han("<Y> a <N> Ç¡Ÿi ’Ÿa­A¶a..");
   /* ÀáŸ¡–A»¡ ´g‰¡ Ç¡ ¤áÌáµA q´a ·¶“e Ç¡“e ¡¡– »¡¶¡¤áŸ¥”a. */
   clear_key_buff();
   do
   {
      if((ch = getch()) == 0) getch();
      if(ch == 'Y' || ch == 'y')
      {
         puts_han("<Y> Ç¡Ÿi ’Ÿa­v¯s“¡”a.");
         break;
      }
      else if(ch == 'N' || ch == 'n')
      {
         puts_han("<N> Ç¡Ÿi ’Ÿa­v¯s“¡”a.");
         break;
      }
   }
   while(1);

   puts_han("\n´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();

   close_han();
}
