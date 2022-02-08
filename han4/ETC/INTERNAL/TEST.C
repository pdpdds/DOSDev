#include "hanio.h"
#include "misc.h"
#include "keydef.h"

void main(void)
{
   int gr_mode;

   set_internal_font();
   if((gr_mode = choose_gr_mode()) == ESC) exit(1);
   if(_init_han2(gr_mode) != OK_HAN) exit(1);

   puts_han("Í¥Ëa ¸w ¯¡Ðñ º—·³“¡”a.");
   puts_han("Ðe‹i‰Á englishˆa ¸i aµ¡ºa?");
   puts_han("´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();

   _close_han2(gr_mode);
}
