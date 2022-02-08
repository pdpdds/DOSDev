#include <stdio.h>
#include <conio.h>
#include "hanio.h"
#include "misc.h"
#include "keydef.h"

void main(int argc, char *argv[])
{
   FILE *stream;
   char string[128];
   int  source_code;
   char ch;

   if(argc < 2)         /* ÑÁ·©¡w‰Á Ðe‹i Å¡—aˆa »¡¸÷–A»¡ ´g´v·i ˜ */
   {
      puts("Usage : examc_06 <file_name> [/j/k/7]");
      puts("        /j = JOHAP");
      puts("        /k = KSC5601");
      puts("        /7 = BIT7");
      exit(1);
   }

   if(argc < 3)         /* Ðe‹i Å¡—aˆa »¡¸÷–A»¡ ´g´v”a¡e */
      source_code = JOHAP;
   else if(!strcmp(argv[2], "/j"))      /* ¹¡ÐsÑw·¡œa¡e */
      source_code = JOHAP;
   else if(!strcmp(argv[2], "/k"))      /* µÅ¬÷Ñw·¡œa¡e */
      source_code = KSC5601;
   else if(!strcmp(argv[2], "/7"))      /* À÷‰Àå Ðe‹i·¡œa¡e */
      source_code = BIT7;
   else                                 /* ¸i¡µ »¡¸÷–A´ö”a¡e */
      source_code = JOHAP;

   if((stream = fopen(argv[1], "rt")) == NULL)  /* ÑÁ·©·i µ¡ÏeÐe”a. */
   {
      puts("file not found");
      exit(2);
   }

   if(init_han() != OK_HAN) exit(4);

   while(!feof(stream))
   {
      if(fgets(string, 127, stream) == NULL)
         break;
      else
      {
         if(source_code != JOHAP) convert_han_code(string, source_code, JOHAP);
         cputs_han(string);
      }

      if(kbhit())                               /* Ç¡ˆa ’‰v”a¡e */
      {
         if((ch = getch()) == 0) getch();       /* ’‰Ÿ¥ Ç¡Ÿi ·ª“e”a.         */
         if(ch == _ESC)                         /* <Esc>ˆa ’‰Ÿ¡¡e {…”a.    */
            break;
         else if(ch == _SPACE)                  /* <Space>ˆa ’‰Ÿ¡¡e ´a¢ Ç¡  */
         {                                      /* a ’Ÿi ˜Œa»¡ ‹¡”aŸ¥”a.  */
            if(!getch()) getch();
         }
      }
   }

   if(feof(stream))                             /* <Esc>Ÿi ’Ÿa»¡ ´g‰¡ ¹A”  */
   {                                            /* ¡ {Œa»¡ ÑÁ·©·i ·ª´ö”a¡e */
      set_attr_han(FONT_SHADE);                 /* ´a¢ Ç¡a ’Ÿi ˜Œa»¡  ‹¡ */
      puts_han("<{>");                         /* ”aŸ¥”a.                   */
      set_attr_han(FONT_INVERSE);
      puts_han("\n\n´a¢ Ç¡a ’Ÿa­A¶a");
      if(!getch()) getch();
   }

   fclose(stream);
   close_han();
}
