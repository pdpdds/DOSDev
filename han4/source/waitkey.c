/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          wait_key_han()                                      */
/*   FILENAME           waitkey.c                                           */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <conio.h>
#include <time.h>
#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int wait_key_han(int second)
/* second Á¡ •·´e Ç¡ˆa ’‰œá»¡‹¡Ÿi ‹¡”aŸ¥”a. */
{
   clock_t time1;

   time1 = clock();
   while(!kbhit())
      if((clock() - time1) / CLK_TCK < second || second < 0)
      /* second Á¡ˆa ‰w‰Á–A»¡ ´g´vˆáa second ˆa ·q®·¡¡e ‰­¢ Ç¡ˆa */
      /* ’‰œá»¡‹¡Ÿi ‹¡”aŸ¡¡a han_wait_func() Ÿi Ñ¡Â‰Ðe”a.           */
      {
         if(han_wait_func != (void (*) (void)) NULL) han_wait_func();
      }
      else
      /* second Á¡ˆa »¡v·a¡e žÏaŸi ¨a¹a aˆq */
         break;

   /* Ç¡ˆa ’‰œá¹v·a¡e 1 ·i, second Á¡ˆa »¡v·a¡e 0 ·i Ÿ¡Èå */
   return(kbhit() ? 1 : 0);
}
