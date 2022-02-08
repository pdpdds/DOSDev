/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          <HAN> demo program - main program                   */
/*   FILENAME           handemo.c                                           */
/*   DATE & TIME        07/08/92 (WED) 03:30                                */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include <graphics.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dos.h>
#include <string.h>
#include <math.h>
#include <conio.h>
#include <ctype.h>
#include <alloc.h>

#include "hanio.h"
#include "extendio.h"
#include "misc.h"
#include "keydef.h"

/****************************************************************************/
/*                               Data Types                                 */
/****************************************************************************/

typedef unsigned char mini;          /* unsigned char Ñw·i mini Ñw·a¡ ¸÷· */

/****************************************************************************/
/*                            External Variables                            */
/****************************************************************************/

/* ÑÁ¡e· Ça‹¡Ÿi ¸á¸wÐa‹¡ ¶áÐe ¶A¦¥e®                   */
/* maxx, maxy, minx, miny   = ¶å•¡¶· µwÐ··i ¤h“e”a.     */
/* system_maxx, system_maxy =   "        "    ¤h»¡´g“e”a. */
int maxx, maxy, minx, miny, system_maxx, system_maxy;
/* ‹¡¥¥ ¸å‰w¬‚ £¹ ¤‰w¬‚ --> ÑÁ¡e· ¬‚·i ¤aŽ‰¡ ¯¼·a¡e */
/* palette Ÿi ¤aŽ—e»¡ ·¡ ¥e®· ˆt·i ¤aŽ¡e –E”a.     */
int base_color = YELLOW, base_bkcolor = LIGHTBLUE;
/* ¸÷¸â µwµb·i ¸é´¢Ða‹¡ ¶áÐ ¸aº ³a·¡“e ¢…¸aµi·e ¥e®µA ¸á¸wÐe”a. */
char *type_any_key = "´a¢ Ç¡a ’Ÿa­A¶a.";

/****************************************************************************/
/*                   Prototypes of the External Functions                   */
/****************************************************************************/

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

/* ”a·q Ðq®—i·e demosub.c µA ¸÷·–A´á ·¶”a. */
void show_graph_and_han(void);  /* Ðe‹i‰Á ‹aœÏ¢ */
void show_pie_chart(void);      /* Ìa·¡ À¡Ëa */
void show_gr_save(void);        /* ‹aœÏ¢ ÑÁ¡e· ¸á¸w */
void change_palette(void);      /* ÌiAËa ¤aŽ‹¡ */
void show_3dbox(void);          /* ·³ÁA ¤b¯a */
void show_play_music(void);     /* ·q´b µeº */
void show_popup_menu(void);     /* Ìs´ó ¡A“A */
void show_pulldown_menu(void);  /* Î‰”a¶… ¡A“A */
void to_dos(void);              /* •¡¯a®É·i Ðe”a. */

void draw_logo(void);           /* ¡‰¡Ÿi ‹aŸ¥”a. */
void do_work(void);             /* Ðw¡¢·i ¬åÈ‚¤h·e Ò ¯©Ð—Ðe”a. */
void ask_quit(void);            /* Ïa¡‹aœ‘·i {‰»¡ ¢ˆ“e”a. */
void draw_dummy_screen(void);   /* ¯¡Ðñ¶w ÑÁ¡e·i Š¬÷Ðe”a. */
void wait(char *msg);           /* ¡A¯¡»¡Ÿi Œq¨b·¡¡e¬á Ç¡·³b·i ‹¡”aŸ¥”a. */
void draw_fourier(int term_no, int peek); /* Ìé¯aŸi ¬a·¥· Ðs·a¡ ‹aŸ¥”a. */
char *color_to_str(int color);  /* color ¤å ¬‚· ·¡Ÿq·i Ÿ¡ÈåÐe”a. */
void show_help(void);           /* •¡¶‘ i·i ¥¡µa º…”a. */
int  error_handler(int errval, int ax, int bp, int si); /* µAœá Ð…—iœá */
int  ctrl_handler(void);	/* control-break Ð…—iœá */
/* ·³ÁA ¤b¯a· ÉA–Ÿ¡Ÿi ‹aŸ¥”a. */
void draw_3dborder(int left, int top, int right, int bottom, int is_relief);
/* ·³ÁA ¤b¯aŸi ‹aŸ¥”a. */
void draw_3dbox(int left, int top, int right, int bottom, int color);

/****************************************************************************/
/*                    Implementation of main() function                     */
/****************************************************************************/

void main(void)
{
   int error_code;

   puts("Hangul Library <HAN> 4.1 by Lee Hyun-Ho...");

   error_code = init_han();     /* Ðe‹i ÑÅ‰w·a¡ Á¡‹¡ÑÁ */
   if(error_code != OK_HAN)     /* Á¡‹¡ÑÁ¯¡ µAœáˆa ¤i¬—Ð–”a¡e */
   {
      switch(error_code)        /* µAœáÅ¡—aµA ˜aœa ¡A¯¡»¡ Â‰bÒ exit */
      {
	 case MEMORY_NOT_ENOUGH   : puts("Memory is not enough."); 			   break;
	 case HAN_FONT_NOT_FOUND  : puts("Hangul font file is not found."); 	     	   break;
	 case ENG_FONT_NOT_FOUND  : puts("English font file is not found."); 		   break;
	 case SPC_FONT_NOT_FOUND  : puts("Special font file is not found.");              break;
	 case HAN_FONT_READ_ERROR : puts("Error occurred in reading Hangul font file.");  break;
	 case ENG_FONT_READ_ERROR : puts("Error occurred in reading English font file."); break;
	 case SPC_FONT_READ_ERROR : puts("Error occurred in reading Special font file."); break;
	 case INVALID_GR_CARD     : puts("Only supports HGC & standard EGA/VGA.");        break;
	 case INITGRAPH_ERROR     : puts("Can't initialize the graphics system.");        break;
	 default                  : puts("Undefined error.");                             break;
      }
      exit(1);
   }

   /* Ða—a¶Á´á µAœá Ð…—iœáµÁ control-break Ð…—iœáŸi ¬é¸÷Ðe”a. */
   harderr(error_handler);
   ctrlbrk(ctrl_handler);
   /* maxx, maxy, minx, miny, system_maxx, system_maxy Ÿi Àa¡ ¬é¸÷ */
   minx = get_minx_han();
   miny = get_miny_han();
   maxx = get_maxx_han();
   maxy = get_maxy_han();
   system_maxx = get_abs_maxx_han();
   system_maxy = get_abs_maxy_han();

   /**************************    º  ·    **************************/
   /* <Ðe> · Ðe/µw ¸åÑÅÇ¡“e <Left Shift>+<Space> ¡ ¸÷–A´á ·¶·aa */
   /* ”a·q‰Á ˆ{·¡ set_han_change_key() Ÿi Ñ¡Â‰Ða¡e <F2> Ç¡¡ ¤aŽ‰ ® */
   /* ·¶”a. µa‹¡¬á“e ‹a· <Left Shift>+<Space> Ÿi ¬a¶wÐe”a.          */
   /* set_han_change_key(F2, 1, 0);                                  */
   /* F2 : ¥eÑÅµA ¬a¶w–A“e Ç¡· Å¡—aˆa keydef.h µA ¸÷·–E F2 œa“e ›u */
   /* 1  : F2 ˆa ´a¯aÇ¡ Å¡—aˆa ´a“¡‰¡ ÑÂ¸wÇ¡ Å¡—aœa“e ›u             */
   /* 0  : F2 Ç¡µÁ ¹¡Ðs–A“e Ç¡¥¡—a Ïiœ‹aˆa ´ô”a“e ›u                */
   /******************************************************************/

   status_on_han();                           /* ¬wÈ œa·¥·i aÈaa‰A Ðe”a. */
   draw_logo();                               /* ¡‰¡ ÑÁ¡e·i ¥¡µaº…”a. */

   /* Ðw¡¢·i ¬åÈ‚¤h´a¬á ¸â¸éÐe Ðq®Ÿi Ñ¡Â‰Ðe”a. Ïa¡‹aœ‘· */
   /* ¹·ža“e do_work() ¦µA¬á ·©´áe”a.                  */
   do_work();
}

/****************************************************************************/
/*                    Implementation of the Subfunctions                    */
/****************************************************************************/

void introduce(void)
/* <Ðe> ·i ­¡ˆÐa“e ‹i·i Â‰bÐe”a */
{
   /* ¶å•¡¶Ÿi ¬é¸÷Ðe Ò ­¡ˆ‹i·i ¥¡µa º…”a. */
   set_window_han(5, 0, system_maxx-5, system_maxy-1);

   set_vbcolor_han(RED, LIGHTCYAN);
   /* ¢…¸w¸÷i Â‰bÐq®· ¬a¶wµŸi ¥© ® ·¶”a. indenet = 0 ·¡£a¡ */
   /* —iµa ³a‹¡Ÿi ¸åÑa Ða»¡ ´g“e”a.                               */
   adv_puts_han("·¡ Ïa¡‹aœ‘·e Ðe‹i œa·¡§aœáŸ¡ <Ðe> 4.1 · µaœáˆa»¡ ‹¡“w—i·i "
		"¥¡µa º‹¡ ¶áÐ  e—i´á»¥ •A¡¡¶w Ïa¡‹aœ‘·³“¡”a.\n", 0);
   set_vbcolor_han(base_color, base_bkcolor);
   adv_puts_han("·¡ Ïa¡‹aœ‘µA¬á ¬a¶wÐe œa·¡§aœáŸ¡“e Ða·¡ÉI(HiTEL) · ¸aža¯©"
		"(GLDOWN)µA hanio.lzh œa“e ÑÁ·©¡ µ©a¹a ·¶¯s“¡”a. ·¡ œa·¡"
                "§aœáŸ¡ˆa Ï©¶aÐa¯¥ ¦…·e ¸aža¯©·i ·¡¶wÐa¯¡‹¡ ¤aœs“¡”a.\n", 0);

   set_attr_han(FONT_INVERSE);
   set_vbcolor_han(LIGHTCYAN, RED);
   puts_han("Ðe‹i/µw¢… ¥eÑÅ·e <Left Shift>+<Space> Ç¡¡ Ða­A¶a.");
   set_vbcolor_han(base_color, base_bkcolor);
   set_attr_han(FONT_NORMAL);
   set_window_han(0, 0, system_maxx, system_maxy-1);

   wait("´a¢ Ç¡a ’Ÿa¡e <Ðe> 4.1 µA ”Ðe •¡¶‘ i·i ¥¡¯© ® ·¶¯s“¡”a.");
   show_help();
}

void to_dos(void)
/* •¡¯a®É·i Ðe”a. */
{
   int graph_mode, ret_code;

   graph_mode = getgraphmode(); /* Ñe¸· ‹aœÏ¢ ¡¡—aŸi ¸á¸wÐe”a. */
   restorecrtmode();            /* ÉB¯aËa ¡¡—a¡ •©a ‘½“e”a. */
   puts("Type EXIT to return to <HAN> demo program..");
   ret_code = system("");       /* •¡¯a®É·i Ðe”a. */
   setgraphmode(graph_mode);    /* ¶¥œ· ‹aœÏ¢ ¡¡—a¡ •©a  ‘½“e”a. */
   if(ret_code == -1) wait("•¡¯a¡ aˆi ®ˆa ´ô¯s“¡”a.");
   /* ÑÁ¡eµA¬á ¬aœa»¥ ¬wÈ œa·¥·i ¬¡ ‹aŸ¥”a. */
   draw_status_han();
}

void show_color(void)
/* 16 ¬‚·a¡ ‹i¸aŸi Â‰bÐa“e ¡¡¯s·i ¥¡µa º…”a. */
{
   int i, j;

   adv_puts_han("set_color_han() ·i ¬a¶wÐa¡e Â‰b–A“e ‹i¸a· ¸å‰w¬‚·i ¤aŽ‰ "
                "® ·¶‰¡ set_bkcolor_han() ·i ¬a¶wÐa¡e ¤‰w¬‚·i ¤aŽ‰ ® ·¶¯s"
                "“¡”a.\n", 0);
   puts_han("<Example>");
   puts_han("set_color_han(RED);");
   puts_han("set_bkcolor_han(YELLOW);\n");
   wait("´a¢ Ç¡a ’Ÿa¡e ¬‚Œi·i ¤aŽ““¡”a.");

   set_color_han(RED);
   set_bkcolor_han(YELLOW);
   puts_han("‘¡œe ¤aÈwµA ¨iˆe ‹i³¡·³“¡”a.\n");
   set_vbcolor_han(base_color, base_bkcolor);

   adv_puts_han("·¡¤åµA“e <Ðe> µA¬á ¬a¶w ˆa“wÐe ¡¡—e ‹i¸a¬‚·i ¥¡‰V¯s“¡”a. "
                "¸å‰w¬‚ 16 ˆa»¡µA ¤‰w¬‚ 16 ˆa»¡Ÿi ¬a¶wÐi ® ·¶·a£a¡ ¡¡– "
                "256 ˆa»¡· ¬‚(256 ÄéœáµÁ“e ”aŸe ·£¡)·a¡ ‹i¸aŸi Â‰bÐi ® "
                "·¶¯s“¡”a.", 0);
   wait(type_any_key);

   clrscr_han();
   vbputs_han(4, 0, "¸å‰w¬‚", RED, LIGHTCYAN);
   vbputs_han(4, 1, "¤‰w¬‚", RED, LIGHTCYAN);
   for(i = 0; i < 16; i++)
   {
      vbprintf_han(10+i*4, 0, RED, LIGHTCYAN, " %02d ", i);
      vbputs_han(  10+i*4, 1, "    ", RED, LIGHTCYAN);
   }
   for(i = 0; i < 16; i++) vbprintf_han(4, i+2, RED, LIGHTCYAN, " %02d   ", i);
   for(i = 0; i < 16; i++) for(j = 0; j < 16; j++) vbputs_han(10+i*4, j+2, "Ðe‹i", j, i);
   if(system_maxy > 23)
   /* ÑÁ¡e· ‹©·¡ˆa Â—¦…Ó¡ ‹© ˜ e Îa¯¡ */
   {
      center_puts_han(19, "¬‚ ¤åÑ¡µA Ð”wÐa“e  Ça¡¬w®", LIGHTCYAN, FONT_NORMAL);
      xputs_han(12, 20, "00 = BLACK     01 = BLUE          02 = GREEN       03 = CYAN     ");
      xputs_han(12, 21, "04 = RED       05 = MAGENTA       06 = BROWN       07 = LIGHTGRAY");
      xputs_han(12, 22, "08 = DARKGRAY  09 = LIGHTBLUE     10 = LIGHTGREEN  11 = LIGHTCYAN");
      xputs_han(12, 23, "12 = LIGHTRED  13 = LIGHTMAGENTA  14 = YELLOW      15 = WHITE    ");
   }

   wait(type_any_key);
}

void change_color(void)
/* ‹¡¥¥ ¸å‰w¬‚‰Á ‹¡¥¥ ¤‰w¬‚·i ¤aŽ…”a. */
{
   int color, bkcolor, color2, bkcolor2, ch;

   /* Ñe¸¬‚·i ¥¡‰ÅÐe”a */
   color   = color2   = get_color_han();
   bkcolor = bkcolor2 = get_bkcolor_han();

   set_vbcolor_han(RED, LIGHTGRAY);
   xputs_han(40 - 7, 8, " ¬‚Œi ¤aŽ‹¡ ");
   draw_box_han(25, 10, 54, 14, 0);
   nvputs_han(25, 16, " ¸å‰w¬‚ =", 30, RED);
   nvputs_han(25, 17, " ¤‰w¬‚ =", 30, RED);
   center_puts_han(abs_maxy_han-3, " ¸å‰w¬‚:<Up>,<Down>  ¤‰w¬‚:<Left>,<Right>"
                   "  ¬é¸÷:<Enter>  Âá­¡:<Esc> ", RED, FONT_NORMAL);
   while(1)
   {
      set_vbcolor_han(color, bkcolor);
      nputs_han(26, 11, "", 28);
      xputs_han(26, 12, "  Ðe‹i œa·¡§aœáŸ¡ <Ðe> 4.1  ");
      nputs_han(26, 13, "", 28);
      set_bkcolor_han(LIGHTGRAY);
      vprintf_han(35, 16, color,   "%11s %2d ", color_to_str(color),   color);
      vprintf_han(35, 17, bkcolor, "%11s %2d ", color_to_str(bkcolor), bkcolor);
      set_bkcolor_han(bkcolor);

      while(1)
      {
	 ch = getch_han();
	 if(ch == 0)
	 {
	    if((ch = getch_han()) == UP || ch == DOWN || ch == LEFT || ch == RIGHT)
	       break;
	    else
	       continue;
	 }
	 else
	 {
	    if(ch == _ESC)
	    {
	       set_vbcolor_han(color2, bkcolor2); /* ¶¥œ¬‚·a¡ ¥¢ŠÐe”a */
	       return;
	    }
	    else if(ch == _ENTER)
	    {
	       base_color   = color;
	       base_bkcolor = bkcolor;
	       return;
	    }
	    else
	       continue;
	 }

      }

      switch(ch)
      {
         case UP    : color = (16 + color - 1) % 16;
                      break;
         case DOWN  : color = (color + 1) % 16;
		      break;
         case LEFT  : bkcolor = (16 + bkcolor - 1) % 16;
                      break;
         case RIGHT : bkcolor = (bkcolor + 1) % 16;
                      break;
      }
   }
}

void show_save_text(void)
/* ÑÁ¡e·i ¸á¸wÐa‰¡ ¥¢ŠÐa“e ¤w¤ó·i ¥¡µa º…”a. */
{
   char *screen, string[MAX_STRLEN+1];
   /* gets_han() µA¬á ·³bˆa“wÐe ¢…¸aµi· ®“e ÂA” MAX_STRLEN ¸a·¡”a */
   /* ˜aœa¬á gets_han() ·i ¬a¶wÐi ˜“e Ðw¬w MAX_STRLEN ¥¡”a Ça‰A ¢…¸a */
   /* µi·i ¸s´a–´á´¡ Ã¡¡w¸â·¥ µAœáˆa ¬—‹¡»¡ ´g“e”a!!!!!!!!!!!!!      */
   /* ·¡ ®“e set_max_strlen_han() ·a¡ ¤aŽ‰ ® ·¶”a.                 */

   adv_puts_han("ÑÁ¡e·i ¸á¸wÐa‰¡ ¥¢ŠÐa“e save_text_han() ‰Á restore_"
                "text_han() µA ”Ð¬á ¥¡‰V¯s“¡”a.\n", 0);
   puts_han("Example>");
   puts_han("screen = save_text_han(left, top, right, bottom);");
   puts_han("   :");
   puts_han("restore_text_han(screen, left, top);\n");
   adv_puts_han("ÑÁ¡e·¡ ¹A”¡ ¸á¸w–A“e»¡ ¥¡‹¡ ¸åµA, ¶¬å ‹i¸aŸi ·³bÐaµa "
                "ÑÁ¡e·i Š¬÷Ðs¯¡”a. ‹a e ·³bÐa‰¡ ¯¼·a¡e ‹a· <Enter> Ç¡Ÿi "
                "’Ÿa¯¡¡e –S“¡”a.", 0);

   do
   {
      if(gets_han(string) == _ESC) break;   /* <Esc> Ç¡Ÿi ’Ÿa¡e ¨a¹a aµ¥”a */
      puts_han(string);
   }
   while(strlen(string));              /* ‹a· <Enter> Ÿi ’‰œá•¡ ¨a¹a aµ¥”a */

   screen = save_text_han(0, 0, maxx, maxy);
   wait("ÑÁ¡e·i ¸á¸wÐ–¯s“¡”a. ´a¢ Ç¡a ’Ÿa¡e ÑÁ¡e·i »¡¶““¡”a.");
   clrscr_han();
   adv_puts_han("ÑÁ¡e·i »¡¶¶¯s“¡”a. ·¡¤åµA“e restore_text_han() ·i ¬a¶wÐaµa "
                "»¡¶‹¡ ¸å· ÑÁ¡e·i ¥¢ŠÐ ¥¡‰V¯s“¡”a.", 0);
   wait(type_any_key);
   restore_text_han(0, 0, screen);
   wait("¹A”¡ ¥¢Šˆa –A´ö¯s“¡Œa?");
}

void show_modify_text(void)
/* ÑÁ¡e· ·©¦¦…· ­¢¬÷‰Á ¬‚·i ¤aŽ“e ¤w¤ó·i ¥¡µa º…”a. */
{
   int attr, color;

   adv_puts_han("·¡£¡ Â‰b–A´á ·¶“e ÑÁ¡e·e modify_text_attr(), modify_text_"
                "color() ¡ ­¢¬÷‰Á ¬‚·i ¤aŽ‰ ® ·¶¯s“¡”a.  å¸á modify_text_"
                "attr() ·i ¯¡ÐñÐ ¥¡‰V¯s“¡”a. ÑÁ¡e· ­¢¬÷·¡ ‰­¢ ¤aŽé ˆõ·³"
                "“¡”a. º—»¡Ðaa¡e ´a¢ Ç¡a ’Ÿa­A¶a.\n", 0);
   puts_han("<Example 1>");
   puts_han("modify_text_attr(2, 2, 48, 7, FONT_NORMAL);\n");
   wait("´a¢ Ç¡a ’Ÿa¡e ¯¡¸bÐs“¡”a.");

   draw_dummy_screen();
   attr = 0;
   clear_key_buff();
   while(!kbhit())
   {
      modify_text_attr(2, 2, 48, 7, attr);
      if(attr >= FONT_VERT_DBL - 1) attr = 0; else attr++;
      /* attr ·¡ FONT_VERT_DBL ¥¡”a Çaˆáa ˆ{·a¡e ÑÂ”¢…¸aˆa Â‰b–E”a. */
      /* ·¡Ÿi  b‹¡ ¶áÐ ·¡µÁ ˆ{·e ¹¡Ã¡Ÿi ÂáÐa“e ˆõ·¡”a                 */
   }

   puts_han("\n­¢¬÷·¡ ¹A”¡ ¥eÑÁÐ–¯s“¡Œa?\n");
   wait(type_any_key);

   adv_puts_han("·¡¤åµA“e modify_text_color() Ÿi ¯¡ÐñÐ ¥¡‰V¯s“¡”a. ÑÁ¡e· "
                "¬‚·¡ ‰­¢ ¤aŽé ˆõ·³“¡”a. º—»¡Ðaa¡e ´a¢ Ç¡a ’Ÿa­A¶a.\n", 0);
   puts_han("<Example 2>");
   puts_han("modify_text_color(2, 2, 48, 7, RED);\n");
   wait("´a¢ Ç¡a ’Ÿa¡e ¯¡¸bÐs“¡”a.");

   draw_dummy_screen();
   color = 0;
   clear_key_buff();
   while(!kbhit())
   {
      modify_text_color(2, 2, 48, 7, color);
      color = (color + 1) % 16;
   }

   puts_han("\n¬‚·¡ ¹A”¡ ¥eÑÁÐ–¯s“¡Œa?");
   wait(type_any_key);
}

void show_scroll_updn(void)
/* ÑÁ¡e· ·©¦¦…·i ¯aÇa©¯¡Ç¡“e ¤w¤ó·i ¥¡µa º…”a. */
{
   char string[MAX_STRLEN+1], attr;

   adv_puts_han("ÑÁ¡e· ·©¦¦…·e xscrollup_han() ‰Á xscrolldn_han() ·a¡ ¯aÇa©"
                "¯¡Ç© ® ·¶¯s“¡”a. xscrollup_han() ·e ¶á¡ Ðeº‰ ¯aÇa©–A•¡¢ Ða"
                "“e Ðq®·¡‰¡ xscrolldn_han() ·e ´aœ¡ Ðeº‰ ¯aÇa©–A•¡¢ Ða“e "
                "Ðq®·³“¡”a.\n", 0);
   wait(type_any_key);

   draw_dummy_screen();
   maxx = get_maxx_han();
   maxy = get_maxy_han();
   minx = get_minx_han();
   miny = get_miny_han();

   puts_han("\n´a¢ Ç¡a ’Ÿa¡e ¶á¡ Ðeº‰ ¯aÇa©Ðs“¡”a.\n");
   puts_han("<Example 1>");
   puts_han("xscrollup_han(minx, miny, maxx, maxy);");
   wait(type_any_key);
   xscrollup_han(minx, miny, maxx, maxy);
   wait("¶á¡ ¯aÇa© –A´ö¯s“¡Œa?");

   puts_han("¸a, ·¡¤åµA“e ´aœ¡ Ðeº‰ ¯aÇa©Ðs“¡”a.\n");
   puts_han("<Example 2>");
   puts_han("xscrolldn_han(minx, miny, maxx, maxy);");
   wait(type_any_key);
   xscrolldn_han(minx, miny, maxx, maxy);
   wait("´aœ¡ ¯aÇa© –A´ö¯s“¡Œa?");

   /* adv_puts_han() ·e ¢…¸w ¸÷i Ðq®·¡£a¡ ¢…¸w· {·i aÈa“e  "\n"  ·i */
   /* ¸aµi· ¬å–µA –¡e ´e–A‹¡ ˜¢…µA puts_han() ·a¡  "\n"  ·i  Â‰bÐe”a. */
   /* ”e, "\n" ·i ¢…¸aµi· {µA –‰ ®“e ·¶”a.                               */
   puts_han("\n");
   adv_puts_han("·¡¤åµA“e ¯aÇa© ´ó‰Á ¯aÇa© ”a¶…·i ‰­¢ ¤e¥¢Ð ¥¡‰V¯s“¡”a. "
                "·¡œý‰A Ða¡e  aÃ¡ ÑÁ¡e·¡ Óe—iŸ¡“e —uÐe “a±·¡ —i ˆõ·³“¡”a. "
                "º—»¡Ðaa¡e ´a¢ Ç¡a ’Ÿa¡e –S“¡”a.", 0);
   wait(type_any_key);

   clear_key_buff();
   while(!kbhit())
   {
      xscrollup_han(minx, miny, maxx, maxy);
      xscrolldn_han(minx, miny, maxx, maxy);
   }

   puts_han("\n¯aÇa©µA ´a¢œå ¢…¹Aˆa ´ô´ö¯s“¡Œa?");
   wait(type_any_key);
}

void change_cursor(void)
/* Äá¬á· ¡¡´··i ¤aŽ“e ¤w¤ó·i ¥¡µa º…”a. */
{
   /* Äá¬á· ¡¡´··¡ USER_DEFINED_CURSOR ·© ˜ ¬a¶wÐi ¬a¶w¸a ¸÷· Äá¬á· ¡¡´· */
   char han_shape[] =   /* ¯³¸a ¡¡´·· Ðe‹i Äá¬á */
   {
      0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80,
      0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0xff, 0xff,
      0xff, 0xff, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80,
      0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x08
   };
   char eng_shape[] =	/* ¯³¸a ¡¡´·· µw¢… Äá¬á */
   {
      0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0xff,
      0xff, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18
   };
   char string[31], exit_key[] = { UP, DOWN, 0 }, key, cursor_num;

   adv_puts_han("set_cursor_han() ·i ¬a¶wÐa¡e Äá¬á· ¡¡´··i ¤aŽ‰ ® ·¶¯s“¡”a.\n", 0);
   adv_puts_han("<Up>, <Down> Ç¡Ÿi ’Ÿa¡e Äá¬á· ¡¡´··¡ ¤aŽé ˆõ·³“¡”a. ¡¡"
                "– 4 ˆa»¡ ¡¡´··¡ º…§¡–A´á ·¶·a“¡ ¯¡ÐñÐ ¥¡­A¶a. ‹a e –a"
                "¡e <Esc> Ç¡Ÿi ’Ÿa¡e –S“¡”a.\n", 0);
   puts_han("<Example>");
   puts_han("set_cursor_han(BOX_CURSOR, \"\", BIG_CURSOR, \"\");");
   wait(type_any_key);

   strcpy(string, "Äá¬á· ¯¡Ðñ");
   cursor_num = 0;
   adv_gets_han_index = 0 * (-1) - 1;   /* ÂAÁ¡ Äá¬á ¶áÃ¡“e 0 */
   while(1)
   {
      draw_box_han(8, 9, 42, 11, BOX_H1V1);
      key = adv_gets_han(10, 10, string, 30, FONT_NORMAL, exit_key);
      if(key == ESC || key == ENTER) break;
      if(key == UP)
	 cursor_num = (cursor_num + 1) % 4;
      else
	 cursor_num = (cursor_num - 1 + 4) % 4;
      set_cursor_han(cursor_num, han_shape, cursor_num, eng_shape);
      adv_gets_han_index = adv_gets_han_index * (-1) - 1;
   }
}

void show_modify_char(void)
/* ‹i¸a ¡¡´··i ¤aŽ“e ¤w¤ó·i ¥¡µa º…”a. */
{
   char string[6], *screen;
   mini attr;

   adv_puts_han("set_attr_han() ·i ¬a¶wÐa¡e ‹i¸a· ¡¡´··i ¤aŽ‰ ® ·¶¯s“¡”a. "
                "”a·qµA¬á ˆb ¡¡´·µA Ð”wÐa“e ®Ÿi ”áÐe ˆõ·i ·³bÐa¡e ¹¡Ðs–E "
                "¡¡´··¡ Â‰b–S“¡”a. µŸi —i´á, ÉA–Ÿ¡Áa»¥ ŠŠ·e ·qµw ¢…¸aŸi "
                "¥¡‰¡ ¯¼·a¡e 2 + 1 + 8 = 11 ·i ·³bÐa¡e –S“¡”a. ¯©¹A¡ Ïa¡‹a"
                "œ‘·i ¼i ˜“e ”a·q‰Á ˆ{·¡ ÑA”áÑÁ·©µA ¸÷·–E  Ça¡¬w®Ÿi OR "
                "¯¡Ç¡¡e –S“¡”a.\n", 0);
   puts_han("<Example>");
   puts_han("set_attr_han(FONT_BOLD | FONT_OUTLINE | FONT_SHADE);");
   puts_han("puts_han(string);\n");

   set_max_strlen_han(5);
   puts_han("‹a e–a¡e ‹a· <Enter> Ç¡Ÿi ’Ÿa­A¶a.\n");
   do
   {
      puts_han("´á˜å ¡¡´·· ‹i¸aŸi ¶¥Ða¯³“¡Œa?");
      puts_han("¥¡É· = 0  ŠŠ·eÁA = 1  ÉA–Ÿ¡ = 2  ‹aŸ±¸a = 4  ·qµw = 8  £»º‰ = 16  ¤e¸å = 32 ");
      gets_han(string);
      attr = (mini) atoi(string);  /* ¢…¸aµi¡ ·³b¤h·e ­¢¬÷·i ®•¸a¡ ¤aŽ…”a */
      if(attr >= FONT_VERT_DBL)
      {
         attr = FONT_NORMAL;
         puts_han("¤ñ¶áŸi ¤õ´áv¯s“¡”a. ”a¯¡ ·³bÐa­A¶a");
         continue;
      }
      set_attr_han(attr);
      if(strlen(string)) puts_han("·¡œå ¡¡´··¡  x¯s“¡Œa?");
      attr = FONT_NORMAL;
   }
   while(strlen(string));
   set_max_strlen_han(MAX_STRLEN);
}

void show_double_char(void)
/* ÑÂ” ¢…¸aŸi Â‰bÐa“e ¤w¤ó·i ¥¡µa º…”a. */
{
   adv_puts_han("­¢¬÷·i FONT_HORIZ_DBL a FONT_VERT_DBL ¡ ¬é¸÷Ða¡e ÑÂ”–E "
                "‹i¸aŸi Â‰bÐi ® ·¶¯s“¡”a. ÑÂ” ¢…¸a“e Èa·¡Ëi ÑÁ¡e·i Š¬÷Ði"
                "˜ ¬a¶wÐa¡e ¹½¯s“¡”a.\n", 0);
   puts_han("<Example>");
   puts_han("aputs_han(x, y, \"Ðe‹i ˆa¡ 2 ¤ ÑÂ”\", FONT_HORIZ_DBL);");
   wait("´a¢ Ç¡a ’Ÿa¡e ÑÂ”–E ‹i¸aŸi ¥© ® ·¶¯s“¡”a.");
   clrscr_han();

   aputs_han(0,  0, "Ðe‹i ˆa¡ 2 ¤ ÑÂ”",      FONT_HORIZ_DBL);
   aputs_han(0,  2, "Ðe‹i ­A¡ 2 ¤ ÑÂ”",      FONT_VERT_DBL);
   aputs_han(0,  4, "Ðe‹i ˆa¡/­A¡ 2 ¤ ÑÂ”", FONT_HORIZ_DBL | FONT_VERT_DBL);
   aputs_han(0,  6, "Ðe‹i ˆa¡ 2 ¤ ÑÂ”",      FONT_HORIZ_DBL | FONT_SHADE);
   aputs_han(0,  8, "Ðe‹i ­A¡ 2 ¤ ÑÂ”",      FONT_VERT_DBL | FONT_SHADOW);
   aputs_han(0, 10, "Ðe‹i ˆa¡/­A¡ 2 ¤ ÑÂ”", FONT_HORIZ_DBL | FONT_VERT_DBL | FONT_SHADE);
   aputs_han(0, 12, "Ðe‹i ˆa¡ 2 ¤ ÑÂ”",      FONT_HORIZ_DBL | FONT_INVERSE);
   aputs_han(0, 14, "Ðe‹i ­A¡ 2 ¤ ÑÂ”",      FONT_VERT_DBL | FONT_BOLD);
   aputs_han(0, 16, "Ðe‹i ˆa¡/­A¡ 2 ¤ ÑÂ”", FONT_HORIZ_DBL | FONT_VERT_DBL | FONT_INVERSE);

   wait(type_any_key);
}

void show_replace_font(void)
/* —¡¯aÇaµA ·¶“e Í¥Ëa ÑÁ·©¡¦Èá Í¥ËaŸi ¤aŽ“e ¤w¤ó·i ¥¡µa º…”a. */
{
   adv_puts_han("replace_font() ·i ¬a¶wÐa¡e —¡¯aÇaµA ·¶“e Í¥Ëa ÑÁ·©·i ·ª´á¬á "
                "Í¥ËaŸi ¤aŽ´á º““¡”a. ‹aœáa ­¢•¡ˆa “aŸ¡£a¡ Ðe ÑÁ¡eµA¬á µaœá"
                "ˆa»¡ Í¥ËaŸi ¬a¶wÐaµa Â‰bÐ´¡ Ði ˜“e set_font() Ÿi ¬a¶wÐa­A"
                "¶a.\n", 0);
   puts_han("<Example>");
   puts_han("replace_font(\"hangod1.font\", \"engital.fnt\", \"\");\n");
   wait(type_any_key);

   puts_han(" å¸á Ðe‹i·i ‰¡—¢ 1¤åÁA, µw¢…·i ·¡È‰Ÿ¢ÁA¡ ¤aŽ‰V¯s“¡”a.\n");
   wait(type_any_key);
   clrscr_han();
   replace_font("hangod1.fnt", "engital.fnt", "");
   /* Ðe‹i Í¥Ëa“e hangod1.fnt ¡, µw¢… Í¥Ëa“e engital.fnt ¡ */
   /* ¤aŽ‰¡ Ëb® ¢…¸a Í¥Ëa“e ‹a”¡ –…”a.                   */
   draw_dummy_screen();

   puts_han("·¡¤åµA“e Ðe‹i·i ‰¡—¢ 2¤åÁA¡ ¤aŽ‰V¯s“¡”a.");
   wait(type_any_key);
   clrscr_han();
   replace_font("hangod2.fnt", "", "");
   draw_dummy_screen();

   puts_han("·¡¤åµA“e Ðe‹i·i ÓiŸ±ÁA¡ ¤aŽ‰V¯s“¡”a.");
   wait(type_any_key);
   clrscr_han();
   replace_font("hanpil.fnt", "", "");
   draw_dummy_screen();

   puts_han("·¡¤åµA“e Ðe‹i·i ­¥‹i³¡ÁA¡ ¤aŽ‰V¯s“¡”a.");
   wait(type_any_key);
   clrscr_han();
   replace_font("hanson1.fnt", "", "");
   draw_dummy_screen();

   puts_han("·¡¤åµA“e Ðe‹i·i ¬‘¢‰ÁA¡ ¤aŽ‰V¯s“¡”a.");
   wait(type_any_key);
   clrscr_han();
   replace_font("hansam.fnt", "", "");
   draw_dummy_screen();

   puts_han("·¡¤åµA“e Ðe‹i·i ¡w¹¡ÁA¡ ¤aŽ‰V¯s“¡”a.");
   wait(type_any_key);
   clrscr_han();
   replace_font("hanmyung.fnt", "", "");
   draw_dummy_screen();
   wait(type_any_key);

   /* ¶¥œ· Í¥Ëa¡ ¤aŽ…”a. */
   replace_font("hangul.fnt", "english.fnt", "");
}

void show_set_font(void)
/* ¡A¡¡Ÿ¡µA ·¶“e Í¥Ëa¡¦Èá Í¥ËaŸi ¤aŽ“e ¤w¤ó·i ¥¡µa º…”a. */
{
   /* Í¥ËaŸi ¸á¸wÐi ¡A¡¡Ÿ¡Ÿi ˆaŸ¡Ç¡“e Í¡·¥Èá */
   char *han_font1, *han_font2, *han_font3;
   unsigned long avail_mem;

   adv_puts_han("set_font() Ÿi ¬a¶wÐa¡e ¡A¡¡Ÿ¡µA £¡Ÿ¡ ·ª´á–… Í¥ËaŸi ¬a¶wÐi ® "
                "·¶¯s“¡”a.\n", 0);
   adv_puts_han("read_han_font(), read_eng_font(), read_spc_font() Ÿi ¬a¶wÐa¡e "
                "¡A¡¡Ÿ¡Ÿi Ði”wÐe Ò ÑÁ·©µA¬á Í¥ËaŸi ·ª´á ‹a ¡A¡¡Ÿ¡µA ¸á¸wÐa“e•A "
                "·¡ Í¥Ëa“e set_font() ¡ ¬a¶wÐi ® ·¶¯s“¡”a. replace_font() “e "
                "»¢¸ó ÑÁ·©µA¬á Í¥Ëa ÑÁ·©·i ·ª·a¡e¬á ¤aŽ»¡ e, set_font() “e "
                "£¡Ÿ¡ ¡A¡¡Ÿ¡µA ·ª´á ‘½·e Í¥Ëa¡ ¤aŽ£a¡ set_font() · ­¢•¡ˆa "
                "¶©—wÓ¡ ¨aŸs“¡”a. ˜aœa¬á Ðe ÑÁ¡eµA¬á µaœáˆa»¡ Í¥ËaŸi ¬a¶wÐaµa "
                "ÑÁ¡e·i Š¬÷Ðaa¡e set_font() Ÿi ¬a¶wÐ´¡ Òa‰ÁŸi ¥© ® ·¶¯s“¡"
                "”a.", 0);
   adv_puts_han("Ï©¶a´ô‰A –E Í¥ËaµA ¬a¶w–E ¡A¡¡Ÿ¡“e free_font() ·i ¬a¶wÐa¡e "
                "•©a ¤h·i ® ·¶¯s“¡”a.\n", 0);
   puts_han("<Example 1>");
   puts_han("read_han_font(&han_font1, \"hangod1.fnt\");");
   puts_han("set_font(han_font1, NULL, NULL);");

   wait(type_any_key);
   clrscr_han();
   set_window_han(0, system_maxy-10, system_maxx, system_maxy-1);
   draw_box_han(0, abs_maxy_han-11, abs_maxx_han, abs_maxy_han-11, BOX_H1V1);
   /* Í¥Ëa· ÀáŸ¡¯¡ ¡A¡¡Ÿ¡ˆa ´á˜ý‰A ¥e•·–A“e»¡ ¥¡µaº‹¡ ¶áÐ */
   /* ÂAÁ¡· ¡A¡¡Ÿ¡´··i avail_mem µA ¸á¸wÐe”a.               */
   printf_han("Àá·q ¬wÈµA¬á q·e ¡A¡¡Ÿ¡ : %8lu bytes\n", (avail_mem = farcoreleft()));
   puts_han(" å¸á Í¥Ëa¦Èá ·ª‰V¯s“¡”a.");

   /* Í¥ËaŸi ”e»¡ ·ª´á –‹¡ e Ðe”a */

   puts_han("read_han_font(&han_font1, \"hangod1.fnt\");");
   if(read_han_font(&han_font1, "hangod1.fnt") != OK_HAN)
   {
      /* µa‹¡¬á han_font1 ·¡ char Ñw Í¡·¥Èá·¡»¡ e ¤å»¡ µe¬e¸aŸi  ÂáÐe  Ò */
      /* ñ‰aº“e ˆõµA º·Ði ˆõ! read_han_font() Ðq®µA¬á  Í¡·¥Èá ¥e®·¥ */
      /* han_font1 · ˆt·i ¤aŽ‹¡ ˜¢…µA char Ñw Í¡·¥Èá· Í¡·¥ÈáŸi ·¥¸a¡ */
      /* ÂáÐa‹¡ ˜¢…·¡”a. ´aº º—¶aÐa”a!                                  */
      wait("¡A¡¡Ÿ¡ˆa ¦¹¢Ðaˆáa Í¥ËaÑÁ·©·¡ ´ô¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
      set_window_han(0, 0, system_maxx, system_maxy-1);
      return;
   }
   printf_han("Ñe¸ q·e ¡A¡¡Ÿ¡ : %8lu bytes\n", farcoreleft());
   wait(type_any_key);

   puts_han("read_han_font(&han_font2, \"hangod2.fnt\");");
   if(read_han_font(&han_font2, "hangod2.fnt") != OK_HAN)
   {
      /* ‹a¸åµA ·ª·e Í¥Ëa“e free ¯¡Ç¥”a. */
      free_font(han_font1, NULL, NULL);
      wait("¡A¡¡Ÿ¡ˆa ¦¹¢Ðaˆáa Í¥ËaÑÁ·©·¡ ´ô¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
      set_window_han(0, 0, system_maxx, system_maxy-1);
      return;
   }
   printf_han("Ñe¸ q·e ¡A¡¡Ÿ¡ : %8lu bytes\n", farcoreleft());
   wait(type_any_key);

   puts_han("read_han_font(&han_font3, \"hanson1.fnt\");");
   if(read_han_font(&han_font3, "hanson1.fnt") != OK_HAN)
   {
      free_font(han_font1, NULL, NULL);
      free_font(han_font2, NULL, NULL);
      wait("¡A¡¡Ÿ¡ˆa ¦¹¢Ðaˆáa Í¥ËaÑÁ·©·¡ ´ô¯s“¡”a. ´a¢ Ç¡a ’Ÿa­A¶a.");
      set_window_han(0, 0, system_maxx, system_maxy-1);
      return;
   }
   printf_han("Ñe¸ q·e ¡A¡¡Ÿ¡ : %8lu bytes\n", farcoreleft());

   puts_han("\n­Aˆa»¡ Í¥ËaŸi ”a·ª´ö¯s“¡”a. ·¡¤åµA“e ÑÁ¡eµA Â‰bÐ ¥¡‰V¯s“¡”a.");
   puts_han("\nset_font(han_font1, NULL, NULL);");
   puts_han("xputs_han(10, 3, \"‰¡—¢ 1¤åÁA·³“¡”a.\");");
   puts_han("set_font(han_font2, NULL, NULL);");
   puts_han("xputs_han(10, 4, \"‰¡—¢ 2¤åÁA·³“¡”a.\");");
   puts_han("set_font(han_font3, NULL, NULL);");
   puts_han("xputs_han(10, 5, \"­¥‹i³¡ÁA·³“¡”a.\");");
   wait(type_any_key);

   set_font(han_font1, NULL, NULL);
   /* µw¢… Í¥ËaµÁ Ëb® ¢…¸a Í¥Ëa· Í¡·¥ÈáŸi NULL ¡ ¬é¸÷Ða¡e */
   /* Ðe‹i Í¥Ëa e ¤aŽå”a.                                    */
   xputs_han(10, 3, "‰¡—¢ 1¤åÁA·³“¡”a.");
   set_font(han_font2, NULL, NULL);
   xputs_han(10, 4, "‰¡—¢ 2¤åÁA·³“¡”a.");
   set_font(han_font3, NULL, NULL);
   xputs_han(10, 5, "­¥‹i³¡ÁA·³“¡”a.");
   puts_han("ÑÁ¡eµA ­Aˆa»¡ Í¥Ëaˆa ”a aÈav¯s“¡Œa?\n");
   wait(type_any_key);

   adv_puts_han("¸a, ·¡¹A ¶¥œ Í¥Ëa¡ •©a ‘½‰V¯s“¡”a. ¶¥œ Í¥Ëa¡ •©a "
	        "‘½·aa¡e ¤e—a¯¡ return_to_org_font() Ÿi Ñ¡Â‰Ð´¡ Ðs“¡”a.\n", 0);
   puts_han("<Example 2>");
   puts_han("return_to_org_font(1, 0, 0);");
   wait(type_any_key);
   /*************** ·¡ ¦¦…·¡ ¶¥œ· Í¥Ëa¡ •©a ‘½“e ¦¦…·¡”a. **************/
   /* return_han  e 1 ·i º´ö·a£a¡ Ðe‹i Í¥Ëa e ¶¥œ”¡ •©a ‘½“e”a.( = 1)  */
   /* µw¢…‰Á Ëb® ¢…¸a“e ¤aŽ… ¸â·¡ ´ô·a£a¡ ¶¥œ”¡ •©Ÿ© Ï©¶aˆa ´ô”a.( = 0) */
   /**************************************************************************/
   return_to_org_font(1, 0, 0);

   puts_han("\n·¡¤åµA“e ­Aˆa»¡ Í¥ËaµA Ði”w–E ¡A¡¡Ÿ¡Ÿi ¥¢Šá(free)¯¡Ç¡‰V¯s“¡”a.\n");
   puts_han("<Example 3>");
   puts_han("free_font(han1_font, eng_font, spc_font);\n");
   wait(type_any_key);
   puts_han("free_font(han_font1, NULL, NULL);");
   wait(type_any_key);
   free_font(han_font1, NULL, NULL);
   printf_han("Ñe¸ q·e ¡A¡¡Ÿ¡ : %8lu bytes\n", farcoreleft());

   puts_han("free_font(han_font2, NULL, NULL);");
   wait(type_any_key);
   free_font(han_font2, NULL, NULL);
   printf_han("Ñe¸ q·e ¡A¡¡Ÿ¡ : %8lu bytes\n", farcoreleft());

   puts_han("free_font(*han_font3, NULL, NULL);");
   wait(type_any_key);
   free_font(han_font3, NULL, NULL);
   printf_han("Ñe¸ q·e ¡A¡¡Ÿ¡ : %8lu bytes\n", farcoreleft());

   printf_han("\nÂAÁ¡· ¡A¡¡Ÿ¡“e %8lu bytes µv¯s“¡”a. Ñe¸ q·e ¡A¡¡Ÿ¡µÁ ·©Ã¡"
	      "Ðs“¡Œa?\n", avail_mem);
   wait(type_any_key);
   set_window_han(0, 0, system_maxx, system_maxy-1);
}

void show_spc_font(void)
/* Ëb® ¢…¸a· ¬a¶w¤ó·i ¥¡µa º…”a. */
{
   int x, y, code;

   adv_puts_han("Àõ ¤a·¡Ëa· ˆt·¡ SPC_ID(=212) ·¡¡e 2 ¤a·¡Ëa· Ëb® ¢…¸a¡ ÀáŸ¡"
                "–S“¡”a. ”a·q µ“e (0, 0) µA 8 ¤å¼ Ëb® ¢…¸aŸi Â‰bÐs“¡”a.\n", 0);
   puts_han("<Example>");
   puts_han("xputs_han(0, 0, \"\\xd4\\x8\");");
   puts_han("   ™¡“e");
   puts_han("put_han_font(0, 0, SPC_ID, 8);\n");
   adv_puts_han("´a¢ Ç¡a ’Ÿa¡e <Ðe>· Ëb® ¢…¸a Í¥ËaÑÁ·© special.fnt "
	        "µA ·¶“e 256 ˆa»¡· Ëb® ¢…¸a—i·i ¥¡¯© ® ·¶¯s“¡”a.", 0);
   wait(type_any_key);

   clrscr_han();
   vclear_text_han(18, 4, 60, 18, LIGHTGRAY);
   set_vbcolor_han(BLACK, LIGHTGRAY);
   draw_box_han(18, 4, 60, 18, BOX_H1V1);
   xputs_han(32, 2, "Ëb® ¢…¸a Å¡—aÎa");
   /* Ëb® ¢…¸a—i·i Â‰bÐe”a */
   for(code = 0; code < 256; code++)
   {
      x = 20 + (code % 20) * 2;
      y = 5 + code / 20;
      put_han_font(x, y, SPC_ID, code);
   }
   set_vbcolor_han(base_color, base_bkcolor);
   wait(type_any_key);
}

void show_output_offset(void)
/* Â‰b µ³­U·i ¹¡¸÷Ða“e ¤w¤ó·i ¥¡µa º…”a. */
{
   int i;

   adv_puts_han("Â‰b µ³­U·e ‹i¸aŸi Â‰bÐi ˜ ¶¥œ· ¹ÁÎaµA¬á ¡y •¡Ëa ´aœ¡ "
                "a¬á Â‰bÐi »¡Ÿi ‰i¸÷Ða“e ˆõ·³“¡”a. ·¡Ÿi ·¡¶wÐa¡e y ¹ÁÎaµA "
                "‰ÅÐe Ðe ‹aœÏ¢ ¹ÁÎa¡ Â‰bÐi ® ·¶¯s“¡”a. ”e, HGC · ‰w¶µA“e "
                "4 •¡Ëa ”e¶á¡ ¸b•·–A£a¡ º·Ða¯¡‹¡ ¤aœs“¡”a.\n", 0);
   puts_han("<Example>");
   puts_han("set_output_offset_han(8);");
   puts_han("puts_han(\"8 •¡Ëa ´aœµA ¿¢Ó³“¡”a.\");\n");
   wait(type_any_key);

   adv_puts_han("´a¢ Ç¡a ’Ÿa¡e Â‰b µ³­U·i 0 ¦Èá 35 Œa»¡ ¤aŽ´ö·i ˜· Â‰b "
                "ÑwÈŸi ¥© ® ·¶¯s“¡”a.", 0);
   wait(type_any_key);
   for(i = 0; i < 36; i++)
   /* Â‰b µ³­U·i 0 ¦Èá 35 Œa»¡ ¤aŽ¡e¬á ˆ{·e ¹ÁÎaµA ‰­¢ ¢…¸aµi·i Â‰b */
   {
      set_output_offset_han(i);
      xputs_han(5+i*2, 12, "Ðe");
   }

   /* Â‰b µ³­U·i ¶¥œ”¡ •©a ‘½“e”a */
   set_output_offset_han(0);
   wait(type_any_key);
}

void show_table(void)
/* 1 ¤a·¡Ëa ‹aœÏ¢ ¢…¸aŸi ¬a¶wÐaµa •¡ÎaŸi ¸b¬÷Ða“e ¤w¤ó·i ¥¡µa º…”a. */
{
   static struct
   {
      char *code;   /* ´¢Î‘ Å¡—a */
      char *name;   /* ´¢Î‘¡w */
      int  price;   /* ˆa‰b */
      char *unit;   /* ”e¶á */
   } drug[10] =     /* •¡ÎaµA Â‰bÐi ¸aža(´¢Î‘ ¸aža) */
   {
      { "A-01", "´a¡¢¬a¯©Ÿ¥ 250 mg",   80, "Ä“¯I" },
      { "A-02", "´‘Ï¡Çiž‚¯a 500 mg",  150, "Ä“¯I" },
      { "A-03", "´qÍ¡¹I M",           250, "Í¡"   },
      { "A-04", "´‚Ë¡Ï¡—a",            80, "¸÷"   },
      { "B-01", "¥A¯aÈa¹A",           100, "¸÷"   },
      { "C-01", "Å¥È‚ 600",           100, "Ä“¯I" },
      { "C-02", "Çi¡ËaŸ¡ a¹© µe‰¡", 1800, "É·"   },
      { "F-01", "Ò¯¡—¥ µe‰¡",       1200, "É·"   },
      { "N-01", "‘¡ž¡¡",            1500, "É·"   },
      { "W-01", "¶áÀ÷®",             250, "¥w"   }
   };
   int i;

   adv_puts_han("¤b¯aŸi ‹aŸ¡“e draw_box_han() ‰Á ´a¯aÇ¡Å¡—a 128 ·¡¬w· ‹aœÏ¢ "
	        "¢…¸a•¡ Â‰bÐi ® ·¶“e eputch_han() ·¡a put_eng_font() Ÿi ¬a¶w"
                "Ða¡e ®ó‰A •¡ÎaŸi ¸b¬÷Ði ® ·¶¯s“¡”a. ´a¢ Ç¡a ’Ÿa¡e ·¡ Ðq®"
                "—i·i ¬a¶wÐaµa ¸b¬÷Ðe •¡ÎaŸi ¥© ® ·¶¯s“¡”a.", 0);
   wait(type_any_key);

   vclrscr_han(LIGHTGRAY);
   set_vbcolor_han(BLACK, LIGHTGRAY);
   draw_box_han( 1, 0, 78, system_maxy-1, 0);
   draw_box_han( 1, 2, 78, 4, 0);
   draw_box_han( 8, 2,  8, system_maxy-1, 0);
   draw_box_han(51, 2, 51, system_maxy-1, 0);
   draw_box_han(65, 2, 65, system_maxy-1, 0);
   put_eng_font( 1, 2, 195);
   put_eng_font( 1, 4, 195);
   put_eng_font(78, 2, 180);
   put_eng_font(78, 4, 180);
   put_eng_font( 8, 2, 194);
   put_eng_font(51, 2, 194);
   put_eng_font(65, 2, 194);
   put_eng_font( 8, 4, 197);
   put_eng_font(51, 4, 197);
   put_eng_font(65, 4, 197);
   put_eng_font( 8, system_maxy-1, 193);
   put_eng_font(51, system_maxy-1, 193);
   put_eng_font(65, system_maxy-1, 193);
   set_color_han(BLUE);
   xputs_han( 3, 3, "Å¡—a");
   xputs_han(24, 3, "´¢    Î‘    ¡w");
   xputs_han(55, 3, "ˆa   ‰b");
   xputs_han(69, 3, "”e  ¶á");

   set_color_han(BLACK);
   for(i = 0; i < 10; i++)      /* •¡Îa· ¶w·i ¥¡µa º…”a. */
   {
      xputs_han(   3, 5+i, drug[i].code);
      xputs_han(  10, 5+i, drug[i].name);
      xprintf_han(53, 5+i, "%11d", drug[i].price);
      xputs_han(  67, 5+i, drug[i].unit);
   }

   /* Äéœá ¡¡“¡ÈáµA¬á“e ¨iˆe¬‚·a¡ Ób¤‚ ¡¡“¡ÈáµA¬á“e ·qµw ¢…¸a¡ */
   /* Èa·¡Ëi·i Â‰bÐe”a.                                         */
   if(in_vga_system)
      center_puts_han(1, " ´¢Î‘ ·³b/®¸÷ ", RED, FONT_NORMAL);
   else
      center_puts_han(1, " ´¢Î‘ ·³b/®¸÷ ", RED, FONT_SHADE);

   draw_horiz_line(0, system_maxy*16-1, (system_maxx+1)*8, BLACK);
   set_vbcolor_han(base_color, base_bkcolor);
   wait(type_any_key);
}

void show_hanja(void)
/* Ðe¸aŸi ·³Â‰bÐa“e ¤w¤ó·i ¥¡µa º…”a. */
{
   unsigned char exit_key[] = { F9, F10, 0 }, key;
   char string[41], hanja_code[3], hangul_code[3];
   int ret_value;

   adv_puts_han("<Ðe>·e Ðe¸aŸi ”až“e Ðq®—i·i ¹A‰·Ða“e•A ·¡ Ðq®—i·i "
                "·¡¶wÐa¡e Ðe¸aŸi ·³Â‰bÐi ® ·¶“e Ðq®Ÿi  e—i ® ·¶¯s“¡”a. "
                "<Ðe>· ·³Â‰b Ðq®µA Ðe¸a ·³Â‰b ‹¡“w·i ý»¡ ´g‰¡ ¥i•¡¡ "
                "¨ ‘½·e ˆõ·e Ðe¸aŸi ¸åÑa ³i Ï©¶aˆa ´ô“e Ïa¡‹aœ‘µA•¡ ”÷Ã¡ Çe "
                "Ðe¸a ·³Â‰b žË¥·¡ —i´áˆa“e ˆõ·i Ï¡Ða‹¡ ¶áÐ¬á ·³“¡”a. Ðe¸a"
                "Ÿi  g·¡ °á´¡ Ða“e ¦…—i·e ·¡ Ïa¡‹aœ‘· ­¡¯aŸi Àq‰¡Ðaµa ¬¡"
                "¶… ·³b Ðq®Ÿi  e—i´á ³a­A¶a.\n", 0);
   adv_puts_han("”a·q µ“e adv_gets_han() ·a¡ Ðe¸aŸi ·³b¤h“e ¤w¤ó·i ¥¡µa "
		"º““¡”a. Ñe—¡BÉ¡Ÿ¡µA Ðe¸a Í¥Ëa ÑÁ·© hanja.fnt ˆa ´ô·a¡e Ðe"
		"¸aŸi ¥© ® ´ô¯s“¡”a. ¬a¶wÐi ® ·¶“e Ç¡“e ”a·q‰Á ˆ{¯s“¡”a.", 0);
   puts_han("------------------------------------");
   puts_han("1. F9  : Ðe‹i·i Ðe¸a¡ ¥eÑÅ");
   puts_han("2. F10 : Ðe¸aŸi Ðe‹i¡ ¥eÑÅ");
   puts_han("3. adv_gets_han() · Ç¡ ‹a”¡ ¬a¶w");
   wait(type_any_key);

   draw_box_han(19, 14, 60, 16, BOX_H1V1);
   strcpy(string, "Ðe‹i‰Á Ðe¸a");
   while(1)
   {
      key = adv_gets_han(20, 15, string, 40, FONT_NORMAL, exit_key);
      /* adv_gets_han() ·¡ Ñ¡Â‰–E »¢ÒµA adv_gets_han_index “e */
      /* Ñe¸· Äá¬á ¶áÃ¡µA ”Ðe ¸÷¥¡Ÿi ˆx“e”a                 */

      if(key == ENTER || key == ESC) break;
      if(key == F9 && (string[adv_gets_han_index] & 0x80))
      /* Ñe¸ Äá¬á ¶áÃ¡· ‹i¸aˆa Ðe‹i·© ˜ Ð”wÐa“e Ðe¸aŸi ¬åÈ‚¤h“e”a */
      {
	 ret_value = choose_hanja(string + adv_gets_han_index, hanja_code);
	 if(ret_value == ENTER)
	 /* ¬åÈ‚–E Ðe¸a· Å¡—a¡ ¤aŽ…”a */
	 {
	    string[adv_gets_han_index]     = hanja_code[0];
	    string[adv_gets_han_index + 1] = hanja_code[1];
	 }
      }
      else if(key == F10 && (unsigned char) string[adv_gets_han_index] >= 0xe0)
      /* Ñe¸ Äá¬á ¶áÃ¡· ‹i¸aˆa Ðe¸a·© ˜“e Ðe‹i¡ ¤aŽ…”a. */
      {
	 hanja_to_hangul(string + adv_gets_han_index, hangul_code);
	 string[adv_gets_han_index]     = hangul_code[0];
	 string[adv_gets_han_index + 1] = hangul_code[1];
      }
      else
      /* <F9>, <F10> Ç¡ˆa ÀáŸ¡–A»¡ ´g·e ‰w¶µA“e ·¥•B¯aŸi ‹a”¡ –…”a. */
      /* »b, Äá¬á· ¶áÃ¡Ÿi ¤aŽ»¡ ´g“e”a.            		       */
      {
	 adv_gets_han_index = adv_gets_han_index * (-1) - 1;
	 continue;
      }

      /* <F9>, <F10> Ç¡ˆa ÀáŸ¡–E ‰w¶ ”a¯¡ adv_gets_han() ·¡  Ñ¡Â‰ */
      /* –A¡e ÂAÁ¡· Äá¬á ¶áÃ¡ˆa Ñe¸· Äá¬á ¶áÃ¡ ¤a¡ ”a·q·¡ –A•¡ */
      /* ¢ adv_gets_han_index  · ˆt·i ¹¡¸÷Ðe”a                   */
      /* ‰·¯¢ : adv_gets_han_index = ¶¥Ða“e Äá¬á ¶áÃ¡ * (-1) - 1   */
      adv_gets_han_index = (adv_gets_han_index + 2) * (-1) - 1;
      if(adv_gets_han_index > (int) strlen(string)) adv_gets_han_index = strlen(string);
   }
   clrscr_han();
   printf_han("·³b¤h·e ‹i¸a“e <%s> ·³“¡”a.", string);
   wait(type_any_key);
}


void toggle_keyboard_type(void)
/* ¸aÌe·i ¤aŽ…”a. */
{
   if(get_keyboard_type() == BUL3_TYPE)
   {
      set_keyboard_type(BUL2_TYPE);
      play("l16o3c");
   }
   else
   {
      set_keyboard_type(BUL3_TYPE);
      play("l16o3e");
   }
   draw_status_han();
   cputs_han("»¡‹q¦Èá ¬a¶wÐi Ðe‹i ¸aÌe·e ");
   if(get_keyboard_type() == BUL3_TYPE)
      puts_han("3 ¤é¯¢·³“¡”a.");
   else
      puts_han("2 ¤é¯¢·³“¡”a.");
   wait(type_any_key);
}

void show_status_line(void)
/* ¬wÈ œa·¥·i aÈaa‰A Ðaˆáa ¬aœa»¡‰A Ða“e ¤w¤ó·i ¥¡µa º…”a. */
{
   char string[MAX_STRLEN+1];

   adv_puts_han("¬wÈ œa·¥·e Ñe¸· ·³b ¬wÈŸi ¥¡µa º“e º‰·³“¡”a. ·¡ º‰· "
                "¡¡´··e ¸÷–E ˆõ•¡ ·¶»¡ e ¬a¶w¸a· ‹¡Ñ¡µA ˜aœa ¸a¯¥·¡ —¡¸a·¥"
                "Ð¬á  e—i ®•¡ ·¶¯s“¡”a.\n", 0);
   puts_han("¬wÈ œa·¥·i aÈaa‰A Ðaa¡e");
   puts_han("status_on_han();\n");
   puts_han("·¡œa‰¡ Ða¡e –A‰¡, ¬aœa»¡‰A Ðaa¡e");
   puts_han("status_off_han();\n");
   puts_han("·¡œa‰¡ Ða¡e –S“¡”a.\n");
   adv_puts_han("¬wÈ œa·¥·¡ ·¶“e º‰·e ¶å•¡¶· µwÐ··i ¤h“e Èá¥¡ C Ñ¡ÑÅ¶w "
                "Ðq®¡“e ”až‰ ® ´ô‰¡ ¯aÇa©•¡ ¬wÈ œa·¥·i ¨… ¦¦…µA¬á e "
                "·¡ž´á»³“¡”a. ¯aÇa©·¡ ´á˜ý‰A –A“e»¡ ¥¡a¡e, ‹i¸aŸi ·³bÐ "
                "¥¡­A¶a. ‹a e ·³bÐa‰¡ ¯¼·a¡e ‹a· <Enter> Ç¡Ÿi ’Ÿa­A¶a.", 0);

   do
   {
      gets_han(string);
      puts_han(string);
   }
   while(strlen(string));

   puts_han("\n¬wÈ œa·¥·i ¬aœa»¡‰A Ð ¥¡‰V¯s“¡”a.\n");
   puts_han("<Example 1>");
   puts_han("status_off_han();");
   wait(type_any_key);
   status_off_han();
   wait(type_any_key);
   puts_han("\n¬wÈ œa·¥·i ”a¯¡ aÈaa‰A Ð ¥¡‰V¯s“¡”a.\n");
   puts_han("<Example 2>");
   puts_han("status_on_han();");
   wait(type_any_key);
   status_on_han();
   wait(type_any_key);
}

void show_adv_gets(void)
/* ¶¡—a¯aÈaÑw ·³bÐq® adv_gets_han() ·i ¬a¶wÐa“e ¤w¤ó·i ¥¡µa º…”a. */
{
   char string[30+1];

   set_window_han(5, 0, system_maxx-5, system_maxy-1);
   adv_puts_han("<Ðe> µA¬á ˆa¸w ¬a¶w §¥•¡ˆa ‘¼·i ˆõ·a¡ ¬—ˆb–A“e Wordstar Ñw "
                "¢…¸aµi ·³bÐq® adv_gets_han() ·i ­¡ˆÐa‰V¯s“¡”a.\n", 0);
   adv_puts_han("µaœá¦…—i·e ÑÁ·© ·¡Ÿq‰Á ˆ{·e ˆõ·i ·³b¤h·i ˜ ¤e¸å–E ¹¡‹a aÐe "
                "¤b¯a ´eµA¬á ·³b·i ¤h“e ˆõ·i ¸aº ¥¡­v·i ˆõ·³“¡”a. ‹a ¤b¯a ´e"
                "µA¬á ·³b·i Ði ˜“e µaœáˆa»¡ Íe»³Ç¡Ÿi ¬a¶wÐi ® ·¶´á Àq ÍeŸ¡Ða"
                "‰¡ ¥¡‹¡µA•¡ ŒiqÐs“¡”a. µå eÐe Ïa¡‹aœ‘·i ¼aa¡e ·¡ Ðq®“e Ï©"
                "®¸â·¥•A, ´á¼å ·¡·AµA¬á·¥»¡ »¡‹qŒa»¡· Ðe‹i œa·¡§aœáŸ¡µA¬á“e "
                "·¡œáÐe ‹¡“w·i ˆa»¥ Ðq®ˆa ¹A‰·–A»¡ ´g´v¯s“¡”a.\n", 0);
   adv_puts_han("<Ðe> ·e ‹aœáÐe Ðq®Œa»¡•¡ ¹A‰·Ða“e•A ‹aˆõ·¡ ¤a¡ adv_gets_han"
                "() ·³“¡”a. ·¡ Ðq®µA¬á ¬a¶wÐi ® ·¶“e Íe»³Ç¡“e ”a·q‰Á ˆ{¯s“¡”a"
                ".\n", 0);
   set_attr_han(FONT_UNDERLINE);
   puts_han("adv_gets_han() µA¬á ¬a¶wÐi ® ·¶“e Íe»³Ç¡");
   set_attr_han(FONT_NORMAL);
   puts_han("1. Äá¬á ·¡•· : <Left>, <Right>, <Ctrl>+<Left>, <Ctrl>+<Right>,");
   puts_han("               <Home>, <End>");
   puts_han("2. ¬s·³/¬b¹A : <Ins>, <Del>, <BS>, <Ctrl>+<T>, <Ctrl>+<Y>,");
   puts_han("               <Ctrl>+<Q>+<Y>");
   puts_han("3. ·³b ¹·ža : <Enter>, <Esc>\n");
   wait(type_any_key);
   clrscr_han();

   set_window_han(0, 0, system_maxx, system_maxy-1);
   adv_puts_han("¸a, ‹aœñ ·¡ Ðq®Ÿi Ðe¤å ¬a¶wÐ ¥³¯¡”a. <Esc> Ç¡a <Enter> Ç¡Ÿi"
                " ’Ÿa¡e ¨a¹a aµ³“¡”a.\n", 0);
   puts_han("<Example>");
   puts_han("strcpy(string, \"adv_gets_han() · ¯¡Ðñ\");");
   puts_han("adv_gets_han(10, 10, string, 30, FONT_NORMAL,\"\");");
   wait(type_any_key);

   draw_box_han(8, 9, 42, 11, BOX_H1V1);
   strcpy(string, "adv_gets_han() · ¯¡Ðñ");
   /* ·¡ ¦¦…µA¬á ¸åµb¥e® adv_gets_han_index · ˆt·i ¤aŽ¡e Äá¬á· */
   /* ÂAÁ¡ ¶áÃ¡Ÿi ¤aŽ‰ ® ·¶”a.                                     */
   adv_gets_han(10, 10, string, 30, FONT_NORMAL, "");
   /* (10, 10) · ¶áÃ¡µA¬á string µA ÂA” 30 ˆ· ¢…¸aŸi FONT_NORMAL */
   /* · ­¢¬÷·a¡ ·³b¤h“e”a. exit_key µA NULL ¢…¸aµi·i »¡¸÷Ð–·a£a¡ */
   /* ¬a¶w¸a¸÷· ÈiÂ‰Ç¡“e ´ô”a                                       */

   xputs_han(0, 15, "·³b–E ¢…¸aµi·e ”a·q‰Á ˆ{¯s“¡”a.");
   aputs_han(0, 16, string, FONT_INVERSE);
   wait(type_any_key);
}

void show_keyflag(void)
/* Ç¡¥¡—a Ïiœ‹aŸi ”až“e ¤w¤ó·i ¥¡µa º…”a. */
{
   unsigned char keyflag, org_keyflag;

   adv_puts_han("<Ðe> µA“e Ç¡¥¡—a Ïiœ‹aŸi ”až“e Ðq®ˆa 4 ˆa»¡ ·¶¯s“¡”a. "
                "”a·q·e ·¡ Ðq®—i·i ¬a¶wÐaµa Ç¡¥¡—a Ïiœ‹a· ˆt·i ´ia º"
                "‹¡•¡ Ða‰¡ ¤aŽ‹¡•¡ Ða“e ¤w¤ó·i ¥¡µa º“e µ·³“¡”a.\n", 0);
   wait(type_any_key);

   /* Ñe¸· Ç¡Ïiœ‹aŸi ¸á¸wÐe”a */
   org_keyflag = get_keyflag_han();

   puts_han("Ñe¸ Ç¡¥¡—a Ïiœ‹a· ¬wÈ“e ”a·q‰Á ˆ{¯s“¡”a.");
   puts_han("”a·q ”e‰¡ ñ´áˆaa¡e ¶E½¢ ®áÏaËaÇ¡µÁ µ¡Ÿe½¢ "
	    "®áÏaËaÇ¡Ÿi ÐqA ’Ÿa­A¶a..");
   puts_han("\n<Ins mode> <CapsLock> <NumLock> <ScrlLock> "
	     "<Alt> <Ctrl> <LeftShift> <RightShift>");
   while(1)
   {
      keyflag = get_keyflag_han();
      /* <LeftShift> Ç¡µÁ <RightShift> Ç¡ˆa •·¯¡µA ’‰œá»¡¡e */
      /* žÏaŸi ¨a¹a aˆe”a.				    */
      if(is_keyflag_han(LEFT_SHIFT | RIGHT_SHIFT)) break;
      gotoxy_han(0, 8);
      /* Ç¡Ïiœ‹a· ˆb §¡ËaŸi Îa¯¡Ðe”a */
      /* <INS> Ç¡ˆa ’‰v“e»¡ ˆñ¬aÐaa¡e INS_MODE µÁ §¡Ëa AND */
      /* Ð´¡ ´i ® ·¶”a. INS µÁ §¡Ëa AND Ða¡e ´e –E”a       */
      printf_han("%5d %10d %9d %10d %8d %6d %8d %12d",
		 (keyflag & INS_MODE)    ? 1 : 0,
		 (keyflag & CAPS_LOCK)   ? 1 : 0,
		 (keyflag & NUM_LOCK)    ? 1 : 0,
		 (keyflag & SCRL_LOCK)   ? 1 : 0,
		 (keyflag & ALT)         ? 1 : 0,
		 (keyflag & CTRL)        ? 1 : 0,
		 (keyflag & LEFT_SHIFT)  ? 1 : 0,
		 (keyflag & RIGHT_SHIFT) ? 1 : 0);
   }

   puts_han("\n\n<NumLock>, <CapsLock>, <ScrlLock> Ç¡ˆa ¡¡– "
	    "’‰œá»¥ ˆõÀáœñ  e—i´á ¥¡‰V¯s“¡”a.");
   set_keyflag_han(NUM_LOCK | CAPS_LOCK | SCRL_LOCK);
   puts_han("”a·q ”e‰¡ ñ´áˆaa¡e ¶E½¢ ®áÏaËaÇ¡µÁ µ¡Ÿe½¢ "
	    "®áÏaËaÇ¡Ÿi ÐqA ’Ÿa­A¶a..");
   /* Ç¡Ïiœ‹aŸi Á¡‹¡ÑÁÐe”a */
   while(!is_keyflag_han(LEFT_SHIFT || RIGHT_SHIFT)) ;

   puts_han("\nÇ¡¥¡—aµA ·¶“e œ‘ÏaŸi Œq¨bˆáa ¥¡‰V¯s“¡”a.");
   puts_han("”a·q ”e‰¡ ñ´áˆaa¡e ¶E½¢ ®áÏaËaÇ¡µÁ µ¡Ÿe½¢ "
	    "®áÏaËaÇ¡Ÿi ÐqA ’Ÿa­A¶a..");
   /* Ç¡Ïiœ‹aŸi Á¡‹¡ÑÁÐe”a */
   set_keyflag_han(0);
   /* œ‘Ïaˆa ·¶“e §¡ËaŸi ‰­¢ É¡‹iÐe”a */
   while(!is_keyflag_han(LEFT_SHIFT || RIGHT_SHIFT))
   {
      toggle_keyflag_han(NUM_LOCK);
      delay(300);
      toggle_keyflag_han(CAPS_LOCK);
      delay(300);
      toggle_keyflag_han(SCRL_LOCK);
      delay(300);
   }

   /* Ç¡Ïiœ‹aŸi ¥¢ŠÐe”a */
   set_keyflag_han(org_keyflag);
   clear_key_buff();
}

void read_file(void)
/* ÑÁ·©·i ·ª´á¬á ÑÁ¡eµA Îa¯¡Ð º“e ¤w¤ó·i ¥¡µa º…”a. */
{
   FILE *stream;
   char text[MAX_STRLEN+1], filename[81], quit, key, han_code;

   adv_puts_han("Ðe‹i¡ –E ÑÁ·©·i ·ª·aa¡e ‹a ÑÁ·©· Ðe‹i Å¡—aŸi ´i´a´¡ "
                "Ðs“¡”a.  e´¢ ¹¡ÐsÑw ·¡¶A· Ðe‹i¡ ¸b¬÷–A´ö”a¡e ¹¡ÐsÑw Ðe"
                "‹i Å¡—a¡ ¥eÑÅÐ´¡ <Ðe> · Â‰b Ðq®¡ Â‰bÐi ® ·¶“e•A "
                "·¡ ˜“e <Ðe> · Ðe‹i Å¡—a ¥eÑÅ Ðq® convert_han_code() Ÿi "
                "¬a¶wÐa¡e –S“¡”a.\n", 0);
   puts_han("<Example>");
   puts_han("convert_han_code(string, KSC5601, JOHAP);");
   puts_han("puts_han(string);\n");
   adv_puts_han("”a·q·e convert_han_code() Ÿi ¬a¶wÐaµa µaœáˆa»¡ Ðe‹i Å¡—a¡ "
                "¸b¬÷–E ÑÁ·©·i ·ª“e µ·³“¡”a. ÑÁ·©· ¶w·¡ Îa¯¡–A“e •¡º—µA "
                "ÑÁ¡e·i ¸q¯¡  ñÂa¡e ´a¢ Ç¡a ’Ÿa‰¡, º—»¡Ðaa¡e <Esc> Ç¡Ÿi "
                "’Ÿa­A¶a.\n", 0);
   adv_puts_han(" å¸á —¡BÉ¡Ÿ¡ ¤b¯a Ðq®Ÿi ¬a¶wÐaµa ¥¡‰¡¯¼·e Ðe‹i ÑÁ·©· "
                "·¡Ÿq·i ·³b¤h‰V¯s“¡”a.\n", 0);
   wait(type_any_key);

   set_vbcolor_han(BLACK, LIGHTGRAY);
   key = dir_box_short((system_maxx - 58) / 2, 5, "*.c", filename);
   if(key != ENTER) return;
   set_vbcolor_han(base_color, base_bkcolor);

   if((stream = fopen(filename,"rt")) == NULL)
   {
      play("mfl16gfedc8");
      printf_han("<%s> ÑÁ·©·¡ ´ô¯s“¡”a.\n", filename);
      wait(type_any_key);
      return;
   }
   cputs_han("·¡ ÑÁ·©· Ðe‹i Å¡—a“e?(1:¹¡ÐsÑw, 2:µÅ¬÷Ñw, 3:À÷‰Àå Ðe‹i) -- ");
   while(strchr("123", key = getch_han()) == NULL) ;
   play("o4e32");
   eputch_han(get_curx_han(), get_cury_han(), key);
   switch(key)
   {
      case '1' : han_code = JOHAP;   break;
      case '2' : han_code = KSC5601; break;
      case '3' : han_code = BIT7;    break;
   }

   quit = 0;
   set_attr_han(FONT_SHADE);
   printf_han("\n\n\n”a·q·e <%s> ÑÁ·©· ¶w·³“¡”a.\n\n", filename);
   set_attr_han(FONT_NORMAL);
   while(!feof(stream) && !quit)
   {
      if(fgets(text, MAX_STRLEN, stream) == NULL)
	 break;
      else
      {
         /* ÑÁ·©· Ðe‹iÅ¡—aˆa ¹¡ÐsÑw·¡ ´a“¡¡e ¹¡ÐsÑw·a¡ ¥eÑÅÐe Ò Â‰b */
         if(han_code != JOHAP) convert_han_code(text, han_code, JOHAP);
	 cputs_han(text);
      }
      if(kbhit())
      {
         if((key = getch_han()) == 0) key = getch_han();
	 if(key == _ESC) quit = 1; else wait("´a¢ Ç¡a ’Ÿa¡e ‰­¢–S“¡”a.");
      }
   }
   fclose(stream);

   set_attr_han(FONT_SHADE);
   if(quit)
      puts_han("\nÑÁ·©¥¡‹¡Ÿi º—”eÐs“¡”a.");
   else
      puts_han("\nÑÁ·©· {·³“¡”a.");
   set_attr_han(FONT_NORMAL);
   wait(type_any_key);
}
