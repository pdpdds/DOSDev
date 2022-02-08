/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          get_abs_maxx_han(), get_abs_maxy_han(),             */
/*                      get_maxx_han(), get_maxy_han(), get_minx_han(),     */
/*                      get_miny_han(), get_curx_han(), get_cury_han()      */
/*   FILENAME           getcordi.c                                          */
/*   DATE & TIME        03/11/92(WED) 22:31                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

/* ¹ÁÎaµA ‰Åe–E ¸åµb ¥e®· ˆt·i Ÿ¡ÈåÐa“e Ðq®—i */

int get_abs_maxx_han(void)
{
   return(abs_maxx_han);
}

int get_abs_maxy_han(void)
{
   return(abs_maxy_han);
}

int get_maxx_han(void)
{
   return(maxx_han);
}

int get_maxy_han(void)
{
   return(maxy_han);
}

int get_minx_han(void)
{
   return(minx_han);
}

int get_miny_han(void)
{
   return(miny_han);
}

int get_curx_han(void)
{
   return(curx_han);
}

int get_cury_han(void)
{
   return(cury_han);
}
