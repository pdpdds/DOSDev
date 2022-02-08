
#include "hanio.h"
#include "extendio.h"

void main(void)
{
   char *string = "­·‘¡·¥·e ·¡œé ª…, »b¬aŸi Ðe ®…¬aˆa ´á“a ®…¬aµv“e»¡•¡ ¢ˆ»¡ "
                  "´g´v”a. ”a e \"·¡ Ðb•¡¥w—i·¡ Â‰¸å·i Ða¡e •¢Ÿ³Š…·¥ˆa a¤i·¥"
                  "ˆaµA ˆa”qÐa‰¡ ·¶·i ¬w”â·¡ ˆ{·e À÷‘e—i· –áÉ·®Ÿi  õ»¡‰A ±¡"
                  "´a ñ‹©ˆáœa\"‰¡ ´áŸa””å ‹a ¡A‹¡·³·¡ ¸qŒe  áŸ¡µA ˜áµ©œv·i "
                  "ª…·¡´ö”a. ‹aœáa µb¯¡ ˆaµt·e ¬—ˆb·¡ —i´ö”a.";
   char buff[51];
   int index = 0, buff_index;

   if(init_han() != OK_HAN) exit(1);

   while(string[index] != 0)    /* ¢…¸aµi· {·¡ ´a“¥ •·´e ¤e¥¢Ðaœa. */
   {
      /* 50¸a³¢ ¤áÌá buffµA ¥¢¬aÐ –… Ò buffŸi ¸÷iÐe”a. */
      buff_index = 0;
      while(buff_index < 50 && string[index] != 0)
         buff[buff_index++] = string[index++];
      buff[buff_index] = 0;

      /* ‰·¤‚·¡ ¬s·³–E ‰w¶ ‹a ® eÇq string· indexŸi ˆq­¡¯¡Ç¥”a. */
      index -= arrange_string(buff, 50);
      puts_han(buff);
   }

   puts_han("\n´a¢ Ç¡a ’Ÿa­A¶a.");
   if(!getch()) getch();

   close_han();
}
