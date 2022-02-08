#include "hanio.h"

void main(void)
{
   char string[MAX_STRLEN+1];

   if(init_han() != OK_HAN) exit(1);

   /* ¶å•¡¶Ÿi ¬é¸÷Ðe”a. */
   set_window_han(10, 5, 69, 20);
   gotoxy_han(20, 5);
   puts_han("´e");
   /* ¶å•¡¶Ÿi ”a¯¡ ¬é¸÷Ðe”a. ·¡ ˜ ¶å•¡¶ µwµb·e (75, 10)µA  "´e" ·¡œa“e */
   /* ¢…¸aµi·i Â‰bÐi ® ·¶•¡¢ ¸s´a º´á´¡ Ðe”a. µa‹¡¬á“e (75, 10) - (79,*/
   /* 12)¡ Ðaµv”a.                                                       */
   set_window_han(75, 10, 79, 12);
   gotoxy_han(0, 0);
   puts_han("¤aŒ{");
   /* ¶å•¡¶Ÿi ¶¥œ”¡ ¥¢ŠÐe”a. */
   set_window_han(10, 5, 69, 20);

   getch();
   close_han();
}
