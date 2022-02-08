/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          <HAN> demo program - subprogram                     */
/*   FILENAME           demosub.c                                           */
/*   DATE & TIME        07/08/92 (WED) 03:50                                */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <alloc.h>
#include <time.h>
#include <math.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>

#include "hanio.h"
#include "extendio.h"
#include "misc.h"
#include "keydef.h"

/****************************************************************************/
/*                            External Variables                            */
/****************************************************************************/

extern int maxx, maxy, minx, miny, system_maxx, system_maxy;
extern int base_color, base_bkcolor;
extern char *type_any_key;

/****************************************************************************/
/*                   Prototypes of the External Functions                   */
/****************************************************************************/

void draw_logo(void);           /* ¡‰¡Ÿi ‹aŸ¥”a. */
void do_work(void);             /* Ðw¡¢·i ¬åÈ‚¤h·e Ò ¯©Ð—Ðe”a. */
void ask_quit(void);            /* Ïa¡‹aœ‘·i {‰»¡ ¢ˆ“e”a. */
void show_help(void);           /* •¡¶‘ i·i ¥¡µa º…”a. */

void draw_dummy_screen(void);   /* ¯¡Ðñ¶w ÑÁ¡e·i Š¬÷Ðe”a. */
void wait(char *msg);           /* ¡A¯¡»¡Ÿi Œq¨b·¡¡e¬á Ç¡·³b·i ‹¡”aŸ¥”a. */
void draw_fourier(int term_no, int peek); /* Ìé¯aŸi ¬a·¥· Ðs·a¡ ‹aŸ¥”a. */
char *color_to_str(int color);  /* color ¤å ¬‚· ·¡Ÿq·i Ÿ¡ÈåÐe”a. */
int  error_handler(int errval, int ax, int bp, int si); /* µAœá Ð…—iœá */
int  ctrl_handler(void);	/* control-break Ð…—iœá */

/* ·³ÁA ¤b¯a· ÉA–Ÿ¡Ÿi ‹aŸ¥”a. */
void draw_3dborder(int left, int top, int right, int bottom, int is_relief);
/* ·³ÁA ¤b¯aŸi ‹aŸ¥”a. */
void draw_3dbox(int left, int top, int right, int bottom, int color);

void show_graph_and_han(void);  /* Ðe‹i‰Á ‹aœÏ¢ */
void show_pie_chart(void);      /* Ìa·¡ À¡Ëa */
void show_gr_save(void);        /* ‹aœÏ¢ ÑÁ¡e· ¸á¸w */
void change_palette(void);      /* ÌiAËa ¤aŽ‹¡ */
void show_3dbox(void);          /* ·³ÁA ¤b¯a */
void show_play_music(void);     /* ·q´b µeº */
void show_popup_menu(void);     /* Ìs´ó ¡A“A */
void show_pulldown_menu(void);  /* Î‰”a¶… ¡A“A */
void to_dos(void);              /* •¡¯a®É·i Ðe”a. */

/* ”a·q Ðq®—i·e handemo.c µA ¸÷·–A´á ·¶”a. */
void introduce(void);           /* ­¡ˆ‹i·i ¥¡µaº…”a. */
void to_dos(void);              /* •¡¯a®É·i Ðe”a. */
void show_color(void);          /* ¬‚Œi ¥¡‹¡ */
void change_color(void);        /* ¬‚Œi ¤aŽ‹¡ */
void show_save_text(void);      /* ÑÁ¡e ¸á¸w‰Á ¥¢Š */
void show_modify_text(void);    /* ÑÁ¡e ¥eÑÁ */
void show_scroll_updn(void);    /* ¯aÇa© */
void change_cursor(void);       /* Äá¬á ¡¡´· */
void show_modify_char(void);    /* ‹i¸a ¡¡´· */
void show_double_char(void);    /* Ðe‹i ÑÂ” */
void show_replace_font(void);   /* Í¥ËaŸi ÑÁ·©µA¬á ·ª´á¬á ¤aŽ‹¡ */
void show_set_font(void);       /* Í¥ËaŸi ¡A¡¡Ÿ¡µA¬á ¤aŽ‹¡ */
void show_spc_font(void);	/* Ëb® ¢…¸a */
void show_output_offset(void);  /* Â‰b µ³­U */
void show_table(void);          /* •¡Îa ¸b¬÷ */
void show_hanja(void);          /* Ðe¸a ·³Â‰b */
void toggle_keyboard_type(void);/* 2/3¤é¯¢ ¸åÑÅ */
void show_status_line(void);    /* ¬wÈ œa·¥ */
void show_adv_gets(void);       /* Wordstar Ñw ·³bÐq® */
void show_keyflag(void);	/* Ç¡¥¡—a Ïiœ‹a */
void read_file(void);           /* ÑÁ·© ·ª‹¡ */

/****************************************************************************/
/*                    Implementation of the Subfunctions                    */
/****************************************************************************/

void draw_logo(void)
/* ¡‰¡ÑÁ¡e·i Â‰bÐe”a */
{
   int box_x, box_y;

   set_vbcolor_han(base_color, base_bkcolor); /* ‹¡¥¥¬‚·a¡ ¬é¸÷ */
   clrscr_han();

   /* ¡‰¡ ÑÁ¡e· ¬w”e ¶E½¢ ¡¡¬áŸ¡ ¹ÁÎaŸi ‰¬e(ÑÁ¡e· º—´wµA ¶áÃ¡Ða•¡¢) */
   box_x = (maxx - minx + 1) / 2 - 18;
   box_y = (maxy - miny + 1) / 2 - 4;

   center_puts_han(box_y - 4, " Ðe‹i œa·¡§aœáŸ¡ <Ðe> 4.1 ", BLUE,
		   FONT_SHADOW | FONT_HORIZ_DBL | FONT_VERT_DBL);
   /* º¡A“AŸi ¡‰¡µÁ ÐqA aÈaa•¡¢ £¡Ÿ¡ ‹aa–…”a */
   set_vbcolor_han(LIGHTGRAY, BLACK);
   naputs_han(0, 0, "  \xd4\xff        ÑÁ¡e ¹A´á       ¢…¸a        ·³b        "
	      "‹aœÏ¢        ·q´b    ‹¡Èa", system_maxx + 1, FONT_INVERSE);

   set_vbcolor_han(LIGHTGRAY, LIGHTGRAY);
   aclear_text_han(box_x, box_y, box_x+37, box_y+7, FONT_INVERSE);
   set_bkcolor_han(RED);
   aputs_han(box_x, box_y+1, " Ðe‹i œa·¡§aœáŸ¡ <Ðe> •A¡¡¶w Ïa¡‹aœ‘ ", FONT_SHADOW | FONT_INVERSE);
   draw_horiz_line(box_x*8, (box_y+3)*16-4, 38*8, BLACK);
   set_bkcolor_han(BLUE);
   aputs_han(box_x, box_y+3, "                Ver  4.1              ", FONT_SHADE | FONT_INVERSE);
   draw_horiz_line(box_x*8, (box_y+4)*16+4, 38*8, BLACK);
   set_bkcolor_han(DARKGRAY);
   aputs_han(box_x, box_y+5, "      e—e ·¡ : ·¡ ÑeÑ¡(easyride)      ", FONT_OUTLINE | FONT_INVERSE);
   aputs_han(box_x, box_y+6, "      e—e i : 1992 ‘e 7 ¶© 8 ·©      ", FONT_INVERSE);

   wait_key_han(5);
   /* wait_key_han(5) “e 5 Á¡ µA Ç¡Ÿi ’Ÿa¡e ‹a Ç¡Ÿi ·ª»¡ ´g‰¡ ¤a¡ Ÿ¡Èå */
   /* Ða£a¡ ’‰œá»¥ Ç¡Ÿi ·ª´á º´á´¡ Ðe”a.                                 */
   if(kbhit() && !getch_han()) getch_han();
}

#define MAIN_MENU_NUM           7     /* º¡A“A· ˆ•® */

void do_work(void)
/* Î‰”a¶… ¡A“A¡ Ðw¡¢·i ¬åÈ‚¤h·e Ò Ðw¡¢¥i¡ Ðq®Ÿi Ñ¡Â‰Ðe”a. */
{
   /* Î‰”a¶… ¡A“A“e pulldown_han() ·a¡  e—a“e ˆõ·¡ Ò©³¥  ˆe”eÐa»¡ e */
   /* µa‹¡¬á“e menu_han() · ¬a¶w¤ó·i ¥¡·¡‹¡ ¶áÐ menu_han() ·a¡ Š */
   /* ÑeÐaµv”a.                                                      */

   /* ¡¡—e ¡A“A¸aža“e ”a·q‰Á ˆ{·e ®…¬á¡ ¸÷·Ðaœa.            */
   /* 1. menu_struct º— ¤µi e ¹A¶AÐe ¶a­¡—i·i  å¸á ¸÷·Ðe”a. */
   /* 2. ˆb ¤µi ¶a­¡Ÿi ¥i•¡· ·¡Ÿq·i ˆa»¡“e ¤µi¡ ¸÷·Ðe”a. */
   /* 3. ”·³¢…·a¡ ˆb ¤µi ¶a­¡Ÿi ”·³Ðe”a.                  */

   /* º¡A“A ¸aža ¸÷· */

   /* 1. º¡A“AµA ‰ÅÐe ¸ažaµA¬á ¤µi e ¹A¶AÐe ¶a­¡—i·i ¸÷· */
   menu_struct main_menu = { 7, 0, 0, 0, -1, FONT_INVERSE, 0, 0, 79, 0, };
   /* 2. ˆb ¤µi ¶a­¡Ÿi ¥i•¡· ·¡Ÿq·i ˆa»¡“e ¤µi¡ ¸÷· */
   char main_menux[MAIN_MENU_NUM] = { 1, 11, 27, 39, 51, 65, 73 },
	main_menuy[MAIN_MENU_NUM] = { 0,  0,  0,  0,  0,  0,  0 };
   char *(main_menu_string[MAIN_MENU_NUM]) =
   {
      " \xd4\xff ", " ÑÁ¡e ¹A´á ", " ¢…¸a ", " ·³b ", " ‹aœÏ¢ ", " ·q´b ", " ‹¡Èa "
   };
   /* 3. ­A¤å¼ ‰Á¸÷·e ´aœµA ·¶”a */

   /* ¦¡A“A ¸aža ¸÷· */

   /* 1. ¦¡A“AµA ‰ÅÐe ¸ažaµA¬á ¤µi e ¹A¶AÐe ¶a­¡—i·i ¸÷· */
   menu_struct sub_menu[MAIN_MENU_NUM] =
   { { 0, },
     { 6, 0, 1, 1, BOX_H1V1, FONT_NORMAL, 11, 1, 26,  8, },
     { 8, 0, 1, 1, BOX_H1V1, FONT_NORMAL, 27, 1, 41, 10, },
     { 5, 0, 1, 1, BOX_H1V1, FONT_NORMAL, 39, 1, 58,  7, },
     { 5, 0, 1, 1, BOX_H1V1, FONT_NORMAL, 51, 1, 70,  7, },
     { 0, },
     { 4, 0, 1, 1, BOX_H1V1, FONT_NORMAL, 64, 1, 78,  6, },
   };
   /* 2. ˆb ¤µi ¶a­¡Ÿi ¥i•¡· ·¡Ÿq·i ˆa»¡“e ¤µi¡ ¸÷· */
   /* 1, 6 ¤å¼ ¦¡A“A ¸aža“e ¡A“A· ˆ•®ˆa 0 ·¡£a¡ ¸÷·Ði Ï©¶a´ô”a. */
   char sub_menux2[6] = { 12, 12, 12, 12, 12, 12 },         sub_menuy2[6] = { 2, 3, 4, 5, 6, 7 },
	sub_menux3[8] = { 28, 28, 28, 28, 28, 28, 28, 28 }, sub_menuy3[8] = { 2, 3, 4, 5, 6, 7, 8, 9 },
	sub_menux4[5] = { 40, 40, 40, 40, 40 },             sub_menuy4[5] = { 2, 3, 4, 5, 6 },
	sub_menux5[5] = { 52, 52, 52, 52, 52 },             sub_menuy5[5] = { 2, 3, 4, 5, 6 },
	sub_menux7[4] = { 65, 65, 65, 65 },                 sub_menuy7[4] = { 2, 3, 4, 5 };
   char *(sub_menu2_string[6]) =
        {
	   " ¬‚Œi ¥¡‹¡    ",
	   " ¬‚Œi ¤aŽ‹¡  ",
	   " ÑÁ¡e ¸á¸w    ",
           " ÑÁ¡e ¥eÑÁ    ",
           " ¬w/Ða ¯aÇa© ",
	   " Äá¬á ¤aŽ‹¡  "
        },
	*(sub_menu3_string[8]) =
	{
	   " ¢…¸a ¡¡´·   ",
	   " ÑÂ” ¢…¸a   ",
	   " ÑÁ·© Í¥Ëa   ",
	   " ¡A¡¡Ÿ¡ Í¥Ëa ",
	   " Ëb® ¢…¸a   ",
	   " Â‰b µ³­U   ",
	   " •¡Îa ¸b¬÷   ",
	   " Ðe¸a ·³Â‰b "
        },
	*(sub_menu4_string[5]) =
	{
	   " ¸aÌe ¤aŽ‹¡      ",
	   " ¬wÈ œa·¥        ",
	   " Ð·¬w–E ·³b Ðq® ",
	   " Ç¡¥¡—a Ïiœ‹a    ",
	   " ÑÁ·© ·ª‹¡        "
	},
	*(sub_menu5_string[5]) =
        {
	   " Ðe‹i‰Á ‹aœÏ¢    ",
           " Ìa·¡(pie) À¡Ëa   ",
	   " ·³ÁA ¤b¯a        ",
	   " ÌiU ¤aŽ‹¡      ",
	   " ‹aœÏ¢ ÑÁ¡e ¸á¸w ",
        },
	*(sub_menu7_string[4]) =
        {
           " Ìs´ó   ¡A“A ",
	   " Î‰”a¶… ¡A“A ",
	   " •¡¯a¡      ",
	   " {          "
	};
   void (*(sub_menu7_func[4])) (void) = { NULL, NULL, NULL, ask_quit };
   /* 3. ­A¤å¼ ‰Á¸÷·e ´aœµA ·¶”a */
   int key, cur_top, cur_sub, i;

   /* 3. ”·³¢…·a¡ ¤µi ¶a­¡Ÿi ”·³Ða“e ‰Á¸÷ */
   main_menu.x      = main_menux;
   main_menu.y      = main_menuy;
   main_menu.string = main_menu_string;

   sub_menu[1].x = sub_menux2; sub_menu[1].y = sub_menuy2;
   sub_menu[2].x = sub_menux3; sub_menu[2].y = sub_menuy3;
   sub_menu[3].x = sub_menux4; sub_menu[3].y = sub_menuy4;
   sub_menu[4].x = sub_menux5; sub_menu[4].y = sub_menuy5;
   sub_menu[6].x = sub_menux7; sub_menu[6].y = sub_menuy7;

   sub_menu[1].string = sub_menu2_string;
   sub_menu[2].string = sub_menu3_string;
   sub_menu[3].string = sub_menu4_string;
   sub_menu[4].string = sub_menu5_string;
   sub_menu[6].string = sub_menu7_string;

   sub_menu[6].func = sub_menu7_func;

   while(1)
   {
      set_attr_han(FONT_NORMAL);
      set_vbcolor_han(base_color, base_bkcolor);
      clrscr_han();
      /* º¡A“AŸi Îa¯¡Ðe”a. */
      cur_top = main_menu.current;
      set_vbcolor_han(LIGHTGRAY, BLACK);
      naputs_han(0, 0, "", system_maxx + 1, main_menu.attr);
      for(i = 0; i < main_menu.number; i++)
	 aputs_han(main_menu.x[i], main_menu.y[i], main_menu.string[i], main_menu.attr);
      aputs_han(main_menu.x[cur_top], main_menu.y[cur_top], main_menu.string[cur_top], main_menu.attr^FONT_INVERSE);

      do
      {
	 /* ¦¡A“AŸi ¬åÈ‚¤h“e”a. */
	 set_vbcolor_han(BLACK, LIGHTGRAY);
	 key = menu_han(&sub_menu[cur_top]);
         cur_sub = sub_menu[cur_top].current;

	 /* ¦¡A“AµA¬á <Left>, <Right>, <Esc> Ç¡Ÿi ’‰œá¬á ¨a¹aaµ¥ ‰w¶ */
	 set_vbcolor_han(LIGHTGRAY, BLACK);
	 if(key == LEFT)
	 {
	    /* º¡A“A ¤aŸi ·¡•· */
	    aputs_han(main_menu.x[cur_top], main_menu.y[cur_top], main_menu.string[cur_top], main_menu.attr);
	    cur_top = (cur_top - 1 + main_menu.number) % main_menu.number;
            main_menu.current = cur_top;
	    aputs_han(main_menu.x[cur_top], main_menu.y[cur_top], main_menu.string[cur_top], main_menu.attr^FONT_INVERSE);
	 }
	 else if(key == RIGHT)
	 {
	    /* º¡A“A ¤aŸi ·¡•· */
	    aputs_han(main_menu.x[cur_top], main_menu.y[cur_top], main_menu.string[cur_top], main_menu.attr);
	    cur_top = (cur_top + 1) % main_menu.number;
            main_menu.current = cur_top;
	    aputs_han(main_menu.x[cur_top], main_menu.y[cur_top], main_menu.string[cur_top], main_menu.attr^FONT_INVERSE);
	 }
      }
      while(key != ENTER);	/* ¡A“Aˆa ¬åÈ‚–I ˜Œa»¡ ¤e¥¢ */

      /* µa‹¡Œa»¡ ¯©Ð—–A¡e ¡A“Aˆa µÅ¸åÓ¡ ¬åÈ‚–E ¬wÈ·¡”a. */
      set_vbcolor_han(base_color, base_bkcolor);
      clear_text_han(0, 0, system_maxx, 0);
      /* ¬åÈ‚–E ¡A“AµA ˜aœa Ðq®Ÿi Ñ¡Â‰Ðe”a. */
      switch(cur_top)
      {
	 case 0 :  	/* <Ðe> */
	    introduce();
	    break;
	 case 1 :    	/* ÑÁ¡e ¹A´á */
	    switch(cur_sub)
	    {
	       case 0 : show_color();       break;
	       case 1 : change_color();     break;
	       case 2 : show_save_text();   break;
               case 3 : show_modify_text(); break;
               case 4 : show_scroll_updn(); break;
	       case 5 : change_cursor();    break;
	    }
	    break;
	 case 2 :       /* ¢…¸a */
	    switch(cur_sub)
	    {
	       case 0 : show_modify_char();   break;
	       case 1 : show_double_char();   break;
	       case 2 : show_replace_font();  break;
	       case 3 : show_set_font();      break;
	       case 4 : show_spc_font();      break;
	       case 5 : show_output_offset(); break;
	       case 6 : show_table();         break;
               case 7 : show_hanja();         break;
	    }
	    break;
	 case 3 :  	/* ·³b */
	    switch(cur_sub)
	    {
	       case 0 : toggle_keyboard_type(); break;
	       case 1 : show_status_line();     break;
	       case 2 : show_adv_gets();        break;
	       case 3 : show_keyflag();         break;
	       case 4 : read_file();            break;
	    }
	    break;
	 case 4 :  	/* ‹aœÏ¢ */
	    switch(cur_sub)
	    {
	       case 0 : show_graph_and_han(); break;
               case 1 : show_pie_chart();     break;
	       case 2 : show_3dbox();         break;
	       case 3 : change_palette();     break;
	       case 4 : show_gr_save();       break;
	    }
	    break;
	 case 5 :       /* ·q´b */
	    show_play_music();
	    break;
	 case 6 :       /* ‹¡Èa */
	    switch(cur_sub)
	    {
	       case 0 : show_popup_menu();    break;
	       case 1 : show_pulldown_menu(); break;
	       case 2 : to_dos();             break;
	       case 3 : break;
               /* {·e menu_han() µA¬á ÑÁ¡e·¡ ¥¢Š–A‹¡ ¸åµA ask_quit() ·¡ */
               /* Ñ¡Â‰–Q·a¡°á ·¡ž´á»¥”a. ·¡œý‰A Ðe ˆõ·e Î‰”a¶… ¡A“Aˆa  Îa */
               /* ¯¡–A´á ·¶“e ¬wÈµA¬á ¯©Ð— ¹·žaŸi ¢‰´á ¥¡‹¡ ¶áÐ¬á·¡”a.    */
            }
	    break;
	 default :
            /* ·¡œáÐe ‰w¶“e ·¶·i ® ´ô»¡ e ¯©®Ÿi ”§¡Ð µAœá ÀáŸ¡Ÿi */
            /* Ð ‘½·a¡e ›uÐa»¡ ´g·e ¤á‹aŸi ®ó‰A Àx·i ® ·¶”a.        */
	    error_exit_msg = "Programming error";
	    error_exit_han("do_work", 1);
      }
   }
}

void ask_quit(void)
/* ¯©Ð—·i ¹·žaÐe”a. */
{
   char *screen;

   /* ÑÁ¡e ¸á¸w */
   screen = save_text_han((system_maxx+1)/2-16, 8, (system_maxx+1)/2+15, 12);
   if(screen == NULL)
   {
      wait("¡A¡¡Ÿ¡ˆa ¦¹¢Ðs“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
      close_han();
      exit(1);
   }

   set_vbcolor_han(BLACK, LIGHTGRAY);
   draw_var_box_han((system_maxx+1)/2-16, 8, (system_maxx+1)/2+15, 12, 1, FONT_NORMAL, BOX_H1V2);
   aputs_han((system_maxx+1)/2-14, 10, "¸÷ i {¯¡‰V¯s“¡Œa? <y/n>  ", FONT_NORMAL);
   clear_key_buff();

   if(get_yesno_han((system_maxx+1)/2+13,10,FONT_NORMAL) == YES)
   /* get_yesno_han() ·e 'Y' a 'y' Ÿi Ã¡¡e YES Ÿi Ÿ¡ÈåÐe”a */
   {
      set_bkcolor_han(BLACK);
      rand_clear_text_han(0, 0, system_maxx, system_maxy, FONT_NORMAL, 80*25*2, 0);
      /* rand_clear_text_han() ·e ÑÁ¡e· ·©¦¦…·i œ…”ñÐa‰A  »¡¶“e  Ðq®·¡”a. */
      /* parameter º— count = 80*25*2 ¡ Ðe ˆõ·e ‹i¸aŸi 80*25*2  ¤å  œ…”ñÐa‰A */
      /* »¡¶… Ò (º—¥¢–A“e ˆõ•¡ Í¡ÐqÐ¬á) ÑÁ¡e·i  Œu·¡  »¡¶œa“e  ·£¡·¡”a. */
      /* ‹i¸a 10 ˆŸi »¡¶‰¡ ‹¡”aŸ¡“e ¯¡ˆe·e delay_time(= 0) milisecond ·¡”a. */
      close_han();
      exit(0);
   }
   /* Ñe¸¬‚‰Á ¸á¸wÐ–”å ÑÁ¡e·i ¥¢Š */
   set_vbcolor_han(base_color, base_bkcolor);
   restore_text_han((system_maxx+1)/2-16, 8, screen);
}

/* •¡¶‘ i· ¦…œ·‰Á ÑÁ¡e Ça‹¡µA ‰Åe–E  Ça¡ ¬w® */
#define HELP_LINE       14                   /* ÑÁ¡eµA Îa¯¡–A“e º‰ ®    */
#define MAX_PAGE        13                   /* Îa¯¡Ði ® ·¶“e ÍA·¡»¡ ® */
#define MAX_HELP        HELP_LINE * MAX_PAGE /* Îa¯¡Ði ® ·¶“e º‰ ®     */

void show_help(void)
/* handemo.hlp ÑÁ·©·i ·ª´á¬á •¡¶‘ i·i ¥¡µa º…”a */
{
   FILE *stream;
   char (*help)[68+2], *screen;
   int  cur_page, re_draw, line_no, key, i;

   /* •¡¶‘ iµA Ï©¶aÐe ¡A¡¡Ÿ¡Ÿi Ði”wÐe”a */
   if((help = malloc((68+2)*MAX_HELP)) == NULL)
   {
      wait("¡A¡¡Ÿ¡ˆa ¦¹¢Ðs“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
      return;
   }

   /* •¡¶‘ i ÑÁ·© quiz.hlp · ¶w·i ·ª´á help µA ¸á¸wÐe”a */
   if((stream = fopen("handemo.hlp", "rt")) == NULL)
   {
      wait("•¡¶‘ i ÑÁ·©·i ·ª·i ® ´ô¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
      return;
   }
   line_no = 0;
   while(!feof(stream))
      if(line_no >= MAX_HELP || fgets(help[line_no], 70, stream) == NULL)
         break;
      else
      {
         /* ¢…¸aµi· {µA ¦›·e '\n' ¢…¸a“e ¬b¹AÐe”a */
         i = strlen(help[line_no]);
         if(help[line_no][i - 1] == '\n') help[line_no][i - 1] = 0;
         line_no++;
      }
   fclose(stream);

   set_vbcolor_han(BLACK, LIGHTGRAY);
   /* ÑÁ¡e·i Š¬÷Ðe”a */
   draw_3dbox(4, 2, abs_maxx_han-4, 2+HELP_LINE+3, LIGHTGRAY);
   set_output_offset_han(12);
   vputs_han(35, 2, "•¡ ¶‘  i", RED);
   draw_horiz_line(4*8+5, 4*16-2,  (abs_maxx_han-9)*8+5, BLUE);
   draw_horiz_line(4*8+5, 18*16+2, (abs_maxx_han-9)*8+5, BLUE);
   set_output_offset_han(4);
   vputs_han(8, 18, "Up/PgUp : ´| ÍA·¡»¡   Down/PgDn : ”a·q ÍA·¡»¡"
                    "   Esc : ‹a e ¥¡‹¡", YELLOW);
   set_output_offset_han(0);

   /* help µA ¸á¸w–E •¡¶‘ i·i ¥¡µa º…”a */
   cur_page = 0;
   re_draw  = 1;
   while(1)
   {
      if(re_draw)
      /* ÍA·¡»¡Ÿi ¤aŽ´ö”a¡e •¡¶‘ i·i ¬¡ Îa¯¡Ðe”a */
      {
         for(i = 0; i < HELP_LINE; i++)
            if(cur_page*HELP_LINE+i >= line_no)
               nputs_han(6, 4+i, "", 68);
            else
               nputs_han(6, 4+i, help[cur_page*HELP_LINE+i], 68);
      }

      /* ¬a¶w¸a· Ç¡·³b·i ÀáŸ¡Ðe”a */
      key = getch_han();
      if(key == _ESC)
         break;
      else if(key == 0)
      {
         switch(getch_han())
         {
            case UP   :
            case PGUP : if(cur_page > 0) cur_page--;
                        re_draw = 1;
                        break;
            case DOWN :
            case PGDN : if(cur_page < MAX_PAGE - 1) cur_page++;
                        re_draw = 1;
                        break;
         }
      }
   }

   /* help µA Ði”w–E ¡A¡¡Ÿ¡Ÿi ¥¢Šá¯¡Ç¥”a */
   free(help);
   set_vbcolor_han(base_color, base_bkcolor);
}

void draw_dummy_screen(void)
/* ¯¡Ðñ·i ¶áÐe ÑÁ¡e·i Š¬÷Ðe”a. */
{
   int i, attr2, color2;

   attr2  = attr_han;  /* Ñe¸ ­¢¬÷‰Á ¬‚ ¸á¸w */
   color2 = color_han;
   /* ­¢¬÷‰Á ¬‚·i ¤aŽ´á ˆa¡a ¢…¸aµi·i Â‰bÐe”a. */
   for(i = 0; i < system_maxy - 1; i++)
   {
      attr_han  = i;
      color_han = i % 16;
      puts_han("Ðe‹i œa·¡§aœáŸ¡ <Ðe> 4.1     Hangul library <HAN> 4.1");
   }
   attr_han  = attr2;  /* ¶¥œ ­¢¬÷‰Á ¬‚·a¡ ¥¢Š */
   color_han = color2;
}

void wait(char *msg)
/* msg Ÿi Â‰bÐe Ò ·³b”‹¡ ¯¡ˆe•·´e Œq¨bˆáŸ¡‰A Ðe”a. */
{
   char *screen, key;
   clock_t time1, time2;
   int x1, x2, y;

   x1 = (system_maxx+1)/2 - strlen(msg)/2;  /* ¸á¸wÐ´¡Ði ÑÁ¡e· x,y ¹ÁÎa */
   x2 = (system_maxx+1)/2 + strlen(msg)/2 + strlen(msg)%2 - 1;
   y = system_maxy;

   screen = save_text_han(x1, y, x2, y);    /* ÑÁ¡e·i screen µA ¸á¸wÐe”a */
   clear_key_buff();

   while(1)
   {
      set_vbcolor_han(YELLOW, RED);

      time1 = clock();
      while(!kbhit())
	 if(((time2 = clock()) - time1) > CLK_TCK / 2)
	 {
	    naputs_han(x1, y, "", strlen(msg), FONT_INVERSE);
	    time1 = time2;
	 }
	 else if(((time2 = clock()) - time1) > CLK_TCK / 4)
	    aputs_han(x1, y, msg, FONT_INVERSE);  /* msg Ÿi Â‰bÐe”a */

      set_vbcolor_han(base_color, base_bkcolor);
      if(getch_han() == 0 && getch_han() == ALTX)
	 ask_quit();
      else
	 break;
   }

   restore_text_han(x1, y, screen);         /* ÑÁ¡e·i ¥¢ŠÐaœa */
}

/* ”a·q·e ‹aœÏ¢µA ‰ÅÐe ˆõ·¡£a¡ ¬w­AÐe º¬â·i ¬—œ¢Ðe”a */

#define PI              3.141592
#define AMP_FACTOR      100/(2*peek/PI)  /* to componsate graphics cordinate */
#define ORIGIN_X        50
#define ORIGIN_Y        150
#define LIMIT_TIME      4
#define get_x(t)        (int) (ORIGIN_X + (t)/LIMIT_TIME*500)
#define get_y(v)        (int) (ORIGIN_Y - (v)*AMP_FACTOR)

/* f(t) = 2*peek/PI*(sin(PI*t) + 1/3*sin(3*PI*t) + 1/5*sin(5*PI*t) + ... )       */

void draw_fourier(int term_no, int peek)
{
   char string[10];
   float t, v, amp, ang_vel;
   int i;

   /* v,t Â‚·i ‹aŸ¥”a. */
   setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
   setcolor(WHITE);
   line(ORIGIN_X, ORIGIN_Y, get_x(LIMIT_TIME)+10, ORIGIN_Y);
   line(ORIGIN_X, get_y(peek/2)-10, ORIGIN_X, get_y(peek/2-peek)+10);
   settextjustify(LEFT_TEXT, CENTER_TEXT);
   outtextxy(get_x(LIMIT_TIME)+10+5, ORIGIN_Y, "t(s)");
   outtextxy(ORIGIN_X-16, get_y(peek/2)-15, "v(V)");

   /* ˆe‰b·i Îa¯¡Ðe”a. */
   settextjustify(RIGHT_TEXT, TOP_TEXT);
   for(t = 0; t <= LIMIT_TIME; t++)
   {
      line(get_x(t), ORIGIN_Y-2, get_x(t), ORIGIN_Y+2);
      itoa((int) t, string, 10);
      outtextxy(get_x(t)-2, ORIGIN_Y+2, string);
   }

   setlinestyle(USERBIT_LINE, 0x8080, NORM_WIDTH);
   line(ORIGIN_X, get_y(peek/2), get_x(LIMIT_TIME)+10, get_y(peek/2));
   itoa(peek/2, string, 10);
   outtextxy(ORIGIN_X-2, get_y(peek/2)+2, string);
   line(ORIGIN_X, get_y(peek/2-peek), get_x(LIMIT_TIME)+10, get_y(peek/2-peek));
   itoa(peek/2-peek, string, 10);
   outtextxy(ORIGIN_X-2, get_y(peek/2-peek)+2, string);

   /* Fourier ¯¡Ÿ¡»a· ÌaÑw·i ‹aŸ¥”a. */
   setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
   setcolor(LIGHTCYAN);
   moveto(ORIGIN_X, ORIGIN_Y);
   for(t = 0; t < LIMIT_TIME && !kbhit(); t += LIMIT_TIME/500.)
   /* 500 ˆ· ¸ñ·i ¿¢“e”a. */
   {
      v = 0;
      for(i = 0; i < term_no; i++)
      {
         amp = (peek * 2 / PI) * (1. / (i * 2 + 1));
         ang_vel = (i * 2 + 1) * PI;
         v += amp * sin(ang_vel * t);
      }
      lineto((int) get_x(t), (int) get_y(v));
   }
   if(t >= LIMIT_TIME) wait(type_any_key);
}

char *color_to_str(int color)
/* color ¤å ¬‚· ¬‚¡w·i Ÿ¡ÈåÐe”a. */
{
   switch(color)
   {
      case  0 : return("ˆñ¸÷");
      case  1 : return("Ìaœw");
      case  2 : return("‘¢¬‚");
      case  3 : return("Ða“i¬‚");
      case  4 : return("¨iˆw");
      case  5 : return("»¥¦…Ñ·");
      case  6 : return("ˆi¬‚");
      case  7 : return("¤j·e ÒA¬‚");
      case  8 : return("´á–¶… ÒA¬‚");
      case  9 : return("¤j·e Ìaœw");
      case 10 : return("µe–¬‚");
      case 11 : return("¤j·e Ða“i¬‚");
      case 12 : return("ºÑ×");
      case 13 : return("¦…Ñ·");
      case 14 : return("‘¡œw");
      case 15 : return("Ó…¬‚");
      default : return("");
   }
}

/* error_handler() · ·¥¸a—i·i ¬a¶wÐa»¡ ´g”áœa•¡ ÄñÌa·©¯¡µA ‰w‰¡ˆa */
/* aµ¡»¡ ´g•¡¢ Ðe”a.                                             */
#pragma warn -par

int error_handler(int errval, int ax, int bp, int si)
/* Ða—a¶Á´á¸â·¥ µAœáˆa ¤i¬—Ðaµv·i ˜ ¯©Ð—–A“e Ðq® */
{
   int box_x, box_y, key, bkcolor2;
   char *screen;

   box_x = (maxx - minx + 1) / 2 - 13;
   box_y = (maxy - miny + 1) / 2 - 2;

   if((screen = save_text_han(box_x, box_y, box_x + 25, box_y + 3)) == NULL)
   {
      wait("¡A¡¡Ÿ¡ˆa ¦¹¢Ðs“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
      return(2);
   }
   bkcolor2    = bkcolor_han;
   bkcolor_han = LIGHTGRAY;
   draw_box_han(box_x, box_y, box_x + 25, box_y + 3, BOX_H1V1);
   clear_text_han(box_x + 1, box_y + 1, box_x + 24, box_y + 2);
   vputs_han(box_x +  2, box_y + 1, " Ða—a¶Á´á µAœá ¤i¬—!!", RED);
   vputs_han(box_x +  2, box_y + 2, "A)Âá­¡, R)”a¯¡, F)¢¯¡", BLACK);
   vputs_han(box_x +  2, box_y + 2, "A", BLUE);
   vputs_han(box_x + 10, box_y + 2, "R", BLUE);
   vputs_han(box_x + 18, box_y + 2, "F", BLUE);
   bkcolor_han = bkcolor2;
   while(strchr("AaRrFf", key = getch_han()) == NULL) ;
   restore_text_han(box_x, box_y, screen);

   switch(key)
   {
      case 'A' :
      case 'a' : close_han(); hardresume(2); break;
      case 'R' :
      case 'r' : hardresume(1); break;
      case 'F' :
      case 'f' : hardresume(0); break;
   }
   return(2);
}

#pragma warn +par

int ctrl_handler(void)
/* <Ctrl>+<Break> Ç¡ˆa ’‰œá¹v·i ˜ Ñ¡Â‰–E”a. */
{
   /* <Ctrl>+<Break> Ç¡ˆa ’‰œá»¡¡e ¢¹¡ˆå ¯©Ð—·i ¹·žaÐe”a. */
   close_han();
   puts("<Ctrl>+<Break> hit. Program aborting...");
   return(0);
}

void draw_3dborder(int left, int top, int right, int bottom, int is_relief)
/* EGA/VGA µA¬á ·³ÁA ¤b¯aŸi  e—a“e•A Ï©¶aÐe ÉA–Ÿ¡Ÿi  ‹aŸ¥”a. */
/* left, top, right, bottom ·e ¡¡– ‹aœÏ¢ ¹ÁÎa”a.            */
/* is_relief ˆa 1 ·¡¡e ´·ˆb, 0 ·¡¡e ·qˆb·a¡ ÉA–Ÿ¡Ÿi ‹aŸ¥”a. */
{
   int width, height;

   if(!in_vga_system)
   {
      width  = right - left;
      height = bottom - top - 1;

      draw_vert_line(left,    top,   height, WHITE);
      draw_vert_line(right-1, top,   height, WHITE);
      draw_vert_line(right,   top+1, height, WHITE);

      draw_horiz_line(left,   top,      width,  WHITE);
      draw_horiz_line(left+1, bottom,   width,  WHITE);
      draw_horiz_line(left,   bottom-1, width,  WHITE);
   }
   else
   {
      width  = right - left + 1;
      height = bottom - top + 1;

      if(is_relief)
      {
         draw_horiz_line(left,   top,      width,   WHITE);
         draw_horiz_line(left,   bottom,   width,   DARKGRAY);
         draw_horiz_line(left+1, bottom-1, width-2, DARKGRAY);

         draw_vert_line(left,    top,   height,   WHITE);
         draw_vert_line(left+1,  top+1, height-2, WHITE);
         draw_vert_line(right,   top,   height,   DARKGRAY);
         draw_vert_line(right-1, top+1, height-2, DARKGRAY);
      }
      else
      {
         draw_horiz_line(left,   top,      width,   DARKGRAY);
         draw_horiz_line(left,   bottom,   width,   WHITE);
         draw_horiz_line(left+1, bottom-1, width-2, WHITE);

         draw_vert_line(left,    top,   height,   DARKGRAY);
         draw_vert_line(left+1,  top+1, height-2, DARKGRAY);
         draw_vert_line(right,   top,   height,   WHITE);
         draw_vert_line(right-1, top+1, height-2, WHITE);
      }
   }
}

void draw_3dbox(int left, int top, int right, int bottom, int color)
/* ¸é” ¹ÁÎa¡ (left, top) - (right, bottom) µwµbµA color µA »¡¸÷–E */
/* ¬‚·a¡ ·³ÁA ¤b¯aŸi ‹aŸ¥”a.                                       */
{
   int x1, y1, x2, y2;

   /* left, top, right, bottom µA Ð”wÐa“e ‹aœÏ¢ ¹ÁÎaŸi ‰¬e */
   x1     = left << 3;
   y1     = top  << 4;
   x2     = (right  << 3) + 7;
   y2     = (bottom << 4) + 15;

   vclear_text_han(left, top, right, bottom, color);
   if(in_vga_system)
   {
      draw_3dborder(x1,   y1,   x2,   y2,   1);
      draw_3dborder(x1+4, y1+8, x2-4, y2-8, 0);
   }
   else
   {
      draw_3dborder(x1, y1, x2, y2, 0);
      draw_horiz_line(x1+5, y1+9, x2-x1-9,  WHITE);
      draw_horiz_line(x1+5, y2-9, x2-x1-9,  WHITE);
      draw_vert_line( x1+5, y1+9, y2-y1-17, WHITE);
      draw_vert_line( x2-5, y1+9, y2-y1-17, WHITE);
   }
}

void show_graph_and_han(void)
/* Ðe‹i‰Á ‹aœÏ¢·¡ ‰·¹¥Ði ® ·¶·q·i ¥¡µa º…”a. */
{
   char string[MAX_STRLEN+1];
   int box_x, term_no, peek;

   adv_puts_han("<Ðe>·e ‹aœÏ¢·a¡ ‹i¸aŸi Â‰bÐa£a¡ ´a¢œå ¢…¹A´ô·¡ Ðe‹i‰Á "
                "‹aœÏ¢·i ¬ã´á¬á ÑÁ¡e·i Š¬÷Ði ® ·¶¯s“¡”a. ”a·q·e ‰·Ðb ¦…´¡"
                "µA¬á º—¶aÐa‰A ¬a¶w–A“e Fourier ¯¡Ÿ¡»aŸi ¯¡£IA·¡­eÐa“e µ"
                "·³“¡”a. ·¡ µµA¬á ¬a¶w–E Ðq®“e rectangular function ·³“¡”a"
                ". ·³b¬aÐw·e ¤e—a¯¡ 0 ¥¡”a Çe ¸÷®¡ ·³bÐa‰¡ ‹a e –a¡e "
                "‹a· <Enter> Ç¡Ÿi ’Ÿa­A¶a.\n", 0);
   puts_han("¡y ˆ· Ðw·a¡ ¬—¬÷Ða‰V¯s“¡Œa?");
   if(scanf_han("%d", &term_no) != 1) return;
   if(term_no <= 0) term_no = 1;

   clrscr_han();
   box_x = maxx - 38;
   set_vbcolor_han(DARKGRAY, LIGHTGRAY);
   aclear_text_han(box_x, maxy-7, box_x+36, maxy-2, FONT_INVERSE);
   set_attr_han(FONT_INVERSE);
   aputs_han(box_x, maxy-6, "  Fourier Series Simulation Program",
	     FONT_INVERSE | FONT_OUTLINE);
   draw_horiz_line(box_x*8, (maxy-6)*16-4, 37*8, LIGHTGRAY);
   draw_horiz_line(box_x*8, (maxy-5)*16+4, 37*8, LIGHTGRAY);
   xputs_han(box_x, maxy-4, "  e—e ·¡ : ‰w¦‚” ¸å¸a‰·Ðb‰Á ·¡ ÑeÑ¡");
   xputs_han(box_x, maxy-3, "  e—e i : 1991 ‘e 3 ¶© 8 ·©");
   set_vbcolor_han(base_color, base_bkcolor);
   set_attr_han(FONT_NORMAL);

   peek = 10;
   settextjustify(RIGHT_TEXT, BOTTOM_TEXT);
   draw_fourier(term_no, peek);

   clrscr_han();
   puts_han("Ðe‹i‰Á ‹aœÏ¢·¡ ¸åÑa Â—•©Ða»¡ ´g·q·i ÑÂ·¥Ða­v¯s“¡Œa?");
   wait(type_any_key);
}

#define STOCK_NUM 6             /* ¸‰¡ Î‘¡¢· ® */
#define PI        3.141592      /* ¶¥º·I */

void show_pie_chart(void)
/* Ìa·¡ À¡ËaŸi ¥¡µa º…”a. */
{
   static struct
   {
      char *name;               /* ´¢Î‘¡w */
      int  price;               /* ˆa‰b */
      int  number;              /* ¸‰¡œ· */
   } stock[STOCK_NUM] =         /* Ìa·¡ À¡ËaµA Îa¯¡–I ·´¢Î‘· ¸‰¡ ¸aža */
   {
      { "´a¡¢¬a¯©Ÿ¥ 250 mg 100 ¸÷",  800,  247 },
      { "´‘Ï¡Çiž‚¯a 500 mg 100 ¸÷", 1500,   95 },
      { "´‚Ë¡Ï¡—a 100 ¸÷",           800,  112 },
      { "¥A¯aÈa¹A 100 ¸÷",          1000,   59 },
      { "Çi¡ËaŸ¡ a¹© µe‰¡",        1800,  189 },
      { "Ò¯¡—¥ µe‰¡",              1200,   87 }
   };
   int   total_number;         /* Á· ¸‰¡œ· */
   long  total_price;          /* Á· ¸‰¡‹q´‚ */
   int   start_angle, end_angle, pie_y, x, y, i;
   char  string[10];
   float ratio;

   adv_puts_han("<Ðe>·i ¬a¶wÐa¡e Ðe‹i‰Á ‹aœÏ¢·i ´a¢œå Â—•©´ô·¡ Â‰bÐi ® ·¶"
                "·i ª… ´a“¡œa init_han() ·i Ñ¡Â‰Ðaµa Ðe‹i ÑÅ‰w·a¡ Á¡‹¡ÑÁÐ–”a"
                "¡e Èá¥¡ C · ‹aœÏ¢ Ðq®—i·i ¡¡– ¬a¶wÐi ® ·¶¯s“¡”a. ”a·q "
                "µ“e <Ðe>·i ¬a¶wÐa¡e Èá¥¡ C · ‹aœÏ¢ Ðq®—i•¡ ¬a¶wÐi ® ·¶"
		"”a“e ˆõ·i ¥¡µa º““¡”a. ·¡ µµA¬á“e pieslice(), setfillstyle()"
		", outtextxy() —w· ‹aœÏ¢ Ðq®—i·¡ ¬a¶w–S“¡”a.", 0);
   wait(type_any_key);
   set_vbcolor_han(WHITE, DARKGRAY);
   clrscr_han();

   /* Èa·¡Ëi‰Á Ðw¡¢·i Îa¯¡ */
   center_puts_han(1, "·´¢Î‘ ¸‰¡ ÑeÑ×", YELLOW, FONT_VERT_DBL | FONT_HORIZ_DBL);
   center_puts_han(3, "- 07/07/92 Ñe¸ -", LIGHTRED, FONT_NORMAL);
   settextjustify(LEFT_TEXT, TOP_TEXT);
   setcolor(WHITE);
   for(i = 0; i < STOCK_NUM; i++)
   {
      x = system_maxx / 2. * (i % 2) + 7;
      y = system_maxy - 4 + (i / 2);
      xputs_han(x + 3, y, stock[i].name);
      if(in_vga_system)
         setfillstyle(SOLID_FILL, 9 + i);
      else
	 setfillstyle(i + 1, WHITE);
      bar(x * 8, y * 16, (x + 2) * 8 - 1, (y + 1) * 16 - 1);
   }

   /* Á· ¸‰¡œ··i total_number µA, Á· ¸‰¡‹q´‚·i total_price µA ¸á¸w */
   for(total_number = total_price = 0, i = 0; i < STOCK_NUM; i++)
   {
      total_number += stock[i].number;
      total_price  += (long) stock[i].price * stock[i].number;
   }

   /* Ìa·¡ À¡Ëa· ¸â¸éÐe ¶áÃ¡Ÿi ‰¬eÐe”a. */
   pie_y = (getmaxy() > 400) ? 230 : 200;
   xputs_han(14,               (pie_y - 120) / 16, "Á· ¸‰¡œ·");
   xputs_han(system_maxx - 23, (pie_y - 120) / 16, "Á· ¸‰¡‹q´‚");
   /* Á· ¸‰¡œ·µA ”Ðe Ìa·¡ À¡ËaŸi ‹aŸ¥”a. */
   for(start_angle = 0, i = 0; i < STOCK_NUM; i++)
   {
      ratio = stock[i].number / (float) total_number;
      if(i == STOCK_NUM - 1)  /* ¯©® µe¬eµA ·Ðe µ¡ÀaŸi ¥¡¸÷ */
         end_angle = 360;
      else
         end_angle = start_angle + ratio * 360;
      /* EGA/VGA µA¬á“e µaœáˆa»¡ ¬‚·a¡, HGC µA¬á“e µaœáˆa»¡ ÌÈå·a¡ */
      /* À¡Ëa· ¦Ÿi À¶…”a.                                        */
      if(in_vga_system)
         setfillstyle(SOLID_FILL, 9 + i);
      else
	 setfillstyle(i + 1, WHITE);
      /* ‰¬e–E ˆb”¡ À¡ËaŸi ‹aŸ¥”a. */
      pieslice(150, pie_y, start_angle, end_angle, 90);

      /* ˆb Ðw¡¢· §¡·I·i Îa¯¡Ðe”a. */
      if(start_angle >= 90 && start_angle <= 270)
         x = 150 + cos((start_angle + 20) / 180. * PI) * 90 - 50;
      else
         x = 150 + cos((start_angle + 20) / 180. * PI) * 90 + 10;
      if(start_angle >= 0 && start_angle <= 180)
	 y = pie_y - sin((start_angle + 20) / 180. * PI) * 90 - 13;
      else
	 y = pie_y - sin((start_angle + 20) / 180. * PI) * 90 + 5;
      sprintf(string, "%4.1f%%", ratio * 100);
      outtextxy(x, y, string);
      start_angle = end_angle;
   }

   /* Á· ¸‰¡‹q´‚µA ”Ðe À¡ËaŸi ‹aŸ¥”a. */
   for(start_angle = 0, i = 0; i < STOCK_NUM; i++)
   {
      ratio = ((long) stock[i].price * stock[i].number) / (float) total_price;
      if(i == STOCK_NUM - 1)  /* ¯©® µe¬eµA ·Ðe µ¡ÀaŸi ¥¡¸÷ */
         end_angle = 360;
      else
         end_angle = start_angle + ratio * 360;
      /* EGA/VGA µA¬á“e µaœáˆa»¡ ¬‚·a¡, HGC µA¬á“e µaœáˆa»¡ ÌÈå·a¡ */
      /* À¡Ëa· ¦Ÿi À¶…”a.                                        */
      if(in_vga_system)
         setfillstyle(SOLID_FILL, 9 + i);
      else
	 setfillstyle(i + 1, WHITE);
      /* ‰¬e–E ˆb”¡ À¡ËaŸi ‹aŸ¥”a. */
      pieslice(getmaxx() - 150, pie_y, start_angle, end_angle, 90);

      /* ˆb Ðw¡¢· §¡·I·i Îa¯¡Ðe”a. */
      if(start_angle >= 90 && start_angle <= 270)
         x = getmaxx() - 150 + cos((start_angle + 20) / 180. * PI) * 90 - 50;
      else
         x = getmaxx() - 150 + cos((start_angle + 20) / 180. * PI) * 90 + 10;
      if(start_angle >= 0 && start_angle <= 180)
	 y = pie_y - sin((start_angle + 20) / 180. * PI) * 90 - 13;
      else
	 y = pie_y - sin((start_angle + 20) / 180. * PI) * 90 + 5;
      sprintf(string, "%4.1f%%", ratio * 100);
      outtextxy(x, y, string);
      start_angle = end_angle;
   }
   wait(type_any_key);
}

void change_palette(void)
/* ÌiU(palette)·i ¤aŽ“e ¤w¤ó·i ¥¡µa º…”a. */
{
   struct palettetype cur_palette, palette;
   int left, top, key, quit, cur_pos, height, i;

   if(!in_vga_system)
   {
      puts_han("·¡ ¡A“A“e EGA/VGA µA¬á e ¬a¶wÐi ® ·¶¯s“¡”a.");
      wait(type_any_key);
      return;
   }

   adv_puts_han("Èá¥¡ C · setpalette() Ÿi ¬a¶wÐa¡e 64 ¬‚ º—µA¬á 16 ˆa»¡Ÿi "
                "¬åÈ‚Ði ® ·¶·a£a¡ ¹± ”á ”a´·Ðe ¬‚· ÑÁ¡e·i Š¬÷Ði ® ·¶¯s"
                "“¡”a. ”a·q µ“e <+>, <-> Ç¡Ÿi ’Ÿa¡e ÌiU·¡ ¤aŽá´á ¶¥Ða“e "
                "¬‚·i ¬åÈ‚Ði ® ·¶•¡¢ Ð º““¡”a. ¬a¶wÐi ® ·¶“e Ç¡“e ”a·q"
                "‰Á ˆ{¯s“¡”a.\n", 0);
   puts_han("<Up>, <Down>, <Left>, <Right> : ¶áÃ¡ ·¡•·");
   puts_han("<+>, <->                      : ÌiU ¬åÈ‚");
   puts_han("<Enter>                       : ÌiU ¥e‰w");
   puts_han("<Esc>                         : Âá­¡\n");
   puts_han("¤b¯a ¦µA Îa¯¡–A“e ®•¸a“e ÌiU¬‚· ¤åÑ¡·³“¡”a.");
   wait(type_any_key);
   clrscr_han();

   getpalette(&cur_palette);  /* Ñe¸ ÌiU·i cur_palette µA ¸á¸w */
   palette = cur_palette;
   height = system_maxy == 29 ? 7 : 5;
   for(i = 0; i < 16; i++)
   {
      left = (i % 4) * 20 + 2;
      top  = (i / 4) * height;
      draw_3dbox(left, top, left+15, top+height-2, i);
      output_offset_han = 8;
      vbprintf_han(left+7, top+height/2-1, (i+8)%16, i, "%2d", palette.colors[i]);
      output_offset_han = 0;
   }

   cur_pos = quit = 0;
   put_han_font((cur_pos%4)*20+9, (cur_pos/4+1)*height-1, SPC_ID, 7);
   while(!quit)
   {
      key = getch_han();
      if(key != 0)
      {
         switch(key)
         {
            case _ESC :
               setallpalette(&cur_palette);
            case _ENTER :
               quit = 1;
               break;
	    case '+' :
	       palette.colors[cur_pos] = (palette.colors[cur_pos] + 1) % 64;
	       setpalette(cur_pos, palette.colors[cur_pos]);
               break;
	    case '-' :
	       palette.colors[cur_pos] = (palette.colors[cur_pos] + 63) % 64;
	       setpalette(cur_pos, palette.colors[cur_pos]);
               break;
         }
         if(key == '+' || key == '-')
         {
            left = (cur_pos % 4) * 20 + 2;
	    top  = (cur_pos / 4) * height;
	    draw_3dbox(left, top, left+15, top+height-2, cur_pos);
	    output_offset_han = 8;
	    vbprintf_han(left+7, top+height/2-1, (cur_pos+8)%16, cur_pos,
			 "%2d", palette.colors[cur_pos]);
	    output_offset_han = 0;
         }
      }
      else
      {
         /* ­¥‹aŸ±·i »¡¶…”a. */
	 xputs_han((cur_pos%4)*20+9, (cur_pos/4+1)*height-1, "  ");
         switch(getch_han())
         {
	    case UP    : cur_pos = (cur_pos + 12) % 16; break;
	    case DOWN  : cur_pos = (cur_pos +  4) % 16; break;
	    case LEFT  : cur_pos = (cur_pos + 15) % 16; break;
	    case RIGHT : cur_pos = (cur_pos +  1) % 16; break;
	 }
         /* ¬¡¶… ¶áÃ¡µA ­¥‹aŸ±·i ‹aŸ¥”a. */
	 put_han_font((cur_pos%4)*20+9, (cur_pos/4+1)*height-1, SPC_ID, 7);
      }
   }

   wait("·¡ ¬‚·¡  a·qµA —s“¡Œa? ´a¢ Ç¡a ’Ÿa­A¶a.");
}

void show_3dbox(void)
/* ·³ÁA ¤b¯aŸi ‹aŸ¡“e ¤w¤ó·i ¥¡µa º…”a. */
{
   int left, top, height, i;

   adv_puts_han("‹aœÏ¢·a¡ ¬å·i ‹aŸ¡“e draw_vert_line(), draw_horiz_line() Ÿi "
	        "¬a¶wÐa¡e µ«e ·³ÁA ¤b¯aŸi ‹aŸ© ® ·¶¯s“¡”a. ·¡ Ïa¡‹aœ‘· ­¡¯a"
                "Ÿi Àq‰¡Ðaµa ¸a¯¥· ‹¡Ñ¡µA  x“e ·³ÁA ¤b¯aŸi  e—e ”a·q, hanio.lib"
                " a ¸a¯¥· œa·¡§aœáŸ¡µA —w¢¯¡Åa ‘½‰¡ ¬a¶wÐa¡e Ò©³¥ ¥¡‹¡ ¹½·e "
                "Ïa¡‹aœ‘·i  e—i ® ·¶¯s“¡”a.", 0);
   wait(type_any_key);
   clrscr_han();

   /* 16 ¬‚·i ¬a¶wÐaµa 16 ˆ· ·³ÁA ¤b¯aŸi ‹aŸ¥”a */
   height = system_maxy == 29 ? 7 : (system_maxy == 24 ? 6 : 5);
   for(i = 0; i < 16; i++)
   {
      left = (i % 4) * 20 + 2;
      top  = (i / 4) * height;
      draw_3dbox(left, top, left+15, top+height-2, i);
      output_offset_han = 8;
      vbprintf_han(left+7, top+height/2-1, (i+8)%16, i, "%2d", i);
      output_offset_han = 0;
   }
   wait(type_any_key);
}

void show_gr_save(void)
/* ‹aœÏ¢ ÑÁ¡e·i ¸á¸wÐa‰¡ ¥¢ŠÐa“e ¤w¤ó·i ¥¡µa º…”a. */
{
   unsigned size;
   void *screen;

   adv_puts_han("‹aœÏ¢ Ðq®µA ·Ð Š¬÷–E ÑÁ¡e·e ¯aÇaŸ¥¤áÌá· ¶w e ¸á¸wÐa“e "
	        "save_text_han() ·a¡“e ¸á¸wÐi ® ´ô»¡ e ·¡£¡»¡ ¸aÁAŸi ¸á¸wÐa“e"
	        " getimage() ¡“e ¸á¸wÐi ® ·¶¯s“¡”a. ”a·q µŸi ¥¡¡e ‹aœÏ¢ Ðq"
                "®µÁ ÑÂ” ¢…¸a¡ Š¬÷–E ÑÁ¡e·¡œa•¡ getimage() Ÿi ¬a¶wÐa¡e  a·q"
                "”¡ ¸á¸wÐa‰¡ ¥¢ŠÐi ® ·¶·q·i ´i ® ·¶¯s“¡”a.", 0);
   wait(type_any_key);
   clrscr_han();

   set_attr_han(FONT_INVERSE);
   vbputs_han(0, 0, "Ðe‹i œa·¡§aœáŸ¡ <Ðe> 4.1", LIGHTGRAY, RED);
   set_attr_han(FONT_VERT_DBL | FONT_HORIZ_DBL);
   vputs_han(0, 1, "Ðe‹i œa·¡§aœáŸ¡ <Ðe> 4.1", BLUE);
   draw_horiz_line(0, 50, 200, LIGHTMAGENTA);
   draw_vert_line(100, 20, 60, LIGHTGREEN);
   circle(100, 50, 30);

   set_attr_han(FONT_NORMAL);
   /* (0, 0) - (400, 80) µwµb·i ¸á¸wÐa“e•A Ï©¶aÐe ¡A¡¡Ÿ¡· ´··i size µA ¸á¸w */
   size = imagesize(0, 0, 400, 80);
   /* Ï©¶aÐe ¡A¡¡Ÿ¡Ÿi Ði”w¤h“e”a */
   if((screen = malloc(size)) == NULL)
   {
      wait("¡A¡¡Ÿ¡ˆa ¦¹¢Ðs“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
      return;
   }
   /* (0, 0) - (400, 80) µwµb·i screen µA ¸á¸wÐe”a */
   getimage(0, 0, 400, 80, screen);
   wait("´a¢ Ç¡a ’Ÿa¡e ÑÁ¡e·i »¡¶““¡”a.");
   clrscr_han();

   wait("´a¢ Ç¡a ’Ÿa¡e ¶¥œ ÑÁ¡e·a¡ ¥¢ŠÐs“¡”a.");
   putimage(0, 0, screen, COPY_PUT);

   wait("¶¥œ ÑÁ¡e·a¡ ¥¢Š–A´ö¯s“¡Œa? ´a¢ Ç¡a ’Ÿa­A¶a.");
   free(screen);
}

void show_play_music(void)
/* ·q´b·i µeºÐa“e ¤w¤ó·i ¥¡µa º…”a. */
{
   char music_data[] = "o3l4 g >g2g e2d c.d8c8.<b16 a2g >e2f8.e16 "
		       "d2c8.<b16 >c2.";

   adv_puts_han("play() Ÿi ¬a¶wÐa¡e BASIC · PLAY ¡ww‰Á ˆá· ˆ{·e ¤w¤ó·a¡ "
                "·q´b·i µeºÐi ® ·¶¯s“¡”a. µŸi —i´á '•¡'·q·i 8 ¦…·qÎa¡ µe"
                "ºÐa“e ˆõ·e ”a·q‰Á ˆ{¯s“¡”a.\n", 0);
   puts_han("<Example>");
   puts_han("play(\"c8\");\n");
   adv_puts_han("·¡¹A ·¡ Ðq®Ÿi ¬a¶wÐaµa ´á˜å ‘¡œ· ¡I¡—¡Ÿi µeºÐ ¥¡‰V¯s“¡"
                "”a. ¢¯e ‘¡œ· ¡I¡—¡·¥»¡  xÂ´á ¥¡­A¶a.\n", 0);
   wait(type_any_key);
   /* "mn" ·a¡ µeº */
   play("mn");
   play(music_data);

   adv_puts_han("play() · ¡wwº— m ¡ww·i ¬a¶wÐa¡e ”aŸe ¤w¤ó·a¡•¡ µeºÐi ® "
                "·¶¯s“¡”a.  å¸á ˆb ·q·i ¦›µa¬á µeºÐa“e ¤w¤ó·³“¡”a.\n", 0);
   wait(type_any_key);
   /* "ml" ¡ µeº */
   play("ml");
   play(music_data);

   /* "mi" ¡ µeº */
   puts_han("ËaA¡©¡ ‹¡¤ó·a¡ µeºÐa“e ¤w¤ó·³“¡”a.\n");
   wait(type_any_key);
   play("mi");
   play(music_data);

   /* "mf" ¡ µeº */
   puts_han(" a»¡ b·a¡ ´aº ¦—aœó‰A µeºÐa“e ¤w¤ó·³“¡”a.\n");
   wait(type_any_key);
   play("mf");
   play(music_data);

   wait("·q´b·¡ aµ¡“¡ Ðe‰i Ów£¡·¶ºa? ´a¢ Ç¡a ’Ÿa­A¶a.");
}

#define MENU_NUMBER 	7	/* ¡A“A· ˆ•® */

void show_popup_menu(void)
/* Ìs´ó ¡A“AŸi  e—a“e ¤w¤ó·i ¥¡µa º…”a. */
{

   char *string[MENU_NUMBER] = { " ¬ ‹i       @N ",
				 " ¦‰œáµ¡‹¡    @O ",
				 " ¸á¸wÐa‹¡    @S ",
				 " ¬·¡Ÿq·a¡  @V ",
				 " ‰w¡¤aŽ‹¡     ",
				 " •¡¯a¡ww´á @F0 ",
				 " {          @X "
                               };
   int  current;		/* ¬åÈ‚–E ¡A“A· ¤åÑ¡ */
   char key;			/*  a»¡ bµA ’‰œá»¥ Ç¡ */

   adv_puts_han("popup_han() ·i ¬a¶wÐa¡e ˆe”eÐa‰A Ìs´ó ¡A“AŸi  e—i ® ·¶¯s"
                "“¡”a. ”a·q µ“e ´aœ´a Ðe‹i· ¬áŸAÀé ¡A“AŸi popup_han() ·a"
                "¡  e—a“e ˆõ·³“¡”a.", 0);
   wait(type_any_key);
   clrscr_han();

   current = 0;
   key = popup_han(33, 8, string, MENU_NUMBER, &current);
   clrscr_han();
   switch(key)
   {
      case ESC   : puts_han("<Esc> Ç¡Ÿi ’Ÿa­v¯s“¡”a.");
		   break;
      case LEFT  : puts_han("<Left> Ç¡Ÿi ’Ÿa­v¯s“¡”a.");
		   break;
      case RIGHT : puts_han("<Right> Ç¡Ÿi ’Ÿa­v¯s“¡”a.");
		   break;
      case ENTER : printf_han("<%s> ¡A“AŸi ¬åÈ‚Ða­v¯s“¡”a.\n", string[current]);
		   break;
   }

   wait(type_any_key);
}

#undef MENU_NUMBER

void show_pulldown_menu(void)
/* Î‰”a¶… ¡A“AŸi  e—a“e ¤w¤ó·i ¥¡µa º…”a. */
{
   /* pulldow_menu_struct “e Ça‹¡ˆa 4024 ¤a·¡Ëaa –A“e ”÷Ã¡ Çe ¸ažaÑw·¡‹¡ */
   /* ˜¢…µA menu_data Ÿi ‹a· ¸a•·¥e®¡ ¬å´åÐa¡e ¯aÈ‚µwµb· ¦¹¢·a¡ ·¥ */
   /* Ðaµa ¯©Ð—¯¡ Ã¡¡w¸â·¥ µAœáˆa ¤i¬—Ði ¶áÐñ·¡ ·¶”a. ”a·q‰Á ˆ{·¡  ¸÷¸â¥e */
   /* ®¡ ¬å´åÐaµa ¸÷¸â¥e®¡ ¬å´åÐa“e ˆõ·¡ ´e¸åÐa”a.                    */
   static pulldown_menu_struct menu_data =
   {
      10, 1,    /* º¡A“A· ˆ•®“e 10 ˆ·¡‰¡ ÂAÁ¡µA“e 1 ¤å ¡A“Aˆa ¬åÈ‚–E”a */
      { 1, 7, 15, 21, 27, 33, 39, 45, 51, 61 },   /* º¡A“A 10 ˆ· x ¹ÁÎa */
      { " <Ðe> ", " ¬áŸAÀé ", " Íe»³ ", " ·³b ", " Àx‹¡ ",
        " ¹¡Ìe ", " Â‰b ", " ÑÁ¡e ", " Ëb®‹¡“w ", " ¬åÈ‚¬aÐw ",
      },
      /* ·¡ ¦¦…µA¬á¦Èá ¦¡A“A 10 ˆˆa ¸÷·–E”a */
      { /* 0 ¤å ¡A“A(<Ðe>) */
        { 6, 0, 1, 1,                /* (1, 1) µA 6 ˆ ¼aŸ¡ ¦¡A“AŸi Š¬÷ */
          { " <Ðe> ·¡œe    ",
            " •¡¶‘ i    F1 ",
            " ¸aÌe¤µi @F1 ",
            " ”ib         ",
            " ‰¬e‹¡       ",
            " ¸åÑÁ¤åÑ¡¦   ",
          }
        },
        /* 1 ¤å ¡A“A(¬áŸAÀé) */
        { 7, 0, 7, 1,                /* (7, 1) µA 7 ˆ ¼aŸ¡ ¦¡A“AŸi Š¬÷ */
          { "¬ ‹i       @N",
            "¦‰œáµ¡‹¡    @O",
            "¸á¸wÐa‹¡    @S",
            "¬·¡Ÿq·a¡  @V",
            "‰w¡¤aŽ‹¡    ",
            "•¡¯a¡ww´á @F0",
            "{          @X",
          }
        },
        /* 2 ¤å ¡A“A(Íe»³) */
        { 7, 0, 15, 1,                /* (15, 1) µA 7 ˆ ¼aŸ¡ ¦¡A“AŸi Š¬÷ */
          { " ¶¥œ”¡   ^Z ",
            " µ¡a–‹¡   ^X ",
            " ¥¢¬a       ^C ",
            " ¦›·¡‹¡     ^V ",
            " »¡¶‹¡     ^E ",
            " §iœâ ·ª‹¡ ^KR ",
            " §iœâ ³a‹¡ ^KW ",
          }
        },
        /* 3 ¤å ¡A“A(·³b) */
        { 7, 0, 21, 1,                /* (21, 1) µA 7 ˆ ¼aŸ¡ ¦¡A“AŸi Š¬÷ */
          { " ¢…¸a      @F2 ",
            " ‹i¸a¡¡´·   @L ",
            " ¢…”e¡¡´·   @T ",
            " ¬å‹aŸ¡‹¡   @D ",
            " ¬w¶wŠ¯©Ð— @I ",
            "  Ça¡¸÷· @B ",
            " µaœá¤å¯©Ð— @Z ",
          }
        },
        /* 4 ¤å ¡A“A(Àx‹¡) */
        { 6, 0, 27, 1,                /* (27, 1) µA 6 ˆ ¼aŸ¡ ¦¡A“AŸi Š¬÷ */
          { " Àx‹¡     ^QF ",
            " ¤aŽ‹¡   ^GA ",
            " Ð—·a¡   ^QG ",
            " ÍA·¡»¡    @G ",
            " ¹¡Ìe¢…¸a  ^G ",
            " ¢…¸a¹·ŸA ^QS ",
          }
        },
        /* 5 ¤å ¡A“A(¹¡Ìe) */
        { 7, 0, 33, 1,                /* (33, 1) µA 7 ˆ ¼aŸ¡ ¦¡A“AŸi Š¬÷ */
          { "  áŸ¡ i      ^NH ",
            " ¡Ÿ¡ i      ^NF ",
            " ˆbº        ^NN ",
            " ‹aŸ±        ^NI ",
            " Ëb® ¢…¸a   ^NS ",
            " ÍA·¡»¡ ¸÷•¥ ^NP ",
            " ÍA·¡»¡ a’‘  ^J ",
          }
        },
        /* 6 ¤å ¡A“A(Â‰b) */
        { 2, 0, 39, 1,                /* (39, 1) µA 2 ˆ ¼aŸ¡ ¦¡A“AŸi Š¬÷ */
          { " ·¥­á        @P ",
            " ÏaŸ¥Èá ¬é¸÷ ^P ",
          }
        },
        /* 7 ¤å ¡A“A(ÑÁ¡e) */
        { 7, 0, 45, 1,                /* (45, 1) µA 7 ˆ ¼aŸ¡ ¦¡A“AŸi Š¬÷ */
          { " ®»¢a’‹¡ ^WV ",
            " ®Íwa’‹¡ ^WH ",
            " ÑÁ¡e”h‹¡   ^WC ",
            " ”aŸeÑÁ¡e    @W ",
            " ¡¡´·¤aŽ‹¡ ^WX ",
            " Ça‹¡¹¡¸é   ^WS ",
            " ÑÁ¡eÑÂ”   ^WZ ",
          }
        },
        /* 8 ¤å ¡A“A(Ëb®‹¡“w) */
        { 3, 0, 51, 1,                /* (51, 1) µA 3 ˆ ¼aŸ¡ ¦¡A“AŸi Š¬÷ */
          { " ¡¢Àa e—i‹¡ ",
            " ¡A·©  á»¡  ",
            " ­¡ Ëa      ",
          }
        },
        /* 9 ¤å ¡A“A(¬åÈ‚¬aÐw) */
        { 8, 0, 61, 1,                /* (61, 1) µA 8 ˆ ¼aŸ¡ ¦¡A“AŸi Š¬÷ */
          { " Îa¯¢ ¥¡·¡‹¡   ",
            " ’…‹q ¥¡·¡‹¡   ",
            "  \x14  ¥¡·¡‹¡    ",
            " ¸á¸w ˆe‰b  30 ",
            " È“¬é¸÷        ",
            " ¤‰w¬‚        ",
            " ¯¡  ‰        ",
            " ¸÷•¥ ¤e¸a•·   ",
          }
        },
      }
   };
   char key;					/*  a»¡ bµA ’‰œá»¥ Ç¡ */
   int  main_menu, sub_menu;                    /* ¬åÈ‚–E º¡A“AµÁ ¦¡A“A */
   int  quit;

   adv_puts_han("pulldown_han() ·i ¬a¶wÐa¡e ˆe”eÐa‰A Î‰”a¶… ¡A“AŸi  e—i ® "
                "·¶¯s“¡”a. ”a·q µ“e ´aœ´a Ðe‹i· º¡A“AŸi pulldown_han() "
                "·a¡  e—a“e ˆõ·³“¡”a. <Esc> Ç¡Ÿi ’Ÿaˆáa \"{ @X\" ¡A“AŸi"
                " ¬åÈ‚Ða¡e {‰ ® ·¶¯s“¡”a.", 0);
   wait(type_any_key);
   clrscr_han();

   quit = 0;
   while(!quit)
   {
      /* ·¡ ¦¦…µA¬á Î‰”a¶… ¡A“Aˆa Š¬÷–E”a */
      key = pulldown_han(&menu_data);

      if(key == ESC) break; 	/* <Esc> Ç¡Ÿi ’‰œá•¡ {·¡ e”a */
      /* ¬åÈ‚–E º¡A“A“e main_menu µA, ¦¡A“A“e sub_menu µA ¸á¸wÐe”a */
      main_menu = menu_data.current;
      sub_menu  = menu_data.sub_menu[main_menu].current;

      /* ·¡¦¦…µA¬á "{" ·¡¶A· ¡A“A•¡ ÐqA ÀáŸ¡Ð º¡e –E”a. */
      switch(main_menu)
      {
         case 1 : switch(sub_menu)
                  {
                     /* º¡A“A“e 1, ¦¡A“A“e 0 ("{" ¡A“A)·¡¡e {·i …”a */
		     case 6 : quit = 1; break;

		  }
		  break;
      /* case 2 : switch(sub_menu)
           :
      */
      }
      nprintf_han(20, 20, 40, "<%s> ¡A“AŸi ¬åÈ‚Ða­v¯s“¡”a",
                  menu_data.sub_menu[main_menu].string[sub_menu]);
   }
}
