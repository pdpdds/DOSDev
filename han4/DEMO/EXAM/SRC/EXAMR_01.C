#include "hanio.h"
#include "extendio.h"

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   puts_han("Ðe‹i Í¥Ëa“e ‰¡—¢ÁA¡ ASCII FONT“e italicÁA¡ ¤aŽ‰V¯s“¡”a.");
   puts_han("´a¢ Ç¡a ’Ÿa­A¶a..\n");
   if(!getch()) getch();
   /* hangod1.fnt : Ðe‹i· Í¥Ëa ÑÁ·©(‰¡—¢ÁA)          */
   /* engital.fnt : ´a¯aÇ¡ ¢…¸a· Í¥Ëa ÑÁ·©(·¡È‰Ÿ¢ÁA) */
   if(replace_font("hangod1.fnt", "engital.fnt", "") != OK_HAN)
      puts_han("\7Í¥ËaŸi ¤aŽ‰ ® ´ô¯s“¡”a..");

   puts_han("”a¯¡ ¶¥œ· FONT¡ •©a ‘½‰V¯s“¡”a.");
   puts_han("´a¢ keya ’Ÿa­A¶a..\n");
   if(!getch()) getch();
   if(replace_font("hangul.fnt", "english.fnt", "") != OK_HAN)
      puts_han("\7Í¥ËaŸi ¤aŽ‰ ® ´ô¯s“¡”a..");

   puts_han("”a¯¡ ¶¥œ· FONT¡ •©´a µÖ¯s“¡”a.");
   puts_han("´a¢ keya ’Ÿa­A¶a..\n");
   if(!getch()) getch();
   close_han();
}
