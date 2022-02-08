/*
 * A configuration utility was written to customize the tde executable file.
 * You only need one file to run tde - the executable.  No configuration files
 * to worry about.
 *
 * With this version, there is no need to figure the offsets each time
 * tde.exe is modified.  The original program to find the offsets was written
 * by Jim Lee, jlee@ece.orst.edu.  Simple pattern matching machines are used
 * to find the offsets.  On one pass thru tde.exe, the machines will quickly
 * find all signatures.  The pattern matching machines are loosely based on
 * the multiple string search algorithm by Alfred Aho and Margaret Corasick.
 *
 * See:
 *
 *   Alfred V. Aho and Margaret J. Corasick, "Efficient String Matching:
 *    An Aid to Bibliographic Search."  _Communications of the ACM_ 18
 *    (No. 6): 333-340, 1975.
 *
 *
 * Program name:  tdecfg
 * Author:        Frank Davis
 * Date:          October 5, 1991
 * Date:          June 5, 1993
 *
 * This program is released into the public domain.  You may distribute
 * it freely, Frank Davis.
 */


#include <bios.h>
#include <dos.h>
#include <io.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tdecfg.h"


struct vcfg cfg;                /* video stuff */
FILE *tde_exe;                  /* FILE pointer to tde.exe */

long sort_offset;
long mode_offset;
long color_offset;
long macro_offset;
long keys_offset;
long two_key_offset;
long help_offset;

struct screen cfg_choice[] = {
   {5,25,"1.  Change colors" },
   {7,25,"2.  Redefine keys" },
   {9,25,"3.  Install new help screen" },
  {11,25,"4.  Set default modes" },
  {13,25,"5.  Install permanent macro file" },
  {15,25,"6.  Read in a configuration file" },
  {17,25,"7.  Exit" },
 {20,20,"Please enter choice: " },
  {0,0,NULL}
};


char *greatest_composer_ever = "W. A. Mozart, 1756-1791";


/*
 * Name:    main
 * Date:    October 5, 1991
 * Notes:   Strategy is fairly straight forward -  1) initialize all the
 *          variables  2) show the user a color sample  3) make the changes
 *          permanent if desired.
 */
void main( int argc, char *argv[] )
{
int  rc;
int  c;
char fname[82];
char *buff;

   /*
    * lets get a 8k buffer for our pattern matching machines.
    */
   if ((buff = malloc( 8200 )) == NULL) {
      puts( "\nNot enough memory." );
      exit( 1 );
   }

   puts( "\nEnter tde executable file name (<Enter> = \"tde.exe\")  :" );
   gets( fname );

   if (strlen(fname) == 0)
      strcpy( fname, "tde.exe" );

   if ((rc = access( fname, EXIST )) != 0) {
      puts( "\nFile not found." );
      exit( 1 );
   } else if ((tde_exe = fopen( fname, "r+b" )) == NULL ) {
      puts( "\nCannot open executable file." );
      exit( 2 );
   }
   find_offsets( buff );
   free( buff );

   video_config( );
   cls( );
   show_box( 0, 0, cfg_choice, NORMAL );
   for (rc=0; rc != 1;) {
      xygoto( 42, 20 );
      c = getkey( );
      while (c != '1' && c != '2' && c != '3' && c != '4' && c != '5' &&
             c != '6' && c != '7')
         c = getkey( );
      switch (c) {
         case '1' :
            tdecolor( );
            show_box( 0, 0, cfg_choice, NORMAL );
            break;
         case '2' :
            tdekeys( );
            show_box( 0, 0, cfg_choice, NORMAL );
            break;
         case '3' :
            tdehelp( );
            show_box( 0, 0, cfg_choice, NORMAL );
            break;
         case '4' :
            tdemodes( );
            show_box( 0, 0, cfg_choice, NORMAL );
            break;
         case '5' :
            tdemacro( );
            show_box( 0, 0, cfg_choice, NORMAL );
            break;
         case '6' :
            tdecfgfile( );
            show_box( 0, 0, cfg_choice, NORMAL );
            break;
         case '7' :
            rc = 1;
            break;
      }
   }
   fcloseall( );
   puts( " " );
   puts( " " );
}


/***********************  original comments  *************************/
/*
** OFFSETS.C    -       Automatically scan tde.exe for config offsets
**
** Author:  Jim Lee (jlee@ece.orst.edu)
**   Date:  5/12/93
** Status:  Released to the public domain
**
**      This little utility takes the drudgery out of updating tdecfg.h
**      every time you re-compile tde.exe.  Just remove the hard-coded
**      offsets in tdecfg.h and replace them with '#include "newoff.h"'.
**      Then run 'offsets tde.exe > newoff.h'.  Now re-compile tdecfg
**      and you're done!
**
*/
/*****************************   end   *******************************/

/*
 * Name:    find_offsets
 * Date:    June 5, 1993
 * Notes:   to increase the speed, I "wired" a pattern matching machine
 *           for each of the signatures, Frank.  the states in the machines
 *           correspond to the characters in the sig's.  on one pass through
 *           the file, all 7 signatures will be found.  all signatures are
 *           8 characters long.  it's also a little faster if we read the file
 *           in big chunks.  also note that we never have to back-up or reread
 *           the file.
 */
void find_offsets( char *buff )
{
long off;
int  m1, m2, m3, m4, m5, m6, m7;
char sig1[8] = { '\x00','\x01','\x02','\x03','\x04','\x05','\x06','\x07' };
char sig2[8] = "$ modes";
char sig3[8] = "$colors";
char sig4[8] = "$macbuf";
char sig5[8] = "$  keys";
char sig6[8] = "$twokey";
char sig7[8] = "$  help";
unsigned int cnt;
register char *b;

   /*
    * Let's start the machines 100k into the executable.
    */
   m1 = m2 = m3 = m4 = m5 = m6 = m7 = 0;
   off = 100000L;
   fseek( tde_exe, off, SEEK_SET );
   while (!feof( tde_exe )) {
      cnt = fread( buff, sizeof(char), 8192, tde_exe );
      b = (char *)buff;
      for (; cnt > 0; off++, cnt--, b++) {
         if (m1 < 8) {
            m1 =  sig1[m1] == *b ? m1+1 : 0;
            if (m1 == 8)
               sort_offset = off - 7L;
         }
         if (m2 < 8) {
            m2 =  sig2[m2] == *b ? m2+1 : 0;
            if (m2 == 8)
               mode_offset = off - 7L;
         }
         if (m3 < 8) {
            m3 =  sig3[m3] == *b ? m3+1 : 0;
            if (m3 == 8)
               color_offset = off - 7L;
         }
         if (m4 < 8) {
            m4 =  sig4[m4] == *b ? m4+1 : 0;
            if (m4 == 8)
               macro_offset = off - 7L;
         }
         if (m5 < 8) {
            m5 =  sig5[m5] == *b ? m5+1 : 0;
            if (m5 == 8)
               keys_offset = off - 7L;
         }
         if (m6 < 8) {
            m6 =  sig6[m6] == *b ? m6+1 : 0;
            if (m6 == 8)
               two_key_offset = off - 7L;
         }
         if (m7 < 8) {
            m7 =  sig7[m7] == *b ? m7+1 : 0;
            if (m7 == 8)
               help_offset = off - 7L;
         }
      }
   }
}


/*
 * Name:    xygoto
 * Date:    July 21, 1991
 * Notes:   Use the video interrupt to set the cursor.
 */
void xygoto( int col, int row )
{
union REGS inregs, outregs;

   inregs.h.ah = 2;
   inregs.h.bh = 0;
   inregs.h.dh = row;
   inregs.h.dl = col;
   int86( VIDEO_INT, &inregs, &outregs );
}


/*
 * Name:    video_config
 * Date:    July 21, 1991
 * Notes:   See main.c for more info.
 */
void video_config( void )
{
#pragma pack( 1 )    /* Use pragma to force packing on byte boundaries. */

struct LOWMEMVID
{
   char     vidmode;           /* 0x449 */
   unsigned scrwid;            /* 0x44A */
   unsigned scrlen;            /* 0x44C */
   unsigned scroff;            /* 0x44E */
   struct   LOCATE
   {
      unsigned char col;
      unsigned char row;
   } csrpos[8];                /* 0x450 */
   struct   CURSIZE
   {
      unsigned char end;
      unsigned char start;
   } csrsize;                  /* 0x460 */
   char      page;             /* 0x462 */
   unsigned  addr_6845;        /* 0x463 */
   char      crt_mode_set;     /* 0x465 */
   char      crt_palette;      /* 0x466 */
   char      system_stuff[29]; /* 0x467 */
   char      rows;             /* 0x484 */
   unsigned  points;           /* 0x485 */
   char      ega_info;         /* 0x487 */
   char      info_3;           /* 0x488 */
} vid;
struct LOWMEMVID _far *pvid = &vid;

#pragma pack( )    /* revert to previously defined pack pragma. */

union REGS in, out;
unsigned char temp, active_display;

   /* Move system information into uninitialized structure variable. */
   movedata( 0, 0x449, FP_SEG( pvid ), FP_OFF( pvid ), sizeof( vid ) );

   cfg.rescan = FALSE;
   in.x.ax =  0x1a00;
   int86( VIDEO_INT, &in, &out );
   temp = out.h.al;
   active_display = out.h.bl;
   if (temp == 0x1a && (active_display == 7 || active_display == 8))
      cfg.adapter = VGA;
   else {
      in.h.ah =  0x12;
      in.h.bl =  0x10;
      int86( VIDEO_INT, &in, &out );
      if (out.h.bl != 0x10) {         /* EGA */
         if (vid.ega_info & 0x08) {
            if (vid.addr_6845 == 0x3d4)
               cfg.adapter = CGA;
            else
               cfg.adapter = MDA;
         } else
            cfg.adapter = EGA;
      } else if (vid.addr_6845 == 0x3d4)
         cfg.adapter = CGA;
      else
         cfg.adapter = MDA;
   }

   if (cfg.adapter == CGA)
      cfg.rescan = TRUE;

   cfg.mode = vid.vidmode;
   if (vid.addr_6845 == 0x3D4) {
      cfg.color = TRUE;
      FP_SEG( cfg.videomem ) = 0xb800;
   } else {
      cfg.color = FALSE;
      FP_SEG( cfg.videomem ) = 0xb000;
   }
   FP_OFF( cfg.videomem ) = 0x0000;
   if (cfg.color == TRUE)
      cfg.attr = COLOR_ATTR;
   else
      cfg.attr = MONO_ATTR;

   cfg.overscan = vid.crt_palette;
}


/*
 * Name:    getkey
 * Date:    July 21, 1991
 * Notes:   Waits for keyboard input and returns the key pressed by the user.
 */
int getkey( void )
{
unsigned key;
unsigned lo;

   /*
    *  _bios_keybrd == int 16.  It returns the scan code in ah, hi part of key,
    *  and the ascii key code in al, lo part of key.
    */
   key = _bios_keybrd( 0 );
   lo = key & 0X00FF;
   lo = (int)((lo == 0) ? (((key & 0XFF00) >> 8) + 256) : lo);
   return( lo );
}


/*
 * Name:    s_output
 * Date:    July 21, 1991
 * Passed:  s:     the string to display
 *          line:  line number to begin display
 *          col:   column number to begin display
 *          attr:  color to display string
 * Notes:   See tdeasm.c for more info
 */
void s_output( char far *s, int line, int col, int attr )
{
int far *screen_ptr;
int max_col;
int off;

   max_col = 80;
   screen_ptr = cfg.videomem;
   off = line * 160 + col * 2;

   ASSEMBLE {
        push    ds              /* MUST save ds */
        push    di              /* save di on stack */
        push    si              /* save si on stack */

        mov     bx, WORD PTR attr               /* keep attribute in bx */
        mov     cx, WORD PTR col                /* put cols in cx */
        mov     dx, WORD PTR max_col            /* keep max_col in dx */
        mov     di, WORD PTR screen_ptr         /* load offset of screen ptr */
        add     di, WORD PTR off
        mov     ax, WORD PTR screen_ptr+2       /* load segment of screen ptr */
        mov     es, ax
        mov     si, WORD PTR s  /* load offset of string ptr */
        or      si, si          /* is it == NULL? */
        je      getout          /* yes, no output needed */
        mov     ax, WORD PTR s+2        /* load segment of string ptr */
        or      ax, ax          /* is pointer == NULL? */
        je      getout          /* yes, no output needed */
        mov     ds, ax          /* load segment of text in ds */
        mov     ah, bl          /* put attribute in AH */
   }
top:

   ASSEMBLE {
        cmp     cx, dx          /* col < max_cols? */
        jge     getout          /* no, thru with line */
        lodsb                   /* get next char in string - put in al */
        or      al, al          /* is it '\0' */
        je      getout          /* yes, end of string */
        stosw                   /* else show attr + char on screen (ah + al) */
        inc     cx              /* col++ */
        jmp     SHORT top       /* get another character */
   }
getout:

   ASSEMBLE {
        pop     si              /* get back si */
        pop     di              /* get back di */
        pop     ds              /* get back ds */
   }
}


/*
 * Name:    hlight_line
 * Date:    July 21, 1991
 * Passed:  x:     column to begin hi lite
 *          y:     line to begin hi lite
 *          lgth:  number of characters to hi lite
 *          attr:  attribute color
 * Notes:   The attribute byte is the hi byte.
 */
void hlight_line( int x, int y, int lgth, int attr )
{
int off, far *pointer;

   pointer = cfg.videomem;
   off = y * 160 + 2 * x + 1;  /* add one - so it points to attribute byte */
   ASSEMBLE {
        push    di              /* save es */

        mov     cx, lgth        /* number of characters to change color */

        mov     di, WORD PTR pointer    /* get destination - video memory */
        add     di, off                 /* add offset */
        mov     ax, WORD PTR pointer+2
        mov     es, ax
        mov     ax, attr        /* attribute */
   }
lite_len:

   ASSEMBLE {
        stosb                   /* store a BYTE */
        inc     di              /* skip over character to next attribute */
        loop    lite_len        /* change next attribute */
        pop     di              /* restore di */
   }
}


/*
 * Name:    scroll_window
 * Date:    October 5, 1991
 * Passed:  lines:  number of lines to scroll
 *          r1:     row scroll starts on
 *          c1:     column scroll start on
 *          r2:     ending row of scroll
 *          c2:     ending column of scroll
 *          attr:   color of scroll
 * Notes:   Clear a window using color.  Use standard BIOS call.  See
 *          any technical reference for more info on VIDEO BIOS services.
 */
void scroll_window( int lines, int r1, int c1, int r2, int c2, int attr )
{
char rah, ral;

   ASSEMBLE {
        mov     ax, lines
        cmp     ax, 0           /* if line < 0  - scroll window down */
        jge     a1

        neg     ax                      /* make lines positive */
        mov     BYTE PTR ral, al        /* save number of lines */
        mov     BYTE PTR rah, 7         /* function 7 -  scroll window down */
        dec     r2                      /* decrement row 2 */
        jmp     SHORT a2
   }
a1:

   ASSEMBLE {
        mov     BYTE PTR ral, al        /* number of lines to scroll */
        mov     BYTE PTR rah, 6         /* function 6 - scroll window up */
   }
a2:

   ASSEMBLE {
        mov     ax, WORD PTR r1         /* get starting row */
        mov     ch, al                  /* put it in ch */
        mov     ax, WORD PTR c1         /* get starting column */
        mov     cl, al                  /* put it in cl */
        mov     ax, WORD PTR r2         /* get ending row */
        mov     dh, al                  /* put it in dh */
        mov     ax, WORD PTR c2         /* get ending column */
        mov     dl, al                  /* put it in cl */
        mov     ax, WORD PTR attr       /* get attribute */
        mov     bh, al                  /* put it in bh */
        mov     ah, BYTE PTR rah        /* get function number */
        mov     al, BYTE PTR ral        /* get number of lines */
        push    bp                      /* ** in some early BIOS versions ** */
        int     VIDEO_INT               /* ** u must save bp              ** */
        pop     bp
   }
}


/*
 * Name:    cls
 * Date:    October 5, 1991
 * Notes:   Clear screen using color.
 */
void cls( void )
{
   scroll_window( 0, 0, 0, 24, 79, NORMAL );
}


/*
 * Name:    show_box
 * Date:    October 5, 1991
 * Passed:  x:     number of lines to scroll
 *          y:     row scroll starts on
 *          p:     column scroll start on
 *          attr:  ending row of scroll
 * Notes:   Easy way to show text on screen.  Based on the display techiniques
 *          in a very good morse code practice program, sorry I don't know
 *          the author's name.  Several morse code pratice programs have been
 *          written, but I am refering to the one written in C and released
 *          around 1984-1986.  Seems like the author was living in California
 *          at that time.
 */
void show_box( int x, int y, struct screen *p, int  attr )
{

   while (p->text) {
      s_output( p->text, p->row+y, p->col+x, attr );
      p++;
   }
}


/*
 * Name:    make_window
 * Date:    October 5, 1991
 * Passed:  col:     upper left column to begin window
 *          row:     upper left row to begin window
 *          width:   number of columns in window
 *          height:  number of rows in window
 *          attr:    color of window border
 * Notes:   Draw the outline of the window then clear all contents.
 *          The begin and ending column have to incremented so when the
 *          window is cleared the borders are not wiped out.
 */
void make_window( int col, int row, int width, int height, int attr )
{
   buf_box( col++, row++, width, height, attr );
   clear_window( col, row, width-2, height-2 );
}


/*
 * Name:    buf_box
 * Date:    October 5, 1991
 * Passed:  col:     upper left column to begin window
 *          row:     upper left row to begin window
 *          width:   number of columns in window
 *          height:  number of rows in window
 *          attr:    color of window border
 * Notes:   Draw the outline of the window.  See tdecfg.h for the outline
 *          characters.
 */
void buf_box( int col, int row, int width, int height, int attr )
{
int  i;
int  row_count;
char string[82];

   if (height > 0 && width > 0 && height < 25 && width < 81) {
      row_count = 1;
      string[0]= U_LEFT;
      for (i=1; i<width-1; i++)
         string[i] = HOR_LINE;
      string[i++] = U_RIGHT; string[i] = '\0';
      s_output( string, row, col, attr );
      ++row_count;
      ++row;

      if (row_count < height) {
         string[0] = VER_LINE;
         string[1] = '\0';
         for (i=1; i<height-1; i++) {
            s_output( string, row, col, attr );
            s_output( string, row, col+width-1, attr );
            ++row;
            ++row_count;
         }
      }

      if (row_count <= height) {
         string[0] = L_LEFT;
         for (i=1; i<width-1; i++)
            string[i] = HOR_LINE;
         string[i++] = L_RIGHT; string[i] = '\0';
         s_output( string, row, col, attr );
      }
   }
}


/*
 * Name:    clear_window
 * Date:    October 5, 1991
 * Passed:  col:     upper left column to begin window
 *          row:     upper left row to begin window
 *          width:   number of columns in window
 *          height:  number of rows in window
 * Notes:   Clear the insides of the window.
 */
void clear_window( int col, int row, int width, int height )
{
/*   scroll_window( height, row, col, row+height-1, col+width-1, NORMAL ); */
   scroll_window( 0, row, col, row+height-1, col+width-1, NORMAL );
}


/*
 * Name:    window_control
 * Date:    October 5, 1991
 * Passed:  window_ptr: pointer to pointer of window (head of window stack)
 *          action:     are SAVEing or RESTOREing the contents of a window
 *          col:        upper left column of window
 *          row:        upper left row of window
 *          width:      number of characters in window
 *          height:     number of rows in window
 * Notes:   Save or restore the contents of the screen under a pop-up or
 *          pull-down window.  Use a stack to store the windows so an unlimited
 *          number of windows may be saved and restored.
 */
void window_control( WINDOW **window_ptr, int action, int col, int row,
                     int width, int height )
{
WINDOW  *p;
size_t  store_me;

   if (action == SAVE) {
      p = (WINDOW *)malloc( sizeof(WINDOW) );
      if (p != NULL) {
         p->n = NULL;

         /*
          * push a window on the stack
          */
         if (*window_ptr != NULL)
            p->n = *window_ptr;
         *window_ptr = p;
         store_me = (width * height) * sizeof( int );
         p->buf = (int *)malloc( store_me );
         save_window( p->buf, col, row, width, height );
      }
   } else if (action == RESTORE) {
      if (*window_ptr != NULL) {
         p = *window_ptr;
         restore_window( p->buf, col, row, width, height );

         /*
          * pop a window off the stack
          */
         *window_ptr = p->n;
         free( p->buf );
         free( p );
}  }  }


/*
 * Name:    save_window
 * Date:    October 5, 1991
 * Passed:  destination: pointer to store contents of screen
 *          col:         upper left column of window
 *          row:         upper left row of window
 *          width:       number of characters in window
 *          height:      number of rows in window
 * Notes:   Do an optimal save.  Save only the contents of the screen that the
 *          window writes over.  Some algorithms save the entire contents of
 *          the screen - wasteful.
 */
void save_window( int *destination, int col, int row, int width, int height )
{
int i, j, offset;
int far *pointer;

   pointer = cfg.videomem;
   offset = row * 80 + col;
   pointer += offset;
   for (i=0; i < height; i++) {
      for (j=0; j < width; j++)
         *destination++ = *(pointer + j);
      pointer += 80;
   }
}


/*
 * Name:    restore_window
 * Date:    October 5, 1991
 * Passed:  source:      pointer of source of contents of screen
 *          col:         upper left column of window
 *          row:         upper left row of window
 *          width:       number of characters in window
 *          height:      number of rows in window
 * Notes:   Do an optimal restore.  Restore only the contents of the screen
 *          that the window writes over.  Some algorithms restore the entire
 *          contents of the screen - wasteful.
 */
void restore_window( int *source, int col, int row, int width, int height )
{
int i, j, offset;
int far *pointer;

   pointer = cfg.videomem;
   offset = row * 80 + col;
   pointer += offset;
   for (i=0; i < height; i++) {
      for (j=0; j < width; j++)
         *(pointer + j) = *source++;
      pointer += 80;
   }
}
