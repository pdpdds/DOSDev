#include "byte.h"
#include <dos.h>

extern void uiswap(unsigned *, unsigned *);
extern void memtomem(unsigned,unsigned,unsigned,unsigned,unsigned,BYTE);

void composition(x1,y1,x2,y2,simage,dx,dy,dimage,mode)
unsigned x1,y1,x2,y2,dx,dy;
int mode;
BYTE *simage,*dimage;
{
 unsigned y,sy;

 if(y2<y1) uiswap(&y2,&y1);
 if(x2<x1) uiswap(&x2,&x1);
 sy=y1;

 for(y=dy; y<=dy+(y2-y1); y++)
 memtomem(FP_SEG(dimage),FP_OFF(dimage)+320*y+dx,FP_SEG(simage),FP_OFF(simage)+(320*sy++ +x1),x2-x1+1,mode);
}