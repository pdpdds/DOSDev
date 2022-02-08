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

int TILE [10][18] ;     /* Ÿ�ϵ���Ÿ�� ����� �迭            */
int MOVETILE [3] ;      /* �����̴� Ÿ�� ����Ÿ�� ������ �迭  */
int NEXTTILE [3] ;      /* ������ ������ Ÿ�� ����Ÿ ����      */
int move_x, move_y ;    /* �����̴� Ÿ���� ����, ���� ��ǥ     */
int PASS = 1;           /* Ÿ���� ������ �� �ִ� ���� (0���Ұ�)*/
unsigned long score;    /* ���� ����� ���� ��������           */

/********************************************************** PLAY GAME */
int retry(void);
/* ������ ������ ��� �Ұ������� �Ǵ� */
void play_game();
/* Ű�Է��� �޾� ������ �����Ѵ� */
void game_reset();
/* ���ӹ��ȭ���� ȭ�鿡 ǥ�� */

/**************************************** TILE MOVE, CHANGE, REMOVE.. */
void move_tile(int move);
/* Ÿ���� ����, ������, �Ʒ��� �������� ó�� */
void move_set();
/* �����̴� Ÿ���� ��ǥ�� ���� */
void remove_sametile();
/* 3�� �̻��� Ÿ�ϵ��� �����ϰ� ������ ����Ѵ� */
void change_tile();
/* �������� Ÿ���� ������ �ٲ۴� */
void check_tile();
/* Ÿ���� ����,����,�밢�� �������� 3���̻� ������ Ȯ�� */
void nexttile_set();
/* ������ Ÿ���� ���Ƿ� ����� ���� */
void random_tile();
/* �������� Ÿ����ǥ�� �ʱ�ȭ �� Ÿ���� ���Ƿ� ����� ���� */

/******************************************************* GAME GRAPHIC */
void make_tile(int i, int j, int color, int type);
/* 32*32 Ÿ���� �׸��� */
void screen_clear();
/* ��ü ȭ���� ����� */
int tile_color(int x);
/* Ÿ���� ���� ���Ѵ� */
void draw_tile();
/* Ÿ�Ϲ� ������ ȭ�鿡 ǥ�� */

/**************************************************************** ETC */
void effect();
/* �Ҹ��� �߻��� Ÿ���� ���� �Ҷ� �׷��� ȿ�� */

void make_tile (int i,    int j,    int color, int type=1)
/*             (������ǥ, ������ǥ, Ÿ�ϻ��� , Ÿ������  ) */
{
    int x, y;                     /* Ÿ���� ��ǥ ���� ����        */
    int a, b;                     /* Ÿ���� ��ǥ ���� ����        */
    x = 32 * i + 150;             /* Ÿ���� ȭ��� ������ǥ       */
    y = 32 * (j - 2);             /* Ÿ���� ȭ��� ������ǥ       */
    a = x + 32;                   /* Ÿ���� �簢�� ��ǥ���(����) */
    b = y + 32;                   /* Ÿ���� �簢�� ��ǥ���(����) */
    setfillstyle ( type, color ); /* Ÿ���� ���� ������ ����    */
    bar (x, y, a, b);             /* Ÿ��(�簢��)�� ȭ�鿡 ǥ��   */

}

void game_reset()
{
    int i, j;    /* for���� ����ϱ� ���� ���� */
    int x, y;    /* for���� ����ϱ� ���� ���� */

    for ( y = 0 ; y < 17 ; y++ )
	   for ( x = 0 ; x < 10 ; x++ )
			TILE [x][y] = 0;
/* Ÿ�� ����Ÿ���� �ʱ�ȭ */
    for ( j = 0 ; j < 17 ; j++) {              
				TILE [0][j] = 15;
				TILE [8][j] = 15;
				make_tile (0, j, TILE[0][j]);
				make_tile (8, j, TILE[8][j]);
				}
/* ���� ������ ����Ÿ�� ���(15)�� ���� */
    for ( i = 0 ; i < 9 ; i++)  {
				TILE [i][16] = 15;    
				make_tile (i, 16, TILE[i][16]);   
				}
/* �ٴ� ������ ����Ÿ�� ���(15)�� ���� */
    for ( i = 10; i < 13; i++)
	   for ( j = 3; j < 8; j++)
				make_tile (i,j,9);
/* ���� ���� Ÿ���� ǥ���ϴ� �׵θ� �簢�� */
    
    settextstyle(0,0,3);          /* �۾�ü �� ũ�� ���� */
    setcolor(15);                 /* ������ ���� ����    */           
    outtextxy(475,200,"NEXT");    /* ���ڸ� ��� (NEXT)  */
    setcolor(1);                  /* ������ ���� ����    */
    outtextxy(453,330,"-SCORE-"); /* ���ڸ� ���         */
    outtextxy(453,360,"|     |"); /* ���ڸ� ���         */
    outtextxy(453,390,"-------"); /* ���ڸ� ���         */
    settextstyle(0,1,6);          /* ����ü �� ũ�⼳��  */
    setcolor(4);                  /* ������ ���� ����    */
    outtextxy(100,50,"HEXA 1.0"); /* ���ڸ� ���         */

}

void draw_tile()
{

     int i,j;  /* for���� ����ϱ� ���� ���� */

     for ( i = 1 ; i < 8 ; i++ )
	  for ( j = 2 ; j < 16 ; j++ )    
		make_tile( i, j, TILE[i][j]);
/* Ÿ���� ���¸� ȭ�鿡 ǥ��      */
     for ( i = 2,j = 0 ; j < 3 ; j++ )
		make_tile ( 11, j+4, NEXTTILE[i--]);
/* ������ ���� Ÿ���� ȭ�鿡 ǥ�� */
     gotoxy(65,24);           /* Ŀ��(80*25)�� ��ǥ�� ���� */
     printf("%10ld",score);   /* ȭ�鿡 ������ ���        */
}

void check_tile()
{
   int crush_x , crush_y , count = 0;
/*  ������ǥ, ������ǥ, ����Ÿ���Ǽ�-1 */
   int i; /* ���� �Ǵ� ������ ��ǥ�� �ӽ������� ���� */

   for ( crush_x = 1; crush_x < 8; crush_x++ )
     for ( crush_y = 0; crush_y < 16; crush_y++ ) {      
	if (abs(TILE[crush_x][crush_y]) == abs(TILE[crush_x][crush_y+1])
	   && TILE[crush_x][crush_y] != 0)
				     count++;
/* |������ ���ӵ� �������� Ÿ�ϼ��ڸ� ��� */
	else {  if ( count > 1 )
	  for ( ; count > -1; count--)
	     TILE[crush_x][crush_y-count] = -abs(TILE[crush_x][crush_y-count]);
	  count = 0;
	     }
/* ���ӵ� ���� ������ Ÿ�� ����Ÿ�� ������ ��ȯ */
                                                  }

   for ( crush_y = 0; crush_y < 16; crush_y++)        
     for ( crush_x = 1; crush_x < 8; crush_x++ ) {
	if (abs(TILE[crush_x][crush_y]) == abs(TILE[crush_x+1][crush_y])
	   && TILE[crush_x][crush_y] != 0)
				     count++;
/* -������ ���ӵ� �������� Ÿ�ϼ��ڸ� ��� */
	else {  if ( count > 1 )
	  for ( ; count > -1; count--)
	     TILE[crush_x-count][crush_y] = -abs(TILE[crush_x-count][crush_y]);
	  count = 0;
	     }
/* ���ӵ� ���� ������ Ÿ�� ����Ÿ�� ������ ��ȯ */
                                                 }

   for ( crush_x = 1 ; crush_x < 6; crush_x++ )
      for ( i = crush_x,crush_y = 15; i < 8; i++ ) {   
	if (abs(TILE[i][crush_y]) == abs(TILE[i+1][--crush_y])
	   && TILE[i][crush_y+1] != 0)
				    count++;
/* /������ ���ӵ� �������� Ÿ�ϼ��ڸ� ���(x�����) */
	else {  if ( count > 1 )
	  for ( ; count > -1; count--)
	     TILE[i-count][crush_y+1+count] = -abs(TILE[i-count][crush_y+1+count]);
	  count = 0;
	     }
/* ���ӵ� ���� ������ Ÿ�� ����Ÿ�� ������ ��ȯ */
                                                   }

   for ( crush_y = 14 ; crush_y > 1; crush_y-- )   
      for ( i = crush_y,crush_x = 1; i > 1; i-- ) {
	if (abs(TILE[crush_x][i]) == abs(TILE[++crush_x][i-1])
	   && TILE[crush_x-1][i] != 0)
				    count++;
/* /������ ���ӵ� �������� Ÿ�ϼ��ڸ� ���(y�����) */
	else {  if ( count > 1 )
	  for ( ; count > -1; count--)
	     TILE[crush_x-1-count][i+count] = -abs(TILE[crush_x-1-count][i+count]);
	  count = 0;
	     }
/* ���ӵ� ���� ������ Ÿ�� ����Ÿ�� ������ ��ȯ */
                                                  }

   for ( crush_x = 7 ; crush_x > 2; crush_x-- )
      for ( i = crush_x,crush_y = 15; i > 0 ; i-- ) {      
	if (abs(TILE[i][crush_y]) == abs(TILE[i-1][--crush_y])
	   && TILE[i][crush_y+1] != 0)
				    count++;
/* \������ ���ӵ� �������� Ÿ�ϼ��ڸ� ���(x�����) */
	else {  if ( count > 1 )
	  for ( ; count > -1; count--)
	     TILE[i+count][crush_y+1+count] = -abs(TILE[i+count][crush_y+1+count]);
	  count = 0;
	     }
/* ���ӵ� ���� ������ Ÿ�� ����Ÿ�� ������ ��ȯ */
						    }


   for ( crush_y = 14 ; crush_y > 1; crush_y-- )
      for ( i = crush_y,crush_x = 7; i > 1; i-- ) {
	if (abs(TILE[crush_x][i]) == abs(TILE[--crush_x][i-1])
	   && TILE[crush_x+1][i] != 0)
				    count++;
/* \������ ���ӵ� �������� Ÿ�ϼ��ڸ� ���(y�����) */
	else {  if ( count > 1 )
	  for ( ; count > -1; count--)
	     TILE[crush_x+1+count][i+count] = -abs(TILE[crush_x+1+count][i+count]);
	  count = 0;
	     }
/* ���ӵ� ���� ������ Ÿ�� ����Ÿ�� ������ ��ȯ */
						  }
}

void effect()
{
    int i,j;            /* for���� ����ϱ� ���� ��������  */
    clock_t start, end; /* �ð��� �����ϱ� ���� ��������   */
    int beep = 80;      /* �Ҹ��� �������� �⺻��          */
    float c_t = 0.3;    /* �ð����̸� ����ϱ� ���� �⺻�� */

    for ( i = 1; i < 8; i++ )
	for ( j = 0; j < 16; j++ )
	    if ( TILE[i][j] < 0 ) {
			     make_tile(i, j, abs(TILE[i][j]), 2);
			     beep = 230;
			     c_t = 0.7;
				  }
/* Ÿ�ϵ���Ÿ�� ������ �κ��� �������� �ϰ� �Ҹ��� �������� �����ʱ�ȭ */

     start = clock(); /* ������ �ð��� �д´� */
     for ( end = clock(); ((end - start)/ CLK_TCK) < c_t; )
		   {
		       sound(beep);
		       end = clock();
		   }
/* c_t�ð� ��ŭ �Ҹ��� ����(�ϵ���� �ӵ��� ������ ���� �ʱ�����) */
     nosound();  /* �Ҹ��� ���� */
}

void remove_sametile()
{
     int i,j;                /* for���� ����ϱ� ���� �������� */
     int down_y;             /* ������ǥ�� �ӽ÷� ����         */
     int increase_score = 1; /* ���� ������ �⺻��             */

recheck:
     effect(); /* �Ҹ��� ���� */
     for ( i = 1 ; i < 8 ; i++ )
	  for ( j = 15 ; j > -1 ; j-- )
	       if( TILE[i][j] < 0 )  {
		   score = score + increase_score;
		   for (down_y = j ; down_y > 0 ; down_y--)
		      {  TILE[i][down_y] = TILE[i][down_y-1];
			 TILE[i][down_y-1] = 0; }
		   j++;              }

/* ���ӵ� Ÿ�� ����Ÿ�� ���ְ� ����Ÿ�� ���������� �̵���Ų�� */
     check_tile();
     for ( i = 1 ; i < 8 ; i++ )
	  for ( j = 15 ; j > -1 ; j-- )
	       if( TILE[i][j] < 0 )  {
			     increase_score++;
			     goto recheck;
				     }
/* recheck���� �̵��� ����Ÿ�� ���ӵ� ����Ÿ�� ������� �ٽ� recheck�� ����) */
     draw_tile(); /* �̵��� ����Ÿ�� ȭ�鿡 ǥ�� */
}


void move_set( ) 
{
     TILE[move_x][move_y]   = MOVETILE[0];
     TILE[move_x][move_y-1] = MOVETILE[1];
     TILE[move_x][move_y-2] = MOVETILE[2];
     draw_tile();
/* �����̴� Ÿ���� ����Ÿ ��ǥ�� ���� �ְ� ȭ�鿡 ǥ�� */ 
}

void move_tile(int move)
{

    TILE[move_x][move_y]   = 0;
    TILE[move_x][move_y-1] = 0;
    TILE[move_x][move_y-2] = 0;
/* �����̴� Ÿ���� ���� ��ġ�� ���� ����Ÿ�� ���ش� */
    switch(move) {
     case 1:
       if ( TILE[move_x-1][move_y] == 0 && move_x != 1 )
			    move_x--;
			    break;
/* ������ �ٸ� Ÿ�� �Ǵ� ���� ���� ��� �������� ��ĭ�̵� */
     case 2:
       if ( TILE[move_x+1][move_y] == 0 && move_x != 7 )
			    move_x++;
			    break;
/* ������ �ٸ� Ÿ�� �Ǵ� ���� ���� ��� �������� ��ĭ�̵� */
     case 3:
       if ( TILE[move_x][move_y+1] == 0 )
			    move_y++;
/* ������ ��ĭ �̵��Ѵ� */
       else
	     PASS = 0;
		 }
/* Ÿ���� ���̻� ������ ������ �� ���� ���·� ����(pass=0) */
    move_set();
/* �������� �Ϸ�� Ÿ���� ���¸� �ٽ� ȭ�鿡 ǥ�� */
}


int tile_color (int x )      /* x : Ÿ���� ���� */
{
     int define_color = x;
     if ( define_color != 1 && define_color != 4) define_color = define_color+8;
     return define_color;
/* 1-15�� ���ϴ� ���� ���� ���� �����Լ��� ����� ���� ��ȯ */
}

void random_tile()
{
    int m_tile;
    randomize();
    for ( m_tile = 0; m_tile < 3; m_tile++)  {
		 MOVETILE[m_tile] = NEXTTILE [m_tile];
		 NEXTTILE[m_tile] = tile_color(random(6)+1);
		  }
/* �����̴� Ÿ���� ������ �����Լ����� ��´� */    

    move_x = 4;  /* �����̴� Ÿ���� ������ǥ �ʱ�ȭ    */
    move_y = 2;  /* �����̴� Ÿ���� ������ǥ �ʱ�ȭ    */
    PASS = 1;    /* Ÿ���� ������ �� �ִ� ����(0=�Ұ�) */
    move_set();  /* ����� ����Ÿ�� ������ ȭ�鿡 ǥ�� */
}

void nexttile_set()
{
     randomize();
     NEXTTILE[0] = tile_color(random(6)+1);
     NEXTTILE[1] = tile_color(random(6)+1);
     NEXTTILE[2] = tile_color(random(6)+1);
/* ������ ���� Ÿ���� ���� �����Լ����� ��´� 
   ���� ���۽� �ѹ� �ʱ�ȭ�� ��Ű�� ������ �ʴ´�.  */
}

void change_tile()
{
   int TEMP_TILE;
   TEMP_TILE = MOVETILE[2];
   MOVETILE[2] = MOVETILE[1];
   MOVETILE[1] = MOVETILE[0];
   MOVETILE[0] = TEMP_TILE;
   move_set();
/* Ÿ���� ������ �ٲ۴� */
}

void play_game()
{
   time_t get_time, compare_time;  /* �ð��� �����ϱ� ���� ����       */
   double check;              /* �ð� ���̸� ����ϱ� ���� ����       */
   int T = 0;                 /* �ð��� ��� �޾Ƶ鿴���� ���        */
   int end = 0;               /* ������ ����Ǵ� ���¸� ����(1���Ұ�) */
   char key;                  /* Ű�Է��� �޾� ���̴� ����            */

   do {
	  while ( !kbhit() ) {     /* Ű�Է��� ���� ��� */
		     if ( PASS == 0 )  {      /* ���̻� Ÿ���� ������ �� ���°�� */
				check_tile();        /* ���ӵ� Ÿ���� �ִ��� üũ */
				remove_sametile();   /* ���ӵ� Ÿ���� ����        */ 
				if (TILE[4][2] != 0) { end = 1; break; }
				random_tile();
				       }
/* ���� Ÿ���� ������ �� ���� ��� ���ӵ� Ÿ���� �����ϰ� ��Ÿ���� �����
   Ÿ���� ���̻� ���� �ڸ��� ���� ��� ������ ������                      */

		     get_time = time( NULL );          /* ������ �ð��� �д´� */
		     if ( T == 0 )  {             /* �ð��� ��� �о����� üũ */
			      compare_time = get_time;  /* 2��° �ð��� �д´� */
			      T = 1;                     
				    }
		     check = difftime( compare_time, get_time ); 
/* ó�� ���� �ð��� �ι�° �ð��� ���̸� ��� */
		     if ( check <= -1 ) {
			      T = 0;
			      move_tile(DOWN);
					}
/* �ð��� ���̰� 1���̸� Ÿ���� ������ �������� �Ѵ� */
			     }
	  if ( end == 1 ) break; /* ������ ��� ����� �� �ִ��� üũ */
	  key = getch();         /* Ű �Է��� �޾� ���δ�.            */
	  if ( key == '5' )           /* ���� '5'Ű�� �ԷµǸ�    */
		      change_tile();  /* Ÿ���� ������ �ٲ۴�     */
	  if ( key == '4' )            /* ���� '4'Ű�� �ԷµǸ�   */
		      move_tile(LEFT); /* Ÿ���� �������� �̵�    */
	  if ( key == '6' )             /* ���� '6'Ű�� �ԷµǸ�  */
		      move_tile(RIGHT); /* Ÿ���� ���������� �̵� */
	  if ( key == ' ' )             /* ���� ' '�� �ԷµǸ�    */
		      while ( PASS == 1 )  { 
			      move_tile(DOWN);
				 T = 0;    }
                   /* �����̴� Ÿ���� ���̻� �Ʒ��� �̵��� �� ������ ���� �̵� */
  
      } while ( key != 'q' && key != 'Q' ); /* 'Q'�� �ԷµǸ� ������ ������ */
}

void screen_clear()
{
    setfillstyle ( 1, 0 );
    bar (0, 0, 639, 479);
/* ȭ����ü�� ���������� ����� */
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
/* ȭ�鿡 "Do you try Again ? (Y/N)"�� ����Ѵ� */
    wow = getch(); /* Ű�� �޾Ƶ��δ� */
    if ( wow == 'y' || wow == 'Y') hexa = 1;
    /* YŰ�� ������ �ٽ� ������ �Ѵ� */
    return hexa;
}

void main()
{
   int gdriver = VGA;      /* �׷��� ����̹��� VGA�� ����          */
   int gmode = VGAHI;      /* �׷��� ��带 (640*480) 16Į��� ���� */
   int game = 0;

   initgraph(&gdriver,&gmode,"");  /* ȭ���� �׷��� ���� ��ȯ */

   do {
     score = 0;        /* ������ ������ 0���� �ʱ�ȭ         */
     screen_clear();   /* ȭ���� �����                      */
     game_reset();     /* ���� ���ȭ���� ȭ�鿡 ǥ��        */
     nexttile_set();   /* ������ Ÿ���� ����                 */
     random_tile();    /* �������� Ÿ���� ����� ȭ�鿡 ǥ�� */
     play_game();      /* Ű�Է��� �����鼭 ������ ����      */
     game = retry();   /* ������ ������ �ٽ� �Ұ������� ���� */
       } while ( game != 0 );

   closegraph();       /* ȭ���� TEXT���� ��ȯ */
   clrscr();           /* ȭ���� �����          */

   cout<<"- hexa test 1.0 version -\n";
   cout<<"|     1996. 11. 13      |\n";
   cout<<"----- by Gu Sang mo -----\n";
   cout<<"   remake 1.0.1 97.01.19   ";
}
