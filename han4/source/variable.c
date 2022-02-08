/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          global variables                                    */
/*   FILENAME           variable.c                                          */
/*   DATE & TIME        05/27/92(MON) 10:30                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#ifndef __LARGE__
#error Not compiled in the large memory model.
#endif
/* If the source text is compiled in any other memory models */
/* except for the large model, issues a fatal error message  */

#include "keydef.h"

/****************************************************************************/
/*                             Macro Constants                              */
/****************************************************************************/

/********************* Macro constants of return value **********************/

/* return codes of init_han() */
enum
{
   OK_HAN,
   MEMORY_NOT_ENOUGH,
   HAN_FONT_NOT_FOUND,
   ENG_FONT_NOT_FOUND,
   SPC_FONT_NOT_FOUND,
   HAN_FONT_READ_ERROR,
   ENG_FONT_READ_ERROR,
   SPC_FONT_READ_ERROR,
   INVALID_GR_CARD,
   INITGRAPH_ERROR,
   ETC_ERROR
};

/****************** Macro constants for the screen buffer *******************/

/*  the structure of the screen buffer                                      */
/*  1st byte : ascii code                                                   */
/*  2nd byte : identifier                                                   */
/*  3rd byte : attribute(256 attributes)                                    */
/*  4th byte : MS 4 bits = background color   LS 4 bits = foreground color  */

/* indenfiers */
#define ENG                     0x00    /* ENGlish                       */
#define HAN1                    0x01    /* HANgul 1st byte               */
#define HAN2                    0x02    /* HANgul 2nd byte               */
#define EDGE                    0x03    /* space to mark adjusting space */

/* attributes  */
#define FONT_NORMAL             0x00
#define FONT_BOLD               0x01
#define FONT_OUTLINE            0x02
#define FONT_SHADOW             0x04
#define FONT_SHADE              0x08
#define FONT_UNDERLINE          0x10
#define FONT_INVERSE            0x20
#define FONT_VERT_DBL           0x40
#define FONT_HORIZ_DBL          0x80

/* colors */
#if     !defined(__COLORS)
#define __COLORS
enum
{
   BLACK,                  /* dark colors */
   BLUE,
   GREEN,
   CYAN,
   RED,
   MAGENTA,
   BROWN,
   LIGHTGRAY,
   DARKGRAY,               /* light colors */
   LIGHTBLUE,
   LIGHTGREEN,
   LIGHTCYAN,
   LIGHTRED,
   LIGHTMAGENTA,
   YELLOW,
   WHITE
};
#endif

/* screen buffer is an array of 90 columns x 30 lines x 4 bytes    */
/* you should not override this, or may meet some serious problems */
#define LIMITX_HAN              90      /* 720/8  (For HGC 720x348) */
#define LIMITY_HAN              30      /* 480/16 (For VGA 640x480) */

/********************** Miscellaneous macro constants  **********************/

/* number of cho, jung, jong, and english */
#define NO_HAN1                 19      /* number of chosungs         */
#define NO_HAN2                 21      /* number of jungsungs        */
#define NO_HAN3                 27      /* number of jongsungs        */
#define NO_ENG                  256     /* number of ASCII characters */

/* number of font series */
#define NO_BUL1                 8       /* 8 series of chosung fonts  */
#define NO_BUL2                 4       /* 4 series of jungsung fonts */
#define NO_BUL3                 4       /* 4 series of jongsung fonts */

/* number of bytes needed to store one character's image */
#define HAN_FONT_SIZE           32      /* 16x16 bits, for hangul     */
#define ENG_FONT_SIZE           16      /*  8x16 bits, for ASCII char */

#define SPC_ID                  212     /* 1st byte of special character */

#define VGA_SCR_WIDTH 		80	/* VGA screen width(in column) */

/* cursor shape */
enum
{
   BIG_CURSOR,
   SMALL_CURSOR,
   BOX_CURSOR,
   USER_DEFINED_CURSOR
};

/* keyboard type */
enum
{
   BUL2_TYPE,
   BUL3_TYPE
};

/* maximum stack size */
#define MAX_STACK	10

/* maximum acceptable string length */
#define MAX_STRLEN      127

#ifndef NULL
#   define      NULL    	0L
#endif

/****************************************************************************/
/*                               Data Types                                 */
/****************************************************************************/

typedef unsigned char mini;

/****************************************************************************/
/*                            External Variables                            */
/****************************************************************************/

/********************** Pointers which point to the fonts *******************/

/* pointers to current fonts */
mini (*han1_font)[NO_HAN1+1][HAN_FONT_SIZE],       /* chosung fonts   */
     (*han2_font)[NO_HAN2+1][HAN_FONT_SIZE],       /* jungsung fonts  */
     (*han3_font)[NO_HAN3+1][HAN_FONT_SIZE],       /* jongsung fonts  */
     (*eng_font)[ENG_FONT_SIZE],                   /* ASCII fonts     */
     (*spc_font)[HAN_FONT_SIZE],                   /* special fonts   */
     or_font[HAN_FONT_SIZE];                       /* for orred fonts */

/* backup of pointers */
mini (*org_han1_font)[NO_HAN1+1][HAN_FONT_SIZE],   /* chosung fonts   */
     (*org_han2_font)[NO_HAN2+1][HAN_FONT_SIZE],   /* jungsung fonts  */
     (*org_han3_font)[NO_HAN3+1][HAN_FONT_SIZE],   /* jongsung fonts  */
     (*org_eng_font)[ENG_FONT_SIZE],               /* ASCII fonts     */
     (*org_spc_font)[HAN_FONT_SIZE];               /* special fonts   */

/**************** Pointer which points to the screen buffer *****************/

mini (*scr_buff)[LIMITX_HAN][4];

/*********************** Variables about the cursor *************************/

/* array that contains the shape of cursor */
mini han_cursor[1+HAN_FONT_SIZE],     /* first bytes = cursor size of x */
     eng_cursor[1+ENG_FONT_SIZE];
mini *cursor_han;                     /* current cursor shape */

/* exist_cursor  : 1, if cursor is on the screen         */
/* draw_cursor   : 1, if function getch draws the cursor */
char exist_cursor, draw_cursor = 1;

/********************* Variables about the video systems ********************/

unsigned video_seg;               /* segment of video RAM                   */
char in_vga_system;               /* 1, if current adapter is EGA/VGA       */
char stay_in_gr_mode;             /* 1 means that no screen mode conversion */
				  /* occurs in init_han(), close_han()      */

/* abs_max?_han : the maximum x,y coordinates of current graphic mode */
/* max?_han     : the maximum x,y coordinates of current window       */
/* min?_han     : the minimum x,y coordinates of current window       */
/* cur?_han     : current x,y coordinates                             */
int abs_maxx_han, abs_maxy_han;
int minx_han, maxx_han, miny_han, maxy_han;
int curx_han, cury_han;
int output_offset_han;                /* starting line number(offset) */

/* attr_han : current attribute */
mini attr_han;
/* color_han   : foreground color */
/* bkcolor_han : background color */
mini color_han = WHITE, bkcolor_han;

char is_status_on_han;         /* 1, if status line is on the screen        */

/* gets_han() can get string of length below <max_strlen_han> */
int max_strlen_han = MAX_STRLEN;
int cursor_twinkle_time = 7;/* cursor twinkle time by number of clock ticks */

/**************************** Other variables *******************************/

char *font_path_han = "";                       /* path of font file        */
char *bgi_path_han  = "";                       /* path of BGI file         */
struct						/* hanja buffer             */
{
   unsigned code;
   char font[HAN_FONT_SIZE];
} *hanja_buff;
unsigned hanja_buff_ptr  = 0;                   /* hanja buffer pointer     */
unsigned hanja_buff_size = 64;                  /* size of hanja buffer     */
