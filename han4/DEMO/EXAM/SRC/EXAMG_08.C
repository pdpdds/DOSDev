#include "hanio.h"
#include "extendio.h"

void main(void)
{
   int i;

   if(init_han() != OK_HAN) exit(1);

   for(i = 0; i < 36; i++)
   {
      set_output_offset_han(i);
      xputs_han(i*2, 0, "Ðe");
   }

   set_output_offset_han(0);
   aputs_han(0, 5, "´a¢ Ç¡a ’Ÿa­A¶a..", FONT_INVERSE);
   if(!getch()) getch();

   close_han();
}
