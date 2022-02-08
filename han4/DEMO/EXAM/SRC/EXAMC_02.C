#include "hanio.h"
#include "extendio.h"
#include "keydef.h"

void main(void)
{
   char hangul_code[3], hanja_code[3];
   int  ret_code;

   if(init_han() != OK_HAN) exit(1);

   puts_han("Ðe¸a¡ ¥eÑÅÐi Ðe‹i·i ·³bÐa­A¶a.");

   set_max_strlen_han(2);       /* ÂA” 2¸aŒa»¡ ·³b¤h·i ® ·¶•¡¢ Ðe”a. */
   set_to_han_mode();           /* Ðe‹i ·³b ¬wÈ¡ ¸åÑÅ */
   disable_han_change();        /* Ðe/µw ¸åÑÅ Ç¡ˆa ¸b•·–A»¡ ´g•¡¢ Ðe”a. */
   gets_han(hangul_code);
   puts_han("\n\n");

   ret_code = choose_hanja(hangul_code, hanja_code);
   switch(ret_code)
   {
      case -1    : puts_han("Ð”wÐa“e Ðe¸aˆa ´ô¯s“¡”a."); break;
      case -2    : puts_han("¡A¡¡Ÿ¡ˆa ¦¹¢Ðs“¡”a."); break;
      case ESC   : puts_han("¬åÈ‚·i Âá­¡Ða­v¯s“¡”a."); break;
      case ENTER : printf_han("<%s> ¸aŸi ¬åÈ‚Ða­v¯s“¡”a.\n", hanja_code); break;
   }

   puts_han("\n\n´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();

   close_han();
}
