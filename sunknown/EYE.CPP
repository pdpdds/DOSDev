// EYE.CPP //////////////////////////////////////////////////////////////////

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
#include "voxel.h"

// DEFINES //////////////////////////////////////////////////////////////////

#define ZMIN    1.252
#define ZMAX		25
#define ZOOMFAC 9

#define HITIT  26600

#define LOGOX   303
#define LOGOY   19
#define LYPOS   (239l-LOGOY)

// EXTERNALS ////////////////////////////////////////////////////////////////

// FUNCTIONS

extern "C" void asmvox(void);
extern "C" void antialias(void);
extern "C" void asmsky(void);

// VARS

extern timer_C timer;
extern long far copper_32, far leapright;
extern word far sseg, far hseg, far fseg, far lseg, far voxpos, far voypos;
extern byte far alti,far ofraction, far interlimit;
extern byte *pressed;
extern byte rgb[];
extern ulong musicstart;
extern float zoomfac;
extern int relspeed;

// FUNCTIONS ////////////////////////////////////////////////////////////////

// ZOOMF

float zoomf(float pro)
{
	pro*=1.1;
	if (pro>1)
		pro=1;
	if (pro>0.5)
		pro=1-pro;
	pro=0.5-pro;
	pro*=2;
	pro-=0.5;
	if (pro<0)
		pro=0;
	pro=2*sqrt(0.25-pro*pro);
	pro=pow(pro,0.1);
	pro=ZMAX-(ZMAX-ZMIN)*pro;
	return pro;
}

// DRAWTITLE

void drawtitle(word sseg, byte *ptr, float pro)
{
	int len=(pro-0.3)*22*LOGOX;
	if (len>LOGOX)
		len=LOGOX;
	if (len<0)
		len=0;
	int xpos=160-len/2;
	byte p=xpos&3;
	word spos=(xpos/4l)+80l*LYPOS;
	int pno=-1;
	long dpos=(long)ptr;
	for (byte plan=p; plan<p+4; plan++)
	{
		pno++;
		byte rp=(plan&3);
		if (plan==4)
			spos++;

		asm mov cl,rp
		asm mov ax,0x0100+MAP_MASK
		asm shl ah,cl
		asm mov dx,SC_INDEX
		asm out dx,ax

		int ant=(len-pno+3)/4;
		if (!ant)
			goto skipant;
		asm mov es,sseg
		asm mov di,spos
		asm push ds
		asm lds si,dpos

moveright:
		asm mov cl,LOGOY
		asm mov ch,ss:[bx]
nextdown:
		asm mov al,[si]
		asm or al,al
		asm jz blank
		asm or al,ch
		asm mov es:[di],al
blank:
		asm inc si
		asm add di,80
		asm dec cl
		asm jnz nextdown
		asm add si,3*LOGOY
		asm add di,1-(80*LOGOY)

		asm dec ant
		asm jnz moveright
		asm pop ds
skipant:
		dpos+=LOGOY;
	}
}

// EYE

void eye(void)
{
	seg_C eye;
	seg_C tittel((long)LOGOX*LOGOY);

	setpalette(EYEPAL);

	FILE *infile=fopen(EYEBMP,"rb");
	for (long h=0; h<65536l; h++)
	{
		byte temp;
		fread(&temp,1,1,infile);
		if (h&255<20)
			temp=0;
		eye.ptr[h]=temp;
	}
	fclose(infile);

	infile=fopen(TITLE,"rb");
	for (int y=0; y<LOGOY; y++)
	for (int x=0; x<LOGOX; x++)
	{
		byte temp;
		fread(&temp,1,1,infile);
		tittel.ptr[y+(LOGOY)*x]=temp;
	}
	fclose(infile);

	word screenseg=0xa000+240*5;

	float vin=0,dvin=0;
	zoomfac=ZOOMFAC;
	int done=0;

	long hold;
	long musek;

	do
	{
		hold=timer.readtimer();
		musek=timer.elapsed(musicstart,hold);
	}while (musek<HITIT);

	ulong estart=timer.readtimer();
	ulong total=END_EYE-timer.elapsed(musicstart,estart);
	int debframe=0;

	do
	{
		debframe++;
		long now=timer.elapsed(estart,timer.readtimer());
		float pro=(float)now/total;
		zoomfac=zoomf(pro);
		vin+=dvin;
		dvin+=
			(pressed[72]-pressed[80])/100.0;
		if (pressed[75])
			zoomfac*=1.01;
		if (pressed[77])
			zoomfac/=1.01;
		vin+=0.02;
		visvox(eye.ptr,vin,screenseg,1);
		drawtitle(screenseg,tittel.ptr,pro);
		yoffset((screenseg-0xa000)/5);
		screenseg+=240*5;
		if (screenseg==240*5*3l+0xa000)
			screenseg=0xa000;

		done|=(pro>=1.0);
	}while (!done);

	frame();
	int yy=(screenseg-0xa000)/5;
	yoffset(yy);

	for (int x=0; x<320; x++)
	for (int y=220; y<240; y++)
	{
		byte temp=getpixel(x,y+yy);
		putpixel(x,y+240,temp);
	}
	yoffset(2*240);

	frame();
	outport(SC_INDEX,0x0f02);

	asm mov ax,0xa000
	asm mov es,ax
	asm xor ax,ax
	asm xor di,di
	asm mov cx,19200
	asm rep stosb
	asm mov cx,19200
	asm rep stosb

	for (int count=0; count<12; count++)
	{
		frame();
		yoffset(2*240-count*2);
	}

	frame();
	outport(SC_INDEX,0x0f02);
	asm mov ax,0xa000
	asm mov es,ax
	asm xor ax,ax
	asm xor di,di
	asm mov cx,32768
	asm rep stosb
	asm mov cx,32768
	asm rep stosb
}