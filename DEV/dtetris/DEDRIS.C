

#include <stdarg.h>
#include <dos.h>
#include <graphics.h>
#include <stdio.h>
#include <conio.h>
#include <process.h>
#include <bios.h>
#include <stdlib.h>
#include <time.h>
#include <alloc.h>
//#include "svga256.h"

#define WD 15
#define X 230
#define Y 40
#define DX 45
#define STX 49

void base_block( void );
void erase( int, int, int, int );
void draw( int, int, int, int);
void clear_board( void );
void comp_board( int, int, int, int, int *);
void test_board( int );
void set_board( int, int, int, int );
void back_screen( void );
void Pause( void );

void sound1( void );
void sound2( void );
void sound3( void );
void sound4( void );

void Good_Bye(void);
void init_SVGA(void);
void display( void );
void highscr( void );
int gprintf( int , int , char *,... );

unsigned char board[21][12];

/* 블럭 모양 설정 */
static char block[7][4][4][4] = {
        { {{ 1, 1, 1, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }},

       {{ 1, 1, 0, 0 },
        { 1, 0, 0, 0 },
        { 1, 0, 0, 0 },
        { 0, 0, 0, 0 }},

       {{ 1, 0, 0, 0 },
        { 1, 1, 1, 0 },
        { 0, 0, 0, 0 },
		  { 0, 0, 0, 0 }},

       {{ 0, 1, 0, 0 },
        { 0, 1, 0, 0 },
        { 1, 1, 0, 0 },
        { 0, 0, 0, 0 }} },//#1

        { {{ 0, 0, 0, 0 },
        { 1, 1, 1, 1 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }},

       {{ 0, 1, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 1, 0, 0 }},

       {{ 0, 0, 0, 0 },
        { 1, 1, 1, 1 },
        { 0, 0, 0, 0 },
		  { 0, 0, 0, 0 }},

       {{ 0, 1, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 1, 0, 0 }} },//#2

        { {{ 0, 1, 1, 0 },
        { 0, 1, 1, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }},

       {{ 0, 1, 1, 0 },
        { 0, 1, 1, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }},

       {{ 0, 1, 1, 0 },
        { 0, 1, 1, 0 },
        { 0, 0, 0, 0 },
		  { 0, 0, 0, 0 }},

       {{ 0, 1, 1, 0 },
        { 0, 1, 1, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }} },//3

        { {{ 0, 1, 0, 0 },
        { 1, 1, 1, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }},

       {{ 0, 1, 0, 0 },
        { 1, 1, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 0, 0 }},

       {{ 0, 0, 0, 0 },
        { 1, 1, 1, 0 },
        { 0, 1, 0, 0 },
		  { 0, 0, 0, 0 }},

       {{ 0, 1, 0, 0 },
        { 0, 1, 1, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 0, 0 }} },//#4

        { {{ 0, 0, 1, 0 },
        { 1, 1, 1, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }},

       {{ 1, 1, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 0, 0 }},

       {{ 1, 1, 1, 0 },
        { 1, 0, 0, 0 },
        { 0, 0, 0, 0 },
		  { 0, 0, 0, 0 }},

       {{ 1, 0, 0, 0 },
        { 1, 0, 0, 0 },
        { 1, 1, 0, 0 },
        { 0, 0, 0, 0 }} },//#5

        { {{ 1, 1, 0, 0 },
        { 0, 1, 1, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }},

       {{ 0, 1, 0, 0 },
        { 1, 1, 0, 0 },
        { 1, 0, 0, 0 },
        { 0, 0, 0, 0 }},

       {{ 1, 1, 0, 0 },
        { 0, 1, 1, 0 },
        { 0, 0, 0, 0 },
		  { 0, 0, 0, 0 }},

       {{ 0, 1, 0, 0 },
        { 1, 1, 0, 0 },
        { 1, 0, 0, 0 },
        { 0, 0, 0, 0 }} },//#6

        { {{ 0, 1, 1, 0 },
        { 1, 1, 0, 0 },
        { 0, 0, 0, 0 },
        { 0, 0, 0, 0 }},

       {{ 1, 0, 0, 0 },
        { 1, 1, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 0, 0 }},

       {{ 0, 1, 1, 0 },
        { 1, 1, 0, 0 },
        { 0, 0, 0, 0 },
		  { 0, 0, 0, 0 }},

       {{ 1, 0, 0, 0 },
        { 1, 1, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 0, 0 }} } };//#7

static int color[8] = { 0,1,2,4,6,10,13,14};
static int patten[8] = {0,4,8,2,11,4,5,5};
static unsigned long newscr;  /* 점수를 저장하는 변수 선언 */

void main( void )
{
 int gd=DETECT,gm;
 initgraph(&gd,&gm,"c:\tc\bgi");

 //for (i=0;i<500;i++) {
  cleardevice();
  newscr=0;
  display();
  base_block();
  highscr();
  display();

  Pause();


 closegraph();
}//main

/* 벽돌 떨구는 함수 */
void base_block( void )
{
 int nx , ny , bios1, i, m, a, num, num1 ;
 int *test=0;

 back_screen();
 clear_board();
 sound3();
 randomize();
 num=random(7);

 while( board[1][6] != 1 )
 {
   a = 0;
   m = 0;
   nx = 5;
   ny = 0;

   num1 = random( 7 );
   draw( num1, 0, -5, 3);

  do
  {
   draw( num, m, nx, ny);

   for( i=0;i<=80;i++ )
   {
    delay( 5 );

    if( kbhit() )
    {
     while( !bioskey( 1 ) );
     bios1 = bioskey( 0 );

     switch( bios1 )
     {
		case 19200 : comp_board( num,m,nx-1,ny,test );
			 if( *test == 1 )
			  *test = 0;

			 else
			 {
			 erase( num, m, nx, ny );

			 --nx;
			 draw( num, m, nx, ny);
			 }
			 break;

		case 18432 : comp_board( num,(m+1)%4,nx,ny,test );
			 if( *test == 1 )
			  *test = 0;
			 else
			 {
			 erase( num, m, nx, ny );
			 m = (++m) % 4;
			 draw( num, m, nx, ny);
			 }
			 break;

		case 19712 : comp_board( num,m,nx+1,ny,test );
			 if( *test == 1 )
			  *test = 0;
			 else
			 {
			 erase( num, m, nx, ny );
			 ++nx;
			 draw( num, m, nx, ny);
			 }

			 break;

		case 14624 : erase( num,m,nx,ny );

			 while( *test != 1 )

			 comp_board( num,m,nx,++ny,test );

			 ny = ny - 1;
			 draw( num,m,nx,ny);
			 set_board( num,m,nx,ny );
			 sound1();
			 test_board( ny );
			 a = 1;
			 break;

		case 283 : closegraph();
			 Good_Bye();
		default : break;
	  }//switch
	 }//if
	 if( a == 1 ) break;

	} //for
	 if( *test != 1 )
	 {
	  comp_board( num,m,nx,ny+1,test );
	  if( *test == 1 )
	  {
		set_board( num, m, nx, ny );
		sound1();
		test_board( ny );

	  }
	  else
	  {
		 erase( num, m, nx, ny );
		 ny++;
	  }//else
	 }//if

  } while( *test == 0 && ny < 20 );
  *test = 0;
  erase( num1, 0, -5, 3 );
  num = num1;
 }//while
 sound2();
}//base_block

/* 벽돌 지우는 함수 */
void erase( int num, int m, int nx, int ny )
{
 int row, col;

 setfillstyle( SOLID_FILL, BLACK );
 for( row=0;row<4;row++ )
  for( col=0;col<4;col++ )
	if( block[num][m][row][col] == 1 )
 bar( X+WD*(nx+col), Y+WD*(ny+row),
  X+WD*(1+nx+col), Y+WD*(1+ny+row));
}//erase

/* 벽돌 그리는 함수 */
void draw( int num, int m, int nx, int ny)
{
 int col, row;

 setfillstyle( patten[num+1] , color[num+1] );
 for( row=0;row<4;row++ )
  for( col=0;col<4;col++ )
   if(block[num][m][row][col] == 1 )
   {
    bar( X+WD*(nx+col), Y+WD*(ny+row),
     X+WD*(1+nx+col), Y+WD*(1+ny+row));
    rectangle( X+WD*(nx+col)+1, Y+WD*(ny+row)+1,
        X+WD*(1+nx+col)-1, Y+WD*(1+ny+row)-1 );

   }//if
}//draw

/* board 배열을 초기화하는 함수 */
void clear_board()
{
 int i, j;

 for( i=0;i<20;i++ );
  for( j=0;j<11;j++ );
   board[i][j] = 0;

 for( i=0;i<20;i++ )
  board[i][0] = 1;

 for( i=0;i<20;i++ )
  board[i][11] = 1;

 for( i=0;i<12;i++ )
  board[20][i] = 1;
}

/* board와 떨어지는 벽돌을 비교하는 함수 */
void comp_board(int num,int m,int nx,int ny,int *test)
{
 int i, j;
 for( i=ny;i<ny+4 && i<=20;i++ )
 {
  for( j=nx; j<nx+4; j++ )
   if( ( board[i][j] == 1 ) && ( board[i][j] == block[num][m][i-ny][j-nx] ))
    *test = 1;
 }
}

/* 떨어지는 벽돌의 모양을 board에 저장하는 함수 */
void set_board( int num, int m, int nx, int ny )
{
 int i,j;

 for( i=ny; ( i< ny+4 && i<20 ) ; i++ )
  for( j=nx;j<nx+4;j++ )
   if( block[num][m][i-ny][j-nx] == 1 )
    board[i][j] = block[num][m][i-ny][j-nx];
}

/* 배경화면을 그려주는 함수 */
void back_screen()
{
 int i, j, k;

 setfillstyle( SOLID_FILL, 7 );
 for( i=0;i<20;i++ )
 {
  bar( X, Y+WD*i, X+WD, Y+WD*(i+1) );
  rectangle( X+1, Y+WD*i+1, X+WD-1, Y+WD*(i+1)-1 );
 }//for

 setfillstyle( SOLID_FILL, 7 );

 for( j=0;j<20;j++ )
 {
  bar( X+WD*11, Y+WD*j, X+WD*12, Y+WD*(j+1) );
  rectangle( X+WD*11+1, Y+WD*j+1, X+WD*12-1, Y+WD*(j+1)-1 );
 }//for

 setfillstyle( SOLID_FILL, 7 );
 for( k=0;k<12;k++ )
 {
  bar( X+WD*k, Y+WD*20, X+WD*(k+1), Y+WD*21 );
  rectangle( X+WD*k+1,Y+WD*20+1, X+WD*(k+1)-1, Y+WD*21-1 );
 }//for

 /* 점수판을 그려주는 함수 */
 setfillstyle( SOLID_FILL, 3 );
 bar( 40, 186, 193, 217 );
 bar( 40, 237, 190, 239 );
 bar( 40, 257, 190, 259 );
 bar( 40, 277, 190, 279 );
 bar( 40, 297, 190, 299 );
 bar( 40, 317, 190, 321 );
 bar( 39, 346, 190, 350 );
 bar( 39, 187, 43, 347 );
 bar( 190, 186, 194, 350 );
 rectangle( 41, 188, 192, 348 );
 line( 41, 214, 191, 214 );
 line( 41, 319, 191, 319 );
 settextstyle( 0, HORIZ_DIR, 1 );
 outtextxy( 46, 194, "HIGH SCORE" );
}


/* 키가 눌릴 때까지 기다리는 함수 */
void Pause( void )
{
 int c;

 c = getch();

 if( c == 0 )
  c = getch();
}

/* board의 한 줄이 다 채워졌는지 검사하는 함수 */
void test_board( int ny )
{
 int i, j, k, test1, n = 1;
 int x,y;
 unsigned long size;
 void far *image;

 if( ny >= 16 )
 {
  i = 19;
  while( n<=(20-ny) )
  {
	x=40,y=328;

	n++;
	test1 = 1;
	for( j=1;j<11;j++ )
	{
	 if( board[i][j] == 0 )
	 test1 = 0;
	}//for
	if( test1 == 1 )
	{
	 size = imagesize( X, Y, X+WD*11, Y+WD*i );
	 image = farmalloc( size );

	 getimage( X+WD, Y, X+WD*11, Y+WD*i-0.5, image );
	 setfillstyle( SOLID_FILL, BLACK );
	 bar( X+WD, Y+WD*i-0.5, X+WD*11, Y+WD*(i+1)+0.5 );

	 sound4();
	 newscr+=50;
	 bar(45,327,188,345);
	 settextstyle(0,0,2);
	 gprintf(x,y,"%9lu",newscr);

	 putimage( X+WD, Y+WD, image, COPY_PUT );

	 farfree( image );

	 for( j=i;j>0;j-- )
	  for( k=1;k<11;k++ )
		board[j][k] = board[j-1][k];

	 for( k=1;k<11;k++ )
	  board[0][k] = 0;
	}//if
	if( test1 != 1 )
	 i--;
  }//while
 }//if

 else
 {
  i = ny + 3;
  while( n<=4 )
  {
	n++;
	test1 = 1;
	x=40,y=328;

	for( j=1;j<11;j++ )
	{
	 if( board[i][j] == 0 )
	  test1 = 0;
	}//for
	if( test1 == 1 )
	{
	 size = imagesize( X, Y, X+WD*11, Y+WD*i );
	 image = farmalloc( size );

	 getimage( X+WD, Y, X+WD*11, Y+WD*i-0.5, image );
	 setfillstyle( SOLID_FILL, BLACK );
	 bar( X+WD, Y+WD*i-0.5, X+WD*11, Y+WD*(i+1)+0.5 );
	 sound4();
	 newscr+=50;
	 bar(45,327,188,345);
	 settextstyle(0,0,2);
	 gprintf(x,y,"%9lu",newscr);

	 putimage( X+WD, Y+WD, image, COPY_PUT );

	 farfree( image );

	 for( j=i;j>0;j-- )
	  for( k=1;k<11;k++ )
		board[j][k] = board[j-1][k];

	 for( k=1;k<11;k++ )
	  board[0][k] = 0;
	}
	if( test1 != 1 )
	 i--;
  }//while
 }//if
}//end

/* 벽돌을 놓을 때 소리내는 함수 */
void sound1( void )
{
 int i;

 for( i=0;i<10;i++ )
 {
  sound( 500 + i );
  delay( 5 );
 }

 nosound();
}

/* 게임이 끝났을 때 소리내는 함수 */ 
void sound2( void )
{
 int h, i;

 for( h=1;h<8;h++ )
 {
  for( i=0;i<15;i++ )
  {
   sound( 1000+2*h*h*i );
   delay( 5+i );
  }
 nosound();
 }
}

/* 게임이 시작할 때 소리내는 함수 */
void sound3( void )
{
 int i;

 for( i=10;i>0;i-- )
 {
  sound( 400+(i+3-1)*i*50 );
  delay( 180 );
 }
 sound( 400 );
 delay( 400 );
 nosound();
}

/* 한 줄을 지울 때 소리내는 함수 */
void sound4( void )
{
 int i, h;

 for( h=1;h<8;h++ )
 {
  for( i=0;i<10;i++ )
  {
   sound( 500+2*h*h*i );
   delay( 3 );
  }
 }
 nosound();
}

/* 점수판의 점수와 newscr을 비교하여 정렬하는 함수 */
void highscr( void )
{
 static unsigned long int begin[5] = { 0, 0, 0, 0, 0 };
 static unsigned long int score[5];
 static unsigned long int t[5];
 int i,j;

 FILE *fp;

 if(( fp = fopen("score.dat", "r")) == NULL )
 {
  fclose( fp );
  fp = fopen( "score.dat", "w+" );
  for( i=0;i<5;i++ )
   fprintf( fp, "%d\n", begin[i] );
 }
 for( i=0;i<5;i++ )
  fscanf( fp, "%lu", &score[i] );
 fclose( fp );
 fp = fopen( "score.dat", "w+" );
 for( i=0;i<5;i++ )
 {
  if( newscr >= score[i] )
  {
   t[0] = score[i];
   score[i] = newscr;
   for( j=0;j<5;j++ )
   {
    t[j+1] = score[i+j+1];
    score[i+j+1] = t[j];
   }
   break;

  }//if
 }//for
 for( i=0;i<5;i++ ) fprintf( fp, "%lu\n", score[i] );
 fclose( fp );
}

/* 점수판에 점수를 표시하는 함수 */
void display( void )
{
 unsigned long int score[5];
 int i, x, y;

 FILE *fp;

 if(( fp = fopen("score.dat", "r")) == NULL )
  highscr();

 else
 {
  for( i=0;i<5;i++ )
	fscanf( fp, "%lu", &score[i] );
 }

 fclose( fp );
 setfillstyle(SOLID_FILL,1);

 for( i=0;i<5;i++ )
 {
  bar( DX, i*20+219, DX+143, i*20+237 );
  x = STX;
  y = i*20+222;
  settextstyle( 2, 0, 7 );
  gprintf(x,y,"%11lu", score[i] );
 }
}

/* 그래픽 모드에서 printf()기능을 하는 함수 */
int gprintf( int xpos, int ypos, char *format,...)
{
 va_list argptr;
 char string[140];
 int count;

 va_start( argptr, format );

 count = vsprintf( string, format, argptr );
 outtextxy( xpos, ypos, string );
 va_end( argptr );
 return( count );
}


void Good_Bye(void)
{
 textcolor(9);
 gotoxy(35,10);
 cprintf("%s","GOOD - BYE");
 sound2();

 textcolor(3);
 gotoxy(30,12);
 cprintf("%s","MADE BY KIM EUN SOO");
 gotoxy(30,13);
 cprintf("%s","TRY AGAIN........");
 sound1();
 exit(0);
}
