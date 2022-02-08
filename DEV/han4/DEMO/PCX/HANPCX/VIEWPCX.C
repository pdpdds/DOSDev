#include <stdio.h>
#include "hanio.h"
#include "extendio.h"
#include "keydef.h"
#include "pcx.h"

void main(void)
{
   char file_name[128];
   int ret;

   /* Ðe‹i ÑÅ‰w Á¡‹¡ÑÁ */
   if(init_han() != OK_HAN)
   {
      puts("Hangul environment error.");
      exit(2);
   }

   while(1)
   {
      /* —¡BÉ¡Ÿ¡ ¤b¯aŸi ¥¡µa º´á ¥© ÑÁ·©·i ¬åÈ‚Ða•¡¢ Ðe”a. */
      ret = dir_box_long((get_maxx_han() - 58) / 2, 5, "*.pcx", file_name);
      /* <Esc>Ç¡Ÿi ’Ÿa¡e {…”a. */
      if(ret == ESC) break;

     /* ÉB¯aËa ¹ÁÎa¡ (0, 0)µA PCX ÑÁ·©·i ¥¡µa º… Ò, Ÿ¡Èå  ˆtµA  ˜aœa */
     /* ¸â¸éÐe ¡A¯¡»¡Ÿi Â‰bÐe”a.		      		       */
     /* show_pcx() ÑÁ·©µA ÑÁ·© ·¡Ÿq e ñ‰a º¡e PCX ÑÁ·©·i ¥© ® ·¶”a. */
     ret = show_pcx(0, 0, file_name);
     switch(ret)
     {
	case OK_PCX :
	   xputs_han(0, maxy_han, "´a¢ Ç¡a ’Ÿa­A¶a.");
	   break;
	case FILE_NOT_FOUND_PCX :
	   xputs_han(0, maxy_han, "ÑÁ·©·¡ ´ô¯s“¡”a.");
	   break;
	case READ_HEADER_ERROR_PCX :
	   xputs_han(0, maxy_han, "ÑÁ·©·i ·ª·i ® ´ô¯s“¡”a.");
	   break;
	case NOT_PCX_FILE :
	   xputs_han(0, maxy_han, "PCX Ñw¯¢· ÑÁ·©·¡ ´a“³“¡”a.");
	   break;
	case NOT_16COLOR_PCX :
	   xputs_han(0, maxy_han, "EGA/VGAµA¬á“e HGC¶w ÑÁ·©·i ¥© ® ´ô¯s“¡”a.");
	   break;
	case NOT_2COLOR_PCX :
	   xputs_han(0, maxy_han, "HGCµA¬á“e 16¬‚¶w ÑÁ·©·i ¥© ® ´ô¯s“¡”a.");
	   break;
	case MEMORY_NOT_ENOUGH_PCX :
	   xputs_han(0, maxy_han, "¡A¡¡Ÿ¡ˆa ¦¹¢Ðs“¡”a.");
	   break;
     }
     if(!getch()) getch();
     clrscr_han();
   }
   close_han();
}
