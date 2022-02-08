/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          get_output_offset_han(), set_output_offset_han()    */
/*   FILENAME           offset.c                                            */
/*   DATE & TIME        12/20/91(FRI) 20:55                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int get_output_offset_han(void)
/* Â‰b µ³­U·i Ÿ¡ÈåÐe”a. */
{
   return(output_offset_han);
}

void set_output_offset_han(int output_offset)
/* Â‰b µ³­U·i ¬é¸÷Ðe”a. */
{
   output_offset_han = output_offset;
}
