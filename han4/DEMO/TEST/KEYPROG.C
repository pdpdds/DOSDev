#include <process.h>
#include "hanio.h"

void main(void)
{
   int state;

   if(_init_han(INIT_VGA) != OK_HAN) exit(1);
   puts_han("This is main program");
   puts_han("Press any key, then I call sub program.");
   if(!getch()) getch();

   if((state = spawnl(P_WAIT, "subprog.exe", NULL)) != 0)
   {
      error_exit_msg = "Can not call sub program";
      error_exit_han("main() of subprog.exe", 1);
   }
   gotoxy_han(0, 6);
   puts_han("I return main program. Press any key.");
   if(!getch()) getch();
   close_han();
}
