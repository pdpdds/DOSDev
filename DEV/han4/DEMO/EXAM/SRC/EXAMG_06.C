#include "hanio.h"
#include "misc.h"
#include "keydef.h"

void main(void)
{
   unsigned char keyflag, org_keyflag;

   if(init_han() != OK_HAN) exit(1);

   /* Ñe¸· Ç¡¥¡—a Ïiœ‹aŸi ¸á¸wÐe”a. */
   org_keyflag = get_keyflag_han();

   /* 1 ”e‰ : get_keyflag_han(), is_keyflag_han() */

   puts_han("Ñe¸ Ç¡¥¡—a Ïiœ‹a· ¬wÈ“e ”a·q‰Á ˆ{¯s“¡”a.");
   puts_han("”a·q ”e‰¡ ñ´áˆaa¡e ¶E½¢ ®áÏaËa Ç¡µÁ µ¡Ÿe½¢ "
            "®áÏaËa Ç¡Ÿi ÐqA ’Ÿa­A¶a..");
   puts_han("\n<Ins mode> <CapsLock> <NumLock> <ScrlLock> "
             "<Alt> <Ctrl> <LeftShift> <RightShift>");
   while(1)
   {
      keyflag = get_keyflag_han();
      /* <LeftShift> Ç¡µÁ <RightShift> Ç¡ˆa •·¯¡µA ’‰a»¡¡e */
      /* žÏaŸi ¨a¹a aˆe”a.                                */
      if(is_keyflag_han(LEFT_SHIFT | RIGHT_SHIFT)) break;
      gotoxy_han(0, 4);
      /* Ç¡¥¡—a Ïiœ‹a· ˆb §¡ËaŸi Îa¯¡Ðe”a. */
      /* <INS> Ç¡ˆa ’‰v“e»¡ ˆñ¬aÐaa¡e INS_MODEµÁ §¡Ëa AND  */
      /* Ð´¡ ´i ® ·¶”a. INSµÁ §¡Ëa ANDÐa¡e ´e –E”a.        */
      printf_han("%5d %10d %9d %10d %8d %6d %8d %12d",
                 (keyflag & INS_MODE)    ? 1 : 0,
                 (keyflag & CAPS_LOCK)   ? 1 : 0,
                 (keyflag & NUM_LOCK)    ? 1 : 0,
                 (keyflag & SCRL_LOCK)   ? 1 : 0,
                 (keyflag & ALT)         ? 1 : 0,
                 (keyflag & CTRL)        ? 1 : 0,
                 (keyflag & LEFT_SHIFT)  ? 1 : 0,
                 (keyflag & RIGHT_SHIFT) ? 1 : 0);
   }

   /* 2 ”e‰ : set_keyflag_han() */

   puts_han("\n\n<NumLock>, <CapsLock>, <ScrlLock> Ç¡ˆa ¡¡– "
            "’‰a»¥ ˆõÀáœñ  e—i´á ¥¡‰V¯s“¡”a.");
   set_keyflag_han(NUM_LOCK | CAPS_LOCK | SCRL_LOCK);
   puts_han("”a·q ”e‰¡ ñ´áˆaa¡e ¶E½¢ ®áÏaËa Ç¡µÁ µ¡Ÿe½¢ "
            "®áÏaËa Ç¡Ÿi ÐqA ’Ÿa­A¶a..");
   /* Ç¡¥¡—a Ïiœ‹aŸi Á¡‹¡ÑÁÐe”a. */
   while(!is_keyflag_han(LEFT_SHIFT || RIGHT_SHIFT)) ;

   /* 3”e‰ : toggle_keyflag_han(); */

   puts_han("\nÇ¡¥¡—aµA ·¶“e œ‘ÏaŸi Œq¨bˆáa ¥¡‰V¯s“¡”a.");
   puts_han("”a·q ”e‰¡ ñ´áˆaa¡e ¶E½¢ ®áÏaËa Ç¡µÁ µ¡Ÿe½¢ "
            "®áÏaËa Ç¡Ÿi ÐqA ’Ÿa­A¶a..");
   /* Ç¡¥¡—a Ïiœ‹aŸi Á¡‹¡ÑÁÐe”a. */
   set_keyflag_han(0);
   /* œ‘Ïaˆa ·¶“e §¡ËaŸi ‰­¢ É¡‹iÐe”a. */
   while(!is_keyflag_han(LEFT_SHIFT || RIGHT_SHIFT))
   {
      toggle_keyflag_han(NUM_LOCK);
      delay(300);
      toggle_keyflag_han(CAPS_LOCK);
      delay(300);
      toggle_keyflag_han(SCRL_LOCK);
      delay(300);
   }

   /* Ç¡¥¡—a Ïiœ‹aŸi ¥¢ŠÐe”a. */
   set_keyflag_han(org_keyflag);
   close_han();
}
