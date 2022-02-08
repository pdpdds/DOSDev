#include "hanio.h"

void main(void)
{
   int year, month, day;

   if(init_han() != OK_HAN) exit(1);

   cputs_han("i¼aŸi ·³bÐa­A¶a(YY MM DD) -- ");
   scanf_han("%d %d %d", &year, &month, &day);
   printf_han("µ¡“i·e %d ‘e %d ¶© %d ·©·³“¡”a.\n", year, month, day);

   puts_han("\n´a¢ Ç¡a ’Ÿa­A¶a..");
   if(!getch()) getch();
   close_han();
}
