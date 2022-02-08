#include <stdio.h>
#include <graphics.h>
#include "hanio.h"
#include "extendio.h"
#include "keydef.h"
#include "pcx.h"

void set_rgb_palette(int color_num, int red, int green, int blue);

void show_16color(void)
/* 16 ¬‚·a¡ ‹i¸aŸi Â‰bÐa“e Ðq® */
{
   int i;

   for(i = 0; i < 16; i++) vputs_han(0, i + 2, "16¬‚·a¡ ‹i¸aŸi Â‰b", i);
   if(!getch()) getch();
   clrscr_han();
}

void set_default_palette(void)
{
   int de_pal[16][3] =
   {
      {  0,  0,  0 }, {  0,  0, 42 }, {  0, 42,  0 }, {  0, 42, 42 },
      { 42,  0,  0 }, { 42,  0, 42 }, { 42, 21,  0 }, { 42, 42, 42 },
      { 21, 21, 21 }, { 21, 21, 63 }, { 21, 63, 21 }, { 21, 63, 63 },
      { 63, 21, 21 }, { 63, 21, 63 }, { 63, 63, 21 }, { 63, 63, 63 }
   };
   int i;

   for(i = 0; i < 16; i++)
      set_rgb_palette(i, de_pal[i][0], de_pal[i][1], de_pal[i][2]);
}

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

   /* Ñe¸ ÌiAËaŸi ¸á¸wÐe”a. */
   puts_han("¶¥œ· ÌiAËa“e ”a·q‰Á ˆ{·e ¬‚·³“¡”a.");
   show_16color();

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

     /* ÌiAËaŸi ¥¢ŠÐa‹¡ ¸å· 16¬‚·i Â‰b */
     puts_han("Ñe¸ ¬é¸÷–E ÌiAËa· ¬‚·e ”a·q‰Á ˆ{¯s“¡”a.");
     show_16color();

     set_default_palette();
     /* ÌiAËaŸi ¥¢ŠÐe Ò· 16¬‚·i Â‰b */
     puts_han("¶¥œ· ÌiAËa¡ ¥¢ŠÐe Ò· ¬‚·e ”a·q‰Á ˆ{¯s“¡”a.");
     show_16color();
   }
   close_han();
}

