/* �a���a�១ �q���A ���e ��� */

#include <bios.h>
#include <graphics.h>

#include "hanio.h"
#include "misc.h"
#include "extendio.h"
//#include "hangraph.h"
#include "keydef.h"
#include "pcx.h"

/* �a�w�a ���� �q���A ���e ��� */
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

/* ��b �e�� ��� */
char *gothic, *hand;        /* �i�a�A�A ���e ͡���� �i�w */
char *screen[80][25];       /* ��З�巁 ���a���e ��w�i ���e ͡���� �A���� �i�w */
int  current_x, current_y;  /* ��З�� ��� ��á�� ���� */


/* �A�� �q�� */
void main ( void )
{
    Make_Text_Title ();
    OpenScreen ();
    Initialize ();
    Opening ();
    Ending ();
}

/* ��З�� ���a���e�A ��� �B�a�a �a���a�� �A���i �a�e ��˥ */
/* ������ 5.3 �w�A�� �� �� ���e �i�A���e(?) �b�A�១       */
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

/* �a��Ϣ ���a, �e�i �a���a�១�A ���e ������ �� ͥ�a ���� �q�� */
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

/* ��З�I���� ���a ���e�i ������ �a�a�e �s�a�e(?) �b�A�a��   */
/* ������ 5.3 ��� ���a���e�i ����Ж�a ���a�� �a�� �h�a���a. */
/* ���⺁ �� 'C�� �a�a���� ��a����'�� �q��                  */
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

/* ���a�� ���e�� �e */
/* ���a�� �a���� �E�a�e �A�a �a�aå���A�A �墉�h�e ������ �A�������a. �a�b���� �a�� */
void Opening ( void )
{
    cleardevice();
    show_pcx ( 0, 0, "opening.bgf" );  /* opening.bgf�e pcx͡���� �a�� �a�� */
getch();
//    delay ( 26000 );
    clear_key_buff();
    Eraser_With_Black();
    Draw_Password_Border();
}

/* �qѡ ���b�i ���e ���w �a�� ���� */
/* �� �a���a���e ҁ�A �a�� �q���i ���a�e �a��Ϣ ���a���e �b�� ϩ�a�q */
void Draw_Password_Border ( void )
{
    cleardevice();
    show_pcx ( 0, 0, "password.bgf" );
    Judgment_Password();
}

/* ���b�E �qѡ�� ��,�� �a���i �a�� */
/* �a���巁 ���b�i ��w, ���� �i���e �a���a�� ���a */
void Judgment_Password ( void )
{
    int rest_chance, judgment;
    char *input_password, *right_password = "�qѡ"; /* ������ �qѡ */
    const char *memo[4] =
       {
	  "�� ��� �����v�� ?",
	  "�b�e �a�q �a M.T �i�� ��a.....",
	  "Ѣ�� �� XX���� ?",
	  "�a���b ���A�� !",
       };  /* �i�� �qѡ ���b�A ���e �A�� ���� */

    set_to_han_mode(); /* �e�i ���b ���a�� ���� */
    /* �qѡ�e �e�i��, ���b ���a�e �w���a��, �e�w ����ǡ�i ������ �a���� */
    /* �ᶂ �a�a�e (?) �qѡ ��˥�i �b���i ���� ���a�a �w���a ���g�a �q. */

    redirect_func = Redirect_Function_Fast;
    /* �e�i ���w�� �b�I�� �i�a�e�� �b �a���A ���e �a�A�i ���e ��˥ ѡ */
    /* Redirect_Function_Fast �e �i�a �b �a���A ���e �a�A�a ���q.         */
    /* �e�A ������a�e �qѡ��˥�A�ᕡ ESC ǡ�A ���e �a���a�� ���a�i ��Ё    */

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

	   if ( judgment == 0 )  /* �qѡ�a �x�v�i �� */
	     {
		play ( "o4l4ceg" );
		setfillstyle ( SOLID_FILL, WHITE );
		bar ( 337, 432, 623, 463 );
		bar ( 17, 432, 173, 463 );
		set_output_offset_han ( 8 );
		aputs_han ( 43, 27, "�q���a�a ����Ё�� ���eЁ", FONT_BOLD );
		set_output_offset_han ( 0 );
		redirect_func = NULL;
		init_window_han();
//		delay ( 30000 );
		delay ( 30 );
//getch();

		Eraser_With_Black();
		clear_key_buff();
		Letter_Contents();
	     } else   /* �qѡ�a �i�v�i �� */
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
    aputs_han ( 43, 27, "�� �e���e My-Girl�e �� �� ���s���a.", FONT_BOLD );
    set_output_offset_han ( 0 );
    redirect_func = NULL;
//    delay ( 23000 );
getch();
    Ending();
}

/* ���A �e�� ���w �����A Ё�w�a�e �q��                     */
/* �a��(�a���A�a �A���᷁ �q�a�i �w�e)�i ���e�A ������     */
/* ���a�e �q�a�i�i ���w�a�� �e�� ���w�� ���b ���a���e �w�� */
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
    printf_han ( "\n\n\n  �e �w !\n\n" );
    printf_han ( "  �a���弁 ���� �a�e�a.\n\n" );
    printf_han ( "�����e�A �q ͡�e�a�� ���a�ᶅ Е�i�i ���a.\n\n" );
    printf_han ( "�� ��i �k�e �i����. �ᢁ ���a.\n\n" );
    printf_han ( "�a�a����? �a�i�� �� �����i �a�a�� ���a�e��.\n\n" );
    printf_han ( "  �a�� �a���� �� ��� �a���i�����a.\n\n" );
    printf_han ( "���i �{�a�e �����a�� �� ���e �}�v�i���e...\n\n" );
    printf_han ( "��ኁ ��ኁ..\n\n" );
    printf_han ( "������ ������..\n\n\n" );
    printf_han ( "�� �a���a���e ������ �a�aå���A�A \n\n" );
    printf_han ( "�����e �e������ �a�a�� �a���弁   \n\n" );
    printf_han ( "�����i �a�a�e ���w�� �e�������a. \n\n" );
    printf_han ( "��e C�a���a���� �������a,\n\n" );
    printf_han ( "�� �e���e �᷁ �� �e�i �a���a�������a.\n\n" );
    printf_han ( "TC 2.0 �A�� ���a�� �a�v�s���a.\n\n" );
    printf_han ( "���A���A �a�q���� ����i �g�� �i�v�s���a.\n\n" );
    printf_han ( "���a�e �����a�V�s���a.\n\n" );
    printf_han ( "����A �b����A���� �e�����a�a �����a?\n\n" );
    printf_han ( "��a���� �wз�e �h�v�a�a ���a �i�����a.\n\n" );
    printf_han ( "��A �i�a�a�e �����e �w��i �g�� ���s���a.\n\n" );
    printf_han ( "C�A ���e ���� ������ <�e 4.1> �a���a�១ \n\n" );
    printf_han ( "�A���a�e ���a�e ��A ���� �a�w�i�󓡔a.\n\n" );
    printf_han ( "�e�e�� 8�e�E �a�aå���i ���e��v�s���a.\n\n" );
    printf_han ( "��qЁ���e ������ �i�a�� �y�� �����e�A�a.\n\n" );
    printf_han ( "�e�w�� �w�wӡ ���v��a.\n\n" );
    printf_han ( "���寢�a�� �e���i �����e ���� ���a�a. \n\n" );
    printf_han ( "�� �a���a���A ���e ���ⷡ �{�eҁ..ҕ\n\n" );
    printf_han ( "�� ����E �a���a���i �e�i�� \n\n" );
    printf_han ( "�a���� �a�aå���A�A �墉�a�e\n\n" );
    printf_han ( "���i ���a�i �󓡔a.\n\n" );
    printf_han ( "�ᢁ �e�eЁ�� �����i���� ���A�a.\n\n" );
    printf_han ( "���a�A ���e �������a ���a ���A ��˥�w�e\n\n" );
    printf_han ( "gembox �A�A�� �A�� ���A�a.\n\n" );
    printf_han ( "�� �a���a���� �������a\n\n" );
    printf_han ( "��a���� ���� �a�����a. \n\n" );
    printf_han ( "�A�a �a�e�� ���� �a�a�a �a���V�s���a.\n\n" );
    printf_han ( "�a�� ���e..\n" );

    redirect_func = NULL;
    init_window_han();
//    delay ( 20000 );
    clear_key_buff();

    Eraser_With_Color ( SOLID_FILL, WHITE );
    Post_Script();
}

/* ���A ���e �q��, ����e �������a�e �a���i��... */
void Post_Script ( void )
{
    show_pcx ( 0, 0, "ps.bgf" );
//    delay ( 20000 );
    set_color_han ( BLUE ); set_bkcolor_han ( WHITE );
    set_font ( hand, NULL, NULL );
    set_window_han ( 57, 9, 79, 26 );
    redirect_func = Redirect_Function_No_Beep;
    printf_han ( "�a�w����(?)�������a?\n\n" );
    printf_han ( "���� �����a. \n\n" );
    printf_han ( "�a�w ������ ����\n\n" );
    printf_han ( "�e���e ���� ����.\n\n" );
    printf_han ( "�a���� �a�׵A �a�a \n\n" );
    printf_han ( "�i�a���V�a?\n\n" );
    printf_han ( "�e�e�� 8�e �A�e\n\n" );
    printf_han ( "�a��a �a��\n\n" );
    printf_han ( "���� �� �s�a��\n\n" );
    printf_han ( "�A �a�aå���A�A\n\n" );
    printf_han ( "�����e �e���v�s���a.\n\n" );
    printf_han ( "�a���b�a��\n\n" );
    printf_han ( "1976�e 5�� 15�� \n\n" );
    printf_han ( "�A �a�aå���� ����\n\n" );
    printf_han ( "�aЁ ���A�a.\n\n" );
    printf_han ( "�a�w�e�a.\n\n" );
    printf_han ( "gembox" );

    redirect_func = NULL;
//    delay ( 25000 );
delay ( 25 );

    Ending();
}


/* ���w �b�� �i�a�e�A ���e �a�A�i ���e �q�� #1 */
void Redirect_Function_No_Beep ( char *string )
{
    if ( kbhit() != 0 ) /* ǡ���a ���b�i ��a�a�� ESC�� �w�� �a���a�� ���a�e�a. */
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

/* ���w �b�� �i�a�e�A ���e �a�A�i ���e �q�� #2 */
/* �i�a �b �a���A '��' �����e ���e ��˥        */
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

/* ���w �b�� �i�a�e�A ���e �a�A�i ���e �q�� #3 */
/* ���A���e �i�a�e�� ���A���a �᷁ ���q          */
/* �i�a�b �a�e �a���A ESC ǡ�a ���b�� ���a���i */
/* ��a�a�� ���e �w��                          */
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


/* ��e ���a�� �����e�i �����A�� �h�a�១�e �q�� */
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

/* ��e���a�� �����e�i �e�A�� �c�a�� �h�a �a�a�e �q�� */
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

/* �����E ���� fillstyle �� �����e�i �h�e �q�� */
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

/* ��� ǡ ���b�� �������i �i�a���e �q�� */
int get_key_scan ( void )
{
    int key = bioskey ( 0 );
    return ( *( ( char * ) & key + 1 ) );
}

/* �a���a���� ���a */
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

/* �a���a�� ���a���i�� �����a�� ������ �i�v�� */
/* ���a���e�i �a���� �a�� �h�a���e ��˥       */
/* ���⺁ �� 'C�� �a�a���� ��a����' �q��     */
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

