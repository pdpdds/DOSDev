#include "hanio.h"
#include "extendio.h"

void main(void)
{
   char *(han_font[6]);         /* Ça‹¡ˆa 6·¥ Í¡·¥Èá ¤µi(Í¥Ëa¶w) */
   int error, i;

   /* 6ˆa»¡ Ðe‹i Í¥ËaŸi Ñe—¡BÉ¡Ÿ¡µA¬á ·ª´á —i·¥”a. */
   error = 0;
   if(read_han_font(&han_font[0], "hangod1.fnt")  != OK_HAN) error = 1;
   if(read_han_font(&han_font[1], "hangod2.fnt")  != OK_HAN) error = 1;
   if(read_han_font(&han_font[2], "hanmyung.fnt") != OK_HAN) error = 1;
   if(read_han_font(&han_font[3], "hanson1.fnt")  != OK_HAN) error = 1;
   if(read_han_font(&han_font[4], "hanson2.fnt")  != OK_HAN) error = 1;
   if(read_han_font(&han_font[5], "hansam.fnt")   != OK_HAN) error = 1;
   /* Ðaaœa•¡ ¹A”¡ ·ª»¡ ¡µÐe Í¥Ëaˆa ·¶·a¡e ¯©Ð—·i º—»¡Ðe”a. */
   if(error)
   {
      puts("font reading error..");
      exit(1);
   }
   if(init_han() != OK_HAN) exit(1);

   puts_han("´a¢ Ç¡a ’Ÿa¡e Í¥ËaŸi ¤aŽ´á Â‰bÐs“¡”a.\n");
   if(!getch()) getch();
   /* 6ˆa»¡ Í¥ËaŸi ¬a¶wÐaµa ¢…¸aµi·i Â‰bÐe”a. */
   for(i = 0; i < 6; i++)
   {
      /* µw¢…Í¥ËaµÁ Ëb®¢…¸a Í¥Ëa“e ‹a”¡ –…”a. */
      set_font(han_font[i], NULL, NULL);
      puts_han("Ðe‹i œa·¡§aœáŸ¡ <Ðe> 4.1");
   }

   /* Í¥ËaµA Ði”w–E ¡A¡¡Ÿ¡Ÿi free¯¡Ç¥ Ò Ðe‹i Í¥Ëa e */
   /* ”a¯¡ ¶¥œ· ˆõ·a¡ –A•©a ‘½“e”a.              */
   for(i = 0; i < 6; i++) free(han_font[i]);
   return_to_org_font(1, 0, 0);

   puts_han("\n´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();
   close_han();
}
