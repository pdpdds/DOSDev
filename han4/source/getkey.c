/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          get_key_han()                                       */
/*   FILENAME           getkey.c                                            */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <conio.h>
#include <bios.h>
#include "hanio.h"
#include "extendio.h"
#include "misc.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int get_key_han(int *ascii)
{
   int scan;

   while(!kbhit())
      if(han_wait_func != (void (*) (void)) NULL) han_wait_func();
   scan   = bioskey(0);
   *ascii = scan & 0x00ff;
   return(scan >> 8);
}
