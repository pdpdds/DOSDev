/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          get_attr_han(), set_attr_han(), get_color_han(),    */
/*                      get_bkcolor_han(), set_color_han(),                 */
/*                      set_bkcolor_han(), set_vbcolor_han()                */
/*   FILENAME           attrcolr.c                                          */
/*   DATE & TIME        03/11/92(WED) 18:52                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

/* ­¢¬÷‰Á ¬‚µA ‰Åe–E ¸åµb ¥e®· ˆt·i Ÿ¡ÈåÐa“e Ðq®—i */

int get_attr_han(void)
{
   return(attr_han);
}

void set_attr_han(int attr)
{
   attr_han = attr;
}

int get_color_han(void)
{
   return(color_han);
}

int get_bkcolor_han(void)
{
   return(bkcolor_han);
}

void set_color_han(int color)
{
   color_han = color;
}

void set_bkcolor_han(int color)
{
   bkcolor_han = color;
}

void set_vbcolor_han(int color, int bkcolor)
{
   color_han   = color;
   bkcolor_han = bkcolor;
}
