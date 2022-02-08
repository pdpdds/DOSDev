#include "hanio.h"

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   /* set_attr_han(), get_attr_han() Ðq® ¬a¶w */
   set_attr_han(FONT_SHADE);
   if(get_attr_han() == FONT_SHADE)
      puts_han("·qµw ¢…¸a·³“¡”a.");
   else
      puts_han("·qµw ¢…¸aˆa ´a“³“¡”a.");

   /* ¸åµb ¥e® attr_han ¬a¶w */
   attr_han = FONT_OUTLINE | FONT_INVERSE; 
   puts_han("\n¤e¸å–E ¶A‰Â¬å ¢…¸a·³“¡”a.");
   if(attr_han == FONT_OUTLINE | FONT_INVERSE)
      puts_han("¤e¸å–E ¶A‰Â¬å ¢…¸a·³“¡”a.");
   else
      puts_han("¤e¸å–E ¶A‰Â¬å ¢…¸aˆa ´a“³“¡”a.");

   /* FONT_INVERSE ­¢¬÷·¡ ¬é¸÷–A´ö“e»¡ ¹¡¬aÐe”a.*/
   /* ‹a· attr_han == FONT_INVERSE¡ Ða¡e  ´e  */
   /* –E”a.                                     */
   if(attr_han & FONT_INVERSE)
      puts_han("\nFONT_INVERSE ­¢¬÷·¡ ¬é¸÷–A´ö¯s“¡”a.");
   else
      puts_han("\nFONT_INVERSE ­¢¬÷·¡ ¬é¸÷–A»¡ ´g´v¯s“¡”a.");
   if(attr_han & FONT_OUTLINE)
      puts_han("FONT_OUTLINE ­¢¬÷·¡ ¬é¸÷–A´ö¯s“¡”a.");
   else
      puts_han("FONT_OUTLINE ­¢¬÷·¡ ¬é¸÷–A»¡ ´g´v¯s“¡”a.");

   attr_han = FONT_NORMAL;
   puts_han("\n´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();
   close_han();
}
