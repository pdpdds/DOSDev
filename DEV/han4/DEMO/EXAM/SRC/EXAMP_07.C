#include "hanio.h"
#include "extendio.h"

void main(void)
{
   char string[] = "Ðe‹i œa·¡§aœáŸ¡ <Ðe> 4.1";

   if(init_han() != OK_HAN) exit(1);

   /* puts_han ‰µi· Ðq®Ÿi ¬a¶wÐaµa string ·i ”a´·Ðe ¤w¤ó·a¡ Â‰bÐe”a. */
   puts_han(string);
   cputs_han(string);
   xputs_han( 0,  2, string);
   nputs_han( 0,  3, string, 11);
   aputs_han( 0,  4, string, FONT_INVERSE);
   vputs_han( 0,  5, string, MAGENTA);
   naputs_han(0,  6, string, 4, FONT_VERT_DBL);
   nvputs_han(0,  8, string, strlen(string), YELLOW);
   vaputs_han(0,  9, string, BLUE, FONT_SHADOW);
   vbputs_han(0, 10, string, RED, LIGHTGRAY);
   eputs_han( 0, 11, string);
   enputs_han(0, 12, string, 11);

   xputs_han(0, 14, "´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();

   close_han();
}
