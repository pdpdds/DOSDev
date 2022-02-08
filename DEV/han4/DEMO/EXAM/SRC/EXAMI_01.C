#include <process.h>
#include "hanio.h"

void main(void)
/* parent process */
{
   int state;

   if(init_han() != OK_HAN) exit(1);
   /* ºÏa¡‹aœ‘µA¬á“e ‹aœÏ¢ Á¡‹¡ÑÁŸi Ðe ¤å Ðe”a. */

   puts_han("ºÏa¡‹aœ‘·¡ ¯©Ð—–A“e º—·³“¡”a.");
   puts_han("´a¢ Ç¡a ’Ÿa¡e ¦Ïa¡‹aœ‘·¡ ¯©Ð—–S“¡”a.");
   if(!getch()) getch();

   if((state = spawnl(P_WAIT, "exami_02.exe", NULL)) != 0)
   /* ‹aœÏ¢ ¡¡—a¡ –… À exami_02.exeŸi ¯©Ð—Ðe”a. */
   /* µAœáˆa ¤i¬—Ða¡e ”ˆ ¡A¡¡Ÿ¡ˆa ¦¹¢Ðe ˆõ·¡”a. */
   {
      error_exit_msg = "exami_02.exeŸi ¯©Ð—Ði ® ´ô¯s“¡”a";
      error_exit_han("exami_02.exe· main()", 1);
   }

   /* ¦Ïa¡‹aœ‘µA¬á Ñe¸ ¹ÁÎaˆa ´a¢Ÿ¡ ¤aŽá´á•¡ */
   /* ºÏa¡‹aœ‘µA“e µwÐ··i º»¡ ´g·a£a¡ ¹ÁÎaŸi */
   /* ¤aŽ´á º»¡ ´g·a¡e ¦Ïa¡‹aœ‘·   ¡A¯¡»¡µÁ */
   /* ‰sÃ¥”a.  ˜aœa¬á gotoxy_han()·i Ñ¡Â‰Ðaµa Ñe */
   /* ¸ ¹ÁÎaŸi ¤aŽ´á º´á´¡ Ðe”a.              */
   gotoxy_han(0, 6);
   puts_han("ºÏa¡‹aœ‘·a¡ •©´a µÖ¯s“¡”a. ´a¢ Ç¡a Ã¡­A¶a..");
   if(!getch()) getch();
   close_han();
}
