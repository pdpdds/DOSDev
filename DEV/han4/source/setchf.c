/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          set_han_change_func(), set_ins_change_func()        */
/*   FILENAME           setchf.c                                            */
/*   DATE & TIME        03/11/92(WED) 18:52                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"

/****************************************************************************/
/*                   Prototypes of the External Functions                   */
/****************************************************************************/

void set_han_change_func(void (*func) (void))
/* Ðe/µw ¸åÑÅÇ¡ˆa ’‰œá»© ˜ a”a Ñ¡Â‰–A“e Ðq®Ÿi ¬é¸÷Ðe”a. */
{
   han_change_func = func;
}

void set_ins_change_func(void (*func) (void))
/* ¬s·³/®¸÷ ¸åÑÅÇ¡ˆa ’‰œá»© ˜ a”a Ñ¡Â‰–A“e Ðq®Ÿi ¬é¸÷Ðe”a. */
{
   ins_change_func = func;
}
