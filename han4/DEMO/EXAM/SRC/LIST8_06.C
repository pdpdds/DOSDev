#include <dos.h>
#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

void main(void)
{
   struct time cur_t;
   int  ch;

   if(init_han() != OK_HAN) exit(1);

   /* Á¡‹¡ ÑÁ¡e·i Š¬÷Ðe”a. */
   draw_box_han(31, 9, 49, 11, BOX_H1V1);
   gettime(&cur_t);
   xprintf_han(32, 10, "%02d ¯¡ %02d ¦… %02d Á¡",
               cur_t.ti_hour, cur_t.ti_min, cur_t.ti_sec);
   xputs_han(30, 13, "<Home>/<End>  : ¯¡ +/-");
   xputs_han(30, 14, "<Up>/<Down>   : ¦… +/-");
   xputs_han(30, 15, "<PgUp>/<PgDn> : Á¡ +/-");
   xputs_han(30, 16, "<Enter>       : ¤aŽ‹¡");
   xputs_han(30, 17, "<Esc>         : Âá­¡");

   /* <Esc>a <Enter> Ç¡Ÿi ’Ÿi ˜Œa»¡ ¤e¥¢Ðe”a. */
   while(1)
   {
      ch = getch();
      if(ch == _ESC)
      /* ‹a· žÏaŸi ¨a¹a aˆe”a. */
         break;
      else if(ch == _ENTER)
      /* ¯¡ˆe·i ¬¡ ¬é¸÷Ða‰¡ žÏaŸi ¨a¹a aˆe”a. */
      {
         settime(&cur_t);
         break;
      }
      else if(ch == 0)
      /* ÑÂ¸w Ç¡ˆa ’‰a¹v·a¡e */
      {
         ch = getch();
         switch(ch)
         {
            case HOME : cur_t.ti_hour = (cur_t.ti_hour + 1)  % 24; break;
            case END  : cur_t.ti_hour = (cur_t.ti_hour + 23) % 24; break;
            case UP   : cur_t.ti_min  = (cur_t.ti_min  + 1)  % 60; break;
            case DOWN : cur_t.ti_min  = (cur_t.ti_min  + 59) % 60; break;
            case PGUP : cur_t.ti_sec  = (cur_t.ti_sec  + 1)  % 60; break;
            case PGDN : cur_t.ti_sec  = (cur_t.ti_sec  + 59) % 60; break;
         }
         /* ¯¡ˆe·i ¬¡ Îa¯¡(Ñe¸ ¯¡ˆe‰Á“e ”aŸi ®•¡ ·¶”a.) */
         xprintf_han(32, 10, "%02d ¯¡ %02d ¦… %02d Á¡",
                     cur_t.ti_hour, cur_t.ti_min, cur_t.ti_sec);
      }
   }

   close_han();
}
