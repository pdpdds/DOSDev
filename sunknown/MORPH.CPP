// MORPH.CPP ////////////////////////////////////////////////////////////////

// Thomas H.

// INCLUDES /////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <math.h>
#include <dos.h>
#include <stdlib.h>
#include <string.h>

#include "dune.h"
#include "template.h"
#include "timer.h"
#include "global.h"
#include "seg.h"
#include "xgraf.h"

// DEFINES //////////////////////////////////////////////////////////////////

#define SLACK 4500
#define SPEEDFAC 0.50

// EXTERNALS ////////////////////////////////////////////////////////////////

// FUNCTIONS

extern "C" void gomorph(void);
extern "C" void drawmorph(void);
extern void quit(int errval);

// VARS

extern byte loop;
extern timer_C timer;
extern long far mo192,
       far mobuffer;
extern word far pixies;
extern ulong musicstart;
extern int relspeed;
extern byte *pressed;

// FUNCTIONS ////////////////////////////////////////////////////////////////

// LOADDATA

void loaddata(byte *ptr, FILE *fp, int number)
{
	byte temp;
	for (word w=0; w<number*3l; w++)
	{
		fread(&temp,1,1,fp);
		ptr[w]=temp;
	}
}

// MAKEFIRST

void makefirst(byte *x,byte *y, byte *f, byte *raw,int pix)
{
  for (word count=0; count<pix; count++)
    x[count]=y[count]=f[count]=0;

  for (count=0; count<pix; count++)
  {
    x[1]=*(raw++);
    y[1]=*(raw++);
    f[1]=*(raw++);
		x+=4;
		y+=4;
		f+=4;
	}
}

// FINDDIFF

void finddiff(byte *x,byte *y, byte *f, byte *raw,int pix,int steps)
{
	word *ix=(word*)x;
	word *iy=(word*)y;
	word *iz=(word*)f;

	for (int count=0; count<pix; count++)
	{
		long tx=*(raw++);
		long ty=*(raw++);
		long tz=*(raw++);

    tx<<=8;
    ty<<=8;
    tz<<=8;

    tx+=128;
    ty+=128;
    tz+=128;

    tx-=ix[count*2];
    ty-=iy[count*2];
    tz-=iz[count*2];

		tx/=(float)steps;
		ty/=(float)steps;
		tz/=(float)steps;

    ix[count*2+1]=tx;
    iy[count*2+1]=ty;
    iz[count*2+1]=tz;
  }
}

// MORPH

void morph(void)
{
	seg_C buffer;
	seg_C tri(3*65536l);
  int steps;

	long lt1=(long)tri.ptr;
	long lt2=lt1+65536l*4096;
	long lt3=lt2+65536l*4096;

	byte *t1=(byte *)lt1;
	byte *t2=(byte *)lt2;
	byte *t3=(byte *)lt3;

	for (long lb=0; lb<65536l; lb++)
		t1[lb]=t2[lb]=t3[lb]=buffer.ptr[lb]=0;

	mobuffer=(long)buffer.ptr;
	mo192=lt1;

	setpalette(MORPHPAL);

  FILE *infile=fopen(MORPHDATA,"rb");
  byte ant;
  int pix;
	fread(&ant,1,1,infile);
	fread(&pix,2,1,infile);

	// First frame:
	seg_C raw(3l*pix);

	loaddata(raw.ptr,infile,pix);
	pixies=pix;
	makefirst(t1,t2,t3,raw.ptr,pix);

	ulong estart=timer.readtimer();
	float left=END_MORPH-timer.elapsed(musicstart,estart)-SLACK;
	left/=10;
	float acc=0;

	steps=SPEEDFAC*relspeed;
	steps=min(steps,96);

	loaddata(raw.ptr,infile,pix);
	finddiff(t1,t2,t3,raw.ptr,pix,steps);
	long ttt[10];
	long gaatt;
	for (int count=1; count<=ant; count++)
	{
		for (int s=0; s<steps; s++)
		{
			asm pusha
			asm push ds
			gomorph();
			drawmorph();
			asm pop ds
			asm popa
		}
		if (count<ant)
		{
			 asm pusha
			 asm push ds
			 loaddata(raw.ptr,infile,pix);
			 finddiff(t1,t2,t3,raw.ptr,pix,steps);
			 asm pop ds
			 asm popa
		}
		acc+=left;
		if (count==1 || count==ant-1)
			acc+=left;

		do
		{
			ulong now=timer.readtimer();
			gaatt=timer.elapsed(estart,now);
		} while (gaatt<acc);
	}
	fclose(infile);
}

// USEPARA

int usepara(int argc, char **argv,byte &guson)
{
  guson=0;
  int testspeed=0;
  for (int teller=1; teller<argc; teller++)
  {
    argv[teller][0]='-';
    int len=strlen(argv[teller]);
    for (int s=1; s<len; s++)
      argv[teller][s]-=32*(argv[teller][s]>'Z');
    testspeed|=
     ((!strcmp(argv[teller],"-FINDSPEED") ||
      (!strcmp(argv[teller],"-SPEED")) ||
      (!strcmp(argv[teller],"-S"))));
    loop|=
     ((!strcmp(argv[teller],"-LOOP") ||
      (!strcmp(argv[teller],"-L"))));
    guson|=
      (!strcmp(argv[teller],"-GUS"));
  }
  guson*=(!testspeed);
  return testspeed;
}

// SHOWSPEED

void showspeed(void)
{
  _AX=3;
  VGAINT;
  printf("Relative speed: %.2f\n\n",relspeed/100.0);
  quit(0);
}

// BYE

void bye(void)
{
	textmode();

	printf(
"Thanks for watching SUPERUNKNOWN by Five, And Then Some. \n\n"
"We hope you enjoyed watching it as much as we enjoyed finding\n"
"that last (???) bug...\n\n");
}

