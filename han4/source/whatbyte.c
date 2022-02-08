/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          what_byte()                                         */
/*   FILENAME           whatbyte.c                                          */
/*   DATE & TIME        11/18/91(MON) 01:28                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"

/****************************************************************************/
/*                   Prototypes of the External Functions                   */
/****************************************************************************/

int what_byte(char *string, int chk_pos)
/* string[chk_pos] · ¹·ŸAŸi ´ia º…”a. */
{
   int index = 0;

   while(string[index])
   {
      if(string[index] & 0x80)
      {
	 if(index == chk_pos) return(HAN1); else index++;
	 if(index == chk_pos) return(HAN2); else index++;
      }
      else
	 if(index == chk_pos) return(ENG); else index++;
   }

   /* string[chk_pos] ˆa é¢…¸a·¡¡e ENG Ÿi Ÿ¡ÈåÐe”a. */
   return(ENG);
}
