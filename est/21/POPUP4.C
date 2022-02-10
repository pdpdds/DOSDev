#define _POPUP4
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <mem.h>
#include <dos.h>
#include <dir.h>
#include <ctype.h>
#include <io.h>
#include <errno.h>
#include <string.h>
#include <conio.h>
#include "est.h"
	#include "21.h"
	#include "proto.h"
#include "mouse.h"
#include "popup.h"
#define LOAD         0
#define SAVE_AS      1
#define READ_BLK     2
#define WRITE_BLK    3
#define READ_MACRO   4
#define WRITE_MACRO  5
#define READ_GLS     6
#define WRITE_GLS    7
#define USEDBLK pop[n].used

int pop_x, pop_y;

extern int bminx, bminy, bmaxx, bmaxy;
extern int graph_mode;
extern text_window tw[];
extern menu_panel panel[];
extern int MaxX,MaxY;
extern BYTE *our_buf;
extern BYTE winicon[];
extern BYTE block_task;
extern unsigned long Scount;
extern BYTE cur_w;
extern BYTE exe_path[];
extern BYTE the21[];
extern BYTE show_rod;
extern BYTE View_time;
extern int m_rate;

popup_window pop[27];

void vHelpDlg(int n);

void vMakeDlgWindow(int n, int x, int y, BYTE use, BYTE **blk,
						 BYTE **dat, BYTE **hed, BYTE **cod, BYTE *winhead)
{
	pop[n].lx=x;
	pop[n].ly=y;
	pop[n].x=MaxX/2-x*4;
	pop[n].y=MaxY/16-y/2;
	pop[n].used=use;
	pop[n].block=(BYTE **)blk;
	pop[n].data=(BYTE **)dat;
	pop[n].comment=(BYTE **)hed;
	pop[n].code=(BYTE **)cod;
	pop[n].m=0;
	strcpy(pop[n].headline, winhead);
}

void vMakeExtDlgWindow(int n, BYTE *data, BYTE *pn, BYTE *process,
								BYTE *vb, BYTE *value, BYTE *set)
{
	pop[n].p=(BYTE *)data;
	pop[n].p_n=(BYTE *)pn;
	pop[n].process=(BYTE *)process;
	pop[n].vb_scr=(BYTE *)vb;
	pop[n].value=(BYTE *)value;
	pop[n].set=(BYTE *)set;
}

void vRevControlObj(int n, int bn, int cn)    // bn means block  num
{                                         // cn means choice num
	int x, y, len, attr;

	attr=pop[n].block[bn][4] & 0x0f;
	if(attr==0 || attr==6 || (n==PW4 && bn==0) || attr==7) return;
	vMCOFF();
	x=pop[n].block[bn][0]*8+pop[n].x+24;    // Bit
	y=pop[n].block[bn][1]+pop[n].y;         // Byte
	len=pop[n].block[bn][3]-pop[n].block[bn][1];
	vXORBarBytex16(x+16*cn, y, len);
	vMCON();
}

void vDrawScrollBarObj(int n, int bn, int cho)
{
	int x, ex, ey;

	vMCOFF();
	x=pop[n].block[bn][0]*8+pop[n].x+24; // Bit
	ex=x+pop[n].block[bn][2]*16;
	ey=pop[n].y+pop[n].block[bn][3];
	pop[n].value[bn]=cho;
	if(pop[n].p_n[bn]) {
		vDrawVScrollBarBit(x-1, ex+2, ey*8+2);
		vDrawVThumbBit(x-1, ex+2, ey*8+2, pop[n].p_n[bn]-1, pop[n].value[bn]+pop[n].vb_scr[bn], 0);
	}
	else {
		vDrawVScrollBarBit(x-1, ex+2, ey*8+2);
		vDrawVThumbBit(x-1, ex+2, ey*8+2, 100, 0, 0);
	}
	vMCON();
}

BYTE record_flag=0;

void vDrawRecordObj(BYTE n, BYTE bn)
{
	int x, y, ex, ey, i, t;
	int cur_rec=0;
	BYTE **t_addr;

	vMCOFF();
	x=pop[n].block[bn][0]*8+pop[n].x+24; // Bit
	y=pop[n].block[bn][1]+pop[n].y;   // Byte
	ex=x+pop[n].block[bn][2]*16;
	ey=pop[n].y+pop[n].block[bn][3];
	for(i=0; i<bn; i++) {
		cur_rec+=pop[n].p_n[i];
	}
	if(pop[n].block[bn][2]<pop[n].p_n[bn]) {
		t=pop[n].block[bn][2];
	}
	else {
		t=pop[n].p_n[bn];
		if(record_flag==0) vNBarBit(x+t*16, y*8, ex, ey*8, 0);
	}
	t_addr=(BYTE **)pop[n].p;
	if(pop[n].p_n[bn]==0) {
		vWriteString(x, y+1, "<´ô·q>", 0);
		vNBarBit(x-2-16, y*8+8, x-3, ey*8+15, 0);
		vDrawVScrollBarBit(x-1, ex+2, ey*8+2);
		vDrawVThumbBit(x-1, ex+2, ey*8+2, 100, 0, 0);
		pop[n].set[bn]=0;
		vMCON();
		return;
	}
	else {
		vNBarBit(x-2-16, y*8+8, x-3, ey*8+15, 0);
//		if(pop[n].process[bn]==0) {
			vWriteString(x-2-17, y+1, t_addr[cur_rec+pop[n].set[bn]], 0);

	}
	if(record_flag==1) {
		vMCON();
		return;
	}
	for(i=0; i<t; i++) {
//		switch(pop[n].process[bn]) {
//			case 0 :
				vWriteString(x+i*16, y+1, t_addr[cur_rec+i+pop[n].vb_scr[bn]], 0);
//				break;
//			case 1 :
//				put_extra(x+i*16, y+1, pop[n].p, cur_rec+i);
//				break;
//		}   // put_extra automatically search '\r' and put from there
	}
	vDrawScrollBarObj(n, bn, pop[n].value[bn]);
	vMCON();
}

void vDrawDlgWindow(BYTE n)
{
	int x, y, lx, ly;

	x=pop[n].x;
	y=pop[n].y;
	lx=pop[n].lx;
	ly=pop[n].ly;

	if(n>1) {
		vGetImage(x, y,	x+(lx<<3), y+ly, our_buf);
		iSwapMem(MSWRITE, (lx*ly)<<3, &(pop[n].m), our_buf);
	}
	else vGetImage(x, y,	x+(lx<<3), y+ly, our_buf+30000);
	vDrawWindowByte(x, y, lx, ly, 0, pop[n].headline);
}

void vOpenDlgWindow(BYTE n)
{
	vMCOFF();
	vDrawDlgWindow(n);
	vDrawObjByte(n);
	vMCON();
}

BYTE the_first=1;

int iXYIsInObj(int n, BYTE *blk, BYTE *cho)
{
	int mx, my, t;
	int x, y, ex, ey, i;

	getpos(mx,my);
	if(mx>pop[n].x && my> pop[n].y*8 &&\
		 mx<pop[n].x+pop[n].lx*8 && my<(pop[n].y+pop[n].ly)*8) {
		for(i=0; i< pop[n].used; i++) {
			if((pop[n].block[i][4]&0x0f)==5) {
				t=18;
			}
			else t=0;  // if variable block y size must enlarged
			x=pop[n].block[i][0]*8+pop[n].x+24;           // Bit
			y=(pop[n].block[i][1]+pop[n].y)*8;            // Byte
			ex=x+pop[n].block[i][2]*16;
			ey=pop[n].block[i][3]*8+pop[n].y*8+t;           // Bit

			if(mx>x && my>y && mx<ex && my<ey) {
				*cho=(mx-x)/16;
				*blk=i;
				if(t!=0) {            // variable data block
					if(pop[n].p_n[i]<pop[n].block[i][2]) {
						t=pop[n].p_n[i]-1;
						if(t<0) t=0;
						if(*cho>t) (*cho)=t;
					}
					if(my>ey-18) {
						if(the_first) {
							(*cho)=pop[n].value[i];
							return 2;
						}
						else return 0;
					}
				}
				return 1;
			}
		}
	}
	return 0;
}

int iExamineMCInObj(int n, BYTE *blk, BYTE *cho)
{
	int t;

	t=iXYIsInObj(n, blk, cho);
	if(t>0) {
		if(lbp==0) return -1;
		else return t;
	}
	else {
		if(lbp==0) return -1;
		else return 0;
	}
}

void vXORPopup(int x, int y, int ex, int ey)
{
	int i;
	for(i=1;i<3;i++) {
		vMCOFF();
		vXORBoxBit(x+i,y+i,ex-i,ey-i);
	}
}

int iMoveDlgWindow(BYTE n)
{
	int x, y, lx, ly, tx, ty;

	x=pop[n].x;
	y=pop[n].y;
	lx=pop[n].lx;
	ly=pop[n].ly;

	if(!lbp) return 0;
	getpos(tx, ty);
	if(tx<x || tx>x+24 || ty<y*8 || ty>y*8+ly*8) return 0;
	if(ty<y*8+22) {
		return -3;    // popup must closed
	}
	else {
		if(iMoveWindow(&x, &y, &lx, &ly)) {
			pop_x=x;
			pop_y=y;
			vMCON();
			return -2;
		}
		else {
			vMCON();
			return 0;
		}
	}
}

#define LSHIFT (bioskey(2) & 0x02)

int iKeyMoveWindow(int *wx, int *wy, int *wlx, int *wly, BYTE code)
{
	INKEY c;
	int x, y, lenx, leny, i;

	x=*wx;
	y=(*wy)*8;
	lenx=(*wlx)*8;
	leny=(*wly)*8;

	for(i=0;i<2;i++) {
		vMCOFF();
		vXORBoxBit(x+i,y+i,x+lenx-i,y+leny-i);
		vMCON();
	}
	for(;;) {
		while(!wCheckKeyHit());
		c.i=bioskey(0);
		for(i=0;i<2;i++) {
			vMCOFF();
			vXORBoxBit(x+i,y+i,x+lenx-i,y+leny-i);
			vMCON();
		}
		if(c.ch[0]) {
			switch(c.ch[0]) {
				case ESC :
					return 0;
				case RETURN :
					(*wx)=x;
					(*wy)=y/8;
					(*wlx)=lenx/8;
					(*wly)=leny/8;
					return 1;
			}
		}
		else {
			switch(c.ch[1]) {
				case DOWN :
					if(!LSHIFT) {
						x+=8;
						if(x+lenx>MaxX) x=MaxX-lenx;
					}
					else if(code) {
						lenx+=8;
						if(lenx+x>MaxX) lenx=MaxX-x;
					}
					break;
				case UP :
					if(!LSHIFT) {
						x-=8;
						if(x<0) x=0;
					}
					else if(code) {
						lenx-=8;
						if(lenx<180) lenx=180;
					}
					break;
				case LEFT :
					if(!LSHIFT) {
						y-=8;
						if(y<0) y=0;
					}
					else if(code) {
						leny-=8;
						if(leny<320) leny=320;
					}
					break;
				case RIGHT :
					if(!LSHIFT) {
						y+=8;
						if(y+leny>MaxY) y=(MaxY-leny);
					}
					else if(code) {
						leny+=8;
						if(leny>MaxY) leny=MaxY-y;
					}
					break;
				case CTRL_DARROW :
					if(!LSHIFT) {
						x+=32;
						if(x+lenx>MaxX) x=MaxX-lenx;
					}
					else if(code) {
						lenx+=32;
						if(x+lenx>MaxX) lenx=MaxX-x;
					}
					break;
				case CTRL_UARROW :
					if(!LSHIFT) {
						x-=32;
						if(x<0) x=0;
					}
					else if(code) {
						lenx-=32;
						if(lenx<180) lenx=180;
					}
					break;
				case CTRL_LARROW :
					if(!LSHIFT) {
						y-=32;
						if(y<0) y=0;
					}
					else if(code) {
						leny-=32;
						if(leny<320) leny=320;
					}
					break;
				case CTRL_RARROW :
					if(!LSHIFT) {
						y+=32;
						if(y+leny>MaxY) y=(MaxY-leny);
					}
					else if(code) {
						leny+=32;
						if(leny+y>MaxY) leny=MaxY-y;
					}
					break;
			}
		}
		for(i=0;i<2;i++) {
			vMCOFF();
			vXORBoxBit(x+i,y+i,x+lenx-i,y+leny-i);
			vMCON();
		}
	}
}

int iKeyMoveDlgWindow(BYTE n)
{
	int x, y, lx, ly;
	BYTE t;

	x=pop[n].x;
	y=pop[n].y;
	lx=pop[n].lx;
	ly=pop[n].ly;
	t=iKeyMoveWindow(&x, &y, &lx, &ly, 0);
	if(t==1) {
		pop_x=x;
		pop_y=y;
	}
	return t;
}

void vCloseDlgWindow(BYTE n)
{
	int x, y, lx, ly;

	x=pop[n].x;
	y=pop[n].y;
	lx=pop[n].lx;
	ly=pop[n].ly;

	vMCOFF();
	if(n>1) {
		if(iSwapMem(MSREAD, (lx*ly)<<3, &(pop[n].m), our_buf)) {
			vPutImage(x, y,	x+(lx<<3), y+ly, our_buf);
		}
		else {
			vErrorMsgfDlg(1, 2, 0);
		}
		vFreeMem(&(pop[n].m));
	}
	else vPutImage(x, y,	x+(lx<<3), y+ly, our_buf+30000);
}

int iGetObjOffset(int n, BYTE cur_blk, BYTE choice)
{
	int i,t_addr=0;

	for(i=0; i<cur_blk+1 ; i++) {
		switch(pop[n].block[i][4] & 0x0f) {
			case 0 :                // means vWriteString
				if(i!=cur_blk) t_addr+=80;
				break;
			case 1 :                // means get_data
				if(i!=cur_blk) {
					t_addr+=pop[n].block[i][2]*15;
				}
				else t_addr+=choice*10;
				break;
			case 2 :                // means multi option
				if(i!=cur_blk) t_addr+=pop[n].block[i][2];
				else t_addr+=choice;
				break;
			case 3 :                // means single option
				if(i!=cur_blk) {
					t_addr+=1;
				}
				break;
			case 4 :                // means button
				if(i!=cur_blk) {
					t_addr+=1;
				}
				break;
			case 5 :
				if(i!=cur_blk) {
					t_addr+=1;
				}
				break;
			case 6 :                // means get_string2
				if(i!=cur_blk) t_addr+=200;
				break;
			case 7 :                // means using iEditor
				if(i!=cur_blk) t_addr+=400;
				break;
		}
	}
	return t_addr;
}

#define MAXCBCHO pop[n].block[cur_blk][2] // all of n block's CNode choice

int iCalcChiose(int n, BYTE blk, BYTE choice)
{
	int i, ret=0, attr;
	if(blk==0) return choice;
	for(i=0; i<blk; i++) {
		attr=pop[n].block[i][4] & 0x0f;
		if(attr!=7 && attr!=5) ret+=pop[n].block[i][2];
	}
	return ret+choice;
}

void vInt2Char(int i, BYTE *buf)
{
	INKEY c;
	c.i=i;
	*buf=c.ch[0];
	*(buf+1)=c.ch[1];
}

void vChar2Int(int *i, BYTE *buf)
{
	INKEY c;
	c.ch[0]=*buf;
	c.ch[1]=*(buf+1);
	*i=c.i;
}

void vNumberDlgWindow(BYTE n)
{
	vMCOFF();
	if(n==100) {
		vOpenDlgWindow(PW2);
		vMCOFF();
		vDrawHScrollBarBit(pop[PW2].x+83, (pop[PW2].y+1)<<3, (pop[PW2].y+pop[PW2].ly-1)<<3);
		vWBoxBit(pop[PW2].x+82, ((pop[PW2].y+1)<<3)-1, pop[PW2].x+97,
				((pop[PW2].y+pop[PW2].ly-1)<<3));
		vWBoxBit(pop[PW2].x+83, ((pop[PW2].y+1)<<3)-1, pop[PW2].x+98,
				((pop[PW2].y+pop[PW2].ly-1)<<3));
	}
	if(n==99) {
		vCloseDlgWindow(PW2);
	}
	vMCON();
}


WORD wGetNumber(int def, int start, int vExit, BYTE *unit, BYTE *head)
{
	INKEY c;
	int old_i,i,k;
	int mx, my;
	BYTE tb, tc;
	char choice=0,j;

	i=def;
	old_i=i;
	strcpy(pop[PW2].headline, head);
START:
	vNumberDlgWindow(100);
	vMCOFF();
	iPrintf(pop[PW2].x+32, pop[PW2].y+2, 0,
					"<¤ñ¶á> %dµA¬á %dŒa»¡", start, vExit);
	vRevControlObj(PW2,choice,0);
	for(;;) {
DATA:
		vMCOFF();
		iPrintf(pop[PW2].x+54, pop[PW2].y+2, 0, "->%5d %s    ", i, unit);
		vDrawHThumbBit(pop[PW2].x+83, (pop[PW2].y+1)<<3,\
								(pop[PW2].y+pop[PW2].ly-1)<<3,\
								vExit-start, i-start, old_i-start);
		vWLineYBit(pop[PW2].x+83, ((pop[PW2].y+1)<<3)-1,
												 ((pop[PW2].y+pop[PW2].ly-1)<<3));
		old_i=i;
		vMCON();
Z:	while(!wCheckKeyHit()) {
			if(rbp) {
				while(rbp);
				goto OUT;
			}
			k=iHScrollBarHandler(pop[PW2].x+83, (pop[PW2].y+1)<<3,
								(pop[PW2].y+pop[PW2].ly-1)<<3,
								 vExit-start, i,old_i,start,vExit);
			if(k!=-100) {
				i=k;
				old_i=i;
				goto DATA;
			}
			getpos(mx, my);
			if(mx>pop[PW2].x+83) goto Z;
			else if(mx<pop[PW2].x+24 && lbp) {
				j=iMoveDlgWindow(PW2);
				if(j==-3) {
					while(lbp);
					goto OUT;
				}
				if(j==-2) {
					vNumberDlgWindow(99);
					pop[PW2].x=pop_x;
					pop[PW2].y=pop_y;
					goto START;
				}
			}
			else if(lbp) {
				tb=choice;
				tc=0;
				for(;;) {
					j=iExamineMCInObj(PW2, &tb, &tc);
					if(j==1) {
						if(tb!=choice) {
							vRevControlObj(PW2, choice, 0);
							choice=tb;
							vRevControlObj(PW2, choice, 0);
						}
					}
					if(j==-1) {
						getpos(mx, my);
						if(mx>pop[PW2].x+24+8 && mx<pop[PW2].x+24+40+8
							 && my>pop[PW2].y*8+26*8 && my<pop[PW2].y*8+26*8+14*8) {
							goto RN;
						}
						else goto Z;
					}
				}
			}
		}
		c.i=bioskey(0);
		vRevControlObj(PW2,choice,0);
		if(c.ch[0]) {
			switch(c.ch[0])  {
				case ESC :  case 'N' : case 'n' :
OUT:			vNumberDlgWindow(99);
					return def;
				case '\r' : case 'D' : case 'd' : case SPACE :
RN:				if(choice==0) {
						vNumberDlgWindow(99);
						return i;
					}
					else if(choice==1) {
						vNumberDlgWindow(99);
						return def;
					}
			}
		}
		else {
			switch(c.ch[1]) {
				case UP : case DOWN :
					choice^=1; break;
				case RIGHT :
					if(i==vExit) break;
					i++; break;
				case LEFT :
					if(i==start) break;
					i--; break;
				case CTRL_LARROW :
					if(i<start+10) {
						i=start; break;
					}
					i-=10; break;
				case CTRL_RARROW :
					if(i>vExit-10) {
						i=vExit; break;
					}
					i+=10; break;
				case PGUP :
					if(i<start+5) {
						i=start; break;
					}
					i-=5; break;
				case PGDN :
					if(i>vExit-5) {
						i=vExit; break;
					}
					i+=5; break;
				case HOME :  case CTRL_PGUP :
					i=start; break;
				case END :   case CTRL_PGDN :
					i=vExit; break;
				case F7 :
					if(iKeyMoveDlgWindow(PW2)) {
						vCloseDlgWindow(PW2);
						pop[PW2].x=pop_x;
						pop[PW2].y=pop_y;
						goto START;
					}
					break;
				case F1 :
					vHelpDlg(POP_HELP+PW2);
					break;
			}
		}
		vRevControlObj(PW2,choice,0);
	}
}

extern BYTE filter[];

void vDispChecked(void)
{
	int x, y, i;

	if(!block_task) return;
	x=pop[PW7].block[0][0]*8+pop[PW7].x+24;           // Bit
	y=pop[PW7].block[0][3]+pop[PW7].y;                // Byte
	for(i=0; i<4; i++) {
		if(filter[i]) vWriteString(x+i*16, y-1, "\xff\xf0", 0);
	}
}

void vSetDlgObj(int n, BYTE cur_blk, BYTE cur_cho, BYTE *addr)
{
	int i, j;
	int x, y, cho_num,len;
	int s, c, e, t;                   // start, CNode, vExit
	int attr;
	BYTE *units;
	int p[6];
	BYTE *t_addr;

	vMCOFF();
	t_addr=addr;
	t_addr+=iGetObjOffset(n, cur_blk, 0);
	switch(pop[n].block[cur_blk][4] & 0x0f) {            //  get attribute
		case 1 :
			t_addr+=cur_cho*15;
			x=pop[n].block[cur_blk][0]*8+pop[n].x+24;           // Bit
			y=pop[n].block[cur_blk][1]+pop[n].y;                // Byte
			units=(t_addr+6);
			len=pop[n].block[cur_blk][3]-pop[n].block[cur_blk][1]-6-strlen(units);
			vChar2Int((int far*)&s, t_addr);
			vChar2Int((int far*)&c, t_addr+2);
			vChar2Int((int far*)&e, t_addr+4);
			cho_num=iCalcChiose(n, cur_blk, cur_cho);
			attr=pop[n].data[cho_num][12];   // save data[12]
			pop[n].data[cho_num][12]=0;
			if(n==PW7) {
				for(t=0; t<4; t++) vChar2Int((int far *)(p+t), addr+2+t*15);
				if(cho_num==1) c=(int)((char)p[cho_num]);
				else c=(int)p[cho_num];
				switch(cho_num) {
					case 0 : s=0; e=40; break;
					case 1 :
						s=1-p[2]; e=p[3]-8;
						if(s<-120) s=-120;
						if(e>120) e=120;
						break;
					case 2 :
						if((int)((char)p[1])>0) {
							s=1; e=p[3]-10-(int)((char)p[1]);
						}
						else {s=1-(int)((char)p[1]); e=p[3]-10;}
						break;
					case 3 :
						if((int)((char)p[1]) > 0) {
							s=p[2]+10+p[1]; e=250;
						}
						else {s=p[2]+10; e=250;}
						break;
				}
				filter[cho_num]=1;
			}
			i=wGetNumber(c, s, e, units, pop[n].data[cho_num]+1);
			pop[n].data[cho_num][12]=attr;   // restore data[12]
			iPrintf(x+cur_cho*16, y+len, 0, "%3d %s",i, units);
			vInt2Char(i, t_addr+2);        // change t_addr by i
			if(n==PW7) vDispChecked();
			break;
		case 2 :
			cho_num=iCalcChiose(n, cur_blk, 0);
			for(j=0; j<pop[n].block[cur_blk][2]; j++) {
				x=pop[n].block[cur_blk][0]*8+pop[n].x+24;           // Bit
				y=pop[n].block[cur_blk][1]+pop[n].y;                // Byte
				t=pop[n].data[cho_num+j][0]-48;
				if(t!=1) {
					if(t==0) attr=0;
					else attr=0x40;
					if(*t_addr) {
						vWriteString(x+j*16, y+1, "\xff\xf0", attr);
					}
					else {
						vWriteString(x+j*16, y+1, " ", attr);
					}
				}
				else if(t==1) {                   // meaning '1'
					if(*t_addr==1) {
						vWriteString(x+j*16, y+1, "\xff\xf0", 0);
					}
					else if(*t_addr==2) {
						vWriteString(x+j*16, y+1, "?", 0);
					}
					else {
						vWriteString(x+j*16, y+1, " ", 0);
					}
				}
				t_addr++;
			}
			break;
		case 3 :
			x=pop[n].block[cur_blk][0]*8+pop[n].x+24;           // Bit
			y=pop[n].block[cur_blk][1]+pop[n].y;                // Byte
			cho_num=iCalcChiose(n, cur_blk, 0);
			for(j=0; j<pop[n].block[cur_blk][2]; j++) {
				t=pop[n].data[cho_num+j][0]-48;
				if(t==2) attr=0x40;
				else {
					attr=0;
				}
				if(*t_addr==j) {
					vWriteString(x+j*16, y+1, "\xff\xf1", attr);
				}
				else {
					vWriteString(x+j*16, y+1, " ", attr);
				}
			}
			break;
		default : break;
	}
	vMCON();
}

void vSetDlgWindow(int n, BYTE *addr)
{
	int i, j;
	int x, y, len, c;
	BYTE *units;
	BYTE *t_addr;

	vMCOFF();
	t_addr=addr;
	for(i=0; i<USEDBLK; i++) {
		switch(pop[n].block[i][4] & 0x0f) {  // block_attr
			case 0 :
				x=pop[n].block[i][0]*8+pop[n].x+24;           // Bit
				y=pop[n].block[i][1]+pop[n].y;                // Byte
				len=pop[n].block[i][3]-pop[n].block[i][1];
				vPutStringLength(x, y, t_addr, 0, 0, len);
				t_addr+=80;
				break;
			case 1 :
				for(j=0; j<pop[n].block[i][2]; j++) {
					x=pop[n].block[i][0]*8+pop[n].x+24;           // Bit
					y=pop[n].block[i][1]+pop[n].y;                // Byte
					units=(t_addr+6);
					len=pop[n].block[i][3]-pop[n].block[i][1]-6-strlen(units);
					vChar2Int((int far*)&c, t_addr+2);
					iPrintf(x+16*j, y+len, 0, "%3d %s", c , units);
					t_addr+=15;
				}
				if(n==PW7) vDispChecked();
				break;
			case 2 :
				vSetDlgObj(n, i, 0, addr);
				t_addr+=pop[n].block[i][2];
				break;
			case 3 :
				vSetDlgObj(n, i, (*addr), addr);
				t_addr++;
				break;
			case 4 :
				t_addr++;
				break;
			case 5 :
				t_addr++;
				break;

			case 7 :
				t_addr+=400;
				break;
		}
	}
	vMCON();
}

int iGetNextObj(int n, BYTE blk, BYTE *addr)
{
	int attr;
	int offset;

	attr=pop[n].block[blk][4] & 0x0f;
	if(attr==3) {
		offset=iGetObjOffset(n, blk, 0);
		if(addr[offset]-1 > pop[n].block[blk][2]) {
			return 0;
		}
		else {
			return addr[offset];
		}
	}
	if(attr==5) {
		return pop[n].value[blk];
	}
	else return 0;
}


BYTE processing=0;
BYTE free_out=0;
void set_process(BYTE n) {processing=n;}
void set_free_out(BYTE n) {free_out=n;}

int iCheckShortCut(int n, BYTE *cur_blk, BYTE *cur_cho, BYTE code)
{
	int t_b, t_c, u, cho_all=0, attr;
	BYTE b;

	u=pop[n].used ;
	for(t_b=0; t_b<u; t_b++) {
		attr=pop[n].block[t_b][4]&0x0f;
		if(attr!=5 && attr!=7) {
			for(t_c=0; t_c< pop[n].block[t_b][2]; t_c++) {
				b=pop[n].data[cho_all][strlen(pop[n].data[cho_all])-1];
				if(b==code) {
					*cur_blk=t_b;
					*cur_cho=t_c;
					return 1;
				}
				cho_all++;
			}
		}
	}
	return 0;
}

int iXYIsInArea(int x, int y, int lx, int ly)
{
	int mx, my;

	getpos(mx, my);
	if(mx>x && mx<x+lx*8 && my>y*8 && my<(y+ly)*8) {
		return 1;
	}
	else return 0;
}

void vVBHead(int n, int bn, int cho)
{
	record_flag=1;
	pop[n].set[bn]=cho+pop[n].vb_scr[bn];
	vDrawRecordObj(n, bn);
	record_flag=0;
}

int iVScrollBarObj(int n, int bn)
{
	int x, ex, ey, t;

	x=pop[n].block[bn][0]*8+pop[n].x+24; // Bit
	ex=x+pop[n].block[bn][2]*16;
	ey=pop[n].y+pop[n].block[bn][3];

	t= iVscrollBarHandler(x-1, ex+2, ey*8+2, pop[n].p_n[bn]-1, pop[n].value[bn]+pop[n].vb_scr[bn],
							pop[n].value[bn]+pop[n].vb_scr[bn], 0, pop[n].p_n[bn]-1);
	return t;
}

BYTE del_flag=0;
BYTE del_able=0;
BYTE ins_able=0;
BYTE num_able=0;

int iDialogDriver(int n, BYTE *buf)
{
	INKEY c;
	static BYTE choice[10], is_out[10], blk_attr[10];
	static BYTE cur_blk=0,cur_cho=0;
	BYTE t_blk, t_cho, ret;
	char code;
	int i, x, y, len, t, t_vb=0;
	BYTE *t_addr;
	BYTE tb1, tc1, getstr_flag=0;
	unsigned long time;
	int mx, my;

	vMCOFF();
	t_addr=buf;
	if(processing==0) {
		for(i=0; i<USEDBLK; i++) {
			blk_attr[i]= pop[n].block[i][4] & 0x0f;        // get only attribute
			is_out[i]  = (pop[n].block[i][4] & 0x20)>>5;   // get only out code
			if(blk_attr[i]!=7) {
				choice[i]  = pop[n].block[i][2]-1;
			}
			else choice[i]=0;
		}
		cur_cho=iGetNextObj(n, 0, buf);
		processing=1;
	}
	if(del_flag==1) {
		if(blk_attr[cur_blk]==5) {
			i=cur_blk;
			if(choice[i]+1>pop[n].p_n[i]-pop[n].vb_scr[i]) {
				if(pop[n].vb_scr[i]>0) pop[n].vb_scr[i]--;
				if(cur_cho+1>pop[n].p_n[i] && cur_cho!=0) {
					pop[n].value[i]--;
					cur_cho--;
				}
			}
			vDrawRecordObj(n, i);
		}
		del_flag=0;
	}
	vSetDlgWindow(n, buf);
	for(;;) {
M:	while(blk_attr[cur_blk]==0 || blk_attr[cur_blk]==6 || blk_attr[cur_blk]==7) {                    //  means iGetString blk
			t_addr=buf;
			t_addr+=iGetObjOffset(n, cur_blk, 0);
			x=pop[n].block[cur_blk][0]*8+pop[n].x+24;           // Bit
			y=pop[n].block[cur_blk][1]+pop[n].y;                // Byte
			len=pop[n].block[cur_blk][3]-pop[n].block[cur_blk][1];
//			if(blk_attr[cur_blk]==0) {
				i=iGetString(1, x, y, 78, len, t_addr);
				vPutStringLength(x, y, t_addr, 0, 0, len);

			if(i==-1) goto OUT;
			else {
				if(i==-2) vPutKey(DOWN, 0);
				else if(i==1) vPutKey(0, RETURN);
				else if(i==-3) vPutKey(UP, 0);
				else if(i==-10) vPutKey(F7, 0);
				else if(i==-11) vPutKey(F1, 0);
				else if(i==-15) vPutKey(0, TAB);
				else if(i==-16) vPutKey(SFT_TAB, 0);
				else if(i==-4) {
					getpos(mx, my);
					vMCON();
					if(mx>pop[n].x && mx<pop[n].x+24 && my>pop[n].y*8 && my<(pop[n].y+pop[n].ly)*8) {
						getstr_flag=0;
					}
					else getstr_flag=1;
					goto Z;
				}
				else goto M;
				break;
			}
		}
		vRevControlObj(n, cur_blk, cur_cho);
		while(!wCheckKeyHit()) {
			if(blk_attr[cur_blk]==0 || blk_attr[cur_blk]==6 || blk_attr[cur_blk]==7) {
				if(getstr_flag) {
					getstr_flag=0;
					goto M;
				}
			}
Z:
			for(;;) {
				if(rbp) {
					while(rbp);
					goto OUT;
				}
				if(!lbp) break;
				getpos(mx, my);
				if(pop[n].x<mx && pop[n].y*8<my && pop[n].x+20>mx && (pop[n].y+pop[n].ly)*8>my) {
					code=iMoveDlgWindow(n);
					if(code==-3) {
						while(lbp);
						goto OUT;
					}
					else {
						vCloseDlgWindow(n);
						pop[n].x=pop_x; pop[n].y=pop_y;
						vOpenDlgWindow(n);
						vSetDlgWindow(n, buf);
						goto M;
					}
				}
				else {
					the_first=1;
					for(;;) {
						if(iXYIsInArea(pop[n].x, pop[n].y, pop[n].lx, pop[n].ly)==1) {
							t_blk=cur_blk;
							t_cho=cur_cho;
							code=iExamineMCInObj(n, &t_blk, &t_cho);
							the_first=0;
							if(blk_attr[t_blk]==5) {
								t_vb=1;
							}
							else
								t_vb=0;
							if(code==1) {
								if(cur_blk!=t_blk || cur_cho!=t_cho) {
									vRevControlObj(n, cur_blk, cur_cho);
									cur_blk=t_blk;
									cur_cho=t_cho;
									if(blk_attr[cur_blk]==5)
										vDrawScrollBarObj(n, cur_blk, cur_cho);
									vRevControlObj(n, cur_blk, cur_cho);
								}
							}
							if(code==0 && t_vb==1) {
								if(pop[n].block[cur_blk][2]>=pop[n].p_n[cur_blk]) {
									the_first=0;
									continue;
								}
								getpos(mx, my);
								for(;my>(pop[n].y+pop[n].block[cur_blk][1])*8 &&\
										 my<(pop[n].y+pop[n].block[cur_blk][3])*8 &&lbp==1;) {
									getpos(mx, my);
									if(mx>pop[n].x+pop[n].block[cur_blk][0]*8+pop[n].block[cur_blk][2]*16+24) {
										if(pop[n].vb_scr[cur_blk]<pop[n].p_n[cur_blk]-choice[cur_blk]-1) {
											pop[n].vb_scr[cur_blk]++;
											vRevControlObj(n, cur_blk, cur_cho);
											vDrawRecordObj(n, cur_blk);
											vRevControlObj(n, cur_blk, cur_cho);
											time=Scount;
											while(Scount<time+9);
										}
										else break;
									}
									else if(mx<pop[n].x+pop[n].block[cur_blk][0]*8+24) {
										if(pop[n].vb_scr[cur_blk]!=0) {
											pop[n].vb_scr[cur_blk]--;
											vRevControlObj(n, cur_blk, cur_cho);
											vDrawRecordObj(n, cur_blk);
											vRevControlObj(n, cur_blk, cur_cho);
											while(Scount<time+9);
										}
										else break;
									}
									else break;
								}
							}
							if(code==-1) {
								vRevControlObj(n, cur_blk, cur_cho);
								if(blk_attr[cur_blk]==0 || blk_attr[cur_blk]==7) goto M;
								if(iXYIsInObj(n, &tb1, &tc1)) {
									goto RN;
								}
								else {
									vRevControlObj(n, cur_blk, cur_cho);
									if(blk_attr[cur_blk] !=0 && blk_attr[cur_blk] !=6 &&\
										 blk_attr[cur_blk] !=7) {
										 goto Z;
									}
									else goto M;
								}
							}
							if(code==2) {
								if(cur_blk!=t_blk || cur_cho!=t_cho) {
									vRevControlObj(n, cur_blk, cur_cho);
									cur_blk=t_blk;
									cur_cho=t_cho;
									vRevControlObj(n, cur_blk, cur_cho);
								}
								t=iVScrollBarObj(n, cur_blk);
								if(t!=-100) {
									if(t>choice[cur_blk]+pop[n].vb_scr[cur_blk]) {
										pop[n].value[cur_blk]=choice[cur_blk];
										pop[n].vb_scr[cur_blk]=t-choice[cur_blk];
									}
									else if(t>pop[n].vb_scr[cur_blk]-1){
										pop[n].value[cur_blk]=t-pop[n].vb_scr[cur_blk];
									}
									else {
										pop[n].value[cur_blk]=0;
										pop[n].vb_scr[cur_blk]=t;
									}
									vRevControlObj(n, cur_blk, cur_cho);
									cur_cho=pop[n].value[cur_blk];
									vDrawRecordObj(n, cur_blk);
									vRevControlObj(n, cur_blk, cur_cho);
								}
							}
						}
						else {
							 vRevControlObj(n, cur_blk, cur_cho);
							 while(iXYIsInArea(pop[n].x, pop[n].y, pop[n].lx, pop[n].ly)==0) {
								 if(lbp==0) break;
							 }
							 vRevControlObj(n, cur_blk, cur_cho);
							 break;
						}
					}
				}
			}
		}
		c.i=bioskey(0);
		vRevControlObj(n, cur_blk, cur_cho);
		vMCOFF();
		if(c.ch[0]) {
			switch(c.ch[0]) {
				case ESC :
OUT:			cur_blk=0;
					cur_cho=0;
					processing=0;
					return -1;
				case RETURN : case SPACE :
SP: RN:
					if(blk_attr[cur_blk] != 5) {
						if(pop[n].data[iCalcChiose(n, cur_blk, cur_cho)][0]=='2') break;
					}
					switch(blk_attr[cur_blk]) {
						case 0 : case 6 : case 7 : // all is iGetString
							if(is_out[cur_blk]) goto FOUT;
							cur_blk++;
							cur_cho=iGetNextObj(n, cur_blk, buf);
							break;
						case 1 :
							vSetDlgObj(n, cur_blk, cur_cho, buf);
							break;
						case 2 : case 3 :
							t_addr=buf;
							t_addr+=iGetObjOffset(n, cur_blk, cur_cho);
							t_cho=iCalcChiose(n, cur_blk, cur_cho);
							if(blk_attr[cur_blk]==2) {
								i=pop[n].data[t_cho][0]-48;
								if(i==1) {
									if(block_task) {
										t=(*t_addr);
										t=t-1;
										if(t==-1) t=2;
										(*t_addr)=t;
									}
									else (*t_addr)^=1;
								}
								else if(i==0) (*t_addr)^=1;
								else break;
							}
							else {
								if(block_task && *t_addr==cur_cho\
									 && pop[n].data[t_cho][0]=='1') (*t_addr)=10;
								else (*t_addr)=cur_cho;
							}
							vSetDlgObj(n, cur_blk, cur_cho, buf);
							break;
						case 4 :
							if(is_out[cur_blk]) {
FOUT: 					ret=cur_blk;
FOUT2:  				cur_blk=0;
								cur_cho=0;
								processing=0;
								return ret;
							}
							else {
								if(choice[cur_blk]>1) {
									pop[n].code[cur_blk][4]=cur_cho;
								}
								return cur_blk;
							}
						case 5 :
							t_addr=buf;
							t_addr+=iGetObjOffset(n, cur_blk, cur_cho);
							(*t_addr)=cur_cho+pop[n].vb_scr[cur_blk];
							vVBHead(n, cur_blk, cur_cho);
							if(is_out[cur_blk]) {
								cur_blk=0;
								cur_cho=0;
								processing=0;
								return cur_blk;
							}
							else if(c.ch[0]==RETURN) {
								cur_blk=pop[n].code[cur_blk][4];
								cur_cho=iGetNextObj(n, cur_blk, buf);
							}
							break;
					}
//					if(n == PW16 && cur_blk==0) {
//						if(
//					}
					break;
				case TAB :
					cur_blk++;
					if(cur_blk==pop[n].used) {
						cur_blk=0;
					}
					cur_cho=iGetNextObj(n, cur_blk, buf);
					break;
				default :
					if(num_able) {
						if(c.ch[0]>='0' && c.ch[0]<'6')	{
							ret=c.ch[0]-48+100;  goto FOUT2;
						}
					}
					if(c.ch[0]>31 && c.ch[0]<123) {
						if(c.ch[0]>96) c.ch[0]-=32;
						if(iCheckShortCut(n, &cur_blk, &cur_cho, c.ch[0])) {
							code=pop[n].block[cur_blk][4] & 0x0f;
							if(code==2 || code ==3) vPutKey(0, SPACE);
							else vPutKey(0, RETURN);
						}
					}
					break;
			}
		}
		else {
			switch(c.ch[1]) {
				case UP :
					if(blk_attr[cur_blk]!=5) {
						if(cur_cho==0) {
							cur_blk=pop[n].code[cur_blk][0];
							cur_cho=iGetNextObj(n, cur_blk, buf);
						}
						else cur_cho--;
					}
					else {
						if(cur_cho==0 && pop[n].vb_scr[cur_blk]!=0) {
							pop[n].vb_scr[cur_blk]--;
							vDrawRecordObj(n, cur_blk);
						}
						else if(cur_cho!=0) cur_cho--;
						vDrawScrollBarObj(n, cur_blk, cur_cho);
					}
					break;
				case DOWN :
					if(blk_attr[cur_blk]!=5) {
						if(cur_cho==choice[cur_blk]) {
							cur_blk=pop[n].code[cur_blk][1];
							cur_cho=iGetNextObj(n, cur_blk, buf);
						}
						else cur_cho++;
					}
					else {
						if(pop[n].p_n[cur_blk] < choice[cur_blk]+1) {
							if(cur_cho<pop[n].p_n[cur_blk]-1) cur_cho++;
						}
						else {
							if(cur_cho<choice[cur_blk]) {
								cur_cho++;
							}
							else if(pop[n].vb_scr[cur_blk]<pop[n].p_n[cur_blk]-choice[cur_blk]-1) {
								pop[n].vb_scr[cur_blk]++;
								vDrawRecordObj(n, cur_blk);
							}
						}
						vDrawScrollBarObj(n, cur_blk, cur_cho);
					}
					break;
				case LEFT :
					cur_blk=pop[n].code[cur_blk][2];
					cur_cho=iGetNextObj(n, cur_blk, buf);
					break;
				case RIGHT :
					cur_blk=pop[n].code[cur_blk][3];
					cur_cho=iGetNextObj(n, cur_blk, buf);
					break;
				case HOME : case PGUP :
					if(blk_attr[cur_blk]==5) {
						if(c.ch[1]==HOME && cur_cho!=0) {
							cur_cho=0;
							pop[n].value[cur_blk]=pop[n].vb_scr[cur_blk]=0;
							vDrawScrollBarObj(n, cur_blk, cur_cho);
						}
						else {
							t=pop[n].vb_scr[cur_blk];
							t-=choice[cur_blk]+1;
							if(t<0) {
								t=0;
								cur_cho=0;
							}
							pop[n].vb_scr[cur_blk]=t;
							pop[n].value[cur_blk]=cur_cho;
							vDrawRecordObj(n, cur_blk);
						}
					}
					else cur_cho=0;
					break;
				case END : case PGDN :
					if(blk_attr[cur_blk]==5) {
						if(pop[n].p_n[cur_blk]>choice[cur_blk]+1 && c.ch[1]==PGDN){
							t=pop[n].vb_scr[cur_blk];
							t+=choice[cur_blk];
							i=pop[n].p_n[cur_blk]-choice[cur_blk]-1;
							if(t>i) {
								t=i;
								cur_cho=choice[cur_blk];
								pop[n].value[cur_blk]=cur_cho;
							}
							pop[n].vb_scr[cur_blk]=t;
							vDrawRecordObj(n, cur_blk);
						}
						else {
							if(pop[n].p_n[cur_blk]<choice[cur_blk]+1) {
								cur_cho=pop[n].p_n[cur_blk]-1;
							}
							else cur_cho=choice[cur_blk];
							pop[n].value[cur_blk]=cur_cho;
							vDrawScrollBarObj(n, cur_blk, cur_cho);
						}
					}
					else cur_cho=pop[n].block[cur_blk][2]-1;
					break;
				case SFT_TAB :
					if(cur_blk>0) cur_blk--;
					else {
						cur_blk=pop[n].used-1;
					}
					cur_cho=iGetNextObj(n, cur_blk, buf);
					break;
				case F7 :
					if(iKeyMoveDlgWindow(n)) {
						vCloseDlgWindow(n);
						pop[n].x=pop_x; pop[n].y=pop_y;
						vOpenDlgWindow(n);
						vSetDlgWindow(n, buf);
					}
					break;
				case INSERT :
          if(ins_able==0) break;
					if(pop[n].p_n[cur_blk]>250) break;
					t_addr=buf;
					t_addr+=iGetObjOffset(n, cur_blk, cur_cho);
					(*t_addr)=cur_cho+pop[n].vb_scr[cur_blk];
					del_flag=1;
					return -10-cur_blk;
				case DELETE :
          if(del_able==0) break;
					if(pop[n].p_n[cur_blk]==0) break;
					t_addr=buf;
					t_addr+=iGetObjOffset(n, cur_blk, cur_cho);
					(*t_addr)=cur_cho+pop[n].vb_scr[cur_blk];
					del_flag=1;
					return -20-cur_blk;
				case F1 :
					vHelpDlg(POP_HELP+n);
					break;
				default :
					if(num_able) {
						if(c.ch[1]>=ALT_1 && c.ch[1]<=ALT_5) {
							ret=c.ch[1]-ALT_1+1+100;  goto FOUT2;
						}
						else if(c.ch[1]==ALT_0) {
							ret=100; goto FOUT2;
						}
					}
					break;
			}
		}
	}
}

#pragma warn -aus
int iUserChoiceDlg(char *str)
{
	INKEY c;
	char choice=0,i;
	BYTE tb, tc=0, move_flag=1;
	int mx, my;

ST:
	vOpenDlgWindow(PW1);
	vMCOFF();
	vWriteString(pop[PW1].x+22+15,pop[PW1].y+2,str,0x01);
	vRevControlObj(PW1,choice, 0);
	vMCON();
	for(;;) {
		while(!wCheckKeyHit()){
			if(rbp) {
				while(rbp);
				goto OUT;
			}
			getpos(mx, my);
			if(mx<pop[PW1].x+24 && move_flag) {
				i=iMoveDlgWindow(PW1);
				if(i==-3) {
					while(lbp);
					goto OUT;
				}
				if(i==-2) goto MOV;
			}
			else if(lbp) {
				tb=choice;
				move_flag=0;
				for(;;) {
					i=iExamineMCInObj(PW1, &tb, &tc);
					if(i==1) {
						if(tb!=choice) {
							vRevControlObj(PW1,choice, 0);
							choice=tb;
							vRevControlObj(PW1, choice, 0);
						}
					}
					else if(i==-1) {
						move_flag=1;
						if(iXYIsInObj(PW1, &tb, &tc)) {
							goto RN;
						}
					}
					else
						break;
				}
			}
		}
		c.i=bioskey(0);
		vRevControlObj(PW1, choice, 0);
		if(!c.ch[0]) {
			switch(c.ch[1]) {
				case RIGHT : case LEFT :
					choice=!choice; break;
				case F7 :
					if(iKeyMoveDlgWindow(PW1)) {
	MOV:			vCloseDlgWindow(PW1);
						pop[PW1].x=pop_x;
						pop[PW1].y=pop_y;
						goto ST;
					}
					break;
				case F1 :
					vHelpDlg(POP_HELP+PW1);
					break;
			}
		}
		else {
			if(c.ch[0]==SPACE) choice=!choice;
			else if(c.ch[0]=='Y' || c.ch[0]=='y' || c.ch[0]=='D' || c.ch[0]=='d') {
				choice=0;
				goto RN;
			}
			else if(c.ch[0]=='N' || c.ch[0]=='n' || c.ch[0]=='C' || c.ch[0]=='c') {
				choice=1;
				goto RN;
			}
			else if(c.ch[0]==RETURN) {
RN:     vCloseDlgWindow(PW1);
				return choice;
			}
			else if(c.ch[0]==ESC) {
OUT:		vCloseDlgWindow(PW1);
				return -1;
			}
		}
		vRevControlObj(PW1, choice, 0);
	}
}
#pragma warn +aus

int huge ihUserChoiceDlg(char *str)
{
	int i;

	i=iUserChoiceDlg(str);
	if(i==-1 || i==1) return 1;
	else return 0;
}

#define _SAVECURPATH vFileNameMerge2(src)
#define MAX_FILE  200

struct file_dat{
	WORD time;
	WORD date;
	unsigned long size;
	BYTE fname[15];   // SNode is flag is dir(0) or file(1) ?
	BYTE nick;
} far fbuf[MAX_FILE];

BYTE far NICK[100][21];

char scr_file=-1;    // CNode selected file number
int Cur_blk=0;       // the block of CNode Fname

BYTE can_get=10;
BYTE path_buf[80];
BYTE cur_fname[15];
BYTE cur_path[80];
WORD tot_file=0;     // total of files which could founded

int iFileListSearch(BYTE b, int n)       // Linear Search of fdbll
{
	BYTE str[3], dest[3];
	int i=0, rtn, t;

	if(tot_file==0) return 0;
	str[0]='0';
	str[1]=b;
	str[2]=0;
	if(fbuf[0].fname[1] >= b) {
		return 0;
	}
	if(fbuf[n+1].fname[1]==b) {
		i=n+1;
	}
	else i=0;
	for(;;) {
		memcpy(dest, fbuf[i].fname, 2);
		dest[2]=0;
		t=strcmp(str, dest);
		if(t > 0) {
			if(i<tot_file-1) {
				i++;
			}
			else {
				rtn=i;
				break;
			}
		}
		else if(t==0) {
			rtn=i;
			break;
		}
		else {
			rtn=i-1;
			break;
		}
	}
	return rtn;
}

BYTE g_drive[MAXDRIVE];
BYTE g_dir[80];
BYTE g_file[MAXFILE];
BYTE g_ext[MAXEXT];

int iFnSplit(BYTE *rev_buf)
{
	setmem(g_drive, MAXDRIVE, 0);
	setmem(g_dir, MAXDIR, 0);
	setmem(g_file, MAXFILE, 0);
	setmem(g_ext, MAXEXT, 0);
	return fnsplit(rev_buf, g_drive, g_dir, g_file, g_ext);
}

BYTE nick_lot=0;
extern BYTE nick_name[];
extern BYTE gwp_data[];

int iGetFileSubject(BYTE *str)
{
	FILE *fp;
	int rtn=0;

	if(nick_lot==100) return 0;

	iFnSplit(str);
	if((fp=fopen(str, "rb"))!=NULL) {
		if(strcmp(g_ext, ".GWP")==0) {
			if(iCheckGWP(1, fp, gwp_data)) {
				strcpy(NICK[nick_lot], nick_name);
				nick_lot++;
				rtn=1;
			}
		}
		else if((strcmp(g_ext, ".HWP")==0) || (strcmp(g_ext, ".11")==0)) {
			if(iCheckHWP(fp)) {
				strcpy(NICK[nick_lot], nick_name);
				nick_lot++;
				rtn=1;
			}
		}
		fclose(fp);
	}
	return rtn;
}

int iGetFileInfo(struct ffblk *fb)
{
	BYTE temp[80];
	int n;

	if(tot_file==MAX_FILE) {
		return 0;
	}
	n=tot_file;
	fbuf[n].date=fb->ff_fdate;
	fbuf[n].time=fb->ff_ftime;
	fbuf[n].size=fb->ff_fsize;
	strcpy(fbuf[n].fname+1, fb->ff_name);
	if(fb->ff_attrib==FA_ARCH) {
		fbuf[n].fname[0]='0';

		if(*path_buf) sprintf(temp, "%s\\%s", path_buf, fbuf[n].fname+1);
		else strcpy(temp, fbuf[n].fname+1);
		if(iGetFileSubject(temp)==0) {
			fbuf[n].nick=255; // 255 meaning No Nick name
		}
		else fbuf[n].nick=nick_lot-1;
	}
	else if(fb->ff_attrib==FA_DIREC) {
		fbuf[n].fname[0]='1';
		fbuf[n].nick=255;
	}
	else return 1;
	tot_file++;
	return 1;
}

int iCompFileName(const void *a, const void *b)
{
	struct file_dat *ta, *tb;
	ta=(struct file_dat *)a;
	tb=(struct file_dat *)b;
	return strcmp(ta->fname, tb->fname);
}

void vSortFileList(void)
{
	qsort((void *)fbuf, tot_file, sizeof(struct file_dat), iCompFileName);
}

BYTE bCheckFile(BYTE *buf)
{
  struct ffblk ffblk;
	return findfirst(buf, &ffblk, FA_ARCH | FA_RDONLY);
}


int iMakeFileList(BYTE check)
{
	int done;
	BYTE temp[80];
	struct ffblk file;

	nick_lot=tot_file=0;
	strcpy(temp, path_buf);
	if(check==1) {
		iFnSplit(temp);
		if(*g_dir==0) return 0;
		return findfirst(temp, &file, FA_DIREC);
	}
	if(*temp!=0)	strcat(temp, "\\");
	strcat(temp, cur_fname);
	done=findfirst(temp, &file, FA_ARCH | FA_RDONLY);
	while(!done) {
		if(file.ff_attrib==FA_ARCH) {
			if(!iGetFileInfo(&file)) return 1;
		}
		done=findnext(&file);
	}
	if(*path_buf) sprintf(temp, "%s\\*.*", path_buf);
	else strcpy(temp, "*.*");
	done=findfirst(temp, &file,FA_DIREC);
	while(!done) {
		if(file.ff_attrib==FA_DIREC) {
			if(strcmp(file.ff_name,".")!=0) {
				if(!iGetFileInfo(&file)) return 1;
			}
		}
		done=findnext(&file);
	}
	if(tot_file==0) {
		return -1;
	}
	return 1;              // '0' means not enough memory '-1'means no match
}

int iDispFileList(int st, int len, int x, int y)
{
	int i;
	WORD d1, d2;
	WORD sz;

	if(tot_file==0) {
		vWriteString(x, y, "<Ìa·©·¡ ´ô·s“¡”a>", 0x20);
		return 0;
	}
	for(i=st;i<st+len;i++) {
		if(i==tot_file) return i-st;
		d1=fbuf[i].date;
		d2=fbuf[i].time;
		sz=(WORD)(fbuf[i].size>>10);
		if(sz==0) sz=1;
		if(fbuf[i].fname[0]=='0') {
			vWriteString(x+(i-st)*16, y, fbuf[i].fname+1, 0);
			iPrintf(x+(i-st)*16, y+13,0,"%4uk %02d/%02d/%02d %02d:%02d",sz, ((d1>>9)+1980)%100,
								(d1<<7)>>12,d1 & 0x001f,d2>>11,(d2<<5)>>10);

			sprintf(g_dir, "%s\\%s", path_buf, fbuf[i].fname);
			if(fbuf[i].nick!=255) {
				vPutStringLength(x+(i-st)*16, y+34, NICK[fbuf[i].nick], 0, 0, 20);
			}
		}
		else {
			iPrintf(x+(i-st)*16, y, 0, "%s\\", fbuf[i].fname+1);
			iPrintf(x+(i-st)*16, y+13,0,"<DIR> %02d/%02d/%02d %02d:%02d",((d1>>9)+1980)%100,
								(d1<<7)>>12,d1 & 0x001f,d2>>11,(d2<<5)>>10);
		}
	}
	return -1;
}

void vSetDriverName(void)
{
	union REGS r;
	r.h.ah=0x44;
	r.h.al=0x0f;
	r.h.bl=1;
	intdos(&r, &r);
}

int iCheckDriver(BYTE drv)
{
	union REGS r;

	drv-='A';
	r.h.ah=0x15;
	r.h.dl=drv;
	int86(0x13, &r, &r);
	return r.h.ah;
}

void vFileNameSplit2(BYTE *src)
{
	iFnSplit(src);
	strcpy(path_buf, g_drive);
	strcat(path_buf, g_dir);
	if(path_buf[strlen(path_buf)-1]=='\\') path_buf[strlen(path_buf)-1]=0;
	strcpy(cur_fname, g_file);
	strcat(cur_fname, g_ext);
}

void vFileNameMerge2(BYTE *dest)
{
	if(path_buf[strlen(path_buf)-1]=='\\') path_buf[strlen(path_buf)-1]=0;
	if(*path_buf) sprintf(dest, "%s\\%s", path_buf, cur_fname);
	else strcpy(dest, cur_fname);
}


BYTE fext[5]={".GWP"};


void vSetExt(BYTE *name)
{
	strcpy(fext, name);
}


int iAnalFileName(BYTE *src)
{
	BYTE dest[80];
	setmem(dest,80,0);
	iFnSplit(src);
	if(*g_file==0) strcpy(g_file, "*");
	if(*g_ext==0) strcpy(g_ext, fext);
	if(*g_drive==0 && *g_dir==0) {
		*path_buf=0;
		strcpy(cur_fname, g_file);
		strcat(cur_fname, g_ext);
		strcpy(src, cur_fname);
		if(strchr(cur_fname, '*')!=NULL || strchr(cur_fname, '?')!=NULL) return 3;
		return 1;
	}
	else {
		if(*g_drive==0) {
			sprintf(dest, "%c:", getdisk()+'A');
			strcpy(g_drive, dest);
		}
		else {
			vSetDriverName();
			if(g_drive[0]=='B')
				if(iCheckDriver('B')==0) g_drive[0]='A';
		}
		if(*g_dir==0) {
			g_dir[0]='\\';
			if(getcurdir(*g_drive-'A'+1, g_dir+1)==-1) return 5;  //reget_string
		}
		else if(getcurdir(*g_drive-'A'+1, path_buf)==-1) return 5; //check drive..

		strcpy(path_buf, g_drive);
		strcat(path_buf, g_dir);
		if(path_buf[strlen(path_buf)-1]=='\\') path_buf[strlen(path_buf)-1]=0;
		strcpy(cur_fname, g_file);
		strcat(cur_fname, g_ext);
		_SAVECURPATH;
		if(strchr(cur_fname, '*')!=NULL || strchr(cur_fname, '?')!=NULL) return 3;
		return 2;
	}
}

WORD wGetDiskFreeSpace(BYTE *rev_buf)
{
	BYTE drive;
	long avail;
	struct dfree disk;

	iFnSplit(rev_buf);
	if(g_drive[0]==0) {
		 drive=getdisk();
	}
	else drive=g_drive[0]-'A';
	getdfree(drive+1, &disk);
	avail=(long)(disk.df_avail)
				 *(long)(disk.df_bsec*disk.df_sclus);
	if(avail==0) avail=1;
	else avail >>= 10;
	return (WORD)avail;
}


void vAnalSlash(BYTE *dest, BYTE *src)
{
	int i,j;

	i=strlen(dest);

	if(strcmp(src, "..")==0) {
		for(j=i;j>1;j--) {
			if(dest[j]=='\\') {
				dest[j]=0;
				break;
			}
		}
	}
	else {
		if(dest[strlen(dest)-1]!='\\') strcat(dest, "\\");
		strcat(dest,src);
	}
}

void vDeleteFile(int n)
{
	int i=0;

	for(i=n; i<tot_file-1; i++) {
		memcpy(&(fbuf[i]), &(fbuf[i+1]), sizeof(struct file_dat));
	}
}

void vGetDlgObjXY(int *x, int *y, int *ex, int *ey, BYTE blk)
{
	*x=pop[PW4].block[blk][0]*8+pop[PW4].x+24;
	*y=pop[PW4].block[blk][1]+pop[PW4].y;
	*ex=*x+pop[PW4].block[blk][2]*16;
	*ey=pop[PW4].y+pop[PW4].block[blk][3];
}

int iIsDone(void)
{
	int x, y, ex, ey;
	int mx, my, ret=0, flag=0;

	getpos(mx, my);
	vGetDlgObjXY(&x, &y, &ex, &ey, 2);
	if(mx>x && mx<ex && my>y*8 && my<ey*8) {
		vRevControlObj(PW4, 2, 0); flag=1;
	}
	while(lbp) {
		getpos(mx, my);
		if(mx>x && mx<ex && my>y*8 && my<ey*8) {
			if(lbp==0) {
				ret=1;	goto OUT;
			}
		}
		else goto OUT;
	}
OUT :
	if(flag) vRevControlObj(PW4, 2, 0);
	return ret;
}


void vDispFileListBox(int n)
{
	int x, ex, y, ey;
	int i, j;

	if(n==100) {
		vOpenDlgWindow(PW4);
		vMCOFF();
		vGetDlgObjXY(&x, &y, &ex, &ey, 1);
		vDrawHScrollBarBit(ex+3, y*8-2, ey*8+2);
		vGetDlgObjXY(&x, &y, &ex, &ey, 0);
		vFileNameMerge2(g_dir);
		vPutStringLength(x, y, g_dir, 0, 0, ey-y-1);
		vWriteString(x, pop[PW4].y+2, "Ìa·©¡w", 0);
	}
	if(n==100 || n==-1)	{
		vGetDlgObjXY(&x, &y, &ex, &ey, 1);
		vBarByte(x, y, ex, ey, 0);
		j=iDispFileList(Cur_blk*10, 10, x, y+1);
		if(j!=-1) can_get=j;
		else can_get=10;
	}
	if(n==100 || n==0) {
		vGetDlgObjXY(&x, &y, &ex, &ey, 1);
		for(j=0;j<10;j++) {
			vWriteString(x+j*16, y, (scr_file==j) ? ">" : " " ,0);
		}
	}
	if(n==100 || n==2) {
		vGetDlgObjXY(&x, &y, &ex, &ey, 1);
		vBarByte(ex+21, y, ex+36, ey, 0);
		i=37-strlen(cur_fname);
		j=0;
		if(strlen(path_buf) > i) {
			j=path_buf[i];
			path_buf[i]='\0';
		}
		if(*path_buf) {
			if(j) {
				iPrintf(ex+21, y, 0, "%s...\\%s", path_buf, cur_fname);
				path_buf[i]=j;
			}
			else iPrintf(ex+21, y, 0, "%s\\%s", path_buf, cur_fname);
		}
		else {
			iPrintf(ex+21, y, 0, "%s\\%s", cur_path, cur_fname);
		}
		i=wGetDiskFreeSpace(path_buf);
		iPrintf(ex+21, y+43,0,"%6uk q·q",i);
	}
	if(n==99) {
		vCloseDlgWindow(PW4);
	}
}

static BYTE sfname[15];
void vEraseOldFileName(void)
{
	sfname[0]=0;
}

int iSetFileList(int flag)
{
	int old;
	static int i;
	static BYTE spath[80];
	static BYTE old_cwd[80];

	old=Cur_blk;
	scr_file=-1;
	Cur_blk=0;
	if(old>0 && flag!=2) {
		vDispFileListBox(-1);
		vDispFileListBox(2);
	}
	fnsplit(path_buf, g_drive, g_dir, g_file, g_ext);
	if(g_drive[0] !='A' && g_drive[0]!='B') {
		if(flag>0 && strcmp(spath, path_buf)==0 && strcmp(sfname, cur_fname)==0 \
							&& strcmp(old_cwd, cur_path)==0) {
			return i;
		}
	}
	if(iMakeFileList(1)!=0) {
		vErrorMsgfDlg(10, 7, 0);
		return -1;
	}
	i=iMakeFileList(0);
	vSortFileList();
	if(flag==1) {
		vDispFileListBox(-1);
		vDispFileListBox(2);
	}
	strcpy(spath, path_buf);
	strcpy(sfname, cur_fname);
	strcpy(old_cwd, cur_path);
	return 1;
}

void vSetFileListHScrollBar(int i,int *old_i)
{
	int x, y, ex, ey;
	vMCOFF();
	vGetDlgObjXY(&x, &y, &ex, &ey, 1);
	vDrawHThumbBit(ex+3, y*8-2, ey*8+2, (tot_file-1)/10, i, *old_i);
	vWLineYBit(ex+3, y*8-2, ey*8+2);
	*old_i=i;
}

void vExitFileListDlg(void)
{
	vDispFileListBox(99);
}

void vRevFileListObj(int choice)
{
	if(choice==0) {
		vRevControlObj(PW4, 0, 0);
		return;
	}
	else if(choice<11) vRevControlObj(PW4, 1, choice-1);
	else vRevControlObj(PW4, choice-9, 0);
}

void vGetBlock(int choice, BYTE *cb, BYTE *cc)
{
	if(choice==0) {
		*cb=0; *cc=0;
	}
	else if(choice<11) {
		*cb=1; *cc=choice-1;
	}
	else {
		*cb=choice-9; *cc=0;
	}
}

int iFileListDlg(BYTE *src, BYTE *fname, BYTE rev_code)
{
	int x, y, ex, ey;
	int mx, my;
	int i=0,old_i=0;
	int choice=0,k,j;
	int code1,code2, t_n;
	char code=0, ret_code=1;
	BYTE cb, cc, tb, tc, ob, oc;
	BYTE tb1, tc1, getstr_flag=0;
	BYTE mem[100];
	INKEY c;

	vMCOFF();
	getcwd(cur_path,80);
	if(rev_code==LOAD) {
		strcpy(menu_PW4[12], Dlog_menu2[0]);
		if(cur_w==0) menu_PW4[12][0]='2';
		else menu_PW4[12][0]='0';
	}
	else strcpy(menu_PW4[12], Dlog_menu2[1]);
	iFnSplit(src);
	if(rev_code!=SAVE_AS) {
		if(strchr(g_file, '*')==0 && strchr(g_file, '?')==0) {
			strcpy(g_file, "*");
		}
	}
	else strcpy(g_ext, fext);
	fnmerge(src, g_drive, g_dir, g_file, g_ext);
	vFileNameSplit2(src);
	strcpy(pop[PW4].headline, Head[rev_code]);
	if(0 == iSetFileList(2)) return 0;
	vDispFileListBox(100);
	vSetFileListHScrollBar(0, &old_i);
	vRevFileListObj(0);
	setmem(mem, 80, 0);
ST:
	for(;;) {
		vSetFileListHScrollBar(i, &old_i);
		vMCON();
		vFileNameMerge2(mem);
		for(;!choice;) {
			vGetDlgObjXY(&x, &y, &ex, &ey, 0);
			code1=iGetString(0, x, y, 78, ey-y, mem);
			for(k=0;mem[k];k++) mem[k]=toupper(mem[k]);
			if(code1==1 || code1==-20 || (code1==-2 && (rev_code==LOAD || rev_code==READ_BLK)) ||
										 (code1==-4 && iIsDone())) {
				code2=iAnalFileName(mem);
				vPutStringLength(x, y, mem, 0, 0, ey-y-1);

				i=0;
				vSetFileListHScrollBar(i, &old_i);
				if(code2==5) continue;
				if(code2==3) {
					code2=iSetFileList(1);
					if(code2==0) {
						goto OUT;
					}
					else if(code2==-1) continue;
					else {
						vPutKey(DOWN, 0);
						break;
					}
				}
				else if(code2==2) {
					if(iMakeFileList(1)==0) {
						vFileNameMerge2(fname);
						vExitFileListDlg();
						_SAVECURPATH;
						if(code1==-20) return 2;
						return 1;
					}
					else {
						vErrorMsgfDlg(10, 7, 0);
						continue;
					}
				}
				else {
					strcpy(fname, cur_fname);
					vExitFileListDlg();
					_SAVECURPATH;
					if(code1==-20) return 2;
					return 1;
				}
			}
			else if(code1==-1) goto OUT;
			else {
				vBarByte(x, y, ex, ey, 0);
				vFileNameMerge2(g_dir);
				vPutStringLength(x, y, g_dir, 0, 0, ey-y-1);
				if(code1==-2)       vPutKey(DOWN, 0);
				else if(code1==-3) 	vPutKey(UP, 0);
				else if(code1==-5) 	vPutKey(PGDN, 0);
				else if(code1==-10) vPutKey(F7, 0);
				else if(code1==-11) vPutKey(F1, 0);
				else if(code1==-4) {
					vMCON();
					getstr_flag=1;
					goto Z;
				}
				else goto ST;
				break;
			}
		}
		while(!wCheckKeyHit()) {
			if(choice==0 && getstr_flag) {
				getstr_flag=0;
				goto ST;
			}
Z:
			vGetDlgObjXY(&x, &y, &ex, &ey, 1);
			k=iHScrollBarHandler(ex+3, y*8-2, ey*8+2, (tot_file-1)/10, i, old_i, 0,(tot_file-1)/10);
			if(k!=-100) {
				if(i!=k) {
					i=k;
					if(i>(tot_file-1)/10) break;
					Cur_blk=i;
					old_i=i;
					scr_file=-1;
					vRevFileListObj(choice);
					vDispFileListBox(-1);
					vDispFileListBox(0);
					if(choice>can_get) choice =can_get;
					vRevFileListObj(choice);
				}
			}
			getpos(mx, my);
			if(ex+3>mx || ex+20<mx) {
				for(;;) {
					if(rbp) {
						while(rbp);
						goto OUT;
					}
					if(!lbp) break;
					getpos(mx, my);
					if(mx>pop[PW4].x && mx<pop[PW4].x+24 && my>pop[PW4].y*8 &&\
						 my<(pop[PW4].y +pop[PW4].ly)*8) {
						code=iMoveDlgWindow(PW4);
						if(code==-3) {
							while(lbp);
							goto OUT;
						}
						if(code==-2) {
							goto MOV;
						}
						break;
					}
					else if(lbp) {
						vGetBlock(choice, &cb, &cc);
						tb=cb; tc=cc;
						ob=cb; oc=cc;
						for(;;) {
							code=iExamineMCInObj(PW4, &tb, &tc);
							if(iXYIsInArea(pop[PW4].x, pop[PW4].y, pop[PW4].lx, pop[PW4].ly)==1) {
								if(code==1) {
									if(cb!=tb || cc!=tc) {
										if(tc<can_get) {
											vRevControlObj(PW4, cb, cc);
											cb=tb;
											cc=tc;
											choice=iCalcChiose(PW4, cb, cc);
											vRevControlObj(PW4, cb, cc);
										}
									}
								}
								if(code==-1) {
									vRevControlObj(PW4, cb, cc);
									choice=iCalcChiose(PW4, cb, cc);
									if((ob==cb && oc==cc && scr_file!=-1) || choice>10) {
										if(iXYIsInObj(PW4, &tb1, &tc1)==1) {
											goto RN;
										}
										else {
											vRevControlObj(PW4, cb, cc);
											if(choice !=0) goto Z;
											else goto ST;
										}
									}
									else {
										if(choice==scr_file+1) goto RN;
										goto SP;
									}
								}
							}
							else {
								vRevControlObj(PW4, cb, cc);
								while(iXYIsInArea(pop[PW4].x, pop[PW4].y, pop[PW4].lx, pop[PW4].ly)==0) {
									if(lbp==0) {
										vRevControlObj(PW4, cb, cc);
										goto Z;
									}
								}
								vRevControlObj(PW4, cb, cc);
							}
						}
					}
				}
			}
		}
		vRevFileListObj(choice);
		vMCOFF();
		c.i=bioskey(0);
		if(c.ch[0]) {
			switch(c.ch[0]) {
				case ESC   :
OUT:
					vExitFileListDlg();
					return 0;
				case SPACE :
SP:					if(choice>0 && choice <11) {
						if(scr_file==choice-1) scr_file=-1;
						else scr_file=choice-1;
						vDispFileListBox(0);
					}
					break;
				case RETURN :
RN:				if(choice==13) goto OUT;
					if(choice==12) {
						if(rev_code==LOAD) {
							if(cur_w==0) break;
							if(scr_file==-1) {
								vErrorMsgfDlg(10, 8, 0);
								break;
							}
							else {
								choice=scr_file+1;
								ret_code=2;               // means change cur_win file
								goto SEND;
							}
						}
						else {
							vHelpDlg(POP_HELP+PW4);
							break;
						}
					}
					if(choice==11) {
						if(scr_file==-1) {
							vErrorMsgfDlg(10, 8, 0);
							break;
						}
						else {
							choice=scr_file+1;
							goto SEND;
						}
					}
					if(choice>0 && choice<11) {
SEND:				scr_file=choice-1;
SEND2:			t_n=Cur_blk*10+scr_file;
						if(fbuf[t_n].fname[0]=='0') {        // if normal file
							scr_file=choice-1;
							vDispFileListBox(0);
							if(*path_buf)
								sprintf(fname, "%s\\%s", path_buf, fbuf[t_n].fname+1);
							else strcpy(fname, fbuf[t_n].fname+1);
							_SAVECURPATH;
							vExitFileListDlg();
							return ret_code;
						}
						else {                                 // if it is directory
							i=0;
							vSetFileListHScrollBar(0, &old_i);
							if(*path_buf==0)	strcpy(path_buf, cur_path);
							vAnalSlash(path_buf, fbuf[t_n].fname+1);
							vGetDlgObjXY(&x, &y, &ex, &ey, 0);
							vBarByte(x, y, ex, ey, 0);
							vFileNameMerge2(g_dir);
							vPutStringLength(x, y, g_dir, 0, 0, ey-y-1);
							iSetFileList(1);
							if(choice>can_get) choice=can_get;
							ret_code=1;
						}
					}
					if(choice==11) {
						t_n=Cur_blk*10+scr_file;
						strcpy(fname, fbuf[t_n].fname+1);
						_SAVECURPATH;
						vExitFileListDlg();
						return 1;
					}
					break;
				default :
					if(choice<1 || choice >10) break;
					if(c.ch[0]>47 && c.ch[0]<123) {
						if(c.ch[0]>96) c.ch[0]-=32;
						j=iFileListSearch(c.ch[0], Cur_blk*10+choice-1);
						if(Cur_blk != (j/10)) {
							Cur_blk=j/10;
							vDispFileListBox(-1);
						}
						choice=(j%10)+1;
						scr_file=-1;
						i=Cur_blk;
					}
					break;
			}
		}
		else {
			switch(c.ch[1]) {
				case UP :
					if(choice>10) {
						choice=10;
						if(choice>can_get) {
							choice=can_get;
							break;
						}
					}
					else {
						choice--;
						if(choice<0) choice=11;
					}
					break;
				case DOWN :
					if(choice>10) choice=0;
					else {
						if(choice==can_get) choice=10;
						choice++;
					}
					break;
				case RIGHT : case PGDN :
					if(choice>0 && choice<11) {
						if(((tot_file-1)/10) <= Cur_blk) break;
						Cur_blk++;
						i++;
						scr_file=-1;
						vDispFileListBox(-1);
						vDispFileListBox(0);
						if(choice>can_get) choice =can_get;
					}
					else if(choice>10 && choice<13) choice++;
					break;
				case LEFT : case PGUP :
					if(choice>0 && choice <11) {
						if(Cur_blk==0) break;
						Cur_blk--;
						i--;
						scr_file=-1;
						vDispFileListBox(-1);
						vDispFileListBox(0);
						if(choice>can_get) choice =can_get;
					}
					if(choice==11) break;
					if(choice>11 &&
						 choice<14) choice--;
					break;
				case F7 :
					if(iKeyMoveDlgWindow(PW4)) {
MOV:				vCloseDlgWindow(PW4);
						pop[PW4].x=pop_x;
						pop[PW4].y=pop_y;
						vDispFileListBox(100);
					}
					break;
				case CTRL_LARROW :  case CTRL_PGUP :
					if(Cur_blk==0) break;
					Cur_blk=0;
					i=0;
					vSetFileListHScrollBar(i,&old_i);
					scr_file=-1;
					vDispFileListBox(-1);
					vDispFileListBox(0);
					choice=1;
					break;
				case CTRL_RARROW :  case CTRL_PGDN :
					if(Cur_blk==(tot_file-1)/10) break;
					Cur_blk=(tot_file-1)/10;
					i=Cur_blk;
					vSetFileListHScrollBar(i,&old_i);
					scr_file=-1;
					vDispFileListBox(-1);
					vDispFileListBox(0);
					choice=can_get;
					break;
				case HOME :
					choice=1;
					break;
				case END :
					choice=10;
					if(choice>can_get) choice=can_get;
					break;
				case F1 :
					vHelpDlg(POP_HELP+PW4);
					break;
				case F3 :
					if(rev_code!=0) break;
					ret_code=2;
					if(choice>0 && choice <11) goto SEND;
					else if(scr_file==-1) vErrorMsgfDlg(10, 8, 0);
					else goto SEND2;
					break;
				case DELETE :
					if(choice>10) break;
					scr_file=choice-1;
					t_n=Cur_blk*10+scr_file;
					vDispFileListBox(0);
					if(fbuf[t_n].fname[0]=='1') {
						vErrorMsgfDlg(10, 10, 0);
						break;
					}
					else {
						if(*path_buf) sprintf(g_dir, "%s\\%s", path_buf, fbuf[t_n].fname+1);
						else strcpy(g_dir, fbuf[t_n].fname+1);
						if(ihUserChoiceDlg("¸÷ i »¡¶¯¡‰V¯s“¡Œa?")) break;
						if(unlink(g_dir)==-1) {
							vErrorMsgfDlg(10, 10, 0);
							break;
						}
						else {
							scr_file=-1;
							vDeleteFile(t_n);
							tot_file--;
							vSetFileListHScrollBar(i,&old_i);
							vDispFileListBox(-1);
							vDispFileListBox(0);
							break;
						}
					}
			}
		}
		vRevFileListObj(choice);
	}
}

#pragma warn -aus

void huge vErrorMsgDlg(BYTE *str)
{
	INKEY c;
	WORD j;
	BYTE tb, tc;
	char i, len;
	int mx, my;

	i=bGetGraphMode();
	if(i<4 || i==7) return;

	len=strlen(str);
	if(len>26) {
		pop[PW0].ly=len+4;
		pop[PW0].block[0][1]=(len+4)/2-8;
		pop[PW0].block[0][3]=pop[PW0].block[0][1]+16;
	}
	else {
		pop[PW0].ly=30;
		pop[PW0].block[0][1]=7;
		pop[PW0].block[0][3]=24;
	}
ST:
	vOpenDlgWindow(PW0);
	vMCOFF();
	vWriteString(pop[PW0].x+22+15, pop[PW0].y+2, str, 0x01);
	vMCON();
	for(;;) {
		while(!wCheckKeyHit()) {
			if(rbp) {
				while(rbp);
				goto OUT;
			}
			getpos(mx, my);
			if(mx<pop[PW0].x+24) {
				i=iMoveDlgWindow(PW0);
				if(i==-3) {
					while(lbp);
					goto OUT;
				}
				if(i==-2) goto MOV;
			}
			else if(lbp) {
				i=iExamineMCInObj(PW0, &tb, &tc);
				if(i==1) {
					while(lbp);
					goto OUT;
				}
			}
		}
		c.i=bioskey(0);
		if(c.ch[0]) {
			if(c.ch[0]==ESC || c.ch[0]==RETURN) {
OUT:		vRevControlObj(PW0, 0, 0);
				for(j=0;j<60000l;j++);
				vRevControlObj(PW0, 0, 0);
				vCloseDlgWindow(PW0);
				return;
			}
		}
		else {
			if(c.ch[1]==F7) {
				if(iKeyMoveDlgWindow(PW0)) {
MOV:			vCloseDlgWindow(PW0);
					pop[PW0].x=pop_x;
					pop[PW0].y=pop_y;
					goto ST;
				}
			}
			else if(c.ch[1]==F1) {
				vHelpDlg(POP_HELP+PW0);
			}
		}
	}
}
#pragma warn +aus

BYTE *Str_err[]={
	"",
	" Ìa·©·i µi ® ´ô¯s“¡”a", //Cannot read
	" Ìa·©·i ³i ® ´ô¯s“¡”a",   //Cannot write
	" Ìa·©·i Àx·i ® ´ô¯s“¡”a",  //Cannot found
	" Ñw¯¢· Ìa·©·¡ ´a“³“¡”a",
	" µAœá·³“¡”a",
	" ¤á¹e·¡ ËiŸ³“¡”a",
	" Ñw¯¢ Ìa·©·¡ ´a“³“¡”a",
};


BYTE *Sys_err[]={
	"¡A¡¡Ÿ¡Ÿi Ði”wÐi ® ´ô¯s“¡”a", //Memory allocate failure
	"ÑÂ¸w ¡A¡¡Ÿ¡ˆa ¦¹¢Ðs“¡”a",    //Not enough Extention Memory
	"¡A¡¡Ÿ¡ ·¡•··i ¯©ÌÐ–¯s“¡”a",  //Memory movement failure
	"Ð”w Íe»³Àw·¡ ´ô¯s“¡”a",
	"¦µAœá·³“¡”a. ESTgroup·a¡ µeœbÐa¯³¯¡µ¡", //Internal error
};

BYTE *Etc_err[]={
	"",
	"Íe»³Àw·e 5ˆ¡ ¹AÐe–A ·¶¯s“¡”a",
	"Å¡—a ¥eÑÅ µAœá·³“¡”a",
	"¬é¸÷–E §iœâ·¡ ´ô¯s“¡”a",
	"§iœâ ¬wÈŸi Ð¹AÐa¯³¯¡µ¡",
	"µe¯s¸wµA¬å ·¡¶wÐi ® ´ô¯s“¡”a",
	"Àx“e ¢…¸aµi·¡ ´ô¯s“¡”a",
	"—¡BÉ¡Ÿ¡ˆa ËiŸ³“¡”a",
	"Ìa·©·i ¬åÈ‚Ða¯³¯¡µ¡(Space)",
	"10ˆ ·¡¬w —w¢Ði ® ´ô¯s“¡”a",
	"»¡¶‰ ® ´ô¯s“¡”a",
	"¦‰µÅ¸åÐe Ìa·©·³“¡”a.",
};


BYTE err_buf[80];

void huge vErrorMsgfDlg(BYTE type, BYTE num, BYTE *str)
{
	switch(type) {
		case 0 : // Argument String Merge error!!
			sprintf(err_buf, "%s%s", str, Str_err[num]);
			break;
		case 1 : // System error!!
			strcpy(err_buf, Sys_err[num]);
			break;
		case 10 : // Etc error!!
			strcpy(err_buf, Etc_err[num]);
			break;
		default :
			sprintf(err_buf, "Fatal Error : Type(%d) Code(%d)", type, num);
			break;
	}
	strcat(err_buf, "!!!");
	vErrorMsgDlg(err_buf);
}



void vPutMenuObj2(int n, int blk, int y, BYTE *menu, BYTE len)
{
	int c;
	c=iCalcChiose(n, blk, 0);
	memcpy(pop[n].data[c]+y, menu, len);
	vMCOFF();
	vWriteString(pop[n].block[blk][0]*8+pop[n].x+24,
						 pop[n].y+pop[n].block[blk][1], pop[n].data[c]+1, 0);
	vMCON();
}

void vPutMenuObj(int n, int blk, int y, BYTE *menu)
{
	vPutMenuObj2(n, blk, y, menu, strlen(menu)-1);
}

extern BYTE load_tab;

int iLoadOptDlg(BYTE *cod, BYTE *way)
{
	BYTE l_opt[20];
	int i;
	setmem(l_opt, 20, 0);

	l_opt[0]=2;
	l_opt[1]=1;
	l_opt[2]=1;
	l_opt[4]=load_tab;
	l_opt[6]=16;
	strcpy(l_opt+8, "Äéœñ ");
	vOpenDlgWindow(PW10);
	i=iDialogDriver(PW10, (BYTE *)l_opt);
	vCloseDlgWindow(PW10);
	if(i==3) {
		*cod=l_opt[0];
		*way=l_opt[1];
		load_tab=l_opt[4];
		return 1;
	}
	else return 0;
}

BYTE Save_buf[170];
extern BYTE save_null;

int iSaveOptDlg(BYTE *cod, BYTE *way)
{
	int i;

	setmem(Save_buf+33, 136, 0);
	Save_buf[0]=0;
	Save_buf[1]=0;
	Save_buf[2]=save_null;
	vOpenDlgWindow(PW16);
	i=iDialogDriver(PW16, (BYTE *)Save_buf);
	vCloseDlgWindow(PW16);
	if(i==4) {
		*cod=Save_buf[0];
		*way=Save_buf[1];
		save_null=Save_buf[2];
		return 1;
	}
	else return 0;
}

extern BYTE disp_return;
extern BYTE disp_newline;
extern BYTE disp_space;
extern BYTE full_vector;

void vDispOptDlg(void)
{
	BYTE dat[4];
	int i;

	dat[0]=disp_return;
	dat[1]=disp_newline;
	dat[2]=disp_space;
	dat[3]=full_vector;
	vOpenDlgWindow(PW11);
	i=iDialogDriver(PW11, (BYTE *)dat);
	if(i==2) {
		vSetDispOpt(dat[2], dat[1], dat[0]);
		full_vector=dat[3];
	}
	vCloseDlgWindow(PW11);
}

void vWipeStringArray(BYTE total, BYTE n, BYTE **array)
{
	int i;
	for(i=n; i<total-1; i++) {
		strcpy(array[i], array[i+1]);
	}
}

void vTestDlg(void)
{
	BYTE p_n, vb=0, value, set=0;
	BYTE process;
	BYTE buf[205];
	int i;
	p_n=20;
	process=0;
	value=0;
	setmem(buf, 205, 0);
	buf[1]='\r';
	vMakeExtDlgWindow(PW13, (BYTE *)v_data, &p_n, &process, &vb, &value, &set);
	vOpenDlgWindow(PW13);
	for(;;) {
		i=iDialogDriver(PW13, buf);
		switch(i) {
			case -1 :
				goto OUT;
			case  0 :  // Write in variable block
				break;
			case  2 :  // Write in iGetString block
				break;
			case  3 :  // Update Word;
				break;
			case  4 :  // Cancle
				goto OUT;
			case -3 :  // Delete WORD
				vWipeStringArray(10, buf[0], v_data);
				p_n--;
				break;
		}
	}
OUT :
	vCloseDlgWindow(PW13);
}

extern BYTE help_data[];

//BYTE help_buf[4000];
BYTE *help_buf;
BYTE U_able;
BYTE D_able;
int help_line;

int iGetHelpData(int n)
{
	FILE *fp;
	unsigned long loc;
	unsigned long tmp;
	WORD len;
	int i;

	iMakeMem();
	sprintf(g_dir, "%s\\21.HLP", exe_path);
	if((fp=fopen(g_dir, "rb"))==NULL) {
		vErrorMsgfDlg(0, 1, "21.HLP");
		return 0;
	}
	if(!iCheckGWP(0, fp, help_data)) {
		fclose(fp);
		vErrorMsgfDlg(0, 7, "•¡¶‘ i");
		return 0;
	}
	setmem(help_buf, 8000, 0);
	fseek(fp, (long)n*4+48, SEEK_SET);
	fread(&loc, 4, 1, fp);
	fread(&tmp, 4, 1, fp);
	len=(WORD)(tmp-loc);
	fseek(fp, loc, SEEK_SET);
	fread(help_buf, len, 1, fp);
	fclose(fp);
	help_line=0;
	for(i=0; i<len; i++) {
		if(help_buf[i]==0) help_line++;
	}
	return 1;
}

WORD wGetHelpDataLoc(int lnum, BYTE *buf)
{
	int i=0;
	WORD loc=1;

	while(i<lnum) {
		while(buf[loc]!=0) loc++;
		loc++;
		i++;
	}
	return loc;
}

void vDispHelpDlg(int n)
{
	int x, y, ex, ey, t, i, line=12;
	WORD loc=0;

	x=pop[PW15].x+24+8; // Bit
	y=pop[PW15].y+2;   // Byte
	ex=x+18*12;
	ey=pop[PW15].y+56;

	vMCOFF();
	if(n==100) {
		vOpenDlgWindow(PW15);
		vMCOFF();
		iPrintf(pop[PW15].x+4, pop[PW15].y+48, 0x04, "%4luKq·q", \
							farcoreleft()>>10);
		vNBoxBit(x-2, y*8-2, ex+2, ey*8+2, 1);
	}
	if(n==100 || n==1) {
		t=pop[PW15].vb_scr[0];
		loc=wGetHelpDataLoc(t, help_buf);
		if(help_line<12) line=help_line;
		for(i=0; i<line; i++) {
			vPutStringLength(x+18*i, y+2, help_buf+loc, 0, 0, 50);
			loc+=strlen(help_buf+loc)+1;
		}
	}
	if(n==100 || n==2) {  // setting arrow
		if(U_able) {
			bminx=bminy=-1;
			bmaxx=MaxX;
			bmaxy=MaxY;
			vPFFfar(x, y, "\xffz\x0\x11\xc6");
			vClipOn(cur_w);
		}
		else {
			vWriteString(x, y, "  ", 0);
		}
		if(D_able) {
			bminx=bminy=-1;
			bmaxx=MaxX;
			bmaxy=MaxY;
			vPFFfar(ex-16, y, "\xffx\x0\x11\xc6");
			vClipOn(cur_w);
		}
		else {
			vWriteString(ex-16, y, "  ", 0);
		}
	}
	vMCON();
}

int iMouseInHelpDlg(void)
{
	BYTE blk, cho;
	int i;
	i=iMoveDlgWindow(PW15);
	if(i!=0) return i;
	i=iExamineMCInObj(PW15, &blk, &cho);
	if(i==1) {
		vRevControlObj(PW15, blk, 0);
		while(lbp);
		vRevControlObj(PW15, blk, 0);
		return blk;
	}
	return -1;
}

void vHelpDlg(int n)
{
	BYTE scr;
	INKEY c;
	int choice;
	Mem hm=0;

	help_buf=(BYTE *)(our_buf+30000);
	if(iGetHelpData(n)==0) return;

	iSwapMem(MSWRITE, 0x7fff, &hm, our_buf);
	scr=0;
	pop[PW15].vb_scr=(BYTE *)(&scr);
	U_able=0;
	if(help_line>12) D_able=1;
	else D_able=0;
	vDispHelpDlg(100);

	for(;;) {
		if(scr) {
			U_able=1;
		}
		else U_able=0;
		if(help_line-scr>13) {
			D_able=1;
		}
		else D_able=0;
		vDispHelpDlg(2);
		while(!wCheckKeyHit()) {
			if(rbp) {
				while(rbp);
				goto OUT;
			}
			choice=iMouseInHelpDlg();
			switch(choice) {
				case -3 : case 4 :
					while(lbp);
					goto OUT;
				case -2 :
					vCloseDlgWindow(PW15);
					pop[PW15].x=pop_x;
					pop[PW15].y=pop_y;
					vDispHelpDlg(100);
					break;
				case -1 : break;
				case  0 : goto U;
				case  1 : goto D;
				case  2 : goto PU;
				case  3 : goto PD;
			}
		}
		c.i=wGetKey(0);
		if(c.ch[0]) {
			switch(c.ch[0]) {
				case ESC : case RETURN :
					goto OUT;
			}
		}
		else if(c.ch[1]) {
			switch(c.ch[1]) {
				case UP :
U:				if(scr>0) scr--;
					vDispHelpDlg(1);
					break;
				case DOWN :
D:				if(help_line-scr>13) scr++;
					vDispHelpDlg(1);
					break;
				case PGUP :
PU:				if(scr>11) scr-=11;
					else scr=0;
					vDispHelpDlg(1);
					break;
				case PGDN :
PD:       if(help_line>12) {
						scr+=11;
						if(scr>help_line-12) scr=help_line-13;
						vDispHelpDlg(1);
					}
					break;
				case F7 :
					if(iKeyMoveDlgWindow(PW15)) {
						vCloseDlgWindow(PW15);
            pop[PW15].x=pop_x;
						pop[PW15].y=pop_y;
						vDispHelpDlg(100);
					}
					break;
			}
		}
	}
OUT:
	vCloseDlgWindow(PW15);
	if(!iSwapMem(MSREAD, 0x7fff, &(hm), our_buf)) vErrorMsgfDlg(1, 2, 0);
	vFreeMem(&hm);
}

BYTE hf_buf[440];  // header, footer buffer

void vHeaderFooterSizeDlg(BYTE flag, WORD s, WORD cur, WORD e, BYTE *str)
{
	int i;
	BYTE align=0;

	setmem(hf_buf, 440, 0);
	strcpy(hf_buf, str);
	hf_buf[440]=align;
	if(flag==0) {
    strcpy(pop[PW17].headline, " áŸµ i");
	}
	else {
    strcpy(pop[PW17].headline, "¡Ÿµ i");
	}
	for(i=0; i<2; i++) {
		vInt2Char(s, hf_buf+401+i*15);
		vInt2Char(cur, hf_buf+403+i*15);
		vInt2Char(e, hf_buf+405+i*15);
    strcpy(hf_buf+407+i*15, "¡e ");
	}
	vOpenDlgWindow(PW17);
	for(;;) {
		i=iDialogDriver(PW17, hf_buf);
		switch(i) {
			case -1 : case 4 :
				goto OUT;
			case 3 :
				goto OUT;
		}
	}
OUT :
	vCloseDlgWindow(PW17);
}

void vHeaderSizeDlg(void)
{
	vHeaderFooterSizeDlg(0, 1, 5, 10, "\r");
}

extern int scr_saver_num;
extern WORD no_work_time;
void vScreenSaverDlg(void)
{
	BYTE buf[20];
	int i;

	setmem(buf, 20, 0);
	buf[0]=scr_saver_num;
	buf[1]=1;
	buf[3]=no_work_time;
	buf[5]=30;
	strcpy(buf+7, "¦… ");
	vOpenDlgWindow(PW18);
	i=iDialogDriver(PW18, buf);
	if(i==2) {
		scr_saver_num=buf[0];
		no_work_time=buf[3];
	}
	vCloseDlgWindow(PW18);
}

BYTE mk_bak=0;
BYTE ins_lock=0;
BYTE asave=0;
BYTE as_time=20;

void vEtcDlg(void)
{
	BYTE buf[20];
	int i, o;

	setmem(buf, 20, 0);
	buf[0]=o=show_rod;
	buf[1]=View_time;
	buf[2]=mk_bak;
	buf[3]=ins_lock;
	buf[4]=asave;
	buf[5]=tw[cur_w].ins>>1;
	buf[8]=as_time;
	buf[6]=1;       	 buf[10]=200;
	strcpy(buf+12, "Á¡ ");
	vOpenDlgWindow(PW23);
	i=iDialogDriver(PW23, buf);
	if(i==2) {
		show_rod=buf[0]; View_time=buf[1];
		mk_bak=buf[2];   ins_lock=buf[3];
		asave=buf[4];    tw[cur_w].ins=(buf[5]<<1) | (tw[cur_w].ins & 1);
		as_time=buf[8];
	}
	vCloseDlgWindow(PW23);
	if(o != show_rod)	vSetRod();
	if(View_time==0) vDispTime(2);
}

extern char *key_name[];

BYTE C_key[10]={0,1};
BYTE C_cnt=2;


void vInsertKeyMap(int n)
{
	int i, t;

	if(C_cnt==10) {
		vErrorMsgfDlg(10, 9, 0);
		return;
	}
	for(i=0; i<C_cnt; i++) {
		t=strcmp(key_name[n], key_name[KEYMAX+i]);
		if(t==0) return;
	}
	strcpy(key_name[KEYMAX+C_cnt], key_name[n]);
	C_key[C_cnt]=n;
	C_cnt++;
	pop[PW19].p_n[1]++;
}

void vDeleteKeyMap(int n)
{
  if(pop[PW19].p_n[1]==0 || C_cnt==0) {
		vErrorMsgfDlg(10, 10, 0);
		return ;
	}
	vWipeStringArray(KEYMAX+10, n+KEYMAX, (BYTE **)key_name);
	movmem(C_key+n+1, C_key+n, 10-n-1);
	C_cnt--;
  pop[PW19].p_n[1]--;
}

void vCircleKeyboardDlg(void)
{
	BYTE p_n[2], process[2], scr[2], value[2], set[2];
	BYTE buf[10];
	BYTE tbuf[10];
	BYTE tsbuf[10][12];
	BYTE tcnt;
	int i;

	p_n[0]=KEYMAX;
	tcnt=p_n[1]=C_cnt;
	memcpy(tbuf, C_key, 10);
	for(i=0;i<10;i++) {
		strcpy(tsbuf[i], key_name[KEYMAX+i]);
	}
	setmem(process, 2, 0);
	setmem(scr, 2, 0);
	setmem(buf, 10, 0);
	setmem(value, 2, 0);
	setmem(set, 2, 0);

	vMakeExtDlgWindow(PW19, (BYTE *)key_name, p_n, process, scr, value, set);
	vOpenDlgWindow(PW19);
	del_able=ins_able=1;
	for(;;) {
		i=iDialogDriver(PW19, buf);
		switch(i) {
			case 5 : case -1 :
				memcpy(C_key, tbuf, 10);
				C_cnt=tcnt;
				for(i=0;i<10;i++) {
					strcpy(key_name[KEYMAX+i], tsbuf[i]);
				}
				goto OUT;
			case 2 :
				vInsertKeyMap(set[0]);
				vDrawRecordObj(PW19, 1);
				break;
			case 3 :
				vDeleteKeyMap(set[1]);
				if(set[1]>0) {
					if(set[1]==value[1] && value[1]>0) value[1]--;
					set[1]--;
				}
				vDrawRecordObj(PW19, 1);
				break;
			case 4 :
				goto OUT;
			case -10 :    // Insert 0 blk to 1 blk
				vInsertKeyMap(value[0]+scr[0]);
				vDrawRecordObj(PW19, 1);
				del_flag=0;
				break;
			case -21 :    // Delete 1 blk's value
				vDeleteKeyMap(value[1]+scr[1]);
				break;
			case -11 : case -20 :
				del_flag=0;
				break;
		}
	}
OUT :
	del_able=ins_able=0;
	vCloseDlgWindow(PW19);
}

int iGetStringDlg(BYTE flag, BYTE *str, BYTE *head)
{
	int i;
	int x, y;

	strcpy(pop[PW20].headline, head);
	vOpenDlgWindow(PW20);
	vMCOFF();
	x=pop[PW20].x+24+pop[PW20].block[0][0]*8;
	y=pop[PW20].y+pop[PW20].block[0][1];
	i=iGetString(flag, x, y, 38, 40, str);
	vCloseDlgWindow(PW20);
	return i;
}

BYTE Reload=0;

void vSaveClosedTextWindow(BYTE *name)
{
	int i;
	if(strlen(name)>30) return;
	for(i=14; i>5; i--) strcpy(dat_PW21[i+1], dat_PW21[i]);
	sprintf(dat_PW21[6],"  ”hÓ¥ %s", name);
	if(Reload<10) Reload++;
}

void vDeleteClosedTextWindow(int n)  // n is 0-9
{
	int i;
	if(Reload==0) return;
	for(i=n+6; i<15; i++) strcpy(dat_PW21[i], dat_PW21[i+1]);
	Reload--;
}

void vMoveUpCTW(int s, int n, int tot, BYTE *dat[])
{
	int i;
//	for(i=0; i<Reload; i++) strcpy(dat[i+s], dat[6+i]);
	for(i=0; i<tot; i++) strcpy(dat[i+s], dat[n+i]);
}

void vMoveDownCTW(int s, int n, int tot, BYTE *dat[])
{
	int i;
	for(i=0; i<Reload; i++)
//		strcpy(dat_PW21[6+Reload-i-1], dat_PW21[s+Reload-i-1]);
		strcpy(dat[n+tot-i-1], dat[s+tot-i-1]);
}

int iDispChangeTextWindowDlg(int code)
{
	int i, j=0;

	switch(code) {
		case 0 :
			for(i=0; i<6; i++) {
				if(tw[i].active) {
					sprintf(dat_PW21[j], "%d %s", i, tw[i].headline);
					j++;
				}
			}
			if(Reload) {
				vMoveUpCTW(j, 6, Reload, dat_PW21); j+=Reload*10;
			}
			return j;
		case 1 :
			for(i=0; i<6; i++) {
				if(dat_PW21[i][0]-48==cur_w) return i;
			}
			return 0;
		case 2 :
			j=dat_PW21[pop[PW21].set[0]][0]-48;
			return j;
	}
	return 0;
}

void vChangeTextWindowDlg(void)
{
	BYTE p_n, process, scr, value, set;
	BYTE buf[5];
	int i, j;

	setmem(buf, 5, 0);
	j=iDispChangeTextWindowDlg(0);
	p_n=(j%10)+j/10;
	j/=10;
	set=iDispChangeTextWindowDlg(1);
	process=scr=value=0;
	vMakeExtDlgWindow(PW21, (BYTE *)dat_PW21, &p_n, &process, &scr, &value, &set);
	vOpenDlgWindow(PW21);
  num_able=1;
	for(;;) {
		i=iDialogDriver(PW21, buf);
		switch(i) {
			case -10 : case -11 :
				num_able=0;
				break;
			case  -1 :
				vCloseDlgWindow(PW21);
				num_able=0;
				return;
			case  0 : case 100 : case 101 : case 102 : case 103 : case 104 : case 105:
				if(i==0) {
					i=iDispChangeTextWindowDlg(2);
				}
				else {
					i=i-100;
				}
				vCloseDlgWindow(PW21);
				if(i>-1) {
					vChangeWindow(i);
				}
				else {
					vMoveDownCTW(p_n-j, 6, Reload, dat_PW21); set-=(p_n-j);
					memmove(dat_PW21[set+6], dat_PW21[set+6]+7, 30);
					vOpenAgain(dat_PW21[set+6]);
					vDeleteClosedTextWindow(set);
				}
				num_able=0;
				return;
		}
	}
}

BYTE str_buf[80];
BYTE far GLSname[80];

extern BYTE gls_data[];

void vGLSFileIO(int code)
{
	FILE *fp;
	int i;

	if(code==0) {    														 // read string form "21.GLS"
		if((fp=fopen(GLSname, "rb"))!=NULL)  {
			if(iCheckGWP(0, fp, gls_data)) {
				for(i=0; i<10; i++) {
					fread(menu_PW22[i]+10, 80, 1, fp);
				}
			}
			else i=100;
			fclose(fp);
		}
		else i=100;
		if(i==100) {
			for(i=0; i<10; i++) {
				menu_PW22[i][10]=0;
			}
		}
	}
	else { 																			 // write string to  "21.GLS"
		if((fp=fopen(GLSname, "wb+"))==NULL) {
			vErrorMsgfDlg(0, 2, "??.GLS");
			return;
		}
		sprintf(g_dir, "%s%s", the21, gls_data);
		fwrite(g_dir, 48, 1, fp);
		for(i=0; i<10; i++) {
			fwrite(menu_PW22[i]+10, 80, 1, fp);
		}
		fclose(fp);
	}
}

void vFreqTextDlg(void)
{
	int i, n, t, x, y;
	BYTE buf[5];

	vGLSFileIO(0);
	vOpenDlgWindow(PW22);
	for(;;) {
		i=iDialogDriver(PW22, buf);
		switch(i) {
			case -1 : case 2 :
				vGLSFileIO(0);
				goto OUT;
			case  1 :
				vGLSFileIO(1);
				goto OUT;
			case  0 :
				n=pop[PW22].code[0][4];
				strcpy(str_buf, menu_PW22[n]+10);

				x=pop[PW22].x+24+pop[PW22].block[0][0]*8+n*16;
				y=pop[PW22].y+pop[PW22].block[0][1]+9;
REGET:  t=iGetString(1, x, y, 78, 49, str_buf);
				if(t!=-1 && t!=0) {
					strcpy(menu_PW22[n]+10, str_buf);
					vMCOFF();
					vPutStringLength(x, y,	str_buf, 0, 0, 49);
				}
				else {
					vPutStringLength(x, y,	menu_PW22[n]+10, 0, 0, 49);
				}
				switch(t) {
					case -2  : vPutKey(DOWN, 0); break;
					case -3  : vPutKey(UP, 0);   break;
					case -10 : vPutKey(F7, 0); break;
					case -11 : vPutKey(F1, 0);   break;
					default :  break;
				}
		}
	}
OUT :
	vCloseDlgWindow(PW22);
}

WORD PG_tot=0;
WORD PG_cur=0;

void vInitPrintStatusWindow(WORD rs, int re, int n)
{
	int x, y;
	int pg_all, ln_all;

	x=pop[PW24].x;
	y=pop[PW24].y;

	vMCOFF();
	vOpenDlgWindow(PW24);
	vMCOFF();
	iPrintf(x+32, y+4, 0, "[%d]¤å Íe»³Àw ·¥­áº—...", n);  // ¤å
	iPrintf(x+62, y+3+20+1, 0, "%3d%%", 0);
	vNBoxBit(x+60-2, (y+3)*8-2, x+60+20+2, (y+3+20)*8+2, 1);
	vMCON();
	ln_all=tw[n].total*tw[n].repeat;
	pg_all=TOT_PAGE;
	if(rs==0xffff) {
		switch(re) {
			case 0 :  PG_tot=ln_all; break;           // total page
			case 1 :  PG_tot=ln_all/pg_all; break;    // CNode page only
			case 2 : case 3 :           							// odd or even
				PG_tot=ln_all/2; break;
		}
	}
	else {
		PG_tot=(int)((long)ln_all*(re-rs+1)/pg_all);
	}
}

void vShowPrintStatus(void)
{
	int x, y;
	WORD delta, pp;
	x=pop[PW24].x;
	y=pop[PW24].y;

	if(PG_cur>PG_tot) PG_cur=PG_tot;
	delta=(WORD)((long)PG_cur*160/PG_tot);
	pp=(WORD)((long)PG_cur*100/PG_tot);
	vMCOFF();
	vNBarBit(x+60, (y+3)*8, x+60+20, (y+3)*8+delta, 1);
	iPrintf(x+62, y+3+20+1, 0, "%3d%%", pp);
	vMCON();
	PG_cur++;
}

void vHidePrintStatus(void)
{
	while(PG_cur<PG_tot+1) vShowPrintStatus();
	PG_cur=PG_tot=0;
	vCloseDlgWindow(PW24);
	vMCON();
}

extern BYTE far MACname[];
BYTE far COMbuf[80];

void vEtcFileIODlg(BYTE flag, BYTE flag2)
{
	int i;
	BYTE *buf;

	if((flag & 0x80)) {
		buf=MACname;
		vSetExt(".MAC");
	}
	else {
		buf=GLSname;
		vSetExt(".GLS");
	}
	strcpy(COMbuf, buf);
	i=iFileListDlg(buf, COMbuf, flag2);
	if(i==1) {
		strcpy(buf, COMbuf);
		if((flag & 0x80)) {
			if(flag & 1)	vWriteKeyMacro(buf);
			else vReadKeyMacro(buf);
		}
		else vGLSFileIO(flag & 1);
	}
	else strcpy(buf, COMbuf);
}

BYTE MRfunc1=0;
BYTE MRfunc2=0;

void vMouseOptDlg(void)
{
	int i;

	setmem(COMbuf, 80, 0);
	COMbuf[0]=1;
	COMbuf[2]=11-m_rate;
	COMbuf[4]=10;
	COMbuf[15]=MRfunc1;
	COMbuf[16]=MRfunc2;
	vOpenDlgWindow(PW25);
	i=iDialogDriver(PW25, COMbuf);
	if(i==3) {
		m_rate=11-COMbuf[2];
		MRfunc1=COMbuf[15];
		MRfunc2=COMbuf[16];
	}
	vCloseDlgWindow(PW25);
}

BYTE CompWay=0;
BYTE SortWay=0;
BYTE CompStart=1;

int iSortOptDlg(void)
{
	int i;

	COMbuf[0]=CompWay;
	COMbuf[1]=SortWay;
	COMbuf[2]=1;
	COMbuf[4]=CompStart;
	COMbuf[6]=250;
  strcpy(COMbuf+8, "µi");
	vOpenDlgWindow(PW26);
	for(;;) {
		i=iDialogDriver(PW26, COMbuf);
		switch(i) {
			case -1 : case 4 :
				vCloseDlgWindow(PW26); return 0;
			case 3 :
				CompWay=COMbuf[0];
				SortWay=COMbuf[1];
				CompStart=COMbuf[4];
				vCloseDlgWindow(PW26); return 1;
		}
	}
}

void vInitDlgWindow(void)
{
	vMakeDlgWindow(PW0, 12, 30, 1, (BYTE **)inf_PW0, menu_PW0,\
																0, 0, (BYTE *)"µAœá");
	vMakeDlgWindow(PW1, 12, 31, 2, (BYTE **)inf_PW1, menu_PW1,\
																0, 0, (BYTE *)"¬åÈ‚");
	vMakeDlgWindow(PW2, 13, 40, 2, (BYTE **)inf_PW2, menu_PW2,\
																0, 0, (BYTE *)" ");
	vMakeDlgWindow(PW3, 34, 41, 10, (BYTE **)inf_PW3, menu_PW3,\
																comment_PW3, cod_PW3, (BYTE *)" ");
	vMakeDlgWindow(PW4, 36, 59, 5, (BYTE **)inf_PW4, menu_PW4,\
																0, cod_PW4, (BYTE *)"Load");
	vMakeDlgWindow(PW5, 27, 50, 8, (BYTE **)inf_PW5, menu_PW5, 0,
															 cod_PW5, (BYTE *)"·¥­á¬é¸÷");
	vMakeDlgWindow(PW6, 22, 33, 0, 0, 0, 0, 0, (BYTE *)"Ðe¸a¬a¸å");
	vMakeDlgWindow(PW7, 23, 36, 4, (BYTE **)inf_PW7, menu_PW7,\
																comment_PW7, cod_PW7, (BYTE *)"  ");
	vMakeDlgWindow(PW8, 28, 38, 7, (BYTE **)inf_PW8, menu_PW8,\
                                comment_PW8, cod_PW8, (BYTE *)"Àx‹¡");
	vMakeDlgWindow(PW9, 34, 38, 8, (BYTE **)inf_PW9, menu_PW9,\
																comment_PW9, cod_PW9, (BYTE *)"¤aŽ‹¡");
	vMakeDlgWindow(PW10, 26, 24, 5, (BYTE **)inf_PW10, menu_PW10,\
																comment_PW10, cod_PW10, (BYTE *)"·ª“e¤w¤ó");
	vMakeDlgWindow(PW11, 19, 40, 4, (BYTE **)inf_PW11, menu_PW11,\
																comment_PW11, cod_PW11, (BYTE *)"ÑÁ¡eÀáŸ¡");
	vMakeDlgWindow(PW13, 20, 34, 5, (BYTE **)inf_PW13, menu_PW13,\
																comment_PW13, cod_PW13, (BYTE *)"Ðe¸a”e´á");
	vMakeDlgWindow(PW14, 28, 54, 7, (BYTE **)inf_PW14, menu_PW14,\
																comment_PW14, cod_PW14, (BYTE *)"ÏaŸ¥Èá¬é¸÷");
	vMakeDlgWindow(PW15, 37, 58, 5, (BYTE **)inf_PW15, menu_PW15,\
																0, 0, (BYTE *)"•¡¶‘ i");
	vMakeDlgWindow(PW16, 23, 40, 6, (BYTE **)inf_PW16, menu_PW16,\
																comment_PW16, cod_PW16, (BYTE *)"¸á¸w¤w¤ó");
	vMakeDlgWindow(PW17, 30, 51, 5, (BYTE **)inf_PW17, menu_PW17,\
																comment_PW17, cod_PW17, (BYTE *)"  ");
	vMakeDlgWindow(PW18, 25, 21, 4, (BYTE **)inf_PW18, menu_PW18,\
																comment_PW18, cod_PW18, (BYTE *)"ÑÁ¡e¥¡Ñ¡");
	vMakeDlgWindow(PW19, 34, 51, 6, (BYTE **)inf_PW19, menu_PW19,\
																comment_PW19, cod_PW19, (BYTE *)"®…ÑÅ¸aÌe");
	vMakeDlgWindow(PW20, 10, 44, 1, (BYTE **)inf_PW20, menu_PW20,\
																0, cod_PW20, (BYTE *)" ");
	vMakeDlgWindow(PW21, 22+8, 26+17, 1, (BYTE **)inf_PW21, 0,\
																comment_PW21, cod_PW21, (BYTE *)"Íe»³ÀwŠaÁA");
	vMakeDlgWindow(PW22, 32, 62, 3, (BYTE **)inf_PW22, menu_PW22,\
																comment_PW22, cod_PW22, (BYTE *)"¬w¶wŠ");
	vMakeDlgWindow(PW23, 24, 23, 4, (BYTE **)inf_PW23, menu_PW23,\
																0, cod_PW23, (BYTE *)"‹¡Èa¬aÐw");
	vMakeDlgWindow(PW24, 12, 30, 0, 0, 0, 0, 0, "·¥­á¬wÑ×");
	vMakeDlgWindow(PW25, 33, 56, 5, (BYTE **)inf_PW25, menu_PW25,\
																comment_PW25, cod_PW25, (BYTE *)" a¶¯aÀáŸ¡");
	vMakeDlgWindow(PW26, 20, 28, 5, (BYTE **)inf_PW26, menu_PW26,\
                                comment_PW26, cod_PW26, (BYTE *)"­¡Ëa");
}

