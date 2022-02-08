#include "hanio.h"
#include "keydef.h"

void main(void)
{
   char code1, code2;

   if(init_han() != OK_HAN)
   {
      puts("Hangul Environment Error..");
      exit(1);
   }

   puts_han("{·i ‹¡ ¶áÐ¬á“e <ESC> Ç¡Ÿi ’Ÿa­A¶a.\n\n\n");

   do
   {
      puts_han("´a¢ Ç¡a ’Ÿa­A¶a.");

      code1 = getch();
      if(code1 == _ESC) break;  /* _ESC“e <ESC> Ç¡· ´a¯aÇ¡ Å¡—aˆt       */
                                /* <ESC> Ç¡Ÿi ’Ÿa¡e žÏaŸi ¨a¹a aˆe”a. */

      if(code1 == 0)    /* ÑÂ¸w Ç¡Ÿi ’‰œö·a¡e */
         printf_han("»¡‹q ’Ÿe Ç¡“e ÑÂ¸w Ç¡¡¬á ");
      else
         printf_han("»¡‹q ’Ÿe Ç¡“e ·©¤e Ç¡¡¬á ");

      printf_han("Å¡—a“e '0x%02X'", code1);
      if(code1 == 0)    /* ÑÂ¸w Ç¡Ÿi ’‰œö·a¡e Ðe ¤å ”á Ç¡Ÿi ·ª“e”a. */
      {
         code2 = getch();
         printf_han(" + '0x%02X'", code2);
      }
      printf_han("·³“¡”a.\n\n");
   }
   while(1);

   close_han();
}
