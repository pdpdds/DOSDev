/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          wait_key_han()                                      */
/*   FILENAME           waitkey.c                                           */
/*   DATE & TIME        03/11/92(WED) 03:50                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <conio.h>
#include <time.h>
#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int wait_key_han(int second)
/* second �� ���e ǡ�a ���ỡ���i ���a���a. */
{
   clock_t time1;

   time1 = clock();
   while(!kbhit())
      if((clock() - time1) / CLK_TCK < second || second < 0)
      /* second ���a �w���A�� �g�v��a second �a �q�����e ���� ǡ�a */
      /* ���ỡ���i ���a���a han_wait_func() �i ѡ�e�a.           */
      {
         if(han_wait_func != (void (*) (void)) NULL) han_wait_func();
      }
      else
      /* second ���a ���v�a�e ���a�i �a�a �a�q */
         break;

   /* ǡ�a ����v�a�e 1 �i, second ���a ���v�a�e 0 �i ���� */
   return(kbhit() ? 1 : 0);
}
