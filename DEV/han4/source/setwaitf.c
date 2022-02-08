/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          set_han_wait_func()                                 */
/*   FILENAME           setwaitf.c                                          */
/*   DATE & TIME        12/20/91(FRI) 20:55                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"

/****************************************************************************/
/*                   Prototypes of the External Functions                   */
/****************************************************************************/

void set_han_wait_func(void (*func) (void))
/* ���b ���� ���e ���e ѡ�A�e �q���i ����e�a. */
{
   han_wait_func = func;
}

