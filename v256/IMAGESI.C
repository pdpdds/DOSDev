#include <alloc.h>
#include "byte.h"
extern void uiswap(unsigned *, unsigned *);
extern void setvmode(char);

void imagesize256(x1,y1,x2,y2,Image)
BYTE **Image;
unsigned x1,y1,x2,y2;
{
	void *image;
	unsigned xx,yy;

        if(y1>y2) uiswap(&y1,&y2);
        if(x1>x2) uiswap(&x1,&x2);
	xx=x2-x1+1; yy=y2-y1+1;

	image=(BYTE *)farmalloc(xx*yy+3);
	if(image==NULL) {
			 setvmode(0x03);
			 printf("Not enough memory!\n");
			 exit(-1);
			 }
	*Image=image;
}

