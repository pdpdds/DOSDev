#include "v256.h"
#include <alloc.h>

void main(void)
{
 BYTE *space,*explo,*image,*getimg;

 set256mode();
 loadrgb("v256.rgb");
 readpcxfilebuf("font.pcx",&space);
 readpcxfilebuf("computer.pcx",&explo);
 displaypcxbuf(space,1);

 imagesize256(5,130,5+58,130+58,&image);
 getimage256(5,130,5+58,130+58,image);

 composition(237,1,294,58,explo,5,130,space,0);
 virtualscreen2(5,130,5+58,130+58,0,0,1,space);
 getch();

 getcomposition(image,5,130,space,1);
 virtualscreen2(5,130,5+58,130+58,0,0,1,space);
 getch();

 imagesize256(60,60,117,117,&getimg);
 getimagevirtual(60,60,117,117,getimg,explo);
 putimage256(50,50,0,getimg);
 getch();

 farfree(image);
 farfree(explo);
 farfree(space);
 settextmode();
}
