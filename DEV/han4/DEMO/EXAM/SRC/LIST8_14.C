#include <dir.h>
#include "hanio.h"
#include "extendio.h"

void main(int argc, char **argv)
{
   char drive[MAXDRIVE], dir[MAXDIR], name[MAXFILE], ext[MAXEXT];
   char path[MAXDRIVE+MAXDIR];

   /* argv[0] µA¬á Ïa¡‹aœ‘·¡ ¯©Ð—–A“e —aœa·¡§a, —¡BÉ¡Ÿ¡Ÿi ŠÐe”a. */
   fnsplit(argv[0], drive, dir, name, ext);
   strcpy(path, drive);
   strcat(path, dir);
   /* BGI ÑÁ·©‰Á Í¥Ëa ÑÁ·©·i Ïa¡‹aœ‘·¡ ¯©Ð—–A“e —¡BÉ¡Ÿ¡µA¬á */
   /* Àx•¡¢ Ðe Ò init_han()·i Ñ¡Â‰Ðe”a.                     */
   set_bgi_font_path(path, path);

   /* Ðe‹i ÑÅ‰w·a¡ Á¡‹¡ÑÁÐe Ò ˆb¹· ¥e®—i· ˆt·i Â‰b */
   if(init_han() != OK_HAN)
   {
      printf("Can\'t initialize Hangul environment..\n");
      printf("drive      : %s\n",   drive);
      printf("directory  : %s\n",   dir);
      printf("program    : %s%s\n", name, ext);
      printf("BGI & font : %s\n",   path);
   }
   else
   {
      printf_han("—aœa·¡§a¡w : %s\n",   drive);
      printf_han("—¡BÉ¡Ÿ¡¡w : %s\n",   dir);
      printf_han("¯©Ð—ÑÁ·©¡w : %s%s\n", name, ext);
      printf_han("Ïa¡‹aœ‘·¡ ¯©Ð—–E —¡BÉ¡Ÿ¡“e %s·³“¡”a.\n\n", path);
      puts_han("´a¢ Ç¡a ’Ÿa­A¶a..");
      if(!getch()) getch();
      close_han();
   }
}
