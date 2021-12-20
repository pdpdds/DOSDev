/*******************  start of original comments  ********************/
/*
 * Written by Douglas Thomson (1989/1990)
 *
 * This source code is released into the public domain.
 */

/*
 * Name:    dte - Doug's Text Editor program - hardware dependent module
 * Purpose: This file contains all the code that needs to be different on
 *           different hardware.
 * File:    hwibm.c
 * Author:  Douglas Thomson
 * System:  This particular version is for the IBM PC and close compatibles.
 *           It write directly to video RAM, so it is faster than other
 *           techniques, but will cause "snow" on most CGA cards. See the
 *           file "hwibmcga.c" for a version that avoids snow.
 *          The compiler is Turbo C 2.0, using one of the large data memory
 *           models.
 * Date:    October 10, 1989
 * Notes:   This module has been kept as small as possible, to facilitate
 *           porting between different systems.
 */
/*********************  end of original comments   ********************/


/*
 * These routines were rewritten for Microsoft C.  They are pretty much system
 * dependent and pretty much Microsoft C dependent.  I also renamed this file
 * "main.c" - easier to find the main function.
 *
 * New editor name:  TDE, the Thomson-Davis Editor.
 * Author:           Frank Davis
 * Date:             June 5, 1991, version 1.0
 * Date:             July 29, 1991, version 1.1
 * Date:             October 5, 1991, version 1.2
 * Date:             January 20, 1992, version 1.3
 * Date:             February 17, 1992, version 1.4
 * Date:             April 1, 1992, version 1.5
 * Date:             June 5, 1992, version 2.0
 * Date:             October 31, 1992, version 2.1
 * Date:             April 1, 1993, version 2.2
 * Date:             June 5, 1993, version 3.0
 *
 * This modification of Douglas Thomson's code is released into the
 * public domain, Frank Davis.  You may distribute it freely.
 */


const char *greatest_composer_ever = "W. A. Mozart, 1756-1791";


#include "tdestr.h"             /* tde types */
#include "common.h"
#include "define.h"
#include "help.h"
#include "tdefunc.h"


#include <dos.h>                /* for renaming files */
#include <bios.h>               /* for direct BIOS keyboard input */
#include <io.h>                 /* for file attribute code */
#include <fcntl.h>              /* open flags */
#if defined( __MSC__ )
   #include <errno.h>
   #include <sys\types.h>       /* S_IWRITE etc */
#endif
#include <sys\stat.h>           /* S_IWRITE etc */

#if defined( __MSC__ )
void (interrupt far *old_control_c)( ); /* variable for old CNTL-C */
void (interrupt far *old_int1b)( );     /* variable for old int 1b */
#endif


/*
 * original control-break checking flag
 */
static int s_cbrk;


/*
 * Name:    main
 * Purpose: To do any system dependent command line argument processing,
 *           and then call the main editor function.
 * Date:    October 10, 1989
 * Passed:  argc:   number of command line arguments
 *          argv:   text of command line arguments
 */
int main( int argc, char *argv[] )
{
#if defined( __MSC__ )
   union REGS inregs, outregs;
#endif

   g_status.found_first = FALSE;
   g_status.arg         = 1;
   g_status.argc        = argc;
   g_status.argv        = argv;

   /*
    * trap control-break to make it harmless, and turn checking off.
    *   trap control-C to make it harmless.
    */
#if defined( __MSC__ )
   inregs.h.ah = 0x33;
   inregs.h.al = 0;
   intdos( &inregs, &outregs );
   s_cbrk = outregs.h.dl;
   old_control_c = _dos_getvect( (unsigned)0x23 );
   _dos_setvect( 0x23, harmless );
   old_int1b = _dos_getvect( (unsigned)0x1b );
   _dos_setvect( 0x1b, ctrl_break );
   inregs.h.ah = 0x33;
   inregs.h.al = 1;
   inregs.h.dl = 0;
   intdos( &inregs, &outregs );
#else
   s_cbrk = getcbrk( );
   ctrlbrk( harmless );
   setcbrk( 0 );
#endif


   /*
    * now, install and initialize our simple Critical Error Handler.
    */
   install_ceh( &ceh );
   ceh.flag = OK;

   initialize( );
   editor( );
   terminate( );
   return(0);
}


/*
 * Name:    error
 * Purpose: To report an error, and usually make the user type <ESC> before
 *           continuing.
 * Date:    June 5, 1991
 * Passed:  kind:   an indication of how serious the error was:
 *                      WARNING: continue after pressing a key
 *                      FATAL:   abort the editor
 *          line:    line to display message
 *          message: string to be printed
 * Notes:   Show user the message and ask for a key if needed.
 */
void error( int kind, int line, char *message )
{
char buff[MAX_COLS+2];          /* somewhere to store error before printing */
char line_buff[(MAX_COLS+2)*2]; /* buffer for char and attribute  */

   /*
    * tell the user what kind of an error it is
    */
   switch (kind) {
      case FATAL:
	 /*
	  * fatal error
	  */
	 assert( strlen( main1 ) < MAX_COLS );
	 strcpy( buff, main1 );
	 break;
     case WARNING:
     default:
	 /*
	  * warning
	  */
	 assert( strlen( main2 ) < MAX_COLS );
	 strcpy( buff, main2 );
	 break;
   }

   /*
    * prepare the error message itself
    */
   strcat( buff, message );

   /*
    * tell the user how to continue editing if necessary
    */
   if (kind == WARNING)
      /*
       * press a key
       */
      strcat( buff, main3 );

   /*
    * output the error message
    */
   save_screen_line( 0, line, line_buff );
   set_prompt( buff, line );

   if (kind == FATAL) {
      /*
       * no point in making the user type <ESC>, since the program is
       *  about to abort anyway...
       */
      terminate( );
      exit( 1 );
   }

   getkey( );
   restore_screen_line( 0, line, line_buff );
   if (g_status.wrapped) {
      g_status.wrapped = FALSE;
      show_search_message( CLR_SEARCH, g_display.mode_color );
   }
}


/*
 * Name:    harmless
 * Purpose: Do nothing when control-C is pressed
 * Date:    June 5, 1991
 * Notes:   Interrupt 23, the Control-C handler, is a MS DOS system function.
 *            Since we want to use Control-C as a regular function key,
 *            let's do absolutely nothing when Control-C is pressed.
 */
#if defined( __MSC__ )
void interrupt far harmless( void )
#else
static int harmless( void )
#endif
{

}


/*
 * Name:    ctrl_break
 * Purpose: Set our control-break flag when control-break is pressed.
 * Date:    June 5, 1992
 * Notes:   Control-break is a little different from Control-C.  When
 *           Control-C is pressed, MS DOS processes it as soon as possible,
 *           which may be quite a while.  On the other hand, when
 *           Control-break is pressed on IBM and compatibles, interrupt 0x1b
 *           is generated immediately.  Since an interrupt is generated
 *           immediately, we can gain control of run-away functions, like
 *           recursive macros, by checking our Control-break flag.
 */
void interrupt far ctrl_break( void )
{
   g_status.control_break = TRUE;
}


/*
 * Name:    terminate
 * Purpose: To free all dynamic structures and unload anything we loaded
 * Date:    June 5, 1991
 */
void terminate( void )
{
union REGS inregs, outregs;
register WINDOW     *wp;        /* register for scanning windows */
WINDOW              *w;         /* free window */
register file_infos *fp;        /* register for scanning files */
file_infos          *f;         /* free files */
int                 i;

   /*
    * restore control-break checking
    */
#if defined( __MSC__ )
   _dos_setvect( 0x1b, old_int1b );
   _dos_setvect( 0x23, old_control_c );
   inregs.h.ah = 0x33;
   inregs.h.al = 1;
   inregs.h.dl = (char)s_cbrk;
   intdos( &inregs, &outregs );
#else
   setcbrk( s_cbrk );
#endif

   /*
    * free the file structures, if not already free.
    */
   fp = g_status.file_list;
   while (fp != NULL) {
      f  = fp;
      fp = fp->next;
      free( f );
   }

   /*
    * free the window structures, if not already free.
    */
   wp = g_status.window_list;
   while (wp != NULL) {
      w  = wp;
      wp = wp->next;
      free( w );
   }


   /*
    * free any character classes in the nfa's.
    */
   for (i=0; i < REGX_SIZE; i++) {
      if (sas_nfa.class[i] == nfa.class[i]  &&  nfa.class[i] != NULL)
	 free( nfa.class[i] );
      else if (sas_nfa.class[i] != NULL)
	 free( sas_nfa.class[i] );
      else if (nfa.class[i] != NULL)
	 free( nfa.class[i] );
   }


   /*
    * reset the cursor size and unload the 83/84 key keyboard utility
    */
   set_cursor_size( mode.cursor_size == SMALL_INS ? g_display.insert_cursor :
						    g_display.overw_cursor );
   if (mode.enh_kbd == FALSE)
      simulate_enh_kbd( 0 );

   /*
    * restore the overscan (border) color
    */
   if (g_display.adapter != MDA)
      set_overscan_color( g_display.old_overscan );
}


/*
 * Name:    initialize
 * Purpose: To initialize all the screen status info that is not hardware
 *           dependent, and call the hardware initialization routine to
 *           pick up the hardware dependent stuff.
 * Date:    June 5, 1991
 * Returns: [g_status and g_display]: all set up ready to go
 * Notes:   It is assumed that g_status and g_display are all \0's to begin
 *           with (the default if they use static storage). If this may
 *           not be the case, then clear them explicitly here.
 */
void initialize( void )
{
int i;

   /*
    * do the hardware initialization first.
    */
   hw_initialize( );

   /*
    * now, initialize the editor modes, pointers, and counters.
    */
   bm.search_defined        = ERROR;
   sas_bm.search_defined    = ERROR;
   g_status.sas_defined     = ERROR;
   g_status.sas_search_type = ERROR;

   regx.search_defined      = ERROR;
   sas_regx.search_defined  = ERROR;

   if (mode.undo_max < 2)
      mode.undo_max = 2;

   g_status.marked_file = NULL;
   g_status.current_window = NULL;
   g_status.current_file = NULL;
   g_status.window_list = NULL;
   g_status.file_list = NULL;
   g_status.buff_node = NULL;

   g_status.window_count = 0;
   g_status.file_count = 0;
   g_status.line_buff_len = 0;
   g_status.tabout_buff_len = 0;
   g_status.command = 0;
   g_status.key_pressed = 0;
   g_status.sas_rcol  = 0;
   g_status.sas_rline = 0;
   g_status.recording_key = 0;

   g_status.key_pending = FALSE;
   g_status.found_first = FALSE;
   g_status.sas_found_first = FALSE;
   g_status.copied = FALSE;
   g_status.wrapped = FALSE;
   g_status.marked = FALSE;
   g_status.macro_executing = FALSE;
   g_status.replace_defined = FALSE;

   g_status.screen_display = TRUE;

   g_status.file_chunk = DEFAULT_BIN_LENGTH;

   g_status.sas_tokens[0] = '\0';
   g_status.path[0] = '\0';
   g_status.sas_path[0] = '\0';
   g_status.rw_name[0] = '\0';
   g_status.pattern[0] = '\0';
   g_status.subst[0] = '\0';


   /*
    * set the number of lines from one page that should still be visible
    *  on the next page after page up or page down.
    */
   g_status.overlap = 1;


   /*
    * initialize the nodes in the nfa.
    */
   for (i=0; i < REGX_SIZE; i++) {
      sas_nfa.node_type[i] = nfa.node_type[i] = 0;
      sas_nfa.term_type[i] = nfa.term_type[i] = 0;
      sas_nfa.c[i] = nfa.c[i] = 0;
      sas_nfa.next1[i] = nfa.next1[i] = 0;
      sas_nfa.next2[i] = nfa.next2[i] = 0;
      sas_nfa.class[i] = nfa.class[i] = NULL;
   }

   /*
    * no macro is executing
    */
   connect_macros( );


   /*
    * clear the screen and show the author's names
    */
   cls( );
   show_credits( );
}


/*
 * Name:    hw_initialize
 * Purpose: To initialize the display ready for editor use.
 * Date:    June 5, 1991
 */
void hw_initialize( void )
{
struct vcfg cfg;       /* defined in .h */
register int *clr;

   /*
    * set up screen size
    */
   g_display.ncols     = MAX_COLS;
   g_display.nlines    = MAX_LINES - 1;
   g_display.mode_line = MAX_LINES;
   g_display.line_length = MAX_LINE_LENGTH;

   /*
    * work out what kind of display is in use, and set attributes and
    *  display address accordingly. Note that this will only work with
    *  close IBM compatibles.
    */

   video_config( &cfg );
   g_display.display_address = (char far *)cfg.videomem;

   /*
    * Use an integer pointer to go thru the color array for setting up the
    * various color fields.
    */
   clr =  cfg.color == FALSE ? &colour.clr[0][0] : &colour.clr[1][0];

   g_display.head_color    = *clr++;
   g_display.text_color    = *clr++;
   g_display.dirty_color   = *clr++;
   g_display.mode_color    = *clr++;
   g_display.block_color   = *clr++;
   g_display.message_color = *clr++;
   g_display.help_color    = *clr++;
   g_display.diag_color    = *clr++;
   g_display.eof_color     = *clr++;
   g_display.curl_color    = *clr++;
   g_display.ruler_color   = *clr++;
   g_display.ruler_pointer = *clr++;
   g_display.hilited_file  = *clr++;
   g_display.overscan      = *clr;

   /*
    * set the overscan color.
    * in terminate( ), the overscan color is returned to old state.
    */
   if (g_display.adapter != MDA)
      set_overscan_color( g_display.overscan );
}


/*
 * Name:    get_help
 * Purpose: save the screen and display key definitions
 * Date:    June 5, 1991
 * Notes:   This routine is dependent on the length of the strings in the
 *          help screen.  To make it easy to load in a new help screen,
 *          the strings are assumed to be 80 characters long followed by
 *          the '\0' character.  It is assumed each that string contains
 *          exactly 81 characters.
 */
int  get_help( WINDOW *window )
{
register char *help;
register int line;

   xygoto( -1, -1 );
   help = help_screen[1];
   for (line=0; help != NULL; line++) {
      s_output( help, line, 0, g_display.help_color );
      help = help_screen[line+2];
   }
   line = getkey( );
   redraw_screen( window );
   return( OK );
}


/*
 * Name:    show_credits
 * Purpose: display authors
 * Date:    June 5, 1991
 */
void show_credits( void )
{
register char *credit;
int  line;

   xygoto( -1, -1 );
   credit = credit_screen[0];
   for (line=0; credit != NULL; ) {
      s_output( credit, line+2, 11, g_display.text_color );
      credit = credit_screen[++line];
   }
}
