/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          get_max_strlen_han(), set_max_strlen_han()          */
/*   FILENAME           maxstrln.c                                          */
/*   DATE & TIME        12/20/91(FRI) 20:55                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int get_max_strlen_han(void)
/* Èá¥¡ C Ñ¡ÑÅ¶w ·³bÐq®¡ ·³b ˆa“wÐe ¢…¸a· ®Ÿi Ÿ¡ÈåÐe”a. */
{
   return(max_strlen_han);
}

void set_max_strlen_han(int length)
/* Èá¥¡ C Ñ¡ÑÅ¶w ·³bÐq®¡ ·³b ˆa“wÐe ¢…¸a· ®Ÿi ¬é¸÷Ðe”a. */
{
   max_strlen_han = length;
}
