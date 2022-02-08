#include <io.h>
#include <dos.h>
#include <stdlib.h>
#include <stdarg.h>
#include <fcntl.h>
#include <alloc.h>
#include "v256.h"

int xposi, yposi, hancolor, xsi__ze__,ysi__ze__;

BYTE  (*hanfont1)[20][32],
      (*hanfont2)[22][32],
      (*hanfont3)[28][32],
      (*engfont)[16];
//    (*hangrfont)[32];

struct himgbuf{
		int x,y;
		BYTE bitmap[32];
	      } hanimgbuf;

struct eimgbuf{
		int x,y;
		BYTE bitmap[16];
	      } engimgbuf;

void     inithan (BYTE *,BYTE *);
void     putimg (int,int,int,void *);
void     hanxy (int,int,int,BYTE *);
void     hanend (void);

void inithan(BYTE *hanfontfile,BYTE *engfontfile )
{
    int handle;

    if((handle = open ( hanfontfile,O_RDONLY|O_BINARY ))!=-1){
	hanfont1=malloc(5120);
	hanfont2=malloc(2816);
	hanfont3=malloc(3584);
//	hangrfont=malloc(3968);
        read ( handle ,hanfont1 ,5120 );
	read ( handle ,hanfont2 ,2816 );
	read ( handle, hanfont3 ,3584 );
//	read ( handle, hangrfont,3968 );
	close ( handle );                                    }
    else { settextmode();
	   printf("FNT file not found or disk error!\n");
	   free(hanfont1);
	   free(hanfont2);
	   free(hanfont3);
//	   free(hangrfont);
	   exit(-1);
	 }
    if((handle = open ( engfontfile,O_RDONLY|O_BINARY ))!=-1){
	engfont=malloc(1536);
	read ( handle, engfont, 1536 );
	close ( handle );
							     }
    else {
	 settextmode();
	 printf("FNT file not found or disk error!\n");
	 hanend();
	 exit(-1);
	 }
    hanimgbuf.x=15; hanimgbuf.y=15;
    engimgbuf.x=7; engimgbuf.y=15;
    xposi=0; yposi=0;
    xsi__ze__=1; ysi__ze__=1;
    hancolor =15;
}

void putimg (int x, int y, int gray, void *bitmap)
{
    char i,j,k,xrep,yrep;
    register int xx;
    BYTE data,hcolor;
    int idx=4,idxx;

	hcolor=hancolor;
	for(i=0; i<=*((int*)bitmap + 1); i++){
	    for(yrep=1; yrep<=ysi__ze__; yrep++){
		xx = x; idxx = idx;
		for(j=0; j<=*((int*)bitmap)/8; j++){
		    data = *((BYTE*)bitmap + idxx++);
		    for (k=7; k>=0; k--)
		      for(xrep=1; xrep<=xsi__ze__; xrep++){
         		  if((data & (1<<k))!= 0) putpixel256(xx,y,hcolor);
			  xx++;                           }
						   }
		y++; if(gray) hcolor++;         }
	    idx=idxx;                         }
}

void hanxy (int xpos, int ypos, int gray, BYTE *fmt)
{
    BYTE data1, data2, first, mid, last, b1, b2, b3,*md,
	 mid_data1[]={0,0,2,0,2,1,2,1,2,3,0,2,1,3,3,1,2,1,3,3,1,1},
	 table[3][32]={
		      { 0,  0,  1,  2,  3,  4,  5,  6,
			7,  8,  9, 10, 11, 12, 13, 14,
		       15, 16, 17, 18, 19,  0,  0,  0,
			0,  0,  0,  0,  0,  0,  0,  0 },
		      { 0,  0,  0,  1,  2,  3,  4,  5,
			0,  0,  6,  7,  8,  9, 10, 11,
			0,  0, 12, 13, 14, 15, 16, 17,
			0,  0, 18, 19, 20, 21,  0,  0 },
		      { 0,  0,  1,  2,  3,  4,  5,  6,
			7,  8,  9, 10, 11, 12, 13, 14,
		       15, 16,  0, 17, 18, 19, 20, 21,
		       22, 23, 24, 25, 26, 27,  0,  0 }
		       };

    register int i; char str[140]={0,};
    va_list argptr;

    va_start(argptr,fmt);
    vsprintf(str,fmt,argptr);
    xposi=xpos; yposi=ypos;

    for(i=0; i<strlen(str); i++)
      {
	data1 = *(str+i);
	if ( data1 > 127 ) {
	    i++; data2 = *(str+i);
	    if ( data1 == 212 );
                             /*   {
		smemtosmem( hanimgbuf.bitmap, hangrfont[data2-128],32 );
		putimg( xposi, yposi, gray, &hanimgbuf );
				}  */
	    else {
		 first = ( data1 & 124 ) >> 2;
		 mid   = ( data1 & 3)*8 + ( data2 >> 5 );
		 last  = data2 & 31;

		 first = table[0][first];
		 mid   = table[1][mid];
		 last  = table[2][last];

                 md=mid_data1;
                 asm{
                     push ds
                     lds bx,md
                     mov al,mid
                     xlat
		     mov b3,al
		     pop ds
		     }

		 switch(first)
		   {
		     case  1:
		     case 16: if ( last == 0 ) b2 = 0;
			      else b2 = 2; break;
		     default: if ( last == 0 ) b2 = 1;
			      else b2 = 3;
		    }

		 switch ( mid )
		   {
		     case  1: case  2: case  3:
		     case  4: case  5: case  6:
		     case  7: case  8: case 21: if(last==0) b1=0; else b1=5; break;
		     case  9: case 13: case 19: if(last==0) b1=1; else b1=6; break;
		     case 14: case 18:          if(last==0) b1=2; else b1=6; break;
		     case 10: case 11: case 12:
		     case 20:                   if(last==0) b1=3; else b1=7; break;
		     case 15: case 16: case 17: if(last==0) b1=4; else b1=7;
		   }

		 if ((mid==0) && (last==0)) b1=0;
		 if (first != 0) {
		     smemtosmem ( hanimgbuf.bitmap, hanfont1[b1][first], 32);
		     putimg(xposi,yposi,gray,&hanimgbuf );
				  }
		 if ( mid != 0 ) {
		     smemtosmem ( hanimgbuf.bitmap, hanfont2[b2][mid], 32);
		     putimg(xposi,yposi,gray,&hanimgbuf );
				 }
		 if ( last != 0 ) {
		     smemtosmem ( hanimgbuf.bitmap, hanfont3[b3][last], 32);
		     putimg(xposi,yposi,gray,&hanimgbuf );
				  }
	      }
	    xposi += 16*xsi__ze__;
	  }
	else {
	    if ( data1 < 32 ) data1 = 32;
	    smemtosmem ( engimgbuf.bitmap, engfont[data1-32], 16);
	    putimg(xposi,yposi,gray,&engimgbuf );
	    xposi += 8*xsi__ze__;
	    if ( xposi > 320 - 8*xsi__ze__ ) {
		xposi = 0; yposi += 16*ysi__ze__;
		if ( yposi > 200 - 16*ysi__ze__ ) yposi = 0;
					     }
	  }
      }
      va_end(argptr);
}

void hanend(void)
{
      free(hanfont1);
      free(hanfont2);
      free(hanfont3);
      free(engfont);
//    free(hangrfont);
      free(engfont);
}

