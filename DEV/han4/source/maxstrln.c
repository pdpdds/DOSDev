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
/* �ᥡ C ѡ�Ŷw ���b�q���� ���b �a�w�e ���a�� ���i �����e�a. */
{
   return(max_strlen_han);
}

void set_max_strlen_han(int length)
/* �ᥡ C ѡ�Ŷw ���b�q���� ���b �a�w�e ���a�� ���i ����e�a. */
{
   max_strlen_han = length;
}
