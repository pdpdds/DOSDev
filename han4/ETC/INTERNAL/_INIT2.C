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

/* �����E ͥ�a ������ ���q */
#define HAN_FONT_FILE           "hangul.fnt"
#define ENG_FONT_FILE           "english.fnt"
#define SPC_FONT_FILE           "special.fnt"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

int _init_han2(int mode)
/* �e�i �ŉw�a�� �������a�e �q�� */
{
   char *program_name = "Hangul Library <HAN> Ver 4.1";
   char *programmer   = "by Lee Hyun-Ho";
   char han_font_file[100], eng_font_file[100], spc_font_file[100];
   int handle, graph_driver, graph_mode, i, j;

   /* program_name �� programmer �� ��ˡ�a�����A ��Ё �A��A�e ���i �w�� */
   program_name++; programmer++;

   /* �a�a������, ͥ�a, �e�a ����A �A���� �i�w */
   if( ((scr_buff   = malloc(LIMITY_HAN*LIMITX_HAN*4)) == NULL) ||
       ((hanja_buff = malloc(sizeof(hanja_buff[0])*hanja_buff_size)) == NULL) )
       return(MEMORY_NOT_ENOUGH);

   /* �e�a ����i �������e�a. */
   for(i = 0; i < hanja_buff_size; i++) hanja_buff[i].code = 0;

   /* ͥ�a�i �a��ǡ�e ͡����i return_to_org_font() �a ѡ�I ���i ����Ё */
   /* org_? �A ���aЁ ���a.                                               */
   org_han1_font = han1_font;
   org_han2_font = han2_font;
   org_han3_font = han3_font;
   org_eng_font  = eng_font;
   org_spc_font  = spc_font;

   /* �a��Ϣ ���a�� �������e�a. */
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

      /* ���e�� ͢�� �����A �ŝe�E ��b �e���i�� �t�i ���e */
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

   /* ���e�i �����a(set_to_gr_mode() �e �a��Ϣ ������ ҁ�A ���e�i */
   /* ������ �g�� ����). �a�a������i �������a�e �a���� ���a.     */
   clear_text_han(0, 0, abs_maxx_han, abs_maxy_han);

   han_wait_func       = (void (*) (void)) NULL;
   han_change_func     = (void (*) (void)) default_han_change_func;
   ins_change_func     = (void (*) (void)) default_ins_change_func;
   redirect_func       = (void (*) (char *)) NULL;

   /* ��᷁ �����i ����e�a. */
   han_cursor[0] = 15;
   eng_cursor[0] = 7;
   set_cursor_han(BIG_CURSOR, "", BIG_CURSOR, "");
   cursor_han = eng_cursor;

   /* �ᥡ C �� delay() �A ���e ��a�����A delay(0) �i ���� ѡ */
   delay(0);

   /* �e�i �ŉw�a�� �������A���a�a�� OK_HAN �i ���� */
   return(OK_HAN);
}

void _close_han2(int mode)
/* �e�i �ŉw�i ���a�e�a. */
{
   /* _init_han() �� ѡ�I �� �i�w�h�e �A�����i ���� */
   free(scr_buff);
   free(hanja_buff);

   /* �B�a�a ���a�� ���� */
   if(!stay_in_gr_mode && mode >= INIT_HGC640_0 && mode <= INIT_TV)
   {
      set_to_text_mode(mode);
      /* set_to_text_mode() �e ���e �wȁ�e �a���� ���e�i �������e */
      /* �g�a�a�� clrscr() �� ���� ������ �e�a.                   */
      clrscr();
   }
}
