/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          lefttrim(), righttrim()                             */
/*   FILENAME           strtrim.c                                           */
/*   DATE & TIME        11/18/91(MON) 01:28                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <string.h>

/****************************************************************************/
/*                   Prototypes of the External Functions                   */
/****************************************************************************/

char *lefttrim(char *string)
/* ¢…¸aµi· ¶E½¢µA ·¶“e ‰·¤‚·i ¹AˆáÐe”a. */
{
   int start = 0, length;

   length = strlen(string);
   while(string[start] == ' ' && start < length) start++;
   if(start != 0) memmove(string, string + start, length - start + 1);

   return(string);
}

char *righttrim(char *string)
/* ¢…¸aµi· µ¡Ÿe½¢µA ·¶“e ‰·¤‚·i ¹AˆáÐe”a. */
{
   int end;

   end = strlen(string);
   while(string[end-1] == ' ') end--;
   string[end] = 0;

   return(string);
}
