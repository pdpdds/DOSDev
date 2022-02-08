#include "hanio.h"
#include "extendio.h"

void func1(int y);

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   /* µAœá ¡A¯¡»¡Ÿi ¥¡‹¡ ¶áÐ ·©¦œá ¸i¡µ–E ·¥¸aŸi ¸å”iÐe”a. */
   func1(40);
   puts_han("´a¢ Ç¡a ’Ÿa­A¶a");
   if(!getch()) getch();

   close_han();
}

void func1(int y)
{
   if(y > get_abs_maxy_han())
   {
      set_attr_han(FONT_INVERSE);
      /* error_exit_msg“e ¸åµb ¥e®·¡”a. */
      error_exit_msg = "y ¹ÁÎaˆa ¤ñ¶áŸi ñ´ö¯s“¡”a!!!";
      /* µAœáˆa ¤i¬—Ðe Ðq®“e main()·¡‰¡ ¹·ža Å¡—a“e 0·¡”a. */
      error_exit_han("main()", 0);
   }

   gotoxy_han(y, 0);
   puts_han("y ¹ÁÎaˆa ¤aŸa‰A »¡¸÷–A´ö¯s“¡”a.");
}
