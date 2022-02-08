#include <stdlib.h>
#include "hanio.h"
#include "misc.h"

void main(void)
{
   char string[3+1];

   if(init_han() != OK_HAN) exit(1);

   /* a·¡Ÿi ·³b¤h·a£a¡ ­A ¸aŸ¡ ·¡¬w ·³b¤h·i Ï©¶aˆa ´ô”a. */
   set_max_strlen_han(3);
   while(1)
   {
      cputs_han("”w¯¥· a·¡“e? --- ");
      gets_han(string);
      if(is_number(string)) break;
      puts_han("\7®•¸a e ·³bÐa­A¶a..");
   }

   /* ·©”e žÏaŸi ¨a¹a aµ¡¡e string·e ®•¸aœa“e ˆõ·i ÑÂ¯¥Ði */
   /* ® ·¶‹¡ ˜¢…µA ´e¯±Ða‰¡ atoi() Ðq®Ÿi Ñ¡Â‰Ði ®  ·¶”a. */
   printf_han("”w¯¥· a·¡“e %d ­A·³“¡”a.\n\n", atoi(string));
   puts_han("´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();

   close_han();
}
