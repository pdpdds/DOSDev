/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          get_scr_buff_code(), get_scr_buff_id(),             */
/*                      get_scr_buff_attr(), get_scr_buff_color(),          */
/*                      get_scr_buff_bkcolor()                              */
/*   FILENAME           getbuff.c                                           */
/*   DATE & TIME        12/20/91(FRI) 20:55                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

/* ¯aÇaŸ¥ ¤áÌáµA ·¶“e ¸÷¥¡Ÿi ´ia º“e Ðq®—i */

int get_scr_buff_code(int x, int y)
{
   return(scr_buff[y][x][0]);
}

int get_scr_buff_id(int x, int y)
{
   return(scr_buff[y][x][1]);
}

int get_scr_buff_attr(int x, int y)
{
   return(scr_buff[y][x][2]);
}

int get_scr_buff_color(int x, int y)
{
   return(scr_buff[y][x][3] & 0x0f);
}

int get_scr_buff_bkcolor(int x, int y)
{
   return((scr_buff[y][x][3] >> 4) & 0x0f);
}
