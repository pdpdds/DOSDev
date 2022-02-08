#include "hanio.h"
#include "extendio.h"
#include "misc.h"

void main(void)
{
   char string[] = "<Ðe>·e Èá¥¡ C¶w Ðe‹i œa·¡§aœáŸ¡·¡”a.";
   int  i;

   if(init_han() != OK_HAN) exit(1);

   xputs_han(0, 0, string);
   for(i = 0; i < strlen(string); i++)
      if(what_byte(string, i) == HAN1) eputch_han(i, 1, 24);

   xputs_han(0, 3, "´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();

   close_han();
}
