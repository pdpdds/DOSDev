/****************************************************************************/
/*   TITLE              Graphics Library                                    */
/*   SUB-TITLE          SVGA detecting function                    	    */
/*   FILENAME           detsvga.c                                           */
/*   DATE & TIME        12/20/92 (SUN) 16:25                                */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "graph.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int detect_svga(void)
/* This routine detects SVGA chip name. */
{
   if(is_et3000())
      return(is_et4000() ? ET4000 : ET3000);
   else if(is_wonder())
      return(is_wonder2() ? WONDER2 : WONDER1);
   else
      return(VGA);
}
