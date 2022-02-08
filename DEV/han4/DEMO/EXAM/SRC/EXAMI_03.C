#include "hanio.h"

void main(void)
{
   char string[MAX_STRLEN+1];

   if(init_han() != OK_HAN) exit(1);

   if(is_han_mode())
      puts_han("»¡‹q·e Ðe‹i ·³b ¬wÈ·³“¡”a.");
   else
      puts_han("»¡‹q·e µw¢… ·³b ¬wÈ·³“¡”a.");
   puts_han("´a¢ ‹i¸aa ·³bÐ ¥¡­A¶a..");
   gets_han(string);

   puts_han("·¡¹A Ðe‹i ·³b ¬wÈ¡ ¤aŽ‰V¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();
   /* ¬a¶w¸aˆa ´a¢ Ç¡a ’Ÿa¡e Ðe‹i ·³b ¬wÈ¡ ¸åÑÅÐe”a. */
   set_to_han_mode();

   puts_han("»¡‹q·e Ðe‹i ·³b ¬wÈ·³“¡”a.");
   puts_han("´a¢ ‹i¸aa ·³bÐ ¥¡­A¶a..");
   gets_han(string);
   puts_han("·¡¹A µw¢… ·³b ¬wÈ¡ ¤aŽ‰V¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();
   /* ¬a¶w¸aˆa ´a¢ Ç¡a ’Ÿa¡e µw¢… ·³b ¬wÈ¡ ¸åÑÅÐe”a. */
   set_to_eng_mode();

   puts_han("»¡‹q·e µw¢… ·³b ¬wÈ·³“¡”a.");
   puts_han("´a¢ ‹i¸aa ·³bÐ ¥¡­A¶a..");
   gets_han(string);

   close_han();
}
