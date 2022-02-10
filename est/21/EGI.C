#define _EGI

#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <string.h>
#include <dir.h>
#include <mem.h>
#include <dos.h>
#include <limits.h>

#include "est.h"
#include "21.h"
#include "proto.h"
#include "graph.h"

#define BIT_1 	0x80
#define BIT_2   0x40
#define BIT_3   0x20
#define BIT_4   0x10
#define BIT_5   0x08
#define BIT_6   0x04
#define BIT_7   0x02
#define BIT_8   0x01
#define NORM_VID  0
#define REV_VID   1
#define TRUE      0xFF
#define INDEXREGS 0x3CE
#define VALREGS 	0x3CF
#define ENABLE  	0x0F

#define BIT_0		0x80;

#define WPOINT(x, y) {*(vid_addr+offsetx[x]+(y>>3)) |= bit_table[y&0x0007];}
#define BPOINT(x, y) {*(vid_addr+offsetx[x]+(y>>3)) &= bit_table_1[y&0x0007];}
#define XORPOINT(x, y){*(vid_addr+offsetx[x]+(y>>3)) ^= bit_table[y&0x0007];}

typedef struct {
	BYTE bit0 : 1 ;
	BYTE bit1 : 1 ;
	BYTE bit2 : 1 ;
	BYTE bit3 : 1 ;
	BYTE bit4 : 1 ;
	BYTE bit5 : 1 ;
	BYTE bit6 : 1 ;
	BYTE bit7 : 1 ;
} BITS;

typedef union {
	BITS b;
	BYTE c;
} BIT;

BYTE *vid_addr;
WORD near offsetx[600];

int MaxX, MaxY;
int bminx=0, bminy=0, bmaxx, bmaxy;
int gminx=0, gminy=0, gmaxx, gmaxy;
WORD work_area_size;
BYTE old_gmode;
int graph_mode=-1;
BYTE color_mode=0;
BYTE fontc[3];
BYTE curc[3];

BYTE near bit_table[]={0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
BYTE near bit_table_1[]={0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0xfe};
BYTE near fill_bit[]={0xff, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01};
BYTE near fill_bit_1[]={0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF};
BYTE near dot[8]={0x00, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x7e, 0x00};
BYTE near box[8]={0x81, 0, 0, 0, 0, 0, 0, 0x81};
BYTE near image_bit[]={0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe};
BYTE near image_bit_1[]={0xff, 0x7f, 0x3f, 0x1f, 0x0f, 0x07, 0x03, 0x01};

extern BYTE cur_w;
extern text_window tw[];
extern popup_window pop[];
extern BYTE rev_flag;

void vPointThick(int x, int y, BYTE mode)
{
	WPOINT(x, y);
	if(mode>1) {
		WPOINT(x+1, y);
		WPOINT(x, y+1);
		WPOINT(x+1, y+1);
	}
	if(mode>2) {
		WPOINT(x, y+2);
		WPOINT(x+1, y+2);
		WPOINT(x+2, y);
		WPOINT(x+2, y+1);
		WPOINT(x+2, y+2);
	}
	if(mode>3) {
		WPOINT(x, y+3);
		WPOINT(x+1, y+3);
		WPOINT(x+2, y+3);
		WPOINT(x+3, y);
		WPOINT(x+3, y+1);
		WPOINT(x+3, y+2);
		WPOINT(x+3, y+3);
	}
}

void vPointNormal(int x, int y, BYTE mode)
{
	BPOINT(x, y);
	if(mode>1) {
		BPOINT(x+1, y);
		BPOINT(x, y+1);
		BPOINT(x+1, y+1);
	}
	if(mode>2) {
		BPOINT(x, y+2);
		BPOINT(x+1, y+2);
		BPOINT(x+2, y);
		BPOINT(x+2, y+1);
		BPOINT(x+2, y+2);
	}
	if(mode>3) {
		BPOINT(x, y+3);
		BPOINT(x+1, y+3);
		BPOINT(x+2, y+3);
		BPOINT(x+3, y);
		BPOINT(x+3, y+1);
		BPOINT(x+3, y+2);
		BPOINT(x+3, y+3);
	}
}

BYTE bGetGraphMode(void)
{
	union REGS r;

	r.h.ah=0x0f;

	return int86(0x10, &r, &r) & 255 ;
}

void vSetHercGraphMode(BYTE flag)
{
	char count;
	WORD *p;
	WORD i;

	char scr_data[][13] ={
	{	0x31, 0x28, 0x29, 0x08, 0x68, 0x02, 0x64,
		0x65, 0x02, 0x03, 0x02, 0x01, 0x0e },
	{ 0x35, 0x2d, 0x2e, 0x07, 0x5b, 0x02, 0x57,
		0x57, 0x02, 0x03, 0x00, 0x00, 0x0a }};


	p=MK_FP(0xb000, 0x0000);
	for(i=0;i<16384;i++) {
		*p++=0;
	}
	outportb( 0x03bf, 0x03);
	outportb( 0x03b8, 0x00);
	for ( count = 0; count < 12; count++) {
		outportb( 0x03b4, count);
		outportb( 0x03b5, *( scr_data[flag] + count));
	}
	outportb( 0x03b8, *( scr_data[flag] + 12));
}



void vSetPalette(BYTE s, BYTE *pal)
{
	union REGS rr;
	BYTE p[3];

	if(graph_mode<VGA640) {
		if(s==63) s=15;
	}
	else {
		if(s==15) s=63;
	}
	memcpy(p, pal, 3);
//	if(graph_mode<CGA200) return;
	rr.h.ah=0x10;
	if(graph_mode<VGA640) {
		p[0]/=21;
		p[1]/=21;
		p[2]/=21;
		rr.h.al=0;
		rr.h.bl=s;
		rr.h.bh=((p[0]&1)<<5) | ((p[1]&1)<<4) | ((p[2]&1)<<3) |
						((p[0]&2)<<1) | ((p[1]&2)) | ((p[2]&2)>>1);
	}
	else {
		rr.h.al=0x10;
		rr.x.bx=s;
		rr.h.dh=p[0];  //r
		rr.h.ch=p[1];  //g
		rr.h.cl=p[2];  //b
	}
	int86(0x10, &rr, &rr);
}

void vInitOffsetX(BYTE flag, WORD width)
{
	register WORD i;

//	offsetx=(WORD *)malloc(600*2);
	if(flag==0)	for(i=0; i<600; i++) offsetx[i]=0x2000*(i&3) + width*(i>>2);
	else if(flag==1) for(i=0; i<600; i++) offsetx[i]=i*width;
	else if(flag==2) {
		for(i=0; i<300; i+=2) {
			offsetx[i]=0x4000*(i&2) + width*(i>>2);
			offsetx[i+1]=0x2000*(i&2) + width*(i>>2);
		}
	}
	else for(i=0; i<600; i++) offsetx[i]=0x2000*(i&1) + width*(i>>1);
}

int iSetVideoMode(BYTE flag)
{
	union REGS inr;
	union REGS outr;
	int success = ON;

	disable();
	inr.h.ah = 0;
	inr.h.al = flag;
	int86( 0x10, &inr, &outr );
	inr.h.ah = 0xf;
	int86( 0x10, &inr, &outr );
	if( flag != outr.h.al )	{
			success = OFF;
	}
	enable();

	return success;
}

video_init vit[]={
	{VGA640, 0x0012, 479, 639, 0xa000, 1, 80},  // This is reserved!!!
	{HERCMONO1 | 0xc0, HERCGRAPHICS, 347, 719, 0xb000, 0, 90},
	{HERCMONO2 | 0xc0, HERCGRAPHICS, 399, 639, 0xb000, 0, 80},
//	{CGA200, 0x0006, 199, 639, 0xb800, 2, 80},
//	{EGA200, 0x000e, 199, 639, 0xa000, 1, 80},
	{EGA350, 0x0010, 349, 639, 0xa000, 1, 80},
//	{EGAMONO | 0x40, 0x000f, 349, 639, 0xa000, 1, 80},
	{VGA640, 0x0012, 479, 639, 0xa000, 1, 80},
	{VGAMONO | 0x40, 0x0011, 479, 639, 0xa000, 1, 80},
	{ET800 | 0x80, 0x0029, 599, 799, 0xa000, 1, 100},
	{TR800 | 0x80, 0x005B, 599, 799, 0xa000, 1, 100},
	{REALTEK800 | 0x80, 0x001F, 599, 799, 0xa000, 1, 100},
	{ATI800 | 0x80, 0x0054, 599, 799, 0xa000, 1, 100},
	{SPC | 0xc0, 0x16, 399, 639, 0xb800, 0, 80},
	{CORONA | 0xc0, 0x61, 399, 639, 0xb800, 0, 80},
	{MIGHTY | 0xc0, 0x6, 399, 639, 0xb800, 2, 80},
	{PARTNER | 0xc0, 0x40, 399, 639, 0xb800, 0, 80},
	{TELEVIDEO | 0xc0, 0x09, 399, 639, 0xb800, 0, 80},
	{TOSHIBA | 0xc0, 0x74, 399, 639, 0xb800, 0, 80},
	{PC24 | 0xc0, 0x69, 399, 639, 0xb800, 0, 80},
	{AT_T | 0xc0, 0x69, 399, 639, 0xb800, 0, 80},
};

int iInitGraphMode(BYTE g_mode, BYTE first_next)
{
	int i;

	old_gmode=bGetGraphMode();

	for(i=0;i<MAXVIDEOKIND;i++) {
		if(g_mode==(vit[i].kind & 0x3f)) {
			if(vit[i].func_num!=HERCGRAPHICS) {
				if(iSetVideoMode(vit[i].func_num)!=ON) return 0;
			}
			else {
				rev_flag=ON;
				if(vit[i].para2==90) vSetHercGraphMode(1);
				else vSetHercGraphMode(0);
			}
			if(g_mode==SPC || g_mode==MIGHTY)	outportb( 0x03df, 0xbe );
			MaxX=vit[i].mx;
			MaxY=vit[i].my;
			vid_addr=(BYTE far *)MK_FP(vit[i].vid_seg, 0);
			vInitOffsetX(vit[i].para1, vit[i].para2);
			graph_mode=g_mode;
			if((vit[i].kind & 0x40)) color_mode=0;
			else color_mode=1;
			if(vit[i].kind & 0x80) pokeb(0x0040, 0x0049, 6);
			if(!first_next) {
				gmaxx=bmaxx=MaxX;
				gmaxy=bmaxy=MaxY;
			}
			work_area_size=(MaxX+1)*((MaxY+7)>>3);
			return 1;
		}
	}
	return 0;
}

void vDeinitGraphMode(void)
{
	union REGS r;
	BYTE p[6]={0, 0, 0, 63, 63, 63};

	if(graph_mode>2) {
		vSetPalette(0, p);
		vSetPalette(63, p+3);
	}
	r.h.ah=0;
	r.h.al=old_gmode;
	int86(0x10, &r, &r);
}


void vGraphClipOff(void)
{
	gminx=gminy=0;
	gmaxy=MaxY;
	gmaxx=MaxX;
}

void vGraphClipOn(int x, int y, int ex, int ey)
{
	gminx=x;
	gminy=y;
	gmaxx=ex;
	gmaxy=ey;
}

void vNLineXBit(int startx, int endx, int y, BYTE mode)
{
	int inc;

	if(y<gminy || y>gmaxy) return;
	if(startx<gminx) startx=gminx;
	else if(startx>gmaxx) startx=gmaxx;
	if(endx<gminx) endx=gminx;
	else if(endx>gmaxx) endx=gmaxx;

	if(startx<endx+1) inc=1;
	else inc=-1;
	endx++;
	if(mode==1) {
		for(;startx<endx;startx+=inc) WPOINT(startx, y);
	}
	else if(mode==0) {
		for(;startx<endx;startx+=inc) BPOINT(startx, y);
	}
	else {
		for(;startx<endx;startx+=inc) XORPOINT(startx, y);
	}
}

void vWLineXBit(int x, int endx, int y)
{
	vNLineXBit(x, endx, y, 1);
}

void vBLineXBit(int x, int endx, int y)
{
	vNLineXBit(x, endx, y, 0);
}

void vXORLineXBit(int x, int endx, int y)
{
	vNLineXBit(x, endx, y, 2);
}

void vWLineYBit(int x, int y, int endy)
{
	int i;
	BYTE *offset;
	BYTE sByte, eByte;

	if (endy<y) {
		i = y;
		y =endy ;
		endy = i;
	}

	if(x<gminx || x>gmaxx) return;
	if(y<gminy) y=gminy;
	if(endy>gmaxy) endy=gmaxy;

	sByte = y>>3;
	eByte = endy>>3;

	offset=vid_addr+offsetx[x];
	if(sByte == eByte) {
		*(offset+sByte) |= (fill_bit[y&7] & fill_bit_1[endy&7]);
	}
	else {
		*(offset+sByte) |= (fill_bit[y & 7]);
		*(offset+eByte) |= (fill_bit_1[endy & 7]);
		for(i= sByte+1 ; i<eByte ; ++i) {
			*(offset + i)=0xff;
		}
	}
}

void vBLineYBit(int x, int y, int endy)
{
	int i;
	BYTE *offset;
	BYTE sByte, eByte;

	if (endy<y) {
		i = y;
		y =endy ;
		endy = i;
	}

	if(x<gminx || x>gmaxx) return;
	if(y<gminy) y=gminy;
	if(endy>gmaxy) endy=gmaxy;

	sByte = y>>3;
	eByte = endy>>3;

	offset=vid_addr+offsetx[x];
	if(sByte == eByte) {
		*(offset+sByte) &= ~(fill_bit[y&7] & fill_bit_1[endy&7]);
	}
	else {
		*(offset+sByte) &= ~(fill_bit[y & 7]);
		*(offset+eByte) &= ~(fill_bit_1[endy & 7]);
		for(i= sByte+1 ; i<eByte ; ++i) {
			*(offset + i)=0;
		}
	}
}

void vXORLineYBit(int x, int y, int endy)
{
	int i;
	BYTE *offset;
	BYTE sByte, eByte;

	if (endy<y) {
		i = y;
		y =endy ;
		endy = i;
	}

	sByte = y>>3;
	eByte = endy>>3;

	if(x<gminx || x>gmaxx) return;
	if(y<gminy) y=gminy;
	if(endy>gmaxy) endy=gmaxy;

	offset=vid_addr+offsetx[x];
	if(sByte == eByte) {
		*(offset+sByte) ^= (fill_bit[y&7] & fill_bit_1[endy&7]);
	}
	else {
		*(offset+sByte) ^= (fill_bit[y & 7]);
		*(offset+eByte) ^= (fill_bit_1[endy & 7]);
		for(i= sByte+1 ; i<eByte ; ++i) {
			*(offset + i)^=0xff;
		}
	}
}

void vNLineYBit(int x, int y, int ey, BYTE mode)
{
	if(mode==1) vWLineYBit(x, y, ey);
	else if(mode==0) vBLineYBit(x, y, ey);
	else vXORLineYBit(x, y, ey);
}

#define absol(a) {if(a&0x8000) a=(a^0xffff)+1;}

void vLineBit(int startx,int starty,int endx,int endy, BYTE mode)
{
	register int t,distance;
	int xerr=0,yerr=0,delta_x=0,delta_y=0;
	int incx, incy;

	delta_x= startx - endx;
	delta_y= starty - endy;

	if(delta_x>0) incx = 1;
	else if( delta_x == 0) incx =0;
	else incx = -1;

	if(delta_y>0) incy = 1;
	else if( delta_y == 0) incy =0;
	else incy = -1;

	absol(delta_x);
	absol(delta_y);

	if( delta_x > delta_y) distance = delta_x;
	else distance = delta_y;

	for( t=0; t<distance+1; t++) {
		if(endx>gminx && endx<gmaxx && endy>gminy && endy<gmaxy) {
			if(mode==1) { WPOINT(endx,endy); }
			else if(mode==0) { BPOINT(endx, endy); }
			else { XORPOINT(endx, endy); }
		}
		xerr += delta_x;
		yerr += delta_y;
    if( xerr > distance) {
			xerr -=distance;
			endx +=incx;
		}
    if( yerr > distance) {
			yerr -=distance;
			endy +=incy;
		}
	}
}

void vNBoxBit(int x, int y, int ex, int ey, BYTE mode)
{
	vNLineXBit(x, ex, y, mode);
	vNLineXBit(x, ex, ey, mode);
	if(mode==1) {
		vWLineYBit(x, y, ey);
		vWLineYBit(ex, y, ey);
	}
	else if(mode==0) {
		vBLineYBit(x, y, ey);
		vBLineYBit(ex, y, ey);
	}
	else {
		vXORLineYBit(x, y, ey);
		vXORLineYBit(ex, y, ey);
	}
}

void vWBoxBit(int startx,int starty,int endx,int endy)
{
	vNBoxBit(startx, starty, endx, endy, 1);
}

void vBBoxBit(int startx,int starty,int endx,int endy)
{
	vNBoxBit(startx, starty, endx, endy, 0);
}

void vXORBoxBit(int startx,int starty,int endx,int endy)
{
	vNBoxBit(startx, starty, endx, endy, 2);
}

void vNBarBit(int x, int y, int ex, int ey, BYTE mode)
{
	for (;x<ex+1;x++)
		vNLineYBit(x, y, ey, mode);
}

void vWBarBit(int startx,int starty,int endx, int endy)
{
	vNBarBit(startx, starty, endx, endy, 1);
}

void vBBarBit(int x,int y,int ex, int ey)
{
	vNBarBit(x, y, ex, ey, 0);
}

void vXORBarBit(int x, int y, int ex, int ey)
{
	vNBarBit(x, y, ex, ey, 2);
}

void vXORBarByte(int x, int y, int h, int len)
{
	BYTE *offset;
	register WORD i, j;

	for(i=x; i<x+h; i++) {
		offset=vid_addr+offsetx[i]+y;
		for(j=0; j<len; j++) *offset++^=0xff;
	}
}

void vXORBarBytex16(int x, int y, int len)
{
	vXORBarByte(x, y, 16, len);
}

void vXORBlockByte(int x, int y, BYTE height, BYTE width)
{
	char *offset;
	register WORD i, j;
	int ex;

	ex=x+height;
	if(y+width<(bminy>>3)+1) return;
	if(ex<bminx+1) return;
	if(x>bmaxx) return;
	if(y>bmaxy>>3) return;
	if(x<bminx+1) {
		x=bminx+1;
	}
	if(ex>bmaxx) ex=bmaxx;
	if(y<(bminy>>3)+1) y=(bminy>>3)+1;
	if(y+width>bmaxy>>3) width=(bmaxy>>3)-y;

	for(i=x; i<ex; i++) {
		offset=vid_addr+offsetx[i]+y;
		for(j=0; j<width; j++) *offset++^=0xff;
	}
}


void vPutFreeImageByte(int x, int y, WORD hsize, WORD vsize, BYTE *image, BYTE mode)
{
	register WORD i, j, count;
	BYTE *offset;

	vsize>>=3;
	if(mode==0) {
		for(i=0, count=0; i<hsize;i++) {
			offset=vid_addr+offsetx[x]+y;
			for(j=0; j<vsize; j++) {
				*offset++=image[count++];
			}
			x++;
		}
	}
	else {
		for(i=0, count=0; i<hsize;i++) {
			offset=vid_addr+offsetx[x]+y;
			for(j=0; j<vsize; j++) {
				*offset++=~image[count++];
			}
			x++;
		}
	}
}

void vPutWordImageBit(int x, int y, BYTE *buf)
{
	register WORD i;
	BYTE restr, restl;
	BYTE *offset;

	restl=y & 0x0007;
	restr=8-restl;
	y=y>>3;
	for(i=0 ;i<32;) {
		offset=vid_addr+offsetx[x++]+y;
		*offset &= (0xff << restr);
		*offset++ |= buf[i] >> restl;
		*offset = buf[i++] << restr;
		*offset++ |= buf[i] >> restl;
		*offset &= (0xff >> restl);
		*offset |= buf[i++] << restr;
	}
}

WORD wSizeImageBuf(int x, int y, int endx, int endy)
{
	return (WORD)((endx-x+1)*(endy-y+1));
}

void vDrawPanelByte(WORD x, WORD y, WORD endx, WORD endy,BYTE flag)
{
	
	WORD sy,ey;

	sy=y*8; ey=endy*8;
	if(flag==1) {         // meaning white background
		vWBarBit(x,sy,endx,ey-1);
		vBLineXBit(x,endx,sy);
		vBLineYBit(x,sy,ey);
		vBLineYBit(endx,sy,ey);
		vBLineXBit(x,endx,ey);
		vBLineXBit(x+1,endx+1,ey+1);
		vBLineXBit(x+2,endx+2,ey+2);
		vBLineYBit(endx+1,sy+1,ey+1);
		vBLineYBit(endx+2,sy+2,ey+2);
	}
	else {
		vBBarBit(x,sy,endx,ey-1);
		vWLineXBit(x,endx,sy);
		vWLineYBit(x,sy,ey);
		vWLineYBit(endx,sy,ey);
		vWLineXBit(x,endx,ey);
		vWLineXBit(x+1,endx+1,ey+1);
		vWLineXBit(x+2,endx+2,ey+2);
		vWLineYBit(endx+1,sy+1,ey+1);
		vWLineYBit(endx+2,sy+2,ey+2);
	}
}


void vChangeColor(BYTE cur_color, BYTE new_color)
{
	BYTE t[3];

	t[0]=((new_color>>5) | ((new_color>>1) & 2))*21;
	t[1]=(((new_color>>4) & 1) | ((new_color) & 2))*21;
	t[2]=(((new_color>>3) & 1) | ((new_color<<1) & 2))*21;
	vSetPalette(cur_color, t);
}


void vSetColors(void)
{
	BYTE c[3]={32, 32, 32};

	vSetPalette(56, c);
//	vSetPalette(7, 48, 48, 48);
	vSetPalette(0, curc);
	vSetPalette(63, fontc);
}

void vSetBlackWhite(void)
{
	BYTE w[3]={63, 63, 63};
	BYTE b[3]={0, 0, 0};
	vSetPalette(0, b);
	vSetPalette(63, w);
}

void vFillScreen(unsigned first, unsigned second)
{
	unsigned vseg, voff;
	unsigned count, count2, d, width;

	vseg=FP_SEG(vid_addr);
	voff=FP_OFF(vid_addr);

	if(vit[graph_mode].vid_seg == 0xa000) {
		count=(MaxX+1)/2;
		d=(MaxY+1)/16;
		asm MOV AX, vseg;
		asm MOV ES, AX;
		asm MOV DI, voff;

		asm	CLD;
		LOOP:
		asm MOV CX, d;
    asm MOV AX, first;
		asm	REP STOSW;
		asm MOV CX, d;
		asm	MOV AX, second;
		asm	REP STOSW;

		asm DEC count;
		asm CMP count, 0;
		asm JNE LOOP;
	}
	else {
		width=(MaxY>>4)+1;
		count=(MaxX+1)/4;//*graph_mode;
		count2=0;
		asm MOV AX, vseg;
		asm MOV ES, AX;
		asm MOV DI, voff;

		LOOP2:
		asm	CLD;
		d=count2*width*2;
    asm MOV AX, first;
		asm MOV DI, d;
		asm MOV CX, width;
		asm	REP STOSW;
		asm	MOV AX, second;
		asm ADD DI, 0x2000;
		asm SUB DI, width;
		asm SUB DI, width;
		asm MOV CX, width;
		asm	REP STOSW;
    asm MOV AX, first;
		asm ADD DI, 0x2000;
		asm SUB DI, width;
		asm SUB DI, width;
		asm MOV CX, width;
		asm	REP STOSW;
		asm	MOV AX, second;
		asm ADD DI, 0x2000;
		asm SUB DI, width;
		asm SUB DI, width;
		asm MOV CX, width;
		asm	REP STOSW;

		asm INC count2;
		asm DEC count;
		asm CMP count, 0;
		asm JNE LOOP2;
	}
}

void vBarByte(int x, int y, int endx, int endy, char attr)
{
	unsigned vseg;
	unsigned voff;
	int i;

	endx++;
	if(x<0) x=0;
	if(endx>MaxX) endx=MaxX;
	if(y<0) y=0;
	if(endy>MaxY) endy=MaxY;
	endy-=y;
	vseg=FP_SEG(vid_addr);

	if(attr==1) {
		for(i=x;i<endx;i++) {
			voff=offsetx[i]+y;
			asm MOV AX, vseg;
			asm MOV ES, AX;
			asm	MOV DI, voff;
			asm MOV CX, endy;
			asm	MOV AL, 0xff;
			asm	CLD;
			asm	REP STOSB;
		}
	}
	else {
		for(i=x;i<endx;i++) {
			voff=offsetx[i]+y;
			asm MOV AX, vseg;
			asm MOV ES, AX;
			asm	MOV DI, voff;
			asm MOV CX, endy;
			asm	MOV AL, 0;
			asm	CLD;
			asm	REP STOSB;
		}
	}
}


void vScrollRightByte(unsigned x, unsigned endx, unsigned y,
									unsigned widthy, unsigned count, unsigned char attr)
{
	unsigned offsets;
	unsigned offsetd;
	unsigned srcy;
	unsigned endy;
	unsigned vseg;

	if(widthy<count) {
		vBarByte(x, y, endx, y+widthy, attr);
		return;
	}
	endy=y+widthy-1;
	srcy=endy-count;
	widthy-=count;

	vseg=FP_SEG(vid_addr);

	for(;x<endx;x++) {
		offsets=offsetx[x]+srcy;
		offsetd=offsetx[x]+endy;
		asm PUSH DS;
		asm PUSH SI;
		asm MOV AX, vseg;
		asm MOV ES, AX; 
		asm MOV DS, AX; 
		asm MOV DI, offsetd;
		asm MOV SI, offsets;
		asm MOV CX, widthy; 
		asm STD;
		asm REP MOVSB;

		asm MOV AX, offsetd;
		asm SUB AX, widthy;
		asm SUB AX, count;
		asm INC AX;
		asm MOV DI, AX;
		asm MOV CX, count;

		asm CMP attr, 0;
		asm JNE label_ff;
		asm MOV AL, 0;
		asm JMP label_0;
label_ff:
    asm MOV AL, 0xff;
label_0:
		asm CLD;
		asm REP STOSB;
		asm POP SI;
		asm POP DS;
	}
}

void vScrollLeftByte(unsigned x, unsigned endx, unsigned y,
									unsigned widthy, unsigned count, unsigned char attr)
{
	unsigned offsets;
	unsigned offsetd;
	unsigned desty;
	unsigned vseg;

	if(widthy<count) {
		vBarByte(x, y, endx, y+widthy, attr);
		return;
	}
	desty=y+count;
	widthy-=count;

	vseg=FP_SEG(vid_addr);

	for(;x<endx;x++) {
		offsetd=offsetx[x]+y;
		offsets=offsetx[x]+desty;
		asm PUSH DS;
		asm PUSH SI;
		asm MOV AX, vseg;
		asm MOV ES, AX; 
		asm MOV DS, AX; 
		asm MOV DI, offsetd;
		asm MOV SI, offsets;
		asm MOV CX, widthy; 
		asm CLD;
		asm REP MOVSB;

		asm MOV AX, offsetd;
		asm ADD AX, widthy
		asm MOV DI, AX;
		asm MOV CX, count;

		asm CMP attr, 0;
		asm JNE label_ff;
		asm MOV AL, 0;
		asm JMP label_0;
label_ff:
		asm MOV AL, 0xff;
label_0:
		asm CLD;
		asm REP STOSB;
		asm POP SI;
		asm POP DS;
	}
}


void vScrollUpByte(unsigned x, unsigned endx, unsigned y,
									unsigned widthy, unsigned count)
{
	unsigned offsets;
	unsigned offsetd;
	unsigned sourx;
	unsigned vseg;


	sourx=x+count;
	endx=endx-count;

	vseg=FP_SEG(vid_addr);

	for(;x<endx;x++, sourx++) {
		offsetd=offsetx[x]+y;
		offsets=offsetx[sourx]+y;
		asm PUSH DS;
		asm PUSH SI;
		asm MOV AX, vseg;
		asm MOV ES, AX; 
		asm MOV DS, AX; 
		asm MOV AX, offsetd;
		asm MOV DI, AX;
		asm MOV AX, offsets;
		asm MOV SI, AX;
		asm MOV CX, widthy; 
		asm CLD;
		asm REP MOVSB;
		asm POP SI;
		asm POP DS;
	}

}

void vScrollDownByte(unsigned x, unsigned endx, unsigned y,
									unsigned widthy, unsigned count)
{
	unsigned offsets;
	unsigned offsetd;
	unsigned sourx;
	unsigned vseg;


	sourx=endx-count;
	x=x+count;

	vseg=FP_SEG(vid_addr);

	for(;endx>x;endx--, sourx--) {
		offsets=offsetx[sourx]+y;
		offsetd=offsetx[endx]+y;
		asm PUSH DS;
		asm PUSH SI;
		asm MOV AX, vseg;
		asm MOV ES, AX; 
		asm MOV DS, AX; 
		asm MOV AX, offsetd;
		asm MOV DI, AX;
		asm MOV AX, offsets;
		asm MOV SI, AX;
		asm MOV CX, widthy; 
		asm CLD;
		asm REP MOVSB;
		asm POP SI;
		asm POP DS;
	}

}

void vPutColorImageLByte(int x, int y, int h, int w, int c, BYTE *buf)
{
	int i, j, count;
	BYTE *offset;

	OUTPUT(0, c);
	OUTPUT(1, 0x0f);
	for(i=x, count=0;i<x+h;i++) {
		offset=vid_addr+offsetx[i]+y;
		for(j=0 ; j<w ; j++) {
			OUTPUT(8, buf[count]);
			*offset++ &= ~(buf[count++]);
		}
	}
	OUTPUT(0, 0);
	OUTPUT(1, 0);
	OUTPUT(3, 0);
	OUTPUT(8, 0xff);
}

BYTE winicon[2][54]={
{ 0x60,0x00,0x00,0x6F,0xFF,0xFE,0x6F,0xFF,0xFE,0x6F,
	0xFF,0xFE,0x6F,0xFF,0xFE,0x6F,0xFF,0xFE,0x6F,0xFF,
	0xFE,0x6E,0x00,0x0E,0x6E,0xFF,0xE6,0x6E,0xFF,0xE6,
	0x6E,0x00,0x06,0x6F,0x00,0x06,0x6F,0xFF,0xFE,0x6F,
	0xFF,0xFE,0x6F,0xFF,0xFE,0x6F,0xFF,0xFE,0x6F,0xFF,
	0xFE,0x6F,0xFF,0xFE},
{	0x00,0x00,0x03,0xF7,0xFF,0xFB,0xF7,0xFF,0xFB,0xF7,
	0xFF,0xFB,0xF7,0xFF,0xFB,0xF7,0xFF,0xFB,0xF7,0xFF,
	0xFB,0xF7,0xF3,0xFB,0xF7,0xE1,0xFB,0xF7,0xC0,0xFB,
	0xF7,0x80,0x7B,0xF7,0x00,0x3B,0xF7,0xFF,0xFB,0xF7,
	0xFF,0xFB,0xF7,0xFF,0xFB,0xF7,0xFF,0xFB,0xF7,0xFF,
	0xFB,0xF7,0xFF,0xFB},
};

BYTE winicon2[9][32]={
 {0x3F,0xFB,0x40,0x07,0x5F,0xF7,0x5E,0xF7,0x5C,0x77,0x58,0x37,
	0x50,0x17,0x5C,0x77,0x5C,0x77,0x5C,0x77,0x5C,0x77,0x5C,0x77,
	0x5F,0xF7,0x5F,0xF7,0x40,0x07,0x3F,0xFB,},
 {0x3F,0xFB,0x40,0x07,0x5F,0xF7,0x5F,0xF7,0x5F,0xF7,0x5F,0xF7,
	0x5F,0xF7,0x5F,0xF7,0x5F,0xF7,0x5F,0xF7,0x5F,0xF7,0x5F,0xF7,
	0x5F,0xF7,0x5F,0xF7,0x40,0x07,0x3F,0xFB,},
 {0x3F,0xFB,0x40,0x07,0x5F,0xF7,0x5F,0xF7,0x5C,0x77,0x5C,0x77,
	0x5C,0x77,0x5C,0x77,0x5C,0x77,0x50,0x17,0x58,0x37,0x5C,0x77,
	0x5E,0xF7,0x5F,0xF7,0x40,0x07,0x3F,0xFB,},
 {0x6A,0xAF,0x55,0x57,0x6A,0xAF,0x55,0x57,0x6A,0xAF,0x55,0x57,
	0x6A,0xAF,0x55,0x57,0x6A,0xAF,0x55,0x57,0x6A,0xAF,0x55,0x57,
	0x6A,0xAF,0x55,0x57,0x6A,0xAF,0x55,0x57,},
 {0x00,0x00,0x7F,0xFE,0x80,0x01,0xBF,0xFD,0xBD,0xFD,0xB9,0xFD,
	0xB0,0x0D,0xA0,0x0D,0xB0,0x0D,0xB9,0xFD,0xBD,0xFD,0xBF,0xFD,
	0x80,0x01,0x7F,0xFE,0xFF,0xFF,0xFF,0xFF,},
 {0x00,0x00,0x7F,0xFE,0x80,0x01,0xBF,0xFD,0xBF,0xFD,0xBF,0xFD,
	0xBF,0xFD,0xBF,0xFD,0xBF,0xFD,0xBF,0xFD,0xBF,0xFD,0xBF,0xFD,
	0x80,0x01,0x7F,0xFE,0xFF,0xFF,0xFF,0xFF,},
 {0x00,0x00,0x7F,0xFE,0x80,0x01,0xBF,0xFD,0xBF,0xBD,0xBF,0x9D,
	0xB0,0x0D,0xB0,0x05,0xB0,0x0D,0xBF,0x9D,0xBF,0xBD,0xBF,0xFD,
	0x80,0x01,0x7F,0xFE,0xFF,0xFF,0xFF,0xFF,},
 {0x00,0x00,0xFF,0xFF,0xAA,0xAA,0x55,0x55,0xAA,0xAA,0x55,0x55,
	0xAA,0xAA,0x55,0x55,0xAA,0xAA,0x55,0x55,0xAA,0xAA,0x55,0x55,
	0xAA,0xAA,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,},
 {0xFF,0xFF,0xFC,0xFF,0xF8,0x7F,0xFC,0xFF,0xFC,0xFF,0xDC,0xEF,
	0x80,0x07,0x80,0x07,0xDC,0xEF,0xFC,0xFF,0xFC,0xFF,0xF8,0x7F,
	0xFC,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,}};

void vDrawVScrollBarBit(WORD x, WORD ex, WORD y)
{
	WORD i;
	if(x & 0x0001) x++;
	for(i=x;i<ex-16;i+=16)
		vPutWordImageBit(i, y, winicon2[3]);
	vPutWordImageBit(x, y, winicon2[0]);
	vPutWordImageBit(ex-16, y, winicon2[2]);
}

void vDrawHScrollBarBit(WORD x, WORD y, WORD ey)
{
	WORD i;
	if(y & 0x0001) y++;
	for(i=y;i<ey-16;i+=16)
		vPutWordImageBit(x, i, winicon2[7]);
	vPutWordImageBit(x, y, winicon2[4]);
	vPutWordImageBit(x, ey-16, winicon2[6]);
}

void vDelVThumbBit(WORD x, WORD ex, WORD y, WORD total, int old_sub)
{
	if(total==0) total=1;
	vPutWordImageBit((int)(x+16+((long)(ex-x-48)*(long)old_sub)/total)&0xfffe, y, winicon2[3]);
}

void vDrawVThumbBit(WORD x, WORD ex, WORD y, WORD total, int sub, int old_sub)
{
	if(total==0) total=1;
	vPutWordImageBit((int)(x+16+((long)(ex-x-48)*(long)old_sub)/total)&0xfffe, y, winicon2[3]);
	vPutWordImageBit((int)(x+16+((long)(ex-x-48)*(long)sub)/total)&0xfffe, y, winicon2[1]);
}

void vDelHThumbBit(WORD x, WORD y, WORD ey, WORD total, int old_sub)
{
	if(total==0) total=1;
	vPutWordImageBit(x, (int)(y+16+((long)(ey-y-48)*(long)old_sub)/total)&0xfffe, winicon2[7]);
}

void vDrawHThumbBit(WORD x, WORD y, WORD ey, WORD total, int sub, int old_sub)
{
	if(total==0) total=1;
	vPutWordImageBit(x, (int)(y+16+((long)(ey-y-48)*(long)old_sub)/total)&0xfffe, winicon2[7]);
	vPutWordImageBit(x, (int)(y+16+((long)(ey-y-48)*(long)sub)/total)&0xfffe, winicon2[5]);
}

void vDarkenTextWindowTitle(int n)
{
	int x, y, ex, ey;

	x=tw[n].x+4;
	ex=tw[n].x+19;
	y=(tw[n].y<<3)+4;
	ey=((tw[n].y+tw[n].ly)<<3)-4;
	vWBarBit(x, y, ex, y+19);
	vWBarBit(x, ey-19, ex, ey);
	vXORBarBit(x, y, ex-1, ey);
}

void vResetTextWindowTitle(void)
{
	BYTE center;
	int x, y, ly;

	x=tw[cur_w].x+4;
	y=tw[cur_w].y;
	ly=tw[cur_w].ly;
	vBarByte(x+1, y+7, x+14, y+ly-3, 1);
	center=(ly-strlen(tw[cur_w].headline))/2+y+1;
	vWriteString(x, center, tw[cur_w].headline, 0x04);
}

void vDrawWindowByte(int x, int y, int lx, int ly, BYTE flag, BYTE *str)
{
	BYTE center;
	int blx, by, bsum;

	blx=lx<<3;
	by=y<<3;
	bsum=(y+ly)<<3;
	vMCOFF();
	vBarByte(x, y, x+blx, y+ly, 0);
	vBarByte(x+4, y, x+19, y+ly, 1);
	vBBoxBit(x, y<<3,x+blx, bsum);
	vWBoxBit(x+1, by+1, x+blx-1, bsum-1);
	vWBoxBit(x+2, by+2, x+blx-2, bsum-2);
	vBBoxBit(x+3, by+3, x+44, bsum-3);
	center=(ly-strlen(str))/2+y+1;
	vWriteString(x+4, center, str, 0x04);
	vPutImage(x+3, y, x+19, y+2, winicon[0]);
	if(flag & 0x01) vPutImage(x+3, y+ly-3, x+19, y+ly-1, winicon[1]);
	if(flag & 0x02) vPutWordImageBit(x+blx-16, bsum-16, winicon2[8]);
}

void vDrawObjByte(int n)
{
	int i, j, cho=0, cmt=0, t;
	int x, y, ex, ey, h, len;
	BYTE **d_t; // Data temp  pointer
	BYTE **c_t; // Comment temp
	BYTE attr, ly;

	d_t=pop[n].data;
	c_t=pop[n].comment;

	for(i=0; i<pop[n].used; i++) {
		x=pop[n].block[i][0]*8+pop[n].x+24; // Bit
		y=pop[n].block[i][1]+pop[n].y;   // Byte
		ex=x+pop[n].block[i][2]*16;
		ey=pop[n].y+pop[n].block[i][3];
		h=pop[n].block[i][4];
		vWBoxBit(x-2, y*8-2, ex+1, ey*8+1);
		if((h&0x0f)==0 || (h&0x0f)==6) vWLineYBit(ex+2, y*8-2, ey*8+1);
		if((h&0x0f)==4) {
			vWLineYBit(ex+2, y*8-1, ey*8+3);
			vWLineXBit(x-1, ex+2, ey*8+2);
			vWLineXBit(x, ex+2, ey*8+3);
		}
		if((h&0x0f)==5) {
			vWBoxBit(x-2-18, y*8-2, x-2, ey*8+1+16);
		}
		if(h & 0xc0) {
			t=c_t[cmt][0]-48;
			if(t<2) {
				attr=0;
			}
			else attr=0x40;
			if(h & 0x40) {
				len=strlen(c_t[cmt]+1);
				vWriteString(x, y-len-1, c_t[cmt]+1, attr);
			}
			else {
				if((h&0x0f)!=5) {
					vWriteString(x-18, y, c_t[cmt]+1, attr);
				}
				else {
					vWriteString(x-18-18, y, c_t[cmt]+1, attr);
				}
			}
			cmt++;
		}
		if((h & 0x0f)!=5 && (h & 0x0f) !=7) {
			ly=pop[n].block[i][3]-pop[n].block[i][1];
			for(j=0; j<pop[n].block[i][2] ; j++) {
				t=d_t[cho+j][0]-48;
				if(t<2) {
					attr=0;
				}
				else attr=0x40;
        vPutStringLength(x+j*16, y, d_t[cho+j]+1, attr, 0, ly); // first Byte of data
			}   			                                                    // means put attribute
			cho+=j;
		}
		else if((h & 0x0f)==5) {     // variable data num block
			vDrawRecordObj(n, i);
			vMCOFF();
		}
	}
}

