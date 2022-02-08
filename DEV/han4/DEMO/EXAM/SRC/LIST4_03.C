#include "hanio.h"

void main(void)
{
   char string[MAX_STRLEN+1];

   if(init_han() != OK_HAN) exit(1);

   /* ¶å•¡¶Ÿi ¬é¸÷Ðe”a. */
   set_window_han(10, 5, 69, 20);
   gotoxy_han(20, 5);
   puts_han("´e");
   /* ¶å•¡¶Ÿi ‹a”¡ –‰¡ ¸é” ¹ÁÎa¡ Â‰bÐa“e Ðq® xputs_han() ·i ¬a¶wÐaµa */
   /* ¤a¡ (75, 10)µA Â‰bÐe”a.                                             */
   xputs_han(75, 10, "¤aŒ{");

   getch();
   close_han();
}
