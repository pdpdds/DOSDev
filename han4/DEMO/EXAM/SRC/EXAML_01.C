#include "hanio.h"
#include "misc.h"
#include "keydef.h"

void main(void)
{
   char string[MAX_STRLEN+1], backup[MAX_STRLEN+1];
   if(init_han() != OK_HAN) exit(1);

   while(1)
   {
      puts_han("¢…¸aµi·i ·³bÐa­A¶a.(¹·ža = <Esc>)");
      if(gets_han(string) == ESC) break;
      /* lefttrim‰Á righttrim·i Ñ¡Â‰Ða¡e ¢…¸aµi· ¶w·¡ */
      /* ¤aŽá£a¡ £¡Ÿ¡ ¥¢¬aŸi Ð ‘½´a´¡ Ðe”a.            */
      strcpy(backup, string);

      printf_han("·³b¤h·e  ¢…¸aµi  = [%s]\n", string);
      printf_han("¹ÁÃb ‰·¤‚ ¬b¹A Ò = [%s]\n", lefttrim(string));
      strcpy(string, backup);
      printf_han("¶Ãb ‰·¤‚ ¬b¹A Ò = [%s]\n", righttrim(string));
      /* ·¡£¡ right trim ·e –E ¬wÈ·¡£a¡ left trim  e */
      /* Ð º¡e ¹Á¶ ‰·¤‚·¡ ¡¡– ¬b¹A–E”a.            */
      printf_han("¹Á¶ ‰·¤‚ ¬b¹A Ò = [%s]\n\n", lefttrim(string));
   }

   close_han();
}
