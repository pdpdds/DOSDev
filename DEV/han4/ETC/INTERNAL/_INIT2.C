/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          _init_han2(), _close_han2()                         */
/*   FILENAME           _inithan2.c                                         */
/*   DATE & TIME        10/25/92(SUN) 23:35                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <alloc.h>
#include <string.h>
#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                             Macro Constants                              */
/****************************************************************************/

/* ¸÷–E Í¥Ëa ÑÁ·©· ·¡Ÿq */
#define HAN_FONT_FILE           "hangul.fnt"
#define ENG_FONT_FILE           "english.fnt"
#define SPC_FONT_FILE           "special.fnt"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int _init_han2(int mode)
/* Ðe‹i ÑÅ‰w·a¡ Á¡‹¡ÑÁÐa“e Ðq® */
{
   char *program_name = "Hangul Library <HAN> Ver 4.1";
   char *programmer   = "by Lee Hyun-Ho";
   char han_font_file[100], eng_font_file[100], spc_font_file[100];
   int handle, graph_driver, graph_mode, i, j;

   /* program_name ‰Á programmer ·¡ µ³Ë¡ a·¡»·µA ·Ð ¹Aˆá–A“e ˆõ·i ¤w»¡ */
   program_name++; programmer++;

   /* ¯aÇaŸ¥¤áÌá, Í¥Ëa, Ðe¸a ¤áÌáµA ¡A¡¡Ÿ¡ Ði”w */
   if( ((scr_buff   = malloc(LIMITY_HAN*LIMITX_HAN*4)) == NULL) ||
       ((hanja_buff = malloc(sizeof(hanja_buff[0])*hanja_buff_size)) == NULL) )
       return(MEMORY_NOT_ENOUGH);

   /* Ðe¸a ¤áÌáŸi Á¡‹¡ÑÁÐe”a. */
   for(i = 0; i < hanja_buff_size; i++) hanja_buff[i].code = 0;

   /* Í¥ËaŸi ˆaŸ¡Ç¡“e Í¡·¥ÈáŸi return_to_org_font() ˆa Ñ¡Â‰–I ˜Ÿi ”§¡Ð */
   /* org_? µA ¥¢¬aÐ –…”a.                                               */
   org_han1_font = han1_font;
   org_han2_font = han2_font;
   org_han3_font = han3_font;
   org_eng_font  = eng_font;
   org_spc_font  = spc_font;

   /* ‹aœÏ¢ ¡¡—a¡ Á¡‹¡ÑÁÐe”a. */
   if(!stay_in_gr_mode)
   {
      if(mode < INIT_HGC640_0 || mode > INIT_TV)
      {
         stay_in_gr_mode = 1;
	 _close_han2(mode);
	 return(INVALID_GR_CARD);
      }

      set_to_gr_mode(mode);
      switch(mode)
      {
	 case INIT_VGA      :
	 case INIT_EGA      :
	 case INIT_EGA_MONO : in_vga_system = 1;
			      video_seg     = 0xa000;
			      break;
	 case INIT_HGC640_0 :
	 case INIT_HGC720_0 : in_vga_system = 0;
			      video_seg     = 0xb000;
			      break;
	 default            : in_vga_system = 0;
			      video_seg     = 0xb800;
			      break;
      }

      /* ÑÁ¡e· Í¢‰Á ‘¼·¡µA ‰Åe–E ¸åµb ¥e®—i· ˆt·i ‰¬e */
      if(mode == INIT_HGC720_0 || mode == INIT_HGC720_1)
         abs_maxx_han = maxx_han = 89;
      else
         abs_maxx_han = maxx_han = 79;
      switch(mode)
      {
	 case INIT_HGC720_0 :
	 case INIT_HGC720_1 :
	 case INIT_EGA      :
	 case INIT_EGA_MONO : abs_maxy_han = maxy_han = 20;
			      break;
	 case INIT_VGA      : abs_maxy_han = maxy_han = 29;
			      break;
	 default 	    : abs_maxy_han = maxy_han = 24;
			      break;
      }
   }

   /* ÑÁ¡e·i »¡¶…”a(set_to_gr_mode() “e ‹aœÏ¢ Á¡‹¡ÑÁ ÒµA ÑÁ¡e·i */
   /* »¡¶‹¡ ´g‹¡ ˜¢…). ¯aÇaŸ¥¤áÌáŸi Á¡‹¡ÑÁÐa“e Òa‰Á•¡ ·¶”a.     */
   clear_text_han(0, 0, abs_maxx_han, abs_maxy_han);

   han_wait_func       = (void (*) (void)) NULL;
   han_change_func     = (void (*) (void)) default_han_change_func;
   ins_change_func     = (void (*) (void)) default_ins_change_func;
   redirect_func       = (void (*) (char *)) NULL;

   /* Äá¬á· ¡¡´··i ¬é¸÷Ðe”a. */
   han_cursor[0] = 15;
   eng_cursor[0] = 7;
   set_cursor_han(BIG_CURSOR, "", BIG_CURSOR, "");
   cursor_han = eng_cursor;

   /* Èá¥¡ C · delay() µA ·¶“e ¤á‹a˜¢…µA delay(0) ·i £¡Ÿ¡ Ñ¡Â‰ */
   delay(0);

   /* Ðe‹i ÑÅ‰w·a¡ Á¡‹¡ÑÁ–A´ö·a£a¡ OK_HAN ·i Ÿ¡Èå */
   return(OK_HAN);
}

void _close_han2(int mode)
/* Ðe‹i ÑÅ‰w·i ¹·žaÐe”a. */
{
   /* _init_han() ·¡ Ñ¡Â‰–I ˜ Ði”w¤h·e ¡A¡¡Ÿ¡Ÿi ¥¢Šá */
   free(scr_buff);
   free(hanja_buff);

   /* ÉB¯aËa ¡¡—a¡ ¸åÑÅ */
   if(!stay_in_gr_mode && mode >= INIT_HGC640_0 && mode <= INIT_TV)
   {
      set_to_text_mode(mode);
      /* set_to_text_mode() “e ÑÁ¡e ¬wÈ e ¤aŽ‰¡ ÑÁ¡e·i »¡¶»¡“e */
      /* ´g·a£a¡ clrscr() ¡ »¢¸ó »¡¶¡´¡ Ðe”a.                   */
      clrscr();
   }
}
