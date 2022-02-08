#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <dos.h>
#include <complex.h>

#define LEFT 1
#define RIGHT 2
#define DOWN 3

int TILE [10][18] ;     /* 타일데이타가 저장될 배열            */
int MOVETILE [3] ;      /* 움직이는 타일 데이타를 저장할 배열  */
int NEXTTILE [3] ;      /* 다음에 떨어질 타일 데이타 저장      */
int move_x, move_y ;    /* 움직이는 타일의 가로, 세로 좌표     */
int PASS = 1;           /* 타일이 움직일 수 있는 상태 (0은불가)*/
unsigned long score;    /* 점수 계산을 위한 전역변수           */

/********************************************************** PLAY GAME */
int retry(void);
/* 게임이 끝나고 계속 할것이지를 판단 */
void play_game();
/* 키입력을 받아 게임을 진행한다 */
void game_reset();
/* 게임배경화면을 화면에 표시 */

/**************************************** TILE MOVE, CHANGE, REMOVE.. */
void move_tile(int move);
/* 타일이 왼쪽, 오른쪽, 아래로 떨어지는 처리 */
void move_set();
/* 움직이는 타일의 좌표를 설정 */
void remove_sametile();
/* 3개 이상의 타일들을 제거하고 점수를 계산한다 */
void change_tile();
/* 떨어지는 타일의 순서를 바꾼다 */
void check_tile();
/* 타일이 가로,세로,대각선 방향으로 3개이상 같은지 확인 */
void nexttile_set();
/* 떨어질 타일을 임의로 만들어 낸다 */
void random_tile();
/* 떨어지는 타일좌표의 초기화 및 타일을 임의로 만들어 낸다 */

/******************************************************* GAME GRAPHIC */
void make_tile(int i, int j, int color, int type);
/* 32*32 타일을 그린다 */
void screen_clear();
/* 전체 화면을 지운다 */
int tile_color(int x);
/* 타일의 색을 정한다 */
void draw_tile();
/* 타일및 점수를 화면에 표시 */

/**************************************************************** ETC */
void effect();
/* 소리를 발생과 타일이 제거 할때 그래픽 효과 */

void make_tile (int i,    int j,    int color, int type=1)
/*             (가로좌표, 세로좌표, 타일색깔 , 타일형태  ) */
{
    int x, y;                     /* 타일의 좌표 변수 설정        */
    int a, b;                     /* 타일의 좌표 변수 설정        */
    x = 32 * i + 150;             /* 타일의 화면상 가로좌표       */
    y = 32 * (j - 2);             /* 타일의 화면상 세로좌표       */
    a = x + 32;                   /* 타일의 사각형 좌표계산(가로) */
    b = y + 32;                   /* 타일의 사각형 좌표계산(세로) */
    setfillstyle ( type, color ); /* 타일의 모양과 색깔을 정함    */
    bar (x, y, a, b);             /* 타일(사각형)을 화면에 표시   */

}

void game_reset()
{
    int i, j;    /* for문을 사용하기 위한 변수 */
    int x, y;    /* for문을 사용하기 위한 변수 */

    for ( y = 0 ; y < 17 ; y++ )
	   for ( x = 0 ; x < 10 ; x++ )
			TILE [x][y] = 0;
/* 타일 데이타들을 초기화 */
    for ( j = 0 ; j < 17 ; j++) {              
				TILE [0][j] = 15;
				TILE [8][j] = 15;
				make_tile (0, j, TILE[0][j]);
				make_tile (8, j, TILE[8][j]);
				}
/* 세로 벽면의 데이타를 흰색(15)로 설정 */
    for ( i = 0 ; i < 9 ; i++)  {
				TILE [i][16] = 15;    
				make_tile (i, 16, TILE[i][16]);   
				}
/* 바닦 벽면의 데이타를 흰색(15)로 설정 */
    for ( i = 10; i < 13; i++)
	   for ( j = 3; j < 8; j++)
				make_tile (i,j,9);
/* 다음 나올 타일을 표시하는 테두리 사각형 */
    
    settextstyle(0,0,3);          /* 글씨체 및 크기 설정 */
    setcolor(15);                 /* 글자의 색깔 설정    */           
    outtextxy(475,200,"NEXT");    /* 글자를 출력 (NEXT)  */
    setcolor(1);                  /* 글자의 색깔 설정    */
    outtextxy(453,330,"-SCORE-"); /* 글자를 출력         */
    outtextxy(453,360,"|     |"); /* 글자를 출력         */
    outtextxy(453,390,"-------"); /* 글자를 출력         */
    settextstyle(0,1,6);          /* 글자체 및 크기설정  */
    setcolor(4);                  /* 문자의 색깔 설정    */
    outtextxy(100,50,"HEXA 1.0"); /* 문자를 출력         */

}

void draw_tile()
{

     int i,j;  /* for문을 사용하기 위한 변수 */

     for ( i = 1 ; i < 8 ; i++ )
	  for ( j = 2 ; j < 16 ; j++ )    
		make_tile( i, j, TILE[i][j]);
/* 타일의 상태를 화면에 표시      */
     for ( i = 2,j = 0 ; j < 3 ; j++ )
		make_tile ( 11, j+4, NEXTTILE[i--]);
/* 다음에 나올 타일을 화면에 표시 */
     gotoxy(65,24);           /* 커서(80*25)의 좌표를 설정 */
     printf("%10ld",score);   /* 화면에 점수를 출력        */
}

void check_tile()
{
   int crush_x , crush_y , count = 0;
/*  가로좌표, 세로좌표, 가은타일의수-1 */
   int i; /* 가로 또는 세로의 좌표를 임시적으로 보관 */

   for ( crush_x = 1; crush_x < 8; crush_x++ )
     for ( crush_y = 0; crush_y < 16; crush_y++ ) {      
	if (abs(TILE[crush_x][crush_y]) == abs(TILE[crush_x][crush_y+1])
	   && TILE[crush_x][crush_y] != 0)
				     count++;
/* |방향의 연속된 같은색깔 타일숫자를 계산 */
	else {  if ( count > 1 )
	  for ( ; count > -1; count--)
	     TILE[crush_x][crush_y-count] = -abs(TILE[crush_x][crush_y-count]);
	  count = 0;
	     }
/* 연속된 같은 색깔의 타일 데이타를 음수로 변환 */
                                                  }

   for ( crush_y = 0; crush_y < 16; crush_y++)        
     for ( crush_x = 1; crush_x < 8; crush_x++ ) {
	if (abs(TILE[crush_x][crush_y]) == abs(TILE[crush_x+1][crush_y])
	   && TILE[crush_x][crush_y] != 0)
				     count++;
/* -방향의 연속된 같은색깔 타일숫자를 계산 */
	else {  if ( count > 1 )
	  for ( ; count > -1; count--)
	     TILE[crush_x-count][crush_y] = -abs(TILE[crush_x-count][crush_y]);
	  count = 0;
	     }
/* 연속된 같은 새깔의 타일 데이타를 음수로 변환 */
                                                 }

   for ( crush_x = 1 ; crush_x < 6; crush_x++ )
      for ( i = crush_x,crush_y = 15; i < 8; i++ ) {   
	if (abs(TILE[i][crush_y]) == abs(TILE[i+1][--crush_y])
	   && TILE[i][crush_y+1] != 0)
				    count++;
/* /방향의 연속된 같은색깔 타일숫자를 계산(x축기준) */
	else {  if ( count > 1 )
	  for ( ; count > -1; count--)
	     TILE[i-count][crush_y+1+count] = -abs(TILE[i-count][crush_y+1+count]);
	  count = 0;
	     }
/* 연속된 같은 새깔의 타일 데이타를 음수로 변환 */
                                                   }

   for ( crush_y = 14 ; crush_y > 1; crush_y-- )   
      for ( i = crush_y,crush_x = 1; i > 1; i-- ) {
	if (abs(TILE[crush_x][i]) == abs(TILE[++crush_x][i-1])
	   && TILE[crush_x-1][i] != 0)
				    count++;
/* /방향의 연속된 같은색깔 타일숫자를 계산(y축기준) */
	else {  if ( count > 1 )
	  for ( ; count > -1; count--)
	     TILE[crush_x-1-count][i+count] = -abs(TILE[crush_x-1-count][i+count]);
	  count = 0;
	     }
/* 연속된 같은 새깔의 타일 데이타를 음수로 변환 */
                                                  }

   for ( crush_x = 7 ; crush_x > 2; crush_x-- )
      for ( i = crush_x,crush_y = 15; i > 0 ; i-- ) {      
	if (abs(TILE[i][crush_y]) == abs(TILE[i-1][--crush_y])
	   && TILE[i][crush_y+1] != 0)
				    count++;
/* \방향의 연속된 같은색깔 타일숫자를 계산(x축기준) */
	else {  if ( count > 1 )
	  for ( ; count > -1; count--)
	     TILE[i+count][crush_y+1+count] = -abs(TILE[i+count][crush_y+1+count]);
	  count = 0;
	     }
/* 연속된 같은 새깔의 타일 데이타를 음수로 변환 */
						    }


   for ( crush_y = 14 ; crush_y > 1; crush_y-- )
      for ( i = crush_y,crush_x = 7; i > 1; i-- ) {
	if (abs(TILE[crush_x][i]) == abs(TILE[--crush_x][i-1])
	   && TILE[crush_x+1][i] != 0)
				    count++;
/* \방향의 연속된 같은색깔 타일숫자를 계산(y축기준) */
	else {  if ( count > 1 )
	  for ( ; count > -1; count--)
	     TILE[crush_x+1+count][i+count] = -abs(TILE[crush_x+1+count][i+count]);
	  count = 0;
	     }
/* 연속된 같은 새깔의 타일 데이타를 음수로 변환 */
						  }
}

void effect()
{
    int i,j;            /* for문을 사용하기 위한 변수설정  */
    clock_t start, end; /* 시간을 저장하기 위한 변수설정   */
    int beep = 80;      /* 소리를 내기위한 기본값          */
    float c_t = 0.3;    /* 시간차이를 계산하기 위한 기본값 */

    for ( i = 1; i < 8; i++ )
	for ( j = 0; j < 16; j++ )
	    if ( TILE[i][j] < 0 ) {
			     make_tile(i, j, abs(TILE[i][j]), 2);
			     beep = 230;
			     c_t = 0.7;
				  }
/* 타일데이타가 음수인 부분을 없어지게 하고 소리를 내기위한 변수초기화 */

     start = clock(); /* 현재의 시간을 읽는다 */
     for ( end = clock(); ((end - start)/ CLK_TCK) < c_t; )
		   {
		       sound(beep);
		       end = clock();
		   }
/* c_t시간 만큼 소리를 낸다(하드웨어 속도에 영향을 받지 않기위해) */
     nosound();  /* 소리를 끈다 */
}

void remove_sametile()
{
     int i,j;                /* for문을 사용하기 위한 변수설정 */
     int down_y;             /* 세로좌표를 임시로 저장         */
     int increase_score = 1; /* 점수 증가의 기본값             */

recheck:
     effect(); /* 소리를 낸다 */
     for ( i = 1 ; i < 8 ; i++ )
	  for ( j = 15 ; j > -1 ; j-- )
	       if( TILE[i][j] < 0 )  {
		   score = score + increase_score;
		   for (down_y = j ; down_y > 0 ; down_y--)
		      {  TILE[i][down_y] = TILE[i][down_y-1];
			 TILE[i][down_y-1] = 0; }
		   j++;              }

/* 연속된 타일 데이타를 없애고 데이타를 순서적으로 이동시킨다 */
     check_tile();
     for ( i = 1 ; i < 8 ; i++ )
	  for ( j = 15 ; j > -1 ; j-- )
	       if( TILE[i][j] < 0 )  {
			     increase_score++;
			     goto recheck;
				     }
/* recheck에서 이동된 데이타중 연속된 데이타가 있을경우 다시 recheck로 간다) */
     draw_tile(); /* 이동된 데이타를 화면에 표시 */
}


void move_set( ) 
{
     TILE[move_x][move_y]   = MOVETILE[0];
     TILE[move_x][move_y-1] = MOVETILE[1];
     TILE[move_x][move_y-2] = MOVETILE[2];
     draw_tile();
/* 움직이는 타일의 데이타 좌표에 색을 넣고 화면에 표시 */ 
}

void move_tile(int move)
{

    TILE[move_x][move_y]   = 0;
    TILE[move_x][move_y-1] = 0;
    TILE[move_x][move_y-2] = 0;
/* 움직이는 타일이 현재 위치한 곳의 데이타를 없앤다 */
    switch(move) {
     case 1:
       if ( TILE[move_x-1][move_y] == 0 && move_x != 1 )
			    move_x--;
			    break;
/* 좌측에 다른 타일 또는 벽이 없을 경우 좌측으로 한칸이동 */
     case 2:
       if ( TILE[move_x+1][move_y] == 0 && move_x != 7 )
			    move_x++;
			    break;
/* 우측에 다른 타일 또는 벽이 없을 경우 우측으로 한칸이동 */
     case 3:
       if ( TILE[move_x][move_y+1] == 0 )
			    move_y++;
/* 밑으로 한칸 이동한다 */
       else
	     PASS = 0;
		 }
/* 타일이 더이상 밑으로 떨어질 수 없는 상태로 설정(pass=0) */
    move_set();
/* 움직임이 완료된 타일의 상태를 다시 화면에 표시 */
}


int tile_color (int x )      /* x : 타일의 색깔 */
{
     int define_color = x;
     if ( define_color != 1 && define_color != 4) define_color = define_color+8;
     return define_color;
/* 1-15중 원하는 색을 쓰기 위해 랜덤함수로 얻어진 수를 변환 */
}

void random_tile()
{
    int m_tile;
    randomize();
    for ( m_tile = 0; m_tile < 3; m_tile++)  {
		 MOVETILE[m_tile] = NEXTTILE [m_tile];
		 NEXTTILE[m_tile] = tile_color(random(6)+1);
		  }
/* 움직이는 타일의 색깔을 랜덤함수에서 얻는다 */    

    move_x = 4;  /* 움직이는 타일의 가로좌표 초기화    */
    move_y = 2;  /* 움직이는 타일의 세로좌표 초기화    */
    PASS = 1;    /* 타일이 움직일 수 있는 상태(0=불가) */
    move_set();  /* 얻어진 데이타를 색으로 화면에 표시 */
}

void nexttile_set()
{
     randomize();
     NEXTTILE[0] = tile_color(random(6)+1);
     NEXTTILE[1] = tile_color(random(6)+1);
     NEXTTILE[2] = tile_color(random(6)+1);
/* 다음에 나올 타일의 색을 랜덤함수에서 얻는다 
   게임 시작시 한번 초기화를 시키고 사용되지 않는다.  */
}

void change_tile()
{
   int TEMP_TILE;
   TEMP_TILE = MOVETILE[2];
   MOVETILE[2] = MOVETILE[1];
   MOVETILE[1] = MOVETILE[0];
   MOVETILE[0] = TEMP_TILE;
   move_set();
/* 타일의 순서를 바꾼다 */
}

void play_game()
{
   time_t get_time, compare_time;  /* 시간을 보관하기 위한 변수       */
   double check;              /* 시간 차이를 계산하기 위한 변수       */
   int T = 0;                 /* 시간을 몇번 받아들였는지 계산        */
   int end = 0;               /* 게임이 진행되는 상태를 설정(1은불가) */
   char key;                  /* 키입력을 받아 들이는 변수            */

   do {
	  while ( !kbhit() ) {     /* 키입력이 없을 경우 */
		     if ( PASS == 0 )  {      /* 더이상 타일이 움직일 수 없는경우 */
				check_tile();        /* 연속된 타일이 있는지 체크 */
				remove_sametile();   /* 연속된 타일을 제거        */ 
				if (TILE[4][2] != 0) { end = 1; break; }
				random_tile();
				       }
/* 만일 타일이 움직일 수 없는 경우 연속된 타일을 제거하고 새타일을 만든다
   타일이 더이상 나올 자리가 없을 경우 게임을 끝낸다                      */

		     get_time = time( NULL );          /* 현재의 시간을 읽는다 */
		     if ( T == 0 )  {             /* 시간을 몇번 읽었는지 체크 */
			      compare_time = get_time;  /* 2번째 시간을 읽는다 */
			      T = 1;                     
				    }
		     check = difftime( compare_time, get_time ); 
/* 처음 읽은 시간과 두번째 시간의 차이를 계산 */
		     if ( check <= -1 ) {
			      T = 0;
			      move_tile(DOWN);
					}
/* 시간의 차이가 1초이면 타일을 밑으로 떨어지게 한다 */
			     }
	  if ( end == 1 ) break; /* 게임이 계속 진행될 수 있는지 체크 */
	  key = getch();         /* 키 입력을 받아 들인다.            */
	  if ( key == '5' )           /* 만약 '5'키가 입력되면    */
		      change_tile();  /* 타일의 순서를 바꾼다     */
	  if ( key == '4' )            /* 만약 '4'키가 입력되면   */
		      move_tile(LEFT); /* 타일을 왼쪽으로 이동    */
	  if ( key == '6' )             /* 만약 '6'키가 입력되면  */
		      move_tile(RIGHT); /* 타일을 오른쪽으로 이동 */
	  if ( key == ' ' )             /* 만약 ' '가 입력되면    */
		      while ( PASS == 1 )  { 
			      move_tile(DOWN);
				 T = 0;    }
                   /* 움직이는 타일이 더이상 아래로 이동할 수 없을때 까지 이동 */
  
      } while ( key != 'q' && key != 'Q' ); /* 'Q'가 입력되면 게임을 끝낸다 */
}

void screen_clear()
{
    setfillstyle ( 1, 0 );
    bar (0, 0, 639, 479);
/* 화면전체를 검은색으로 지운다 */
}

int retry(void)
{
    char wow;
    int hexa = 0;

    screen_clear();
    setfillstyle (1,9);
    bar(0,170, 639, 250);
    settextstyle(0,0,3);
    setcolor(14);
    outtextxy(30,200,"Do You Try Again ? (Y/N)");
/* 화면에 "Do you try Again ? (Y/N)"를 출력한다 */
    wow = getch(); /* 키를 받아들인다 */
    if ( wow == 'y' || wow == 'Y') hexa = 1;
    /* Y키를 누르면 다시 게임을 한다 */
    return hexa;
}

void main()
{
   int gdriver = VGA;      /* 그래픽 드라이버를 VGA로 설정          */
   int gmode = VGAHI;      /* 그래픽 모드를 (640*480) 16칼라로 설정 */
   int game = 0;

   initgraph(&gdriver,&gmode,"");  /* 화면을 그래픽 모드로 전환 */

   do {
     score = 0;        /* 게임의 점수를 0으로 초기화         */
     screen_clear();   /* 화면을 지운다                      */
     game_reset();     /* 게임 배경화면을 화면에 표시        */
     nexttile_set();   /* 떨어질 타일을 설정                 */
     random_tile();    /* 떨어지는 타일을 만들고 화면에 표시 */
     play_game();      /* 키입력을 받으면서 게임을 진행      */
     game = retry();   /* 게임이 끝나고 다시 할것인지를 결정 */
       } while ( game != 0 );

   closegraph();       /* 화면을 TEXT모드로 전환 */
   clrscr();           /* 화면을 지운다          */

   cout<<"- hexa test 1.0 version -\n";
   cout<<"|     1996. 11. 13      |\n";
   cout<<"----- by Gu Sang mo -----\n";
   cout<<"   remake 1.0.1 97.01.19   ";
}
