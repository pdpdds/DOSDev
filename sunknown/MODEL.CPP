// MODEL.CPP ////////////////////////////////////////////////////////////////

// Thomas H.

// INCLUDES /////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <math.h>
#include <new.h>
#include <stdlib.h>
#include <conio.h>

#include "template.h"
#include "dos.h"
#include "global.h"
#include "model.h"
#include "xgraf.h"
#include "obj.h"
#include "timer.h"

// DEFINES //////////////////////////////////////////////////////////////////

#define OBK1PL	120
#define MAXLIKE 10
#define MINSPEED 200

// EXTERNALS ////////////////////////////////////////////////////////////////

// FUNCTIONS

extern void quit(int errval);
extern "C"
{
	void draw80(void);
	void draw64haze(void);
	void draw80haze(void);
	void drawradar(void);
	void dummy(void);
	void hswap(void);
	void vswap(void);
	void putimage(void);
}

// VARS

extern int relspeed,
       far idx,
       far idy,
       far inx,
       far iny,
       far idx,
       far idy,
       far idx4,
	     far idy4,
       far leftdi,
       far size14,
       far spos,
       far farx,
       far fary,
	     far farfac,
       far farno,
       far farinvfac;
extern long far spd_ptr,
       far sw_ptr,
       far FA,
       far FB,
       far FBC,
       far FAC,
       far cxmin,
       far cxmax,
       far cymin,
	     far cymax,
       far Vx,
       far Vy,
       far Nx,
       far Ny;
extern byte far sizeb,
       *pressed;
extern word far CAMx,
       far CAMy;

// VARS /////////////////////////////////////////////////////////////////////

word *sw_fp;
byte antobk[ZVALS];
long antobk_ptr=(long)antobk;

// FUNCTIONS ////////////////////////////////////////////////////////////////

// NEWHANDLER

void newhandler(void)
{
	_AX=3;
	asm int 0x10
	printf(
"Sorry - you'll need some 620Kb of conventional memory in order to run\n"
"SUPERUNKNOWN. Try removing unneccesary device drivers, and/or loading them\n"
"into high memory.\n\n");
	quit(1);
}

// CONSTRUCTOR

model_C::model_C()
{
	set_new_handler(newhandler);

	allwlen=new word[240*(2*CAMANGLES+1)];

	maxilen=new float[2*CAMANGLES+1];
	minilen=new float[2*CAMANGLES+1];
	screenseg=0xa000;
	pageno=0;
	player[0].rc=&coor[0];
	player[1].rc=&coor[1];

	calclen(OVERCAMZ,OVERCAMX,OVERMAXZ,0,240);
	for (int pl=1; pl>=0; pl--)
		calclen(CAMZ,CAMX,MAXZ,1+pl,pl?HSIZE0:HSIZE1);

	sw_fp=(word*)sw_ptr;

	for (int her=0; her<256; her++)
	{
		word adr=(her&0x1f)*8+64*(her&0xe0)+49152;
		sw_fp[her]=adr;
	}
	colormap=0;
}

// CONSTRUCTOR, CAM_C

cam_C::cam_C()
{
	bigpixels=0;
	fjellpaa=1;
	radarzoom=950;
	radaron=0;
}

// DESTRUCTOR

model_C::~model_C()
{
	if (allwlen)
		delete allwlen;
	if (maxilen)
		delete maxilen;
	if (minilen)
		delete minilen;
}

// CAM::SETANGLE

void cam_C::setangle(void)
{
	camx=CAMX;
	camz=CAMZ;
	maxz=MAXZ;
}

// LOAD

void model_C::load(char *filnavn)
{
  FILE *infile=fopen(filnavn,"rb");
	for (long t=0; t<65536l; t++)
	{
		byte temp;
		fread(&temp,1,1,infile);
		map.ptr[t]=temp;
	}

	cam[0].lastx=cam[1].lastx=cam[0].lasty=cam[1].lasty=-1000000l;
	fclose(infile);
}

// DRAW

void model_C::draw(void)
{
	drawover(player[0].rc->x,player[0].rc->y);
}

// DRAWOVER

void model_C::drawover(long xposl, long yposl)
{
	swapmap(cam[0].area.ptr,xposl,yposl,cam[0].lastx,cam[0].lasty);

	word xpos=xposl;
	word ypos=yposl;
	word angle=cam[0].angle,tempsi;
	long area_ptr=(long)cam[0].area.ptr;
	int localrel=relspeed,
	    tempdx,
      qinx[240],
			qiny[240],
			lx[240],
			ly[240],
			wx80[240],
			wy80[240];

	float vin=angle*2*M_PI/ANGLES;
	float co=cos(vin);
	float si=sin(vin);
	float dx=co*cam[0].maxl;
	float dy=si*cam[0].maxl;

	float ftemp=co;
	co=-si;
	si=ftemp;
	float nx=co*FOCAL;
	float ny=si*FOCAL;

	int wdx=dx;
  int wdy=dy;
  int wnx=nx;
	int wny=ny;
  int winx80,winy80;

  int idx,idy;
	int backx=-dx/3.0;
  int backy=-dy/3.0;

	xpos+=backx;
  ypos+=backy;

	cxmin=xposl-32767;
	cymin=yposl-32767;
	cxmax=xposl+32767;
	cymax=yposl+32767;

	for (int pre=0; pre<240; pre++)
  {
		word wfac=cam[0].wlen[pre];

    asm mov cx,wfac

    asm xor bx,bx
    asm test wdx,0x8000
    asm jz xplus
		asm inc bx
		asm neg wdx
xplus:
    asm mov ax,cx
    asm mul wdx
    asm or bx,bx
    asm jz xplusok
    asm neg dx
		asm neg wdx
xplusok:
    asm mov idx,dx

		asm xor bx,bx
    asm test wdy,0x8000
    asm jz yplus
		asm inc bx
    asm neg wdy
yplus:
    asm mov ax,cx
    asm mul wdy
    asm or bx,bx
    asm jz yplusok
    asm neg dx
    asm neg wdy
yplusok:
    asm mov idy,dx

    asm mov ax,cx
    asm imul wnx
    asm mov winx80,dx

    asm mov ax,cx
    asm imul wny
    asm mov winy80,dx

    asm xor bx,bx
		asm test wnx,0x8000
    asm jz nxplus
    asm inc bx
    asm neg wnx
nxplus:
    asm mov ax,cx
    asm mul wnx
    asm or bx,bx
		asm jz nxplusok
    asm neg dx
    asm neg wnx
nxplusok:
    asm mov winx80,dx

		asm xor bx,bx
    asm test wny,0x8000
    asm jz nyplus
    asm inc bx
    asm neg wny
nyplus:
    asm mov ax,cx
    asm mul wny
    asm or bx,bx
    asm jz nyplusok
    asm neg dx
    asm neg wny
nyplusok:
    asm mov winy80,dx

		int lxtemp=xpos+idx;
		int lytemp=ypos+idy;
		lx[pre]=lxtemp;
		ly[pre]=lytemp;

		wx80[pre]=winx80;
		wy80[pre]=winy80;

		qinx[pre]=winx80/80;
		qiny[pre]=winy80/80;
	}

	int temp160=160;
	int temp320=320;
	int temp107=107;

	int wx,wy;


	asm push ds
	asm pusha
	asm lds si,area_ptr

	_AX=screenseg;
	asm mov es,ax

	int planverdi;
	byte invers;
	int plval0=0x100+MAP_MASK;
	int plval2=0x400+MAP_MASK;
	if (localrel<MINSPEED)
	{
		plval0=0x300+MAP_MASK;
		plval2=0xc00+MAP_MASK;
	}
// PLANE 0:
		asm mov ax,plval0
		asm mov dx,SC_INDEX
		asm out dx,ax
		asm mov si,2*240-2
		asm mov di,39
next_0:
		asm mov tempsi,si
		asm mov bx,qiny[si]
		asm mov dx,ly[si]
		asm mov cx,lx[si]
		asm mov si,qinx[si]
		draw80();
		asm mov si,tempsi
		asm dec si
		asm dec si
		asm jns next_0
		if (plval0==0x300+MAP_MASK)
			goto pl2;
// PLANE 1:
		asm mov ax,0x0200+MAP_MASK
		asm mov dx,SC_INDEX
		asm out dx,ax
		asm mov si,2*240-2
		asm mov di,39
next_1:
		asm mov bx,wx80[si]
		asm mov wx,bx
		wx/=temp320;

		asm mov bx,wy80[si]
		asm mov wy,bx
		wy/=temp320;

		asm mov tempsi,si
		asm mov bx,qiny[si]
		asm mov dx,ly[si]
		asm mov cx,lx[si]
		asm mov si,qinx[si]
		asm add cx,wx
		asm add dx,wy

		draw80();
		asm mov si,tempsi
		asm dec si
		asm dec si
		asm jns next_1

// PLANE 2:
pl2:
		asm mov ax,plval2
		asm mov dx,SC_INDEX
		asm out dx,ax
		asm mov si,2*240-2
		asm mov di,39
next_2:
		asm mov bx,wx80[si]
		asm mov wx,bx
		wx/=temp160;

		asm mov bx,wy80[si]
		asm mov wy,bx
		wy/=temp160;

		asm mov tempsi,si
		asm mov bx,qiny[si]
		asm mov dx,ly[si]
		asm mov cx,lx[si]
		asm mov si,qinx[si]
		asm add cx,wx
		asm add dx,wy
		draw80();
		asm mov si,tempsi
		asm dec si
		asm dec si
		asm jns next_2
		if (plval0==0x300+MAP_MASK)
			goto pl4;
// PLANE 3:
		asm mov ax,0x0800+MAP_MASK
		asm mov dx,SC_INDEX
		asm out dx,ax
		asm mov si,2*240-2
		asm mov di,39
next_3:
		asm mov bx,wx80[si]
		asm mov wx,bx
		wx/=temp107;

		asm mov bx,wy80[si]
		asm mov wy,bx
		wy/=temp107;
    asm mov tempsi,si
		asm mov bx,qiny[si]
		asm mov dx,ly[si]
		asm mov cx,lx[si]
		asm mov si,qinx[si]
		asm add cx,wx
		asm add dx,wy
		draw80();
		asm mov si,tempsi
		asm dec si
		asm dec si
		asm jns next_3
pl4:
	asm popa
	asm pop ds
}

// NEWPAGE

void model_C::newpage(void)
{
	yoffset((screenseg-0xa000)/5);
	screenseg+=1200;
	pageno++;
	if (screenseg==0xa000+3600)
	{
		screenseg=0xa000;
    pageno=0;
	}
}

// CALCLEN

void model_C::calclen(float camz,float camx, float maxz, word camangle,
	word h)
{
	float len;
	for (int count=h-1; count>=0; count--)
	{
		float dx=camx-((float)maxz*count/h);
		len=camx*(camz/dx);

		if (count==h-1)
			maxilen[camangle]=len;
		float lenfac=(len-camx)/(maxilen[camangle]-camx);
		int ilenfac=(lenfac*lenfac*7.99);
		int minlen=len;
		len/=maxilen[camangle];
		allwlen[240*camangle+count]=65535.99*len;
		fit(0,ilenfac,7);
		if (!count)
			minilen[camangle]=minlen;
	}
}

// CAMANGLE

void model_C::camangle(int nview)
{
	view=nview;
	for (int count=0; count<2; count++)
	{
		cam[count].setangle();
		cam[count].maxl=maxilen[view];
		cam[count].wlen=&allwlen[240*view];
	}
}

// SWAPMAP

void model_C::swapmap(byte *ptr,long nyx,long nyy, long& oldx,long& oldy)
{
	long ds00=(long)map.ptr;

	nyx-=32768;
	nyy-=32768;

	nyx/=256;
	nyy/=256;

	int nyxw=nyx;
	int nyyw=nyy;

		asm push ds

		asm lds ax,ds00

		for (int y=oldy+256; y<nyy+256; y++)
		{
			word yb=y<<8;
			byte xb=nyx;
			long esbx=(long)&ptr[xb+yb];
			asm les bx,esbx
			asm mov dx,nyxw
			asm mov cx,y
			hswap();
		}

		for (y=nyy; y<oldy; y++)
		{
			word yb=y<<8;
			byte xb=nyx;
			long esbx=(long)&ptr[xb+yb];
			asm les bx,esbx
			asm mov dx,nyxw
			asm mov cx,y
			hswap();
		}

		for (int x=oldx+256; x<nyx+256; x++)
		{
			word yb=nyy<<8;
			byte xb=x;
			long esbx=(long)&ptr[xb+yb];
			asm les bx,esbx
			asm mov dx,x
			asm mov cx,nyyw
			vswap();
		}
		for (x=nyx; x<oldx; x++)
		{
			word yb=nyy<<8;
			byte xb=x;
			long esbx=(long)&ptr[xb+yb];
			asm les bx,esbx
			asm mov dx,x
			asm mov cx,nyyw
			vswap();
		}

		asm pop ds

	oldx=nyx;
	oldy=nyy;
}

// SETPOS

void model_C::setpos(int camno,long nyx,long nyy)
{
	long ds00=(long)map.ptr;

	nyx-=32768;
	nyy-=32768;

	nyx>>=8;
	nyy>>=8;

	word nyxw=nyx;

	asm push ds

	asm lds ax,ds00

	long ptr=(long)cam[camno].area.ptr;

	for (int y=nyy; y<nyy+256; y++)
	{
		word yb=y<<8;
		byte xb=nyx;
		word temp=xb+yb;
		long esbx=ptr+temp;
		asm les bx,esbx
		asm mov dx,nyxw
		asm mov cx,y
		hswap();
	}

	asm pop ds

	cam[camno].lastx=nyx;
	cam[camno].lasty=nyy;
}

