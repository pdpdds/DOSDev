/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          getch_han()                                         */
/*   FILENAME           getchhan.c                                          */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <conio.h>
#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int getch_han(void)
/* �e ���� ǡ�i ���b�h�e�a. */
{
   /* ���b ���� ���e���e han_wait_func() �i ���� ѡ */
   while(!kbhit())
      if(han_wait_func != (void (*) (void)) NULL) han_wait_func();
   /* ���b�h�e ǡ�� �a�aǡš�a�i ���� */
   return(getch());
}
