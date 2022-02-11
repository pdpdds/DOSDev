// DUNE.CPP /////////////////////////////////////////////////////////////////

// Thomas H.

// INCLUDES /////////////////////////////////////////////////////////////////

#include <math.h>
#include <dos.h>
#include <stdlib.h>
#include <stdio.h>

#include "template.h"
#include "dune.h"
#include "seg.h"
#include "timer.h"
#include "global.h"
#include "xgraf.h"

// DEFINES //////////////////////////////////////////////////////////////////

//#define EQUAL

// EXTERNALS ////////////////////////////////////////////////////////////////

// FUNCTIONS

extern "C" void makeplasma(void);
extern "C" void drawrelieff(void);

// VARS

extern timer_C timer;
extern int relspeed;
extern long far dune_map,
       far dune_buffer,
       far dune_light;
extern ulong musicstart;
extern byte *pressed,
       rgb[768];;
extern word far dunepl1,
       far dunepl2,
       far dunepl3;
extern far word duneplus;

// FUNCTIONS ////////////////////////////////////////////////////////////////

// MODE13

void mode13(void)
{
  _AX=0x13;
  VGAINT;
}

// FINDRELSPEED

void findrelspeed(void)
{
  mode13();
	long end,start=timer.readtimer();
  relspeed=0;
	do
	{
		asm xor di,di
		asm mov ax,0xa000
		asm mov es,ax
		asm xor ax,ax
		asm mov cx,15430
next_test:
		asm mov [es:di],ax
		asm add di,2
		asm dec cx
		asm jnz next_test
		relspeed++;
		end=timer.readtimer();
	}while (timer.elapsed(start,end)<1000.0);
}

// SETRGBDEFAULT

void setrgbdefault(void)
{
	FILE *infile=fopen(DUNEPAL,"rb");
	for (int count=0; count<768; count++)
	{
		byte temp;
		fread(&temp,1,1,infile);
		rgb[count]=temp;
	}
	fclose(infile);
}

// SETPAL

void setpal(int no)
{
	byte null=0;
	outportb(0x3c8,0);
	if (!no)
		for (int tell=0; tell<192; tell++)
			outportb(0x3c9,null);

	frame();
	outportb(0x3c8,64);
	for (int count=0; count<192; count++)
	{
		byte here=rgb[count+192*(1+no)];
		outportb(0x3c9,here);
	}
}


// FIXLIGHT

void fixlight(byte *ptr)
{
	for (int x=0; x<256; x++)
	for (int y=0; y<256; y++)
	{
		int res=(x-y);
		res+=((x+y)>>3)+128;
    fit(0,res,255);
		if (!(x+y))
			res-=5;
		*(ptr++)=res>>2;
	}
}

// FLIP

void flip(byte *data)
{
	asm pusha
	asm push ds

	asm lds si,data
	asm mov cx,400
l1:
	asm mov dx,256
l2:
	asm not word ptr[si]

	asm add si,2
	asm dec dx
	asm jnz l2

	asm mov dx,ds
	asm add dx,512/16
	asm mov ds,dx
	asm xor si,si
	asm dec cx
	asm jnz l1

	asm pop ds
	asm popa
}

// LOADPLASMA

void loadplasma(byte *ptr,char *filnavn,int sym)
{
	FILE *infile=fopen(filnavn,"rb");
	long l2,l=(long)ptr;
	if (!sym)
		l2=l+(65536l*32*200l);
	else
		l2=l+(65536l*32l*399l);
	byte *ptr2=(byte*)l2;
	for (int y=0; y<200; y++)
	{
		for (int x=0; x<256; x++)
		{
			byte temp;
			fread(&temp,1,1,infile);
			ptr[x]=ptr2[x]=temp;
			if (!sym)
			{
				ptr[x+256]=temp;
				ptr2[x+256]=temp;
			}
			else
			{
				ptr[511-x]=temp;
				ptr2[511-x]=temp;
			}
		}
		l+=65536l*32l;
		if (!sym)
			l2+=65536l*32;
		else
			l2-=65536l*32;
		ptr=(byte*)l;
		ptr2=(byte*)l2;
	}
	fclose(infile);
}

// DRAWALL

void drawall(float count)
{
	dunepl1=
		64.0*(1+sin(count*1.0)*0.4+sin(count*1.5)*0.6);
	dunepl2=
		64.0*(1+sin(count*1.3)*0.7+sin(count*1.9)*0.3);
	dunepl3=
		64.0*(1+sin(count*0.6)*0.5+sin(count*1.3)*0.5);

	dunepl1<<=9;
	dunepl2<<=9;
	dunepl3<<=9;

	dunepl1+=
		32+64.0*(1+sin(count*0.8)*0.5+sin(count*2.0)*0.5);
	dunepl2+=
		32+64.0*(1+sin(count*1.9)*0.3+sin(count*1.3)*0.7);
	dunepl3+=
		32+64.0*(1+sin(count*1.6)*0.8+sin(count*2.4)*0.2);

#ifdef EQUAL
	dunepl2=dunepl3=dunepl1;
#endif

	makeplasma();
	drawrelieff();
}

// GOMORPH

void gomorph(long msek, long start)
{
	long diff=timer.elapsed(start,timer.readtimer());
	msek-=diff;
	int fadin=msek/5;
	int fadout=0.8*msek;
	long now;
	do
	{
		now=timer.elapsed(start,timer.readtimer());
		now-=diff;
		if (now<=fadin)
		{
			duneplus=now*64.0/fadin;
			duneplus*=257;
		}
		if (now>=fadout)
		{
			duneplus=(msek-now)*64.0/fadin;
			duneplus*=257;
		}
		float count=45.0+now/3000.0;
		drawall(count);
		count+=0.03;
	}while (now<msek);
}

// DUNE

void dune(void)
{
	setrgbdefault();

	seg_C map(512l*400l);
	seg_C buffer;
	seg_C lighting;

	dune_map=(long)map.ptr;
	dune_buffer=(long)buffer.ptr;
	dune_light=(long)lighting.ptr;

	mode13();
	fixlight(lighting.ptr);

	ulong start=timer.readtimer();
	ulong gone=timer.elapsed(musicstart,timer.readtimer());
	ulong left=END_DUNE-gone;
	left/=3;

	setpal(0);

	loadplasma(map.ptr,DUNEMAP1,0);
	flip(map.ptr);
	gomorph(left,start);

	setpal(1);
	flip(map.ptr);
	gomorph(left*2,start);

	setpal(2);
	loadplasma(map.ptr,DUNEMAP2,1);
	gomorph(left*3,start);
}
