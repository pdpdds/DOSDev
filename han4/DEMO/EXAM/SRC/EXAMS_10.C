#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

void main(void)
{
   unsigned char exit_key[] = { UP, DOWN, 0 };
   int  twinkle_time, ret_code;
   char string[41];

   if(init_han() != OK_HAN) exit(1);

   twinkle_time = 7;
   draw_box_han(19, 9, 60, 11, BOX_H1V1);
   xprintf_han(20, 12, "Ñe¸ twinkle_time·e %3d·³“¡”a.", twinkle_time);
   xputs_han(20, 14, "----- ¬a¶w Ç¡ -----");
   xputs_han(20, 15, "<Up>   : ”á ¨iŸ¡");
   xputs_han(20, 16, "<Down> : ”á ÀåÀåÓ¡");
   xputs_han(20, 17, "<Esc>  : {");
   strcpy(string, "Ðe‹i œa·¡§aœáŸ¡ <Ðe> 4.1");
   while(1)
   {
      ret_code = adv_gets_han(20, 10, string, 40, FONT_NORMAL, exit_key);
      if(ret_code == ESC)
         break;
      else if(ret_code == UP)
      {
         if(twinkle_time > 0) twinkle_time--;
      }
      else if(ret_code == DOWN)
      {
         if(twinkle_time < 255) twinkle_time++;
      }
      set_twinkle_time(twinkle_time);
      xprintf_han(41, 12, "%3d", twinkle_time);
   }

   close_han();
}
