#include "hanio.h"
#include "extendio.h"

void main(void)
{
   if(init_han() != OK_HAN) exit(1);

   switch(get_video_seg())
   {
      case 0xb000 : puts_han("HGC 0 �A���� �a�w��");
                    break;
      case 0xb800 : puts_han("HGC 1 �A���� �a�w��");
                    break;
      case 0xa000 : puts_han("EGA/VGA �a�w��");
                    break;
   }

   puts_han("\n�a�� ǡ�a ���a�A�a..");
   if(!getch()) getch();
   close_han();
}
