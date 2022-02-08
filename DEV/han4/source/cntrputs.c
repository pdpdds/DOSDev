/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          center_puts_han()                                   */
/*   FILENAME           cntrputs.c                                          */
/*   DATE & TIME        03/25/92(WED) 12:43                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void center_puts_han(int y, char *string, int color, int attr)
/* y �弁 ���� ���e�A color �� ��w���� attr �� �����a�� ���a�i�i �b�e�a. */
{
   vaputs_han((attr & FONT_HORIZ_DBL ? (abs_maxx_han+1-2*strlen(string)) / 2
				     : (abs_maxx_han+1-strlen(string))   / 2),
	       y, string, color, attr);
}
