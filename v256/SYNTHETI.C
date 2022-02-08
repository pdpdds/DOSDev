#include "byte.h"
#include <dos.h>
#include <alloc.h>

extern void imagesize256(unsigned,unsigned,unsigned,unsigned,BYTE **);
extern void getimage256(unsigned,unsigned,unsigned,unsigned,BYTE *);
extern void putimage256(unsigned,unsigned,int,BYTE *);
extern void memtomem(unsigned,unsigned,unsigned,unsigned,unsigned,BYTE);

void synthetic(x1,y1,x2,y2,simage)
unsigned x1,y1,x2,y2;
BYTE *simage;
{
   BYTE *dimage;

   imagesize256(x1,y1,x2,y2,&dimage);
   getimage256(x1,y1,x2,y2,dimage);

   memtomem(FP_SEG(dimage),(FP_OFF(dimage)+3),FP_SEG(simage),(FP_OFF(simage)+3),(x2-x1)*(y2-y1),0);

   putimage256(x1,y1,0,dimage);
   farfree(dimage);
}



