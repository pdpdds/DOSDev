#include <dos.h>
#include "hanio.h"
#include "extendio.h"

/* Ðq® ¶¥Ñw */
void my_redirect_func(char *string);

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   /* Â‰b–A“e •A·¡Èáˆa my_redirect_func·a¡ ¸å”i–A•¡¢ Ðe”a.      */
   /* puts_han(), cputs_han(), printf_han() ¡¡– ¸»¡Ð··¡ ˆa“wÐa”a.*/
   redirect_func = my_redirect_func;
   printf_han("*****\n");
   puts_han("·¡ Ïa¡‹aœ‘·e Â‰b ¸»¡Ð··i  ·¡¶wÐaµa");
   puts_han("Â‰b ­¢•¡Ÿi º‰·¡“e ¤w¤ó·i ¥¡µa º““¡”a.");
   cputs_han("*****\n");
   /* Â‰b–A“e •A·¡Èáˆa ´a¢ Ðq®µA•¡ ¸å”i–A»¡ ´g•¡¢ Ðe”a. */
   redirect_func = NULL;
   puts_han("\n´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();

   close_han();
}

void my_redirect_func(char *string)
/* ¸»¡Ð·–E •A·¡ÈáŸi ÀáŸ¡Ða“e Ðq® */
{
   /* ‰·¤‚ ¢…¸a ¶A· ¢…¸aˆa Â‰b–I ˜ a”a 'Ë¢' ­¡Ÿ¡Ÿi …”a. */
   if(strcmp(string, " "))
   {
      sound(300);
      delay(10);
      nosound();
      delay(190);
   }
}
