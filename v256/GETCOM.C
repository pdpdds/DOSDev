#include "byte.h"
#include <dos.h>

extern void memtomem(unsigned,unsigned,unsigned,unsigned,unsigned,BYTE);

void getcomposition(simage,dx,dy,dimage,mode)
unsigned dx,dy;
int mode;
BYTE *simage,*dimage;
{
 unsigned y,count,si_off=FP_OFF(simage)+3;

 count=simage[0]+simage[1]+1;

 for(y=dy; y<=dy+simage[2]; y++){
 memtomem(FP_SEG(dimage),FP_OFF(dimage)+320*y+dx,FP_SEG(simage),si_off,count,mode);
 si_off+=count;
                                }
}