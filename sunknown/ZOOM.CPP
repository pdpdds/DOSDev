// ZOOM.CPP /////////////////////////////////////////////////////////////////

// Thomas H.

// INCLUDES /////////////////////////////////////////////////////////////////

#include <alloc.h>
#include <stdlib.h>
#include <math.h>
#include <dos.h>
#include "stdio.h"
#include "timer.h"
#include "xgraf.h"
#include "vx.h"
#include "seg.h"

// DEFINES  /////////////////////////////////////////////////////////////////

#define HITIT		69050
#define MAXFRAMES 1564

#define FRAMES	34
#define EXT     192
#define ROUGHSPLITS 12
#define FRACX   0
#define FRACY   0
#define RGBR 3
#define RGBG 2
#define RGBB 6

// EXTERNALS ////////////////////////////////////////////////////////////////

// FUNCTIONS

extern "C" void asmzoom(void);
extern void newhandler(void);

// VARS

extern ulong musicstart;
extern long far copper_32,
       plustimer;
extern timer_C timer;
extern vx_C vx;
extern byte *pressed,
       rgb[];
extern long far farfrac1, far farfrac2, far fartwist;
extern word far fartune;
extern int far shx,
       far shy,
       far sdx,
       far sdy,
       far khx,
       far khy,
       far kdx,
       far kdy;

// VARS /////////////////////////////////////////////////////////////////////

word zoomvramptr=0xa000;
static byte *bitdata[FRAMES-1],
       *frac1b,
       *frac2b,
       *frac3b,
       *frac1,
       *frac2,
       *frac3;
word datalengths[FRAMES-1];
word innerlen,
     innerleft,
     innertop,
     innerfac,
     inneradr,
     farfac;
byte startx,
     starty,
     nearstartx,
     nearstarty,
     *globalbmp,
     *datas,
     datamask,
     lastpopped;
int csdata=0;

// FUNCTIONS ////////////////////////////////////////////////////////////////

// SETZCOLORS

void setzcolors(void)
{
	for (int count=1; count<256; count++)
	{
		float r=sin(count*0.5*RGBR*2*M_PI/256);
		float g=sin(count*0.5*RGBG*2*M_PI/256);
		float b=sin(count*0.5*RGBB*2*M_PI/256);

		r=fabs(r)*63;
		g=fabs(g)*63;
		b=fabs(b)*63;

		vx.setrgb(count,r,g,b);
	}
}

// ROUGHSPLIT

void roughsplit(byte *from, byte *to,word stripno)
{
	long from_ptr=(long)from;
	long to_ptr=(long)to;
	to_ptr+=256*stripno*(EXT/ROUGHSPLITS);
	from_ptr+=(EXT/4)+(EXT/4)*256+(256*stripno*(EXT/ROUGHSPLITS)/2);

	asm pusha
	asm push ds

	asm cld
	asm les di,to_ptr
	asm lds si,from_ptr

	asm mov bx,es
	asm mov ch,(EXT/ROUGHSPLITS)/2
nextline:
	asm mov cl,EXT/2
nextpixel:
	asm mov al,[si]
	asm inc si
	asm mov ah,al
	asm mov [es:di+256],ax
	asm mov [es:di],ax
	asm inc di
	asm inc di
	asm dec cl
	asm jnz nextpixel
	asm add di,EXT+128
	asm add si,EXT/2+64
	asm dec ch
	asm jnz nextline
	asm pop ds
	asm popa

}

// POPBIT

byte _fastcall popbit(void)
{
	asm les bx,dword ptr datas
	asm mov al,[es:bx]
	asm and al,datamask
	asm shr datamask,1
	asm jnc ok
	asm mov datamask,128
	asm inc word ptr datas
ok:
  return _AL; // Return value already in AL
}

// CTEST

void near _pascal ctest(int x,int y,byte len)
{
#ifdef NOTUNE
	return;
#endif
	static byte p,fac;
	int yy,xx;
	asm les bx,dword ptr datas
	asm mov yy,2
newx:
	asm mov xx,2
nextx:
	asm les bx,dword ptr datas
	asm mov al,[es:bx]
	asm and al,datamask
	asm shr datamask,1
	asm jnc ok
	asm mov datamask,128
	asm inc word ptr datas
ok:
	asm and al,al
	asm jnz doit
	goto staylow;
doit:
	if (len>2)
	{
		static word nx,ny;
		word len2=len;
		len2>>=1;
		nx=0;
		ny=0;
		if (xx==1)
			nx=len2;
		if (yy==1)
			ny=len2;
		ctest(x+nx,y+ny,len2);
	}
	else
	{
		byte color=0;
		asm les bx,dword ptr datas
		asm mov al,[es:bx]
		asm and al,datamask
		asm shr datamask,1
		asm jnc againok
		asm mov datamask,128
		asm inc word ptr datas
againok:
		asm and al,al
		asm jz notvip

			if (popbit())
				color=1;
			else
				color=-1;
			goto vipdone;
notvip:
			color=0;
			p=popbit();
			fac=1;
			if (popbit())	color+=fac;	fac<<=1;
			if (popbit())	color+=fac;	fac<<=1;
			if (popbit())	color+=fac;	fac<<=1;
			if (p)
				color+=2;
			else
			{
				color+=247;
				if (color==247)
				{
					fac=1;
					color=0;
					if (popbit())	color+=fac;	fac<<=1;
					if (popbit())	color+=fac;	fac<<=1;
					if (popbit())	color+=fac;	fac<<=1;
					if (popbit())	color+=fac;	fac<<=1;
					if (popbit())	color+=fac;	fac<<=1;
					if (popbit())	color+=fac;	fac<<=1;
					if (popbit())	color+=fac;	fac<<=1;
					if (popbit())	color+=fac;	fac<<=1;
				}
			}
vipdone:
		globalbmp[(x+2-xx)+((y+2-yy)<<8)]+=color;
	}
staylow:
	asm dec xx
	asm jz xdone
	asm jmp nextx
xdone:
	asm dec yy
	asm jz ydone
	asm jmp newx
ydone:
}

// FINEMASK

void finemask(byte *bits,byte *bmp,word no)
{
	static long bit_ptr;
	bit_ptr=(long)bits;
	if (!no)
		datas=(byte *)bit_ptr;
	globalbmp=bmp;
	datamask=128;
	ctest((no%6)*(EXT/6),(no/6)*(EXT/6),(EXT/6));
	while (datamask<0x80)
    popbit();
}

// CONV

word conv(int x,int y,int tune)
{
	float fx,fy;

	float nearmov=tune/48.0;
	float mov=nearmov/2;

	fx=x-FRACX-EXT/2;
	fy=y-FRACY-EXT/2;
	fx/=EXT/2.0;
	fy/=EXT/2.0;
	float fxx=fx*(EXT/2)*(1-mov)+EXT/2;
	float fyy=fy*(EXT/2)*(1-mov)+EXT/2;

	int xxx=fxx;
	int yyy=fyy;
	fxx-=xxx;
	fyy-=yyy;
	startx=255.99*fxx;
	starty=255.99*fyy;
	farfac=255.99*(1-mov);

	return yyy+EXT*xxx;
}

// SETZOOM

void setzoom(int zfac)
{
	if (!zfac)
	{
		innerleft=FRACX+EXT/4;
		innertop=FRACY+EXT/4;
		innerlen=EXT/2+1;
		innerfac=255;
		return;
	}
	float mov=zfac/48.0;
	float innerlenf=EXT/(2.0-mov);
	innerlen=innerlenf;
	float realinnerleft=FRACX+EXT/2-innerlen/2;
	float realinnertop=FRACY+EXT/2-innerlen/2;
	innertop=realinnertop;
	innerleft=realinnerleft;
	realinnerleft-=innerleft;
	realinnertop-=innertop;
	nearstartx=0;
	nearstarty=0;
	inneradr=innerleft+80*innertop;
	innerfac=255*(2.0-mov);
	innerfac-=256;
}

// DRAWZOOM

void drawzoom(long frac2,long frac1,int tune1)
{
	float v=v=M_PI*tune1/96.0;

	tune1%=48;

	float fac=1.0/(2-tune1/48.0);

	float si=sin(v);
	float co=cos(v);

	float fdx=-256*si/fac;
	float fdy=256*co/fac;

	sdx=fdx;
	sdy=fdy;
	kdx=fdx/2;
	kdy=fdy/2;

	shx=(256*96-128)-80*fdy-60*fdx;
	shy=(256*96-128)+80*fdx-60*fdy;

	khx=(256*96-128)-40*fdy-30*fdx;
	khy=(256*96-128)+40*fdx-30*fdy;

	if (tune1>45)
		tune1=45;

	asm pusha
	asm push ds
	farfrac1=frac1;
	farfrac2=frac2;
	fartune=tune1;
	asm mov ax,zoomvramptr
	asm mov es,ax
	asmzoom();
	asm pop ds
	asm popa
}

// SETUPZOOMPAGE

void setupzoompage(void)
{
	vx.blockfill(0,30000,0);
	vx.blockfill(30000,65535,0);
	vx.setoffset(0);
}


// ZOOM

// frac1=closest, center of picture
// frac2=coming up, closest next time
// frac3=under construction

void zoom(void)
{
	outport(0x3d4,0x4309);

	seg_C s1(EXT*256l);
	seg_C s2(EXT*256l);
	seg_C s3(EXT*256l);

	frac1=s1.ptr;
	frac2=s2.ptr;
	frac3=s3.ptr;

	FILE *infile=fopen(FRACTWIST,"rb");
	byte *fa=(byte*)fartwist;
	for (int count=0; count<14720; count++)
	{
		byte temp;
		fread(&temp,1,1,infile);
 		*(fa++)=temp;
	}
	fclose(infile);

	setupzoompage();

	for (word bl=0; bl<EXT*256l; bl++)
		frac1[bl]=frac2[bl]=frac3[bl]=0;

	setpalette(ZOOMPAL);

	FILE *f3;
	f3=fopen(FRACFILE6,"rb");
	for (int xx=0; xx<192; xx++)
	for (int yy=0; yy<192; yy++)
	{
		byte temp;
		fread(&temp,1,1,f3);
		frac1[xx+256l*yy]=temp;
	}
	fclose(f3);

	f3=fopen(FRACFILE7,"rb");
	fread(datalengths,2,FRAMES-1,f3);
	long sum=0;
	long forrigepos;
	FILE *o05=fopen(FRACFILE5,"rb");
	for (int su=0; su<FRAMES-1; su++)
	{
		forrigepos=sum;
		sum+=datalengths[su]+32;
		word len;
		byte *temp;
		if (sum<3l*96l*128l)
		{
			temp=(byte*)(copper_32+forrigepos);
			csdata++;
			len=datalengths[su];
		}
		else
			temp=(byte *)farmalloc((len=datalengths[su])+32);
		if (!temp)
			newhandler();
		fread(temp,1,len,o05);
		bitdata[su]=temp;
	}
	fclose(o05);
	long estart,saalangt;

	byte *fixfirst=bitdata[0];
	long left;
	float per;

	for (int t=0; t<48; t++)
	{
		if (t<12)
			roughsplit(frac1,frac2,t);
		else
			finemask(fixfirst,frac2,t-12);
	}

	long akkurat;
	long fno=0;

	float hitto=HITIT;
	for (int runde=0; runde<FRAMES-1; runde++)
	{
		byte *thisjust=bitdata[runde+1];
		for (int tune=0; tune<48; tune++)
		{
			fno++;
			if (tune<12)
				roughsplit(frac2,frac3,tune);
			else
				if (runde<FRAMES)
					finemask(thisjust,frac3,tune-12);
			setzoom(tune);
			if (!(runde+tune))
			{
				vx.setoffset((zoomvramptr-0xa000+1200)*16);
				drawzoom((long)frac1,(long)frac2,tune+48*runde);
				switchpage(0xa000,0xa000+1200,0);
			}
			else
			{
				if (!runde && tune==1)
				{
					long now,gone;
					do
					{
						now=timer.readtimer();
						gone=timer.elapsed(musicstart,now);
					} while (gone<HITIT);
					per=(END_FRACT-HITIT)/((float)MAXFRAMES);
				}
				drawzoom((long)frac1,(long)frac2,tune+48*runde);
			}

			vx.setoffset((zoomvramptr-0xa000)*16);
			zoomvramptr+=1200;
			if (zoomvramptr==0xa000+3600)
				zoomvramptr=0xa000;

			while ((akkurat=timer.elapsed(musicstart,timer.readtimer()))<hitto);
			hitto+=per;
			if (akkurat>END_FRACT)
			{
				modex();
				goto nomore;
			}
			if (fno>=MAXFRAMES)
				goto nomore;
		}
		byte *temp=frac3;
		frac3=frac1;
		frac1=frac2;
		frac2=temp;
	}

nomore:
	for (int fr=0; fr<FRAMES-1; fr++)
	{
		byte *temp=bitdata[fr];
		if(temp && fr>=csdata)
			farfree(temp);
	}
	outport(0x3d4,0x4109);
}

