/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          init_han(), close_han()                             */
/*   FILENAME           inithan.c                                           */
/*   DATE & TIME        03/11/92(WED) 19:17                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <graphics.h>
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

int init_han(void)
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
       ((han1_font  = malloc(NO_BUL1*(NO_HAN1+1)*HAN_FONT_SIZE)) == NULL) ||
       ((han2_font  = malloc(NO_BUL2*(NO_HAN2+1)*HAN_FONT_SIZE)) == NULL) ||
       ((han3_font  = malloc(NO_BUL3*(NO_HAN3+1)*HAN_FONT_SIZE)) == NULL) ||
       ((eng_font   = malloc(NO_ENG*ENG_FONT_SIZE)) == NULL) ||
       ((spc_font   = malloc(NO_ENG*HAN_FONT_SIZE)) == NULL) ||
       ((hanja_buff = malloc(sizeof(hanja_buff[0])*hanja_buff_size)) == NULL) )
      return(MEMORY_NOT_ENOUGH);

   /* ͥ�a ������ ���q�i �i�� */
   i = font_path_han[strlen(font_path_han) - 1];
   strncpy(han_font_file, font_path_han, 80);
   strncpy(eng_font_file, font_path_han, 80);
   strncpy(spc_font_file, font_path_han, 80);
   han_font_file[80] = 0;
   eng_font_file[80] = 0;
   spc_font_file[80] = 0;
   if(font_path_han[0] && i != ':' && i != '\\')
   {
      strcat(han_font_file, "\\");
      strcat(eng_font_file, "\\");
      strcat(spc_font_file, "\\");
   }
   strcat(han_font_file, HAN_FONT_FILE);
   strcat(eng_font_file, ENG_FONT_FILE);
   strcat(spc_font_file, SPC_FONT_FILE);

   /* ͥ�a �����A�� ͥ�a�i ���e�a. */
   i = replace_font(han_font_file, eng_font_file, spc_font_file);
   if(i)
   {
      /* �A��a �i���a�e �i�w�h�e �A�����i ���ᯡǥ�a. */
      stay_in_gr_mode = 1;
      close_han();
      return(i);
   }

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
      detectgraph(&graph_driver, &graph_mode);
      switch(graph_driver)
      {
	 case HERCMONO : graph_mode = HERCMONOHI;
			 in_vga_system = 0;
			 video_seg = 0xb000;
			 break;
	 case EGA      : graph_mode = EGAHI;
			 in_vga_system = 1;
			 video_seg = 0xa000;
			 break;
	 case VGA      : graph_mode = VGAHI;
			 in_vga_system = 1;
			 video_seg = 0xa000;
			 break;
	 default       : stay_in_gr_mode = 1;
			 close_han();
			 return(INVALID_GR_CARD);
      }

      initgraph(&graph_driver, &graph_mode, bgi_path_han);
      if(graphresult() != grOk)
      {
	 stay_in_gr_mode = 1;
	 close_han();
	 return(INITGRAPH_ERROR);
      }

      /* ���e�� ͢�� �����A �ŝe�E ��b �e���i�� �t�i ���e */
      abs_maxx_han = maxx_han = (getmaxx() -  7) >> 3;
      abs_maxy_han = maxy_han = (getmaxy() - 15) >> 4;
   }

   /* �a�a������i �������e�a. */
   for(i = 0; i <= abs_maxy_han; i++)
      for(j = 0; j <= abs_maxx_han; j++)
      {
	 scr_buff[i][j][0] = ' ';
	 scr_buff[i][j][1] = ENG;
	 scr_buff[i][j][2] = 0;
	 scr_buff[i][j][3] = color_han | (bkcolor_han << 4);
      }

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

void close_han(void)
/* �e�i �ŉw�i ���a�e�a. */
{
   /* _init_han() �� ѡ�I �� �i�w�h�e �A�����i ���� */
   free(scr_buff);
   free(org_han1_font);
   free(org_han2_font);
   free(org_han3_font);
   free(org_eng_font);
   free(org_spc_font);
   free(hanja_buff);

   /* �B�a�a ���a�� ���� */
   if(!stay_in_gr_mode) closegraph();
}
