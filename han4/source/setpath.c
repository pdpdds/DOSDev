/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          set_bgi_font_path()                                 */
/*   FILENAME           setpath.c                                           */
/*   DATE & TIME        12/20/91(FRI) 20:55                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void set_bgi_font_path(char *bgi_path, char *font_path)
/* BGI ÑÁ·©‰Á Í¥Ëa ÑÁ·©·¡ ·¶“e —¡BÉ¡Ÿ¡Ÿi ¬é¸÷Ðe”a. */
{
   bgi_path_han  = bgi_path;
   font_path_han = font_path;
}
