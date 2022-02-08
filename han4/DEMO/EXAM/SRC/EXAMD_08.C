#include <time.h>
#include "hanio.h"
#include "extendio.h"

void main(void)
{
   clock_t time1, time2;

   if(init_han() != OK_HAN) exit(1);

   puts_han("Äá¬áŸi Œq¨b·¡“e Ïa¡‹aœ‘·³“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a..");

   /* Ç¡ˆa ’‰Ÿ¡»¡ ´g·e •·´e draw_cursor_han Ðq®Ÿi ‰­¢ Ñ¡Â‰Ðe”a. */
   /* µa‹¡¬á cursor_twinkle_time·e ¸åµb ¥e®·¡”a.                 */
   /* (1) ¸åµb ¥e® draw_cursor· ¸÷Ã¡“e 1·¥•A  e´¢ ·¡ ˆt·i 0·a */
   /*     ¡ ¤aŽ´ö”a¡e ·¡ ¦¦…µA¬á ”a¯¡ 1¡ ¤aŽ´á º´á´¡ Äá¬áˆa */
   /*     ‹aa»¥”a.                                               */
   /* (2) cursor_han·e ¥¡É· ˜“e eng_cursorŸi ˆaŸ¡Ç¡“e•A  Ðe‹i Äá */
   /*     ¬áŸi Â‰bÐa‰¡ ¯¼”a¡e ·¡ ¦¦…µA¬á  cursor_han = han_curs */
   /*     or; œa‰¡ Ð º´á´¡ Ðe”a.                                */
   time1 = clock();
   while(!kbhit())
      if( ((time2 = clock()) - time1) > cursor_twinkle_time )
      {
         draw_cursor_han(curx_han, cury_han);
         time1 = time2;
      }
   if(!getch()) getch();

   close_han();
}
