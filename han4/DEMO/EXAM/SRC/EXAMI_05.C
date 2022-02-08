#include "hanio.h"
#include "extendio.h"

void main(void)
{
   int color;

   if(init_han() != OK_HAN) exit(1);

   if(!is_vga_mode())
   /* HGCŸi ¬a¶wÐa‰¡ ·¶·a¡e ¯©Ð—·i º—”eÐe”a. */
   {
      puts_han("EGA/VGA µA¬á e ¯©Ð— ˆa“wÐe Ïa¡‹aœ‘·³“¡”a.");
      puts_han("´a¢ Ç¡a ’Ÿa­A¶a..");
      if(!getch()) getch();
      close_han();
      exit(1);
   }

   for(color = 0; color < 16; color++)
      vputs_han(0, color, "”a´·Ðe ¬‚· Ðe‹i·³“¡”a.", color);

   xputs_han(0, 17, "´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();

   close_han();
}
