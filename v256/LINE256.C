#include "byte.h"
extern xorputpixel256(unsigned,unsigned,BYTE);
extern putpixel256(unsigned,unsigned,BYTE);

void line256(startx,starty,endx,endy,mode,color)
unsigned startx,starty,endx,endy;
int mode;
BYTE color;
{
	register int t,distance,xerr=0,yerr=0,delta_x,delta_y,incx,incy;

	delta_x=endx-startx;
	delta_y=endy-starty;
	if(delta_x>0) incx=1; else if(delta_x==0) incx=0; else incx=-1;
	if(delta_y>0) incy=1; else if(delta_y==0) incy=0; else incy=-1;

	delta_x=abs(delta_x);
	delta_y=abs(delta_y);
	if(delta_x>delta_y) distance=delta_x; else distance=delta_y;

	if(mode){
	   xorputpixel256(startx,starty,color);
	   for(t=0; t<=distance; t++){
	   xorputpixel256(startx,starty,color);
	   xerr+=delta_x;
	   yerr+=delta_y;
	   if(xerr>distance){ xerr-=distance; startx+=incx;}
	   if(yerr>distance){ yerr-=distance; starty+=incy;}
				     }
	   xorputpixel256(startx,starty,color);
		}
	else{
	     putpixel256(startx,starty,color);
	     for(t=0; t<=distance; t++){
	     putpixel256(startx,starty,color);
	     xerr+=delta_x;
	     yerr+=delta_y;
	     if(xerr>distance){ xerr-=distance; startx+=incx;}
	     if(yerr>distance){ yerr-=distance; starty+=incy;}
				      }
	     putpixel256(startx,starty,color);
	    }
}

