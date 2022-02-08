#include "hanio.h"
#include "extendio.h"
#include "misc.h"
#include "keydef.h"

void main(void)
{
   int delay_time = 10;
   char ch;

   if(init_han() != OK_HAN) exit(1);

   /* Ç¡ ¬a¶w¤ó·i ´ia º…”a. */
   set_attr_han(FONT_SHADE);
   puts_han("¬a¶wÇ¡");
   set_attr_han(FONT_NORMAL);
   puts_han("<Up>   : delay_time 1 »wˆa");
   puts_han("<Down> : delay_time 1 ˆq­¡");
   puts_han("<PgUp> : delay_time 10 »wˆa");
   puts_han("<PgDn> : delay_time 10 ˆq­¡");
   puts_han("<Home> : delay_time·i 0·a¡ ¬é¸÷(ÐaÐeÃ¡)");
   puts_han("<End>  : delay_time·i 500·a¡ ¬é¸÷(¬wÐeÃ¡)");
   puts_han("<Esc>  : {‹¡");
   puts_han("\n´a¢ Ç¡a ’Ÿa­A¶a..");

   if(!getch()) getch();
   clrscr_han();

   /* <Esc> Ç¡ˆa ’‰Ÿ© ˜Œa»¡ ‰­¢ ¤b¯aŸi ‹aŸ¥”a. */
   aprintf_han(33, 20, FONT_INVERSE, "delay_time = %3d", delay_time);
   while(1)
   {
      draw_var_box_han(20, 10, 60, 18, BOX_H1V1, FONT_NORMAL, delay_time);
      delay(300);
      clear_text_han(20, 10, 60, 18);

      if(kbhit())
      {
         if((ch = getch()) == _ESC) break;
         if(ch == 0)
         {
            switch(ch = getch())
            {
               case UP   : if(delay_time <= 499) delay_time++;
                           break;
               case DOWN : if(delay_time >= 1) delay_time--;
                           break;
               case PGUP : if(delay_time <= 490) delay_time += 10;
                           break;
               case PGDN : if(delay_time >= 10) delay_time -= 10;
                           break;
               case HOME : delay_time = 0;
                           break;
               case END  : delay_time = 500;
                           break;
            }
            aprintf_han(33, 20, FONT_INVERSE, "delay_time = %3d", delay_time);

            /* Ç¡ ¤áÌáŸi »¡¶…”a. */
            clear_key_buff();
         }
      }
   }

   close_han();
}
