// VOXEL.CPP ////////////////////////////////////////////////////////////////

// Thomas H.

// INCLUDES /////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <math.h>
#include <dos.h>
#include <stdlib.h>

#include "template.h"
#include "voxel.h"
#include "timer.h"
#include "global.h"
#include "seg.h"
#include "xgraf.h"

// DEFINES //////////////////////////////////////////////////////////////////

#define VOXX	128
#define VOXY	96

#define BACK  0.5
#define HFAC	0.3
#define SENTERY 145

#define ZOOMFAC 1.5873
#define HITIT   125600
#define REALLY	1
#define RANFAC  127
#define RANDOMIZE 1

//#define CHEAT
//#define OVERVIEW

// EXTERNALS ////////////////////////////////////////////////////////////////

// FUNCTIONS

extern "C" void asmvox(void);
extern "C" void interpolate(void);
extern "C" void asmsky(void);

// VARS

extern timer_C timer;
extern long plustimer;
extern long far copper_32,
       far leapright;
extern word far sseg,
       far hseg,
       far fseg,
       far lseg,
       far voxpos,
       far voypos;
extern byte far alti,
       far ofraction,
       far interlimit,
       *pressed,
       rgb[];
extern ulong musicstart;
extern int relspeed;

// VARS /////////////////////////////////////////////////////////////////////

word *ra1,
     *ra2,
     *ra3,
     *ra4;
float zoomfac=ZOOMFAC;

// RECURSE

void recurse(word *ra,word pos,word len)
{
	long l=ra[pos];
	long r=ra[pos+len];
	long maxr=(long)len*RANFAC;
	long ny=random(2l*maxr+1)-maxr;
	if (len>=32)
		ny*=4;
	long her=(long)l+r+ny;
	her>>=1;
	if (her<0)
		her=0;
	if (her>16383)
		her=16383l;
	int plus=len>>1;
	ra[pos+plus]=her;
	if (plus==1)
		return;
	recurse(ra,pos,plus);
	recurse(ra,pos+plus,plus);
}

// MAKERAND

void makerand(word *ra)
{
	ra[0]=ra[256]=8192;
	recurse(ra,0,256);
}

// DIST

float dist(int no)
{
	float fac=no/96.0;
	fac=pow(fac,1.05);
	return fac*256.0;
}

// PRECALC

void precalc(byte *lookup, byte *copper)
{
	byte *olookup=lookup;
	byte *ocopper=copper;
	FILE *fp=fopen(VOXPRE,"rb");
	byte *leap=(byte*)leapright;

	for (int o=0; o<64; o++)
	{
		float vekk=64.0-o;
		float s=max((double)0,64-sqrt(4096.0-vekk*vekk));
		leap[o]=leap[127-o]=2*s;
	}

	for (word cut=0; cut<128l*96l*3; cut++)
	{
		byte temp;
		fread(&temp,1,1,fp);
		ocopper[cut]=temp;
	}

	for (cut=0; cut<96l*256l; cut++)
	{
		byte temp;
		fread(&temp,1,1,fp);
		olookup[cut]=temp;
	}

	fclose(fp);
}

// VISVOX

void visvox(byte *page,float camv, word screenseg,int step)
{
#ifndef CHEAT
	int tempdx,tempcx;
	byte count;

	float co=cos(camv);
	float si=sin(camv);

	float hx=co*256/zoomfac;
	float hy=si*256/zoomfac;

	int x0=32768-160*hx+120*hy;
	int y0=32768-160*hy-120*hx;

	int plx=hx;
	int ply=hy;
	int plx4=hx*4;
	int ply4=hy*4;

	asm pusha
	asm push ds
	asm lds si,page

	int mapval=MAP_MASK-256+512*step;

	for (byte plan=0; plan<4; plan+=step)
	{
		asm mov ax,screenseg
		asm mov es,ax

		asm mov ax,mapval
		asm mov cl,plan
		asm shl ah,cl
		asm mov dx,SC_INDEX
		asm out dx,ax

		asm xor di,di
		asm mov cx,x0
		asm mov dx,y0

		asm mov count,80
		asm mov si,plx
next_right:
		asm mov tempcx,cx
		asm mov tempdx,dx
		asm mov ah,60
next_down:
		asm mov bl,ch
		asm mov bh,dh
		asm mov al,[bx]
		asm mov [es:di],al
		asm add di,80
		asm sub cx,ply
		asm add dx,si
		asm mov bl,ch
		asm mov bh,dh
		asm mov al,[bx]
		asm mov [es:di],al
		asm add di,80
		asm sub cx,ply
		asm add dx,si
		asm mov bl,ch
		asm mov bh,dh
		asm mov al,[bx]
		asm mov [es:di],al
		asm add di,80
		asm sub cx,ply
		asm add dx,si
		asm mov bl,ch
		asm mov bh,dh
		asm mov al,[bx]
		asm mov [es:di],al
		asm add di,80

		asm sub cx,ply
		asm add dx,si
		asm dec ah
		asm jnz next_down

		asm add di,1-240*80
		asm mov cx,tempcx
		asm mov dx,tempdx
		asm add cx,plx4
		asm add dx,ply4
		asm dec count
		asm jnz next_right

		asm mov cx,plx
		asm add x0,cx
		asm mov cx,ply
		asm add y0,cx

	}
	asm pop ds
	asm popa
#else
	asm pusha
	asm push ds

	asm lds si,page
	asm mov ax,0xa000
	asm mov es,ax
	asm xor si,si
	asm xor di,di

	asm mov dx,200
l1:
	asm mov cx,128
	asm rep movsw
	asm add di,320-256
	asm dec dx
	asm jnz l1
	asm pop ds
	asm popa

#endif
}

// LOADSKY

void loadsky(byte *sky)
{
	FILE *infile=fopen(CLOUDFILE,"rb");
	for (int qy=0; qy<256; qy++)
	for (int qx=0; qx<256; qx++)
	{
		byte col;
		fread(&col,1,1,infile);
		sky[qx+(qy<<8)]=col;
	}
	fclose(infile);
}

// FIXSKY

void fixsky(byte *page, byte *sky, char skyx, byte skyy)
{
  asm pusha
  asm push ds

  asm les di,page
  asm lds bx,sky
  asm add bh,skyx
  asm add bl,skyy

  asmsky();

  asm pop ds
  asm popa
}

// COLORSTRIP

void colorstrip(byte *height, byte *color, byte stripe)
{
	for (int x=0; x<256; x++)
	{
		byte xx=x+2;
		byte xxx=x-2;
		int her=height[x+(stripe<<8)];
		int der=height[xx+(stripe<<8)];
		int tre=height[xxx+(stripe<<8)];
		int diff=3*der-2*her-1*tre;
		diff+=64;
		fit(2,diff,127);
		color[x+(stripe<<8)]=diff;
	}
}

// MAKESTRIP

void makestrip(byte *ptr,int no,float pro)
{
	int minus=384-no;
	if (pro>0.65)
		minus=(pro-0.65)*250;
	fit(0,minus,127);
	float f1=1+sin(no/23.0+2.5);
	float f2=1+sin(no/21.0+1.5+f1/3);
	float f3=1+sin(no/34.0+2.5);
	float f4=1+sin(no/16.0+1.5+f3/2);

	int pl1=f1*4+no/2.0;
	int pl2=f2*8-no/1.5;
	int pl3=f3*7+no/4;
	int pl4=f4*3-no/3;

	byte *optr=&ptr[no<<8];

	for (int count=0; count<256; count++)
	{
		word v1=ra1[(count+pl1)&255];
		word v2=ra2[(count+pl2)&255];
		word v3=ra3[(count+pl3)&255];
		word v4=ra4[(count+pl4)&255];
		word vv=v1+v2+v3+v4;
		long sumb=vv>>9;
		sumb-=minus;
		if (sumb<0)
			sumb=random(2);
		*(optr++)=128+sumb;
	}
}

// VOXEL

void voxel(void)
{
#if RANDOMIZE
	randomize();
#endif
	zoomfac=ZOOMFAC;
#ifdef CHEAT
	asm mov ax,0x13
	asm int 0x10
#endif

	setpalette(VOXELPAL);

	seg_C height;
	seg_C color;
	seg_C lookup;
	seg_C sky;
	seg_C randata(4096);

	ra1=(word*)&randata.ptr[0];
	ra2=(word*)&randata.ptr[512];
	ra3=(word*)&randata.ptr[1024];
	ra4=(word*)&randata.ptr[1536];

	hseg=(long)height.ptr>>16;
	fseg=(long)color.ptr>>16;
	lseg=(long)lookup.ptr>>16; 

#ifndef OVERVIEW
	loadsky(sky.ptr);
#endif

#ifndef OVERVIEW
	precalc(lookup.ptr, (byte*)copper_32);
#endif
	makerand(ra1);
	makerand(ra2);
	makerand(ra3);
	makerand(ra4);

	for (long e=0; e<65536l; e++)
	{
		height.ptr[e]=random(2)+128;
		height.ptr[e]=128;
		color.ptr[e]=59+random(10);
	}

	int s;
	seg_C page;
	sseg=(long)page.ptr>>16;

	for (s=0; s<1280; s++)
	{
		height.ptr[s]=(s>=4*256)?138:128;
		color.ptr[s]=0;
	}

	word screenseg=0xa000;

	word xpos=0,ypos=0;
	long dxpos=0,dypos=0;
	float frames=0,dvin=0,vin=M_PI_2,alt=0;
	int done=0,pixsize=1+(relspeed<300);

	long estart=timer.readtimer();
	long sofar=timer.elapsed(musicstart,estart);
	long left=END_VOXEL-sofar-1100l;
	int sretn=1-2*(random(100)>20);
	do
	{
		long gaatt=timer.readtimer();
		gaatt=timer.elapsed(estart,gaatt);
		float pro=(float)gaatt/left;
		if (pro>0.90)
		{
			float zfac=1+(pro-0.90)*150;
			zoomfac=ZOOMFAC*zfac*zfac;
		}

		if (pressed[59])
			pixsize=1;
		if (pressed[60])
			pixsize=2;
		if (pressed[61])
			interlimit++;
		if (pressed[62])
			interlimit--;
		voypos=ypos>>8;
		voxpos=xpos;
		ofraction=voxpos;
		frames++;
		if (pro>0.80)
			vin+=(pro-0.80)*4.6*sretn;
		dvin+=(pressed[77]-pressed[75])*0.007;
		vin+=dvin;

		if (dypos>512)
			dypos=512;

		long hit=timer.elapsed(musicstart,timer.readtimer());
		if (hit<HITIT)
			dypos=0;

		byte test1=xpos>>8;
		byte test0=test1-15;
		byte test2=test2+15;
		byte testy=voypos+30;
		word testypos=256*testy;

		int der0=height.ptr[test0+testypos];
		int der2=height.ptr[test2+testypos];

		float vfacx=sin(vin);
		float vkorr=cos(vin);
		if (dypos)
			dvin+=vkorr*0.01;
		dvin*=0.975;
		if (vfacx<0)
			vfacx=0;
		int hdiff=(der2-der0);
		if (dypos)
			dvin+=-hdiff*0.00009;

		if (dypos)
			xpos+=dxpos-1000*cos(vin);
		ypos+=dypos;

		static word voypos2;

		makestrip(height.ptr,voypos2+254,pro);
		makestrip(height.ptr,voypos2+255,pro);
		colorstrip(height.ptr,color.ptr,(voypos2+252)&255);
		colorstrip(height.ptr,color.ptr,(voypos2+253)&255);

		alt+=2*(pressed[72]-pressed[80]);

		voypos2+=dypos/256;
		testy=voypos2;
		byte xx=(xpos>>8);
		xx+=4;
		float inall=height.ptr[xx+256*testy];
		float inall2=height.ptr[xx+256*testy+5120];
		if (pro>0.8)
			inall2=128+((pro-0.8)*1270);
		if (inall2>inall)
			inall=inall2;
		inall-=110;
		if (inall>127)
			inall=127;
		float qq=20;
		if (inall>alt)
			qq=3;
		if (!dypos)
			qq=3;
		float diff=(inall-alt)/qq;
		alt+=diff;
		if (diff<0.5 && !dypos)
			dypos=512;

		fit((float)0,alt,(float)127);
		alti=alt;
    asmvox();
		interpolate();
		fixsky(page.ptr,sky.ptr,(xpos+50000000l)/256l,0);
#ifdef OVERVIEW
		visvox(color.ptr,vin,screenseg,pixsize);
#else
		visvox(page.ptr,vin,screenseg,pixsize);
#endif

#ifndef CHEAT
		yoffset((screenseg-0xa000)/5);
		screenseg+=240*5;
		if (screenseg==240*5*3l+0xa000)
			screenseg=0xa000;
#endif

		done|=(pro>=1.0);
	}while (!done);

	byte col=getpixel(160,100);
	float rr=rgb[col*3];
	float rg=rgb[col*3+1];
	float rb=rgb[col*3+2];
	for (int count=0; count<70; count++)
	{
		float fac=1-count/69.0;
		byte r=rr*fac;
		byte g=rg*fac;
		byte b=rb*fac;
		frame();
		setrgb(col,r,g,b);
	}
}



