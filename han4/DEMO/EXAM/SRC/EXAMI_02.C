#include "hanio.h"

void main(void)
/* child process */
{
   int state;

   return;

   stay_in_gr_mode = 1;
   /* ·¡œý‰A Ða¡e init_han() Ðq®Ÿi Ñ¡Â‰Ð•¡ ‹aœÏ¢ Á¡‹¡ÑÁ žË¥·i ˆåášå”a. */
   if((state = init_han()) != OK_HAN) exit(state);

   /* ˆt·¡ ¸÷Ð»¡»¡ ´g·e ¥e®—i· ˆt·i »¢¸ó »¡¸÷Ðe”a. */
   in_vga_system = 1;
   video_seg     = 0xa000;
   abs_maxx_han  = maxx_han = 79;
   abs_maxy_han  = maxy_han = 29;

   /* ºÏa¡‹aœ‘µA¬á Ñe¸ ¹ÁÎaˆa ´a¢Ÿ¡ ¤aŽá´á•¡ ¦Ïa¡‹aœ‘µA“e µwÐ··i º»¡ */
   /* ´g·a£a¡ ¹ÁÎaŸi ¤aŽ´á º»¡ ´g·a¡e ºÏa¡‹aœ‘· ¡A¯¡»¡µÁ ‰sÃ¥”a. ˜aœa */
   /* ¬á gotoxy_han()·i Ñ¡Â‰Ðaµa Ñe¸ ¹ÁÎaŸi ¤aŽ´á º´á´¡ Ðe”a.            */
   gotoxy_han(0, 3);
   puts_han("¦Ïa¡‹aœ‘·¡ ¯©Ð—–A“e º—·³“¡”a.");
   puts_han("´a¢ Ç¡a ’Ÿa¡e ºÏa¡‹aœ‘·a¡ •©´aˆs“¡”a..");
   if(!getch()) getch();

   /* stay_in_gr_modeˆa 1·¡£a¡ ‹aœÏ¢ ¡¡—a¡ –… À ¹·žaÐe”a. */
   close_han();
}
