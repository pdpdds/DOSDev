/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          setactivepage_han()                                 */
/*   FILENAME           actpage.c                                           */
/*   DATE & TIME        03/11/92(WED) 18:57                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <graphics.h>
#include "hanio.h"

/****************************************************************************/
/*                   Prototypes of the External Functions                   */
/****************************************************************************/

void setactivepage_han(int page)
/* ��ˡ�a �A�����i �a���a. */
{
   if(in_vga_system) return;    /* HGC �A��e �a���i �a���a. */

   setactivepage(page);
   switch(page)
   {
      case 0 : video_seg = 0xb000;
	       break;
      case 1 : video_seg = 0xb800;
	       break;
   }
}
