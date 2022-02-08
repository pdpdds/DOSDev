/* œa·¡§aœáŸ¡ Ðq®µA ”Ðe ¬å´å */

#include <bios.h>
#include <graphics.h>

#include "hanio.h"
#include "misc.h"
#include "extendio.h"
//#include "hangraph.h"
#include "keydef.h"
#include "pcx.h"

/* ¬a¶w¸a ¸÷· Ðq®µA ”Ðe ¬å´å */
void Make_Text_Title ( void );
void Initialize ( void );
void OpenScreen ( void );
void Opening ( void );
void Draw_Password_Border ( void );
void Judgment_Password ( void );
void Letter_Contents ( void );
void Post_Script ( void );
void Redirect_Function_No_Beep ( char *string );
void Redirect_Function_Beep ( char *string );
void Redirect_Function_Fast ( char *string );
void Eraser_With_Black ( void );
void Eraser_With_Black_End ( void );
void Eraser_With_Color ( int style, int color );
int get_key_scan ( void );
void Ending ( void );
void CloseScreen ( void );

/* ¸åµb ¥e® ¬å´å */
char *gothic, *hand;        /* ‹i¸aÁAµA ”Ðe Í¡·¥Èá Ði”w */
char *screen[80][25];       /* ¯©Ð—¸å· •¡¯aÑÁ¡e ¸á¸w·i ¶áÐe Í¡·¥Èá ¡A¡¡Ÿ¡ Ði”w */
int  current_x, current_y;  /* ¯©Ð—¸å Äá¬á ¶áÃ¡· ‹¡´â */


/* ¡A·¥ Ðq® */
void main ( void )
{
    Make_Text_Title ();
    OpenScreen ();
    Initialize ();
    Opening ();
    Ending ();
}

/* ¯©Ð—¸å •¡¯aÑÁ¡eµA Äéœá ÉB¯aËa œa·¥·a¡ ¹A¡¢·i ³a“e žË¥ */
/* ·¡´¡‹¡ 5.3 —wµA¬á ¥© ® ·¶“e ³i•A´ô“e(?) ´b­A¬áŸ¡       */
void Make_Text_Title ( void )
{
    printf ( "\n\n" );
    textcolor ( WHITE ); textbackground ( BLACK );
    cprintf ( " " ); textbackground ( BLUE );
    cprintf ( "  From.. Oh-joon    " ); textcolor ( YELLOW );
    cprintf ( " < Not a letter, But this is my heart >  " );
    textcolor ( WHITE );  cprintf ( "  To.. My-Girl  " );
    textcolor ( LIGHTGRAY ); textbackground ( BLACK );
    textbackground ( BLACK ); textcolor ( LIGHTGRAY );
    printf ( "\n\n" );
//    delay ( 6000 );
delay ( 60 );
}

/* ‹aœÏ¢ ¡¡—a, Ðe‹i œa·¡§aœáŸ¡µA ”Ðe Á¡‹¡ÑÁ £¹ Í¥Ëa ¡—· Ðq® */
void Initialize ( void )
{
//    int d = VGA, m = VGAHI, errorcode;

    if ( init_han() != OK_HAN )
      {
	 printf ( "Error : %d", init_han() );
	 exit ( 0 );
      }
#if 0
    initgraph ( &d, &m, "" );
    errorcode = graphresult();

    if ( errorcode != grOk )
       {
	  clrscr(); gotoxy(25,6); sound(1000); delay(200); nosound();
	  printf ("[Graphics Mode Setting Error]"); gotoxy(13,10);
	  printf ("Error Code : %s", grapherrormsg(errorcode));
	  gotoxy (25,15); printf("Please Check out your system !");
	  gotoxy (25,18); printf("Press any key to quit...");
	  gotoxy (0, 25); getch();
	  exit(1);
       }
#endif

    read_han_font ( &gothic, "hangod2.fnt" );
    read_han_font ( &hand,   "hanson2.fnt" );

    clear_key_buff ();
}

/* ¯©Ð—–I˜· •¡¯a ÑÁ¡e·i ¹Á¶¡ ˆaŸa“e ¸s”aÐe(?) ´b­A¬aŸ¡   */
/* ·¡´¡‹¡ 5.3 Àáœñ •¡¯aÑÁ¡e·i ‹¡´âÐ–”a ¹·ža¯¡ ”a¯¡ ”h´aº…”a. */
/* ‹±¬âº ¸á 'C¡ Ða—a¶Á´á ˆå—aŸ¡‹¡'¬á Àq‰¡                  */
void OpenScreen ( void )
{
    int i, j;

    current_x = wherex ();
    current_y = wherey ();

    for ( i = 0 ; i < 80 ; i++ )
     {
	gettext ( i + 1, 1, i + 1, 25, &screen [i] );
     }

    movetext (  2, 1, 40, 25,  1, 1 );
    movetext ( 41, 1, 79, 25, 42, 1 );
    window ( 40, 1, 41, 25 );
    clrscr();

    window ( 1, 1, 80, 25 );

    for ( j = 0 ; j < 40 ; j++ )
    {
	movetext ( 2 ,1 ,40 - j, 25, 1, 1 );
	movetext ( 40 + j, 1, 79, 25, 40 + j + 1, 1 );
	delay ( 30 );
    }
}

/* µ¡Ïa“· ÑÁ¡e· µeÂ‰ */
/* µ¡Ïa“· ‹aŸ±· ¡EËa“e ¹Aˆa µaÀaÃ¥ŠµA‰A ¬å¢‰¤h·e ¯¡»³· ¹A¡¢·³“¡”a. ¸a¸b¢…Š ´a“± */
void Opening ( void )
{
    cleardevice();
    show_pcx ( 0, 0, "opening.bgf" );  /* opening.bgf“e pcxÍ¡ •· ‹aŸ± Ìa·© */
getch();
//    delay ( 26000 );
    clear_key_buff();
    Eraser_With_Black();
    Draw_Password_Border();
}

/* ´qÑ¡ ·³b·i ¶áÐe ¤‰w ‹aŸ± ¡—· */
/* ·¡ ‹aŸ±Ìa·©·e ÒµA ³a·© Ðq®Ÿi ‰¡aÐe ‹aœÏ¢ ¹ÁÎa‰¬e ¸b´ó·¡ Ï©¶aÐq */
void Draw_Password_Border ( void )
{
    cleardevice();
    show_pcx ( 0, 0, "password.bgf" );
    Judgment_Password();
}

/* ·³b–E ´qÑ¡· ¸÷,¦ µa¦Ÿi ˆaŸ± */
/* ”a¬õ¤å· ·³b·i Ðá¶w, ¡¡– ËiŸ¡¡e Ïa¡‹aœ‘ ¹·ža */
void Judgment_Password ( void )
{
    int rest_chance, judgment;
    char *input_password, *right_password = "´qÑ¡"; /* ·±·· ´qÑ¡ */
    const char *memo[4] =
       {
	  "á ¤é°á ·¸´á¤áv“¡ ?",
	  "¸b‘e µaŸq a M.T ˆi˜ áˆa.....",
	  "Ñ¢¯¡ á XX·¡“¡ ?",
	  " a»¡ b ‹¡ÒA´¡ !",
       };  /* ËiŸ¥ ´qÑ¡ ·³bµA ”Ðe ¡A¡¡ ¸÷· */

    set_to_han_mode(); /* Ðe‹i ·³b ¡¡—a¡ ¸åÑÅ */
    /* ´qÑ¡“e Ðe‹i¡, ·³b ¡¡—a“e µw¢…·a¡, Ðeµw ¸åÑÅÇ¡Ÿi ·±·¡ ¤aŽ¡¬á */
    /* ”á¶‚ Ša¢aÐe (?) ´qÑ¡ žË¥·i ¸b¬÷Ði ®•¡ ·¶·aa ¬w”ˆa ŠáÀg´a Ðq. */

    redirect_func = Redirect_Function_Fast;
    /* Ðe‹i ¢…¸w·¡ Â‰b–I˜ ‹i¸aˆe· Â‰b ¬a·¡µA ¯¡ˆe µa·AŸi –“e žË¥ Ñ¡Â‰ */
    /* Redirect_Function_Fast “e ‹i¸a Â‰b ¬a·¡µA ¯¡ˆe µa·Aˆa ´ô·q.         */
    /* ‹e•A µá°ö¡Ða¡e ´qÑ¡žË¥µA¬á•¡ ESC Ç¡µA ·Ðe Ïa¡‹aœ‘ ¹·žaŸi ¶áÐ    */

    for ( rest_chance = 0 ; rest_chance < 5 ; rest_chance++ )
       {
	   set_font ( hand, NULL, NULL );
	   set_color_han ( BLACK ); set_bkcolor_han ( WHITE );
	   set_output_offset_han ( 8 );
	   set_window_han ( 10, 27, 18, 27 );
	   gets_han ( input_password );
	   set_output_offset_han ( 0 );
	   judgment = strcmp ( input_password, right_password );
	   set_color_han ( BLUE );

	   if ( judgment == 0 )  /* ´qÑ¡ˆa  x´v·i ˜ */
	     {
		play ( "o4l4ceg" );
		setfillstyle ( SOLID_FILL, WHITE );
		bar ( 337, 432, 623, 463 );
		bar ( 17, 432, 173, 463 );
		set_output_offset_han ( 8 );
		aputs_han ( 43, 27, "¸q¯¡a a ·¯±Ð¬á £¡´eÐ", FONT_BOLD );
		set_output_offset_han ( 0 );
		redirect_func = NULL;
		init_window_han();
//		delay ( 30000 );
		delay ( 30 );
//getch();

		Eraser_With_Black();
		clear_key_buff();
		Letter_Contents();
	     } else   /* ´qÑ¡ˆa Ëiv·i ˜ */
	     {
		sound ( 1000 ); delay ( 200 ); nosound();
		setfillstyle ( SOLID_FILL, WHITE );
		set_color_han ( BLUE ); set_bkcolor_han ( WHITE );
		bar ( 17, 432, 173, 463 );
		bar ( 300 - rest_chance * 30, 432, 330 - rest_chance * 30, 463 );
		bar ( 337, 432, 623, 463 );
		set_output_offset_han ( 8 );
		aputs_han ( 43, 27, memo[rest_chance], FONT_BOLD );
		set_output_offset_han ( 0 );
		init_window_han();
		clear_key_buff();
	     }
       }

    setfillstyle ( SOLID_FILL, WHITE );
    bar ( 337, 432, 623, 463 );
    bar ( 17, 432, 173, 463 );
    set_color_han ( RED ); set_bkcolor_han ( WHITE );
    set_output_offset_han ( 8 );
    aputs_han ( 43, 27, "·¡ Íe»¡“e My-Girl e ¥© ® ·¶”s“¡”a.", FONT_BOLD );
    set_output_offset_han ( 0 );
    redirect_func = NULL;
//    delay ( 23000 );
getch();
    Ending();
}

/* ¯©¹A Íe»¡ ¶w ¦¦…µA Ð”wÐa“e Ðq®                     */
/* ‹aŸ±(ÏaŸ¥­A¯a ¡A·¡Äá· ¤qÐa“i ¸w¡e)·i ÑÁ¡eµA ªŸ¡‰¡     */
/* ¬Œa e ¤qÐa“i·i ¤‰w·a¡ Íe»¡ ¶w·¡ ÁÁ´b ªa»¡“e ¤w¯¢ */
void Letter_Contents ( void )
{
    int a;

    cleardevice();
    show_pcx ( 0, 0, "contents.bgf" );
//    delay ( 10000 );

    set_color_han   ( WHITE );
    set_bkcolor_han ( BLACK );
    set_window_han ( 6, 4, 52, 21 );
    set_font ( hand, NULL, NULL );
    redirect_func = Redirect_Function_Beep;
    printf_han ( "To. My-Girl K. M. J." );
    redirect_func = NULL;
    redirect_func = Redirect_Function_No_Beep;
    printf_han ( "\n\n\n  ´e ‘w !\n\n" );
    printf_han ( "  ¯a¢¤å¼ ¬—·© Â‚ÐaÐe”a.\n\n" );
    printf_han ( "µ¡œ… eµA Àq Í¡‹eÐa‰¡ ¯·‹aœá¶… Ð•¬i·i ¥¥”a.\n\n" );
    printf_han ( "¢ áŸi ”k·e i³¡´¡. á¢ ¹½”a.\n\n" );
    printf_han ( "“aa»¡“¡? Ða“i·¡ á ¬—·©·i Â‚ÐaÐa‰¡ ·¶”a“eˆõ.\n\n" );
    printf_han ( "  ‹aˆå ‹aœýŠ á ¤é°á ¯a¢¬i·¡Ša.\n\n" );
    printf_han ( "µ•i ˆ{·a¡e ¯¡»³ˆa¬á ´ –‰·e }´v·i¤óÐe...\n\n" );
    printf_han ( "´á¼áŠ ¸á¼áŠ..\n\n" );
    printf_han ( "Š—¯¡œ÷ Š—¯¡œ÷..\n\n\n" );
    printf_han ( "·¡ Ïa¡‹aœ‘·e ¥¥·¥· µa¸aÃ¥ŠµA‰A \n\n" );
    printf_han ( "¥¡“e Íe»¡¡¬á ‹a‘a· ¯a¢¤å¼   \n\n" );
    printf_han ( "¬—·©·i Â‚ÐaÐa“e ¶w· Íe»¡·³“¡”a. \n\n" );
    printf_han ( "¸á“e CÏa¡‹aœ‘· Á¡¥¡·¡¡a,\n\n" );
    printf_han ( "·¡ Íe»¡“e ¸á· Àõ Ðe‹i Ïa¡‹aœ‘·³“¡”a.\n\n" );
    printf_han ( "TC 2.0 µA¬á ÄñÌa·© Ðaµv¯s“¡”a.\n\n" );
    printf_han ( "Š…•AŠ…•A aŸq”¡ º¬â·i  g·¡ ”i´v¯s“¡”a.\n\n" );
    printf_han ( "­¡¯a“e ‰·ˆÐa‰V¯s“¡”a.\n\n" );
    printf_han ( "µ¸åµA ˜b¸÷¤éA“±· Íe»¡­¡¯aˆa ·¶´öºa?\n\n" );
    printf_han ( "´á“a¸÷•¡ µwÐ··e ¤h´v·aa ¸åÑa ËiŸ³“¡”a.\n\n" );
    printf_han ( "®ó‰A  iÐa¸a¡e ¢¯¢Ðe ¤w¤ó·i  g·¡ °ö¯s“¡”a.\n\n" );
    printf_han ( "CµA ”Ðe ‹¡Á¡ »¡¯¢‰Á <Ðe 4.1> œa·¡§aœáŸ¡ \n\n" );
    printf_han ( "AÌáœå¯a e ·¶·a¡e ®ó‰A ¦…¬â ˆa“wÐiˆó“¡”a.\n\n" );
    printf_han ( " ee»¡ 8‘e–E µa¸aÃ¥ŠŸi ¶áÐeˆáµv¯s“¡”a.\n\n" );
    printf_han ( "Àá·qÐ¥¡“e ‰¡¤‚·  iŒa»¡ ¡yº‰ °ö´ö“e•A¶a.\n\n" );
    printf_han ( "¤e·w·¡ ¬w”wÓ¡ ¹½´v´á¶a.\n\n" );
    printf_han ( "·¡œå¯¢·a¡ Íe»¡Ÿi ¥¡“e ˆõ•¡ ¹½´a¶a. \n\n" );
    printf_han ( "·¡ Ïa¡‹aœ‘µA ”Ðe ¦…¬â·¡ {eÒ..Ò•\n\n" );
    printf_han ( "”á ˆ¬å–E Ïa¡‹aœ‘·i  e—i´á \n\n" );
    printf_han ( "¸a¯¥· µa¸aÃ¥ŠµA‰A ¬å¢‰Ða¡e\n\n" );
    printf_han ( "¸÷ i ¹½´aÐi ˆó“¡”a.\n\n" );
    printf_han ( "á¢ ˆe”eÐ¬á ¦…¬âÐiˆõ•¡ ´ô‘A¶a.\n\n" );
    printf_han ( "­¡¯aµA ”Ðe »©¢…·¡a ‹¡Èa µ¡ŸA žË¥—w·e\n\n" );
    printf_han ( "gembox µA‰A¡ ¡A·© º­A¶a.\n\n" );
    printf_han ( "¸å Ïa¡‹aœ£· Á¡¥¡“¡Œa\n\n" );
    printf_han ( "´áa¶…ˆå ¢ˆ»¡  a¯¡Š¶a. \n\n" );
    printf_han ( "¹Aˆa ´a“eˆå ¡¡– ˆaŸaÁa —aŸ©‰V¯s“¡”a.\n\n" );
    printf_han ( "‹aœñ ·¡ e..\n" );

    redirect_func = NULL;
    init_window_han();
//    delay ( 20000 );
    clear_key_buff();

    Eraser_With_Color ( SOLID_FILL, WHITE );
    Post_Script();
}

/* Â¯¥µA ”Ðe Ðq®, ´á¬éÏe ‰¡¤‚·¡œa“e Èa·¡Ëi·... */
void Post_Script ( void )
{
    show_pcx ( 0, 0, "ps.bgf" );
//    delay ( 20000 );
    set_color_han ( BLUE ); set_bkcolor_han ( WHITE );
    set_font ( hand, NULL, NULL );
    set_window_han ( 57, 9, 79, 26 );
    redirect_func = Redirect_Function_No_Beep;
    printf_han ( "¬aœw‰¡¤‚(?)·¡¡Š¶a?\n\n" );
    printf_han ( "§¡£© ·³“¡”a. \n\n" );
    printf_han ( "¬aœw ‰¡¤‚·© ®•¡\n\n" );
    printf_han ( "”e®…Ðe Â¯¥·© ®•¡.\n\n" );
    printf_han ( "¸a¯¥· ¬aÑ×µA ˜aœa \n\n" );
    printf_han ( "”iœa»¡‰Vºa?\n\n" );
    printf_han ( " ee»¡ 8‘e –A“e\n\n" );
    printf_han ( "‹aœáa ´a»¢\n\n" );
    printf_han ( "­¥•¡ ¡µ ¸s´a¥¥\n\n" );
    printf_han ( "¹A µaÀaÃ¥ŠµA‰A\n\n" );
    printf_han ( "¥¡“e Íe»¡µv¯s“¡”a.\n\n" );
    printf_han ( " a»¡ b·a¡\n\n" );
    printf_han ( "1976‘e 5¶© 15·© \n\n" );
    printf_han ( "¹A µaÀaÃ¥Š· ¬—·©\n\n" );
    printf_han ( "Â‚ÐaÐ º­A¶a.\n\n" );
    printf_han ( "¬aœwÐe”a.\n\n" );
    printf_han ( "gembox" );

    redirect_func = NULL;
//    delay ( 25000 );
delay ( 25 );

    Ending();
}


/* ¢…¸w Â‰bº— ‹i¸aˆeµA ¯¡ˆe µa·AŸi –“e Ðq® #1 */
void Redirect_Function_No_Beep ( char *string )
{
    if ( kbhit() != 0 ) /* Ç¡¥¡—a ·³b·i ˆñ¬aÐa‰¡ ESC· ‰w¶ Ïa¡‹aœ‘ ¹·žaÐe”a. */
       {
	  if ( get_key_scan() == ESC )
	     {
		redirect_func = NULL;
		Ending();
	     } else
	     {
		redirect_func = NULL;
//		delay ( 1500 );
		delay ( 150 );
		redirect_func = Redirect_Function_No_Beep;
	     }
       } else
       {
	    if ( strcmp ( string, " " ) )
	       {
//		  delay ( 2000 );
		  delay ( 200 );
	       }
       }
}

/* ¢…¸w Â‰bº— ‹i¸aˆeµA ¯¡ˆe µa·AŸi –“e Ðq® #2 */
/* ‹i¸a Â‰b ¬a·¡µA '«¡' ­¡Ÿ¡“e “e žË¥        */
void Redirect_Function_Beep ( char *string )
{
    if ( kbhit() != 0 )
       {
	  if ( get_key_scan() == ESC )
	     {
		redirect_func = NULL;
		Ending();
	     } else
	     {
		redirect_func = NULL;
		sound ( 500 ); delay ( 10 ); nosound();
//		delay ( 2300 );
		delay ( 230 );

		redirect_func = Redirect_Function_No_Beep;
	     }
       } else
       {
	    if ( strcmp ( string, " " ) )
	       {
		   sound ( 300 ); delay ( 10 ); nosound();
//		   delay ( 2800 );
		   delay ( 280 );
	       }
       }
}

/* ¢…¸w Â‰bº— ‹i¸aˆeµA ¯¡ˆe µa·AŸi –“e Ðq® #3 */
/* ¯©¹A¡“e ‹i¸aˆe· —©A·¡ˆa ˆá· ´ô·q          */
/* ‹i¸aÂ‰b Ða“e ¬a·¡µA ESC Ç¡·a ·³b·¡ ·¶“a¡Ÿi */
/* ˆñ¬aÐa‹¡ ¶áÐe ¤w¤ó·±                          */
void Redirect_Function_Fast ( char *string )
{
    if ( kbhit() != 0 )
       {
	  if ( get_key_scan() == ESC )
	     {
		redirect_func = NULL;
		Ending();
	     } else
	     {
		redirect_func = NULL;
		delay ( 2 );
		redirect_func = Redirect_Function_No_Beep;
	     }
       } else
       {
	  if ( strcmp ( string, " " ) )
	     {
		delay ( 2 );
	     }
       }
}


/* ˆñ·e ¬‚·a¡ ¸åÑÁ¡e·i ¹Á¶µA¬á ”h´a¤áŸ¡“e Ðq® */
void Eraser_With_Black ( void )
{
    int x;

    setfillstyle ( SOLID_FILL, BLACK );
    for ( x = 5 ; x < 321 ; x += 5 )
      {
	 bar ( 0, 0, x, 479 ); bar ( 639 - x, 0, 639, 479 );
	 delay ( 17 );
      }
}

/* ˆñ·e¬‚·a¡ ¸åÑÁ¡e·i ´eµA¬á ¤c·a¡ ”h´a aˆa“e Ðq® */
void Eraser_With_Black_End ( void )
{
    int x;

    setfillstyle ( SOLID_FILL, BLACK );
    for ( x = 5 ; x < 321 ; x += 5 )
      {
	 bar ( 319 - x, 0, 319, 479 ); bar ( 320, 0, 320 + x, 479 );
	 delay ( 17 );
      }
}

/* »¡¸÷–E ¬‚‰Á fillstyle ¡ ¸åÑÁ¡e·i ”h“e Ðq® */
void Eraser_With_Color ( int style, int color )
{
    int x;

    setfillstyle ( style, color );
    for ( x = 5 ; x < 321 ; x += 5 )
      {
	 bar ( 0, 0, x, 479 ); bar ( 639 - x, 0, 639, 479 );
	 delay ( 17 );
      }
}

/* ´á˜å Ç¡ ·³b·¡ ·¶´ö¡Ÿi ´ia“e Ðq® */
int get_key_scan ( void )
{
    int key = bioskey ( 0 );
    return ( *( ( char * ) & key + 1 ) );
}

/* Ïa¡‹aœ‘· ¹·ža */
void Ending ( void )
{
    clear_key_buff();
    init_window_han();
    Eraser_With_Black_End();
    cleardevice();
    closegraph();
    close_han();
    CloseScreen();
}

/* Ïa¡‹aœ‘ ¯¡¸a‰¡Ði˜ ‹¡´âÐa‰¡ ¹Á¶¡ ˆiœv”å */
/* •¡¯aÑÁ¡e·i ‹a”¡ ”a¯¡ ”h´aº“e žË¥       */
/* ‹±¬âº ¸á 'C¡ Ða—a¶Á´á ˆå—aŸ¡‹¡' Àq¹¡     */
void CloseScreen ( void )
{
    int c;

    window ( 1, 1, 80, 25 );

    for ( c = 0 ; c < 39 ; c++ )
    {
	puttext  ( 1, 1, 1, 25, &screen [39 - c] );
	puttext  ( 80, 1, 80, 25, &screen [40 + c] );
	movetext ( 1, 1, c + 1, 25, 2, 1 );
	movetext ( 80 - c, 1, 80, 25, 79 - c, 1 );
	delay ( 40 );
    }

    puttext ( 1, 1, 1, 25, &screen [ 0 ] );
    puttext ( 80, 1, 80, 25, &screen [ 79 ] );
    gotoxy  ( current_x, current_y );
    free ( screen );
    exit ( 1 );
}

