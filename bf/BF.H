
#include <sys\stat.h>
#include <process.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <fcntl.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <mem.h>
#include <io.h>

#include "game_lib.h"

#define _BF   0
#define _F16  1
#define _F14  2
#define _F15  3
#define _F18  4
#define _A10  5
#define _F117 6
#define _FSX  7
#define _F22  8
#define _MF1  9
#define _F4   10

#define MAXMYAIR 5
#define MAXCHAIR 5

#define MEMXMAX  1000
#define MEMYMAX  1000
#define MEMXMIN -1000
#define MEMYMIN -1000

#define MAXGUN   6
#define MAXFLAG  7

struct stage_data {
   char number ;
   char name[30] ;
   char view_count ;
   char end_view_count ;
   long view_time ;
   char air_kill[11] ;
   char air_new[11] ;
   char clear ;                // 0 == Not Clear, 1 == Stage Clear
} ;

struct turbo_data {
   unsigned char attr ;
   unsigned char count ;
} ;

struct my_air_data {
   int  x ;
   int  y ;
   char body ;               // My Air Health
   char fly_mode ;           // Flying Mode 0-15
   char mis_count ;          // My Air(Blue Falcon) Missile Count
   char flare ;              // Flash Count
   char kill_mode ;
   char kill_attr ;
   char kill_count ;
   char kill ;               // 0 = Lefe, 1 = Kill
   char air  ;               // Air Attrib (0=bf,1=f16,2=f14,3=f15,...)
   char loop ;               // Lotate Loop
   char fly_attr ;           // 0 = Not Lotate, 1 = LEFT, 2 = RIGHT
   char fly_count ;          // Char Air Lotate Count
   char wing_man_num ;       // Wing Man Number ( 0 ~ 9 )
   char ai        ;          // 0 = Low, ~ 4 = HIGH
   char roll_attr ;          // 0=NotRoll, 1=Roll
   char roll_count ;         // Roll Image Count
   char roll_delay ;         // Roll & Roll Delay Count
} ;

struct my_air_target_data {
   char attr ;               // 0 = (Target Not Get), 1 = (Get Target)
   char target ;             // Character Target Value
   char mes_attr ;           // 0=No, 1=Start, 2=Oh!, 3=My Kill
   char mes_air  ;           // Target Air Name
   char mes_time ;           // Delay Time Count Loop
} ;

struct my_gun_data {
   char weapon ;             // 1 = Gun... 2 = Laser... 3 = Auto Laser
   int  x[MAXGUN] ;
   int  y[MAXGUN] ;
   int  attr[MAXGUN] ;
   char fly_mode[MAXGUN] ;
   char count ;
} ;

struct my_mis_target_data {
   int x ;
   int y ;
   char attr ;        //**** attr=0(Air Not Found) attr=1(Static Target) ***
   char target ;
} ;

struct my_mis_data {
   char weapon ;
   int  x ;
   int  y ;
   int  attr ;
   char fly_mode ;
   char count ;
   char loop ;
   unsigned char lotate_count ;          // 150 Count Loop
   char lotate_fly_mode ;
} ;

struct my_laser_data {
   char attr ;
   char count ;
} ;

struct wing_man_data {
   char kill ;          // 0 = Life, 1 = Kill
   char stage_exp ;     // Wing Man Stage Exp ( = Stage Kill Air )
   char total_exp ;     // Wing Man Total Exp ( = Total Kill Air )
   char level ;         // Wing Man Level ( = Ai )
   char sungjang ;      // Sung Jang ( % )
   char name[7] ;       // Wing Man Name
} ;

struct buster_data {
   char x[2][11][2][16] ;   // [ground][Air_number][Buster_Size][air_fly]
   char y[2][11][2][16] ;   // 2 * 11 * 2 * 16 = 704 Byte
   char attr ;              // 0 = Sky, 1 = Ground
   char size ;              // 0 = Small, 1 = Large
   char img_num[11] ;       // (0=one buster) (1=two buster) (2=two ground buster)
} ;

extern void story_score_write( unsigned int ) ;   // Story Score Write Board
extern void panic_score_write( unsigned int ) ;   // Panic Score Write Board

extern struct stage_data stage ;
extern struct turbo_data my_turbo[MAXMYAIR] ;
extern struct my_air_data my_air[MAXMYAIR] ;
extern struct my_air_target_data my_air_target[MAXMYAIR] ;
extern struct my_gun_data my_gun[MAXMYAIR] ;
extern struct my_mis_target_data my_mis_target ;
extern struct my_mis_data my_mis ;
extern struct my_laser_data my_laser[MAXMYAIR] ;
extern struct wing_man_data wing_man[10] ;          // 10 Wing Man, ( Number 0 = Kim Gun il, Special )
extern struct buster_data buster ;

extern unsigned char far *mcga0 ;
extern unsigned char far *mcga1 ;
extern unsigned char far *mcga2 ;
extern unsigned char far *mcga3 ;

extern int  XMS_hand ;
extern int  XMS_air  ;
extern int  XMS_back ;
extern int  XMS_pcm   ;
extern int  XMS_image ;
extern char XMS_air_select[11] ;
extern long XMS_image_addr[80] ;
extern unsigned int XMS_image_size[80] ;

extern unsigned char mouse_shape[454] ;    // Mouse Image Data
extern unsigned char Pal[256][3] ;         // Vga Palette Data
extern char time_int_push ;                // Palette Animation ( 0=Play, 1=Stop )
extern char st[80] ;                       // String
extern char panicmode ;                    // Blue Falcon Panic Mode (==1)
extern char clear_scenario ;               // 0 = Not Clear. 1 = Stage Clear
extern unsigned int  top_score_board[7] ;    // Top Score 1 ~ 7
extern unsigned char top_score_name[7][35] ; // Top Score Player Names 1 ~ 7
extern unsigned int  panic_score_board[7] ;    // Panic Score 1 ~ 7
extern unsigned char panic_score_name[7][35] ; // Panic Player Names 1 ~ 7

extern char far *air_image_buff ;
extern char far *buster_image[3][2][16] ;  // (0=one buster) (1=two buster) (2=two ground buster)

extern char bk_sx[2][16] ;
extern char bk_sy[2][16] ;

extern unsigned char mis_static_time[8] ;
extern unsigned char mis_static_body[8] ;
extern unsigned char mis_static_loop[8] ;

extern char select_man[9] ;    // Select Wing Man Number 0 ~ 8 ( 9 )
extern char select_attr[9] ;   // Select Wing Man attr ( 0=No, 1=Yes )
