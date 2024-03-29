/**************************************************************************/
/*  Ή�a���w : �Q�i���� ver0.1 (Computer Tools 0.1)                       */
/*  �a���a���� : �� �w�� (�e��1 , HanCom1 , hancom1)                      */
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
      7, 1,        /* ���A�A�� �����e 7������ �A���A�e 1�� �A�A�a ��Ȃ�E�a.*/
      { 1, 15, 25, 35, 45, 55, 65 },      /* ���A�A 7���� x ���a */
      { " <�Q�i����> ", " ���� ", " ���Bɡ�� ", " ���a�a ", " ���� ",
	" ���ɡA�A ", " �b�i�A�A ",
       },
      /* �� �����A�ᦁ�� ���A�A 7 ���a �����E�a. */
      { /* 0 �� �A�A <�Q�i����> */
	{ 7, 0, 1, 2,                      /* (1, 1)�A 6���a�� ���A�A�i ���� */
	  { " <�Q�i����>   ",
            " �����i    F1 ",
	    " �i�a . ���e  ",
	    " �e�e�i�b     ",
	    " ������ѡ�ş� ",
	    " �a�i�� ���w  ",
	    " �{����       ",
          }
        },
	/* 1 �� �A�A(����) */
	{ 5, 0, 15, 2,                      /* (15, 1)�A 5���a�� ���A�A�i ���� */
	  { "������       ",
	    "���a�a��     ",
	    "���q�e�w�a�� ",
	    "�����a����   ",
	    "�e���a��     ",
          }
        },
	/* 2 �� �A�A(���Bɡ��) */
	{ 4, 0, 25, 2,                    /* (25, 1)�A 4���a�� ���A�A�i ���� */
	  { " �e�i��     ",
	    " ������     ",
	    " ���q�a���� ",
	    " ������     ",
          }
        },
	/* 3 �� �A�A(���a�a) */
	{ 4, 0, 35, 2,                    /* (35, 1)�A 4���a�� ���A�A�i ���� */
	  { " �������a�� ",
	    " ���a�a��   ",
	    " ���q�a���� ",
	    " �e���a��   ",
          }
        },
	/* 4 �� �A�A(����) */
	{ 5, 0, 45, 2,                    /* (45, 1)�A 5���a�� ���A�A�i ���� */
	  { " �Q�i����     ",
	    " �A��������   ",
	    " ���a�a����   ",
	    " ���Bɡ������ ",
	    " ��������     ",
          }
        },
	/* 5 �� �A�A(���ɡA�A) */
	{ 7, 0, 55, 2,                    /* (50, 1)�A 7���a�� ���A�A�i ���� */
	  { " �����a     ",
	    " �a����     ",
	    " ������     ",
	    " �����w     ",
	    " ���e��     ",
	    " ���e��ѡ   ",
	    " �A�������e ",
          }
        },
	/* 6 �� �A�A(�b�i�A�A) */
	{ 4, 0, 61, 2,                    /* (55, 1)�A 4���a�� ���A�A�i ���� */
	  { " �����e���� ",
	    " �a��Ϣ     ",
	    " �q�b       ",
	    " �A��       ",
          }
        },
      }
   };

/**************************************************************************/
/*              �q �� �� �w                                               */
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
   char key;                                           /* �a���b�A ���a�� ǡ */
   int  main_menu, sub_menu;                 /* ��Ȃ�E ���A�A�� ���A�A */
   set_internal_font();
   if(init_han2() != OK_HAN) exit(1);
   status_on_han();
   disp_status();
   disp_logo();

   while(1)
   {
      /* �� �����A�� Ή�a�� �A�A�a �����E�a. */
      key = pulldown_han(&menu_data);

      if(key == ESC) break;                  /* <Esc> ǡ�i ���ᕡ �{�� �e�a. */
      /* ��Ȃ�E ���A�A�e main_menu �A, ���A�A�e sub_menu�A ��w�e�a. */
      main_menu = menu_data.current;
      sub_menu  = menu_data.sub_menu[main_menu].current;

      /* �� �����A�� '�{' ���A�� �A�A�� �q�A �១Ё ���e �E�a. */
      switch(main_menu)
      {
	 case 0 :   /* �Q�i���� */
	    switch(sub_menu)
	    {
	      case 0 :  /* �Q�i���� */
		 c_tools();
		 break;
	      case 1 : /* �����i */
		 show_help();
		 break;
	      case 2 : /* �i�a - ���e */
		 date_time();
		 break;
	      case 3 : /* �e�e�i�b */
		 carlenda();
		 break;
	      case 4 : /* ������ѡ �e�w */
		 password();
		 break;
	      case 5 : /* �a�i�� ���w */
		 to_dos();
		 break;
	      case 6 : /* �{���� */
		 ending();
		 break;
		 }
		 break;
	 case 1 :  /* ���� */
	    switch(sub_menu)
	  {
	      case 0 : /*������ */
		 file_del();
		 break;
	      case 1 : /*���a�a�� */
		 file_copy();
		 break;
	      case 2 : /*���q�e�w�a�� */
		 file_ren();
		 break;
	      case 3 : /* ���� �a���� */
		 file_attr();
		 break;
	      case 4 : /* �e�� �a�� */
		 file_edit();
		 break;
	      }
	      break;
	 case 2 : /* ���Bɡ�� */
	   switch(sub_menu)
	   {
	      case 0 : /* �e�i�� */
		 dir_make();
		 break;
	      case 1 : /* ������ */
		 dir_del();
		 break;
	      case 2 : /* ���q �a���� */
		 dir_ren();
		 break;
	      case 3 : /* ������ */
		 dir_mov();
		 break;
		 }
		 break;
	 case 3 : /* ���a�a */
	    switch(sub_menu)
	    {
	      case 0 : /* ������ �a�� */
		 disk_del();
		 break;
	      case 1 : /* ���a �a�� */
		 disk_copy();
		 break;
	      case 2 : /* ���q �a���� */
		 disk_ren();
		 break;
	      case 3 : /* �e���a�� */
		 disk_edit();
		 break;
		 }
		 break;
	 case 4 : /* ���� */
	    switch(sub_menu)
	    {
	      case 0 : /* �Q�i ���� */
		 info_com();
		 break;
	      case 1 : /* �A���� ���� */
		 info_mem();
		 break;
	      case 2 : /* ���a�a ���� */
		 info_disk();
		 break;
	      case 3 : /* ���Bɡ�� ���� */
		 info_dir();
		 break;
	      case 4 : /* ���� ���� */
		 info_file();
		 break;
		 }
		 break;
	 case 5 : /* ���� �A�A */
	    switch(sub_menu)
	    {
	      case 0 : /* �����a */
		 iljung();
		 break;
	      case 1 : /* �a���� */
		 gagebu();
		 break;
	      case 2 : /* ������ */
		 adress();
		 break;
	      case 3 : /* �����w */
		 diary();
		 break;
	      case 4 : /* ���e�� */
		 calc();
		 break;
	      case 5 : /* ���e��ѡ */
		 mail_number();
		 break;
	      case 6 : /* �A���� ���e */
		 world_time();
		 break;
		 }
		 break;
	 case 6 : /* �b�i �A�A */
	    switch(sub_menu)
	    {
	      case 0 : /* ���� �e���� */
		 wordpro();
		 break;
	      case 1 : /* �a��Ϣ */
		 graph_tool();
		 break;
	      case 2 : /* �q�b */
		 music_tool();
		 break;
	      case 3 : /* �A�� */
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
  aputs_han((80 - 24) / 2, 0, "�Q�i ���� Ver0.1", FONT_INVERSE | FONT_UNDERLINE);
  getdate(&cur_d); gettime(&cur_t);
  aprintf_han(1, 0, FONT_INVERSE | FONT_UNDERLINE, "%4d�e %2d�� %2d��",
	     cur_d.da_year, cur_d.da_mon, cur_d.da_day);
  aprintf_han(65, 0, FONT_INVERSE | FONT_UNDERLINE, "%02d�� %02d�� %02d��",
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
	 aputs_han(58, 0, "��ҁ", FONT_INVERSE | FONT_UNDERLINE);
      else
	 aputs_han(58, 0, "����", FONT_INVERSE | FONT_UNDERLINE);
      if(cur_t.ti_hour % 12 == 0)
	 aputs_han(65, 0, "12", FONT_INVERSE | FONT_UNDERLINE);
      else
	 aprintf_han(65, 0, FONT_INVERSE | FONT_UNDERLINE, "%2d", cur_t.ti_hour % 12);
      if(cur_d.da_day != prv_d.da_day)
     {
      aprintf_han(1, 0, FONT_INVERSE | FONT_UNDERLINE, "%4d�e %2d�� %2d��",
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
  vbputs_han(box_x, 13,    "         �Q�i���� Ver 0.1                ",DARKGRAY, YELLOW);
  else
  aputs_han(box_x, 13,     "         �Q�i���� Ver 0.1                ",FONT_SHADE | FONT_INVERSE);
  draw_box_han(box_x, 14, box_x + 40, 14, BOX_H1V1);
  set_bkcolor_han(BLUE);
  xputs_han(box_x, 15,     "  �e�e�� : ���w��(�e��1,HanCom1,hancom1) ");
  aputs_han(box_x, 16,     "  �e�e�� : �e�� ���a�Q 1992              ",FONT_OUTLINE | FONT_INVERSE);
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