#include "hanio.h"
#include "extendio.h"

void main(void)
{
   int i;

   if(init_han() != OK_HAN) exit(1);

   /* ¬‚·i ¤aŽ´á ˆa¡a ®»¢¬å·i ¹Á¶¡ 200ˆ³¢ ‹aŸ¥”a. */
   for(i = 0; i < 200; i++)
   {
      draw_vert_line(i,           0,   i, i % 16);
      draw_vert_line(420 - i,     0,   i, i % 16);
   }

   /* ¬‚·i ¤aŽ´á ˆa¡a ®»¢¬å· £»µA ®Íw¬å·i 100ˆ ‹aŸ¥”a. */
   for(i = 0; i < 100; i++)
   {
      draw_horiz_line(210 - i, 220 + i, i << 1, i % 16);
   }

   vbputs_han(0, 13, "´a¢ Ç¡a ’Ÿa­A¶a..", RED, LIGHTGRAY);
   if(!getch()) getch();

   close_han();
}
