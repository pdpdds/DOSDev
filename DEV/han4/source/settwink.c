/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          set_twinkle_time()                                  */
/*   FILENAME           settwink.c                                          */
/*   DATE & TIME        12/20/91(FRI) 20:55                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"

/****************************************************************************/
/*                   Prototypes of the External Functions                   */
/****************************************************************************/

void set_twinkle_time(int twinkle_time)
/* Äá¬áˆa Œq¤b·¡“e ¯¡ˆe·i ¹¡¸÷Ðe”a. */
{
   if(twinkle_time < 0 || twinkle_time > 255) return;
   cursor_twinkle_time = twinkle_time;
}
