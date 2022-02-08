#include "hanio.h"
#include "keydef.h"

void main(void)
{
   char string[MAX_STRLEN+1];

   if(init_han() != OK_HAN) exit(1);

   /* ¬wÈ œa·¥·¡ aÈaa‰A Ðe”a. */
   puts_han("´a¢ Ç¡a ’Ÿa¡e ¬wÈ œa·¥·¡ aÈas“¡”a.");
   if(!getch()) getch();
   status_on_han();

   puts_han("¬wÈ œa·¥·¡ aÈav¯s“¡”a.\n");
   /* ¢…¸aµi·i ¤e¥¢Ð¬á ·³b¤h“e”a. */
   while(1)
   {
      puts_han("¢…¸aµi·i ·³bÐ ¥¡­A¶a.({ = <Esc>)");
      if(gets_han(string) == ESC) break;
   }

   /* ¬wÈ œa·¥·i ¬aœa»¡‰A Ðe”a. */
   puts_han("\n´a¢ Ç¡a ’Ÿa¡e ¬wÈ œa·¥·¡ ¬aœa»³“¡”a.");
   if(!getch()) getch();
   status_off_han();

   puts_han("¬wÈ œa·¥·¡ ¬aœa¹v¯s“¡”a.\n");
   puts_han("´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();
   close_han();
}
