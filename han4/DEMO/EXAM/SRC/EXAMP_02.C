#include <dos.h>
#include "hanio.h"
#include "extendio.h"

void main(void)
{
   struct time now;

   if(init_han() != OK_HAN) exit(1);

   /* ¯¡ˆe·i ŠÐaµa now µA ¸á¸wÐe”a.*/
   gettime(&now);

   /* printf_han ‰µi· Ðq®Ÿi ¬a¶wÐaµa ¯¡ˆe·i ”a´·Ðe ¤w¤ó·a¡ Â‰bÐe”a.*/
   printf_han("%2d ¯¡ %2d ¦… %2d Á¡",
              now.ti_hour, now.ti_min, now.ti_sec);
   xprintf_han( 0,  1, "%2d ¯¡ %2d ¦… %2d Á¡",
              now.ti_hour, now.ti_min, now.ti_sec);
   nprintf_han( 0,  2, 11, "%2d ¯¡ %2d ¦… %2d Á¡",
              now.ti_hour, now.ti_min, now.ti_sec);
   aprintf_han( 0,  3, FONT_OUTLINE, "%2d ¯¡ %2d ¦… %2d Á¡",
              now.ti_hour, now.ti_min, now.ti_sec);
   vprintf_han( 0,  4, MAGENTA, "%2d ¯¡ %2d ¦… %2d Á¡",
              now.ti_hour, now.ti_min, now.ti_sec);
   naprintf_han(0,  5, 8, FONT_VERT_DBL, "%2d ¯¡ %2d ¦… %2d Á¡",
              now.ti_hour, now.ti_min, now.ti_sec);
   nvprintf_han(0,  7, 14, CYAN, "%2d ¯¡ %2d ¦… %2d Á¡",
              now.ti_hour, now.ti_min, now.ti_sec);
   vaprintf_han(0,  8, RED, FONT_SHADE, "%2d ¯¡ %2d ¦… %2d Á¡",
              now.ti_hour, now.ti_min, now.ti_sec);
   vbprintf_han(0,  9, DARKGRAY, GREEN, "%2d ¯¡ %2d ¦… %2d Á¡",
              now.ti_hour, now.ti_min, now.ti_sec);
   eprintf_han( 0, 10, "%2d ¯¡ %2d ¦… %2d Á¡",
              now.ti_hour, now.ti_min, now.ti_sec);
   enprintf_han(0, 11, 14, "%2d ¯¡ %2d ¦… %2d Á¡",
              now.ti_hour, now.ti_min, now.ti_sec);

   xputs_han(0, 13, "´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();

   close_han();
}
