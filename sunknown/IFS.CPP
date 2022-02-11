// IFS.CPP //////////////////////////////////////////////////////////////////

// Thomas H.

// INCLUDES /////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>

#include "template.h"
#include "ifs.h"
#include "xgraf.h"
#include "seg.h"
#include "keyscan.h"
#include "timer.h"
#include "math.h"
#include "xgraf.h"

// DEFINES  /////////////////////////////////////////////////////////////////

#define HITIT 204900
#define MAXLEV  7

// EXTERNALS ////////////////////////////////////////////////////////////////

// FUNCTIONS

extern "C"
{
  void asmifs(void);
  void blanker(void);
  void farmove(void);
  void farmove320(void);
  void onestrip(void);
  void movebak32(void);
  drawpage32(void);
  void onestrip(void);
}

// VARS

extern timer_C timer;
extern byte *pressed;
extern byte rgb[768];
extern long far farfrom, far farto;
extern long far fardotpage;
extern word far skheight;
extern ulong musicstart;
extern byte far ifslev;
extern word far ifsx, far ifsy;
extern int far ifsdx, far ifsdy;
extern word far ifslen;
extern long far writepage32;

// VARS /////////////////////////////////////////////////////////////////////

// SCALARS

int frameno,
    *sintab,
    *costab,
    number;
long segptr;
float sizefac=1,
      async=0,
      rfac1,
      rfac2;

// ARRAYS

int lbend[MAXLEV]	={-20,-20,-20,-20,-20},
		rbend[MAXLEV]	={20,20,20,20,20};
word lfac[MAXLEV]	={192*256,192*256,192*256,192*256,192*256},
		 rfac[MAXLEV]	={192*256,192*256,192*256,192*256,192*256};

// FUNCTIONS ////////////////////////////////////////////////////////////////

// SCALE

void scale(byte *ptr, byte *to,int number)
{
	long lptr=(long)ptr;
	number&=~1;
	word blanks=(200-number)/2;
	if (blanks)
	{
		writepage32=(long)to;
		_DX=blanks;
		blanker();
	}
	long lto=blanks<<8;
	lto+=(long)to;
	word first=0;
	if (number)
	{
		long jump=(256*200l);
		jump/=number;
		for (int count=0; count<number; count++)
		{
			word line=first>>8;
			first+=jump;
			long from=lptr+line*320;

			asm pusha
			asm push ds
			asm les di,lto
			asm lds si,from
			onestrip();
			asm pop ds
			asm popa

			lto+=256;
		}
	}
	if (blanks)
	{
		writepage32=(long)to+256*(200-blanks);
		_DX=blanks;
		blanker();
	}
}

// DRAWPAGE

void drawpage(byte *orig)
{
	long from=(long)orig;
	long to=0xa0000020;
	asm pusha
	asm push ds
	asm les di,to
	asm lds si,from
	drawpage32();
	asm pop ds
	asm popa
}

// SETPARAMETERS

void setparameters(float pro)
{
	sizefac=pow(pro,0.05);
	async=pro/40.0;
	pro=pow(pro,1.2);
	float sfac=1;
	for (int count=0; count<MAXLEV; count++)
	{
		float pro2=pro+count*async;
		lbend[count]=130*sin(pro2*rfac1)/number;
		rbend[count]=-100*sin(pro2*rfac2)/number;
		lfac[count]=(52768l+6000*sin(pro2*57))*sfac;
		rfac[count]=(52768l+6000*sin(pro2*39))*sfac;
		sfac*=sizefac;
	}
}

// DRAWIFS

void drawifs(word xp,word yp,word vin,word leng,byte level)
{
	vin&=255;
	ifsx=xp;
	ifsy=yp;
	ifsdx=costab[vin];
	ifsdy=sintab[vin];
	ifslen=leng;
	ifslev=level;
	asm pusha
	asm push ds
	asm lds si,segptr
	asmifs();
	asm pop ds
	asm popa
	if (level==MAXLEV)
		return;
	xp=ifsx;
	yp=ifsy;
	unsigned long l2=(unsigned long)leng*lfac[level];
	l2>>=16;
	int v2=vin+lbend[level];
	level++;
	drawifs(ifsx,ifsy,v2,l2,level);
	l2=(unsigned long)leng*rfac[level];
	l2>>=16;
	v2=vin+rbend[level];
	drawifs(xp,yp,v2,l2,level);
}

// TURN

int turn(float pro)
{
	if (pro<0.4)
		pro-=0.1;
	if (pro<0)
		pro=0;
	if (pro>0.5)
		pro=1-pro;
	if (pro<0.05)
		return 0;
	if (pro>0.10)
		return 200;
	pro-=0.05;
	pro*=10*M_PI;
	return 200*sin(pro);
}

// IFS

void ifs(void)
{
	seg_C orig(64000);
	seg_C buffer;
	seg_C sint(512);
	seg_C cost(512);

	sintab=(int*)sint.ptr;
	costab=(int*)cost.ptr;

	for (long count=0; count<256; count++)
	{
		sintab[count]=256*sin(count*2*M_PI/256);
		costab[count]=256*cos(count*2*M_PI/256);
	}

	FILE *infile=fopen(MARBLEBMP,"rb");
	for (long o=0; o<64000l; o++)
	{
		byte temp;
		fread(&temp,1,1,infile);

		if (o<320 || o>=63680l)
			temp=0;
		orig.ptr[o]=temp;
	}
	fclose(infile);

	_AX=0x13;
	asm int 0x10

	setpalette(IFSPAL);

	count=0;
	float frames=0;
	int done=0;
	number=3+2*(random(100)>80);
	if (random(100)>95)
		number=random(8)+1;

	while (timer.elapsed(musicstart,timer.readtimer())<HITIT);

	long vstart=timer.readtimer();
	long left=timer.elapsed(musicstart,vstart);
	left=(float)END_IFS-left-1000;

	rfac1=31+random(100)/20.0;
	rfac2=41+random(100)/20.0;

	do
	{
		long now=timer.readtimer();
		float pro=timer.elapsed(vstart,now);
		pro/=left;
		fit((float)0,pro,(float)1);
		frames++;

		scale(orig.ptr,buffer.ptr,turn(pro));

		int height=pro*250-70;
		fit(0,height,7);

		skheight=257*height;
		float slen=pro*10*20.0*256;
		slen=min(slen,(float)27*256);
		if (pro>0.9)
			slen*=10*(1-pro);

		for (int io=0; io<10; io++)
			if(pressed[59+io])
				number=io+1;

		setparameters(pro);
		static float dv=0,ddv=0;
		segptr=(long)buffer.ptr;
		float v=pro*pro*1800+dv;
		dv+=ddv;
		ddv+=(pressed[77]-pressed[75])/3.0;
		float plfac=256/number;
		for (int no=0; no<number; no++)
			drawifs((128-skheight)*256l,(100-skheight)*256l,v+plfac*no,slen,0);

		drawpage(buffer.ptr);

		count++;
		frameno++;
		if (pro>=1)
			done=1;
	} while (!done);
}
