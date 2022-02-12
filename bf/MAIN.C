    //#####################################################################//
   //                                      Middle Game Team               //
  //      -=* BLUE FALCON PROGRAM *=-      Art By byulbram               //
 //                                        Program By Jo Jae Goo        //
//#####################################################################//

#include "bf.h"

#define PCM_MULT_GUN  "mgun"
#define PCM_KILL      "kill"
#define PCM_BURSTER1  "buster1"
#define PCM_BURSTER2  "buster2"
#define PCM_MISSILE   "missile"
#define PCM_TURBO     "turbo"
#define PCM_AIR_CRASH "aircrash"
#define PCM_GUN_CRASH "guncrash"
#define PCM_LASER     "laser"
#define PCM_LASER_BIM "laserbim"

#define XMS_IMG_MISSILE   0                    //************** 16 Missile
#define XMS_IMG_KILL      16                   //************** 8 Kill Image
#define XMS_IMG_WARNING0  24
#define XMS_IMG_WARNING1  25
#define XMS_IMG_RADER     26
#define XMS_IMG_TARGET    27
#define XMS_IMG_MASK      28
#define XMS_IMG_EYES      29                   //************** 9 Eyes Image
#define XMS_IMG_KILL_WING 38                   //************** 5 Kill Image
#define XMS_IMG_BACK0     43
#define XMS_IMG_BACK1     44
#define XMS_IMG_MASKS     45                   //************* 10 Masks Image
#define XMS_IMG_ROLL0     55                   //***** 8 Solgae Roll (RIGHT)
#define XMS_IMG_ROLL1     63                   //***** 8 Solgae Roll (LEFT)

#define OFF      0
#define ON       1

extern void new_char( int number, int mode ) ;
extern int VoiceHardWare ;

char XMS_air_select[11] ;                                  // Max Air Count ~
long         XMS_image_addr[80] = { 0L, } ;
unsigned int XMS_image_size[80] = { 0,  } ;

char far *image_buff ;
char far *gun_image[16] ;
char far *buster_image[3][2][16] ; // Buster Image [fly.ground][size][fly]
char far *flare_image[6] ;
char far *air_flag_image[MAXFLAG] ;
char far *mis_flag_image[MAXFLAG] ;

char bk_sx[2][16] = {
		       {-4,-3,-2,-1, 0, 1, 2, 3, 4, 3, 2, 1, 0,-1,-2,-3 },
		       {-8,-6,-4,-2, 0, 2, 4, 6, 8, 6, 4, 2, 0,-2,-4,-6 }
		    } ;
char bk_sy[2][16] = {
		       { 0, 1, 2, 3, 4, 3, 2, 1, 0,-1,-2,-3,-4,-3,-2,-1 },
		       { 0, 2, 4, 6, 8, 6, 4, 2, 0,-2,-4,-6,-8,-6,-4,-2 }
		    } ;

char speed_x[11][16] = {
		       {-17,-13, -9,-5, 0, 5,  9, 13, 17, 13,  9, 5, 0,-5, -9,-13 },
		       {-16,-12, -8,-4, 0, 4,  8, 12, 16, 12,  8, 4, 0,-4, -8,-12 },
		       {-18,-14,-10,-6, 0, 6, 10, 14, 18, 14, 10, 6, 0,-6,-10,-14 },
		       {-18,-14,-10,-6, 0, 6, 10, 14, 18, 14, 10, 6, 0,-6,-10,-14 },
		       {-16,-12, -8,-4, 0, 4,  8, 12, 16, 12,  8, 4, 0,-4, -8,-12 },
		       {-16,-12, -8,-4, 0, 4,  8, 12, 16, 12,  8, 4, 0,-4, -8,-12 },
		       {-16,-12, -8,-4, 0, 4,  8, 12, 16, 12,  8, 4, 0,-4, -8,-12 },
		       {-18,-14,-10,-6, 0, 6, 10, 14, 18, 14, 10, 6, 0,-6,-10,-14 },
		       {-18,-14,-10,-6, 0, 6, 10, 14, 18, 14, 10, 6, 0,-6,-10,-14 },
		       {-16,-12, -8,-4, 0, 4,  8, 12, 16, 12,  8, 4, 0,-4, -8,-12 },
		       {-16,-12, -8,-4, 0, 4,  8, 12, 16, 12,  8, 4, 0,-4, -8,-12 }
		    } ;
char speed_y[11][16] = {
		       { 0, 5,  9, 13, 17, 13,  9, 5, 0,-5, -9,-13,-17,-13, -9,-5 },
		       { 0, 4,  8, 12, 16, 12,  8, 4, 0,-4, -8,-12,-16,-12, -8,-4 },
		       { 0, 6, 10, 14, 18, 14, 10, 6, 0,-6,-10,-14,-18,-14,-10,-6 },
		       { 0, 6, 10, 14, 18, 14, 10, 6, 0,-6,-10,-14,-18,-14,-10,-6 },
		       { 0, 4,  8, 12, 16, 12,  8, 4, 0,-4, -8,-12,-16,-12, -8,-4 },
		       { 0, 4,  8, 12, 16, 12,  8, 4, 0,-4, -8,-12,-16,-12, -8,-4 },
		       { 0, 4,  8, 12, 16, 12,  8, 4, 0,-4, -8,-12,-16,-12, -8,-4 },
		       { 0, 6, 10, 14, 18, 14, 10, 6, 0,-6,-10,-14,-18,-14,-10,-6 },
		       { 0, 6, 10, 14, 18, 14, 10, 6, 0,-6,-10,-14,-18,-14,-10,-6 },
		       { 0, 4,  8, 12, 16, 12,  8, 4, 0,-4, -8,-12,-16,-12, -8,-4 },
		       { 0, 4,  8, 12, 16, 12,  8, 4, 0,-4, -8,-12,-16,-12, -8,-4 }
		    } ;

//*************************  BF  F16 F14 F15 F18 A10 F117 FSX F22 MF1 F4
char air_static_body[11] = { 18, 12, 16, 20, 14, 12, 12,  14, 18, 10, 12 } ;
char air_static_loop[11] = { 0,  0,  1,  2,  1,  2,  1,   1,  2,  2,  1  } ;
char air_static_mi_c[11] = { 6,  4,  4,  6,  4,  0,  4,   4,  6,  1,  2  } ;
char air_static_scor[11] = { 80, 20, 30, 50, 30, 30, 80,  70, 70, 30, 30 } ;

unsigned char mis_static_time[8] = { 10, 15, 20, 8,  15, 20, 10, 35 } ;
unsigned char mis_static_body[8] = { 10, 10, 12, 12, 14, 10, 14, 17 } ;
unsigned char mis_static_loop[8] = { 2,  6,  4,  1,  4,  2,  2,  6  } ;

char flare_code[] = { 0, 1, 2, 1, 2, 3, 4, 5 } ;

unsigned char stage_new_char_air[15][20] =
{
   { _MF1, _MF1, _MF1, },                             // 1 STAGE
   { _MF1, _F4,  _MF1, _F4, },
   { _F14, _F18, _F14, _F18, },
   { _F15, _MF1, _F15, _MF1, },
   { _A10, _A10, _A10, _F16, _F16, },                 // 5 STAGE
   { _MF1, _MF1, _MF1, _F15, _F15, _F15, },
   { _F15, _F15, _F15, _F16, _F16, _F16, _F16, },
   { _F16, _F16, _F117, _F117, _F117 },
   { _MF1, _MF1, _MF1, _F15, _F15, },
   { _F4,  _MF1, _MF1, },                             // 10 STAGE
   { _F14, _F14, _F18, _F18, _F18, },
   { _FSX, _FSX, _FSX, _MF1, _MF1, },
   { _FSX, _FSX, _F4,  _F4, },
   { _F15, _F15, _FSX, _FSX, _FSX, _FSX, _FSX, _F117, _F117, },
   { _F16, _F16, _F22, _F22, _F22, _FSX, _FSX, _FSX,  _F15,  _F15, }
} ;
unsigned char stage_new_char_ai[15][20]  =               // Char Air Ai(0~4)
{
   { 1, 1, 2, },                                         // 1 STAGE
   { 2, 2, 2, 2, },
   { 2, 2, 2, 2, },
   { 2, 3, 2, 3, },
   { 2, 2, 2, 2, 3, },                                   // 5 STAGE
   { 3, 2, 2, 3, 2, 1, },
   { 2, 2, 1, 3, 4, 3, 4, },
   { 2, 2, 2, 2, 2, },
   { 3, 2, 2, 2, 2, },
   { 2, 2, 1, },                                         // 10 STAGE
   { 3, 3, 3, 2, 2, },
   { 3, 2, 2, 3, 2, },
   { 2, 1, 2, 1, },
   { 3, 3, 3, 2, 2, 4, 2, 3, 3, },
   { 2, 3, 2, 3, 2, 3, 2, 2, 3, 2, }                      // 15 STAGE
} ;

char zeiper_gun_x[16] =        //*** Zeiper Special Mode ... Gun X Start Data
{
25, 41, 32, 27, 38, 38, 30, 13, 12, 12, 15, 16, 20, 23, 26, 30
} ;
char zeiper_gun_y[16] =        //*** Zeiper Special Mode ... Gun Y Start Data
{
 7,  0,  3,  3,  6,  4,  7,  1,  7, 16, 25, 27, 21, 20, 25, 16
} ;

int zeiper_laser_sx[16] =   //*** Zeiper Special Mode ... Laser X Start Data
{
187,185,174,164,172,168,153,136,133,138,144,149,154,163,166,176
} ;
int zeiper_laser_sy[16] =   //*** Zeiper Special Mode ... Laser Y Start Data
{
 86, 75, 75, 74, 73, 73, 74, 76, 86, 98,109,114,112,113,112,100
} ;
int zeiper_laser_ex[16] =   //*** Zeiper Special Mode ... Laser X End Data
{
320,320,255,190,172,150, 80, -1, -1, -1, 55,110,154,210,255,320
} ;
int zeiper_laser_ey[16] =   //*** Zeiper Special Mode ... Laser Y End Data
{
 86, 30, -1, -1, -1, -1, -1, 30, 86,150,200,200,200,200,200,150
} ;

unsigned char stage_new_char_mode ;
unsigned char stage_new_char_count ;

int  bk_x[2], bk_y[2] ;
char automatic = 1    ;      // 0 = Auto Air,   1 = Use Air
char panicmode = 0    ;      // 0 = Story Mode, 1 = Panic Mode
char panicattr        ;      // Char Air Circle Number ( 0 => 10 )
char panic_air_num    ;      // Panic Mode Air Circle Select ( 0 => 4 )
char clear_scenario   ;      // 0 = Not Clear.... 1 = Stage Clear

int start_x    ;
int start_y    ;
int start_file ;
int start_attr ;

int back_x ;
int back_y ;
char back_scroll_mode ;      // 0=default, 1=ground, 2=shadow
char back_scroll_speed ;
char bf_back_page[2] ;

long mis_delay ;
long mis_blink_delay ;
char mis_blink_attr  ;

int  oh_ch_gun_random ;
int  oh_ch_mis_random ;

unsigned char kill_pos_x[8] ;           // Air Kill Image X Center Pos
unsigned char kill_pos_y[8] ;           // Air Kill Image Y Center Pos
unsigned char kill_air[10] ;            // 0=F16,  1=F14, 2=F15, 3=F18, 4=A10
unsigned char total_kill_air[10] ;      // 5=F117, 6=FSX, 7=F22, 8=MF1, 9=F4
unsigned int  total_score ;
unsigned int  stage_score ;
unsigned int  top_score_board[7] ;       // Top Score 1 ~ 7
unsigned char top_score_name[7][35] ;    // Top Score Player Names 1 ~ 7
unsigned int  panic_score_board[7] ;     // Panic Score 1 ~ 7
unsigned char panic_score_name[7][35] ;  // Panic Score Player Names 1 ~ 7
char score_view_count ;
long score_view_time  ;
char speed = 1 ;                        // Game Speed
char rader_attr = 0 ;                   // Rader Attr (0==Small,1==Large)
char rader_scale_mode ;                 // Rader Scale Mode ( 0~Not, 1~Yes)
char rader_scale_data_x ;               // Rader Scale Data X
char rader_scale_data_y ;               // Rader Scale Data Y
char my_air_crash_attr = 0 ;            // My Air Crash On(1) Off(0) Check
char my_air_crash_num = 0 ;             // My Air Crash Ch Number
char ch_air_crash_attr = 0 ;            // Ch Air Crash On(1) Off(0) Check
char ch_air_crash_num = 0 ;             // Ch Air Crash Ch Number
char JoyStickOnOff = 0 ;                // Joystick 1=On 0=Off (Default=Off)
int  joy_x, joy_y, joy_b ;              // Joystick X, Y, Button Value
char st[80] ;                           // String
char exit_code ;                        // Exit Code ( 0 = EXIT, 1 = PLAY )
int stage_title_pos_x ;                 // View Title Position X
int stage_title_pos_y ;                 // View Title Position Y
int stage_mask_pos_x ;                  // Stage Start Mask X Pos
int stage_mask_pos_y ;                  // Stage Start Mask Y Pos

struct stage_data stage ;
struct turbo_data my_turbo[MAXMYAIR] ;
struct my_air_data my_air[MAXMYAIR] ;
struct my_air_target_data my_air_target[MAXMYAIR] ;
struct wing_man_data wing_man[10] ;
struct buster_data buster ;

struct ch_air_data {
   int  x ;
   int  y ;
   char body ;               // Ch Air Health
   char fly_mode ;           // Flying Mode 0-15
   char mis_count ;          // Char Air(F16,F14,F15...) Missile Count
   char flare ;              // Flash Count
   char kill ;               // 0 = Lefe, 1 = Kill
   char kill_attr ;          // Char Kill Attrib
   char air  ;               // Air Attrib (0=bf,1=f16,2=f14,3=f15)
   char loop ;               // Lotate Loop
   char fly_attr ;           // 0 = LEFT, 9 = RIGHT
   char fly_count ;          // Char Air Lotate Count
   char ai        ;          // 0 = Low, ~ 4 = HIGH
} ch_air[MAXCHAIR] ;

struct my_gun_data my_gun[MAXMYAIR] ;

struct ch_gun_data {
   int  x[MAXGUN] ;
   int  y[MAXGUN] ;
   int  attr[MAXGUN] ;
   char fly_mode[MAXGUN] ;
   char count ;
} ch_gun[MAXCHAIR] ;

struct my_mis_target_data my_mis_target ;
struct my_mis_data my_mis ;

struct ch_mis_data {
   char weapon ;
   int  x ;
   int  y ;
   int  attr ;
   char fly_mode ;
   char count ;
   char loop ;
   unsigned char lotate_count ;          // 150 Count Loop
   char lotate_fly_mode ;
} ch_mis = { 0, 0, 0, 0, 0, 0, 0, 0, 0 } ;

struct my_laser_data my_laser[MAXMYAIR] ;

struct my_flare_data {
   int  x ;
   int  y ;
   char attr ;
   char count ;
} my_flare[MAXMYAIR] ;

struct ch_flare_data {
   int  x ;
   int  y ;
   char attr ;
   char count ;
} ch_flare[MAXCHAIR] ;

struct my_flag_data {
   int  x[MAXFLAG] ;
   int  y[MAXFLAG] ;
   char attr[MAXFLAG] ;
   char count[MAXFLAG] ;
} my_flag[MAXMYAIR] ;

struct ch_flag_data {
   int  x[MAXFLAG] ;
   int  y[MAXFLAG] ;
   char attr[MAXFLAG] ;
   char count[MAXFLAG] ;
} ch_flag[MAXCHAIR] ;

struct my_mis_flag_data {
   int  x[MAXFLAG] ;
   int  y[MAXFLAG] ;
   char attr[MAXFLAG] ;
   char count[MAXFLAG] ;
} my_mis_flag ;

struct ch_mis_flag_data {
   int  x[MAXFLAG] ;
   int  y[MAXFLAG] ;
   char attr[MAXFLAG] ;
   char count[MAXFLAG] ;
} ch_mis_flag ;

struct my_wing_data {
   int  x ;
   int  y ;
   char fly_mode ;
   char img_num ;
   int  flag_x[4] ;
   int  flag_y[4] ;
   char flag_count[4] ;
} my_wing[MAXMYAIR][5] ;

struct ch_wing_data {
   int  x ;
   int  y ;
   char fly_mode ;
   char img_num ;
   int  flag_x[4] ;
   int  flag_y[4] ;
   char flag_count[4] ;
} ch_wing[MAXCHAIR][5] ;

struct eye_ani_data {
   char attr ;
   char count ;
   long ani_time ;
} eye_ani ;

void stage_data_init( void )
{
   int i, j ;

   image_buff = (char far *)farmalloc( 20000L ) ;            // Image Buffer
   for( i=0; i<16; i++ ) gun_image[i] = (char far *)farmalloc( (9L*9L)+4L ) ;
   for( j=0; j<3; j++ )
   {
      for( i=0; i<16; i++ )
      {
         buster_image[j][0][i] = (char far *)farmalloc( (11L*11L)+4L ) ;
	     buster_image[j][1][i] = (char far *)farmalloc( (22L*19L)+4L ) ;
      }
   }
   for( i=0; i<6;  i++ ) flare_image[i] = (char far *)farmalloc( (22L*18L)+4L ) ;

   if( !panicmode ) scenario_start() ;        // Scenario Data Initialize
   Buster_XY_Data_Load() ;                    // Game Air Buster XY Load
}

void bf_data_close( void )
{
   int i, j ;

   farfree( image_buff ) ;
   for( i=0; i<16; i++ ) farfree( gun_image[i] ) ;
   for( j=0; j<3; j++ )
   {
      for( i=0; i<16; i++ )
      {
    	 farfree( buster_image[j][0][i] ) ;
	     farfree( buster_image[j][1][i] ) ;
      }
   }
   for( i=0; i< 6; i++ ) farfree( flare_image[i] ) ;
   for( i=0; i<MAXFLAG; i++ ) farfree( air_flag_image[i] ) ;
   for( i=0; i<MAXFLAG; i++ ) farfree( mis_flag_image[i] ) ;

   if( !panicmode ) scenario_close() ;        // Scenario Data Close
}

void XMSGetImage( int sx, int sy, int ex, int ey, int XMS_number, char *sour_buff )
{
   long xms_off_set = 0L ;

   BufGetImage( sx, sy, ex, ey, image_buff, sour_buff ) ;
   if( XMS_number )
   {
      xms_off_set = XMS_image_addr[XMS_number-1] + XMS_image_size[XMS_number-1] ;
   }

   XMS_image_addr[XMS_number] = xms_off_set ;
   XMS_image_size[XMS_number] = (ex-sx+1L)*(ey-sy+1L)+4L ;
   memtoxms( image_buff, XMS_image, xms_off_set, XMS_image_size[XMS_number] ) ;
//   printf("\n Read XMS Memory = %u Byte", XMS_image_size[XMS_number] ) ; getch();
}

void XMSPutImage( int sx, int sy, int XMS_number, char *dest_buff )
{
   xmstomem( XMS_image, XMS_image_addr[XMS_number], image_buff, XMS_image_size[XMS_number] ) ;
   BufSpritePutImage( sx, sy, image_buff, dest_buff ) ;
}

int air_sx[16] = { 0,  60,  120, 180, 0,  60,  120, 180, 0,   60,  120, 180, 0,   60,  120, 180 } ;
int air_sy[16] = { 0,  0,   0,   0,   50, 50,  50,  50,  100, 100, 100, 100, 150, 150, 150, 150 } ;
int air_ex[16] = { 59, 119, 179, 239, 59, 119, 179, 239, 59,  119, 179, 239, 59,  119, 179, 239 } ;
int air_ey[16] = { 49, 49,  49,  49,  99, 99,  99,  99,  149, 149, 149, 149, 199, 199, 199, 199 } ;

void read_image( void )
{
   int kisx[] = {   1,  51, 128, 214,  4, 81, 152, 204 } ;
   int kisy[] = {   1,   1,   1,   1,  3,  3,  21,  50 } ;
   int kixs[] = {  48,  76,  85,  84, 74, 68,  48,  17 } ;
   int kiys[] = {  41,  57,  68,  71, 61, 61,  43,  14 } ;

   int fisx[MAXFLAG] = {   1,   6,  17,  33,  57,  93, 129 } ;
   int fisy[MAXFLAG] = {  70,  70,  70,  70,  70,  70,  70 } ;
   int fixs[MAXFLAG] = {   4,  10,  15,  23,  35,  35,  36 } ;
   int fiys[MAXFLAG] = {   3,   9,  12,  19,  27,  30,  31 } ;

   int i, j ;

   for( i=0; i<MAXFLAG; i++ )
   {
      air_flag_image[i] = (char far *)farmalloc( fixs[i]*fiys[i]+4L ) ;
      mis_flag_image[i] = (char far *)farmalloc( fixs[i]*fiys[i]+4L ) ;
   }

   ViewIMG( "f16pic1", mcga1 ) ;
   i = 0 ;
   for( j=0; j<8; j++, i++ )
      BufGetImage( 294,    1+(j*8),  294+10,    1+(j*8)+7,  buster_image[0][0][i], mcga1 ) ;
   for( j=0; j<8; j++, i++ )
      BufGetImage( 294+11, 1+(j*8),  294+11+10, 1+(j*8)+7,  buster_image[0][0][i], mcga1 ) ;

   i = 0 ;
   for( j=0; j<8; j++, i++ )
      BufGetImage( 246,    1+(j*19), 246+21,    1+(j*19)+18, buster_image[0][1][i], mcga1 ) ;
   for( j=0; j<8; j++, i++ )
      BufGetImage( 246+22, 1+(j*19), 246+22+21, 1+(j*19)+18, buster_image[0][1][i], mcga1 ) ;

   ViewIMG( "solgae", mcga1 ) ;
   i = 0 ;
   for( j=0; j<8; j++, i++ )
      BufGetImage( 294,    1+(j*8),  294+10,    1+(j*8)+7,  buster_image[1][0][i], mcga1 ) ;
   for( j=0; j<8; j++, i++ )
      BufGetImage( 294+11, 1+(j*8),  294+11+10, 1+(j*8)+7,  buster_image[1][0][i], mcga1 ) ;

   i = 0 ;
   for( j=0; j<8; j++, i++ )
      BufGetImage( 246,    1+(j*19), 246+21,    1+(j*19)+18, buster_image[1][1][i], mcga1 ) ;
   for( j=0; j<8; j++, i++ )
      BufGetImage( 246+22, 1+(j*19), 246+22+21, 1+(j*19)+18, buster_image[1][1][i], mcga1 ) ;

   ViewIMG( "solgae-g", mcga1 ) ;
   i = 0 ;
   for( j=0; j<8; j++, i++ )
      BufGetImage( 294,    1+(j*11),  294+10,    1+(j*11)+10,  buster_image[2][0][i], mcga1 ) ;
   for( j=0; j<8; j++, i++ )
      BufGetImage( 294+11, 1+(j*11),  294+11+10, 1+(j*11)+10,  buster_image[2][0][i], mcga1 ) ;

   i = 0 ;
   for( j=0; j<8; j++, i++ )
      BufGetImage( 246,    1+(j*19), 246+21,    1+(j*19)+18, buster_image[2][1][i], mcga1 ) ;
   for( j=0; j<8; j++, i++ )
      BufGetImage( 246+22, 1+(j*19), 246+22+21, 1+(j*19)+18, buster_image[2][1][i], mcga1 ) ;

   ViewIMG( "bf2pic", mcga1 ) ;
   i = 0 ;
   for( j=0; j<8; j++, i++ )
      XMSGetImage( 1+(j*32), 102,    1+(j*32)+30, 102+21,    XMS_IMG_MISSILE+i, mcga1 ) ;

   for( j=0; j<8; j++, i++ )
      XMSGetImage( 1+(j*32), 102+23, 1+(j*32)+30, 102+23+21, XMS_IMG_MISSILE+i, mcga1 ) ;

   for( i=0; i<4; i++ )
   {
      XMSGetImage( kisx[i], kisy[i], kisx[i]+kixs[i]-1, kisy[i]+kiys[i]-1, XMS_IMG_KILL+i, mcga1 ) ;
      kill_pos_x[i] = (*image_buff>>1) ;
      kill_pos_y[i] = (*(image_buff+2)>>1) ;
   }

   ViewIMG( "bf4pic", mcga1 ) ;
   for( i=4; i<8; i++ )
   {
      XMSGetImage( kisx[i], kisy[i], kisx[i]+kixs[i]-1, kisy[i]+kiys[i]-1, XMS_IMG_KILL+i, mcga1 ) ;
      kill_pos_x[i] = (*image_buff>>1) ;
      kill_pos_y[i] = (*(image_buff+2)>>1) ;
   }

   ViewIMG( "bf2pic", mcga1 ) ;
   for( i=0; i<MAXFLAG; i++ )
   {
      BufGetImage( fisx[i], fisy[i], fisx[i]+fixs[i]-1, fisy[i]+fiys[i]-1, air_flag_image[i], mcga1 ) ;
      BufGetImage( fisx[i], fisy[i]+78, fisx[i]+fixs[i]-1, fisy[i]+fiys[i]+78-1, mis_flag_image[i], mcga1 ) ;
   }

     i = 0 ;
     for( j=0; j<8; j++, i++ )
	BufGetImage( 300, 1+(j*10), 300+9-1, 1+(j*10)+9-1, gun_image[i], mcga1 ) ;

     for( j=0; j<8; j++, i++ )
	BufGetImage( 310, 1+(j*10), 310+9-1, 1+(j*10)+9-1, gun_image[i], mcga1 ) ;

     for( j=0; j<6; j++ )
	BufGetImage( 1+(j*25), 180, 1+(j*25)+21, 180+17, flare_image[j], mcga1 ) ;

   XMSGetImage( 167, 80,  167+89, 80+20,  XMS_IMG_WARNING0, mcga1 ) ;
   XMSGetImage( 167, 148, 167+89, 148+20, XMS_IMG_WARNING1, mcga1 ) ;
   XMSGetImage( 258, 80,  258+46, 80+35,  XMS_IMG_RADER,    mcga1 ) ;
   XMSGetImage( 258, 118, 258+34, 118+21, XMS_IMG_TARGET,   mcga1 ) ;

   BufGetImage( 10, 45, 10+29, 45+14, mouse_shape, mcga1 ) ;

   ViewIMG( "bf3pic", mcga1 ) ;
   XMSGetImage( 0, 0, 62, 51, XMS_IMG_MASK, mcga1 ) ;
   i = 0 ;
   for( j=0; j<4; j++, i++ )
      XMSGetImage( 64+(j*30), 0,  64+(j*30)+27, 10,    XMS_IMG_EYES+i, mcga1 ) ;
   for( j=0; j<3; j++, i++ )
      XMSGetImage( 64+(j*30), 12, 64+(j*30)+27, 12+10, XMS_IMG_EYES+i, mcga1 ) ;
   for( j=0; j<2; j++, i++ )
      XMSGetImage( 64+(j*30), 24, 64+(j*30)+27, 24+10, XMS_IMG_EYES+i, mcga1 ) ;

   ViewIMG( "bf4pic", mcga1 ) ;
   for( i=0; i<5; i++ )
      XMSGetImage( 4+(i*34), 66, 4+(i*34)+33, 66+34, XMS_IMG_KILL_WING+i, mcga1 ) ;

   ViewIMG( "bfbk1", mcga1 ) ;
   XMSGetImage( 0, 0,  219, 86,    XMS_IMG_BACK0, mcga1 ) ;
   XMSGetImage( 0, 87, 225, 87+44, XMS_IMG_BACK1, mcga1 ) ;

   ViewIMG( "bf3pic", mcga1 ) ;
   for( i=0, j=0; i<5; i++, j++ )
      XMSGetImage( 64*j, 53, (64*j)+62, 53+51, XMS_IMG_MASKS+i, mcga1 ) ;
   for( i=5, j=0; i<10; i++, j++ )
      XMSGetImage( 64*j, 53+52, (64*j)+62, 53+52+51, XMS_IMG_MASKS+i, mcga1 ) ;

   ViewIMG( "roll", mcga1 ) ;
   for( i=0,j=0; i<6; i++, j++ )
      XMSGetImage( 1+(53*i), 1+(34*0), 1+(53*i)+51, 1+(34*0)+32, XMS_IMG_ROLL0+j, mcga1 ) ;
   for( i=0; i<2; i++, j++ )
      XMSGetImage( 1+(53*i), 1+(34*1), 1+(53*i)+51, 1+(34*1)+32, XMS_IMG_ROLL0+j, mcga1 ) ;
   for( i=0,j=0; i<6; i++, j++ )
      XMSGetImage( 1+(53*i), 1+(34*2), 1+(53*i)+51, 1+(34*2)+32, XMS_IMG_ROLL1+j, mcga1 ) ;
   for( i=0; i<2; i++, j++ )
      XMSGetImage( 1+(53*i), 1+(34*3), 1+(53*i)+51, 1+(34*3)+32, XMS_IMG_ROLL1+j, mcga1 ) ;

   AniColor() ;
}

void bf_init_data( void )
{
   int init_total_exp[10] = { 25, 0,  0,  0,  0,  0,  0,  0,  0,  0  } ;
   int init_level[10]     = { 4,  3,  3,  2,  2,  2,  2,  1,  1,  1  } ;
   int init_sungjang[10]  = { 0,  30, 40, 30, 20, 40, 10, 50, 10, 60 } ;
   int i ;                    // (Number 0) Kim Gun Il , Special Wing Man

   oh_ch_gun_random = 30  + ( 5  * 8 ) ;
   oh_ch_mis_random = 200 + ( 30 * 8 ) ;

   for( i=0; i<10; i++ )
   {
      wing_man[i].kill = 0 ;                       // Wing Man Life On
      wing_man[i].stage_exp = 0 ;                  // Wing Man Stage Exp Zero
      wing_man[i].total_exp = init_total_exp[i] ;  // Wing Man Total Exp
      wing_man[i].level     = init_level[i] ;      // Wing Man Level
      wing_man[i].sungjang  = init_sungjang[i] ;   // Wing Man Sung Jang
   }

   strcpy( wing_man[0].name, "‹±ˆå·©" ) ;
   strcpy( wing_man[1].name, "‹±¶wÑe" ) ;
   strcpy( wing_man[2].name, "Àa¸÷Ñ¡" ) ;
   strcpy( wing_man[3].name, "´e¡wÑÅ" ) ;
   strcpy( wing_man[4].name, "·¡  Ðe" ) ;
   strcpy( wing_man[5].name, "¤b–q" ) ;
   strcpy( wing_man[6].name, "Ðe¸÷¶…" ) ;
   strcpy( wing_man[7].name, "·A¹AÑw" ) ;
   strcpy( wing_man[8].name, "¸wº…Àé" ) ;
   strcpy( wing_man[9].name, "‘¡‰w·©" ) ;

   for( i=0; i<9; i++ )
   {
      select_man[i]  = 0 ;              // Wing Man Number Clear ( 0~9 )
      select_attr[i] = 0 ;              // Wing Man ATTR Clear ( 0,1 )
   }

   for( i=0; i<10; i++ )
   {
      kill_air[i] = 0 ;
      total_kill_air[i] = 0 ;
   }
   total_score = 0 ;

   stage.number = 0 ;
   score_view_count = 0 ;

   panicattr     = 0 ;      // Panic Char Air Circle Number ( 0 => 10 )
   panic_air_num = 0 ;      // Panic Mode Air Circle Select ( 0 => 4 )

   exit_code = 1 ;

   SetRgbPaletteSnow( (unsigned char *)Pal ) ;
}

void Load_Air_Image( char load_air_name[20], int xms_air_number, int xms_air_page )
{
   int i ;

   memtoxms( mcga1, XMS_hand, 0L, 64000 ) ;

   ViewIMG( load_air_name, mcga1 ) ;
   for( i=0; i<16; i++ )
   {
      BufGetImage( air_sx[i], air_sy[i], air_ex[i], air_ey[i], image_buff, mcga1 ) ;
      memtoxms( image_buff, XMS_air, (long)xms_air_page*64000L+(long)i*3004L, 3004 ) ;
   }
   XMS_air_select[xms_air_number] = xms_air_page ;

   if( !strcmp( load_air_name, "SOLGAE"   ) || !strcmp( load_air_name, "SOLGAE-N"   ) ||
       !strcmp( load_air_name, "F14PIC"   ) || !strcmp( load_air_name, "F15PIC"     ) ||
       !strcmp( load_air_name, "F15PIC-N" ) || !strcmp( load_air_name, "F18PIC"     ) ||
       !strcmp( load_air_name, "F22PIC"   ) ) buster.img_num[xms_air_number] = 1 ;
   else
   {
      if( !strcmp( load_air_name, "SOLGAE-G" ) ) buster.img_num[xms_air_number] = 2 ;
      else                                       buster.img_num[xms_air_number] = 0 ;
   }

   xmstomem( XMS_hand, 0L, mcga1, 64000 ) ;
}

void Load_Back_Image( int number )
{
   sprintf( st, "bfback%d0", number ) ;
   ViewIMG( st, mcga1 ) ;
   memtoxms( mcga1, XMS_back, 0L, 64000 ) ;

   sprintf( st, "bfback%d1", number ) ;
   ViewIMG( st, mcga1 ) ;
   memtoxms( mcga1, XMS_back, 64000L, 64000 ) ;

   sprintf( st, "bfback%d2", number ) ;
   ViewIMG( st, mcga1 ) ;
   memtoxms( mcga1, XMS_back, 128000L, 64000 ) ;

   sprintf( st, "bfback%d3", number ) ;
   ViewIMG( st, mcga1 ) ;
   memtoxms( mcga1, XMS_back, 192000L, 64000 ) ;
}

void set_stage( void )
{
   int i, j ;

   stage.number++ ;

   if( !panicmode )                           //**** Stage Start Visual ****
   {
      unsigned char iPal[256][3] ;

      black_palette( 1 ) ;                    // Set Slow Black Palette
      BufXLine( 0, 0, 64000, 0, mcga0 ) ;     // Screen Black Clear
      sprintf( st, "stage%ds", stage.number ) ;
      if( CheckFLI( st ) )
      {
         memtoxms( mcga1, XMS_hand, 0L,     64000L ) ;
         memtoxms( mcga2, XMS_hand, 64000L, 64000L ) ;
         memcpy( &iPal, &Pal, 768L ) ;           // Palette Backup
         ViewFLI( st, 0 ) ;                      // FLIC Animation
         black_palette( 1 ) ;                    // Set Slow Black Palette
         BufXLine( 0, 0, 64000, 0, mcga0 ) ;     // Screen Black Clear
         memcpy( &Pal, &iPal, 768L ) ;           // Palette Restore
         xmstomem( XMS_hand, 0L,     mcga1, 64000L ) ;
         xmstomem( XMS_hand, 64000L, mcga2, 64000L ) ;
      }
      SetRgbPaletteSnow( (unsigned char *)Pal ) ;
   }

   stage.end_view_count = 0 ;

   oh_ch_gun_random -= 5 ;
   oh_ch_mis_random -= 30 ;
   stage_score = 0 ;
   clear_scenario = 0 ;

   for( j=0; j<MAXMYAIR; j++ )
   {
      my_air[j].x = 130 ;
      my_air[j].y = 75 ;
      my_air[j].fly_mode = 0 ;
      my_air[j].kill = 1 ;
      my_air[j].kill_mode = 0 ;
      my_air[j].kill_attr = 0 ;
      my_air[j].kill_count = 0 ;
      my_air[j].loop = 0 ;
      my_air[j].fly_attr = 0 ;
      my_air[j].fly_count = 0 ;
      my_air[j].wing_man_num = 0 ;
      my_air[j].ai = 4 ;
      my_air[j].roll_attr = 0 ;
      my_air[j].roll_count = 0 ;
      my_air[j].roll_delay = 0 ;

      my_air_target[j].attr = 0 ;
      my_air_target[j].target = 0 ;
      my_air_target[j].mes_attr = 0 ;
      my_air_target[j].mes_air  = 0 ;
      my_air_target[j].mes_time = 0 ;

      my_flare[j].x = 0 ;
      my_flare[j].y = 0 ;
      my_flare[j].attr = 0 ;
      my_flare[j].count = 0 ;

      my_turbo[j].attr = 0 ;
      my_turbo[j].count = 0 ;
   }
   my_air[0].kill = 0 ;

   for( j=0; j<MAXCHAIR; j++ )
   {
      ch_flare[j].x = 0 ;
      ch_flare[j].y = 0 ;
      ch_flare[j].attr = 0 ;
      ch_flare[j].count = 0 ;
   }

   for( j=0; j<MAXMYAIR; j++ )
   {
      my_laser[j].attr = 0 ;
      my_laser[j].count = 0 ;

      my_gun[j].weapon = 1 ;                     // Weapon 0=No,1=Gun,2=Laser
      for( i=0; i<MAXGUN; i++ )                  // Air Gun Init
      {
	     my_gun[j].x[i] = 0 ;
	     my_gun[j].y[i] = 0 ;
	     my_gun[j].attr[i] = 0 ;
	     my_gun[j].fly_mode[i] = 0 ;
      }
      my_gun[j].count = 0 ;
   }

   for( i=0; i<MAXFLAG; i++ )                     // Air Flag Init
   {
      my_mis_flag.x[i] = 0 ;
      my_mis_flag.y[i] = 0 ;
      my_mis_flag.attr[i] = 0 ;
      my_mis_flag.count[i] = 0 ;

      ch_mis_flag.x[i] = 0 ;
      ch_mis_flag.y[i] = 0 ;
      ch_mis_flag.attr[i] = 0 ;
      ch_mis_flag.count[i] = 0 ;
   }

   my_mis.attr = 0 ;

   ch_mis.attr = 0 ;
   ch_mis.weapon = 0 ;

   my_mis_target.x = 0 ;
   my_mis_target.y = 0 ;
   my_mis_target.attr = 0 ;
   my_mis_target.target = 0 ;

   for( j=0; j<MAXCHAIR; j++ )
   {
      ch_air[j].x = 30000 ;                       // Start Hidden Char Air
      ch_air[j].y = 30000 ;
      ch_air[j].kill = 1 ;
      ch_air[j].kill_attr = 0 ;

      ch_air[j].fly_attr = 0 ;
      ch_air[j].fly_count = 0 ;

      for( i=0; i<MAXGUN; i++ )                   // Air Gun Init
      {
         ch_gun[j].x[i] = 0 ;
	 ch_gun[j].y[i] = 0 ;
	 ch_gun[j].attr[i] = 0 ;
	 ch_gun[j].fly_mode[i] = 0 ;
	 ch_gun[j].count = 0 ;
      }
   }

   eye_ani.attr  = 0 ;
   eye_ani.count = 0 ;

   for( i=0; i<10; i++ ) kill_air[i] = 0 ;

   stage_new_char_mode = 1 ;
   stage_new_char_count = 0 ;

   for( i=0; i<11; i++ )
   {
      stage.air_new[i] = 0 ;
      stage.air_kill[i] = 0 ;
   }

   buster.size = 1 ;
   for( i=0; i<11; i++ ) buster.img_num[i] = 0 ;

   switch( stage.number )
   {
      case 1 :
           if( panicmode )
           {
              //*********** Blue Falcon Panic Mode ************
              panicattr = 0 ;
	      strcpy( stage.name, "Panic Mode" ) ;
	      stage.air_new[_MF1] = stage.air_kill[_MF1] = 1 ;
              if( my_air[0].air == _MF1 ) Load_Air_Image( "MF1PIC",  _MF1, 0 ) ;
              if( my_air[0].air == _F4  ) Load_Air_Image( "F4PIC",   _F4,  0 ) ;
              if( my_air[0].air == _F16 ) Load_Air_Image( "F16PIC2", _F16, 0 ) ;
              if( my_air[0].air == _F14 ) Load_Air_Image( "F14PIC",  _F14, 0 ) ;
              if( my_air[0].air == _F15 ) Load_Air_Image( "F15PIC",  _F15, 0 ) ;
              if( my_air[0].air == _F18 ) Load_Air_Image( "F18PIC",  _F18, 0 ) ;
              if( my_air[0].air == _F22 ) Load_Air_Image( "F22PIC",  _F22, 0 ) ;
              if( my_air[0].air == _FSX ) Load_Air_Image( "FSXPIC",  _FSX, 0 ) ;
              if( my_air[0].air == _BF  ) Load_Air_Image( "SOLGAE",  _BF,  0 ) ;
	      Load_Air_Image( "MF1PIC", _MF1, 1 ) ;
	      buster.attr = 0 ;                       // 0 = Sky, 1 = Ground
	      back_scroll_mode = 0 ;
	      back_scroll_speed = 3 ;
	      Load_Back_Image( 6 ) ;
           }
           else
           {
	       //************ Shadow Sprite Mode **************
	       strcpy( stage.name, "'1 ¸w : Š…‰·©Ðb· ¬aa·¡'") ;
	       stage.air_new[_MF1] = stage.air_kill[_MF1] = 3 ;
	       Load_Air_Image( "F16PIC2", _F16, 0 ) ;  // Load Air Image Data
	       Load_Air_Image( "MF1PIC", _MF1, 1 ) ;
	       buster.attr = 0 ;                       // 0 = Sky, 1 = Ground
//	       back_scroll_mode = 2 ;                  // New Scroll Test
	       back_scroll_mode = 0 ;
	       back_scroll_speed = 1 ;
	       Load_Back_Image( 4 ) ;
           }
	       break ;
      case 2 :
	       strcpy( stage.name, "'2 ¸w : ÎŸe ! i´aµ¡Ÿa”a'" ) ;
	       stage.air_new[_F4]  = stage.air_kill[_F4]  = 2 ;
	       stage.air_new[_MF1] = stage.air_kill[_MF1] = 2 ;
	       Load_Air_Image( "SOLGAE-N", _BF, 0 ) ;  // Load Air Image Data
	       Load_Air_Image( "F4PIC", _F4, 1 ) ;
	       Load_Air_Image( "MF1PIC", _MF1, 2 ) ;
	       buster.attr = 0 ;                       // 0 = Sky, 1 = Ground
	       back_scroll_mode = 0 ;
	       back_scroll_speed = 1 ;
	       Load_Back_Image( 1 ) ;
	       break ;
      case 3 :
	       strcpy( stage.name, "'3 ¸w : £¡ ÐŠ…· •¡¸å'" ) ;
	       stage.air_new[_F14] = stage.air_kill[_F14] = 2 ;
	       stage.air_new[_F18] = stage.air_kill[_F18] = 2 ;
	       Load_Air_Image( "SOLGAE", _BF, 0 ) ;    // Load Air Image Data
	       Load_Air_Image( "F14PIC", _F14, 1 ) ;
	       Load_Air_Image( "F18PIC", _F18, 2 ) ;
	       buster.attr = 0 ;                       // 0 = Sky, 1 = Ground
	       back_scroll_mode = 0 ;
	       back_scroll_speed = 2 ;
	       Load_Back_Image( 6 ) ;
	       break ;
      case 4 :
	       strcpy( stage.name, "'4 ¸w : Ða“i·i ´è“e ¸a'" ) ;
	       stage.air_new[_F15] = stage.air_kill[_F15] = 2 ;
	       stage.air_new[_MF1] = stage.air_kill[_MF1] = 2 ;
	       Load_Air_Image( "SOLGAE", _BF, 0 ) ;    // Load Air Image Data
	       Load_Air_Image( "F15PIC", _F15, 1 ) ;
	       Load_Air_Image( "MF1PIC", _MF1, 2 ) ;
	       buster.attr = 0 ;                       // 0 = Sky, 1 = Ground
	       back_scroll_mode = 0 ;
	       back_scroll_speed = 1 ;
	       Load_Back_Image( 0 ) ;
	       break ;
      case 5 :
	       //*********** GROUND ATTACK ************
	       strcpy( stage.name, "'5 ¸w : ·©¥¥Š… ¬wŸB'" ) ;
	       stage.air_new[_F16] = stage.air_kill[_F16] = 2 ;
	       stage.air_new[_A10] = stage.air_kill[_A10] = 3 ;
	       Load_Air_Image( "SOLGAE-G", _BF, 0 ) ;  // Load Air Image Data
	       Load_Air_Image( "F16PIC-G", _F16, 1 ) ;
	       Load_Air_Image( "A10PIC-G", _A10, 2 ) ;
	       buster.attr = 1 ;                       // 0 = Sky, 1 = Ground
	       back_scroll_mode = 1 ;
	       back_scroll_speed = 5 ;
	       Load_Back_Image( 5 ) ;
	       break ;
      case 6 :
	       //********* Bomber Kill Stage **********
	       strcpy( stage.name, "'6 ¸w : Ða“i· ’…'" ) ;
	       stage.air_new[_MF1] = stage.air_kill[_MF1] = 3 ;
	       stage.air_new[_F15] = stage.air_kill[_F15] = 3 ;
	       Load_Air_Image( "SOLGAE", _BF, 0 ) ;    // Load Air Image Data
	       Load_Air_Image( "F15PIC", _F15, 1 ) ;
	       Load_Air_Image( "MF1PIC", _MF1, 2 ) ;
	       buster.attr = 0 ;                       // 0 = Sky, 1 = Ground
	       back_scroll_mode = 0 ;
	       back_scroll_speed = 2 ;
	       Load_Back_Image( 4 ) ;
	       break ;
      case 7 :
	       strcpy( stage.name, "'7 ¸w : ‰b•©! ˆñ·e Í¢Î—!'" ) ;
	       stage.air_new[_F15] = stage.air_kill[_F15] = 3 ;
//	       stage.air_new[_F16] = stage.air_kill[_F16] = 4 ;  Event Char
	       Load_Air_Image( "SOLGAE", _BF, 0 ) ;    // Load Air Image Data
	       Load_Air_Image( "F15PIC", _F15, 1 ) ;
	       buster.attr = 0 ;                       // 0 = Sky, 1 = Ground
	       back_scroll_mode = 0 ;
	       back_scroll_speed = 2 ;
	       Load_Back_Image( 0 ) ;
	       break ;
      case 8 :
	       //************ Night Stage *************
	       strcpy( stage.name, "'8 ¸w : ·Aw·i ¸s”a'" ) ;
	       stage.air_new[_F16]  = stage.air_kill[_F16]  = 2 ;
	       stage.air_new[_F117] = stage.air_kill[_F117] = 3 ;
	       Load_Air_Image( "SOLGAE-N", _BF, 0 ) ;  // Load Air Image Data
	       Load_Air_Image( "F16PIC2N", _F16, 1 ) ;
	       Load_Air_Image( "F117PIC", _F117, 2 ) ;
	       buster.attr = 0 ;                       // 0 = Sky, 1 = Ground
	       back_scroll_mode = 0 ;
	       back_scroll_speed = 3 ;
	       Load_Back_Image( 3 ) ;
	       break ;
      case 9 :
	       //********* Laser Attack Mode **********
	       strcpy( stage.name, "'9 ¸w : º‚·q· Äi'" ) ;
	       stage.air_new[_MF1] = stage.air_kill[_MF1] = 3 ;
	       stage.air_new[_F15] = stage.air_kill[_F15] = 2 ;
	       Load_Air_Image( "SOLGAE", _BF, 0 ) ;    // Load Air Image Data
	       Load_Air_Image( "MF1PIC", _MF1, 1 ) ;
	       Load_Air_Image( "F15PIC", _F15, 2 ) ;
	       buster.attr = 0 ;                       // 0 = Sky, 1 = Ground
	       back_scroll_mode = 0 ;
	       back_scroll_speed = 4 ;
	       Load_Back_Image( 0 ) ;
	       break ;
      case 10:
	       //********* Return F16 stage ***********
	       strcpy( stage.name, "'10 ¸w : º‚·q· Äi(2)'" ) ;
	       stage.air_new[_F4]  = stage.air_kill[_F4]  = 1 ;
	       stage.air_new[_MF1] = stage.air_kill[_MF1] = 2 ;
	       Load_Air_Image( "F16PIC2", _F16, 0 ) ;  // Load Air Image Data
	       Load_Air_Image( "F4PIC2", _F4, 1 ) ;
	       Load_Air_Image( "MF1PIC", _MF1, 2 ) ;
	       buster.attr = 0 ;                       // 0 = Sky, 1 = Ground
	       back_scroll_mode = 0 ;
	       back_scroll_speed = 4 ;
	       Load_Back_Image( 4 ) ;
	       break ;
      case 11:
	       strcpy( stage.name, "'11 ¸w : Ìa•¡¶á· ‰i¸å'" ) ;
	       stage.air_new[_F14] = stage.air_kill[_F14] = 2 ;
	       stage.air_new[_F18] = stage.air_kill[_F18] = 3 ;
	       Load_Air_Image( "SOLGAE", _BF, 0 ) ;    // Load Air Image Data
	       Load_Air_Image( "F14PIC", _F14, 1 ) ;
	       Load_Air_Image( "F18PIC", _F18, 2 ) ;
	       buster.attr = 0 ;                       // 0 = Sky, 1 = Ground
	       back_scroll_mode = 0 ;
	       back_scroll_speed = 4 ;
	       Load_Back_Image( 6 ) ;
	       break ;
      case 12:
	       //*********** GROUND ATTACK ************
	       strcpy( stage.name, "'12 ¸w : Ìa•¡¶á· ‰i¸å(2)'" ) ;
	       stage.air_new[_FSX] = stage.air_kill[_FSX] = 3 ;
	       stage.air_new[_MF1] = stage.air_kill[_MF1] = 2 ;
	       Load_Air_Image( "SOLGAE-G", _BF, 0 ) ;  // Load Air Image Data
	       Load_Air_Image( "FSXPIC-G", _FSX, 1 ) ;
	       Load_Air_Image( "MF1PIC-G", _MF1, 2 ) ;
	       buster.attr = 1 ;                       // 0 = Sky, 1 = Ground
	       back_scroll_mode = 1 ;
	       back_scroll_speed = 1 ;
	       Load_Back_Image( 7 ) ;
	       break ;
      case 13:
	       strcpy( stage.name, "'13 ¸w : Í¢Î—¸å´¡'" ) ;
	       stage.air_new[_FSX] = stage.air_kill[_FSX] = 2 ;
	       stage.air_new[_F4]  = stage.air_kill[_F4]  = 2 ;
	       Load_Air_Image( "SOLGAE", _BF, 0 ) ;    // Load Air Image Data
	       Load_Air_Image( "FSXPIC", _FSX, 1 ) ;
	       Load_Air_Image( "F4PIC", _F4, 2 ) ;
	       buster.attr = 0 ;                       // 0 = Sky, 1 = Ground
	       back_scroll_mode = 0 ;
	       back_scroll_speed = 4 ;
	       Load_Back_Image( 0 ) ;
	       break ;
      case 14:
	       strcpy( stage.name, "'14 ¸w : ¶á‹¡! ¬á¶‰”Í¢‰b'" ) ;
	       stage.air_new[_F15]  = stage.air_kill[_F15]  = 2 ;
	       stage.air_new[_FSX]  = stage.air_kill[_FSX]  = 3 ;
//	       stage.air_new[_FSX]  = stage.air_kill[_FSX]  = 2 ;  Event
//	       stage.air_new[_F117] = stage.air_kill[_F117] = 2 ;
	       Load_Air_Image( "SOLGAE-N", _BF, 0 ) ;  // Load Air Image Data
	       Load_Air_Image( "F15PIC", _F15, 1 ) ;
	       Load_Air_Image( "FSXPIC", _FSX, 2 ) ;
	       buster.attr = 0 ;                       // 0 = Sky, 1 = Ground
	       back_scroll_mode = 0 ;
	       back_scroll_speed = 4 ;
	       Load_Back_Image( 1 ) ;
	       break ;
      case 15:
	       strcpy( stage.name, "'15 ¸w : i »¡Åa¥Áº¡..'" ) ;
	       stage.air_new[_F16] = stage.air_kill[_F16] = 2 ;
	       stage.air_new[_F22] = stage.air_kill[_F22] = 3 ;
//	       stage.air_new[_F15] = stage.air_kill[_F15] = 4 ;
//	       stage.air_new[_FSX] = stage.air_kill[_FSX] = 3 ;
	       Load_Air_Image( "SOLGAE", _BF, 0 ) ;    // Load Air Image Data
	       Load_Air_Image( "F16PIC2", _F16, 1 ) ;
	       Load_Air_Image( "F22PIC", _F22, 2 ) ;
	       buster.attr = 0 ;                       // 0 = Sky, 1 = Ground
	       back_scroll_mode = 0 ;
	       back_scroll_speed = 4 ;
	       Load_Back_Image( 4 ) ;
	       break ;
   }

   if( !panicmode )
   {
      if( (stage.number != 1) && (stage.number != 10) ) AdLib_Start( "bf-sel" ) ;
      Select_Weapon() ;              // Select Weapons
      Select_WingMan() ;             // Select Wing Mans

      scenario_program( 0 ) ;        // Scenario Data Init
   }

   for( j=0; j<MAXMYAIR; j++ )
   {
      my_air[j].body      = air_static_body[my_air[j].air] ;
      if( !panicmode )
      {
         my_air[j].flare     = 10 ;
         my_air[j].mis_count = air_static_mi_c[my_air[j].air] ;
      } else {
         my_air[j].flare     = 0 ;
         my_air[j].mis_count = 0 ;
      }

      for( i=0; i<MAXFLAG; i++ ) my_flag[j].attr[i] = 0 ;
   }

   rader_scale_mode = 2 ;
   rader_scale_data_x = 12 ;
   rader_scale_data_y = 1 ;

   back_x = 0 ;         	  // Back Data & Back Scroll Data Init
   back_y = 0 ;
   switch( back_scroll_mode )
   {
      case 0 :
	 bf_back_page[0] = 0 ;
	 bf_back_page[1] = 1 ;
	 break ;
      case 1 :
      case 2 :
	 bf_back_page[0] = 0 ;
	 bf_back_page[1] = 2 ;
	 break ;
   }
   for( i=0; i<2; i++ )
   {
      bk_x[i] = random( 480 ) - 80 ;
      bk_y[i] = random( 300 ) - 50 ;
   }

   if( CheckPlay() ) AdLib_End() ;

   if( (stage.number != 1) && (stage.number != 10) ) stage_demo( 0 ) ;

   stage.view_count = 1 ;
   stage.view_time = GetTime() ;
   start_attr = 1 ;

   stage.clear = 0 ;

   if( !panicmode ) sprintf( st, "bf-bgm%d", stage.number ) ;
   else             sprintf( st, "bf-panic" ) ;
   AdLib_Start( st ) ;
}

int lotate_air( char *fly_mode, int key )
{
   switch( key )
   {
      case M_UP    :
		   if( (*fly_mode >= 5) && (*fly_mode <= 11) ) { *(fly_mode)-=1; return 1; }
		   if( (*fly_mode >=13) && (*fly_mode <= 14) ) { *(fly_mode)+=1; return 2; }
		   if( (*fly_mode >= 0) && (*fly_mode <=  3) ) { *(fly_mode)+=1; return 2; }
		   if( *fly_mode == 15 ) { *fly_mode = 0 ;  return 2 ; }
		   if( *fly_mode == 12 ) { *(fly_mode)+=1 ; return 2 ; }
		   return 0 ;
      case M_DOWN  :
		   if( (*fly_mode >= 5) && (*fly_mode <= 11) ) { *(fly_mode)+=1; return 2; }
		   if( (*fly_mode >= 1) && (*fly_mode <=  3) ) { *(fly_mode)-=1; return 1; }
		   if( (*fly_mode >=13) && (*fly_mode <= 15) ) { *(fly_mode)-=1; return 1; }
		   if( *fly_mode ==  0 ) { *fly_mode = 15 ; return 1 ; }
		   if( *fly_mode ==  4 ) { *(fly_mode)+=1 ; return 2 ; }
		   return 0 ;
      case M_LEFT  :
		   if( (*fly_mode >= 1) && (*fly_mode <=  7) ) { *(fly_mode)+=1; return 2; }
		   if( (*fly_mode >= 9) && (*fly_mode <= 15) ) { *(fly_mode)-=1; return 1; }
		   if( *fly_mode ==  0 ) { *(fly_mode)+=1 ; return 2 ; }
		   return 0 ;
      case M_RIGHT :
		   if( (*fly_mode >= 1) && (*fly_mode <=  7) ) { *(fly_mode)-=1; return 1; }
		   if( (*fly_mode >= 9) && (*fly_mode <= 14) ) { *(fly_mode)+=1; return 2; }
		   if( *fly_mode == 15 ) { *fly_mode = 0 ;  return 2 ; }
		   if( *fly_mode ==  8 ) { *(fly_mode)+=1 ; return 2 ; }
		   return 0 ;
      case M_HOME  :
		   if( (*fly_mode >= 7) && (*fly_mode <= 13) ) { *(fly_mode)-=1; return 1; }
		   if( (*fly_mode >= 0) && (*fly_mode <= 5 ) ) { *(fly_mode)+=1; return 2; }
		   if( *fly_mode == 15 ) { *fly_mode = 0 ;  return 2 ; }
		   if( *fly_mode == 14 ) { *(fly_mode)+=1 ; return 2 ; }
		   return 0 ;
      case M_END   :
		   if( (*fly_mode >= 3) && (*fly_mode <= 9 ) ) { *(fly_mode)+=1; return 2; }
		   if( (*fly_mode >=11) && (*fly_mode <= 15) ) { *(fly_mode)-=1; return 1; }
		   if( *fly_mode == 1 ) { *(fly_mode) -= 1 ; return 1 ; }
		   if( *fly_mode == 0 ) { *fly_mode = 15 ; return 1 ; }
		   if( *fly_mode == 2 ) { *(fly_mode)+=1 ; return 2 ; }
		   return 0 ;
      case M_PAGEUP   :
		   if( (*fly_mode >= 3) && (*fly_mode <= 9 ) ) { *(fly_mode)-=1; return 1; }
		   if( (*fly_mode >= 0) && (*fly_mode <= 1 ) ) { *(fly_mode)+=1; return 2; }
		   if( (*fly_mode >=11) && (*fly_mode <= 14) ) { *(fly_mode)+=1; return 2; }
		   if( *fly_mode == 15 ) { *fly_mode = 0 ;  return 2 ; }
		   if( *fly_mode == 10 ) { *(fly_mode)+=1 ; return 2 ; }
		   return 0 ;
      case M_PAGEDOWN   :
		   if( (*fly_mode >= 7) && (*fly_mode <= 13) ) { *(fly_mode)+=1; return 2; }
		   if( (*fly_mode >= 1) && (*fly_mode <= 5 ) ) { *(fly_mode)-=1; return 1; }
		   if( *fly_mode == 0  ) { *fly_mode = 15 ; return 1 ; }
		   if( *fly_mode == 15 ) { *fly_mode = 14 ; return 2 ; }
		   if( *fly_mode == 6  ) { *(fly_mode)+=1 ; return 2 ; }
		   return 0 ;
   }
   return 0 ;
}

void new_char( int number, int mode )
{
   int i, j ;

   j = 0 ;
   for( i=0; i<11; i++ )
   {
      if( !stage.air_new[i] ) j++ ;
   }
   if( j == 11 ) return ;

   switch( mode )
   {
      case 0 :
	       ch_air[number].x = MEMXMIN ;
	       ch_air[number].y = ( random( MEMYMAX<<1 ) - MEMYMAX ) ;
	       ch_air[number].fly_mode = 0 ;
	       break ;
      case 1 :
	       ch_air[number].x = MEMXMAX ;
	       ch_air[number].y = ( random( MEMYMAX<<1 ) - MEMYMAX ) ;
	       ch_air[number].fly_mode = 8 ;
	       break ;
      case 2 :
	       ch_air[number].x = ( random( MEMXMAX<<1 ) - MEMXMAX ) ;
	       ch_air[number].y = MEMYMIN ;
	       ch_air[number].fly_mode = 12 ;
	       break ;
      case 3 :
	       ch_air[number].x = ( random( MEMXMAX<<1 ) - MEMXMAX ) ;
	       ch_air[number].y = MEMYMAX ;
	       ch_air[number].fly_mode = 4 ;
	       break ;
   }

   i = 1 ;
   while( i )
   {
      if( !panicmode )
      {
         if( !stage_new_char_mode ) j = (random(10)+1) ;
         else                       j = stage_new_char_air[stage.number-1][stage_new_char_count] ;
         if( stage.air_new[j] )
         {
	    ch_air[number].air = j ;
	    ch_air[number].body = air_static_body[j] ;
	    ch_air[number].mis_count = air_static_mi_c[j] ;
	    stage.air_new[j]-- ;
	    i = 0 ;
         }
      } else {
         for( i=0; i<11; i++ )
         {
            if( stage.air_new[i] )
            {
	       ch_air[number].air = i ;
	       ch_air[number].body = air_static_body[i] ;
	       ch_air[number].mis_count = 0 ;
	       stage.air_new[i]-- ;
               break;
            }
         }
         i = 0 ;
      }
   }
   if( !stage_new_char_mode || panicmode ) ch_air[number].ai = random(3)+1 ;
   else ch_air[number].ai = stage_new_char_ai[stage.number-1][stage_new_char_count++] ;
   ch_air[number].flare = 6 ;
   for( i=0; i<MAXFLAG; i++ )                  // Air Flag Init
   {
      ch_flag[number].x[i] = 0 ;
      ch_flag[number].y[i] = 0 ;
      ch_flag[number].attr[i] = 0 ;
      ch_flag[number].count[i] = 0 ;
   }
   ch_air[number].kill = 0 ;
   ch_air[number].kill_attr = 0 ;
   ch_air[number].loop = 0 ;
}

void move_char( void )
{
   int i, j ;

   if( !stage.view_count && !stage.end_view_count &&
       !score_view_count )
   {
      for( j=automatic; j<MAXMYAIR; j++ )               // ** My Airs Turbo Start !!!
      {
	 if( !my_turbo[j].attr && !my_turbo[j].count && !my_air[j].kill )
	 {
	    if( my_air[j].ai == 0 ) i = 500 ;
	    if( my_air[j].ai == 1 ) i = 200 ;
	    if( my_air[j].ai == 2 ) i = 100 ;
	    if( my_air[j].ai == 3 ) i = 75 ;
	    if( my_air[j].ai == 4 ) i = 50 ;

	    if( random(i)==(i>>1) ) my_turbo[j].attr = 1 ;   // **** Turbo On
	 }
      }
   }

   for( j=0; j<MAXMYAIR; j++ )
   {
      if( my_air[j].kill )
      {

	 if( (my_air[j].x > -500) && (my_air[j].x < 500) &&
	     (my_air[j].y > -500) && (my_air[j].y < 500) )
	 {
	    my_air[j].x += bk_sx[1][my_air[j].fly_mode] ;
	    my_air[j].y += bk_sy[1][my_air[j].fly_mode] ;
	    if( my_turbo[j].attr )
	    {
	       my_air[j].x += bk_sx[1][my_air[j].fly_mode] ;
	       my_air[j].y += bk_sy[1][my_air[j].fly_mode] ;
	    }
	 }
      }
   }

   if( !stage.view_count )
   {
      for( i=0; i<MAXMYAIR; i++ )             // Find Character Target
      {
	 if( !my_air[i].kill && !my_air_target[i].attr && !my_air_target[i].mes_attr )
	 {
	    if( !ch_air[i].kill )
	    {
	       my_air_target[i].attr = 1 ;
	       my_air_target[i].target = i ;
	    }
	    else
	    {
	       for( j=0; j<MAXCHAIR; j++ )
	       {
		  if( !ch_air[j].kill )
		  {
		     my_air_target[i].attr = 1 ;
		     my_air_target[i].target = j ;
		  }
	       }
	    }
	    if( my_air_target[i].attr == 1 )
	    {
	       my_air_target[i].mes_attr = 1 ;
	       my_air_target[i].mes_time = 0 ;
	       my_air_target[i].mes_air = ch_air[my_air_target[i].target].air ;
	    }
	 }
      }

      if( !my_mis.attr )                      //********* Get Target *********
      {
	 if( ch_air[my_mis_target.target].kill || !my_mis_target.attr )
	 {
	    for( i=0; i<MAXCHAIR; i++ )
	    {
	       if( !ch_air[i].kill && (ch_air[i].air!=6) ) // F117 Not Found!
	       {
		  my_mis_target.attr = 1 ;
		  my_mis_target.target = i ;
		  break ;
	       }
	    }
	    if( i == MAXCHAIR ) my_mis_target.attr = 0 ;   //*** Air Not Found
	 }
      }

      if( my_mis_target.attr == 1 )    //******** Static Target ********
      {
	 if( ch_flare[my_mis_target.target].attr )
	 {
	    my_mis_target.x = ch_flare[my_mis_target.target].x ;
	    my_mis_target.y = ch_flare[my_mis_target.target].y ;
	 }
	 else
	 {
	    my_mis_target.x = ch_air[my_mis_target.target].x ;
	    my_mis_target.y = ch_air[my_mis_target.target].y ;
	 }
      }

      for( i=automatic; i<MAXMYAIR; i++ )
      {
	 if( !my_air[i].kill )
	 {
	    if( my_air[i].air == 1 ) j = 1 ;           // F16
	    if( my_air[i].air == 2 ) j = random(10) ;  // F14
	    if( my_air[i].air == 3 ) j = random(20) ;  // F15
	    if( stage.view_count || stage.end_view_count  // Stage End
		|| score_view_count ) j = 0 ;
	    if( j || my_air[i].fly_count )                  // Air Lotate
	    {
	       if( !my_air[i].loop )
	       {
		  if( !my_air[i].ai )
		  {
		     if( !my_air[i].fly_count ) my_air[i].fly_attr = random(10) ;
		     else                       my_air[i].fly_count-- ;
		     switch( my_air[i].fly_attr )
		     {
			case 0 :
				 if( my_air[i].fly_mode == 0  ) my_air[i].fly_mode = 15 ;
				 else my_air[i].fly_mode-- ;
				 break ;
			case 9 :
				 if( my_air[i].fly_mode == 15 ) my_air[i].fly_mode = 0 ;
				 else my_air[i].fly_mode++ ;
				 break ;
		     }
		  }
		  else
		  {
		     if( !my_air[i].fly_count )
		     {
			if( my_air[i].ai == 1 )
			{
			   if( !random(6) ) my_air[i].fly_count = 3 ;
			}
			if( my_air[i].ai == 2 )
			{
			   if( !random(4) ) my_air[i].fly_count = 2 ;
			}
			if( my_air[i].ai == 3 )
			{
			   if( !random(2)  ) my_air[i].fly_count = 2 ;
			}
			if( my_air[i].ai == 4 )
			{
			   my_air[i].fly_count = 1 ;
			}
		     }
		     if( my_air[i].fly_count )
		     {
			if( ((my_air[i].x+30) < ch_air[my_air_target[i].target].x) && ((my_air[i].y+25) < ch_air[my_air_target[i].target].y) )
			{
			   lotate_air( &my_air[i].fly_mode, M_PAGEDOWN ) ;
			}
			else
			{
			   if( ((my_air[i].x+30) < ch_air[my_air_target[i].target].x) && ((my_air[i].y+25) > (ch_air[my_air_target[i].target].y+50)) )
			   {
			      lotate_air( &my_air[i].fly_mode, M_PAGEUP ) ;
			   }
			   else
			   {
			      if( ((my_air[i].x+30) > (ch_air[my_air_target[i].target].x+60)) && ((my_air[i].y+25) < ch_air[my_air_target[i].target].y ) )
			      {
				 lotate_air( &my_air[i].fly_mode, M_END ) ;
			      }
			      else
			      {
				 if( ((my_air[i].x+30) > (ch_air[my_air_target[i].target].x+60)) && ((my_air[i].y+25) > (ch_air[my_air_target[i].target].y+50)) )
				 {
				    lotate_air( &my_air[i].fly_mode, M_HOME ) ;
				 }
				 else
				 {
				    if( (my_air[i].x+30) > (ch_air[my_air_target[i].target].x+30) )
				    {
				       lotate_air( &my_air[i].fly_mode, M_LEFT ) ;
				    }
				    else
				    {
				       if( (my_air[i].y+25) < (ch_air[my_air_target[i].target].y+25) )
				       {
					  lotate_air( &my_air[i].fly_mode, M_DOWN ) ;
				       }
				       else
				       {
					  if( (my_air[i].y+25) > (ch_air[my_air_target[i].target].y+25) )
					  {
					     lotate_air( &my_air[i].fly_mode, M_UP ) ;
					  }
					  else
					  {
					     if( (my_air[i].x+30) < (ch_air[my_air_target[i].target].x+30) )
					     {
						lotate_air( &my_air[i].fly_mode, M_RIGHT ) ;
					     }
					  }
				       }
				    }
				 }
			      }
			   }
			}
			my_air[i].fly_count-- ;
		     }
		  }
		  my_air[i].loop = air_static_loop[my_air[i].air] ;
	       }
	       else
		  my_air[i].loop-- ;
	    }

	    my_air[i].x += -(speed_x[my_air[i].air][my_air[i].fly_mode]) + (speed_x[my_air[0].air][my_air[0].fly_mode]) ;
	    my_air[i].y += -(speed_y[my_air[i].air][my_air[i].fly_mode]) + (speed_y[my_air[0].air][my_air[0].fly_mode]) ;
	    if( my_turbo[i].attr )
	    {
	       my_air[i].x -= bk_sx[1][my_air[0].fly_mode] ;
	       my_air[i].y -= bk_sy[1][my_air[0].fly_mode] ;
	    }
	    if( my_turbo[0].attr )               // * My BF Turbo On (Scroll)
	    {
	       my_air[i].x += bk_sx[1][my_air[0].fly_mode] ;
	       my_air[i].y += bk_sy[1][my_air[0].fly_mode] ;
	    }

	    if( Crash( my_air[0].x-30, my_air[0].y-20, my_air[0].x+90, my_air[0].y+70,
		       my_air[i].x-30, my_air[i].y-20, my_air[i].x+90, my_air[i].y+70 ) )
	    {
	       if( !my_air_crash_attr && i )
	       {
		  sayPCM( PCM_AIR_CRASH, 1 ) ;
		  my_air_crash_attr = 1 ;
		  my_air_crash_num = i ;
	       }
	    }
	    else
	    {
	       if( i == my_air_crash_num ) my_air_crash_attr = 0 ;
	    }
	 }
	 else
	 {
	    my_air[i].x += (speed_x[0][my_air[0].fly_mode]) ;
	    my_air[i].y += (speed_y[0][my_air[0].fly_mode]) ;
	    if( my_turbo[i].attr )
	    {
	       my_air[i].x += bk_sx[1][my_air[0].fly_mode] ;
	       my_air[i].y += bk_sy[1][my_air[0].fly_mode] ;
	    }
	 }
	 if( my_air[i].x > MEMXMAX  )
	 {
	    if( !my_air[i].kill ) my_air[i].x = MEMXMIN ;
	 }
	 if( my_air[i].x < MEMXMIN )
	 {
	    if( !my_air[i].kill ) my_air[i].x = MEMXMAX ;
	 }
	 if( my_air[i].y > MEMYMAX  )
	 {
	    if( !my_air[i].kill ) my_air[i].y = MEMYMIN ;
	 }
	 if( my_air[i].y < MEMYMIN )
	 {
	    if( !my_air[i].kill ) my_air[i].y = MEMYMAX ;
	 }
      }

      for( i=0; i<MAXCHAIR; i++ )
      {
	 if( !ch_air[i].kill )
	 {
	    if( ch_air[i].air == 1 ) j = 1 ;           // F16
	    if( ch_air[i].air == 2 ) j = random(10) ;  // F14
	    if( ch_air[i].air == 3 ) j = random(20) ;  // F15
	    if( j || ch_air[i].fly_count ) // Air Lotate
	    {
	       if( !ch_air[i].loop )
	       {
		  if( !ch_air[i].ai )
		  {
		     if( !ch_air[i].fly_count ) ch_air[i].fly_attr = random(10) ;
		     else                       ch_air[i].fly_count-- ;
		     switch( ch_air[i].fly_attr )
		     {
			case 0 :

				 if( ch_air[i].fly_mode == 0  ) ch_air[i].fly_mode = 15 ;
				 else ch_air[i].fly_mode-- ;
				 break ;
			case 9 :
				 if( ch_air[i].fly_mode == 15 ) ch_air[i].fly_mode = 0 ;
				 else ch_air[i].fly_mode++ ;
				 break ;
		     }
		  }
		  else
		  {
		     if( !ch_air[i].fly_count )
		     {
			if( ch_air[i].ai == 1 )
			{
			   if( !random(6) ) ch_air[i].fly_count = 3 ;
			}
			if( ch_air[i].ai == 2 )
			{
			   if( !random(4) ) ch_air[i].fly_count = 2 ;
			}
			if( ch_air[i].ai == 3 )
			{
			   if( !random(2)  ) ch_air[i].fly_count = 2 ;
			}
			if( ch_air[i].ai == 4 )
			{
			   ch_air[i].fly_count = 1 ;
			}
		     }
		     if( ch_air[i].fly_count )
		     {
			if( ((ch_air[i].x+30) < my_air[0].x) && ((ch_air[i].y+25) < my_air[0].y) )
			{
			   lotate_air( &ch_air[i].fly_mode, M_PAGEDOWN ) ;
			}
			else
			{
			   if( ((ch_air[i].x+30) < my_air[0].x) && ((ch_air[i].y+25) > (my_air[0].y+50)) )
			   {
			      lotate_air( &ch_air[i].fly_mode, M_PAGEUP ) ;
			   }
			   else
			   {
			      if( ((ch_air[i].x+30) > (my_air[0].x+60)) && ((ch_air[i].y+25) < my_air[0].y ) )
			      {
				 lotate_air( &ch_air[i].fly_mode, M_END ) ;
			      }
			      else
			      {
				 if( ((ch_air[i].x+30) > (my_air[0].x+60)) && ((ch_air[i].y+25) > (my_air[0].y+50)) )
				 {
				    lotate_air( &ch_air[i].fly_mode, M_HOME ) ;
				 }
				 else
				 {
				    if( (ch_air[i].x+30) > (my_air[0].x+30) )
				    {
				       lotate_air( &ch_air[i].fly_mode, M_LEFT ) ;
				    }
				    else
				    {
				       if( (ch_air[i].y+25) < (my_air[0].y+25) )
				       {
					  lotate_air( &ch_air[i].fly_mode, M_DOWN ) ;
				       }
				       else
				       {
					  if( (ch_air[i].y+25) > (my_air[0].y+25) )
					  {
					     lotate_air( &ch_air[i].fly_mode, M_UP ) ;
					  }
					  else
					  {
					     if( (ch_air[i].x+30) < (my_air[0].x+30) )
					     {
						lotate_air( &ch_air[i].fly_mode, M_RIGHT ) ;
					     }
					  }
				       }
				    }
				 }
			      }
			   }
			}
			ch_air[i].fly_count-- ;
		     }
		  }
		  ch_air[i].loop = air_static_loop[ch_air[i].air] ;
	       }
	       else
		  ch_air[i].loop-- ;
	    }

	    ch_air[i].x += -(speed_x[ch_air[i].air][ch_air[i].fly_mode]) + (speed_x[my_air[0].air][my_air[0].fly_mode]) ;
	    ch_air[i].y += -(speed_y[ch_air[i].air][ch_air[i].fly_mode]) + (speed_y[my_air[0].air][my_air[0].fly_mode]) ;
	    if( my_turbo[0].attr )
	    {
	       ch_air[i].x += bk_sx[1][my_air[0].fly_mode] ;
	       ch_air[i].y += bk_sy[1][my_air[0].fly_mode] ;
	    }

	    if( Crash( my_air[0].x-30, my_air[0].y-20, my_air[0].x+90, my_air[0].y+70,
		       ch_air[i].x-30, ch_air[i].y-20, ch_air[i].x+90, ch_air[i].y+70 ) )
	    {
	       if( !ch_air_crash_attr )
	       {
		  sayPCM( PCM_AIR_CRASH, 1 ) ;
		  ch_air_crash_attr = 1 ;
		  ch_air_crash_num = i ;
	       }
	    }
	    else
	    {
	       if( i == ch_air_crash_num ) ch_air_crash_attr = 0 ;
	    }
	 }
	 else
	 {
	    ch_air[i].x += (speed_x[0][my_air[0].fly_mode]) ;
	    ch_air[i].y += (speed_y[0][my_air[0].fly_mode]) ;
	    if( my_turbo[0].attr )
	    {
	       ch_air[i].x += bk_sx[1][my_air[0].fly_mode] ;
	       ch_air[i].y += bk_sy[1][my_air[0].fly_mode] ;
	    }
	 }
	 if( ch_air[i].x > MEMXMAX  )
	 {
	    if( !ch_air[i].kill ) ch_air[i].x = MEMXMIN ;
	    else                  new_char( i, 1 ) ;
	    if( my_mis_target.target == i ) my_mis_target.attr = 0 ;
	 }
	 if( ch_air[i].x < MEMXMIN )
	 {
	    if( !ch_air[i].kill ) ch_air[i].x = MEMXMAX ;
	    else                  new_char( i, 0 ) ;
	    if( my_mis_target.target == i ) my_mis_target.attr = 0 ;
	 }
	 if( ch_air[i].y > MEMYMAX  )
	 {
	    if( !ch_air[i].kill ) ch_air[i].y = MEMYMIN ;
	    else                  new_char( i, 2 ) ;
	    if( my_mis_target.target == i ) my_mis_target.attr = 0 ;
	 }
	 if( ch_air[i].y < MEMYMIN )
	 {
	    if( !ch_air[i].kill ) ch_air[i].y = MEMYMAX ;
	    else                  new_char( i, 3 ) ;
	    if( my_mis_target.target == i ) my_mis_target.attr = 0 ;
	 }
      }
   }

   if( my_mis_target.attr )
   {
      if( ch_flare[my_mis_target.target].attr )
      {
	 ch_flare[my_mis_target.target].x += (bk_sx[1][my_air[0].fly_mode]<<1) ;
	 ch_flare[my_mis_target.target].y += (bk_sy[1][my_air[0].fly_mode]<<1) ;
	 if( my_turbo[0].attr )
	 {
	    ch_flare[my_mis_target.target].x += bk_sx[1][my_air[0].fly_mode] ;
	    ch_flare[my_mis_target.target].y += bk_sy[1][my_air[0].fly_mode] ;
	 }
      }
   }

   for( j=0; j<MAXMYAIR; j++ )
   {
      if( my_flare[j].attr )
      {
	 my_flare[j].x += (bk_sx[1][my_air[j].fly_mode]<<1) ;
	 my_flare[j].y += (bk_sy[1][my_air[j].fly_mode]<<1) ;
	 if( my_turbo[0].attr )
	 {
	    my_flare[j].x += bk_sx[1][my_air[j].fly_mode] ;
	    my_flare[j].y += bk_sy[1][my_air[j].fly_mode] ;
	 }
      }
      for( i=0; i<MAXFLAG; i++ )
      {
	 if( my_flag[j].attr[i] && !my_air[j].kill_count )
	 {
	    my_flag[j].x[i] += (bk_sx[1][my_air[j].fly_mode]<<1) ;
	    my_flag[j].y[i] += (bk_sy[1][my_air[j].fly_mode]<<1) ;
	    if( my_turbo[0].attr )
	    {
	       my_flag[j].x[i] += bk_sx[1][my_air[j].fly_mode] ;
	       my_flag[j].y[i] += bk_sy[1][my_air[j].fly_mode] ;
	    }
	 }
      }
   }
   for( i=0; i<MAXCHAIR; i++ )
   {
      for( j=0; j<MAXFLAG; j++ )
      {
	 if( ch_flag[i].attr[j] )
	 {
	    ch_flag[i].x[j] += (bk_sx[1][my_air[0].fly_mode]<<1) ;
	    ch_flag[i].y[j] += (bk_sy[1][my_air[0].fly_mode]<<1) ;
	    if( my_turbo[0].attr )
	    {
	       ch_flag[i].x[j] += bk_sx[1][my_air[0].fly_mode] ;
	       ch_flag[i].y[j] += bk_sy[1][my_air[0].fly_mode] ;
	    }
	 }
      }
   }
}

void my_body_attack( int air_number, int a_count, int a_mode )
{
   int i ;

   while( a_count-- )
   {
      my_air[air_number].body-- ;
      if( !my_air[air_number].body )
      {
	 if( air_number )
	 {
	    if( random(100) < 50 )
	    {
	       wing_man[my_air[air_number].wing_man_num].kill = 1 ;
	       my_air_target[air_number].mes_attr = 3 ;
	       my_air_target[air_number].mes_time = 0 ;
	       my_air_target[air_number].mes_air  = my_air[air_number].wing_man_num ;
	    }
	    else
	    {
	       my_air_target[air_number].mes_attr = 4 ;
	       my_air_target[air_number].mes_time = 0 ;
	       my_air_target[air_number].mes_air  = my_air[air_number].wing_man_num ;
	    }
	 }
	 my_air[air_number].kill_mode = a_mode ;
	 sayPCM( PCM_KILL, 1 ) ;
	 break ;
      }
   }
   if( my_air[air_number].body < MAXFLAG )
   {
      for( i=0; i<(MAXFLAG-my_air[air_number].body); i++ )
      {
	 my_flag[air_number].attr[i]  = 1 ;
	 my_flag[air_number].x[i]     = my_air[air_number].x+random(30)+15 ;
	 my_flag[air_number].y[i]     = my_air[air_number].y+random(30)+15 ;
	 my_flag[air_number].count[i] = i ;
      }
   }
}

void ch_body_attack( int air_number, int my_air_number, int a_count )
{
   int i ;

   while( a_count-- )
   {
      ch_air[air_number].body-- ;
      if( !ch_air[air_number].fly_count ) ch_air[air_number].fly_count = 4 ;

      if( !ch_air[air_number].body )
      {
	 ch_air[air_number].kill = 1 ;
	 sayPCM( PCM_KILL, 1 ) ;

	 if( stage.air_kill[ch_air[air_number].air] ) stage.air_kill[ch_air[air_number].air]-- ;
	 kill_air[ch_air[air_number].air-1]++ ;
	 total_kill_air[ch_air[air_number].air-1]++ ;

	 stage_score += air_static_scor[ch_air[air_number].air] ;

	 for( i=0; i<MAXFLAG; i++ )
	    ch_flag[air_number].attr[i] = 0 ;

	 my_air_target[my_air_number].attr = 0 ;
	 my_air_target[my_air_number].target = 0 ;

	 if( my_air_number )
	 {
	    wing_man[my_air[my_air_number].wing_man_num].stage_exp++ ;

	    my_air_target[my_air_number].mes_attr = 2 ;
	    my_air_target[my_air_number].mes_time = 0 ;
	    my_air_target[my_air_number].mes_air  = my_air[my_air_number].wing_man_num ;
	 }

	 break ;
      }
      else
      {
	 if( ch_air[air_number].body < MAXFLAG )
	 {
	    for( i=0; i<(MAXFLAG-ch_air[air_number].body); i++ )
	    {
	       ch_flag[air_number].attr[i]  = 1 ;
	       ch_flag[air_number].x[i]     = ch_air[air_number].x+random(30)+15 ;
	       ch_flag[air_number].y[i]     = ch_air[air_number].y+random(30)+15 ;
	       ch_flag[air_number].count[i] = i ;
	    }
	 }
      }
   }
}

void move_gun( void )
{
   int i, j, k ;

   for( j=0; j<MAXMYAIR; j++ )
   {
      if( my_gun[j].count && (my_gun[j].count<=MAXGUN) ) //*** Gun Seting ***
      {
         if( panicmode > 1 )              //*** Zeiper Mode Gun XY Seting ***
         {
	    my_gun[j].x[my_gun[j].count-1] = my_air[j].x+zeiper_gun_x[my_air[j].fly_mode] ;
	    my_gun[j].y[my_gun[j].count-1] = my_air[j].y+zeiper_gun_y[my_air[j].fly_mode] ;
         }
         else                             //*** Normal Mode Gun XY Seting ***
         {
	    my_gun[j].x[my_gun[j].count-1] = my_air[j].x+30-4 ;
	    my_gun[j].y[my_gun[j].count-1] = my_air[j].y+25-1 ;
         }
	 my_gun[j].attr[my_gun[j].count-1] = 1 ;
	 my_gun[j].fly_mode[my_gun[j].count-1] = my_air[j].fly_mode ;
	 my_gun[j].count++ ;
      }

      for( i=0; i<MAXGUN; i++ )
      {
	 if( my_gun[j].attr[i] )
	 {
	    my_gun[j].x[i] += - (speed_x[my_air[j].air][my_gun[j].fly_mode[i]]) ;
	    my_gun[j].y[i] += - (speed_y[my_air[j].air][my_gun[j].fly_mode[i]]) ;
	    if( my_turbo[0].attr )
	    {
	       my_gun[j].x[i] += bk_sx[1][my_air[j].fly_mode] ;
	       my_gun[j].y[i] += bk_sy[1][my_air[j].fly_mode] ;
	    }

	    if( my_gun[j].x[i] > (my_air[j].x+160+20) ) my_gun[j].attr[i] = 0 ;
	    else
	    if( my_gun[j].x[i] < (my_air[j].x-160-20) ) my_gun[j].attr[i] = 0 ;
	    else
	    if( my_gun[j].y[i] > (my_air[j].y+160+20) ) my_gun[j].attr[i] = 0 ;
	    else
	    if( my_gun[j].y[i] < (my_air[j].y-160-20) ) my_gun[j].attr[i] = 0 ;

	    for( k=0; k<MAXCHAIR; k++ )
	    {
	       if( my_gun[j].attr[i] && !ch_air[k].kill )
	       {
		  if( Crash( my_gun[j].x[i],    my_gun[j].y[i],    my_gun[j].x[i]+9,  my_gun[j].y[i]+9,
			     ch_air[k].x+10, ch_air[k].y+10, ch_air[k].x+40, ch_air[k].y+30 ) )
		  {
		     ch_body_attack( k, j, 1 ) ;
		     my_gun[j].attr[i] = 0 ;
		  }
	       }
	    }
	 }
      }
      for( i=0; i<MAXGUN; i++ )
      {
	 if( my_gun[j].attr[i] ) break ;
      }
      if( i == MAXGUN ) my_gun[j].count = 0 ;

      if( !stage.view_count && !stage.end_view_count &&
	  !score_view_count && !my_gun[j].count && !my_air[j].kill )
      {
	 if( my_air_target[j].attr )
	 {
	    if( Crash( my_air[j].x-160,    my_air[j].y-100,
		       my_air[j].x+60+160, my_air[j].y+60+100,
		       ch_air[my_air_target[j].target].x,    ch_air[my_air_target[j].target].y,
		       ch_air[my_air_target[j].target].x+60, ch_air[my_air_target[j].target].y+60 ) )
	    {
	       if( !j )
	       {
		  if( !automatic )
		  {
		     if( my_gun[j].weapon == 1 )
		     {
			if( !my_gun[j].count )
			{
			   my_gun[j].count = 1 ;
			   sayPCM( PCM_MULT_GUN, 1 ) ;
			}
		     }
		     if( my_gun[j].weapon == 2 )
		     {
			if( my_gun[j].fly_mode[j] )
			{
			   my_gun[j].fly_mode[1] = 1 ;
			   my_gun[j].fly_mode[0] = 0 ;
			   sayPCM( PCM_LASER, 1 ) ;
			}
			else my_gun[j].fly_mode[0]++ ;
		     }
		  }
	       }
	       else my_gun[j].count = 1 ;
	    }
	 }
      }
   }

   //*************************************************************************

   for( j=0; j<MAXCHAIR; j++ )
   {
      if( ch_gun[j].count && (ch_gun[j].count<=MAXGUN) )
      {
	 ch_gun[j].x[ch_gun[j].count-1] = ch_air[j].x + 30 ;
	 ch_gun[j].y[ch_gun[j].count-1] = ch_air[j].y + 25 ;
	 ch_gun[j].attr[ch_gun[j].count-1] = 1 ;
	 ch_gun[j].fly_mode[ch_gun[j].count-1] = ch_air[j].fly_mode ;
	 ch_gun[j].count++ ;
      }

      for( i=0; i<MAXGUN; i++ )
      {
	 if( ch_gun[j].attr[i] )
	 {
	    ch_gun[j].x[i] += - (speed_x[ch_air[j].air][ch_gun[j].fly_mode[i]]) -
				(speed_x[ch_air[j].air][ch_gun[j].fly_mode[i]]) + (speed_x[my_air[0].air][my_air[0].fly_mode]) ;
	    ch_gun[j].y[i] += - (speed_y[ch_air[j].air][ch_gun[j].fly_mode[i]]) -
				(speed_y[ch_air[j].air][ch_gun[j].fly_mode[i]]) + (speed_y[my_air[0].air][my_air[0].fly_mode]) ;
	    if( my_turbo[0].attr )
	    {
	       ch_gun[j].x[i] += bk_sx[1][my_air[0].fly_mode] ;
	       ch_gun[j].y[i] += bk_sy[1][my_air[0].fly_mode] ;
	    }

	    if( ch_gun[j].x[i] > (ch_air[j].x+160+20) ) ch_gun[j].attr[i] = 0 ;
	    else
	    if( ch_gun[j].x[i] < (ch_air[j].x-160-20) ) ch_gun[j].attr[i] = 0 ;
	    else
	    if( ch_gun[j].y[i] > (ch_air[j].y+160+20) ) ch_gun[j].attr[i] = 0 ;
	    else
	    if( ch_gun[j].y[i] < (ch_air[j].y-160-20) ) ch_gun[j].attr[i] = 0 ;

	    for( k=0; k<MAXMYAIR; k++ )
	    {
	       if( ch_gun[j].attr[i] && !my_air[k].kill_mode )
	       {
		  if( Crash( ch_gun[j].x[i], ch_gun[j].y[i], ch_gun[j].x[i]+9, ch_gun[j].y[i]+9,
			     my_air[k].x+10, my_air[k].y+10, my_air[k].x+40,   my_air[k].y+30 ) )
		  {
		     if( !k )
		     {
			SetBFPalette( 1 ) ;
			sayPCM( PCM_GUN_CRASH, 1 ) ;
		     }
		     my_body_attack( k, 1, 2 ) ;
		     ch_gun[j].attr[i] = 0 ;
		  }
	       }
	    }
	 }
      }

      for( i=0; i<MAXGUN; i++ )
      {
	 if( ch_gun[j].attr[i] ) break ;
      }
      if( i == MAXGUN ) ch_gun[j].count = 0 ;

      if( !stage.view_count && !stage.end_view_count && !score_view_count )
      {
	 for( i=0; i<MAXMYAIR; i++ )
	 {
	    if( !ch_gun[j].count && !ch_air[j].kill )
	    {
	       if( Crash( ch_air[j].x-160,    ch_air[j].y-100,
			  ch_air[j].x+60+160, ch_air[j].y+60+100,
			  my_air[i].x,    my_air[i].y,
			  my_air[i].x+60, my_air[i].y+60 ) )
	       {
		  ch_gun[j].count = 1 ;
	       }
	    }
	 }
      }
   }
}

void laser_gun_attack( int air_number )
{
   int distance, xerr=0, yerr=0, delta_x, delta_y, incx, incy ;
   int startx, starty, endx, endy ;
   int i, j ;

   xerr = 0 ;
   yerr = 0 ;
   incx = 0 ;
   incy = 0 ;

   i = MAXCHAIR ;

   if( panicmode > 1 )         //******* Zeiper Mode
   {
      startx = zeiper_laser_sx[my_air[air_number].fly_mode] ;
      starty = zeiper_laser_sy[my_air[air_number].fly_mode] ;
   }
   else                        //******* Normal Mode
   {
      startx = 160 ;
      starty = 100 ;
   }

   if( my_gun[air_number].weapon == 3 )        //*** Automatic Power Laser
   {
      i = MAXCHAIR ;
      for( i=0; i<MAXCHAIR; i++ )
      {
         if( !ch_air[i].kill )
         {
            if( Crash( ch_air[i].x, ch_air[i].y, ch_air[i].x+60, ch_air[i].y+40,
                       0, 0, 320, 200 ) )
            {
               endx = ch_air[i].x+30+random(120)-60 ;
               endy = ch_air[i].y+20+random(80)-40 ;
               break ;
            }
         }
      }
   }

   if( i==MAXCHAIR )
   {
      if( panicmode > 1 )         //******* Zeiper Mode
      {
         endx   = zeiper_laser_ex[my_air[air_number].fly_mode] ;
         endy   = zeiper_laser_ey[my_air[air_number].fly_mode] ;
      }
      else                        //******* Normal Mode
      {
         switch( my_air[air_number].fly_mode )
         {
            case 0  :  endx = 319 ; endy = 100 ; break ;
            case 1  :  endx = 319 ; endy = 50  ; break ;
            case 2  :  endx = 260 ; endy = 1   ; break ;
            case 3  :  endx = 200 ; endy = 1   ; break ;
            case 4  :  endx = 160 ; endy = 1   ; break ;
            case 5  :  endx = 120 ; endy = 1   ; break ;
            case 6  :  endx = 60  ; endy = 1   ; break ;
            case 7  :  endx = 1   ; endy = 50  ; break ;
            case 8  :  endx = 1   ; endy = 100 ; break ;
            case 9  :  endx = 1   ; endy = 160 ; break ;
            case 10 :  endx = 80  ; endy = 199 ; break ;
            case 11 :  endx = 120 ; endy = 199 ; break ;
            case 12 :  endx = 160 ; endy = 199 ; break ;
            case 13 :  endx = 200 ; endy = 199 ; break ;
            case 14 :  endx = 240 ; endy = 199 ; break ;
            case 15 :  endx = 319 ; endy = 170 ; break ;
         }
      }
   }

   delta_x = endx - startx ;
   delta_y = endy - starty ;
   if( delta_x > 0 ) incx=1 ; else if( delta_x == 0 ) incx=0 ; else incx=-1 ;
   if( delta_y > 0 ) incy=1 ; else if( delta_y == 0 ) incy=0 ; else incy=-1 ;

   delta_x = abs( delta_x ) ;
   delta_y = abs( delta_y ) ;
   if( delta_x > delta_y ) distance = delta_x ; else distance = delta_y ;

   for( i=0; i<=distance; i++ )
   {
      BufPutPixel( startx, starty, 254, mcga1 ) ;
      xerr += delta_x ;
      yerr += delta_y ;
      if( xerr > distance ) { xerr -= distance ; startx += incx ; }
      if( yerr > distance ) { yerr -= distance ; starty += incy ; }
      for( j=0; j<MAXCHAIR; j++ )
      {
	 if( my_air[air_number].body && !ch_air[j].kill )
	 {
	    if( Crash( startx-1,       starty-1,       startx+1,       starty+1,
		       ch_air[j].x+10, ch_air[j].y+10, ch_air[j].x+40, ch_air[j].y+30 ) )
	    {
	       ch_body_attack( j, air_number, 2 ) ;
	       return ;
	    }
	 }
      }
   }
}

void laser_bim_attack( int air_number )
{
   int distance, xerr=0, yerr=0, delta_x, delta_y, incx, incy ;
   int startx, starty, endx, endy ;
   int i, j ;

   xerr = 0 ;
   yerr = 0 ;
   incx = 0 ;
   incy = 0 ;

   startx = 160 ;
   starty = 100 ;
   switch( my_air[air_number].fly_mode )
   {
      case 0  : endx = 318 ; endy = 100 ; break ;
      case 1  :	endx = 318 ; endy = 50  ; break ;
      case 2  :	endx = 260 ; endy = 1   ; break ;
      case 3  :	endx = 200 ; endy = 1   ; break ;
      case 4  :	endx = 160 ; endy = 1   ; break ;
      case 5  :	endx = 120 ; endy = 1   ; break ;
      case 6  :	endx = 60  ; endy = 1   ; break ;
      case 7  :	endx = 1   ; endy = 50  ; break ;
      case 8  :	endx = 1   ; endy = 100 ; break ;
      case 9  :	endx = 1   ; endy = 160 ; break ;
      case 10 :	endx = 80  ; endy = 198 ; break ;
      case 11 :	endx = 120 ; endy = 198 ; break ;
      case 12 :	endx = 160 ; endy = 198 ; break ;
      case 13 :	endx = 200 ; endy = 198 ; break ;
      case 14 :	endx = 240 ; endy = 198 ; break ;
      case 15 :	endx = 318 ; endy = 170 ; break ;
   }

   delta_x = endx - startx ;
   delta_y = endy - starty ;
   if( delta_x > 0 ) incx=1 ; else if( delta_x == 0 ) incx=0 ; else incx=-1 ;
   if( delta_y > 0 ) incy=1 ; else if( delta_y == 0 ) incy=0 ; else incy=-1 ;

   delta_x = abs( delta_x ) ;
   delta_y = abs( delta_y ) ;
   if( delta_x > delta_y ) distance = delta_x ; else distance = delta_y ;

   for( i=0; i<=distance; i++ )
   {
      BufPutPixel( startx,   starty,   254, mcga1 ) ;
      BufPutPixel( startx+1, starty,   254, mcga1 ) ;
      BufPutPixel( startx,   starty+1, 254, mcga1 ) ;
      BufPutPixel( startx+1, starty+1, 254, mcga1 ) ;
      xerr += delta_x ;
      yerr += delta_y ;
      if( xerr > distance ) { xerr -= distance ; startx += incx ; }
      if( yerr > distance ) { yerr -= distance ; starty += incy ; }
      for( j=0; j<MAXCHAIR; j++ )
      {
	 if( my_air[air_number].body && !ch_air[j].kill )
	 {
	    if( Crash( startx-2,       starty-2,       startx+2,       starty+2,
		       ch_air[j].x+10, ch_air[j].y+10, ch_air[j].x+40, ch_air[j].y+30 ) )
	    {
	       ch_body_attack( j, air_number, 10 ) ;
	       return ;
	    }
	 }
      }
   }
}

void move_mis( void )
{
   int i, j, k ;

   for( j=0; j<MAXMYAIR; j++ )
   {
      if( my_flare[j].attr )
      {
	 if( my_flare[j].attr == 1 )
	 {
	    my_flare[j].x = my_air[j].x + (30-11) ;
	    my_flare[j].y = my_air[j].y + (25-9) ;
	    my_flare[j].attr++ ;
	    my_flare[j].count = 0 ;
	 }
	 else
	 {
	    my_flare[j].count++ ;
	    if( my_flare[j].count == 8 ) my_flare[j].attr = 0 ;
	 }
      }
   }

   if( my_mis_flag.attr[0] == 1 )
   {
      if( my_mis_flag.attr[(MAXFLAG-1)] == 0 )    //********* Locate Flag Init ********
      {
	 my_mis_flag.attr[my_mis_flag.count[0]] = 1 ;
	 my_mis_flag.x[my_mis_flag.count[0]] = my_mis.x + 15 ;
	 my_mis_flag.y[my_mis_flag.count[0]] = my_mis.y + 10 ;
	 my_mis_flag.count[my_mis_flag.count[0]] = 0 ;
      }
   }

   if( my_mis.attr == 1 )
   {
      my_mis.attr++ ;
      my_mis.count = 3 ;

      my_mis.x = my_air[0].x + 30 ;
      my_mis.y = my_air[0].y + 25 ;
      my_mis.fly_mode = my_air[0].fly_mode ;
      my_mis.lotate_count = mis_static_time[my_mis.weapon] ;
      my_mis.loop = 0 ;

      my_mis_flag.attr[0] = 1 ;
      my_mis_flag.x[0] = my_mis.x + 15 ;
      my_mis_flag.y[0] = my_mis.y + 10 ;
      my_mis_flag.count[0] = 0 ;

      sayPCM( PCM_MISSILE, 1 ) ;
   }

   if( my_mis.attr == 2 )
   {
      if( !my_mis.count )
      {
	 my_mis.attr++ ;
	 my_mis.count = 3 ;
      }
      else
	 my_mis.count-- ;
   }

   if( my_mis.attr == 3 )
   {
      if( !my_mis.count )
      {
	 my_mis.attr++ ;
	 my_mis.count = 3 ;
      }
      else
	 my_mis.count-- ;
   }

   if( my_mis.attr == 4 )
   {
      if( !my_mis.count ) my_mis.attr++ ;
      else                my_mis.count-- ;
   }

   if( my_mis.attr == 5 )
   {
      if( (my_mis.loop == mis_static_loop[my_mis.weapon]) && my_mis.lotate_count && my_mis_target.attr )
      {
	 my_mis.loop = 0 ;
	 my_mis.lotate_count-- ;
	 if( ((my_mis.x+15) < my_mis_target.x) && ((my_mis.y+11) < my_mis_target.y) )
	 {
	    lotate_air( &my_mis.fly_mode, M_PAGEDOWN ) ;
	    my_mis.lotate_fly_mode = my_mis.fly_mode ;
	 }
	 else
	 {
	    if( ((my_mis.x+15) < my_mis_target.x) && ((my_mis.y+11) > (my_mis_target.y+50)) )
	    {
	       lotate_air( &my_mis.fly_mode, M_PAGEUP ) ;
	       my_mis.lotate_fly_mode = my_mis.fly_mode ;
	    }
	    else
	    {
	       if( ((my_mis.x+15) > (my_mis_target.x+60)) && ((my_mis.y+11) < my_mis_target.y ) )
	       {
		  lotate_air( &my_mis.fly_mode, M_END ) ;
		  my_mis.lotate_fly_mode = my_mis.fly_mode ;
	       }
	       else
	       {
		  if( ((my_mis.x+15) > (my_mis_target.x+60)) && ((my_mis.y+11) > (my_mis_target.y+50)) )
		  {
		     lotate_air( &my_mis.fly_mode, M_HOME ) ;
		     my_mis.lotate_fly_mode = my_mis.fly_mode ;
		  }
		  else
		  {
		     if( (my_mis.x+15) > (my_mis_target.x+30) )
		     {
			lotate_air( &my_mis.fly_mode, M_LEFT ) ;
			my_mis.lotate_fly_mode = my_mis.fly_mode ;
		     }
		     else
		     {
			if( (my_mis.y+11) < (my_mis_target.y+25) )
			{
			   lotate_air( &my_mis.fly_mode, M_DOWN ) ;
			   my_mis.lotate_fly_mode = my_mis.fly_mode ;
			}
			else
			{
			   if( (my_mis.y+11) > (my_mis_target.y+25) )
			   {
			      lotate_air( &my_mis.fly_mode, M_UP ) ;
			      my_mis.lotate_fly_mode = my_mis.fly_mode ;
			   }
			   else
			   {
			      if( (my_mis.x+15) < (my_mis_target.x+30) )
			      {
				 lotate_air( &my_mis.fly_mode, M_RIGHT ) ;
				 my_mis.lotate_fly_mode = my_mis.fly_mode ;
			      }
			   }
			}
		     }
		  }
	       }
	    }
	 }
      }
      else
	 my_mis.loop++ ;
   }

   if( my_mis.attr )
   {
      if( my_mis.attr == 2 )
      {
	 my_mis.x += - (bk_sx[0][my_mis.fly_mode]<<1) + (speed_x[my_air[0].air][my_air[0].fly_mode]) ;
	 my_mis.y += - (bk_sy[0][my_mis.fly_mode]<<1) + (speed_y[my_air[0].air][my_air[0].fly_mode]) ;
      }
      if( my_mis.attr == 3 )
      {
	 my_mis.x += - (bk_sx[0][my_mis.fly_mode]<<2) + (speed_x[my_air[0].air][my_air[0].fly_mode]) ;
	 my_mis.y += - (bk_sy[0][my_mis.fly_mode]<<2) + (speed_y[my_air[0].air][my_air[0].fly_mode]) ;
      }
      if( my_mis.attr == 4 )
      {
	 my_mis.x += - ((bk_sx[0][my_mis.fly_mode]<<2)+(bk_sx[0][my_mis.fly_mode]<<1)) + (speed_x[my_air[0].air][my_air[0].fly_mode]) ;
	 my_mis.y += - ((bk_sy[0][my_mis.fly_mode]<<2)+(bk_sy[0][my_mis.fly_mode]<<1)) + (speed_y[my_air[0].air][my_air[0].fly_mode]) ;
      }
      if( my_mis.attr == 5 )
      {
	 my_mis.x += - (bk_sx[0][my_mis.fly_mode]<<3) + (speed_x[my_air[0].air][my_air[0].fly_mode]) ;
	 my_mis.y += - (bk_sy[0][my_mis.fly_mode]<<3) + (speed_y[my_air[0].air][my_air[0].fly_mode]) ;
      }
      if( my_turbo[0].attr )
      {
	 my_mis.x += bk_sx[1][my_air[0].fly_mode] ;
	 my_mis.y += bk_sy[1][my_air[0].fly_mode] ;
      }
      if( my_mis.x > MEMXMAX ) my_mis.attr = 0 ;
      else
      if( my_mis.x < MEMXMIN ) my_mis.attr = 0 ;
      else
      if( my_mis.y > MEMYMAX ) my_mis.attr = 0 ;
      else
      if( my_mis.y < MEMYMIN ) my_mis.attr = 0 ;

      for( j=0; j<MAXCHAIR; j++ )
      {
	 if( my_mis.attr && !ch_air[j].kill )
	 {
	    if( Crash( my_mis.x,       my_mis.y,       my_mis.x+31,    my_mis.y+22,
		       ch_air[j].x+10, ch_air[j].y+10, ch_air[j].x+40, ch_air[j].y+30 ) )
	    {
	       sayPCM( PCM_KILL, 1 ) ;
	       ch_body_attack( j, 0, mis_static_body[my_mis.weapon] ) ;
	       my_mis.attr = 0 ;
	    }
	 }
      }
   }

   /************************************************************************/

   if( !ch_air[my_mis_target.target].kill && ch_air[my_mis_target.target].flare &&
      my_mis.attr && (random(50)==25) && !ch_flare[my_mis_target.target].attr )
   {
      ch_air[my_mis_target.target].flare-- ;
      ch_flare[my_mis_target.target].x = ch_air[my_mis_target.target].x + (30-11) ;
      ch_flare[my_mis_target.target].y = ch_air[my_mis_target.target].y + (25-9) ;
      ch_flare[my_mis_target.target].attr = 1 ;
      ch_flare[my_mis_target.target].count = 0 ;
   }
   else
   {
      if( ch_flare[my_mis_target.target].attr )
      {
	 ch_flare[my_mis_target.target].count++ ;
	 if( ch_flare[my_mis_target.target].count == 8 ) ch_flare[my_mis_target.target].attr = 0 ;
      }
   }

   if( ch_mis_flag.attr[0] == 1 )
   {
      if( ch_mis_flag.attr[(MAXFLAG-1)] == 0 )    //********* Locate Flag Init ********
      {
	 ch_mis_flag.attr[ch_mis_flag.count[0]] = 1 ;
	 ch_mis_flag.x[ch_mis_flag.count[0]] = ch_mis.x + 15 ;
	 ch_mis_flag.y[ch_mis_flag.count[0]] = ch_mis.y + 10 ;
	 ch_mis_flag.count[ch_mis_flag.count[0]] = 0 ;
      }
   }

   if( ch_mis.attr == 1 )
   {
      ch_mis.attr++ ;
      mis_delay       = GetTime() ;
      mis_blink_delay = GetTime() ;
      mis_blink_attr  = 0 ;
   }

   if( ch_mis.attr == 2 )
   {
      if( (GetTime()-mis_delay) > 300L )
      {
	 if( stage.air_kill[_BF]  || stage.air_kill[_F16] || stage.air_kill[_F14]  || stage.air_kill[_F15] ||
	     stage.air_kill[_F18] || stage.air_kill[_A10] || stage.air_kill[_F117] || stage.air_kill[_FSX] ||
	     stage.air_kill[_F22] || stage.air_kill[_MF1] || stage.air_kill[_F4] )
	 {
	    ch_mis.attr++ ;
	    ch_mis.count = 3 ;
	    while( 1 )
	    {
	       i = random( MAXCHAIR ) ;
	       if( !ch_air[i].kill && ch_air[i].mis_count ) break ;
	    }
	    ch_air[i].mis_count-- ;
	    ch_mis.weapon = random( 8 ) ;
	    ch_mis.x = ch_air[i].x + 30 ;
	    ch_mis.y = ch_air[i].y + 25 ;
	    ch_mis.lotate_count = mis_static_time[ch_mis.weapon] ;
	    ch_mis.fly_mode = ch_air[i].fly_mode ;
	    ch_mis.loop = 0 ;

	    ch_mis_flag.attr[0] = 1 ;
	    ch_mis_flag.x[0] = ch_mis.x + 15 ;
	    ch_mis_flag.y[0] = ch_mis.y + 10 ;
	    ch_mis_flag.count[0] = 0 ;
	 }
	 else
	    ch_mis.attr = 0 ;
      }
   }

   if( ch_mis.attr == 3 )
   {
      if( !ch_mis.count )
      {
	 ch_mis.attr++ ;
	 ch_mis.count = 3 ;
      }
      else
	 ch_mis.count-- ;
   }

   if( ch_mis.attr == 4 )
   {
      if( !ch_mis.count )
      {
	 ch_mis.attr++ ;
	 ch_mis.count = 3 ;
      }
      else
	 ch_mis.count-- ;
   }

   if( ch_mis.attr == 5 )
   {
      if( !ch_mis.count ) ch_mis.attr++ ;
      else                ch_mis.count-- ;
   }

   if( ch_mis.attr == 6 )
   {
      if( (ch_mis.loop == mis_static_loop[ch_mis.weapon]) && ch_mis.lotate_count )
      {
	 ch_mis.loop = 0 ;
	 ch_mis.lotate_count-- ;
	 i = -1 ;
	 for( k=0; k<MAXMYAIR; k++ )           //***** FLARE FIND LOOP  *****
	 {
	    if( my_flare[k].attr )             //***** FLARE ON ?       *****
	    {
	       i = my_flare[k].x ;
	       j = my_flare[k].y ;
	    }
	 }
	 if( i == -1 )                         // ***** FLARE NOT FOUND *****
	 {
	    i = my_air[0].x ;
	    j = my_air[0].y ;
	 }
	 if( ((ch_mis.x+15) < i) && ((ch_mis.y+11) < j) )
	 {
	    lotate_air( &ch_mis.fly_mode, M_PAGEDOWN ) ;
	    ch_mis.lotate_fly_mode = ch_mis.fly_mode ;
	 }
	 else
	 {
	    if( ((ch_mis.x+15) < i) && ((ch_mis.y+11) > (j+50)) )
	    {
	       lotate_air( &ch_mis.fly_mode, M_PAGEUP ) ;
	       ch_mis.lotate_fly_mode = ch_mis.fly_mode ;
	    }
	    else
	    {
	       if( ((ch_mis.x+15) > (i+60)) && ((ch_mis.y+11) < j ) )
	       {
		  lotate_air( &ch_mis.fly_mode, M_END ) ;
		  ch_mis.lotate_fly_mode = ch_mis.fly_mode ;
	       }
	       else
	       {
		  if( ((ch_mis.x+15) > (i+60)) && ((ch_mis.y+11) > (j+50)) )
		  {
		     lotate_air( &ch_mis.fly_mode, M_HOME ) ;
		     ch_mis.lotate_fly_mode = ch_mis.fly_mode ;
		  }
		  else
		  {
		     if( (ch_mis.x+15) > (i+30) )
		     {
			lotate_air( &ch_mis.fly_mode, M_LEFT ) ;
			ch_mis.lotate_fly_mode = ch_mis.fly_mode ;
		     }
		     else
		     {
			if( (ch_mis.y+11) < (j+25) )
			{
			   lotate_air( &ch_mis.fly_mode, M_DOWN ) ;
			   ch_mis.lotate_fly_mode = ch_mis.fly_mode ;
			}
			else
			{
			   if( (ch_mis.y+11) > (j+25) )
			   {
			      lotate_air( &ch_mis.fly_mode, M_UP ) ;
			      ch_mis.lotate_fly_mode = ch_mis.fly_mode ;
			   }
			   else
			   {
			      if( (ch_mis.x+15) < (i+30) )
			      {
				 lotate_air( &ch_mis.fly_mode, M_RIGHT ) ;
				 ch_mis.lotate_fly_mode = ch_mis.fly_mode ;
			      }
			   }
			}
		     }
		  }
	       }
	    }
	 }
      }
      else
	 ch_mis.loop++ ;
   }

   if( ch_mis.attr >= 3 )
   {
      if( ch_mis.attr == 3 )
      {
	 ch_mis.x += - (bk_sx[0][ch_mis.fly_mode]<<1) ;
	 ch_mis.y += - (bk_sy[0][ch_mis.fly_mode]<<1) ;
      }
      if( ch_mis.attr == 4 )
      {
	 ch_mis.x += - (bk_sx[0][ch_mis.fly_mode]<<2) ;
	 ch_mis.y += - (bk_sy[0][ch_mis.fly_mode]<<2) ;
      }
      if( ch_mis.attr == 5 )
      {
	 ch_mis.x += - ((bk_sx[0][ch_mis.fly_mode]<<2)+(bk_sx[0][ch_mis.fly_mode]<<1)) ;
	 ch_mis.y += - ((bk_sy[0][ch_mis.fly_mode]<<2)+(bk_sy[0][ch_mis.fly_mode]<<1)) ;
      }
      if( ch_mis.attr == 6 )
      {
	 ch_mis.x += - (bk_sx[0][ch_mis.fly_mode]<<3) ;
	 ch_mis.y += - (bk_sy[0][ch_mis.fly_mode]<<3) ;
      }
      if( my_turbo[0].attr )
      {
	 ch_mis.x += bk_sx[1][my_air[0].fly_mode] ;
	 ch_mis.y += bk_sy[1][my_air[0].fly_mode] ;
      }
      if( ch_mis.x > MEMXMAX ) ch_mis.attr = 0 ;
      else
      if( ch_mis.x < MEMXMIN ) ch_mis.attr = 0 ;
      else
      if( ch_mis.y > MEMYMAX ) ch_mis.attr = 0 ;
      else
      if( ch_mis.y < MEMYMIN ) ch_mis.attr = 0 ;

      for( j=0; j<MAXMYAIR; j++ )
      {
	 if( (ch_mis.attr >= 3) && !my_air[j].kill_mode )
	 {
	    if( Crash( ch_mis.x,    ch_mis.y,    ch_mis.x+31, ch_mis.y+22,
		       my_air[j].x+10, my_air[j].y+10, my_air[j].x+40, my_air[j].y+30 )
		&& !my_air[j].roll_attr )
	    {
	       if( !j ) SetBFPalette( 2 ) ;
	       sayPCM( PCM_KILL, 1 ) ;
	       my_body_attack( j, mis_static_body[ch_mis.weapon], 1 ) ;
	       ch_mis.attr = 0 ;
	    }
	 }
      }
   }

   if( !stage.view_count && !stage.end_view_count && !score_view_count &&
       !ch_mis.attr && (random(oh_ch_mis_random)==100) )
   {
      for( i=0; i<MAXCHAIR; i++ )
      {
	 if( !ch_air[i].kill && ch_air[i].mis_count )
	 {
	    if( !panicmode ) ch_mis.attr = 1 ;
	    break ;
	 }
      }
   }
}

void chk_max_xy( int attr )
{
   if( my_turbo[0].attr )
   {
      bk_x[attr] += (bk_sx[1][my_air[0].fly_mode]<<1) ;
      bk_y[attr] += (bk_sy[1][my_air[0].fly_mode]<<1) ;
   }
   if( (bk_x[attr] >  320) && (random(20)==10) )      { bk_x[attr] = -230 ; bk_y[attr] = random( 200 ) ; }
   else if( (bk_x[attr] < -230) && (random(20)==10) ) { bk_x[attr] = 320 ;  bk_y[attr] = random( 200 ) ; }
   if( (bk_y[attr] >  200) && (random(20)==10) )      { bk_x[attr] = random( 320 ) ;  bk_y[attr] = -190 ; }
   else if( (bk_y[attr] < -190) && (random(20)==10) ) { bk_x[attr] = random( 320 ) ;  bk_y[attr] = 200 ; }
}

void move_bk( void )
{
   int i ;

   switch( back_scroll_speed )
   {
      case 0 :
	       break ;
      case 1 : back_x += bk_sx[0][my_air[0].fly_mode] ;
	       back_y += bk_sy[0][my_air[0].fly_mode] ;
	       break ;
      case 2 : back_x += bk_sx[1][my_air[0].fly_mode] ;
	       back_y += bk_sy[1][my_air[0].fly_mode] ;
	       break ;
      case 3 : back_x += (bk_sx[1][my_air[0].fly_mode]+bk_sx[0][my_air[0].fly_mode]) ;
	       back_y += (bk_sy[1][my_air[0].fly_mode]+bk_sy[0][my_air[0].fly_mode]) ;
	       break ;
      case 4 : back_x += bk_sx[1][my_air[0].fly_mode]<<1 ;
	       back_y += bk_sy[1][my_air[0].fly_mode]<<1 ;
	       break ;
      case 5 : back_x += bk_sx[1][my_air[0].fly_mode]<<2 ;
	       back_y += bk_sy[1][my_air[0].fly_mode]<<2 ;
	       break ;
   }
   if( my_turbo[0].attr && back_scroll_speed )
   {
      back_x += bk_sx[1][my_air[0].fly_mode] ;
      back_y += bk_sy[1][my_air[0].fly_mode] ;
   }

   if( !back_scroll_mode )
   {
      if( back_x < -319 ) back_x += 320 ;
      if( back_x > 0    ) back_x -= 320 ;
      if( back_y < -199 )
      {
	 back_y += 200 ;
	 if( bf_back_page[0] == 3 ) bf_back_page[0] = 0 ;
	 else                       bf_back_page[0]++ ;
	 if( bf_back_page[1] == 3 ) bf_back_page[1] = 0 ;
	 else                       bf_back_page[1]++ ;
      }
      if( back_y > 0    )
      {
	 back_y -= 200 ;
	 if( bf_back_page[0] == 0 ) bf_back_page[0] = 3 ;
	 else                       bf_back_page[0]-- ;
	 if( bf_back_page[1] == 0 ) bf_back_page[1] = 3 ;
	 else                       bf_back_page[1]-- ;
      }
   }
   else
   {
      if( back_x < -319 )
      {
	 back_x += 320 ;
	 for( i=0; i<2; i++ )
	 {
	    switch( bf_back_page[i] )
	    {
	       case 0 :
	       case 2 : bf_back_page[i]++ ;   break ;
	       case 1 : bf_back_page[i] = 0 ; break ;
	       case 3 : bf_back_page[i] = 2 ; break ;
	    }
	 }
      }
      if( back_x > 0    )
      {
	 back_x -= 320 ;
	 for( i=0; i<2; i++ )
	 {
	    switch( bf_back_page[i] )
	    {
	       case 1 :
	       case 3 : bf_back_page[i]-- ;   break ;
	       case 0 : bf_back_page[i] = 1 ; break ;
	       case 2 : bf_back_page[i] = 3 ; break ;
	    }
	 }
      }
      if( back_y < -199 )
      {
	 back_y += 200 ;
	 for( i=0; i<2; i++ )
	 {
	    switch( bf_back_page[i] )
	    {
	       case 0 :
	       case 1 : bf_back_page[i] += 2 ; break ;
	       case 2 : bf_back_page[i] = 0 ;  break ;
	       case 3 : bf_back_page[i] = 1 ;  break ;
	    }
	 }
      }
      if( back_y > 0    )
      {
	 back_y -= 200 ;
	 for( i=0; i<2; i++ )
	 {
	    switch( bf_back_page[i] )
	    {
	       case 2 :
	       case 3 : bf_back_page[i] -= 2 ; break ;
	       case 0 : bf_back_page[i] = 2 ;  break ;
	       case 1 : bf_back_page[i] = 3 ;  break ;
	    }
	 }
      }
   }

   switch( back_scroll_speed )
   {
      case 0 : bk_x[0] += bk_sx[1][my_air[0].fly_mode] ;
	       bk_y[0] += bk_sy[1][my_air[0].fly_mode] ;
	       break ;
      case 1 : bk_x[0] += bk_sx[1][my_air[0].fly_mode] ;
	       bk_y[0] += bk_sy[1][my_air[0].fly_mode] ;
	       break ;
      case 2 : bk_x[0] += (bk_sx[1][my_air[0].fly_mode]+bk_sx[0][my_air[0].fly_mode]) ;
	       bk_y[0] += (bk_sy[1][my_air[0].fly_mode]+bk_sy[0][my_air[0].fly_mode]) ;
	       break ;
      case 3 : bk_x[0] += bk_sx[1][my_air[0].fly_mode]<<1 ;
	       bk_y[0] += bk_sy[1][my_air[0].fly_mode]<<1 ;
	       break ;
      case 4 : bk_x[0] += bk_sx[1][my_air[0].fly_mode]<<2 ;
	       bk_y[0] += bk_sy[1][my_air[0].fly_mode]<<2 ;
	       break ;
      case 5 : bk_x[0] += bk_sx[1][my_air[0].fly_mode]<<3 ;
	       bk_y[0] += bk_sy[1][my_air[0].fly_mode]<<3 ;
	       break ;
   }
   chk_max_xy( 0 ) ;

   switch( back_scroll_speed )
   {
      case 0 : bk_x[1] += bk_sx[1][my_air[0].fly_mode] ;
	       bk_y[1] += bk_sy[1][my_air[0].fly_mode] ;
	       break ;
      case 1 : bk_x[1] += (bk_sx[1][my_air[0].fly_mode]+bk_sx[0][my_air[0].fly_mode]) ;
	       bk_y[1] += (bk_sy[1][my_air[0].fly_mode]+bk_sy[0][my_air[0].fly_mode]) ;
	       break ;
      case 2 : bk_x[1] += bk_sx[1][my_air[0].fly_mode]<<1 ;
	       bk_y[1] += bk_sy[1][my_air[0].fly_mode]<<1 ;
	       break ;
      case 3 : bk_x[1] += bk_sx[1][my_air[0].fly_mode]<<2 ;
	       bk_y[1] += bk_sy[1][my_air[0].fly_mode]<<2 ;
	       break ;
      case 4 : bk_x[1] += bk_sx[1][my_air[0].fly_mode]<<3 ;
	       bk_y[1] += bk_sy[1][my_air[0].fly_mode]<<3 ;
	       break ;
      case 5 : bk_x[1] += bk_sx[1][my_air[0].fly_mode]<<4 ;
	       bk_y[1] += bk_sy[1][my_air[0].fly_mode]<<4 ;
	       break ;
   }
   chk_max_xy( 1 ) ;
}

void Air_Output( int x, int y, char fly_mode, char air_air, unsigned char *buff )
{
   //**********> X Size 60, Y Size 50, Header 4 byte, 60 * 50 + 4 = 3004 Byte

   xmstomem( XMS_air, (long)XMS_air_select[air_air]*64000L+(long)fly_mode*3004L, image_buff, 3004 ) ;
   BufSpritePutImage( x, y, image_buff, buff ) ;
}

void Rader_Output( int x, int y, unsigned char color )
{
   x -= 160 ;
   y -= 100 ;
   if( !rader_attr )       //****** Zoom Mode (0=Normal,1=Zoom In)
   {
      if( x < 160 ) x = 245+2+21+(x/52) ;
      else          x = 245+2+21+(x/52) ;
      if( y < 100 ) y = 20+2+13+(y/78) ;
      else          y = 20+2+13+(y/78) ;
      BufPutPixel( x, y, color, mcga1 ) ;
   }
   else
   {
      if( (x > -500 ) && (x < 500 ) &&
	  (y > -500 ) && (y < 500 ) )
      {
	 if( x < 160 ) x = 245+2+21+(x/26) ;
	 else          x = 245+2+21+(x/26) ;
	 if( y < 100 ) y = 20+2+13+(y/39) ;
	 else          y = 20+2+13+(y/39) ;
	 BufPutPixel( x, y, color, mcga1 ) ;
      }
   }
}

extern char *wing_man_mess1[10]  ;   // Wing Man Talk Message (Air Start)
extern char *wing_man_mess21[10] ;   // Wing Man Talk Message (1 Kill)
extern char *wing_man_mess22[10] ;   // Wing Man Talk Message (2 Kill)
extern char *wing_man_mess23[10] ;   // Wing Man Talk Message (3 Kill)
extern char *wing_man_mess3[10]  ;   // Wing Man Talk Message (My Kill)
extern char *wing_man_mess4[10]  ;   // Wing Man Talk Message (My Life)

void output_sprite( void )
{
   int i, j, k ;

   //************************ New Stage Set And Start ***********************
   if( stage.end_view_count == 2 )
   {
      if( (GetTime()-stage.view_time) > 550L )
      {
	 if( !panicmode )
         {
            unsigned char iPal[256][3] ;

            scenario_program( 2 ) ;            // Scenario Programming End

            sprintf( st, "stage%de", stage.number ) ;
            if( CheckFLI( st ) )
            {
               black_palette( 1 ) ;                    // Set Slow Black Palette
               BufXLine( 0, 0, 64000, 0, mcga0 ) ;     // Screen Black Clear
               memtoxms( mcga1, XMS_hand, 0L,     64000L ) ;
               memtoxms( mcga2, XMS_hand, 64000L, 64000L ) ;
               memcpy( &iPal, &Pal, 768L ) ;           // Palette Backup
               ViewFLI( st, 0 ) ;                      // FLIC Animation
               black_palette( 1 ) ;                    // Set Slow Black Palette
               BufXLine( 0, 0, 64000, 0, mcga0 ) ;     // Screen Black Clear
               memcpy( &Pal, &iPal, 768L ) ;           // Palette Restore
               SetRgbPaletteSnow( (unsigned char *)Pal ) ;
               xmstomem( XMS_hand, 0L,     mcga1, 64000L ) ;
               xmstomem( XMS_hand, 64000L, mcga2, 64000L ) ;
            }

            if( stage.number == 15 )                 //****** End Stage
            {
               exit_code = 0 ;                       //****** Exit Code
            }
            else
            {
               black_palette( 1 ) ;                     // Set Slow Black Palette
               AdLib_Start( "bf-demo0" ) ;
               sprintf( st, "bf-eye%d", stage.number ) ;
               ViewIMG( st, mcga0 ) ;
               white_palette() ;                        // Set Slow Orignal Palette
               waitmSec( 5000 ) ;
               i = 0 ;
               j = 0 ;
               SetPalette( 169, i, i, i ) ;
               Hprint( 20, 170, "[ Q ]", 169 ) ;
               while( !keycode[M_ESC] && !keycode[M_ENTER] && !keycode[M_SPACEBAR] )
               {
                  if( keycode[M_Q] )                    // Q key is Save Game
                  {
                     stage.number++ ;
                     exit_code = 0 ;
                     return ;
                  }
                  if( j == 0 )
                  {
                     if( i == 63 ) j = 1 ;
                     else i++ ;
                  }
                  if( j == 1 )
                  {
                     if( i ==  0 ) j = 0 ;
                     else i-- ;
                  }
                  SetPalette( 169, i, i, i ) ;
               }
               set_stage() ;                        // New Stage Data Setting
            }
         }
      }
   }

   //*************************** Screen Clear *******************************
   if( speed )
   {
//      FillPage386( 237, mcga1 ) ;

      *(mcga3+0) = 319 &  0x00ff ;
      *(mcga3+1) = 319 >> 8      ;
      *(mcga3+2) = 199 &  0x00ff ;
      *(mcga3+3) = 199 >> 8      ;

      switch( back_scroll_mode )
      {
	 case 0 :
	    xmstomem( XMS_back, bf_back_page[0]*64000L, mcga3+4, 64000 ) ;
	    BufPutImage( back_x,     back_y,     mcga3, mcga1 ) ;
	    BufPutImage( back_x+320, back_y,     mcga3, mcga1 ) ;
	    xmstomem( XMS_back, bf_back_page[1]*64000L, mcga3+4, 64000 ) ;
	    BufPutImage( back_x,     back_y+200, mcga3, mcga1 ) ;
	    BufPutImage( back_x+320, back_y+200, mcga3, mcga1 ) ;
	    break ;
	 case 1 :
	    xmstomem( XMS_back, bf_back_page[0]*64000L, mcga3+4, 64000 ) ;
	    BufPutImage( back_x,     back_y,     mcga3, mcga1 ) ;
	    switch( bf_back_page[0] )
	    {
	       case 0 :
	       case 2 :
		  xmstomem( XMS_back, (bf_back_page[0]+1)*64000L, mcga3+4, 64000 ) ;
		  break ;
	       case 1 :
		  xmstomem( XMS_back, 0L, mcga3+4, 64000 ) ;
		  break ;
	       case 3 :
		  xmstomem( XMS_back, 2L*64000L, mcga3+4, 64000 ) ;
		  break ;
	    }
	    BufPutImage( back_x+320, back_y,     mcga3, mcga1 ) ;

	    xmstomem( XMS_back, bf_back_page[1]*64000L, mcga3+4, 64000 ) ;
	    BufPutImage( back_x,     back_y+200, mcga3, mcga1 ) ;
	    switch( bf_back_page[1] )
	    {
	       case 0 :
	       case 2 :
		  xmstomem( XMS_back, (bf_back_page[1]+1)*64000L, mcga3+4, 64000 ) ;
		  break ;
	       case 1 :
		  xmstomem( XMS_back, 0L, mcga3+4, 64000 ) ;
		  break ;
	       case 3 :
		  xmstomem( XMS_back, 2L*64000L, mcga3+4, 64000 ) ;
		  break ;
	    }
	    BufPutImage( back_x+320, back_y+200, mcga3, mcga1 ) ;
	    break ;
	 case 2 :
	    BufXLine( 0, 0, 64000, 34, mcga1 ) ;
	    break ;
      }
   }
   else
   {
      BufXLine( 0, 0, 64000, (4<<4)+2, mcga1 ) ;
   }

   //*************************** MY GUN & MIS Output *************************
   for( j=0; j<MAXMYAIR; j++ )
   {
      if( my_gun[j].weapon == 1 )                  //******* Auto Gun Weapon
      {
	 for( i=0; i<MAXGUN; i++ )
	 {
	    if( my_gun[j].attr[i] )
	       BufSpritePutImage( my_gun[j].x[i], my_gun[j].y[i], gun_image[my_gun[j].fly_mode[i]], mcga1 ) ;
	 }
      }
      if( my_gun[j].weapon == 2 )                  //******* Laser Weapon
      {
	 if( my_gun[j].fly_mode[1] )
	 {
	    laser_gun_attack( j ) ;
	    my_gun[j].fly_mode[1] = 0 ;
	 }
      }
      if( my_gun[j].weapon == 3 )                  //****** Auto Laser Weapon
      {
	 if( my_gun[j].fly_mode[1] )
	 {
	    laser_gun_attack( j ) ;
	    my_gun[j].fly_mode[1] = 0 ;
	 }
      }

      if( my_flare[j].attr ) BufSpritePutImage( my_flare[j].x, my_flare[j].y, flare_image[flare_code[my_flare[j].count]], mcga1 ) ;
      if( my_laser[j].count )
      {
	 if( my_laser[j].count == 30 )
	 {
	    laser_bim_attack( j ) ;
	 }
	 my_laser[j].count-- ;
      }
   }
   if( my_mis.attr ) XMSPutImage( my_mis.x, my_mis.y, XMS_IMG_MISSILE+my_mis.fly_mode, mcga1 ) ;

   //************************** CHAR GUN & MIS Image Output ******************
   for( j=0; j<MAXCHAIR; j++ )
   {
      for( i=0; i<MAXGUN; i++ )
      {
	 if( ch_gun[j].attr[i] )
	    BufSpritePutImage( ch_gun[j].x[i], ch_gun[j].y[i], gun_image[ch_gun[j].fly_mode[i]], mcga1 ) ;
      }
   }
   if( ch_flare[my_mis_target.target].attr )
      BufSpritePutImage( ch_flare[my_mis_target.target].x, ch_flare[my_mis_target.target].y,
			 flare_image[flare_code[ch_flare[my_mis_target.target].count]], mcga1 ) ;
   if( ch_mis.attr >= 3 ) XMSPutImage( ch_mis.x, ch_mis.y, XMS_IMG_MISSILE+ch_mis.fly_mode, mcga1 ) ;

   if( !buster.size ) buster.size = 1 ;
   else               buster.size = 0 ;

   //************************** CHAR Air Image Output ************************
   if( !stage.view_count )
   {
      for( k=0; k<MAXCHAIR; k++ )
      {
	 if( ( ch_air[k].kill > 0 ) && ( ch_air[k].kill < 14 ) )
	 {
	    if( ch_air[k].kill_attr == 0 )
	    {
	       ch_wing[k][0].fly_mode = ch_air[k].fly_mode ;
	       ch_wing[k][1].fly_mode = ch_air[k].fly_mode + 2 ;
	       ch_wing[k][2].fly_mode = ch_air[k].fly_mode - 2 ;
	       ch_wing[k][3].fly_mode = ch_air[k].fly_mode + 6 ;
	       ch_wing[k][4].fly_mode = ch_air[k].fly_mode - 6 ;
	       for( i=0; i<5; i++ )
	       {
		  ch_wing[k][i].x       = ch_air[k].x + 30 ;
		  ch_wing[k][i].y       = ch_air[k].y + 25 ;
		  ch_wing[k][i].img_num = 0 ;
		  for( j=0; j<4; j++ )
		  {
		     ch_wing[k][i].flag_x[j]     = ch_wing[k][i].x + random(10)+5 ;
		     ch_wing[k][i].flag_y[j]     = ch_wing[k][i].y + random(10)+5 ;
		     ch_wing[k][i].flag_count[j] = j ;
		  }

		  if( ch_wing[k][i].fly_mode < 0  ) ch_wing[k][i].fly_mode += 16 ;
		  if( ch_wing[k][i].fly_mode > 15 ) ch_wing[k][i].fly_mode -= 16 ;
	       }

	       ch_air[k].kill_attr++ ;
	    }
	    if( ch_air[k].kill_attr == 1 )
	    {
	       for( i=0; i<5; i++ )
	       {
		  ch_wing[k][i].x -= bk_sx[1][ch_wing[k][i].fly_mode] ;
		  ch_wing[k][i].y -= bk_sy[1][ch_wing[k][i].fly_mode] ;
		  if( my_turbo[0].attr )
		  {
		     ch_wing[k][i].x -= bk_sx[1][ch_wing[k][i].fly_mode] ;
		     ch_wing[k][i].y -= bk_sy[1][ch_wing[k][i].fly_mode] ;
		  }
		  XMSPutImage( ch_wing[k][i].x, ch_wing[k][i].y, XMS_IMG_KILL_WING+ch_wing[k][i].img_num, mcga1 ) ;
		  if( ch_wing[k][i].img_num == 4 ) ch_wing[k][i].img_num = 0 ;
		  else                             ch_wing[k][i].img_num++ ;

		  for( j=0; j<4; j++ )
		  {
		     ch_wing[k][i].flag_x[j] += (bk_sx[1][ch_air[k].fly_mode]<<1) ;
		     ch_wing[k][i].flag_y[j] += (bk_sy[1][ch_air[k].fly_mode]<<1) ;
		     if( my_turbo[0].attr )
		     {
			ch_wing[k][i].flag_x[j] += bk_sx[1][ch_air[k].fly_mode] ;
			ch_wing[k][i].flag_y[j] += bk_sy[1][ch_air[k].fly_mode] ;
		     }
		     BufSpritePutImage( ch_wing[k][i].flag_x[j], ch_wing[k][i].flag_y[j], air_flag_image[ch_wing[k][i].flag_count[j]], mcga1 ) ;
		     if( ch_wing[k][i].flag_count[j] == 3 )
		     {
			ch_wing[k][i].flag_x[j]     = ch_wing[k][i].x + random(10)+5 ;
			ch_wing[k][i].flag_y[j]     = ch_wing[k][i].y + random(10)+5 ;
			ch_wing[k][i].flag_count[j] = 0 ;
		     }
		     else
			ch_wing[k][i].flag_count[j]++ ;
		  }
	       }

	       XMSPutImage( ch_air[k].x-kill_pos_x[ch_air[k].kill>>1]+30,
			    ch_air[k].y-kill_pos_y[ch_air[k].kill>>1]+25,
			    XMS_IMG_KILL+(ch_air[k].kill>>1), mcga1 ) ;
	       if( ch_air[k].kill <  14 ) ch_air[k].kill++ ;
	       if( ch_air[k].kill == 14 ) new_char( k, random(4) ) ;
	    }
	 }
	 else
	 {
	    if( (start_attr == 50) && !ch_air[k].kill )
	    {
	       if( !buster.size ) BufSpritePutImage( ch_air[k].x+buster.x[buster.attr][ch_air[k].air][buster.size][ch_air[k].fly_mode], ch_air[k].y+buster.y[buster.attr][ch_air[k].air][buster.size][ch_air[k].fly_mode], buster_image[buster.img_num[ch_air[k].air]][buster.size][ch_air[k].fly_mode], mcga1 ) ;
	       else               BufSpritePutImage( ch_air[k].x+buster.x[buster.attr][ch_air[k].air][buster.size][ch_air[k].fly_mode], ch_air[k].y+buster.y[buster.attr][ch_air[k].air][buster.size][ch_air[k].fly_mode], buster_image[buster.img_num[ch_air[k].air]][buster.size][ch_air[k].fly_mode], mcga1 ) ;
	       Air_Output( ch_air[k].x, ch_air[k].y, ch_air[k].fly_mode, ch_air[k].air, mcga1 ) ;
	    }
	 }
      }
   }

   //************************** MY Air Image Output **************************
   for( k=0; k<MAXMYAIR; k++ )
   {
      if( my_air[k].kill_mode )
      {
	 if( my_air[k].kill_mode == 1 )
	 {
	    if( my_air[k].kill_attr == 0 )
	    {
	       my_wing[k][0].fly_mode = my_air[k].fly_mode ;
	       my_wing[k][1].fly_mode = my_air[k].fly_mode + 2 ;
	       my_wing[k][2].fly_mode = my_air[k].fly_mode - 2 ;
	       my_wing[k][3].fly_mode = my_air[k].fly_mode + 6 ;
	       my_wing[k][4].fly_mode = my_air[k].fly_mode - 6 ;
	       for( i=0; i<5; i++ )
	       {
		  my_wing[k][i].x       = my_air[k].x + 30 ;
		  my_wing[k][i].y       = my_air[k].y + 25 ;
		  my_wing[k][i].img_num = 0 ;
		  for( j=0; j<4; j++ )
		  {
		     my_wing[k][i].flag_x[j]     = my_wing[k][i].x + random(10)+5 ;
		     my_wing[k][i].flag_y[j]     = my_wing[k][i].y + random(10)+5 ;
		     my_wing[k][i].flag_count[j] = j ;
		  }

		  if( my_wing[k][i].fly_mode < 0  ) my_wing[k][i].fly_mode += 16 ;
		  if( my_wing[k][i].fly_mode > 15 ) my_wing[k][i].fly_mode -= 16 ;
	       }

	       my_air[k].kill_attr++ ;
	    }
	    if( my_air[k].kill_attr == 1 )
	    {
	       for( i=0; i<5; i++ )
	       {
		  my_wing[k][i].x -= bk_sx[1][my_wing[k][i].fly_mode] ;
		  my_wing[k][i].y -= bk_sy[1][my_wing[k][i].fly_mode] ;
		  if( my_turbo[0].attr )
		  {
		     my_wing[k][i].x -= bk_sx[1][my_wing[k][i].fly_mode] ;
		     my_wing[k][i].y -= bk_sy[1][my_wing[k][i].fly_mode] ;
		  }
		  XMSPutImage( my_wing[k][i].x, my_wing[k][i].y, XMS_IMG_KILL_WING+my_wing[k][i].img_num, mcga1 ) ;
		  if( my_wing[k][i].img_num == 4 ) my_wing[k][i].img_num = 0 ;
		  else                             my_wing[k][i].img_num++ ;

		  for( j=0; j<4; j++ )
		  {
		     my_wing[k][i].flag_x[j] += (bk_sx[1][my_air[0].fly_mode]<<1) ;
		     my_wing[k][i].flag_y[j] += (bk_sy[1][my_air[0].fly_mode]<<1) ;
		     if( my_turbo[0].attr )
		     {
			my_wing[k][i].flag_x[j] += bk_sx[1][my_air[0].fly_mode] ;
			my_wing[k][i].flag_y[j] += bk_sy[1][my_air[0].fly_mode] ;
		     }
		     BufSpritePutImage( my_wing[k][i].flag_x[j], my_wing[k][i].flag_y[j], air_flag_image[my_wing[k][i].flag_count[j]], mcga1 ) ;
		     if( my_wing[k][i].flag_count[j] == 3 )
		     {
			my_wing[k][i].flag_x[j]     = my_wing[k][i].x + random(10)+5 ;
			my_wing[k][i].flag_y[j]     = my_wing[k][i].y + random(10)+5 ;
			my_wing[k][i].flag_count[j] = 0 ;
		     }
		     else
			my_wing[k][i].flag_count[j]++ ;

		  }
	       }

	       XMSPutImage( my_air[k].x-kill_pos_x[my_air[k].kill>>1]+30,
			    my_air[k].y-kill_pos_y[my_air[k].kill>>1]+25,
			    XMS_IMG_KILL+(my_air[k].kill>>1), mcga1 ) ;
	       if( my_air[k].kill < 14 ) my_air[k].kill++ ;
	    }
	 }
	 if( my_air[k].kill_mode == 2 )
	 {
	    if( my_air[k].kill_attr == 0 )
	    {
	       my_air[k].kill_count = 30 ;
	       my_air[k].kill_attr++ ;
	    }
	    if( my_air[k].kill_attr == 1 )
	    {
	       my_air[k].kill_count-- ;
	       if( !my_air[k].kill_count )
	       {
		  my_air[k].kill_mode = 1 ;
		  my_air[k].kill_attr = 0 ;
		  my_air[k].kill = 1 ;
		  sayPCM( PCM_KILL, 1 ) ;
	       }
	    }
	 }
      }
   }

   if( !my_air[0].kill )
   {
      if( (start_attr >= 1) && (start_attr < 50) )
      {
	 start_attr++ ;
	 if( start_attr == 3  )
	 {
	    start_file = open( "bf.idb", O_RDONLY | O_BINARY, S_IWRITE | S_IREAD ) ;
	    sayPCM( PCM_BURSTER1, 1 ) ;
	 }
	 if( ( start_attr == 6  ) || ( start_attr == 8  ) ||
	     ( start_attr == 10 ) || ( start_attr == 12 ) ||
	     ( start_attr == 14 ) || ( start_attr == 16 ) ||
	     ( start_attr == 18 ) )
	 {
	    read( start_file, &start_x, 2L ) ;
	    read( start_file, &start_y, 2L ) ;
	    read( start_file, mcga2+4L, (start_x+1)*(start_y+1) ) ;
	    *(mcga2+0) = start_x &  0x00ff ;
	    *(mcga2+1) = start_x >> 8      ;
	    *(mcga2+2) = start_y &  0x00ff ;
	    *(mcga2+3) = start_y >> 8      ;

	    for( j=0; j<MAXMYAIR; j++ )
	    {
	       if( !my_air[j].kill )
		  BufSpritePutImage( my_air[j].x-(start_x>>1)+30, my_air[j].y-(start_y>>1)+25, mcga2, mcga1 ) ;
	    }
	 }
	 if( (start_attr == 7  ) || (start_attr == 9  ) ||
	     (start_attr == 11 ) || (start_attr == 13 ) ||
	     (start_attr == 15 ) || (start_attr == 17 ) ||
	     (start_attr == 19 ) )
	 {
	    for( j=0; j<MAXMYAIR; j++ )
	    {
	       if( !my_air[j].kill )
		  BufSpritePutImage( my_air[j].x-(start_x>>1)+30, my_air[j].y-(start_y>>1)+25, mcga2, mcga1 ) ;
	    }
	 }
	 if( start_attr == 20 )
	 {
	    close( start_file ) ;
	    start_attr = 50 ;
	 }
      }
      if( stage.end_view_count==2 )
      {
	 for( j=0; j<MAXMYAIR; j++ )
	 {
	    if( !my_air[j].kill )
	    {
	       my_air[j].x -= bk_sx[0][my_air[j].fly_mode] ;
	       my_air[j].y -= bk_sy[0][my_air[j].fly_mode] ;
	    }
	 }
      }
      if( start_attr >= 20 )
      {
	 for( j=0; j<MAXMYAIR; j++ )
	 {
	    if( !my_air[j].kill )
	    {
	       if( my_turbo[j].attr )
		  BufSpritePutImage( my_air[j].x+buster.x[buster.attr][my_air[j].air][buster.size][my_air[j].fly_mode], my_air[j].y+buster.y[buster.attr][my_air[j].air][buster.size][my_air[j].fly_mode], buster_image[buster.img_num[my_air[j].air]][buster.size][my_air[j].fly_mode], mcga1 ) ;
	    }

	    if( !my_air[j].kill )
	    {
	       if( my_air[j].roll_attr )      //************* Solgae Roll !!!
	       {
		  if( my_air[j].roll_delay == 1 )
		  {
		     if( my_air[j].roll_count == 7 )
		     {
			my_air[j].roll_attr = 0 ;
			my_air[j].roll_count = 0 ;
			my_air[j].roll_delay = 0 ;
		     }
		     else
			my_air[j].roll_count++ ;
		     my_air[j].roll_delay = 0 ;
		  }
		  else my_air[j].roll_delay++ ;
		  if( my_air[j].fly_mode == 0 )
		     XMSPutImage( my_air[j].x+3, my_air[j].y+7, XMS_IMG_ROLL0+my_air[j].roll_count, mcga1 ) ;
		  else
		     XMSPutImage( my_air[j].x+5, my_air[j].y+7, XMS_IMG_ROLL1+my_air[j].roll_count, mcga1 ) ;
	       }
	       else
		  Air_Output( my_air[j].x, my_air[j].y, my_air[j].fly_mode, my_air[j].air, mcga1 ) ;
	    }
	 }
	 if( start_attr > 40 ) start_attr = 50 ;
	 else                  start_attr++ ;
      }
   }

   //*************************** FLAG OUTPUT *********************************
   for( j=0; j<MAXMYAIR; j++ )
   {
      if( !my_air[j].kill )
      {
	 for( i=0; i<MAXFLAG; i++ )
	 {
	    if( my_flag[j].attr[i] )
	    {
	       BufSpritePutImage( my_flag[j].x[i]-(*air_flag_image[my_flag[j].count[i]]>>1),
				  my_flag[j].y[i]-(*(air_flag_image[my_flag[j].count[i]]+2)>>1),
				  air_flag_image[my_flag[j].count[i]], mcga1 ) ;
	       if( my_flag[j].count[i] == (MAXFLAG-1) )
	       {
		  my_flag[j].count[i] = 0 ;
		  my_flag[j].x[i] = my_air[j].x+random(30)+15 ;
		  my_flag[j].y[i] = my_air[j].y+random(30)+15 ;
	       }
	       else my_flag[j].count[i]++ ;
	    }
	 }
      }
   }
   for( j=0; j<MAXCHAIR; j++ )
   {
      if( !my_air[j].kill )
      {
	 for( i=0; i<MAXFLAG; i++ )
	 {
	    if( ch_flag[j].attr[i] )
	    {
	       BufSpritePutImage( ch_flag[j].x[i]-(*air_flag_image[ch_flag[j].count[i]]>>1),
				  ch_flag[j].y[i]-(*(air_flag_image[ch_flag[j].count[i]]+2)>>1),
				  air_flag_image[ch_flag[j].count[i]], mcga1 ) ;
	       if( ch_flag[j].count[i] == (MAXFLAG-1) )
	       {
		  ch_flag[j].count[i] = 0 ;
		  ch_flag[j].x[i] = ch_air[j].x + random(30)+15 ;
		  ch_flag[j].y[i] = ch_air[j].y + random(30)+15 ;
	       }
	       else ch_flag[j].count[i]++ ;
	    }
	 }
      }
   }

   for( i=0; i<MAXFLAG; i++ )
   {
      if( my_mis_flag.attr[i] )
      {
	 BufSpritePutImage( my_mis_flag.x[i]-(*mis_flag_image[my_mis_flag.count[i]]>>1),
			    my_mis_flag.y[i]-(*(mis_flag_image[my_mis_flag.count[i]]+2)>>1),
			    mis_flag_image[my_mis_flag.count[i]], mcga1 ) ;
	 if( my_mis_flag.count[i] == (MAXFLAG-1) )
	 {
	    my_mis_flag.count[i] = 0 ;
	    my_mis_flag.x[i] = my_mis.x+15 ;
	    my_mis_flag.y[i] = my_mis.y+10 ;
	 }
	 else my_mis_flag.count[i]++ ;
      }
      if( !my_mis.attr ) my_mis_flag.attr[i] = 0 ;

      if( ch_mis_flag.attr[i] )
      {
	 BufSpritePutImage( ch_mis_flag.x[i]-(*mis_flag_image[ch_mis_flag.count[i]]>>1),
			    ch_mis_flag.y[i]-(*(mis_flag_image[ch_mis_flag.count[i]]+2)>>1),
			    mis_flag_image[ch_mis_flag.count[i]], mcga1 ) ;
	 if( ch_mis_flag.count[i] == (MAXFLAG-1) )
	 {
	    ch_mis_flag.count[i] = 0 ;
	    ch_mis_flag.x[i] = ch_mis.x+15 ;
	    ch_mis_flag.y[i] = ch_mis.y+10 ;
	 }
	 else ch_mis_flag.count[i]++ ;
      }

      if( !ch_mis.attr ) ch_mis_flag.attr[i] = 0 ;
   }

   if( !panicmode ) scenario_program( 1 ) ;

   //************************** Back Image Output ****************************
   if( speed && (back_scroll_mode!=2) )
   {
      XMSPutImage( bk_x[0], bk_y[0], XMS_IMG_BACK0, mcga1 ) ;
      XMSPutImage( bk_x[1], bk_y[1], XMS_IMG_BACK1, mcga1 ) ;
   }

   //************************ Target Output **********************************
   if( !my_air[0].kill && (start_attr==50) && my_mis_target.attr &&
       !stage.view_count && !stage.end_view_count && !panicmode )
   {
      XMSPutImage( my_mis_target.x-10, my_mis_target.y+20, XMS_IMG_TARGET, mcga1 ) ;
   }

   //************************ Shadow Scroll **********************************
   if( back_scroll_mode == 2 )
   {
      *(mcga3+0) = 319 &  0x00ff ;
      *(mcga3+1) = 319 >> 8      ;
      *(mcga3+2) = 199 &  0x00ff ;
      *(mcga3+3) = 199 >> 8      ;

      xmstomem( XMS_back, bf_back_page[0]*64000L, mcga3+4, 64000 ) ;
      BufBackSpritePutImage( back_x,     back_y,     mcga3, mcga1 ) ;
      switch( bf_back_page[0] )
      {
	 case 0 :
	 case 2 :
	    xmstomem( XMS_back, (bf_back_page[0]+1)*64000L, mcga3+4, 64000 ) ;
	    break ;
	 case 1 :
	    xmstomem( XMS_back, 0L, mcga3+4, 64000 ) ;
	    break ;
	 case 3 :
	    xmstomem( XMS_back, 2L*64000L, mcga3+4, 64000 ) ;
	    break ;
      }
      BufBackSpritePutImage( back_x+320, back_y,     mcga3, mcga1 ) ;

      xmstomem( XMS_back, bf_back_page[1]*64000L, mcga3+4, 64000 ) ;
      BufBackSpritePutImage( back_x,     back_y+200, mcga3, mcga1 ) ;
      switch( bf_back_page[1] )
      {
	 case 0 :
	 case 2 :
	    xmstomem( XMS_back, (bf_back_page[1]+1)*64000L, mcga3+4, 64000 ) ;
	    break ;
	 case 1 :
	    xmstomem( XMS_back, 0L, mcga3+4, 64000 ) ;
	    break ;
	 case 3 :
	    xmstomem( XMS_back, 2L*64000L, mcga3+4, 64000 ) ;
	    break ;
      }
      BufBackSpritePutImage( back_x+320, back_y+200, mcga3, mcga1 ) ;
   }

   //************************* Blue Falcon Rader Output **********************
   if( my_air[0].kill && (rader_scale_mode==1) )
   {
      rader_scale_data_x = 46 ;
      rader_scale_data_y = 35 ;
      rader_scale_mode = 3 ;
   }
   if( (start_attr==50) && rader_scale_mode )
   {
      switch( rader_scale_mode )
      {
         case 1 :
            XMSPutImage( 245, 20, XMS_IMG_RADER, mcga1 ) ;
            break ;
         case 2 :
            if( rader_scale_data_x != 46 ) rader_scale_data_x++ ;
            if( rader_scale_data_y != 35 ) rader_scale_data_y++ ;
            if( (rader_scale_data_x==46) && (rader_scale_data_y==35) ) rader_scale_mode = 1 ;
            XMSPutImage( -100, -100, XMS_IMG_RADER, mcga1 ) ;
            Scale( 245+23-(rader_scale_data_x>>1), 20+17-(rader_scale_data_y>>1),
                   rader_scale_data_x, rader_scale_data_y, image_buff, mcga1 ) ;
            break ;
         case 3 :
            if( rader_scale_data_x != 12 ) rader_scale_data_x-- ;
            if( rader_scale_data_y != 1  ) rader_scale_data_y-- ;
            if( (rader_scale_data_x==12) && (rader_scale_data_y==1) ) rader_scale_mode = 0 ;
            XMSPutImage( -100, -100, XMS_IMG_RADER, mcga1 ) ;
            Scale( 245+23-(rader_scale_data_x>>1), 20+17-(rader_scale_data_y>>1),
                   rader_scale_data_x, rader_scale_data_y, image_buff, mcga1 ) ;
         break ;
      }
      if( !stage.view_count && (rader_scale_mode==1) )
      {
	 for( i=1; i<MAXMYAIR; i++ )
	 {
	    if( !my_air[i].kill ) Rader_Output( my_air[i].x, my_air[i].y, 144 ) ;
	 }
	 for( i=0; i<MAXCHAIR; i++ )
	 {
	    if( !ch_air[i].kill )
	    {
	       if( ch_air[i].air != 6 )   //**** AIR=6 F117 Rader Not Found !
		  Rader_Output( ch_air[i].x, ch_air[i].y, 58 ) ;
	    }
	 }
	 if( my_mis_target.attr )
	 {
	    if( ch_air[my_mis_target.target].air != 6 )   // AIR=6 F117 Rader Not Found !
	    {
	       Rader_Output( my_mis_target.x-100,
			     my_mis_target.y-100, (6<<4)+3 ) ;
	       Rader_Output( my_mis_target.x+100,
			     my_mis_target.y-100, (6<<4)+3 ) ;
	       Rader_Output( my_mis_target.x-100,
			     my_mis_target.y+100, (6<<4)+3 ) ;
	       Rader_Output( my_mis_target.x+100,
			     my_mis_target.y+100, (6<<4)+3 ) ;
	    }
	 }
      }
      if( my_mis.attr      ) Rader_Output( my_mis.x, my_mis.y, 189 ) ;
      if( ch_mis.attr >= 3 ) Rader_Output( ch_mis.x, ch_mis.y, 189 ) ;

      stage_rader() ;
   }

   //************************ Stage Start Message Output *********************
   if( stage.view_count == 1 )
   {
      if( (GetTime()-stage.view_time) > 300L )
      {
	 stage.view_count = 2 ;
	 stage.view_time  = GetTime() ;
	 eye_ani.attr = 0 ;
         stage_title_pos_x = 20 ;
         stage_title_pos_y = -25 ;
         stage_mask_pos_x = 245 ;
         stage_mask_pos_y = 200 ;
      }
   }

   if( stage.view_count == 2 )
   {
      if( (GetTime()-stage.view_time) < 700L )
      {
         BufHprint( stage_title_pos_x, stage_title_pos_y, stage.name, 102, mcga1 ) ;
         if( ((GetTime()-stage.view_time) > 0L) &&
             ((GetTime()-stage.view_time) < 150L) ) stage_title_pos_y += 3 ;
         if( ((GetTime()-stage.view_time) > 550L) &&
             ((GetTime()-stage.view_time) < 700L) ) stage_title_pos_x -= 12 ;
         if( ((GetTime()-stage.view_time) > 0L) &&
             ((GetTime()-stage.view_time) < 150L) )
         {
             if( stage_mask_pos_y > 140 ) stage_mask_pos_y -= 3  ;
             else                         stage_mask_pos_y = 140 ;
         }
	 XMSPutImage( stage_mask_pos_x, stage_mask_pos_y, XMS_IMG_MASK, mcga1 ) ;//245 140
	 if( !eye_ani.attr )
	 {
	    if( random(50)==5 )
	    {
	       eye_ani.attr = 1 ;
	       eye_ani.ani_time = GetTime() ;
	    }
	    else
	    {
	       if( random(50)==5 )
	       {
		  eye_ani.attr = 2 ;
		  eye_ani.ani_time = GetTime() ;
	       }
	       else
	       {
		  if( random(40)==5 )
		  {
		     eye_ani.attr = 3 ;
		     eye_ani.ani_time = GetTime() ;
		  }
		  else
		  {
		     if( random(40)==5 )
		     {
			eye_ani.attr = 4 ;
			eye_ani.ani_time = GetTime() ;
			eye_ani.count = 5 ;
		     }
		  }
	       }
	    }
	 }
	 else
	 {
	    if( ( eye_ani.attr == 1 ) || ( eye_ani.attr == 2 ) || ( eye_ani.attr == 3 ) )
	    {
	       if( (GetTime()-eye_ani.ani_time) > 50L ) eye_ani.attr = 0 ;
	    }
	    if( eye_ani.attr == 4 )
	    {
	       if( (GetTime()-eye_ani.ani_time) > 15L )
	       {
		  if( eye_ani.count == 6 ) eye_ani.attr = 5 ;
		  else
		  {
		     eye_ani.count++ ;
		     eye_ani.ani_time = GetTime() ;
		  }
	       }
	    }
	    if( eye_ani.attr == 5 )
	    {
	       if( (GetTime()-eye_ani.ani_time) > 15L )
	       {
		  eye_ani.count-- ;
		  if( eye_ani.count == 5 ) eye_ani.attr = 0 ;
		  eye_ani.ani_time = GetTime() ;
	       }
	    }
	 }
	 if( eye_ani.attr == 1 ) XMSPutImage( 245+11, stage_mask_pos_y+25, XMS_IMG_EYES+7, mcga1 ) ;
	 if( eye_ani.attr == 2 ) XMSPutImage( 245+11, stage_mask_pos_y+25, XMS_IMG_EYES+8, mcga1 ) ;
	 if( eye_ani.attr == 3 ) XMSPutImage( 245+11, stage_mask_pos_y+25, XMS_IMG_EYES+4, mcga1 ) ;
	 if( eye_ani.attr == 4 ) XMSPutImage( 245+11, stage_mask_pos_y+25, XMS_IMG_EYES+eye_ani.count, mcga1 ) ;
	 if( eye_ani.attr == 5 ) XMSPutImage( 245+11, stage_mask_pos_y+25, XMS_IMG_EYES+eye_ani.count, mcga1 ) ;
      }
      else
      {
	 stage.view_count = 3 ;
	 stage.view_time = GetTime() ;
	 eye_ani.count = 0 ;
	 eye_ani.ani_time = GetTime() ;
         stage_title_pos_x = -170 ;
         stage_title_pos_y = 60 ;
         stage_mask_pos_x = 245 ;
         stage_mask_pos_y = 140 ;
      }
   }

   if( stage.view_count == 3 )
   {
      if( (GetTime()-stage.view_time) > 200L )
      {
	 stage.view_count = 4 ;
	 if( !panicmode ) scenario_program( 1 ) ;  // Scenario Start
         else             stage.view_count = 0 ;   // Direct Start
      }
      else
      {
         if( (GetTime()-stage.view_time) > 100L )
         {
            if( eye_ani.count == 3 ) stage_mask_pos_x += 10 ;
         }
	 XMSPutImage( stage_mask_pos_x, 140, XMS_IMG_MASK, mcga1 ) ;
	 if( (GetTime()-eye_ani.ani_time) > 15L )
	 {
	    eye_ani.ani_time = GetTime() ;
	    XMSPutImage( stage_mask_pos_x+11, 140+25, XMS_IMG_EYES+eye_ani.count, mcga1 ) ;
	    if( eye_ani.count != 3 ) eye_ani.count++ ;
	 }
	 else
	 {
	    XMSPutImage( stage_mask_pos_x+11, 140+25, XMS_IMG_EYES+eye_ani.count, mcga1 ) ;
	 }
      }
   }

   //************************ Air vs Air Message Output *********************
   for( i=1; i<MAXMYAIR; i++ )
   {
      if( my_air_target[i].mes_attr )
      {
	 if( my_air_target[i].mes_attr == 1 )
	 {
	    if( my_air_target[i].mes_time < 30 )
	    {
	       XMSPutImage( 20, 140, XMS_IMG_MASKS+my_air[i].wing_man_num, mcga1 ) ;
	       BufHprint( 90, 150, wing_man_mess1[my_air_target[i].mes_air-1], 3<<5, mcga1 ) ;
	       my_air_target[i].mes_time++ ;
	    }
	    else my_air_target[i].mes_attr = 0 ;
	    break ;
	 }

	 if( my_air_target[i].mes_attr == 2 )
	 {
	    if( my_air_target[i].mes_time < 30 )
	    {
	       XMSPutImage( 20, 140, XMS_IMG_MASKS+my_air[i].wing_man_num, mcga1 ) ;
	       if( wing_man[my_air_target[i].mes_air].stage_exp == 1 ) BufHprint( 90, 150, wing_man_mess21[my_air_target[i].mes_air], 3<<5, mcga1 ) ;
	       if( wing_man[my_air_target[i].mes_air].stage_exp == 2 ) BufHprint( 90, 150, wing_man_mess22[my_air_target[i].mes_air], 3<<5, mcga1 ) ;
	       if( wing_man[my_air_target[i].mes_air].stage_exp >= 3 ) BufHprint( 90, 150, wing_man_mess23[my_air_target[i].mes_air], 3<<5, mcga1 ) ;
	       my_air_target[i].mes_time++ ;
	    }
	    else my_air_target[i].mes_attr = 0 ;
	    break ;
	 }

	 if( my_air_target[i].mes_attr == 3 )
	 {
	    if( my_air_target[i].mes_time < 30 )
	    {
	       XMSPutImage( 20, 140, XMS_IMG_MASKS+my_air[i].wing_man_num, mcga1 ) ;
	       BufHprint( 90, 150, wing_man_mess3[my_air_target[i].mes_air], 3<<5, mcga1 ) ;
	       my_air_target[i].mes_time++ ;
	    }
	    else my_air_target[i].mes_attr = 0 ;
	    break ;
	 }

	 if( my_air_target[i].mes_attr == 4 )
	 {
	    if( my_air_target[i].mes_time < 30 )
	    {
	       XMSPutImage( 20, 140, XMS_IMG_MASKS+my_air[i].wing_man_num, mcga1 ) ;
	       BufHprint( 90, 150, wing_man_mess4[my_air_target[i].mes_air], 3<<5, mcga1 ) ;
	       my_air_target[i].mes_time++ ;
	    }
	    else my_air_target[i].mes_attr = 0 ;
	    break ;
	 }
      }
   }

   //************************ Oh! Char Missile ******************************
   if( ch_mis.attr == 2 )
   {
      if( (GetTime()-mis_blink_delay) > 20L )
      {
	 if( !mis_blink_attr ) mis_blink_attr = 1 ;
	 else                  mis_blink_attr = 0 ;
	 mis_blink_delay = GetTime() ;
      }
      if( !mis_blink_attr ) XMSPutImage( 115, 40, XMS_IMG_WARNING0, mcga1 ) ;
      else                  XMSPutImage( 115, 40, XMS_IMG_WARNING1, mcga1 ) ;
   }

   //************************ Stage Clear Message Output ********************
   if( panicmode )                       //***** Panic Mode Clear Check *****
   {
      scenario_program( 2 ) ;            //***** Panic Scenario Check   *****
      if( !stage.end_view_count && !score_view_count && !my_mis.attr &&
          clear_scenario && !ch_mis.attr )
      {
         clear_scenario = 0 ;                   //***** Scenario Clear ******
         if( panicmode == 4 )                   //**** Robot Panic Stage ****
         {
	    stage.air_new[_F14] = stage.air_kill[_F14] = 1 ;
	    Load_Air_Image( "Z-ENEMY", _F14, 1 ) ;
         }
         else                                   //***** Air Panic Stage *****
         {
            panicattr++ ;                       //***** Character Plus ******
            if( panicattr == 9 ) panicattr = 0 ;
            if( panicattr == 0 )
            {
	       stage.air_new[_MF1] = stage.air_kill[_MF1] = 1 ;
	       Load_Air_Image( "MF1PIC", _MF1, 1 ) ;
            }
            if( panicattr == 1 )
            {
               stage.air_new[_F4]  = stage.air_kill[_F4]  = 1 ;
               Load_Air_Image( "F4PIC", _F4, 1 ) ;
            }
            if( panicattr == 2 )
            {
               stage.air_new[_F16] = stage.air_kill[_F16] = 1 ;
	       Load_Air_Image( "F16PIC2", _F16, 1 ) ;
            }
            if( panicattr == 3 )
            {
	       stage.air_new[_F14] = stage.air_kill[_F14] = 1 ;
	       Load_Air_Image( "F14PIC", _F14, 1 ) ;
            }
            if( panicattr == 4 )
            {
	       stage.air_new[_F15] = stage.air_kill[_F15] = 1 ;
	       Load_Air_Image( "F15PIC", _F15, 1 ) ;
            }
            if( panicattr == 5 )
            {
	       stage.air_new[_F18] = stage.air_kill[_F18] = 1 ;
	       Load_Air_Image( "F18PIC", _F18, 1 ) ;
            }
            if( panicattr == 6 )
            {
	       stage.air_new[_FSX] = stage.air_kill[_FSX] = 1 ;
	       Load_Air_Image( "FSXPIC", _FSX, 1 ) ;
            }
            if( panicattr == 7 )
            {
	       stage.air_new[_F22] = stage.air_kill[_F22] = 1 ;
	       Load_Air_Image( "F22PIC", _F22, 1 ) ;
            }
            if( panicattr == 8 )
            {
	       stage.air_new[_BF] = stage.air_kill[_BF] = 1 ;
	       Load_Air_Image( "SOLGAE", _BF, 1 ) ;
            }
            panic_air_num++ ;
            if( panic_air_num == MAXCHAIR ) panic_air_num = 0 ;
            new_char( panic_air_num, random(4) ) ;
         }
      }
   }
   else                                  //**** Orignal Mode Clear Check ****
   {
      if( !stage.end_view_count && !score_view_count && !my_mis.attr &&
          clear_scenario && !ch_mis.attr )
      {
	 ch_mis.attr = 0 ;
	 for( i=0; i<MAXFLAG; i++ ) ch_mis_flag.attr[i] = 0 ;
	 AdLib_End() ;
	 total_score += (my_air[0].mis_count*50) + stage_score ;
	 AdLib_Start( "bf-clear" ) ;
	 stage.end_view_count = 1 ;
	 stage.view_time = GetTime() ;
         stage_mask_pos_x = 320 ;
         stage_mask_pos_y = 140 ;
	 eye_ani.attr = 1 ;                        //**** Mask Eye Initialize
	 eye_ani.count = 3 ;
	 eye_ani.ani_time = GetTime() ;
      }
   }

   if( stage.end_view_count == 1 )
   {
      if( (GetTime()-stage.view_time) < 200L )
      {
         if( stage_mask_pos_x > 245 )
         {
            stage_mask_pos_x -= 16  ;
            if( stage_mask_pos_x < 245 ) stage_mask_pos_x = 245 ;
         }
	 XMSPutImage( stage_mask_pos_x,    140,    XMS_IMG_MASK, mcga1 ) ;
	 XMSPutImage( stage_mask_pos_x+11, 140+25, XMS_IMG_EYES+3, mcga1 ) ;
      }
      else
      {
         if( (GetTime()-stage.view_time) < 500L )
         {
	    XMSPutImage( 245, 140, XMS_IMG_MASK, mcga1 ) ;
	    if( eye_ani.attr )
	    {
	       if( (GetTime()-eye_ani.ani_time) > 15L )
	       {
	          eye_ani.ani_time = GetTime() ;
	          if( eye_ani.count != 0 ) eye_ani.count-- ;
	          else                     eye_ani.attr = 0 ;
	          XMSPutImage( 245+11, 140+25, XMS_IMG_EYES+eye_ani.count, mcga1 ) ;
	       }
	       else
	          XMSPutImage( 245+11, 140+25, XMS_IMG_EYES+eye_ani.count, mcga1 ) ;
	    }
         }
      }
      if( (GetTime()-stage.view_time) < 500L )
      {
         if( stage_title_pos_x < 80 ) stage_title_pos_x += 16 ;
         if( (GetTime()-stage.view_time) > 400L ) stage_title_pos_x += 16 ;
	 BufMHprint( stage_title_pos_x, stage_title_pos_y, "-* ¸b ¸å ¬÷ ‰· ! *-", 3<<4, mcga1 ) ;
      }
      else
      {
	 stage.end_view_count = 2 ;
	 stage.view_time = GetTime() ;
         stage_title_pos_x = 70 ;             //**** Stage Title Data
         stage_title_pos_y = -140 ;
         rader_scale_data_x = 46 ;            //**** Rader Data Initialize
         rader_scale_data_y = 35 ;
         rader_scale_mode = 3 ;
      }
   }

   if( stage.end_view_count == 2 )
   {
      if( (GetTime()-stage.view_time) < 550L )
      {
         if( stage_title_pos_y < 30 ) stage_title_pos_y += 8 ;
	 BufMHprint( stage_title_pos_x, stage_title_pos_y, "-*- ·±¢ µÅ® ¸ñ® -*-", 6<<4, mcga1 ) ;
	 sprintf( st, "·±  ¢  ¸ñ  ® : %d ¸ñ", stage_score ) ;
	 BufHprint( stage_title_pos_x+10, stage_title_pos_y+50, st, 204, mcga1 ) ;
	 sprintf( st, "£¡¬a·©  ¥¡á¯a : %d ¸ñ", my_air[0].mis_count*50 ) ;
	 BufHprint( stage_title_pos_x+10, stage_title_pos_y+80, st, 204, mcga1 ) ;
	 sprintf( st, "Á· ¸ñ ® : %d ¸ñ", total_score ) ;
	 BufMHprint( stage_title_pos_x+10, stage_title_pos_y+120, st, 3<<4, mcga1 ) ;
         if( stage_mask_pos_y < 200 ) stage_mask_pos_y += 3 ;
	 XMSPutImage( stage_mask_pos_x,    stage_mask_pos_y,    XMS_IMG_MASK, mcga1 ) ;
      }
   }

   //************************** Continue Message Output **********************
   if( score_view_count == 1 )
   {
      if( stage_title_pos_x < 90 ) stage_title_pos_x += 16 ;
      else                         stage_title_pos_x  = 90 ;
      BufHprint( stage_title_pos_x, stage_title_pos_y, "-*- ‰A ·± µ¡ ¤á -*-", 204, mcga1 ) ;
   }

   //************************** Screen Output ********************************
   time_int_push = 1 ;
   MoveData( mcga1, mcga0, 64000 ) ;
   time_int_push = 0 ;
}

void score_board( void )
{
   long mes_time ;
   int x1, x2 ;

   AdLib_End() ;                                     // AdLib Music Song Stop
   ViewIMG( "SELECT", mcga1 ) ;

   BufHprint( 76, 21, "-*- ÎŸe  Á·¸ñ® -*-", 108, mcga1 ) ;
   BufHprint( 75, 20, "-*- ÎŸe  Á·¸ñ® -*-", 103, mcga1 ) ;
   sprintf( st, "ÎŸe  Á· ¸ñ® : %d ¸ñ", total_score ) ;
   BufHprint( 66, 171, st, (5<<4)+15, mcga1 ) ;
   BufMHprint( 65, 170, st, 3<<4, mcga1 ) ;

   MoveData( mcga1, mcga2, 64000 ) ;
   MoveData( mcga1, mcga0, 64000 ) ;

   AdLib_Start( "bfdemo3" ) ;
   white_palette() ;                      // Set Slow White Palette

   x1 = -96 ;
   x2 = 326 ;
   while( 1 )
   {
      mes_time = GetTime() ;
      MoveData( mcga2+15000, mcga1+15000, 34000 ) ;
      sprintf( st,       "F16  : %d ”", total_kill_air[0] ) ;
      BufHprint( x1+1, 61+(0<<4), st, 214, mcga1 ) ;
      BufHprint( x1,   60+(0<<4), st, 204, mcga1 ) ;

      sprintf( st,       "F14  : %d ”", total_kill_air[1] ) ;
      BufHprint( x2+1, 61+(0<<4), st, 214, mcga1 ) ;
      BufHprint( x2,   60+(0<<4), st, 204, mcga1 ) ;

      sprintf( st,       "F15  : %d ”", total_kill_air[2] ) ;
      BufHprint( x1+1, 61+(1<<4), st, 214, mcga1 ) ;
      BufHprint( x1,   60+(1<<4), st, 204, mcga1 ) ;

      sprintf( st,       "F18  : %d ”", total_kill_air[3] ) ;
      BufHprint( x2+1, 61+(1<<4), st, 214, mcga1 ) ;
      BufHprint( x2,   60+(1<<4), st, 204, mcga1 ) ;

      sprintf( st,       "A10  : %d ”", total_kill_air[4] ) ;
      BufHprint( x1+1, 61+(2<<4), st, 214, mcga1 ) ;
      BufHprint( x1,   60+(2<<4), st, 204, mcga1 ) ;

      sprintf( st,       "F117 : %d ”", total_kill_air[5] ) ;
      BufHprint( x2+1, 61+(2<<4), st, 214, mcga1 ) ;
      BufHprint( x2,   60+(2<<4), st, 204, mcga1 ) ;

      sprintf( st,       "FSX  : %d ”", total_kill_air[6] ) ;
      BufHprint( x1+1, 61+(3<<4), st, 214, mcga1 ) ;
      BufHprint( x1,   60+(3<<4), st, 204, mcga1 ) ;

      sprintf( st,       "F22  : %d ”", total_kill_air[7] ) ;
      BufHprint( x2+1, 61+(3<<4), st, 214, mcga1 ) ;
      BufHprint( x2,   60+(3<<4), st, 204, mcga1 ) ;

      sprintf( st,       "MF1  : %d ”", total_kill_air[8] ) ;
      BufHprint( x1+1, 61+(4<<4), st, 214, mcga1 ) ;
      BufHprint( x1,   60+(4<<4), st, 204, mcga1 ) ;

      sprintf( st,       "F4   : %d ”", total_kill_air[9] ) ;
      BufHprint( x2+1, 61+(4<<4), st, 214, mcga1 ) ;
      BufHprint( x2,   60+(4<<4), st, 204, mcga1 ) ;

      MoveData( mcga1+15000, mcga0+15000, 34000 ) ;
      while( GetTime() < mes_time+3L ) ;        // Delay Speed
      if( x1 > 55  ) break ;
      if( x1 < 55  ) x1 += 8 ;
      if( x2 > 175 ) x2 -= 8 ;
   }

   key_clear() ;
   Getkey(1) ;
   black_palette( 1 ) ;                   // Set Slow Black Palette
   black_palette( 0 ) ;                   // All Black Palette
}

void chk_turbo( void )
{
   int i ;

   for( i=0; i<MAXMYAIR; i++ )
   {
      if( my_turbo[i].attr == 1 )
      {
	 my_turbo[i].attr = 2 ;
	 my_turbo[i].count = 40 ;
      }

      if( my_turbo[i].attr == 2 )
      {
	 if( !my_turbo[i].count ) my_turbo[i].attr = 3 ;
	 else                     my_turbo[i].count-- ;
      }

      if( my_turbo[i].attr == 3 )
      {
	 my_turbo[i].attr = 0 ;
	 my_turbo[i].count = 100 ;
      }

      if( !my_turbo[i].attr && my_turbo[i].count ) my_turbo[i].count-- ;
   }
}

//********************************* START ************************************

void start( void )
{
   long speed_loop = 0 ;
   int i, pal_ani_color=0 ;

   key_clear() ;                                  // Key Buffer Clear
   speed_loop = GetTime() ;                       // Init Loop Time

   SetInt08() ;                                   // RGB Time Interrupt ON
   SetInt09() ;                                   // Set Multi Key
   time_int_push = 1 ;                            // Palette Interrupt Stop

   set_stage() ;                                  // Stage Seting

   time_int_push = 0 ;                            // Palette Interrupt Start
//   stage2_1() ;
   SetRgbPaletteSnow( (unsigned char *)Pal ) ;
//   palette_view();

   while( exit_code )
   {
//      if( JoyStickOnOff )
//      {
//	 JoyStickPos( &joy_x, &joy_y ) ;
//	 JoyStickButton( &joy_b ) ;
//	 joy_x /= 10 ;
//	 joy_y /= 10 ;
//	 if( (joy_x > 3 ) && (joy_x < 10) && (joy_y < 3  ) ) keycode[M_UP      ] = 1 ;
//	 if( (joy_x > 3 ) && (joy_x < 10) && (joy_y > 10 ) ) keycode[M_DOWN    ] = 1 ;
//	 if( (joy_x < 3 ) && (joy_y > 3 ) && (joy_y < 10 ) ) keycode[M_LEFT    ] = 1 ;
//	 if( (joy_x > 10) && (joy_y > 3 ) && (joy_y < 10 ) ) keycode[M_RIGHT   ] = 1 ;
//	 if( (joy_x < 3 ) && (joy_y < 3 )                  ) keycode[M_HOME    ] = 1 ;
//	 if( (joy_x < 3 ) && (joy_y > 10)                  ) keycode[M_END     ] = 1 ;
//	 if( (joy_x > 10) && (joy_y < 3 )                  ) keycode[M_PAGEUP  ] = 1 ;
//	 if( (joy_x > 10) && (joy_y > 10)                  ) keycode[M_PAGEDOWN] = 1 ;
//	 switch( joy_b )
//	 {
//	    case 1 : keycode[M_SPACEBAR] = 1 ; break ;
//	    case 2 : keycode[M_CTRL    ] = 1 ; break ;
//	    case 3 : keycode[M_ALT     ] = 1 ; break ;
//	 }
//      }

      if( keycode[M_UP]       || keycode[M_DOWN]   || keycode[M_LEFT]   || keycode[M_RIGHT]     ||
	  keycode[M_HOME]     || keycode[M_END]    || keycode[M_PAGEUP] || keycode[M_PAGEDOWN]  ||
	  keycode[M_SPACEBAR] || keycode[M_CTRL]   || keycode[M_ESC]    || keycode[M_ALT]       ||
	  keycode[M_LSHIFT]   || keycode[M_RSHIFT] || keycode[M_ENTER] )
      {
	 if( !my_air[0].kill_mode&&(start_attr==50)&&(stage.end_view_count!=2) )
	 {
	    if( (keycode[M_UP]       || keycode[M_DOWN] || keycode[M_LEFT]   || keycode[M_RIGHT]     ||
		 keycode[M_HOME]     || keycode[M_END]  || keycode[M_PAGEUP] || keycode[M_PAGEDOWN])
		&& automatic )
	    {
	       if( !my_air[0].loop )
	       {
		  if( keycode[M_UP] && keycode[M_DOWN] && !my_air[0].roll_attr && ((my_air[0].fly_mode==0)||(my_air[0].fly_mode==8)) && (my_air[0].air==0) )
		  {                              // Up & Down Key ( ROLL!!! )
		     my_air[0].roll_attr = 1 ;   // Solgae Roll Start !!!
		     my_air[0].roll_count = 0 ;  // Solgae Roll Count
		     my_air[0].roll_delay = 0 ;  // Roll & Roll Delay Count
		     if( my_air[0].fly_attr == i )
		     {
			if( my_air[0].fly_count ) my_air[0].fly_count-- ;
		     }
		     else
		     {
			my_air[0].fly_count++ ;
		     }
		  }
		  if( !my_air[0].roll_attr )     // Solgae Roll !!!!!
		  {
		     if( keycode[M_UP       ] ) i=lotate_air( &my_air[0].fly_mode, M_UP        ) ; // Up Key
		     if( keycode[M_DOWN     ] ) i=lotate_air( &my_air[0].fly_mode, M_DOWN      ) ; // Down Key
		     if( keycode[M_LEFT     ] ) i=lotate_air( &my_air[0].fly_mode, M_LEFT      ) ; // Left Key
		     if( keycode[M_RIGHT    ] ) i=lotate_air( &my_air[0].fly_mode, M_RIGHT     ) ; // Right Key
		     if( keycode[M_HOME     ] ) i=lotate_air( &my_air[0].fly_mode, M_HOME      ) ; // Home Key
		     if( keycode[M_END      ] ) i=lotate_air( &my_air[0].fly_mode, M_END       ) ; // End Key
		     if( keycode[M_PAGEUP   ] ) i=lotate_air( &my_air[0].fly_mode, M_PAGEUP    ) ; // Page Up Key
		     if( keycode[M_PAGEDOWN ] ) i=lotate_air( &my_air[0].fly_mode, M_PAGEDOWN  ) ; // Page Down Key
		     if( my_air[0].fly_attr == i )
		     {
			if( my_air[0].fly_count ) my_air[0].fly_count-- ;
		     }
		     else
		     {
			my_air[0].fly_count++ ;
		     }
		  }

		  SetFlyPalette( my_air[0].fly_count ) ;
		  my_air[0].loop = air_static_loop[my_air[0].air] ;

		  if( stage.view_count )
		     for( i=1; i<MAXMYAIR; i++ ) my_air[i].fly_mode = my_air[0].fly_mode ;
	       }
	       else
		  my_air[0].loop-- ;
	    }
	    else
	    {
	       if( my_air[0].fly_count ) my_air[0].fly_count-- ;
	       SetFlyPalette( my_air[0].fly_count ) ;
	    }

	    if( keycode[M_SPACEBAR] && automatic )
	    {
	       if( !my_air[0].kill )
	       {
		  if( my_gun[0].weapon == 1 )     //**** Gun Weapon Start
		  {
		     if( !my_gun[0].count )
		     {
			my_gun[0].count = 1 ;
			sayPCM( PCM_MULT_GUN, 1 ) ;
		     }
		  }
                                                  //**** Laser Weapon Start
		  if( (my_gun[0].weapon == 2) || (my_gun[0].weapon == 3) )
		  {
		     if( my_gun[0].fly_mode[0] )
		     {
			my_gun[0].fly_mode[1] = 1 ;
			my_gun[0].fly_mode[0] = 0 ;
			sayPCM( PCM_LASER, 1 ) ;
		     }
		     else my_gun[0].fly_mode[0]++ ;
		  }
	       }
	    }
	    if( keycode[M_ALT  ] && automatic )
	    {
	       if( !my_turbo[0].attr && !my_turbo[0].count )
	       {
		  my_turbo[0].attr = 1 ;
		  sayPCM( PCM_TURBO, 1 ) ;
	       }
	    }
	    if( keycode[M_CTRL ] && automatic )
	    {
	       if( my_laser[0].attr )
	       {
		  if( !my_laser[0].count )
		  {
		     my_laser[0].count = 30 ;
		     sayPCM( PCM_LASER_BIM, 1 ) ;
		  }
	       }
	       else
	       {
		  if( !my_mis.attr && my_air[0].mis_count )
		  {
		     keycode[M_CTRL] = 0 ;
		     my_mis.attr = 1 ;                    // My Missile Start
		     my_air[0].mis_count-- ;              // My Missile --
		  }
	       }
	    }
	    if( keycode[M_LSHIFT] && automatic )
	    {
	       keycode[M_LSHIFT] = 0 ;
	       if( rader_attr ) rader_attr = 0 ;
	       else             rader_attr = 1 ;
	    }
	    if( keycode[M_RSHIFT] && automatic )
	    {
	       keycode[M_RSHIFT] = 0 ;
	       if( !my_flare[0].attr && my_air[0].flare )
	       {
		  my_flare[0].attr = 1 ;
		  my_air[0].flare-- ;
	       }
	    }
	    if( keycode[M_ENTER] && automatic )
	    {
	       keycode[M_ENTER] = 0 ;
	       if( !my_mis.attr && my_mis_target.attr ) // Turn Target
	       {
		  for( i=0; i<MAXCHAIR; i++ )
		  {
		     if( !ch_air[i].kill ) break ;
		  }
		  if( i != MAXCHAIR )
		  {
		     i = my_mis_target.target ;
		     while( 1 )
		     {
			if( i == (MAXCHAIR-1) ) i = 0 ;
			else                    i++   ;
			if( !ch_air[i].kill )
			{
			   my_mis_target.target = i ;
			   my_mis_target.x = ch_air[i].x ;
			   my_mis_target.y = ch_air[i].y ;
			   break ;
			}
		     }
		  }
	       }
	    }
	    if( keycode[M_ESC] && (start_attr==50) && !stage.view_count )
               my_body_attack( 0, 50, random(2)+1 ) ;  // Exit
	 }
      }
      else
      {
	 if( my_air[0].fly_count ) my_air[0].fly_count-- ;
	 SetFlyPalette( my_air[0].fly_count ) ;
      }
      if( keycode[M_INSERT] )
      {
	 switch( getch() )
	 {
	    case 'm' : { printf(" %lu bytes", farcoreleft() ); getch(); } break ; // Hip Free Memory View
	    case 'c' : clear_scenario = 1 ;          //****** Clear Scenario
                       break ;
	    case 'p' : { palette_view(); } break ;
//	    case '1' : Buster_XY_Edit_Mode() ; break ;
//	    case '2' : Rader_XY_Edit_Mode() ; break ;
	 }
      }
      chk_turbo() ;

      if( (keycode[M_SPACEBAR] || keycode[M_ESC]) && (score_view_count == 1) )
      {
	 if( (GetTime()-score_view_time) > 500L )
	 {
	    exit_code = 0 ;
	    i = ON ;
	 }
      }

      //***************** Z F... Zeiper Mode ****************
      if( keycode[M_Z] && keycode[M_F] )
      {
         keycode[M_Z] = 0 ;
         switch( panicmode )
         {
            case 1 :
               panicmode++ ;                          //**** Zeiper Mode ****
	       Load_Air_Image( "ZEIPER",  my_air[0].air, 0 ) ;
               for( i=0; i<MAXCHAIR; i++ )
               {
                  if( !ch_air[i].kill )
	             Load_Air_Image( "Z-ENEMY", ch_air[i].air, 1 ) ;
               }
               my_gun[0].weapon = 1 ;                 //***  Gun Weapon  ****
               break ;
            case 2 :
               panicmode++ ;
               my_gun[0].weapon = 2 ;                 //*** Laser Weapon  ***
               break ;
            case 3 :
               panicmode++ ;                          //** All Robot Stage **
               break ;
         }
      }

      //************************** POWER Key... *****************************
      if( keycode[M_B] && keycode[M_R] )
      {
         my_gun[0].weapon = 3 ;                   //*** Automatic Power Laser
      }

      if( my_air[0].kill && !score_view_count )
      {                                           // Blue Falcon Kill
	 total_score += stage_score ;
	 score_view_count = 1 ;
	 score_view_time = GetTime() ;
      }

      if( start_attr==50 )
      {
         move_char() ;                            // Character Move
         move_gun() ;                             // Move Air Gun
         move_mis() ;                             // Move Missile
      }
      move_bk() ;                                 // Back Image Move

      output_sprite() ;                           // Image OutPut

      if( !exit_code ) break ;                    // Game Save & Over

      if( !CheckPlay() )
      {
         if( !panicmode ) sprintf( st, "bf-bgm%d", stage.number ) ;
         else             sprintf( st, "%s", "bf-panic" ) ;
	 AdLib_Start( st ) ;
      }
//      if( (getVoiceRemainSample()<2000) && !my_air[0].kill && (VoiceHardWare!=PC_SPEAKER) &&
//         (start_attr==50) ) sayPCM( PCM_BURSTER2, 1 ) ;

      SetBFPalette( 0 ) ;                         // Set Orignal Palette

      while( GetTime() < speed_loop+5L ) ;        // Delay Speed
      speed_loop = GetTime() ;

      if( pal_ani_color == 2 ) SetPalette( 254, 63, 0, 0 ) ;
      if( pal_ani_color == 4 ) SetPalette( 254, 63, 63, 63 ) ;
      pal_ani_color++ ;
      if( pal_ani_color == 5 ) pal_ani_color = 0 ;
   }

   black_palette( 1 ) ;                           // Set Slow Black Palette
   black_palette( 0 ) ;                           // All Black Palette

   if( (stage.number == 15) && clear_scenario )   //**** Ending Demo
      ending1();

   score_board() ;                                // View Score Board

   panic_score_write( total_score ) ;             // Panic Mode Score

   if( (stage.number != 15) && !panicmode )
      save_continue_data() ;                      // Save Continue Data

   AdLib_End() ;                                  // End AdLib Song
   EndInt08() ;                                   // RGB Time Interrupt OFF
   EndInt09() ;                                   // Close Multi Key
}
/*
void JoystickTest( void )
{
   int joy_x, joy_y, joy_b ;

   clrscr() ;
   while( !kbhit() )
   {
      gotoxy( joy_x/3, joy_y/3 ) ;
      printf( " " ) ;
      JoyStickPos( &joy_x, &joy_y ) ;
      JoyStickButton( &joy_b ) ;
      gotoxy( 1, 1 ) ; printf( "x=%3d, y=%3d, b=%3d", joy_x/10, joy_y/10, joy_b ) ;
      gotoxy( joy_x/3, joy_y/3 ) ;
      printf( "*" ) ;
      if( joy_b )
      {
	 if( (joy_b == 1) || (joy_b == 3) )
	 {
	    gotoxy( 70, 23 ) ;
	    printf( "AA" ) ;
	 }
	 if( (joy_b == 2) || (joy_b == 3) )
	 {
	    gotoxy( 76, 23 ) ;
	    printf( "BB" ) ;
	 }
      }
      else
      {
	 gotoxy( 70, 23 ) ;
	 printf( "            " ) ;
      }
   }
   getch() ;
}
        */
void main( int argc, char *argv[] )
{
   int bf_key, i ;

//   printf( "\n Joystick ? (Y/N)" ) ;
//   key = Getkey( 1 ) ;
//   if( key == 'y' ) { JoyStickOnOff = 1 ; JoystickTest() ; }

   if( argc < 3 )
   {
      XMS_Free_Memory_Check() ;          // XMS Free Momory Check

      while( 1 )
      {
         i = BfConfig( 0, 1 ) ;
         printf( "\n\n Continue Game ? [Y/N]" ) ;
         while( 1 )
         {
	    bf_key = Getkey( 1 ) ;
	    if( (bf_key == 'Y') || (bf_key == 'y') ) break ;
	    if( (bf_key == 'N') || (bf_key == 'n') ) break ;
         }
         if( (bf_key == 'Y') || (bf_key == 'y') )
         {
            if( i == 2 ) execl( "bf.dem", "bf.dem", "START", NULL) ;
            SetVga() ;                    // VGA 13H Mode Seting
            break ;
         }
         if( (bf_key == 'N') || (bf_key == 'n') )
         {
	    i = BfConfig( 1, 1 ) ;
         }
      }
   }
   else
   {
      init_xms() ;                          // Initialize XMS Memory
   }

   randomize() ;                            // Randomize Initialze
   i = BfConfig( 0, 0 ) ;                   // Config Data Read And Set
   main_data_init() ;                       // Main Datas Initialze
   stage_data_init() ;                      // Game Stage Memory Initialze
   read_image() ;                           // Blue Falcon Image Read
   if( CheckPlay() ) AdLib_End() ;          // Stop AdLib Music
   InitMouse() ;                            // Mouse Time Interrupt ON

//   printf(" %lu bytes", farcoreleft() ); getch(); // Hip Free Memory View

   do
   {
      bf_key = bf_title() ;
      if( bf_key == 3 ) break ;
      if( bf_key )
      {
         bf_init_data() ;
         if( bf_key == 2 ) load_continue_data() ;    //**** Continue Game
         if( argc == 2 )
	 {
	    if( !strcmpi( argv[1], "/passcode" ) )
	    {
	       Hprint( 0,  0, "- ˆa‰¡¯¼·e ¯aÉA·¡»¡Ÿi ·³bÐa­A¶a !!! -", 8<<4 ) ;
	       Hprint( 0, 20, "       ( 1~15 )", 8<<4 ) ;
	       printf(  "\n\n\n  ->" ) ;
    	       scanf( "%d", &stage.number ) ;
	       stage.number-- ;

	       Hprint( 0, 60, "- ´á˜å ‹¡¹··a¡ Ða¯¡‰V¯s“¡Œa ? -", 8<<4 ) ;
	       Hprint( 0, 80, "  ( 1-BF, 2-F16, 3-F14, 4-F15, 5-F18 )", 8<<4 ) ;
	       Hprint( 0, 100,"  ( 6-A10, 7-F117, 8-FSX, 9-F22, 10-MF1, 11-F4 )", 8<<4 ) ;
	       bf_key = Getkey( 1 ) ;
	       my_air[0].air = 0 ;
	       if( bf_key == '2' ) my_air[0].air = 1 ;
	       if( bf_key == '3' ) my_air[0].air = 2 ;
	       if( bf_key == '4' ) my_air[0].air = 3 ;
	       if( bf_key == '5' ) my_air[0].air = 4 ;
	       if( bf_key == '6' ) my_air[0].air = 5 ;
	       if( bf_key == '7' ) my_air[0].air = 6 ;
	       if( bf_key == '8' ) my_air[0].air = 7 ;
	       if( bf_key == '9' ) my_air[0].air = 8 ;
	       if( bf_key == '0' ) my_air[0].air = 9 ;
	       if( bf_key == '-' ) my_air[0].air = 10 ;

	       Hprint( 0, 140, "- ¸a•··a¡ ÎŸe Ÿi ¹¡¸÷ÐiŒa¶a ? -", 8<<4 ) ;
	       Hprint( 0, 160, "  ( 0 ¸a•· ¹¡¸÷, 1 ‰A·±¸a )       ", 8<<4 ) ;
	       bf_key = Getkey( 1 ) ;
	       if( bf_key == '0' ) automatic = 0 ;
	       else                automatic = 1 ;
	    }
	 }
	 else
	    my_air[0].air = 0 ;
         if( panicmode )
         {
            panic_air_select() ;             //**** Panic Mode Air Select
         }
	 start() ;                           //**** Start Game
      }
   } while( bf_key ) ;

//   printf(" %lu bytes", farcoreleft() ); getch(); // Hip Free Memory View
   if( ((stage.number!=15) || !clear_scenario) && (bf_key!=3) )
   {
      ViewIMG( "gameover", mcga1 ) ;         // Game Over Demo
      black_palette( 0 ) ;                   // Set All Black Palette
      MoveData( mcga1, mcga0, 64000 ) ;
      AdLib_Start( "gameover" ) ;
      white_palette() ;
      waitmSec( 5000 ) ;
      key_clear() ;
      while( CheckPlay() && !kbhit() ) ;
      if( kbhit() ) Getkey( 1 ) ;
      black_palette( 1 ) ;                   // Set Slow Black Palette
      AdLib_End() ;
   }

   EndVga() ;
   endVoice() ;
   bf_data_close() ;                          // Stage Memory Free
   main_data_close() ;                        // Main Memory Free
   key_clear() ;
   CloseMouse() ;                             // Mouse Time Interrupt OFF

   if( bf_key == 3 ) execl( "bf.mmo", "bf.mmo", NULL) ;
   else              printf( "\n    But...the War would not be over...\n" ) ;
}
