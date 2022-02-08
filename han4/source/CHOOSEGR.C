/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          choose_gr_mode()                                    */
/*   FILENAME           choosegr.c                                          */
/*   DATE & TIME        05/09/92(SAT) 12:05                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#ifndef __LARGE__
#error Not compiled in the large memory model.
#endif
/* œa»¡ ·¡¶A· ¡A¡¡Ÿ¡ ¡¡•I¡ ÄñÌa·©–A¡e µAœá ¡A¯¡»¡Ÿi Â‰bÐe Ò */
/* ÄñÌa·©·i º—”eÐa•¡¢ Ðe”a.                                    */

#include <conio.h>
#include <stdio.h>


/****************************************************************************/
/*                   Prototypes of the External Functions                   */
/****************************************************************************/

int choose_gr_mode(void)
/* ‹aœÏ¢ ¡¡—aŸi ¬åÈ‚¤h“e”a. "Q. Quit" ¡A“AŸi ¬åÈ‚Ðaˆáa */
/* <Esc> Ç¡Ÿi ’Ÿa¡e -1 ·i Ÿ¡ÈåÐe”a.                     */
{
   int key, mode;

   puts("Hangul Library <HAN> 4.1  Graphics mode setup");
   puts("----------------------------------------------");
   puts("       0. Hercules 0 page (640x400x 2)");
   puts("       1. Hercules 1 page (640x400x 2)");
   puts("       2. Hercules 0 page (720x348x 2)");
   puts("       3. Hercules 1 page (720x348x 2)");
   puts("       4. VGA             (640x480x16)");
   puts("       5. EGA             (640x350x16)");
   puts("       6. EGA mono        (640x350x 2)");
   puts("       7. PC24            (640x400x 2)");
   puts("       8. AT&T            (640x400x 2)");
   puts("       9. Daewoo Corona   (640x400x 2)");
   puts("       A. Toshiba         (640x400x 2)");
   puts("       B. Samsung SPC     (640x400x 2)");
   puts("       C. Kabil Televideo (640x400x 2)");
   puts("       Q. Quit");
   puts("-----------------------------------------");
   cputs("Choose graphics mode -- ");

   do
   {
      key = getch();
      if(key >= '0' && key <= '9')
	 mode = key - '0';
      else if(key >= 'A' && key <= 'C')
	 mode = key - 'A' + 10;
      else if(key >= 'a' && key <= 'c')
	 mode = key - 'a' + 10;
      else if(key == 'Q' || key == 'q' || key == 27)
	 mode = -1;
      else
	 continue;
   }
   while(0);

   return(mode);
}
