#include "hanio.h"

void main(void)
{
   char bgi_path[128], font_path[128];

   printf("BGI directory  = ? ");
   gets(bgi_path);
   printf("font directory = ? ");
   gets(font_path);
   set_bgi_font_path(bgi_path, font_path);
   if(init_han() != OK_HAN) exit(1);

   puts_han("´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();
   close_han();
}
