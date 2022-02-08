/**************************************************************************/
/*  Î‰‹aŸ±¡w : ­QËi‰·Š ver0.1 (Computer Tools 0.1)                       */
/*  Ïa¡‹aœ á : ‹± ¡wÑÅ (ÐeÄñ1 , HanCom1 , hancom1)                      */
/*  Copyright (c) HanCom System 1992                                      */
/**************************************************************************/

#include "misc.h"
#include "hanio.h"
#include "extendio.h"
#include "keydef.h"
#include <alloc.h>
#include <conio.h>
#include <dir.h>
#include <dos.h>
#include <graphics.h>
#include <io.h>
#include <mem.h>
#include <stdio.h>
#include <time.h>

#define default_color()       set_vbcolor_han(BLACK, LIGHTGRAY)

   pulldown_menu_struct menu_data =
   {
      7, 1,        /* º¡A“A· ˆ•®“e 7ˆ·¡‰¡ ÂAÁ¡µA“e 1¤å ¡A“Aˆa ¬åÈ‚–E”a.*/
      { 1, 15, 25, 35, 45, 55, 65 },      /* º¡A“A 7ˆ· x ¹ÁÎa */
      { " <­QËi‰·Š> ", " ÑÁ·© ", " —¡BÉ¡Ÿ¡ ", " —¡¯aÇa ", " ¸÷¥¡ ",
	" ¬—ÑÉ¡A“A ", " Ëb¥i¡A“A ",
       },
      /* ·¡ ¦¦…µA¬á¦Èá ¦¡A“A 7 ˆˆa ¸÷·–E”a. */
      { /* 0 ¤å ¡A“A <­QËi‰·Š> */
	{ 7, 0, 1, 2,                      /* (1, 1)µA 6ˆ¼aŸ¡ ¦¡A“AŸi Š¬÷ */
	  { " <­QËi‰·Š>   ",
            " •¡¶‘ i    F1 ",
	    " i¼a . ¯¡ˆe  ",
	    "  e‘e”ib     ",
	    " §¡£©¤åÑ¡‰ÅŸ¡ ",
	    " a—i·¡ ‹¡“w  ",
	    " {‹¡       ",
          }
        },
	/* 1 ¤å ¡A“A(ÑÁ·©) */
	{ 5, 0, 15, 2,                      /* (15, 1)µA 5ˆ¼aŸ¡ ¦¡A“AŸi Š¬÷ */
	  { "»¡¶‹¡       ",
	    "¥¢¬aÐa‹¡     ",
	    "·¡Ÿq¥e‰wÐa‹¡ ",
	    "­¢¬÷¤aŽ‹¡   ",
	    "Íe»³Ða‹¡     ",
          }
        },
	/* 2 ¤å ¡A“A(—¡BÉ¡Ÿ¡) */
	{ 4, 0, 25, 2,                    /* (25, 1)µA 4ˆ¼aŸ¡ ¦¡A“AŸi Š¬÷ */
	  { "  e—i‹¡     ",
	    " »¡¶‹¡     ",
	    " ·¡Ÿq¤aŽ‹¡ ",
	    " µ«‹¡‹¡     ",
          }
        },
	/* 3 ¤å ¡A“A(—¡¯aÇa) */
	{ 4, 0, 35, 2,                    /* (35, 1)µA 4ˆ¼aŸ¡ ¦¡A“AŸi Š¬÷ */
	  { " Á¡‹¡ÑÁÐa‹¡ ",
	    " ¥¢¬aÐa‹¡   ",
	    " ·¡Ÿq¤aŽ‹¡ ",
	    " Íe»³Ða‹¡   ",
          }
        },
	/* 4 ¤å ¡A“A(¸÷¥¡) */
	{ 5, 0, 45, 2,                    /* (45, 1)µA 5ˆ¼aŸ¡ ¦¡A“AŸi Š¬÷ */
	  { " ­QËi¸÷¥¡     ",
	    " ¡A¡¡Ÿ¡¸÷¥¡   ",
	    " —¡¯aÇa¸÷¥¡   ",
	    " —¡BÉ¡Ÿ¡¸÷¥¡ ",
	    " ÑÁ·©¸÷¥¡     ",
          }
        },
	/* 5 ¤å ¡A“A(¬—ÑÉ¡A“A) */
	{ 7, 0, 55, 2,                    /* (50, 1)µA 7ˆ¼aŸ¡ ¦¡A“AŸi Š¬÷ */
	  { " ·©¸÷Îa     ",
	    " ˆa‰¦     ",
	    " º­¡¢     ",
	    " ·©‹¡¸w     ",
	    " ‰¬e‹¡     ",
	    " ¶Íe¤åÑ¡   ",
	    " ­A‰·¯¡ˆe ",
          }
        },
	/* 6 ¤å ¡A“A(Ëb¥i¡A“A) */
	{ 4, 0, 61, 2,                    /* (55, 1)µA 4ˆ¼aŸ¡ ¦¡A“AŸi Š¬÷ */
	  { " ¢…¬áÍe»³‹¡ ",
	    " ‹aœÏ¢     ",
	    " ·q´b       ",
	    " ‰A·±       ",
          }
        },
      }
   };

/**************************************************************************/
/*              Ðq ® ¶¥ Ñw                                               */
/**************************************************************************/
 void disp_status(void);
 void disp_logo(void);
 void draw_top(void);
 void show_watch();
 void c_tools();
 void show_help();
 void date_time();
 void carlenda();
 void password();
 void to_dos();
 void ending();
 void file_del();
 void file_copy();
 void file_ren();
 void file_attr();
 void file_edit();
 void dir_make();
 void dir_del();
 void dir_ren();
 void dir_mov();
 void disk_del();
 void disk_copy();
 void disk_ren();
 void disk_edit();
 void info_com();
 void info_mem();
 void info_disk();
 void info_dir();
 void info_file();
 void iljung();
 void gagebu();
 void adress();
 void diary();
 void calc();
 void mail_number();
 void world_time();
 void wordpro();
 void graph_tool();
 void music_tool();
 void game_tool();

 void main()
 {
   char key;                                           /*  a»¡ bµA ’‰a»¥ Ç¡ */
   int  main_menu, sub_menu;                 /* ¬åÈ‚–E º¡A“AµÁ ¦¡A“A */
   set_internal_font();
   if(init_han2() != OK_HAN) exit(1);
   status_on_han();
   disp_status();
   disp_logo();

   while(1)
   {
      /* ·¡ ¦¦…µA¬á Î‰”a¶… ¡A“Aˆa Š¬÷–E”a. */
      key = pulldown_han(&menu_data);

      if(key == ESC) break;                  /* <Esc> Ç¡Ÿi ’‰œá•¡ {·¡ e”a. */
      /* ¬åÈ‚–E º¡A“A“e main_menu µA, ¦¡A“A“e sub_menuµA ¸á¸wÐe”a. */
      main_menu = menu_data.current;
      sub_menu  = menu_data.sub_menu[main_menu].current;

      /* ·¡ ¦¦…µA¬á '{' ·¡¶A· ¡A“A•¡ ÐqA ÀáŸ¡Ð º¡e –E”a. */
      switch(main_menu)
      {
	 case 0 :   /* ­QËi‰·Š */
	    switch(sub_menu)
	    {
	      case 0 :  /* ­QËi‰·Š */
		 c_tools();
		 break;
	      case 1 : /* •¡¶‘ i */
		 show_help();
		 break;
	      case 2 : /* i¼a - ¯¡ˆe */
		 date_time();
		 break;
	      case 3 : /*  e‘e”ib */
		 carlenda();
		 break;
	      case 4 : /* §¡£©¤åÑ¡ ¥e‰w */
		 password();
		 break;
	      case 5 : /* a—i·¡ ‹¡“w */
		 to_dos();
		 break;
	      case 6 : /* {‹¡ */
		 ending();
		 break;
		 }
		 break;
	 case 1 :  /* ÑÁ·© */
	    switch(sub_menu)
	  {
	      case 0 : /*»¡¶‹¡ */
		 file_del();
		 break;
	      case 1 : /*¥¢¬aÐa‹¡ */
		 file_copy();
		 break;
	      case 2 : /*·¡Ÿq¥e‰wÐa‹¡ */
		 file_ren();
		 break;
	      case 3 : /* ­¢¬÷ ¤aŽ‹¡ */
		 file_attr();
		 break;
	      case 4 : /* Íe»³ Ða‹¡ */
		 file_edit();
		 break;
	      }
	      break;
	 case 2 : /* —¡BÉ¡Ÿ¡ */
	   switch(sub_menu)
	   {
	      case 0 : /*  e—i‹¡ */
		 dir_make();
		 break;
	      case 1 : /* »¡¶‹¡ */
		 dir_del();
		 break;
	      case 2 : /* ·¡Ÿq ¤aŽ‹¡ */
		 dir_ren();
		 break;
	      case 3 : /* µ«‹¡‹¡ */
		 dir_mov();
		 break;
		 }
		 break;
	 case 3 : /* —¡¯aÇa */
	    switch(sub_menu)
	    {
	      case 0 : /* Á¡‹¡ÑÁ Ða‹¡ */
		 disk_del();
		 break;
	      case 1 : /* ¥¢¬a Ða‹¡ */
		 disk_copy();
		 break;
	      case 2 : /* ·¡Ÿq ¤aŽ‹¡ */
		 disk_ren();
		 break;
	      case 3 : /* Íe»³Ða‹¡ */
		 disk_edit();
		 break;
		 }
		 break;
	 case 4 : /* ¸÷¥¡ */
	    switch(sub_menu)
	    {
	      case 0 : /* ­QËi ¸÷¥¡ */
		 info_com();
		 break;
	      case 1 : /* ¡A¡¡Ÿ¡ ¸÷¥¡ */
		 info_mem();
		 break;
	      case 2 : /* —¡¯aÇa ¸÷¥¡ */
		 info_disk();
		 break;
	      case 3 : /* —¡BÉ¡Ÿ¡ ¸÷¥¡ */
		 info_dir();
		 break;
	      case 4 : /* ÑÁ·© ¸÷¥¡ */
		 info_file();
		 break;
		 }
		 break;
	 case 5 : /* ¬—ÑÉ ¡A“A */
	    switch(sub_menu)
	    {
	      case 0 : /* ·©¸÷Îa */
		 iljung();
		 break;
	      case 1 : /* ˆa‰¦ */
		 gagebu();
		 break;
	      case 2 : /* º­¡¢ */
		 adress();
		 break;
	      case 3 : /* ·©‹¡¸w */
		 diary();
		 break;
	      case 4 : /* ‰¬e‹¡ */
		 calc();
		 break;
	      case 5 : /* ¶Íe¤åÑ¡ */
		 mail_number();
		 break;
	      case 6 : /* ­A‰· ¯¡ˆe */
		 world_time();
		 break;
		 }
		 break;
	 case 6 : /* Ëb¥i ¡A“A */
	    switch(sub_menu)
	    {
	      case 0 : /* ¢…¬á Íe»³‹¡ */
		 wordpro();
		 break;
	      case 1 : /* ‹aœÏ¢ */
		 graph_tool();
		 break;
	      case 2 : /* ·q´b */
		 music_tool();
		 break;
	      case 3 : /* ‰A·± */
		 game_tool();
		 break;
		 }
		 break;
		 }
	 }
 }

 void disp_status(void)
 {
  draw_status_han();
  set_attr_han(FONT_INVERSE);
  set_vbcolor_han(LIGHTGRAY, BLACK);
 }
 void draw_top(void)
 {
  struct date cur_d;
  struct time cur_t;

  naputs_han(0, 0, "", 80, FONT_INVERSE | FONT_UNDERLINE);
  aputs_han((80 - 24) / 2, 0, "­QËi ‰·Š Ver0.1", FONT_INVERSE | FONT_UNDERLINE);
  getdate(&cur_d); gettime(&cur_t);
  aprintf_han(1, 0, FONT_INVERSE | FONT_UNDERLINE, "%4d‘e %2d¶© %2d·©",
	     cur_d.da_year, cur_d.da_mon, cur_d.da_day);
  aprintf_han(65, 0, FONT_INVERSE | FONT_UNDERLINE, "%02d¯¡ %02d¦… %02dÁ¡",
	     cur_t.ti_hour, cur_t.ti_min, cur_t.ti_sec);
  aput_eng_font(18, 0, 179, FONT_INVERSE | FONT_UNDERLINE);
  aput_eng_font(56, 0, 179, FONT_INVERSE | FONT_UNDERLINE);
  aput_eng_font(63, 0, 179, FONT_INVERSE | FONT_UNDERLINE);
  show_watch();
  }
 void show_watch(void)
 {
  static struct date prv_d; struct date cur_d;
  static struct time prv_t; struct time cur_t;
  static int called_num = 0;

  getdate(&cur_d); gettime(&cur_t);

  if(cur_t.ti_sec != prv_t.ti_sec)
  {
   aprintf_han(75, 0, FONT_INVERSE | FONT_UNDERLINE, "%2d", cur_t.ti_sec);
   if(cur_t.ti_min != prv_t.ti_min)
   {
    aprintf_han(70, 0, FONT_INVERSE | FONT_UNDERLINE, "%2d", cur_t.ti_min);
    if(cur_t.ti_min != prv_t.ti_hour)
    {
     if(called_num != 0)
      play("o3l4ceg>c2");
      else
	 called_num = 1;
      if(cur_t.ti_hour / 12)
	 aputs_han(58, 0, "µ¡Ò", FONT_INVERSE | FONT_UNDERLINE);
      else
	 aputs_han(58, 0, "µ¡¸å", FONT_INVERSE | FONT_UNDERLINE);
      if(cur_t.ti_hour % 12 == 0)
	 aputs_han(65, 0, "12", FONT_INVERSE | FONT_UNDERLINE);
      else
	 aprintf_han(65, 0, FONT_INVERSE | FONT_UNDERLINE, "%2d", cur_t.ti_hour % 12);
      if(cur_d.da_day != prv_d.da_day)
     {
      aprintf_han(1, 0, FONT_INVERSE | FONT_UNDERLINE, "%4d‘e %2d¶© %2d·©",
		  cur_d.da_year, cur_d.da_mon, cur_d.da_day);
      prv_d = cur_d;
      }
      prv_t.ti_hour = cur_t.ti_hour;
      }
      prv_t.ti_min = cur_t.ti_min;
      }
      prv_t.ti_sec = cur_t.ti_sec;
      }
    }
 void disp_logo(void)
 {
  int box_x;

  set_bkcolor_han(BLUE);
  clrscr_han();
  box_x = (abs_maxx_han + 1 ) / 2 -20;
  set_vbcolor_han(LIGHTGRAY, CYAN);
  set_attr_han(FONT_INVERSE);
  set_bkcolor_han(BLACK);
  draw_box_han(box_x, 12, box_x + 40, 12, BOX_H1V1);
  if(in_vga_system)
  vbputs_han(box_x, 13,    "         ­QËi‰·Š Ver 0.1                ",DARKGRAY, YELLOW);
  else
  aputs_han(box_x, 13,     "         ­QËi‰·Š Ver 0.1                ",FONT_SHADE | FONT_INVERSE);
  draw_box_han(box_x, 14, box_x + 40, 14, BOX_H1V1);
  set_bkcolor_han(BLUE);
  xputs_han(box_x, 15,     "   e—e·¡ : ‹±¡wÑÅ(ÐeÄñ1,HanCom1,hancom1) ");
  aputs_han(box_x, 16,     "   e—e‰µ : ÐeÄñ ¯¡¯aÉQ 1992              ",FONT_OUTLINE | FONT_INVERSE);
  xputs_han(box_x, 17,     "  Copyright (c) HanCom System 1992       ");
  default_color();
  set_attr_han(FONT_NORMAL);
  draw_top();
  set_han_wait_func(show_watch);
  clear_key_buff();
  if(wait_key_han(5))
  {
   if(!getch_han()) getch_han();
   }
  set_bkcolor_han(BLUE);
  clrscr_han();
  box_x = (abs_maxx_han + 1) /2 -20;
  set_vbcolor_han(LIGHTGRAY, CYAN);
  set_attr_han(FONT_INVERSE);
  set_bkcolor_han(BLACK);
  default_color();
  set_attr_han(FONT_NORMAL);
  draw_top();
  set_han_wait_func(show_watch);
  clear_key_buff();
  }