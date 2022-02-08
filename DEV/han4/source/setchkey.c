/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          set_han_change_key()                                */
/*   FILENAME           setchkey.c                                          */
/*   DATE & TIME        12/20/91(FRI) 20:55                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void set_han_change_key(int code, int is_extend, int flag)
/* Ðe/µw ¸åÑÅÇ¡Ÿi ¤aŽ…”a. */
{
   han_change_code      = code;
   han_change_flag      = flag;
   is_han_change_extend = is_extend;
}
