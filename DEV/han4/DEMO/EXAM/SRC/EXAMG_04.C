#include "hanio.h"
#include "misc.h"
#include "keydef.h"

void main(void)
{
   unsigned char keyflag;
   int ascii, scan;

   if(init_han() != OK_HAN) exit(1);

   puts_han("´a¢ Ç¡a ’Ÿa¡e ‹a Ç¡· ´a¯aÇ¡ Å¡—aµÁ ¯aÄ… Å¡—aŸi "
            "´ia º“e Ïa¡‹aœ‘·³“¡”a.");
   puts_han("º—”eÐa‰¡ ¯¼·a¡e <Left Shift> Ç¡µÁ <End> Ç¡Ÿi ÐqA ’Ÿa­A¶a.");

   while(1)
   {
      scan = get_key_han(&ascii);
      keyflag = get_keyflag_han();
      if(scan == END && (keyflag & LEFT_SHIFT) != 0) break;
      printf_han("´a¯aÇ¡ Å¡—a = %02x  ¯aÄ… Å¡—a = %02x\n", ascii, scan);
   }

   close_han();
}
