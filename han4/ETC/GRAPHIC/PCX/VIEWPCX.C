#include "d:/work/han/test/hanio.h"
#include "pcx.h"

void main(int argc, char **argv)
{
   int mode, ret, i;

   if((mode = choose_gr_mode()) == -1) exit(1);
   if(_init_han(mode) != OK_HAN) exit(2);

   if((ret = show_pcx(0, 0, argv[1])) != OK_PCX)
   {
      _close_han(mode);
      puts("Can't view this file.");
      printf("error code is %d\n", ret);
      exit(3);
   }

   if(!getch()) getch();
   clrscr_han();
   for(i = 0; i < 16; i++) vputs_han(0, i, "testing palette..", i);

   if(!getch()) getch();
   _close_han(mode);
}
