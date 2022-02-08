/****************************************************************************/
/*   TITLE              Hangul I/O Libarary <HAN> 4.1                       */
/*   SUB-TITLE          get_ch(), wait_key() function                       */
/*   FILENAME           get_ch.c                                            */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <conio.h>
#include <time.h>
#include "hanio.h"

int getch_han(void)
{
   while(!kbhit())
      if(han_wait_func != (void (*) (void)) 0) han_wait_func();
   return(getch());
}

void wait_key_han(int second)
/* as far as no keystroke does occur, wait for <second> seconds */
{
   clock_t time1;

   time1 = clock();
   while(!kbhit())
      if( (clock() - time1) / CLK_TCK < second)
      {
         if(han_wait_func != (void (*) (void)) 0) han_wait_func();
         /* 0 means NULL */
      }
      else
         break;
   return;
}


