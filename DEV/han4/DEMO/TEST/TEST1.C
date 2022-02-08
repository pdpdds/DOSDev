#include <graphics.h>

void main(void)
{
   int gd, gm;

   gd = VGA;
   gm = VGAHI;
   initgraph(&gd, &gm, "");
   circle(100, 100, 50);
   getch();
   system("test2");
   closegraph();
}
