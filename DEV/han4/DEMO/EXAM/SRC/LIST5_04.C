#include <conio.h>

void main(void)
{
   int x, y;
   int code;

   clrscr();
   gotoxy(33, 3);
   puts("ASCII code table");

   for(code = 0; code < 256; code++)
   {
      x = 21 + (code % 20) * 2;
      y = 6 + code / 20;
      gotoxy(x, y);
      putch(code);
   }

   gotoxy(21, 21);
   puts("Type any key");
   if(!getch()) getch();
}
