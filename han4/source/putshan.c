/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                           */
/*   SUB-TITLE          puts_han()                                          */
/*   FILENAME           putshan.c                                           */
/*   DATE & TIME        03/11/92(WED) 22:31                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int puts_han(char *string)
/* Ñe¸ ¹ÁÎaµA ¢…¸aµi·i Â‰bÐe”a. ¢…¸aµi Â‰bÒ ˆÐ—·i Ðe”a. */
{
   int index = 0;

   while(string[index])
   {
      putch_han(string + index);
      if(string[index++] & 0x80) index++;
   }
   putch_han("\n");

   return(index == 0 ? 0 : string[index-1]);
}
