
#include "bf.h"

//**************************** Missile Stage ***************************
#define MAX_MIS 20
#define MAX_FLAG 7
#define VOC1 "kill"

struct mis_flag_data {
   int  x ;
   int  y ;
   char count ;
   char onoff ;
} ;

struct stage_mis {
   int  x ;
   int  y ;
   char attr ;
   char fly_mode ;
   struct mis_flag_data flag[MAX_FLAG] ;
} stage_mis[MAX_MIS] ;

unsigned char stage_count ;
unsigned int stage_mis_count ;

unsigned char mis_fly_mode0 ;
unsigned char mis_fly_mode1 ;
unsigned char mis_garo_size ;
unsigned char mis_sero_size ;

unsigned char special_char_attr ;        // Event !!!
int special_char_loop ;                  // Event Character Loop

char far *jang_mis_image[2] ;

extern struct my_flag_data {
   int  x[MAX_FLAG] ;
   int  y[MAX_FLAG] ;
   char attr[MAX_FLAG] ;
   char count[MAX_FLAG] ;
} my_flag ;

extern char far *mis_image[16] ;
extern char far *mis_flag_image[MAX_FLAG] ;
extern char speed_x[4][16] ;
extern char speed_y[4][16] ;

void new_mis( int number )
{
   int i ;

   stage_mis[number].attr = 1 ;
   if( number < (MAX_MIS/2) )
   {
      stage_mis[number].x = MEMXMAX ;
      stage_mis[number].y = ( random( MEMYMAX<<1 ) - MEMYMAX ) ;
   }
   else
   {
      stage_mis[number].x = MEMXMIN ;
      stage_mis[number].y = ( random( MEMYMAX<<1 ) - MEMYMAX ) ;
   }

   for( i=0; i<MAX_FLAG; i++ )
   {
      stage_mis[number].flag[i].x = stage_mis[number].x + 16 ;
      stage_mis[number].flag[i].y = stage_mis[number].y + 3  ;
      stage_mis[number].flag[i].count = i ;
      stage_mis[number].flag[i].onoff = 1 ;
   }
}

void mis_move_output( void )
{
   int i, j ;

   for( i=0; i<MAX_MIS; i++ )
   {
      if( stage_mis[i].attr )
      {
	 stage_mis[i].x += - (bk_sx[0][stage_mis[i].fly_mode]<<3) + (speed_x[my_air[0].air][my_air[0].fly_mode]) ;
	 stage_mis[i].y += - (bk_sy[0][stage_mis[i].fly_mode]<<3) + (speed_y[my_air[0].air][my_air[0].fly_mode]) ;
	 if( my_turbo[0].attr )
	 {
	    stage_mis[i].x += bk_sx[1][stage_mis[i].fly_mode] ;
	    stage_mis[i].y += bk_sy[1][stage_mis[i].fly_mode] ;
	 }

	 if( stage_mis[i].x > MEMXMAX ) stage_mis[i].attr = 0 ;
	 if( stage_mis[i].x < MEMXMIN ) stage_mis[i].attr = 0 ;
	 if( stage_mis[i].y > MEMYMAX ) stage_mis[i].attr = 0 ;
	 if( stage_mis[i].y < MEMYMIN ) stage_mis[i].attr = 0 ;
      }
      else
      {
	 if( (random(50)==25) && (stage_count==2) ) new_mis( i ) ;
      }

      if( !my_air[0].kill && stage_mis[i].attr )
      {
	 if( Crash( my_air[0].x+10, my_air[0].y+10, my_air[0].x+40,       my_air[0].y+30,
		    stage_mis[i].x, stage_mis[i].y, stage_mis[i].x+mis_garo_size, stage_mis[i].y+mis_sero_size ) )
	 {
	    SetBFPalette( 2 ) ;
	    sayPCM( VOC1, 1 ) ;
	    my_body_attack( mis_static_body[2], 1 ) ;
	    stage_mis[i].attr = 0 ;
	 }
      }

      if( stage_mis[i].attr )
      {
	 if( stage_mis[i].fly_mode == mis_fly_mode0 )
	    BufSpritePutImage( stage_mis[i].x, stage_mis[i].y, jang_mis_image[0], mcga1 ) ;
	 else
	    BufSpritePutImage( stage_mis[i].x, stage_mis[i].y, jang_mis_image[1], mcga1 ) ;
	 for( j=0; j<MAX_FLAG; j++ )
	 {
	    stage_mis[i].flag[j].x += speed_x[my_air[0].air][my_air[0].fly_mode] ;
	    stage_mis[i].flag[j].y += speed_y[my_air[0].air][my_air[0].fly_mode] ;
	    if( my_turbo[0].attr )
	    {
	       stage_mis[i].flag[j].x += bk_sx[1][my_air[0].fly_mode] ;
	       stage_mis[i].flag[j].y += bk_sy[1][my_air[0].fly_mode] ;
	    }

	    BufSpritePutImage( stage_mis[i].flag[j].x-(*mis_flag_image[stage_mis[i].flag[j].count]>>1),
			       stage_mis[i].flag[j].y-(*(mis_flag_image[stage_mis[i].flag[j].count]+2)>>1),
			       mis_flag_image[stage_mis[i].flag[j].count], mcga1 ) ;
	    if( stage_mis[i].flag[j].count == MAX_FLAG-1 )
	    {
	       if( stage_mis[i].flag[j].onoff )
	       {
		  stage_mis[i].flag[j].onoff = 0 ;
		  stage_mis[i].flag[j].x = stage_mis[i].x + 16 ;
		  stage_mis[i].flag[j].y = stage_mis[i].y + 3  ;
		  stage_mis[i].flag[j].count = 0 ;
	       }
	       else stage_mis[i].flag[j].onoff = 1 ;
	    }
	    else
	    {
	       if( stage_mis[i].flag[j].onoff )
	       {
		  stage_mis[i].flag[j].onoff = 0 ;
		  stage_mis[i].flag[j].count++ ;
	       }
	       else stage_mis[i].flag[j].onoff = 1 ;
	    }
	 }
      }
   }
}

//************************* Bomber Kill Stage **************************

#define BOM_GUN 7
#define VOC6 "guncrash"

struct bom_flag_data {
   int  x ;
   int  y ;
   char attr ;
   char count ;
} ;

struct bom_gun_data {
   int  x ;
   int  y ;
   char attr ;
   char fly_mode ;
} ;

struct bomber_data {
   int x ;
   int y ;
   char attr ;
   char body ;
   char fly_mode ;
   struct bom_gun_data gun[2][BOM_GUN] ;
   struct bom_flag_data flag[3][MAX_FLAG] ;
} bomber ;

char bomber_gun_fly_mode[2][BOM_GUN] = {
   { 10, 12, 14, 0, 2, 4, 6 } , { 2, 4, 6, 8, 10, 12, 14 }
} ;

char far *bomber_image ;

unsigned int stage_bom_count ;

extern char far *gun_image[16] ;
extern char far *air_flag_image[MAXFLAG] ;

void bomber_move_output( void )
{
   int i, j ;

   if( bomber.attr )
   {
      bomber.x += - ((bk_sx[0][bomber.fly_mode]<<1)+bk_sx[0][bomber.fly_mode]-2) + (speed_x[my_air[0].air][my_air[0].fly_mode]) ;
      bomber.y += - ((bk_sy[0][bomber.fly_mode]<<1)+bk_sy[0][bomber.fly_mode]) + (speed_y[my_air[0].air][my_air[0].fly_mode]) ;
      if( my_turbo[0].attr )
      {
	 bomber.x += bk_sx[1][bomber.fly_mode] ;
	 bomber.y += bk_sy[1][bomber.fly_mode] ;
      }

      if( bomber.x > MEMXMAX ) bomber.x = MEMXMIN ;
      if( bomber.x < MEMXMIN ) bomber.x = MEMXMAX ;
      if( bomber.y > MEMYMAX ) bomber.y = MEMYMIN ;
      if( bomber.y < MEMYMIN ) bomber.y = MEMYMAX ;

      if( bomber.body )
      {
	 if( Crash( my_air[0].x+10,  my_air[0].y+10,  my_air[0].x+40,  my_air[0].y+30,
		    bomber.x+140, bomber.y-100, bomber.x+340, bomber.y+150 ) )
	 {
	    for( i=0; i<BOM_GUN; i++ )
	    {
	       if( !bomber.gun[0][i].attr )
	       {
		  bomber.gun[0][i].x = bomber.x + 220 ;
		  bomber.gun[0][i].y = bomber.y + 25 ;
		  bomber.gun[0][i].attr = 1 ;
		  bomber.gun[0][i].fly_mode = bomber_gun_fly_mode[0][i] ;
	       }
	    }
	 }
	 if( Crash( my_air[0].x+10,  my_air[0].y+10,  my_air[0].x+40,  my_air[0].y+30,
		    bomber.x-100, bomber.y-100, bomber.x+100, bomber.y+150 ) )
	 {
	    for( i=0; i<BOM_GUN; i++ )
	    {
	       if( !bomber.gun[1][i].attr )
	       {
		  bomber.gun[1][i].x = bomber.x + 20 ;
		  bomber.gun[1][i].y = bomber.y + 25 ;
		  bomber.gun[1][i].attr = 1 ;
		  bomber.gun[1][i].fly_mode = bomber_gun_fly_mode[1][i] ;
	       }
	    }
	 }
      }

      for( j=0; j<2; j++ )
      {
	 for( i=0; i<BOM_GUN; i++ )
	 {
	    if( bomber.gun[j][i].attr )
	    {
	       bomber.gun[j][i].x += - (speed_x[0][bomber.gun[j][i].fly_mode]<<1) +
				       (speed_x[my_air[0].air][my_air[0].fly_mode]) ;
	       bomber.gun[j][i].y += - (speed_y[0][bomber.gun[j][i].fly_mode]<<1) +
				       (speed_y[my_air[0].air][my_air[0].fly_mode]) ;
	       if( my_turbo[0].attr )
	       {
		  bomber.gun[j][i].x += bk_sx[1][my_air[0].fly_mode] ;
		  bomber.gun[j][i].y += bk_sy[1][my_air[0].fly_mode] ;
	       }

	       BufSpritePutImage( bomber.gun[j][i].x, bomber.gun[j][i].y, gun_image[bomber.gun[j][i].fly_mode], mcga1 ) ;

	       if( bomber.gun[j][i].x >  340 ) bomber.gun[j][i].attr = 0 ;
	       else
	       if( bomber.gun[j][i].x < -20  ) bomber.gun[j][i].attr = 0 ;
	       else
	       if( bomber.gun[j][i].y >  220 ) bomber.gun[j][i].attr = 0 ;
	       else
	       if( bomber.gun[j][i].y < -20  ) bomber.gun[j][i].attr = 0 ;

	       if( Crash( my_air[0].x+10,  my_air[0].y+10,  my_air[0].x+40,  my_air[0].y+30,
			  bomber.gun[j][i].x,   bomber.gun[j][i].y,
			  bomber.gun[j][i].x+9, bomber.gun[j][i].y+9 ) )
	       {
		  if( my_air[0].body > 0 )
		  {
		     SetBFPalette( 1 ) ;
		     sayPCM( VOC6, 1 ) ;
		     my_body_attack( 2, 2 ) ;
		     bomber.gun[j][i].attr = 0 ;
		  }
	       }
	    }
	 }
      }

      BufSpritePutImage( bomber.x, bomber.y, bomber_image, mcga1 ) ;

      for( i=0; i<MAXGUN; i++ )
      {
	 if( my_gun[0].attr[i] && bomber.body )
	 {
	    if( Crash( my_gun[0].x[i], my_gun[0].y[i], my_gun[0].x[i]+9, my_gun[0].y[i]+9,
		       bomber.x+5,  bomber.y+5,  bomber.x+210,  bomber.y+40 ) )
	    {
	       bomber.body-- ;
	       if( bomber.body <= 0 )
	       {
		  sayPCM( VOC1, 1 ) ;
		  bomber.body = 0 ;
	       }
	       else
	       {
		  if( bomber.body == 15 )
		  {
		     for( j=0; j<MAX_FLAG; j++ )
		     {
			bomber.flag[0][j].x = bomber.x + random(210)+5 ;
			bomber.flag[0][j].y = bomber.y + random(40)+5 ;
			bomber.flag[0][j].attr = 1 ;
			bomber.flag[0][j].count = j ;
		     }
		  }
		  if( bomber.body == 10 )
		  {
		     for( j=0; j<MAX_FLAG; j++ )
		     {
			bomber.flag[1][j].x = bomber.x + random(210)+5 ;
			bomber.flag[1][j].y = bomber.y + random(40)+5 ;
			bomber.flag[1][j].attr = 1 ;
			bomber.flag[1][j].count = j ;
		     }
		  }
		  if( bomber.body == 5  )
		  {
		     for( j=0; j<MAX_FLAG; j++ )
		     {
			bomber.flag[2][j].x = bomber.x + random(210)+5 ;
			bomber.flag[2][j].y = bomber.y + random(40)+5 ;
			bomber.flag[2][j].attr = 1 ;
			bomber.flag[2][j].count = j ;
		     }
		  }
	       }
	       my_gun[0].attr[i] = 0 ;
	    }
	 }
      }

      if( my_mis.attr && bomber.body )
      {
	 if( Crash( my_mis.x,   my_mis.y,   my_mis.x+31,  my_mis.y+22,
		    bomber.x+5, bomber.y+5, bomber.x+210, bomber.y+40 ) )
	 {
	    bomber.body -= mis_static_body[my_mis.weapon] ;

	    if( bomber.body <= 0 )
	    {
	       sayPCM( VOC1, 1 ) ;
	       bomber.body = 0 ;
	    }
	    if( bomber.body <= 15 )
	    {
	       for( j=0; j<MAX_FLAG; j++ )
	       {
		  bomber.flag[0][j].x = bomber.x + random(210)+5 ;
		  bomber.flag[0][j].y = bomber.y + random(40)+5 ;
		  bomber.flag[0][j].attr = 1 ;
		  bomber.flag[0][j].count = j ;
	       }
	    }
	    if( bomber.body <= 10 )
	    {
	       for( j=0; j<MAX_FLAG; j++ )
	       {
		  bomber.flag[1][j].x = bomber.x + random(210)+5 ;
		  bomber.flag[1][j].y = bomber.y + random(40)+5 ;
		  bomber.flag[1][j].attr = 1 ;
		  bomber.flag[1][j].count = j ;
	       }
	    }
	    if( bomber.body <= 5  )
	    {
	       for( j=0; j<MAX_FLAG; j++ )
	       {
		  bomber.flag[2][j].x = bomber.x + random(210)+5 ;
		  bomber.flag[2][j].y = bomber.y + random(40)+5 ;
		  bomber.flag[2][j].attr = 1 ;
		  bomber.flag[2][j].count = j ;
	       }
	    }
	    my_mis.attr = 0 ;
	 }
      }

      for( i=0; i<3; i++ )
      {
	 for( j=0; j<MAX_FLAG; j++ )
	 {
	    if( bomber.flag[i][j].attr )
	    {
	       BufSpritePutImage( bomber.flag[i][j].x-(*air_flag_image[bomber.flag[i][j].count]>>1),
				  bomber.flag[i][j].y-(*(air_flag_image[bomber.flag[i][j].count]+2)>>1),
				  air_flag_image[bomber.flag[i][j].count], mcga1 ) ;
	       if( bomber.flag[i][j].count == MAX_FLAG-1 )
	       {
		  bomber.flag[i][j].count = 0 ;
		  bomber.flag[i][j].x = bomber.x + random(210)+5 ;
		  bomber.flag[i][j].y = bomber.y + random(40)+5 ;
	       }
	       else bomber.flag[i][j].count++ ;
	    }
	 }
      }
   }
}

void stage_rader( void )
{
   int i ;

/*   if( (stage.number == 3) || (stage.number == 6) )
   {
      for( i=0; i<MAX_MIS; i++ )
      {
	 if( stage_mis[i].attr )
	    Rader_Output( stage_mis[i].x, stage_mis[i].y, (3<<4)+7 ) ;
      }
   }           */
   if( stage.number == 4 )
   {
      if( bomber.attr )
      {
	 Rader_Output( bomber.x,     bomber.y, 8<<4 ) ;
	 Rader_Output( bomber.x+50,  bomber.y, 8<<4 ) ;
	 Rader_Output( bomber.x+100, bomber.y, 8<<4 ) ;
	 Rader_Output( bomber.x+150, bomber.y, 8<<4 ) ;
      }
   }
}

void scenario_start( void )
{
   ViewIMG( "bomber", mcga2 ) ;
   bomber_image = (char far *)farmalloc( 221L*51L+4L ) ;
   BufGetImage( 0, 0, 219, 49, bomber_image, mcga2 ) ;
}

void scenario_close( void )
{
   farfree( bomber_image ) ;
}

void scenario_program( int attr )
{
   int i, j ;

   if( attr == 0 ) stage_count = 0 ;
   if( attr == 2 ) stage_count = 255 ;
   switch( stage_count )
   {
      case 0 :
	       for( j=1; j<MAXMYAIR; j++ ) my_air[j].kill = 1 ;
	       j = 1 ;
	       for( i=0; i<9; i++ )
	       {
		  if( select_attr[i] )
		  {
		     my_air[j].fly_mode = 0 ;
		     my_air[j].wing_man_num = select_man[i] ;
		     my_air[j].ai  = wing_man[select_man[i]].level ;
		     my_air[j].air = 0 ;
		     my_air[j].kill = 0 ;
		     j++ ;
		  }
	       }
	       my_air[1].x = 130 ;
	       my_air[1].y = 75  - 60 ;

	       my_air[2].x = 130 ;
	       my_air[2].y = 75  + 60 ;

	       my_air[3].x = 130 - 70 ;
	       my_air[3].y = 75 ;

	       my_air[4].x = 130 + 70 ;
	       my_air[4].y = 75 ;

	       if( stage.number == 1 )
	       {
		  for( j=0; j<MAXMYAIR; j++ ) my_air[j].air  = 1 ;
                  for( j=1; j<MAXMYAIR; j++ ) my_air[j].kill = 1 ;
	       }
/*	       if( stage.number == 3 )
	       {
		  for( i=0; i<MAX_MIS; i++ ) stage_mis[i].attr = 0 ;
		  ViewIMG( "bf2pic", mcga2 ) ;
		  jang_mis_image[0] = (char far *)farmalloc( 32L*5L+4L ) ;
		  jang_mis_image[1] = (char far *)farmalloc( 32L*5L+4L ) ;
		  BufGetImage( 168, 170, 168+31, 170+4, jang_mis_image[0], mcga2 ) ;
		  BufGetImage( 168, 175, 168+31, 175+4, jang_mis_image[1], mcga2 ) ;
		  mis_fly_mode0 = 8  ;
		  mis_fly_mode1 = 0  ;
		  mis_garo_size = 31 ;
		  mis_sero_size = 4  ;
	       }                                                          */
	       if( stage.number == 6 )
	       {
		  bomber.attr = 0 ;
	       }
               if( stage.number == 7 )
               {
                  special_char_loop = 0 ;
                  special_char_attr = 1 ;
               }
               if( stage.number == 14 )
               {
                  special_char_loop = 0 ;
                  special_char_attr = 1 ;
               }
               if( stage.number == 15 )
               {
                  special_char_loop = 0 ;
                  special_char_attr = 1 ;
               }
/*	       if( stage.number == 6 )
	       {
		  for( i=0; i<MAX_MIS; i++ ) stage_mis[i].attr = 0 ;
		  ViewIMG( "bf2pic", mcga2 ) ;
		  jang_mis_image[0] = (char far *)farmalloc( 30L*15L+4L ) ;
		  jang_mis_image[1] = (char far *)farmalloc( 30L*15L+4L ) ;
		  BufGetImage( 232, 170, 232+29, 170+14, jang_mis_image[0], mcga2 ) ;
		  BufGetImage( 201, 170, 201+29, 170+14, jang_mis_image[1], mcga2 ) ;
		  mis_fly_mode0 = 9  ;
		  mis_fly_mode1 = 1  ;
		  mis_garo_size = 29 ;
		  mis_sero_size = 14 ;
	       }                                                          */
	       if( stage.number == 9 ) //*********** Laser Stage **********
	       {
		  my_laser[0].attr = 1 ;
		  my_gun[0].weapon = 2 ;
	       }
	       if( stage.number == 10 ) //******* Return F16 stage *********
	       {
		  for( j=0; j<MAXMYAIR; j++ ) my_air[j].air  = 1 ;
                  for( j=1; j<MAXMYAIR; j++ ) my_air[j].kill = 1 ;
	       }
	       stage_count++ ;
	       break ;
      case 1 :
/*	       if( (stage.number == 3) || (stage.number == 6) )
	       {
		  if( stage.view_count == 4 )
		  {
		     for( i=0; i<MAX_MIS; i++ )
		     {
			if( i < (MAX_MIS/2) ) stage_mis[i].fly_mode = mis_fly_mode0 ;
			else                  stage_mis[i].fly_mode = mis_fly_mode1 ;
			if( random(50) == 25 ) new_mis( i ) ;
		     }
		     stage_mis_count = 0 ;
		     stage_count++ ;
		  }
	       }
	       else
	       {                                                          */
		  if( stage.view_count == 4 )
		  {
		     stage.view_count = 0 ;
		     if( stage.number == 6 ) stage_bom_count = 0 ;
		     stage_count++ ;
		  }
//	       }
	       break ;
      case 2 :
/*	       if( (stage.number == 3) || (stage.number == 6) )
	       {
		  mis_move_output() ;
		  if( stage_mis_count == 800 )
		  {
		     stage_count++ ;
		     stage_mis_count = 0 ;
		  }
		  else stage_mis_count++ ;
	       }                                                          */
	       if( stage.number == 6 )
	       {
		  if( stage_bom_count == 800 )
		  {
		     bomber.x = (MEMXMAX-100) ;
		     bomber.y = (MEMYMAX-100) ;
		     bomber.attr = 1 ;
		     bomber.body = 28 ;
		     bomber.fly_mode = 0 ;
		     for( j=0; j<BOM_GUN; j++ )
		     {
			bomber.gun[0][j].attr = 0 ;
			bomber.gun[1][j].attr = 0 ;
		     }
		     for( i=0; i<3; i++ )
		     {
			for( j=0; j<MAX_FLAG; j++ )
			{
			   bomber.flag[i][j].attr = 0 ;
			}
		     }
		     stage_bom_count = 0 ;
		     stage_count++ ;
		  }
		  else stage_bom_count++ ;
	       }

               if( stage.number == 7 )
               {
                  if( !my_air[0].kill_mode  && special_char_attr     &&
                      !stage.air_kill[_BF]  && !stage.air_kill[_F16] && !stage.air_kill[_F14]  && !stage.air_kill[_F15] &&
                      !stage.air_kill[_F18] && !stage.air_kill[_A10] && !stage.air_kill[_F117] && !stage.air_kill[_FSX] &&
                      !stage.air_kill[_F22] && !stage.air_kill[_MF1] && !stage.air_kill[_F4]   &&
                      !stage.end_view_count && !my_mis.attr )
                  {
                     if( (special_char_loop > 50) && (special_char_loop < 200) )
              	        XMSPutImage( 245, 140, 28, mcga1 ) ;
                     if( (special_char_loop > 50) && (special_char_loop < 100) )
  	                BufHprint( 20, 150, "     Ðq¸÷·¡´öa?", 169, mcga1 ) ;
                     if( (special_char_loop > 100) && (special_char_loop <150) )
	                BufHprint( 20, 150, "11¯¡ ¤wÐ·.. F16 4 ””a..", 169, mcga1 ) ;
                     if( (special_char_loop > 150) && (special_char_loop <200) )
	                BufHprint( 20, 150, "  ”a¯¡ ¸åÊµA —i´áˆe”a!", 169, mcga1 ) ;

                     if( (special_char_loop > 250) && (special_char_loop < 350) )
              	        XMSPutImage( 245, 140, 45, mcga1 ) ;
                     if( (special_char_loop > 250) && (special_char_loop <300) )
	                BufHprint( 20, 150, "  »¥¬÷! µa‹¡“e ˆå·©·¡”a.", 169, mcga1 ) ;
                     if( (special_char_loop > 300) && (special_char_loop <350) )
	                BufHprint( 20, 150, "a•¡ ˆ{·¡ •¡µÁº‰V”a. ˆa¸a!", 169, mcga1 ) ;

                     if( special_char_loop != 400 ) special_char_loop++ ;
                     else
                     {
                        special_char_attr = 0 ;
                        stage.air_new[_F16] = stage.air_kill[_F16] = 4 ;
	                Load_Air_Image( "F16PIC1", _F16, 1 ) ;

                        // ** Special Kim Gun il
	                my_air[4].x = 130 ;
	                my_air[4].y = 75  - 60 ;
                        my_air[4].body = 36 ;
		        my_air[4].fly_mode = 0 ;
		        my_air[4].wing_man_num = 0 ;
		        my_air[4].ai  = 4 ;
		        my_air[4].air = 0 ;
		        my_air[4].kill = 0 ;
                        my_air[4].kill_mode = 0 ;
                        my_air[4].kill_attr = 0 ;
                        my_air[4].kill_count = 0 ;
                     }
                  }
                  else
                  {
                     if( !my_air[0].kill_mode  && !special_char_attr    &&
                         !stage.air_kill[_BF]  && !stage.air_kill[_F16] && !stage.air_kill[_F14]  && !stage.air_kill[_F15] &&
                         !stage.air_kill[_F18] && !stage.air_kill[_A10] && !stage.air_kill[_F117] && !stage.air_kill[_FSX] &&
                         !stage.air_kill[_F22] && !stage.air_kill[_MF1] && !stage.air_kill[_F4]   &&
                         !stage.end_view_count && !my_mis.attr )
                         clear_scenario = 1 ;
                  }
               }

               if( stage.number == 14 )
               {
                  if( !my_air[0].kill_mode  && special_char_attr     &&
                      !stage.air_kill[_BF]  && !stage.air_kill[_F16] && !stage.air_kill[_F14]  && !stage.air_kill[_F15] &&
                      !stage.air_kill[_F18] && !stage.air_kill[_A10] && !stage.air_kill[_F117] && !stage.air_kill[_FSX] &&
                      !stage.air_kill[_F22] && !stage.air_kill[_MF1] && !stage.air_kill[_F4]   &&
                      !stage.end_view_count && !my_mis.attr )
                  {

                     if( (special_char_loop > 50) && (special_char_loop < 200) )
              	        XMSPutImage( 245, 140, 45, mcga1 ) ;
                     if( (special_char_loop > 50) && (special_char_loop <100) )
	                BufHprint( 40, 150, "¸÷·· ¬a•¡ —w¸w·¡¯¡”a!", 169, mcga1 ) ;
                     if( (special_char_loop > 100) && (special_char_loop <150) )
	                BufHprint( 40, 150, "µb¯¡ £¡Š…‹¡•¡ ¡´á·¶´öŠ…..", 169, mcga1 ) ;
                     if( (special_char_loop > 150) && (special_char_loop <200) )
	                BufHprint( 40, 150, "·¡¤å °a¶‘·e ¸we·¡ ´a“¥ˆé!", 169, mcga1 ) ;

                     if( special_char_loop != 250 ) special_char_loop++ ;
                     else
                     {
                        special_char_attr = 0 ;
	                stage.air_new[_FSX]  = stage.air_kill[_FSX]  = 2 ;
	                stage.air_new[_F117] = stage.air_kill[_F117] = 2 ;
	                Load_Air_Image( "F117PIC", _F117, 1 ) ;
	                Load_Air_Image( "FSXPIC", _FSX, 2 ) ;

                        // ** Special Kim Gun il
	                my_air[4].x = 130 ;
	                my_air[4].y = 75  - 60 ;
                        my_air[4].body = 36 ;
		        my_air[4].fly_mode = 0 ;
		        my_air[4].wing_man_num = 0 ;
		        my_air[4].ai  = 4 ;
		        my_air[4].air = 0 ;
		        my_air[4].kill = 0 ;
                        my_air[4].kill_mode = 0 ;
                        my_air[4].kill_attr = 0 ;
                        my_air[4].kill_count = 0 ;
                     }
                  }
                  else
                  {
                     if( !my_air[0].kill_mode  && !special_char_attr    &&
                         !stage.air_kill[_BF]  && !stage.air_kill[_F16] && !stage.air_kill[_F14]  && !stage.air_kill[_F15] &&
                         !stage.air_kill[_F18] && !stage.air_kill[_A10] && !stage.air_kill[_F117] && !stage.air_kill[_FSX] &&
                         !stage.air_kill[_F22] && !stage.air_kill[_MF1] && !stage.air_kill[_F4]   &&
                         !stage.end_view_count && !my_mis.attr )
                         clear_scenario = 1 ;
                  }
               }

               if( stage.number == 15 )
               {
                  if( !my_air[0].kill_mode  && special_char_attr     &&
                      !stage.air_kill[_BF]  && !stage.air_kill[_F16] && !stage.air_kill[_F14]  && !stage.air_kill[_F15] &&
                      !stage.air_kill[_F18] && !stage.air_kill[_A10] && !stage.air_kill[_F117] && !stage.air_kill[_FSX] &&
                      !stage.air_kill[_F22] && !stage.air_kill[_MF1] && !stage.air_kill[_F4]   &&
                      !stage.end_view_count && !my_mis.attr )
                  {

                     switch( special_char_attr )
                     {
                        case 1 :
                        if( (special_char_loop > 50) && (special_char_loop < 150) )
                        {
                           XMSPutImage( 245, 140, 45, mcga1 ) ;
	                   BufHprint( 10, 130, "1 Íe” ·±¢µÅ®.", 169, mcga1 ) ;
	                   BufHprint( 10, 150, "Í¢‰b‹¡“e ÊÐwÐaµv”a.", 169, mcga1 ) ;
	                   BufHprint( 10, 170, "‹¡»¡¡ ŠáÑÅÐa¸a!", 169, mcga1 ) ;
                        }
                        if( (special_char_loop > 150) && (special_char_loop < 250) )
                        {
              	           XMSPutImage( 245,    140,    28, mcga1 ) ;
                           XMSPutImage( 245+11, 140+25, 32, mcga1 ) ;
	                   BufHprint( 10, 130, "¸â‹¡ˆa ”á ¡©aµ¥”a.", 169, mcga1 ) ;
	                   BufHprint( 10, 150, "Í¢‰b‹¡Ÿi »¡Ç¡¡a ŠáÑÅÐaœa.", 169, mcga1 ) ;
	                   BufHprint( 10, 170, "ˆa –áŸi  {‰V”a.", 169, mcga1 ) ;
                        }
                        if( (special_char_loop > 250) && (special_char_loop < 350) )
                        {
                           XMSPutImage( 245, 140, 45, mcga1 ) ;
	                   BufHprint( 10, 130, "£¡´eÐa”a. »¥¬÷.", 169, mcga1 ) ;
	                   BufHprint( 10, 150, "¬wÈˆa a¨a¬á •¡¶‰® ´ô”a.", 169, mcga1 ) ;
	                   BufHprint( 10, 170, " å¸á ŠáÑÅÐa‰V”a. ¸i ¦ÈbÐe”a.", 169, mcga1 ) ;
                        }
                        if( (special_char_loop > 350) && (special_char_loop < 450) )
                        {
              	           XMSPutImage( 245,    140,    28, mcga1 ) ;
                           XMSPutImage( 245+11, 140+25, 32, mcga1 ) ;
	                   BufHprint( 10, 140, "¹½´a! –á“e ‰A  {‰aœa.", 169, mcga1 ) ;
	                   BufHprint( 10, 160, "¢ ¬i´a •©´aˆa‰V”a‰¡ ¸åÐº¡!", 169, mcga1 ) ;
                        }
                        if( (special_char_loop > 450) && (special_char_loop < 550) )
                        {
              	           XMSPutImage( 245,    140,    28, mcga1 ) ;
                           XMSPutImage( 245+11, 140+25, 32, mcga1 ) ;
	                   BufHprint( 10, 140, "µ¥”a! ¸÷¯¥¤a¼bÀaŸ¡¸a!", 169, mcga1 ) ;
	                   BufHprint( 10, 160, "FSX 3 ”.. ·©¥¥Š…·¡Š…!", 169, mcga1 ) ;
                        }
                        if( special_char_loop != 600 ) special_char_loop++ ;
                        else
                        {
	                   stage.air_new[_FSX] = stage.air_kill[_FSX] = 3 ;
                           Load_Air_Image( "FSXPIC", _FSX, 1 ) ;
                           special_char_attr++ ;
                           special_char_loop = 0 ;
                        }
                        break ;

                        case 2 :
                        if( (special_char_loop > 50) && (special_char_loop < 150) )
                        {
              	           XMSPutImage( 245,    140,    28, mcga1 ) ;
                           XMSPutImage( 245+11, 140+25, 32, mcga1 ) ;
	                   BufHprint( 10, 140, "·¡œé®ˆa...´a»¢ {a»¡ ´g´v”a!", 169, mcga1 ) ;
	                   BufHprint( 10, 160, "F15 2 ” ¸ó‹eº—...!", 169, mcga1 ) ;
                        }
                        if( special_char_loop != 200 ) special_char_loop++ ;
                        else
                        {
	                   stage.air_new[_F15] = stage.air_kill[_F15] = 2 ;
	                   Load_Air_Image( "F15PIC", _F15, 1 ) ;
                           special_char_attr++ ;
                        }
                        break ;

                        case 3 :
                        if( !my_air[0].kill_mode  && !stage.air_kill[_F15] &&
                            !stage.end_view_count && !my_mis.attr )
                        {
                            special_char_attr++ ;
                            special_char_loop = 0 ;
                        }
                        break ;

                        case 4 :
                        if( (special_char_loop > 50) && (special_char_loop < 150) )
                        {
                           XMSPutImage( 245, 140, 45, mcga1 ) ;
	                   BufHprint( 10, 140, "‰¡ s”a. »¥¬÷!", 169, mcga1 ) ;
	                   BufHprint( 10, 160, "·¡ ½¢·e ¡¡– ¢¬aÓ¡ ŠáÑÅÐ–”a!", 169, mcga1 ) ;
                        }
                        if( (special_char_loop > 150) && (special_char_loop < 250) )
                        {
              	           XMSPutImage( 245,    140,    28, mcga1 ) ;
                           XMSPutImage( 245+11, 140+25, 32, mcga1 ) ;
	                   BufHprint( 10, 140, "·¡½¢•¡ ”Â— À÷­¡ˆa {eˆõ ˆ{”a.", 169, mcga1 ) ;
	                   BufHprint( 10, 160, "ŠáÑÅÐa‰V·a“¡, ´ñÑ¡ ¦ÈbÐe”a.", 169, mcga1 ) ;
                        }
                        if( special_char_loop != 300 ) special_char_loop++ ;
                        else                           special_char_attr = 0 ;
                        break ;
                     }
                  }
                  else
                  {
                     if( !my_air[0].kill_mode  && !special_char_attr    &&
                         !stage.air_kill[_BF]  && !stage.air_kill[_F16] && !stage.air_kill[_F14]  && !stage.air_kill[_F15] &&
                         !stage.air_kill[_F18] && !stage.air_kill[_A10] && !stage.air_kill[_F117] && !stage.air_kill[_FSX] &&
                         !stage.air_kill[_F22] && !stage.air_kill[_MF1] && !stage.air_kill[_F4]   &&
                         !stage.end_view_count && !my_mis.attr )
                         clear_scenario = 1 ;
                  }
               }

               if( (stage.number != 7) && (stage.number != 14) && (stage.number != 15) )
               {
                  if( !my_air[0].kill_mode  &&
                      !stage.air_kill[_BF]  && !stage.air_kill[_F16] && !stage.air_kill[_F14]  && !stage.air_kill[_F15] &&
                      !stage.air_kill[_F18] && !stage.air_kill[_A10] && !stage.air_kill[_F117] && !stage.air_kill[_FSX] &&
                      !stage.air_kill[_F22] && !stage.air_kill[_MF1] && !stage.air_kill[_F4]   &&
                      !stage.end_view_count && !my_mis.attr )
                      clear_scenario = 1 ;
               }
	       break ;
      case 3 :
/*	       if( (stage.number == 3) || (stage.number == 6) )
	       {
		  mis_move_output() ;
		  if( stage_mis_count == 150 )
		  {
		     stage_count++ ;
		     stage.view_count = 0 ;
		     farfree( jang_mis_image[0] ) ;
		     farfree( jang_mis_image[1] ) ;
		  }
		  else stage_mis_count++ ;
	       }                                                          */
	       if( stage.number == 6 )
	       {
		  my_mis_target.x = bomber.x ;
		  my_mis_target.y = bomber.y ;
		  my_mis_target.attr = 2 ;
		  my_mis_target.target = 0 ;

		  bomber_move_output() ;
		  if( !my_air[0].kill && !bomber.body && bomber.attr &&
		      (stage_bom_count==30) )
		  {
		     my_mis_target.attr = 1 ;
		     bomber.fly_mode = 15 ;
                     clear_scenario = 1 ;          //******** Clear Scenario
		     stage.view_count = 0 ;
		     stage_bom_count = 0 ;
		     stage_count++ ;
		  }
		  else
		  {
		     if( !bomber.body ) stage_bom_count++ ;
		  }
	       }
	       break ;
      case 4 :
	       if( stage.number == 6 ) bomber_move_output() ;
	       break ;
      case 255 :
               if( panicmode )   //****** Panic Mode Air Clear Check *******
               {
                  if( !my_air[0].kill_mode  &&
                      !stage.air_kill[_BF]  && !stage.air_kill[_F16] && !stage.air_kill[_F14]  && !stage.air_kill[_F15] &&
                      !stage.air_kill[_F18] && !stage.air_kill[_A10] && !stage.air_kill[_F117] && !stage.air_kill[_FSX] &&
                      !stage.air_kill[_F22] && !stage.air_kill[_MF1] && !stage.air_kill[_F4]   &&
                      !stage.end_view_count && !my_mis.attr )
                      clear_scenario = 1 ;
               }
               else              //********** Orignal Story Mode ***********
               {
	          if( stage.number == 1 )
	          {
		     for( j=0; j<MAXMYAIR; j++ ) my_air[j].air = 0 ;
	          }
	          if( stage.number == 6 ) // ******* Bomber Kill Stage *******
	          {
		     bomber.attr = 0 ;
	          }
	          if( stage.number == 9 ) //*********** Laser Stage **********
	          {
		     my_laser[0].attr = 0 ;
		     my_gun[0].weapon = 1 ;
	          }
	          if( stage.number == 10 )
	          {
		     for( j=0; j<MAXMYAIR; j++ ) my_air[j].air = 0 ;
	          }
               }
   }
}