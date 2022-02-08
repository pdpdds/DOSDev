#include "hanio.h"

void main(void)
{
   char string[MAX_STRLEN+1];
   if(init_han() != OK_HAN) exit(1);
   status_on_han();

   puts_han("»¡‹q·e µw¢… ·³b ¬wÈ·³“¡”a. Ðe‹i ·³b ¬wÈ¡ ¤aŽ‰V¯s“¡”a.");
   puts_han("´a¢ ‹i¸aa ·³bÐ ¥¡­A¶a..");
   gets_han(string);

   /* ¬a¶w¸aˆa ´a¢ Ç¡a ’Ÿa¡e Ðe‹i ·³b ¬wÈ¡ ¸åÑÅÐe ”a·q, */
   /* default_han_change_func()·i Ñ¡Â‰Ðe”a.                  */
   set_to_han_mode();
   default_han_change_func();

   puts_han("\n»¡‹q·e Ðe‹i ·³b ¬wÈ·³“¡”a. "
            "”a¯¡ µw¢… ·³b ¬wÈ¡ ¤aŽ‰V¯s“¡”a.");
   puts_han("´a¢ ‹i¸aa ·³bÐ ¥¡­A¶a..");
   gets_han(string);

   /* ¬a¶w¸aˆa ´a¢ Ç¡a ’Ÿa¡e µw¢… ·³b ¬wÈ¡ ¸åÑÅÐe ”a·q, */
   /* default_han_change_func()·i Ñ¡Â‰Ðe”a.                   */
   set_to_eng_mode();
   default_han_change_func();

   puts_han("\n\n´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();

   close_han();
}
