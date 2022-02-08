/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          cputs_han()                                         */
/*   FILENAME           cputshan.c                                          */
/*   DATE & TIME        03/11/92(WED) 22:31                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void cputs_han(char *string)
/* �e�� ���a�A ���a�i�i �b�e�a. ���a�i �bҁ ��З�i �a�� �g�e�a. */
{
   int index = 0;

   while(string[index])
   {
      putch_han(string + index);
      if(string[index++] & 0x80) index++;
   }
}
