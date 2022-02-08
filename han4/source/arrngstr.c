/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          arrange_string()                                    */
/*   FILENAME           arrngstr.c                                          */
/*   DATE & TIME        11/18/91(MON) 01:28                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#ifndef __LARGE__
#error Not compiled in the large memory model.
#endif
/* If the source text is compiled in any other memory models */
/* except for the large model, issues a fatal error message  */

#include <string.h>
#include "hanio.h"
#include "misc.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int arrange_string(char *string, int width)
/* arrange one line in string */
{
   int space_num = 0, rescan, index;

   index = width - 1;
   if(strlen(string) < width) return(0);
   while(what_byte(string, width - 1) == HAN1 || string[width-1] == ' ')
   {
      rescan = 0;
      /* if the character to be deleted is space, do not count the number */
      /* of the deleted characters by insertion of blanks                 */
      if(string[width-1] != ' ') space_num++;
      while(string[index] == ' ')
         if(index-- == 0)
         {
            index = width - 1;
	    if(++rescan == 2) break;
	 }
      if(rescan == 2)
      {
         space_num--;
	 break;
      }
      rescan = 0;
      while(string[index] != ' ')
      if(index-- == 0)
      {
         index = width - 1;
	 if(++rescan == 2) break;
      }
      if(rescan == 2)
      {
         space_num--;
         break;
      }
      memmove(string + index + 1, string + index, (width - 1) - index);
      string[index] = ' ';
   }
   string[width] = 0;
   return(space_num);
}
