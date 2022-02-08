#include <dos.h>
#include <graphics.h>
#include <alloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#include "svga256.h"


int jztimer(void)
{        /* count elapsed time since last call (in ticks) */
 union REGS sreg , dreg ;
 long etime , delta ;
 static long stime ;		       /* time-of-day from last call*/

 sreg.x.ax = 0 ;		     /* operation = get time count */
 int86(0x1a,&sreg,&dreg);	    /* do software interrupt */
				       /* assemble 32-bit TOD value */
 etime = ( ((long) dreg.x.cx) << 16 ) + dreg.x.dx;
 delta = etime - stime ;
 if( (dreg.x.ax & 0xff) != 0)       /* new day since last call? */
    delta = delta + 0x01800B0L ;   /* yes-add 1 day in ticks*/
 stime = etime ;		       /* save TOD for next call */
 return( (int) (delta / 18 )) ;	      /* return as an integer */
}

/* detects EGA or VGA cards */
int huge detectSVGA(void)
{
   int driver, mode, sugmode = SVGA1024x768x256;
#if 0
   detectgraph(&driver, &mode);
   if ((driver == EGA) || (driver == VGA))
      /* return suggested video mode number */
      return sugmode;
   else
      /* return an error code */
      return grError;
#endif
      /* return video mode number */
      return sugmode;

}

int huge detectSVGA(void);

struct PTS
{
  int x, y;
};	/* Structure to hold vertex points	*/


void main(void)
{
    struct PTS poly[ 6 ];		/* Space to hold datapoints	*/
    int g_driver = DETECT,g_error =0, g_mode;

    installuserdriver("SVGA256",detectSVGA);
    initgraph(&g_driver,&g_mode,"");
    g_error = graphresult();
    if (g_error != 0)
    {
       printf("%s \n",grapherrormsg(g_error));
       exit(1);
    }
    settextstyle(3,HORIZ_DIR,3);
    setcolor(WHITE);
    outtextxy(getmaxx()/2,getmaxy()/2,"Sample Text");
    (void)getch();

//    (void) jztimer();
    setfillstyle(SOLID_FILL,120);
    setcolor(75);    fillellipse(85,75,50,50);
    setcolor(3);     setfillstyle(EMPTY_FILL,0);

    poly[0].x = 50;
    poly[0].y = 50;
    poly[1].x = 75;
    poly[1].y = 30;
    poly[2].x = 150;
    poly[2].y = 50;
    poly[3].x = 225;
    poly[3].y = 18;
    poly[4].x = 290;
    poly[4].y = 150;
    poly[5].x = 150;
    poly[5].y = 100;
    poly[6].x = 10;
    poly[6].y = 180;
    poly[7].x = 50;
    poly[7].y = 50;

    fillpoly( 8, (int far *)poly );    /* Draw the actual polygon	    */
    setfillstyle(XHATCH_FILL,4);
    floodfill(75,75,3);
    (void)getch();
//    restorecrtmode();
    closegraph();
//    printf("\nIt took %d seconds to do 300 circles with our driver.\n", jztimer());
    exit(0);
//    return(0);
}
