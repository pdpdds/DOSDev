#pragma inline
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>
#include <time.h>
#include <mem.h>

#define ON          1
#define OFF         0
#define TRUE        1
#define FALSE       0
#define DBLCLK      6
#define MAX       100
#define MCLK mbuf[mh].clk
#define MBUT mbuf[mh].but
#define DOUBLE_CLK  8
#define RIGHT_CLK   4
#define LEFT_CLK    2

int mouse_left, mouse_top, mouse_state;
int mouse_exist = FALSE ;
int ExecFlag = 0 ;
int mouse_x = 160 ;
int mouse_y = 100 ;
int mouse_right  = 320 ;
int mouse_bottom = 200 ;

unsigned char mouse_shape[454] ;

struct mouse_buf {
  clock_t clk;
  int but;
};

struct mouse_buf pbuf={0,0};
struct mouse_buf mbuf[MAX];
int mh=0,mt=0;

extern unsigned char far *mcga0 ;
extern unsigned char far *mcga1 ;

int ExecOk()
{
   asm mov ax,1;
   asm xchg ax,ExecFlag;
   asm cmp ax,1;
   asm jne ok;
   return 0;
 ok:
   return 1;
}

int ExecEnd()
{
   ExecFlag=0;
}

void far event_handler(void)
{
  int aax,bbx;
  asm push ax
  asm push ds
  bbx=_BX;

  asm mov ax,DGROUP
  asm mov ds,ax
  if( mt==MAX-1 ) mt=0;
  MCLK=clock();
  MBUT=bbx;
  mt++;

  _BX=bbx;
  asm pop ds
  asm pop ax
}

void MouseEvent( unsigned mask, void far (*handler)() )
{
  struct SREGS seg ;
  union REGS mr ;

  mr.x.ax = 12 ;
  mr.x.cx = mask ;
  seg.es  = FP_SEG( handler ) ;
  mr.x.dx = FP_OFF( handler ) ;
  int86x( 0x33, &mr, &mr, &seg ) ;
}

void MouseReset( void )
{
  asm mov ax, 0
  asm int 33h
}

int MouseTest( void )
{
  asm mov ax, 0
  asm int 33h
  return _AX ;
}

static void interrupt (*oldint8)(void);
void interrupt newint8(void)
{
   int x=0, y=0 ;
/*
   asm 	mov     al, 00100000b
   asm 	out     20h, al
   asm  sti;
*/
   asm cli

   asm mov ax, 0x0b
   asm int 33h
   asm mov x, cx
   asm mov y, dx

   if( (x||y) && !ExecOk() )
   {
      if( mouse_state ) BufMoveImage( mouse_x, mouse_y, mouse_x+29, mouse_y+14, mcga1, mcga0 ) ;
      mouse_x += x ;
      mouse_y += y ;
      if( mouse_x < mouse_left   ) mouse_x = mouse_left   ;
      if( mouse_x > mouse_right  ) mouse_x = mouse_right  ;
      if( mouse_y < mouse_top    ) mouse_y = mouse_top    ;
      if( mouse_y > mouse_bottom ) mouse_y = mouse_bottom ;
      if( mouse_state )
      {
	 BufSpritePutImage( mouse_x, mouse_y, mouse_shape, mcga0 ) ;
      }
   }

   asm sti

   oldint8();
}


void mouse_cursor_off( void )
{
   asm cli ;

   if( mouse_exist )
   {
      BufMoveImage( mouse_x, mouse_y, mouse_x+29, mouse_y+14, mcga1, mcga0 ) ;
      mouse_state = OFF ;
   }
   asm sti ;
}

void mouse_cursor_on( void )
{
   asm cli ;

   if( mouse_exist )
   {
      BufSpritePutImage( mouse_x, mouse_y, mouse_shape, mcga0 ) ;
      mouse_state = ON ;
   }
   asm sti ;
}

void set_int8( void )
{
   oldint8 = getvect( 8 ) ;
   setvect( 8, newint8 ) ;
}

void CloseMouse( void )
{
   if( mouse_exist )
   {
      setvect( 8, oldint8 ) ;
      MouseReset() ;
   }
}

int InitMouse( void )
{
//   if( !MouseTest() )
//   {
      mouse_exist = FALSE ;
      return FALSE ;
//   }
   mouse_exist = TRUE ;
   mouse_state = OFF  ;
   mouse_left  = mouse_top=0 ;
   mouse_x = 160 ;
   mouse_y = 100 ;
   mouse_right  = 320 ;
   mouse_bottom = 200 ;

   MouseReset() ;
   MouseEvent( 2, event_handler ) ;
   set_int8() ;
   return TRUE ;
}

int read_mouse( void )
{
   union REGS inregs;

   if( !mouse_exist ) return 0 ;
   if( mh != mt )
   {
      if( mh==MAX-1 ) mh=0;
      if ((MCLK-pbuf.clk)<DBLCLK )
      {
	 memcpy(&pbuf,&mbuf[mh],sizeof(pbuf));
	 mh++;
	 return(DOUBLE_CLK);
      }
      else
      {
	 memcpy(&pbuf,&mbuf[mh],sizeof(pbuf));
	 mh++;
	 return(LEFT_CLK);
      }
   }
   return NULL;
}

