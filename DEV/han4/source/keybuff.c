/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          clear_key_buff()                                    */
/*   FILENAME           keybuff.c                                           */
/*   DATE & TIME        03/11/92(WED) 18:13                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#ifndef __LARGE__
#error Not compiled in the large memory model.
#endif
/* If the source text is compiled in any other memory models */
/* except for the large model, issues a fatal error message  */

#include "misc.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void clear_key_buff(void)
/* clear keyboard buffer */
{
   char *ptr = (char *) 0x0000041a;

   /* let contents of address 0x0000041a equal to that of address 0x0000041c */
   *ptr = *(ptr + 2);
}
