/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          gotoxy_han()                                        */
/*   FILENAME           gotoxy.c                                            */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void gotoxy_han(int x, int y)
/* �e�� ���a�i �w�� ���a�� (x, y) �� ����e�a. */
{
   curx_han = x + minx_han;
   cury_han = y + miny_han;

   /* ���a�i �i�� �����e �w�� error_exit_han() ѡ */
   if(curx_han < 0 || curx_han > maxx_han ||
      cury_han < 0 || cury_han > maxy_han)
   {
      error_exit_msg = "out of range error";
      error_exit_han("gotoxy_han", 1);
   }
}
