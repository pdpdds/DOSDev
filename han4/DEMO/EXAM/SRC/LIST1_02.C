#include <graphics.h>

void main(void)
{
   int gd, gm;

   gd = DETECT;
   gm = 0;
   initgraph(&gd, &gm, "");

   if(graphresult() != grOk)
   {
      puts("Graphics error");
      exit(1);
   }

   puts("�e�i�� �b�A���s���a.");
   circle(100, 100, 100);
   getch();
   closegraph();
}
