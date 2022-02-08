#include "byte.h"
#include <dos.h>

extern void uiswap(unsigned *, unsigned *);
extern void memtomem(unsigned,unsigned,unsigned,unsigned,unsigned,BYTE);

void virtualscreen2 (x1,y1,x2,y2,vx,vy,mode,image)
unsigned x1,y1,x2,y2,vx,vy;
int mode;
BYTE *image;
{
    unsigned yy;

    if(y2<y1) uiswap(&y2,&y1);
    if(x2<x1) uiswap(&x2,&x1);

 for(yy=y1; yy<=y2; yy++)
 memtomem(0x0a000,320*(yy-y1+vy)+vx,FP_SEG(image),FP_OFF(image)+(320*yy+x1),x2-x1+1,mode);
}
