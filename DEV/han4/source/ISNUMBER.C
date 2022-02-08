/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          is_number()                                         */
/*   FILENAME           isnumber.c                                          */
/*   DATE & TIME        11/18/91(MON) 01:28                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

/****************************************************************************/
/*                   Prototypes of the External Functions                   */
/****************************************************************************/
#include <string.h>

int is_number(char *string)
/* string ·¡ ®•¸a¡ e ·¡ž´á¹a ·¶·a¡e 1 ·i Ÿ¡ÈåÐe”a. */
{
   int index;

   for(index = 0; index < strlen(string); index++)
      if(string[index] < '0' || string[index] > '9') return(0);

   return(1);
}
