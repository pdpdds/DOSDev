#include "hanio.h"

void main(void)
{
   char string[MAX_STRLEN+1];

   if(init_han() != OK_HAN) exit(1);
   status_on_han();

   puts_han("»¡‹q·e Ðe/µw ¸åÑÅ Ç¡ˆa ¸b•·–S“¡”a. <Left Shift>+<Space> Ç¡Ÿi "
            "’Ÿa¡e Ðe/µw ¸åÑÅ·¡ –A“e»¡ ¯¡ÐñÐ ¥¡­A¶a..");
   gets_han(string);

   puts_han("\n·¡¤åµA“e µw¢… e ·³b ˆa“wÐa•¡¢  e—i´á ¥¡‰V¯s“¡”a.");
   puts_han("´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();

   /* µw¢… ·³b ¬wÈ¡  e—e Ò Ðe/µw ¸åÑÅ Ç¡· ¸b•··i ‹q»¡¯¡Ç¥”a. */
   set_to_eng_mode();
   disable_han_change();
   default_han_change_func();

   puts_han("\nµw¢… e ·³bÐi ® ·¶“e ¬wÈ·³“¡”a. Ðe/µw ¸åÑÅ Ç¡ˆa ¸b•·–A“e»¡ "
            "¯¡ÐñÐ ¥¡­A¶a..");
   gets_han(string);

   puts_han("\n·¡¤åµA“e Ðe‹i e ·³b ˆa“wÐa•¡¢  e—i´á ¥¡‰V¯s“¡”a.");
   puts_han("´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();

   /* Ðe‹i ·³b ¬wÈ¡  e—e”a. µa¸åÓ¡ Ðe/µw ¸åÑÅ Ç¡“e ¸b•·–A»¡ ´g“e”a. */
   set_to_han_mode();
   default_han_change_func();

   puts_han("\nÐe‹i e ·³bÐi ® ·¶“e ¬wÈ·³“¡”a. Ðe/µw ¸åÑÅ Ç¡ˆa ¸b•·–A“e»¡ "
            "¯¡ÐñÐ ¥¡­A¶a..");
   gets_han(string);

   puts_han("\n·¡¤åµA“e ”a¯¡ Ðe‹i‰Á µw¢… ¡¡–Ÿi ·³bÐi ® ·¶•¡¢  e—i´á "
            "¥¡‰V¯s“¡”a.");
   puts_han("´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();

   /* Ðe/µw ¸åÑÅ Ç¡· ¸b•··i Ðá¶wÐe Ò µw¢… ·³b ¬wÈ¡ ¤aŽ…”a. */
   enable_han_change();
   set_to_eng_mode();
   default_han_change_func();

   puts_han("\nÐe‹i‰Á µw¢… ¡¡– ·³bÐi ® ·¶“e ¬wÈ·³“¡”a. "
            "Àá·qµA“e µw¢… ·³b ¬wÈˆa –S“¡”a. Ðe/µw ¸åÑÅ Ç¡ˆa "
            "¸b•·–A“e»¡ ¯¡ÐñÐ ¥¡­A¶a..");
   gets_han(string);

   close_han();
}
