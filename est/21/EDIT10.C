#define _EDIT10
#include <stdio.h>
#include <stdlib.h>
#include <alloc.h>
#include <dos.h>
#include <string.h>
#include <ctype.h>
#include <mem.h>
#include <dir.h>
#include <io.h>
#include <fcntl.h>
#include <sys\types.h>
#include <time.h>
#include "est.h"
#include "21.h"
#include "proto.h"

#include "graph.h"
#include "hanin.h"
#include "hanin2.h"
#include "mouse.h"
#include "engkbd.h"
#include "dvorak2.h"

#define _LOCSP wTL+=SCODE
#define _LOCWP wTL+=WCODE
#define _LOCSM wTL-=SCODE
#define _LOCWM wTL-=WCODE
#define _SETSTART vResetHanin()
#define _PUTCURLINE iPTC(iCX, cur_lnum, CNode)
#define _COL  dc=tw[cur_w].cc
#define _CLEARWIN(x) vBarByte(x, (bminy>>3)+1, bmaxx, (bmaxy>>3), 0)
#define pTB	CNode->line
#define _OUTDBLINK cur_hsize=CNode->hs[cur_lnum-1] & 0x70
#define bGetLH(lnum, t) (t->hs[lnum-1] & 0x70)
#define iCX tw[cur_w].cx
#define iCY tw[cur_w].cy
#define wTL tw[cur_w].bl
#define PRN_SCR	 2/3

#define TON				2
#define ON		    1
#define OFF 	    0
#define YES			  1
#define NO        0
#define NEW			  0
#define OLD			  1
#define TODN			1
#define TOUP			2
#define HANNULL		0x8441
#define SCODE				4
#define WCODE       5

enum
{ASCII=0, CHOSONG1, CHOSONG2, JUNGSONG1, JUNGSONG2, JONGSONG1=10, JONGSONG2};

NODE *SNode=0, *CNode=0, *ENode=0;
WORD ttloc=0;
WORD dtloc=0;
int ex_err;


NODE *top=0;
int top_x;
BYTE top_lnum;
WORD top_cr;
WORD top_cp;

#define LINEGAB		lc[0]
#define INDENT		lc[1]
#define LEFTR			lc[2]
#define RIGHTR    lc[3]
#define ALINE			lc[4]
#define LNUM			lc[5]

BYTE def_lc[6]={4, 2, 1, 70, 4, 1};
BYTE load_tab=8;
BYTE hfnum=0;
BYTE efnum=0;
BYTE jfnum=0;

BYTE exe_path[80];

BYTE load_lc[6]={4, 4, 4, 70, 4, 1};
BYTE cursor_flag=OFF;
BYTE on_off=0;
BYTE eff=0;
BYTE eff_filter;
BYTE sis=0x11;
BYTE sis_filter;
BYTE hc[5]={0, 0, 0, 0x11, 0xe0};
BYTE ec[4]={0, 0, 0x11, 0x40};
BYTE jc[5]={0xff, 0, 0, 0x11, 0xd0};
BYTE sc[4]={0, 0, 0x11, 0x20};
BYTE wc[5]={0xff, 0, 0, 0x11, 0xc0};
BYTE ic[4]={32, 0, 0x11, 0x40};
BYTE cur_hsize=16;

BYTE start=ON;
BYTE status=0;
BYTE han_kbd=1;
BYTE eng_kbd=0;

BYTE cur_lnum=1;
BYTE key_flag=0;
BYTE skip_flag=0;
BYTE block_task=0;
BYTE btasking=0;
BYTE block_go=0;
BYTE set_font_flag=0;
WORD tot_page=1;
BYTE line_state=0;
BYTE line_drawing=0;
BYTE line_style=0;
BYTE page_div_flag=0;

blk sb, eb, setb;

int dc=1;
CODE2 c;
WORD SYSPEED=0;

int set_graph_mode;

extern BYTE color_mode;
extern menu_panel panel[];
extern BYTE cur_w;
extern text_window tw[];
extern text_window def_tw;
extern BYTE *our_buf;
extern popup_window pop[];
extern BYTE full_vector;
extern BYTE current_color;
extern BYTE font_color;
extern BYTE around_color;
extern int MaxX, MaxY;
extern int bminx, bminy, bmaxx, bmaxy;
extern BYTE swp_path[];
extern BYTE the21[];
extern BYTE gwp_data[];
extern BYTE swp_data[];
extern BYTE cfg_data[];
extern BYTE hwp11_data[];
extern BYTE hwp12_data[];
extern BYTE path_buf[];
extern BYTE g_drive[];
extern BYTE g_dir[];
extern BYTE g_file[];
extern BYTE g_ext[];
extern BYTE nick_name[];
extern int last_top;
extern WORD work_pagex;
extern BYTE *psstr;
extern BYTE find_flag;
extern blk found;
extern BYTE disp_japan;
extern BYTE kkana_tbl[];
extern BYTE hkana_tbl[];
extern void (*xms_func)();
extern BYTE xms_state;
extern BYTE ems_state;
extern BYTE vms_state;
extern BYTE printing;
extern BYTE C_key[];
extern BYTE C_cnt;
extern BYTE C_h_e=0;
extern BYTE disp_space, disp_newline, disp_return;
extern BYTE rev_flag;
extern BYTE prn_number;
extern BYTE *gsf[];
extern BYTE mouse_on_off;
extern BYTE *vid_addr;
extern WORD offsetx[];
extern BYTE is_laser;
extern BYTE ins_lock;
extern BYTE MRfunc1;
extern BYTE MRfunc2;
extern BYTE Ext_buf[];
extern BYTE E_flag;

static WORD near wScr2Prn(WORD n)
{
	if(is_laser) return n*5/4;
	else         return n*3/2;
}

static WORD near wPrn2Scr(WORD n)
{
	if(is_laser) return n*4/5;
	else         return n*2/3;
}

static void near _WINSCRUP(int sbit)
{
	vScrollUpByte(bminx+1, bmaxx+2, (bminy>>3)+1, tw[cur_w].ly-3, sbit);
}

static void near _WINSCRDN(int sbit)
{
	vScrollDownByte(bminx, bmaxx-1, (bminy>>3)+1, tw[cur_w].ly-3, sbit);
}

static void near _WINSCRR(int sbyte)
{
	vScrollRightByte(bminx, bmaxx, (bminy>>3)+1,	tw[cur_w].ly-3, sbyte, 0);
}

static void near _WINSCRL(int sbyte)
{
	vScrollLeftByte(bminx, bmaxx, (bminy>>3)+1, tw[cur_w].ly-3, sbyte, 0);
}

static void near vResetHanin(void)
{
	start=ON; status=CHOSONG1; vResetHangulStack(); c.i=HANNULL;
}

static BYTE co_key_tbl[6]={0x40, 2, 1, 3, 0x08, 4};

BYTE bGetKeyState(BYTE c)
{
	return bioskey(2) & co_key_tbl[c];
}

void vDispScriptStatus(void)
{
	if(ec[3]==0x44) vWriteString(tw[cur_w].x+4, tw[cur_w].y+3, "^", 0x04);
	else if(ec[3]==0x45) vWriteString(tw[cur_w].x+4, tw[cur_w].y+3, "v", 0x04);
	else vWriteString(tw[cur_w].x+4, tw[cur_w].y+3, " ", 0x04);
}

void vPutTabChar(int n, int i, BYTE flag) // n is win num, i is lot
{                                          // flag is draw or erase
	if(flag==0) vBarByte(tw[n].x+37, (bminy+1)/8+i, tw[n].x+40, (bminy+1)/8+i+1, 0);
	else {
		vPutSmallFont(tw[n].x+37, (bminy+1)/8+i, 5, 1);
	}
}

void vPutTab(int n)
{
	int i;
	BYTE t, scr;
	BYTE *buf;

	scr=tw[n].scr_c;
	buf=(BYTE *)tw[n].tab;
	vBarByte(tw[n].x+37, (bminy+1)/8, tw[n].x+40, bmaxy/8, 0);
	for(i=0; i<tw[n].ly-2; i++) {
		t=buf[(scr+i)/8] & (0x80>>((scr+i)%8));
		if(t) vPutTabChar(n, i, 1);
	}
}

extern BYTE show_rod;

void vPutRod(int n, BYTE check)
{
	int dens[6]={180, 120, 240, 180, 360, 300};
	int i, dns, w_g;
	BYTE scr;
	static int lm=-1, rm=-1, ind=-1, sc=-1, wnum=-1;
	WORD x_byte, j;

	if(show_rod==0) return;
	scr=tw[cur_w].scr_c;
	vPutTab(cur_w);
	if(check && scr==sc && lm==CNode->lc[2] && rm==CNode->lc[3]
						 && ind==(int)((char)CNode->lc[1]) && wnum==n) return;
	sc=scr;
	lm=CNode->lc[2];
	rm=CNode->lc[3];
	ind=(int)((char)CNode->lc[1]);
	wnum=n;
	vBarByte(tw[n].x+41, (bminy+1)/8, tw[n].x+49, bmaxy/8, 0);
	dns=tw[n].prn_dens;
	w_g=tw[n].word_gap;
	x_byte=(dens[dns]*100/((12+w_g)*254));
	j=(100/x_byte)*5*scr;
	j/=100;
	x_byte=(x_byte*16)/10;
	for(i=bminy; i<bmaxy; i+=x_byte, j++) {
		if(!(j%5)) {
			vWLineXBit(tw[n].x+46, tw[n].x+49, i);
			if(!(j%25)) vPutSmallFont(tw[n].x+42, (i+1)/8, j/25, 0);
		}
		else {
			vWLineXBit(tw[n].x+48, tw[n].x+49, i);
		}
	}
	vPutSmallFont(tw[n].x+42, bminy/8+lm-scr, 1, 1);    // vLeftKey margin
	vPutSmallFont(tw[n].x+42, bminy/8+rm-scr, 2, 1);   // vRightKey margin
	vPutSmallFont(tw[n].x+42, bminy/8+ind-scr+lm, 3, 1);   // indent
	vWLineYBit(tw[n].x+50, (tw[n].y<<3)+3, (tw[n].y+tw[n].ly<<3)-17);
}

BYTE *hs_buf=0;

BYTE *pGetLHB(int size)
{
	int i;
	int sum=0;

	for(i=0;i<20000;i++) {
		if(sum==size) break;
		if(hs_buf[i]==0) sum++;
		else sum=0;
	}
	if(sum) {
		setmem(hs_buf+i-size, size, 0x10);
		return hs_buf+i-size;
	}
	else return 0;
}

void vFreeLHB(BYTE *t, int size)
{
	setmem(t, size, 0);
}

BYTE *pRegetLHB(int size, NODE *n)
{
	BYTE *tp;
	BYTE *hs;

	hs=n->hs;
	if((tp=pGetLHB(size))!=0) {
		memcpy(tp, hs, n->lc[5]);
//		setmem(hs, n->lc[5], 0);
		return tp;
	}
	else return 0;
}

#define ONEPASS 20
#define TWOPASS 50

int iMakeMem(void)
{
	NODE *t;
	WORD iDelKey=0;
	WORD max;
	int i;

	if(SNode==0) return 0;
	max=dtloc+ONEPASS;
	for(i=0;i<TWOPASS;i++) {
RESCAN:
		if(max>ttloc-ONEPASS) max=ttloc-ONEPASS;
		t=SNode->next;
		while(t) {
			if(t->tt>=dtloc && t->tt<max) {
				if(t->mh==0) {
					if(iSwapMem(1, t->bn, &(t->mh), t->line)!=1) {
						vErrorMsgfDlg(1, 1, 0);
						break;
					}
					free(t->line);
					iDelKey++;
				}
			}
			t=t->next;
		}
		if(iDelKey<ONEPASS) {
			dtloc=max;
			if(dtloc+ONEPASS<ttloc)	max+=ONEPASS;
			if(i==TWOPASS-1) iFreeFontBuffer(1);
		}
		else break;
	}
	dtloc=max;
	return iDelKey;
}

void *pGetMem(WORD size)
{
	void *p;

	if(size==0) return 0;
	do {
		p=(void *)malloc(size);
		if(p==0) {
			if(iMakeMem()==0) return 0;
//			reset_hs();
		}
	} while(p==0);
	return p;
}

void *pRegetMem(void *p, WORD size)
{
	void *tp;

	if(size==0) return 0;
	do {
		tp=(void *)realloc(p, size);
		if(tp==0) {
			if(iMakeMem()==0) return 0;
//			reset_hs();
		}
	} while(tp==0);
	return tp;
}

int iLockNode(NODE *n)
{
//	BYTE retry=0;
	if(n->mh==0) return 0;
RETRY:
	n->line=_GETLINE(n->bn);
	if(n->line==0) {
		vErrorMsgfDlg(1, 0, 0);
		return 0;

	}
	else {
		if(iSwapMem(0, n->bn, &(n->mh), our_buf)!=1) {
			vErrorMsgfDlg(1, 2, 0);
			return 0;
		}
		vFreeMem(&(n->mh));
		memcpy(n->line, our_buf, n->bn);
		n->tt=ttloc++;
	}
	return 1;
}

WORD wGetLTLoc(void)
{
	return ttloc++;
}

int iLoc2Col(WORD loc, NODE *n)
{
	int col=1;
	WORD c=0;
	BYTE *buf;

	_CHECKNODE(n);
	buf=n->line;
	while(buf[c]!='\r'){
		if(loc==c) break;
		if(buf[c]>127) {
			if(buf[c+2]>127 && col!=1) {
				col=1;
				continue;
			}
			col+=((buf[c+3] & 0x0f)<<1);
			c+=5;
		}
		else {
			if(buf[c]==0) {
				col++;
				c++;
				continue;
			}
			if(buf[c+1]>127 && col!=1) {
				col=1;
				continue;
			}
			col+=(buf[c+2] & 0x0f);
			c+=4;
		}
	}
	return col;
}

WORD wLine2Loc(BYTE lnum, NODE *temp)
{
	WORD l=0;
	BYTE *buf;
	BYTE count;
	BYTE flag;

	buf=temp->line;
	for(count=0;count<lnum;count++) {
		flag=0;
		for(;;){
			if(buf[l]>127) {
				if(buf[l+2]>127 && flag) break;
				l+=5;
			}
			else {
				if(buf[l]==0) {
					l++;
					continue;
				}
				if(buf[l]==13) return l;
				if(buf[l+1]>127 && flag) break;
				l+=4;
			}
			flag=1;
		}
	}
	return l;
}

WORD wLoc2Line(WORD loc, BYTE *buf)
{
	WORD l;
	BYTE lnum=1;

	for(l=0;l<loc;) {
		if(buf[l]>127) {
			if(buf[l+2]>127 && l) lnum++;
			l+=5;
		}
		else {
			if(buf[l]==0) {
				l++;
				continue;
			}
			if(buf[l]==13) return lnum;
			if(buf[l+1]>127 && l) lnum++;
			l+=4;
		}
	}
	return lnum;
}

static WORD near wGetTotalLineNum(void)
{
	NODE *t;
	WORD sum=0;

	t=SNode->next;
	while(t) {
		sum+=t->lc[5];
		t=t->next;
	}
	return sum;
}

void vDividePage(int cod)
{
	int i;
	WORD tot=0;
	BYTE divide_line=0;
	int added=0;
	BYTE slnum;
	NODE *temp;

	tw[cur_w].total=wGetTotalLineNum();
	tot_page=1;
	temp=SNode->next;
	slnum=0;
	while(temp) {
		if(temp->kd==0) {
			for(i=slnum;i<temp->lc[5];) {
				if(tot || divide_line) {
					if(temp->hs[i] & 0x01) {
						slnum=i; tot=0; temp->hs[i]|=0x80;
						tot_page++; i++;
						divide_line=ON;
						continue;
					}
					else temp->hs[i] &= 0x70;
				}
				divide_line=OFF;
				tot+=(wScr2Prn((temp->hs[i] & 0x70))+temp->lc[0]*LGAP_MUL(cur_w));
				if(tot>work_pagex) {
					if(!added) i++;
					slnum=i;
					tot=0;
					temp->hs[i]|=0x80;
					tot_page++;
					continue;
				}
				added++;
				i++;
				if(wCheckKeyHit() && cod==0) return;
			}
		}
		temp=temp->next;
		slnum=0;
	}
	tw[cur_w].prn_range[8]=(tot_page>>8);
	tw[cur_w].prn_range[9]=(tot_page & 0x00ff);
	page_div_flag=0;
}

void vUserDividePage(void)
{
	if(!tw[cur_w].active) return;
	if(!CNode->prev->prev && cur_lnum==1) return;
	if(CNode->hs[cur_lnum-1] & 0x01) {
		CNode->hs[cur_lnum-1] &=0x7f;
		CNode->hs[cur_lnum-1] &=0xfe;
	}
	else {
		CNode->hs[cur_lnum-1] |=0x80;
		CNode->hs[cur_lnum-1] |=0x01;
	}
	key_flag=ON;
}

NODE *npGetPageStartNode(BYTE *slnum, NODE *temp)
{
	int i;

	while(temp->prev->prev) {
		for(i=*slnum;i>=0;i--) {
			if(temp->hs[i]>127) {
				(*slnum)=i+1;
				return temp;
			}
		}
		temp=temp->prev;
		(*slnum)=temp->lc[5]-1;
	}
	(*slnum)=1;
	return SNode->next;
}

NODE *npGetPageEndNode(BYTE *slnum, NODE *temp)
{
	int i;
	BYTE s=0;

	while(temp) {
		for(i=*slnum;i<temp->lc[5];i++) {
			if(temp->hs[i]>127 && s) {
				(*slnum)=i+1;
				return temp;
			}
			s=1;
		}
		temp=temp->next;
		(*slnum)=0;
	}
	(*slnum)=ENode->lc[5]+1;
	return ENode;
}

NODE *npGetPageStart(int page_num, BYTE *slnum)
{
	NODE *temp;
	NODE *t;
	BYTE page_count=1;
	BYTE i;

  temp=SNode->next;
	if(page_num==1) {
    (*slnum)=1;
		return temp;
	}
	while(temp) {
		for(i=0;i<temp->lc[5];i++) {
			if(temp->hs[i]>127) {
				page_count++;
				(*slnum)=i+1;
				t=temp;
				if(page_count==page_num) return temp;
			}
		}
		temp=temp->next;
	}
	return t;
}

#include "align.c"
#include "dblink.c"
#include "tcursor.c"
#include "combine.c"
#include "390.c"
#include "inputjap.c"
#include "gets.c"
//#include "gets2.c"

int iSetCurrentLine(void)  // setting colume from dc
{
	BYTE *buf;
	int c;
	BYTE tc;
	WORD l, tl;

	_CHECKNODE(CNode);
	buf=CNode->line;
	cur_hsize=bGetLH(cur_lnum, CNode);
	wTL=l=wLine2Loc(cur_lnum-1, CNode);
	if(cur_lnum==1)
		c=(int)((char)CNode->lc[1])+CNode->lc[2];
	else
		c=CNode->lc[2];
	while(buf[l]!='\r'){
		if(buf[l]>127) {
			if(buf[l+5]>127) {
				if(buf[l+7]>127) break;
			}
			else {
				if(buf[l+5]!='\r' && buf[l+6]>127) break;
			}
			c+=((buf[l+3] & 0x0f)<<1);
			if(c==dc) {
				l+=5;
				break;
			}
			if(c>dc) {
				c-=((buf[l+3] & 0x0f)<<1);
				break;
			}
			l+=5;
		}
		else {
			if(buf[l+4]>127) {
				if(buf[l+6]>127) break;
			}
			else if(buf[l+4]==0) {
				tl=l+4;
				tc=c+(buf[l+2] & 0x0f);
				while(buf[tl]==0) {
					tc++;
					if(tc>dc) {
						wTL=l;
						return c;
					}
					tl++;
				}
				if(tc==dc) {
					wTL=tl;
					return tc;
				}
				c=tc;
				l=tl;
				continue;
			}
			else {
				if(buf[l+4]!='\r' && buf[l+5]>127) break;
			}
			c+=(buf[l+2] & 0x0f);
			if(c==dc) {
				l+=4;
				break;
			}
			if(c>dc) {
				c-=(buf[l+2] & 0x0f);
				break;
			}
			l+=4;
		}
	}
	wTL=l;
	return c;
}


void vGetCursorSize(void)  // setting cursor size from wTL
{
	if(set_font_flag) return;
	if(pTB[wTL]>127) {
		if((pTB[wTL+4] & 0xe0)==0xe0) //HANGUL
			hc[4]=pTB[wTL+4];
		else hc[4]=0xe0;
		eff=pTB[wTL+2];
		sis=pTB[wTL+3];
		cursor_hsize=(sis>>4);
		cursor_vsize=(sis & 0x0f);
	}
	else {
		if(pTB[wTL]=='\r') {
			if(wTL==0 || pTB[wTL-1]==0) {
				hc[4]=0xe0;
				ec[3]=0x40;
				eff=0;
				sis=0x11;
				cursor_hsize=1;
				cursor_vsize=1;
				vDispScriptStatus();
				return;
			}
			if(pTB[wTL-1]>127) {
				if((pTB[wTL-1] & 0xe0)==0xe0) //HANGUL
					hc[4]=pTB[wTL-1];
				else hc[4]=0xe0;
			}
			else {
				if((pTB[wTL-1] & 0x48)==0x40) ec[3]=pTB[wTL-1];
				else ec[3]=0x40;
				vDispScriptStatus();
			}
			eff=pTB[wTL-3];
			sis=pTB[wTL-2];
			cursor_hsize=(sis>>4);
			cursor_vsize=(sis & 0x0f);
			return;
		}
		else {
			if((pTB[wTL+3] & 0x48)==0x40) ec[3]=pTB[wTL+3];
			else ec[3]=0x40;
			eff=pTB[wTL+1];
			sis=pTB[wTL+2];
			cursor_hsize=(sis>>4);
			cursor_vsize=(sis & 0x0f);
			vDispScriptStatus();
		}
	}
}

static void near vSetTopRow(void) // setting CNode top row from top NODE and lnum
{
	NODE *temp;
	int i;

	temp=SNode->next;
	top_cr=1;
	top_cp=1;
	while(temp!=top) {
		top_cr+=temp->lc[5];
		for(i=0;i<temp->lc[5];i++) {
			if(temp->hs[i]>127) top_cp++;
		}
		temp=temp->next;
	}
	top_cr+=top_lnum-1;
}

void vSetTopVar(void)  // setting top NODE, top lnum, top row and top x
{  									// from CNode, iCX and curlnum
	BYTE ss;
	BYTE hsize;

	top_x=iCX;
	top=CNode;
	if(iCX==(bminx+1)) {
		top_lnum=cur_lnum;
		vSetTopRow();
		return;
	}
	ss=cur_lnum-1;
	while(top->prev) {
		for(top_lnum=ss;top_lnum>0;top_lnum--) {
			hsize=bGetLH(top_lnum, top);
			top_x-=(hsize+wPrn2Scr(top->lc[0]));
			if(top_x<bminx+2)	{
				vSetTopRow();
				return;
			}
		}
		top=top->prev;
		ss=top->lc[5];
	}
	top=SNode->next;
	top_lnum=1;
	top_cr=1;
	top_cp=1;
	iCX-=(top_x-bminx-1);
	top_x=bminx+1;
}

static void near vMoveUpTop(int sbit)
{
	BYTE hsize;
	int tx;

	tx=top_x+bGetLH(top_lnum, top)+wPrn2Scr(top->lc[0])-sbit;
	if(tx<bminx+2) {
		for(;;) {
			if(top_lnum<top->lc[5]) top_lnum++;
			else {
				top=top->next;
				top_lnum=1;
			}
			if(top->hs[top_lnum-1]>127) top_cp++;
			hsize=bGetLH(top_lnum, top);
			top_cr++;
			tx+=(hsize+wPrn2Scr(top->lc[0]));
			if(tx>bminx) break;
		}
		top_x=tx-hsize-wPrn2Scr(top->lc[0]);
	}
	else top_x-=sbit;
}

static void near vMoveDnTop(int sbit)
{
	BYTE hsize;

	top_x+=sbit;
	if(top_x>bminx+1) {
		for(;;) {
			if(top->hs[top_lnum-1]>127) top_cp--;
			if(top_lnum>1) top_lnum--;
			else {
				top=top->prev;
				top_lnum=top->lc[5];
			}
			hsize=bGetLH(top_lnum, top);
			top_cr--;
			top_x-=(hsize+wPrn2Scr(top->lc[0]));
			if(top_x<bminx+2) break;
		}
	}
}

static int near iSetEndLine(int *tx, BYTE *skip, WORD *cr, WORD *cp)
{
	BYTE sline;
	BYTE hsize;
	BYTE ss;
	NODE *temp;

	if(iCX+cur_hsize+wPrn2Scr(CNode->lc[0])==bmaxx)
			return cur_lnum;
	ss=cur_lnum;
	temp=CNode;
	while(temp) {
		for(sline=ss;sline<temp->lc[5]+1;sline++) {
			if(temp->hs[sline-1]>127) (*cp)++;
			hsize=bGetLH(sline, temp);
			if(*tx+hsize+wPrn2Scr(temp->lc[0])>=bmaxx) return sline;
			if(!temp->next && sline+1>temp->lc[5]) return sline;
			(*tx)+=hsize+wPrn2Scr(temp->lc[0]);
			(*cr)++;
		}
		temp=temp->next;
		if(!temp) break;
		(*skip)++;
		ss=1;
	}
	return sline-1;
}

static void near vSkipLine(void)
{
	BYTE flag=0;
	while(1) {
		if(*psstr==0) psstr++;
		else if(*psstr>127) {
			if(*(psstr+2)>127 && flag) return;
			psstr+=5;
		}
		else {
			if(*psstr==13) return;
			if(*(psstr+1)>127 && flag) return;
			psstr+=4;
		}
		flag=1;
	}
}

static WORD near wSkipNode(WORD x, BYTE sline, WORD *cr, WORD *cp, NODE *temp)
{
	BYTE lnum;
	BYTE hsize;

	for(lnum=sline;lnum<temp->lc[5]+1;lnum++) {
		if(temp->hs[lnum-1]>127) (*cp)++;
		hsize=bGetLH(lnum, temp);
		(*cr)++;
		x+=hsize+wPrn2Scr(temp->lc[0]);
	}
	return x;
}

static void near vClearCurLine(int x, BYTE line_gap)
{
	int cx, cex;

	if(line_gap<1) return;
	cx=x;
	cex=x+line_gap-1;
	if(cx>bmaxx || cex<bminx) return;
	if(cx<bminx) cx=bminx;
	else if(cex>bmaxx) cex=bmaxx;
	vBarByte(cx, (bminy>>3)+1, cex, bmaxy>>3, 0);
}

static void near vPutHeaderMark(int x, int kd)
{
	int y;
	BYTE nc[3]={0xff, 0xf0, 0};

	y=(bminy>>3)-tw[cur_w].scr_c+1;

	nc[1]|=kd+1;
	if(bminy>>3<y) {
		clipping=ON;
		vWriteString(x, (bminy>>3)-tw[cur_w].scr_c+1, nc, 0);
		clipping=OFF;
		vClipOn(cur_w);
	}
}

static void near vDispBlock(int x, int y, BYTE height, WORD cr, BYTE lnum)
{
	if(sb.row==eb.row) {
		if(block_task==ON) {
			if(cr==sb.row) {
				vBPT(x, y, sb.y, eb.y, height);
				return;
			}
		}
		else if(lnum==1) goto VBLOCK;
	}
	else {
		if(block_task==ON) {
			if(cr==sb.row) {
				vBPT(x, y, sb.y, 0x0fff, height);
				return;
			}
			else if(cr==eb.row){
				vBPT(x, y, 0x8000, eb.y, height);
				return;
			}
			else if(cr<eb.row && cr>sb.row) {
				vBPT(x, y, 0x8000, 0x0fff, height);
				return;
			}
		}
		else if(lnum==1) {
VBLOCK:
			if(cr<=eb.row && cr>=sb.row) {
				if(sb.y>eb.y) vBPT(x, y, eb.y, sb.y, height);
				else vBPT(x, y, sb.y, eb.y, height);
				return;
			}
		}
	}
	vPT(x, y, height);
}

static int near iSumLeft(BYTE lnum, NODE *temp)
{
	if(lnum==1)
		return (bminy>>3)+(int)((char)temp->lc[1])+temp->lc[2]-tw[cur_w].scr_c;
	else
		return (bminy>>3)+temp->lc[2]-tw[cur_w].scr_c;
}

static int near iPTC(WORD x, BYTE lnum, NODE *temp)
{
	BYTE hsize;
	int y;

	_CHECKNODE(temp);
	hsize=bGetLH(lnum, temp);
	if(ex_err==0) vErrorMsgfDlg(1, 0, 0);
	else {
		psstr=temp->line;
		psstr+=wLine2Loc(lnum-1, temp);
		y=iSumLeft(lnum, temp);
		if(block_task) vDispBlock(x, y, hsize, tw[cur_w].cr, lnum);
		else vPT(x, y, hsize);
		if(temp->kd && lnum==1) vPutHeaderMark(x, temp->kd);
	}
	vClearCurLine(x+hsize, wPrn2Scr(temp->lc[0]));
//  if((temp->hs[lnum-1] >127) && x>bminx) vWLineYBit(x, bminy+1, bmaxy-1);
	return hsize;
}

static void near vDispPrevRow(void)
{
	int y;
	BYTE flag;

	if(cur_lnum>1) {
		_CHECKNODE(CNode);
		psstr=CNode->line;
		psstr+=wLine2Loc(cur_lnum-2, CNode);
		if(cur_lnum==2) {
			flag=1;
			y=(bminy>>3)+(int)((char)CNode->lc[1])+CNode->lc[2]-tw[cur_w].scr_c;
		}
		else {
			y=(bminy>>3)+CNode->lc[2]-tw[cur_w].scr_c;
			flag=0;
		}

		vDispBlock(iCX-bGetLH(cur_lnum-1, CNode)-wPrn2Scr(CNode->lc[0]), y,
			bGetLH(cur_lnum-1, CNode), tw[cur_w].cr-1, flag);
	}
	else if(CNode->prev->prev) {
		_CHECKNODE(CNode->prev);
		psstr=CNode->prev->line;
		psstr+=wLine2Loc(CNode->prev->lc[5]-1, CNode->prev);
		if(CNode->prev->lc[5]==1) {
			y=(bminy>>3)+(int)((char)CNode->prev->lc[1])+CNode->prev->lc[2]-tw[cur_w].scr_c;
			flag=1;
		}
		else {
			y=(bminy>>3)+CNode->prev->lc[2]-tw[cur_w].scr_c;
			flag=0;
		}
		vDispBlock(iCX-(CNode->prev->hs[CNode->prev->lc[5]-1] & 0x70)\
			-wPrn2Scr(CNode->prev->lc[0]),
			y, (CNode->prev->hs[CNode->prev->lc[5]-1] & 0x70), tw[cur_w].cr-1, flag);
	}
}

static void near vDispNextRow(void)
{
	if(cur_lnum<CNode->lc[5]) {
		_CHECKNODE(CNode);
		psstr=CNode->line;
		psstr+=wLine2Loc(cur_lnum, CNode);
		vDispBlock(iCX+cur_hsize+wPrn2Scr(CNode->lc[0]),
			(bminy>>3)+CNode->lc[2]-tw[cur_w].scr_c,
			CNode->hs[cur_lnum] & 0x70, tw[cur_w].cr+1, 0);
	}
	else if(CNode->next) {
		_CHECKNODE(CNode->next);
		psstr=CNode->next->line;
		vDispBlock(iCX+cur_hsize+wPrn2Scr(CNode->lc[0]),
			(bminy>>3)+(int)((char)CNode->next->lc[1])+CNode->next->lc[2]-tw[cur_w].scr_c,
			CNode->next->hs[0] & 0x70, tw[cur_w].cr+1, 1);
	}
}

static WORD near wDispNode(int x, BYTE sline, NODE *temp, WORD *cr)
{
	BYTE lnum;
	BYTE hsize;
	int y, sx, ex;

	_CHECKNODE(temp);
	psstr=temp->line;
	psstr+=wLine2Loc(sline-1, temp);

	for(lnum=sline;lnum<temp->lc[5]+1;lnum++) {
		if(key_flag && wCheckKeyHit()) return 1;
		hsize=bGetLH(lnum, temp);
		if(iCX!=x && x+hsize+wPrn2Scr(temp->lc[0])>bminx+1)	{
			ex=x+hsize;
			sx=x;
			if(sx<bminx+1) sx=bminx;
			else if(ex>bmaxx) ex=bmaxx;
			if(x+hsize>bminx+1) {
				y=iSumLeft(lnum, temp);
				if(block_task) vDispBlock(x, y, hsize, *cr, lnum);
				else vPT(x, y, hsize);
				if(temp->kd && lnum==1) vPutHeaderMark(x, temp->kd);
			}
			else vSkipLine();
			vClearCurLine(x+hsize, wPrn2Scr(temp->lc[0]));
		}
		else vSkipLine();
//		if((temp->hs[lnum-1] > 127) && x>bminx) vWLineYBit(x, bminy+1, bmaxy-1);
		(*cr)++;
		x+=hsize+wPrn2Scr(temp->lc[0]);
		if(x>=bmaxx) {
			key_flag=OFF;
			return 0;
		}
	}
	return x;
}

static void near vDispPageDivide(void)
{
	NODE *temp;
	char sline;
	char lnum;
	int tx;
	BYTE check=1;

	vMCOFF();
	tx=top_x;
	sline=top_lnum;
	temp=top;
	tw[cur_w].cp=top_cp;
	while(temp) {
		for(lnum=sline;lnum<temp->lc[5]+1;lnum++) {
			if((temp->hs[lnum-1] & 0x80) && tx>bminx) {
				vWLineYBit(tx, bminy+1, bmaxy-1);
				if(check) tw[cur_w].cp++;
			}
			if(temp==CNode && lnum<=cur_lnum) check=0;
			tx+=bGetLH(lnum, temp)+wPrn2Scr(temp->lc[0]);
			if(tx>=bmaxx) return;
		}
		if(wCheckKeyHit()) return;
		temp=temp->next;
		sline=1;
	}
	vDispRowStatus();
}

void vDispText(void)
{
	NODE *temp;
	char sline;
	int tx, y;
	WORD i, cr;
	BYTE tflag;

	vMCOFF();
	tflag=key_flag;
	if(key_flag==ON || skip_flag==ON) {
		cr=top_cr;
		tx=top_x;
		sline=top_lnum;
		temp=top;
	}
	else {

		tx=iCX;
		temp=CNode;
		if(cur_lnum!=1)
			for(i=cur_lnum-1;i>0;i--)
				tx-=((temp->hs[i-1] & 0x70)+wPrn2Scr(temp->lc[0]));
		sline=1;
	}
	_CHECKNODE(CNode);
	psstr=CNode->line;
	psstr+=wLine2Loc(cur_lnum-1, CNode);
	i=bGetLH(cur_lnum, CNode);
	y=iSumLeft(cur_lnum, CNode);
	if(block_task) vDispBlock(iCX, y, i, tw[cur_w].cr, cur_lnum);
	else {// if(full_vector!=1) {
		key_flag=OFF;	vPT(iCX, y, i);	key_flag=tflag;
	}
//	else vPT(iCX, y, i);
	if(CNode->kd && cur_lnum==1) vPutHeaderMark(iCX, CNode->kd);
	vClearCurLine(iCX+i, wPrn2Scr(CNode->lc[0]));
//	if((CNode->hs[cur_lnum-1] >127) && iCX>bminx) vWLineYBit(iCX, bminy+1, bmaxy-1);
	while(temp) {
		tx=wDispNode(tx, sline, temp, &cr);
		if(key_flag && wCheckKeyHit()) return;
		if(!tx) {
			key_flag=OFF;
			return;
		}
		temp=temp->next;
		sline=1;
	}
	key_flag=OFF;
	vBarByte(tx, (bminy>>3)+1, bmaxx, bmaxy>>3, 0);
}

void vShowFound(BYTE flag)
{
	WORD i;
	WORD loc;
	static BYTE len;

	if(flag) {
		loc=wTL;
		len=0;
		for(i=0;i<find_flag;) {
			if(pTB[loc]==0) {
			  loc++;
        len++;
				continue;
			}
			if(pTB[loc]>127) {
				len+=((pTB[loc+3] & 0x0f)<<1);
				loc+=5;
				i+=2;
			}
			else {
				len+=pTB[loc+2] & 0x0f;
				loc+=4;
				i++;
			}
		}
	}
	vXORBlockByte(iCX, iCY, cur_hsize, len);
}

static BYTE line_scr=0;

static int near iSyncScroll(BYTE how)
{
	BYTE shift_byte;

	if(iCY<(bminy>>3)+1) {
		if(line_scr==1) return YES;
		if(tw[cur_w].scr_c) {
			shift_byte=(bminy>>3)+1-iCY;
			_WINSCRR(shift_byte);
			tw[cur_w].scr_c-=shift_byte;
			vPutRod(cur_w, 0);
			if(block_task) setb.y+=shift_byte;
			key_flag=ON;
			iCY=(bminy>>3)+1;
			return YES;
		}
	}
	else {
		if(pTB[wTL]>127) {
			if(iCY+((pTB[wTL+3] & 0x0f)<<1)>((bmaxy>>3)-1)) {
				shift_byte=iCY+((pTB[wTL+3] & 0x0f)<<1)-(bmaxy>>3)+1;
				goto SHIFT_GO;
			}
		}
		else {
			if(pTB[wTL]!='\r') {
				if(iCY+(pTB[wTL+2] & 0x0f)>((bmaxy>>3)-1)) {
					shift_byte=iCY+(pTB[wTL+2] & 0x0f)-(bmaxy>>3)+1;
					goto SHIFT_GO;
				}
			}
			else {
				if(iCY>((bmaxy>>3)-1))	{
					shift_byte=iCY-(bmaxy>>3)+1;
SHIFT_GO:
					if(line_scr==1) return YES;
					shift_byte+=how;
					iCY-=shift_byte;
					_WINSCRL(shift_byte);
					tw[cur_w].scr_c+=shift_byte;
					vPutRod(cur_w, 0);
					if(block_task) setb.y-=shift_byte;
					key_flag=ON;
					return YES;
				}
			}
		}
	}
	return NO;
}

int iSyncDC(void)
{
	BYTE flag;

	tw[cur_w].cc=iSetCurrentLine();
	iCY=(bminy>>3)+tw[cur_w].cc-tw[cur_w].scr_c;
	flag=iSyncScroll(0);
	vDispColumeStatus();
	vDispRowStatus();
	return flag;
}

void vSyncCR(BYTE flag)
{
	if(iCX<bminx+1) {
		vMoveDnTop(bminx+1-iCX);
		if(!flag) {
			if(bminx+1-iCX<bmaxx-bminx) _WINSCRDN(bminx+1-iCX);
		}
		key_flag=ON;
		iCX=bminx+1;
	}
	else if(iCX+cur_hsize+wPrn2Scr(CNode->lc[0])>bmaxx) {
		vMoveUpTop(iCX+cur_hsize+wPrn2Scr(CNode->lc[0])-bmaxx);
		if(!flag) {
			if(iCX+cur_hsize+wPrn2Scr(CNode->lc[0])-bmaxx<bmaxx-bminx)
				_WINSCRUP(iCX+cur_hsize+wPrn2Scr(CNode->lc[0])-bmaxx);
		}
		key_flag=ON;
		iCX=bmaxx-cur_hsize-wPrn2Scr(CNode->lc[0]);
	}
}

static void near vBatchSync(void)
{
	iSyncScroll(0);
	vSyncCR(1);
	vPutRod(cur_w, 0);
	vDispColumeStatus();
	vDispRowStatus();
}

static void near vHome(void)
{
	if(tw[cur_w].scr_c) {
		_WINSCRR(tw[cur_w].scr_c);
		if(block_task) setb.y+=tw[cur_w].scr_c;
		tw[cur_w].scr_c=0;
		vPutRod(cur_w, 1);
		key_flag=ON;
	}
	if(cur_lnum==1)
		_COL=CNode->lc[2]+(int)((char)CNode->lc[1]);
	else
		_COL=CNode->lc[2];
	iCY=(bminy>>3)+dc;
}

void vEndKeyTask(void)
{
	if(block_task) return;
	skip_flag=key_flag;
	if(key_flag) {
		key_flag=OFF;
		vDispText();
		skip_flag=OFF;
	}
	return;
}

static void near vHomeKey(void)
{
	WORD temp;
	temp=wLine2Loc(cur_lnum-1, CNode);
	if(temp==wTL) {
		vEndKeyTask();
		return;
	}
	wTL=temp;
	vHome();
	if(block_task) key_flag=ON;
	vDispColumeStatus();
}

static int near iEndOfLine(void)
{
	if(cur_lnum==1)
		_COL=iLoc2Col(wTL, CNode)+(int)((char)CNode->lc[1])+CNode->lc[2]-1;
	else
		_COL=iLoc2Col(wTL, CNode)+CNode->lc[2]-1;
	iCY=(bminy>>3)+dc-tw[cur_w].scr_c;
	return iSyncScroll(0);
}

static void near vEndLine(void)
{
	WORD temp;

	if(pTB[wTL]=='\r') return;
	temp=wLine2Loc(cur_lnum, CNode);
	if(pTB[temp]!='\r') {
		if(pTB[temp-1]>127) temp-=5;
		else temp-=4;
	}
	if(temp==wTL) {
		vEndKeyTask();
		return;
	}
	wTL=temp;
	iEndOfLine();
	if(block_task) key_flag=ON;
	vDispColumeStatus();
}

static int near iMoveNextLine(WORD flag)
{
	int t;
	if(flag) {
		iCX+=(cur_hsize+wPrn2Scr(CNode->lc[0]));
		tw[cur_w].cr++;
		cur_lnum++;
		_OUTDBLINK;
		_COL=CNode->lc[2];
		iCY=(bminy>>3)+dc;
	}
	else {
		if(!CNode->next) {
			vEndKeyTask();
			return NO;
		}
		iCX+=(cur_hsize+wPrn2Scr(CNode->lc[0]));
		tw[cur_w].cr++;
		CNode=CNode->next;
		cur_lnum=1;
		_OUTDBLINK;
		wTL=0;
		_COL=CNode->lc[2]+(int)((char)CNode->lc[1]);
		iCY=(bminy>>3)+dc;
		vPutRod(cur_w, 1);
	}
	t=iCX+cur_hsize+wPrn2Scr(CNode->lc[0]);
	if(t>bmaxx) {
		vMoveUpTop(t-bmaxx);
		if(!tw[cur_w].scr_c) {
			_WINSCRUP(t-bmaxx);
			iCX=bmaxx-cur_hsize-wPrn2Scr(CNode->lc[0]);
			key_flag=ON;
		}
		iCX=bmaxx-cur_hsize-wPrn2Scr(CNode->lc[0]);
	}
	if(CNode->hs[cur_lnum-1]>127) tw[cur_w].cp++;
	if(tw[cur_w].scr_c) {
		_WINSCRR(tw[cur_w].scr_c);
		if(block_task) setb.y+=tw[cur_w].scr_c;
		tw[cur_w].scr_c=0;
		vPutRod(cur_w, 1);
		key_flag=ON;
	}
	block_go=DOWN;
	vDispRowStatus();
	vDispColumeStatus();
	return YES;
}

static int near iMovePrevLine(WORD flag)
{

	if(CNode->hs[cur_lnum-1] & 0x80) tw[cur_w].cp--;
	if(flag) {
		cur_lnum--;
		_OUTDBLINK;
	}
	else {
		if(!CNode->prev->prev) {
			vEndKeyTask();
			return NO;
		}
		CNode=CNode->prev;
		cur_lnum=CNode->lc[5];
		_OUTDBLINK;
		wTL=CNode->bn-1;
		vPutRod(cur_w, 1);
	}
	iCX-=(cur_hsize+wPrn2Scr(CNode->lc[0]));
	tw[cur_w].cr--;
	flag=iEndOfLine();
	if(iCX<bminx+1) {
		vMoveDnTop(bminx+1-iCX);
		if(!flag) {

			_WINSCRDN(bminx+1-iCX);
			iCX=bminx+1;
			key_flag=ON;

		}
		else iCX=bminx+1;
	}
	vDispRowStatus();
	vDispColumeStatus();
	block_go=UP;
	return YES;
}

static void near vRightKey(void)
{
	BYTE tshift;

	if(pTB[wTL]=='\r') {
		if(iMoveNextLine(0)) vEndKeyTask();
//		if(block_task) key_flag=ON;
		return;
	}
	if(pTB[wTL]>127) {
		tshift=(pTB[wTL+3] & 0x0f)<<1;
		iCY+=tshift;
		_COL+=tshift;
		_LOCWP;
	}
	else {
		tshift=(pTB[wTL+2] & 0x0f);
		iCY+=tshift;
		_COL+=tshift;
		_LOCSP;
		while(!pTB[wTL]) {
			wTL++;
			dc++;
			tw[cur_w].cc++;
			iCY++;
		}
	}
	if(pTB[wTL]!='\r') {
		if(pTB[wTL]>127) {
			if(pTB[wTL+2]>127) {
				if(iMoveNextLine(1)) vEndKeyTask();
//				if(block_task) key_flag=ON;
				return;
			}
		}
		else {
			if(pTB[wTL+1]>127) {
				if(iMoveNextLine(1)) vEndKeyTask();
//				if(block_task) key_flag=ON;
				return;
			}
		}
	}
	if(iSyncScroll(tw[cur_w].ly>>2)) vEndKeyTask();
	vDispColumeStatus();
	if(block_task) key_flag=ON;
}

static void near vLeftKey(void)
{
	BYTE tshift;
	int t=0;

	if(wTL==0) {
		if(iMovePrevLine(0)) vEndKeyTask();
//		if(block_task) key_flag=ON;
		return;
	}
	while(!pTB[wTL-1]) {
		wTL--;
		dc--;
		tw[cur_w].cc--;
		iCY--;
	}
	if(pTB[wTL-1]>127) {
		tshift=(pTB[wTL-2] & 0x0f)<<1;
		iCY-=tshift;
		_COL-=tshift;
		_LOCWM;
	}
	else {
		tshift=(pTB[wTL-2] & 0x0f);
		iCY-=tshift;
		_COL-=tshift;
		_LOCSM;
	}
	if(cur_lnum==1) t=(int)((char)CNode->lc[1]);
	if(dc<CNode->lc[2]+t) {
		if(iMovePrevLine(1)) vEndKeyTask();
		if(block_task) key_flag=ON;
		return;
	}
	if(iCY<(bminy>>3)+1) {

		if(tw[cur_w].scr_c>tw[cur_w].ly>>2) {
			tshift=tw[cur_w].ly>>2;
			tw[cur_w].scr_c-=tshift;
		}
		else {
			tshift=tw[cur_w].scr_c;
			tw[cur_w].scr_c=0;
		}
		iCY+=tshift;
		_WINSCRR(tshift);
		vPutRod(cur_w, 1);
		if(block_task) setb.y+=tshift;
		key_flag=ON;
		vEndKeyTask();
	}
	vDispColumeStatus();
	if(block_task) key_flag=ON;
}

static int near iUpKey(BYTE ctrl)
{
	BYTE flag;
	BYTE value;

	if(CNode->hs[cur_lnum-1] & 0x80) tw[cur_w].cp--;
	if(cur_lnum!=1) {
		cur_lnum--;
	}
	else {
		if(!CNode->prev->prev) {
			vEndKeyTask();
			return 0;
		}
		CNode=CNode->prev;
		cur_lnum=CNode->lc[5];
		vPutRod(cur_w, 1);
	}
	_CHECKNODE(CNode);
	flag=iSyncDC();
	value=cur_hsize+wPrn2Scr(CNode->lc[0]);
	if(ctrl) iCX-=value;
	tw[cur_w].cr--;
	vDispRowStatus();
	vSyncCR(flag);
	if(block_task) {
		block_go=UP;
		key_flag=ON;
	}
	return value;
}

static int near iDownKey(BYTE ctrl)
{
	BYTE value;

	if(CNode->lc[5]>cur_lnum) {
		value=cur_hsize+wPrn2Scr(CNode->lc[0]);
		if(ctrl) iCX+=value;
		tw[cur_w].cr++;
		cur_lnum++;
	}
	else {
		if(!CNode->next) {
			vEndKeyTask();
			return 0;
		}
		value=cur_hsize+wPrn2Scr(CNode->lc[0]);
		if(ctrl) iCX+=value;
		tw[cur_w].cr++;
		CNode=CNode->next;
		cur_lnum=1;
		vPutRod(cur_w, 1);
	}
	_CHECKNODE(CNode);
	if(CNode->hs[cur_lnum-1] & 0x80) tw[cur_w].cp++;
	vSyncCR(iSyncDC());
	if(block_task) {
		block_go=DOWN;
		key_flag=ON;
	}
	return value;
}


void vSetCurrentNode(void) 
{												// from top setting , CNode and wTL;
	NODE *temp;
	int tx;
	WORD cr, cp;
	BYTE sline;
	WORD c=0;
	int col;

	temp=top;
	cr=top_cr;
	cp=top_cp;
	sline=top_lnum;
	tx=top_x;

	while(temp!=CNode) {
		_CHECKNODE(temp);
		if(ex_err==0) {
			vErrorMsgfDlg(1, 0, 0);
			CNode=temp->prev;
			break;
		}
		tx=wSkipNode(tx, sline, &cr, &cp, temp);
		temp=temp->next;
		sline=1;
	}
	cur_lnum=1;
	if(CNode->hs[cur_lnum-1]>127) cp++;
	col=(int)((char)CNode->lc[1])+1;
	while(pTB[c]!='\r') {
		if(wTL==c) break;
		if(pTB[c]==0) {
			col++;
			c++;
		}
		else if(pTB[c]>127) {
			col+=((pTB[c+3] & 0x0f)<<1);
			c+=5;
		}
		else {
			col+=(pTB[c+2] & 0x0f);
			c+=4;
		}
		if(pTB[c]==0) {
			if(wTL==c) break;
			col++;
			c++;
		}
		else if(pTB[c]=='\r') break;
		else if(pTB[c]>127) {
			if(pTB[c+2]>127) {
				col=1;
				cur_lnum++;
				if(CNode->hs[cur_lnum-1]>127) cp++;
				continue;
			}
		}
		else {
			if(pTB[c+1]>127) {
				col=1;
				cur_lnum++;
				if(CNode->hs[cur_lnum-1]>127) cp++;
				continue;
			}
		}
	}
	_COL=col+CNode->lc[2]-1;
	for(;sline<cur_lnum;sline++) {
		if(CNode->hs[sline-1]>127) cp++;
		tx+=bGetLH(sline, CNode)+wPrn2Scr(CNode->lc[0]);
		cr++;
	}
	if(sline>cur_lnum) {
		cur_hsize=bGetLH(cur_lnum, CNode)+wPrn2Scr(CNode->lc[0]);
		top_lnum=cur_lnum;
		top_cr--;
		cr--;
	}
	else cur_hsize=bGetLH(cur_lnum, CNode);
	iCY=(bminy>>3)+dc-tw[cur_w].scr_c;
	iCX=tx;
	tw[cur_w].cr=cr;
	tw[cur_w].cp=cp;
}

static int near iSetCurrentNodeXY(int x, int y, BYTE flag)
{
	static NODE *old=0;
	static int old_y=0;
	static BYTE old_lnum=0;
	int tx, hsize;
	BYTE tlnum;
	NODE *temp;

	_SETSTART;
	y=((y-bminy)>>3)+1;
	tx=top_x;
	tlnum=top_lnum;
	temp=top;
	while(1) {
		for(;tlnum<temp->lc[5]+1;tlnum++) {
			hsize=bGetLH(tlnum, temp);
			if(x>tx && x<tx+hsize+wPrn2Scr(temp->lc[0])) goto CHECK;
			else {
				tx+=hsize+wPrn2Scr(temp->lc[0]);
			}
		}
		tlnum=1;
		temp=temp->next;
		if(!temp) return 0;
	}
CHECK:
	if(flag && old==temp && old_lnum==tlnum && old_y==y) return 2;
	old=CNode=temp;
	old_lnum=cur_lnum=tlnum;
	old_y=y;
	dc=y+tw[cur_w].scr_c;
	_COL=iSetCurrentLine();
	vSetCurrentNode();
	vBatchSync();
	vGetCursorSize();
	if(block_task) key_flag=ON;
	return 1;
}

static void near vAltHomeEndKey(BYTE flag)
{
	if(flag) wTL=0;
	else wTL=CNode->bn-1;
	vSetCurrentNode();
	vBatchSync();
	if(block_task) key_flag=ON;
}

static void near vPgUpKey(void)
{
	int sum=0;
	BYTE hsize;

	if(!CNode->prev->prev && cur_lnum==1) {
		vEndKeyTask();
		return;
	}
	while(1) {
		hsize=bGetLH(cur_lnum, CNode)+wPrn2Scr(CNode->lc[0]);
		if(sum+hsize>bmaxx-bminx) break;
		sum+=hsize;
		if(cur_lnum<2) {
			if(!CNode->prev->prev) {
				iCX=bminx+1;
				break;
			}
			CNode=CNode->prev;
			cur_lnum=CNode->lc[5];
		}
		else cur_lnum--;
	}
	tw[cur_w].cc=iSetCurrentLine();
	vSetTopVar();
	vSetCurrentNode();
	vBatchSync();
	key_flag=ON;
}

static void near vPgDnKey(void)
{
	int sum=0;
	BYTE hsize;

	if(!CNode->next && cur_lnum==CNode->lc[5]) {
		vEndKeyTask();
		return;
	}
	while(1) {
		hsize=bGetLH(cur_lnum, CNode)+wPrn2Scr(CNode->lc[0]);
		if(sum+hsize>bmaxx-bminx) break;
		sum+=hsize;
		if(cur_lnum<CNode->lc[5]) cur_lnum++;
		else {
      if(!CNode->next) break;
			CNode=CNode->next;
			cur_lnum=1;
		}
	}
	tw[cur_w].cc=iSetCurrentLine();
	vSetTopVar();
  top_x=bminx+1;
	vSetCurrentNode();
	vBatchSync();
	key_flag=ON;
}

static void near vCtrlRightKey(void)
{
	while(1) {
		vRightKey();
		if(pTB[wTL]=='\r' && !CNode->next) break;
		if(wTL==0 && pTB[wTL]!=32 && pTB[wTL]!='.' &&
			 pTB[wTL]!='\r') break;

		if(pTB[wTL-1]<128) {
			if((pTB[wTL-1]==0 ||
					pTB[wTL-4]==32 || pTB[wTL-4]=='.') &&
				(pTB[wTL]!=32 && pTB[wTL]!='.') &&
				pTB[wTL]!='\r') {
			  break;
      }
    }
  }
}

static void near vCtrlLeftKey(void)
{
	while(1) {
		vLeftKey();
		if(wTL==0 && !CNode->prev->prev) break;
		if(wTL==0 && pTB[wTL]!=32 && pTB[wTL]!='.' &&
				pTB[wTL]!='\r') break;
		if(pTB[wTL-1]<128) {
			if((pTB[wTL-1]==0 ||
					pTB[wTL-4]==32 || pTB[wTL-4]=='.') &&
				(pTB[wTL]!=32 && pTB[wTL]!='.') &&
				pTB[wTL]!='\r') {
				break;
      }
    }
  }
}

static void near vCtrlHomeKey(void)
{
	if(iCX==bminx+1) {
		vEndKeyTask();
		return;
	}
	cur_lnum=top_lnum;
	iCX=top_x;
	tw[cur_w].cr=top_cr;
	tw[cur_w].cp=top_cp;
	CNode=top;
	vSyncCR(iSyncDC());
	if(block_task) key_flag=ON;
}

static void near vCtrlEndKey(void)
{
	BYTE skip=0;
	int i;

	if(iCX+cur_hsize==bminx-1) {
		vEndKeyTask();
		return;
	}
	cur_lnum=iSetEndLine(&iCX, &skip, &(tw[cur_w].cr), &(tw[cur_w].cp));
	for(i=0;i<skip;i++) CNode=CNode->next;
	vPutRod(cur_w, 1);
	vSyncCR(iSyncDC());
	if(block_task) key_flag=ON;
}

static void near vCtrlDownKey(void)
{
	NODE *temp;
	BYTE sbit;
	BYTE tlnum;
	int tx;
	WORD i, j;

	if((sbit=iDownKey(0))==0) return;
	vMoveUpTop(sbit);
	_WINSCRUP(sbit);
	tx=iCX;
	sbit=0;
	i=tw[cur_w].cr;
	j=tw[cur_w].cp;
	tlnum=iSetEndLine(&tx, &sbit, &i, &j);
	temp=CNode;
	for(i=0;i<sbit;i++) temp=temp->next;
	iPTC(tx, tlnum, temp);
	if(tlnum==1) {
		temp=temp->prev;
    tlnum=temp->lc[5];
  }
	else {
		tlnum--;
	}
	tx-=bGetLH(tlnum, temp)+wPrn2Scr(temp->lc[0]);
	iPTC(tx, tlnum, temp);
	key_flag=ON;
}

static void near vCtrlUpKey(void)
{
	int sbit;

	if(!top->prev->prev && top_lnum==1) {
		iUpKey(1);
		return;
	}
	if((sbit=iUpKey(0))==0) return;
	vMoveDnTop(sbit);
	_WINSCRDN(sbit);
  key_flag=ON;
}

static void near vCtrlPgUpKey(void)
{
	if(!CNode->prev->prev && cur_lnum==1 && wTL==0) {
		vEndKeyTask();
		return;
	}
	top_x=iCX=bminx+1;
	_CHECKNODE(SNode->next);
	top=CNode=SNode->next;
	wTL=0;
	_COL=(int)((char)CNode->lc[1])+CNode->lc[2];
	iCY=(bminy>>3)+dc;
	if(block_task) setb.y+=tw[cur_w].scr_c;
	tw[cur_w].scr_c=0;
	top_lnum=cur_lnum=1;
	_OUTDBLINK;
	top_cr=tw[cur_w].cr=1;
	top_cp=tw[cur_w].cp=1;
	key_flag=ON;
	vPutRod(cur_w, 1);
	vDispColumeStatus();
	vDispRowStatus();
}

static void near vCtrlPgDnKey(void)
{
	if(!CNode->next && cur_lnum==CNode->lc[5] && pTB[wTL]=='\r') {
		vEndKeyTask();
		return;
	}
	_CHECKNODE(ENode);
	CNode=ENode;
	cur_lnum=CNode->lc[5];
	_OUTDBLINK;
	wTL=CNode->bn-1;
	if(cur_lnum==1)
		_COL=iLoc2Col(wTL, CNode)+CNode->lc[2]+(int)((char)CNode->lc[1]);
	else
		_COL=iLoc2Col(wTL, CNode)+CNode->lc[2];
	iCY=(bminy>>3)+dc;
	iCX=bmaxx-cur_hsize-wPrn2Scr(CNode->lc[0]);
	tw[cur_w].cr=tw[cur_w].total;
	tw[cur_w].cp=tot_page;
	vSetTopVar();
	iSyncDC();
	key_flag=ON;
}

static void near vCtrlYKey(void)
{
	WORD loc;
	BYTE flag;
	BYTE top_flag;

	if(tw[cur_w].cr==top_cr) top_flag=ON;
	else top_flag=OFF;
	if(CNode->lc[5]==1) {
		if(CNode->next) {
			CNode=CNode->next;
			vDeleteNode(CNode->prev);
			wTL=0;
			cur_lnum=1;
		}
		else if(CNode->prev->prev) {
			wTL=0;
			CNode->line=_REGETLINE(1, CNode->line);
			CNode->line[0]='\r';
			CNode->bn=1;
		}
		else {
			vResetNodeList();
			wTL=0;
			cur_lnum=1;
		}
	}
	else {
		if(cur_lnum==CNode->lc[5]) {
			loc=wLine2Loc(cur_lnum-1, CNode);
			pTB[loc]='\r';
			CNode->bn=loc+1;
			CNode->lc[5]--;
			CNode->line=_REGETLINE(CNode->bn, CNode->line);
			loc=CNode->lc[5];
			tw[cur_w].total--;
			if(CNode->next) {
				if((flag=iMergeNode(CNode))!=1) {
					if(!flag) vErrorMsgfDlg(1, 0, 0);
					key_flag=ON;
				}
				if(CNode->lc[5]>loc) cur_lnum=loc;
				else cur_lnum=CNode->lc[5];
			}
			else cur_lnum=CNode->lc[5];
			vAnalText(CNode);
			wTL=wLine2Loc(cur_lnum-1, CNode);
		}
		else {
			wTL=wLine2Loc(cur_lnum-1, CNode);
			loc=wLine2Loc(cur_lnum, CNode);
			movmem(CNode->line+loc, CNode->line+wTL, CNode->bn-loc);
			CNode->bn-=loc-wTL;
			CNode->lc[5]--;
			CNode->line=_REGETLINE(CNode->bn, CNode->line);
			vAnalText(CNode);
			tw[cur_w].total--;
		}
	}
	if(top_flag) {
		top=CNode;
		top_x=iCX;
		top_cr=tw[cur_w].cr;
		top_lnum=cur_lnum;
	}
	vSetCurrentNode();
	vHome();
	_OUTDBLINK;
	vGetCursorSize();
	vSyncCR(1);
	tw[cur_w].modify=ASV_MODIF=ON;
	if(!key_flag) key_flag=TON;
	vDispColumeStatus();
	vDispRowStatus();
}

int iInsertSpaceChar(BYTE c, WORD loc, NODE *temp)
{
	BYTE i;
	BYTE *str;

	str=_REGETLINE(temp->bn+4*c, temp->line);
	if(!str) {
		vErrorMsgfDlg(1, 0, 0);
		return 0;
	}
	temp->line=str;

	movmem(temp->line+loc, temp->line+loc+4*c, temp->bn-loc);
	temp->bn+=4*c;
	for(i=0;i<c;i++, loc+=4) memcpy(temp->line+loc, ic, 4);
	return 1;
}

int iDelBytes(BYTE v, WORD loc, NODE *temp)
{
	BYTE sum=0;

	while(temp->line[loc]!='\r') {
		if(temp->line[loc]==0) {
			temp->bn--;
			movmem(temp->line+loc+1, temp->line+loc, temp->bn-loc);
			sum++;
		}
		else if(temp->line[loc]>127) {
			sum+=(temp->line[loc+3] & 0x0f)<<1;
			temp->bn-=5;
			movmem(temp->line+loc+5, temp->line+loc, temp->bn-loc);
		}
		else {
			sum+=temp->line[loc+2] & 0x0f;
			temp->bn-=4;
			movmem(temp->line+loc+4, temp->line+loc, temp->bn-loc);
		}
		if(sum>=v) return sum;
	}
	return v;
}

static int near iDelBytesBack(BYTE v, WORD loc, NODE *temp)
{
	int sum=0;

	if(loc==0) return 0;
	while(loc!=0) {
		if(temp->line[loc-1]==0) {
			movmem(temp->line+loc, temp->line+loc-1, temp->bn-loc+1);
			sum++;
			temp->bn--;
			loc--;
		}
		else if(temp->line[loc-1]>127) {
			movmem(temp->line+loc, temp->line+loc-5, temp->bn-loc+1);
			sum+=(temp->line[loc-2] & 0x0f)<<1;
			temp->bn-=5;
			loc-=5;
		}
		else {
			movmem(temp->line+loc, temp->line+loc-4, temp->bn-loc);
			sum+=temp->line[loc-2] & 0x0f;
			temp->bn-=4;
			loc-=4;
		}
		if(sum>=v) return sum;
	}
	return v;
}

static void near vCopyBytes(BYTE flag, BYTE *dat)
{
//	BYTE i;
	if(pTB[wTL]>127) {
		if(pTB[wTL+2]>127) dat[flag-3]|=0x80;
	}
	else {
		if(pTB[wTL+1]>127) dat[flag-3]|=0x80;
	}
	memcpy(pTB+wTL, dat, flag);
}

static int near iSumBytes(WORD sloc, WORD eloc, BYTE *str)
{
	int sum=0;

	while(sloc<eloc) {
		if(str[sloc]>127) {
			sum+=(str[sloc+3] & 0x0f)<<1;
			sloc+=5;
		}
		else {
			if(str[sloc]==0) {
				sloc++;
				sum++;
			}
			else {
				sum+=(str[sloc+2] & 0x0f);
				sloc+=3;
			}
		}
	}
	return sum;
}

static WORD near wFindLineChar(BYTE *check)
{
	WORD loc=0;
	BYTE *str;

	(*check)=0;
	loc=wTL;
	str=CNode->line;
	while(str[loc]!='\r') {
		if(str[loc]>127) {
			loc+=5;
			(*check)=0;
		}
		else if(str[loc]==0) {
			loc++;
			(*check)++;
		}
		else {
			if(str[loc+3]==0x23) return loc;
			if(str[loc]==32) (*check)+=(str[loc+2] & 0x0f);
			else (*check)=0;
			loc+=4;
		}
	}
	return 0xffff;
}

static int near iDelKey(void)
{
	BYTE flag, check, del_size=0;
	WORD keep_loc;

	if((!CNode->next) && pTB[wTL]=='\r') return 0;
	if(tw[cur_w].ins>1) {
		if(pTB[wTL]=='\r') return 0;
		if(pTB[wTL]<128 && pTB[wTL+3]==0x23) return 0;
	}
	if(pTB[wTL]>127) {
		del_size=(pTB[wTL+3] & 0x0f)<<1;
		CNode->bn-=5;
		movmem(pTB+wTL+5, pTB+wTL, CNode->bn-wTL);
	}
	else if(pTB[wTL]!='\r') {
		del_size=(pTB[wTL+2] & 0x0f);
		CNode->bn-=4;
		movmem(pTB+wTL+4, pTB+wTL, CNode->bn-wTL);
	}
	else {
		if((flag=iMergeNode(CNode))!=1) {
			if(!flag) vErrorMsgfDlg(1, 0, 0);
			key_flag=ON;
			return 0;
		}
		if(!key_flag) key_flag=TON;
	}
	if(tw[cur_w].ins>1 && del_size) {
		keep_loc=wFindLineChar(&check);
		if(keep_loc!=0xffff) iInsertSpaceChar(del_size, keep_loc, CNode);
	}
	vAnalText(CNode);
	vSetCurrentNode(); 
	vBatchSync();
	if(!key_flag) key_flag=TON;
	return 1;
}

static void near vCtrlTKey(void)
{
	BYTE flag=1;

	while(flag) {
		if(pTB[wTL]<128) {
			if(pTB[wTL]==32 || pTB[wTL]=='\r') flag=0;
		}
		if(iDelKey()==0) break;
	}
//	while(pTB[wTL]==32) {
	if(tw[cur_w].ins<2) {
		for(flag=0;flag<250 && pTB[wTL]==32;flag++) {
			if(iDelKey()==0) break;
		}
	}
}

static void near vCtrlBSKey(void)
{
	WORD flag=1;

	if(wTL==0 && CNode->prev->prev==0) return;
	while(1) {
		vLeftKey();
		if(pTB[wTL]==32 || pTB[wTL]=='\r') iDelKey();
		else {
			vRightKey();
			break;
		}
	}
	while(flag) {
		vLeftKey();
		if(pTB[wTL]<128) {
			if(pTB[wTL]==32 || pTB[wTL]=='\r') flag=0;
		}
		if(iDelKey()==0) break;
		if(wTL==0) break;
	}
}

static void near vAltYKey(void)
{
	int del_count=0;
	WORD loc=wTL;

	while(1) {
		if(pTB[loc]=='\r') break;
		if(pTB[loc] > 127) {
			if(pTB[loc+2] & 0x80) break;
			loc+=5;
		}
		else {
			if(pTB[loc]==0) {
				loc++;
				continue;
			}
			if(pTB[loc+1] & 0x80) break;
			loc+=4;
		}
		del_count++;
	}
	while(del_count--) {
		if(iDelKey()==0) break;
	}
}

static int near iIntoBuf(BYTE wors, BYTE skip, BYTE *dat)
{
	BYTE need, sum, tlnum, check;
	BYTE *str;
	WORD keep_loc;

	_CHECKNODE(CNode);
	need=(dat[wors-2] & 0x0f)*(wors-3);
	if(tw[cur_w].ins>1) {
		keep_loc=wFindLineChar(&check);
		if(keep_loc==0xffff) check=0;
		else if(check<need) {
			if((tw[cur_w].ins & 1)) return 0;
			else {
				if(need>iSumBytes(wTL, keep_loc, CNode->line)) return 0;
			}
		}
	}
	str=_REGETLINE(CNode->bn+wors, CNode->line);
	if(!str) {
		vErrorMsgfDlg(1, 0, 0);
		return 0;
	}
	movmem(str+wTL, str+wTL+wors, CNode->bn-wTL);
	setmem(str+wTL, wors, 0);
	CNode->line=str;
	CNode->bn+=wors;
	keep_loc+=wors;
	vCopyBytes(wors, dat);
	if(tw[cur_w].ins==3 && check) {
		sum=iDelBytesBack(need, keep_loc, CNode);
		if(sum>need) iInsertSpaceChar(sum-need, keep_loc, CNode);
	}
	else {
		if(!(tw[cur_w].ins & 1) && pTB[wTL+wors]!='\r') {
			sum=iDelBytes(need, wTL+wors, CNode);
			if(sum>need) iInsertSpaceChar(sum-need, wTL+wors, CNode);
		}
	}
	if(skip) {
		wTL+=wors;
		tlnum=cur_lnum;
		vAnalText(CNode);
		vSetCurrentNode(); 
		iSyncScroll(tw[cur_w].ly>>2);
		vSyncCR(1);
		if(tlnum!=cur_lnum) {
			iCY+=tw[cur_w].scr_c;
			_WINSCRR(tw[cur_w].scr_c);
			tw[cur_w].scr_c=0;
		}
	}
	else if(!line_state) {
		vAnalText(CNode);
		vSetCurrentNode(); 
		iSyncScroll(tw[cur_w].ly>>2);
		vSyncCR(1);
	}
	if(!key_flag) key_flag=TON;
	vDispColumeStatus();
	vDispRowStatus();
	return 1;
}

void vDeleteStr(int len)
{
	int sum=0;

	do {
		if(pTB[wTL]>127) sum+=2;
		else if(pTB[wTL]=='\r') break;
		else sum++;
		if(iDelKey()==0) break;
	} while(sum<len);
}

void vInsertStr(BYTE *str)
{
	while(*str) {
		if(*str>127) {
			hc[0]=*(str++);
			hc[1]=*(str++);
			hc[2]=eff & 0x7f;
			hc[3]=sis;
			if(!iIntoBuf(WCODE, 1, hc)) break;
		}
		else {
			ec[0]=*(str++);
			ec[1]=eff & 0x7f;
			ec[2]=sis;
			if(!iIntoBuf(SCODE, 1, ec)) break;
		}
	}
}

extern BYTE *menu_PW22[];

void vUseGlossary(int num)
{
	vInsertStr(menu_PW22[num]+10);
}

static int near iGetTabLoc(int cur_loc)
{
	int i, j, sj;
	BYTE mask=0x80;

	sj=(cur_loc & 7)+1;
	for(i=cur_loc>>3;i<30;i++) {
		for(j=sj;j<8;j++) {
			if((mask>>j) & tw[cur_w].tab[i]) {
				return (i<<3)+j;
			}
		}
		sj=0;
	}
	return cur_loc;
}

static int near iGetPrevTapLoc(int cur_loc)
{
	int i, j, sj;
	BYTE mask=0x80;

	sj=cur_loc%8;
	for(i=cur_loc/8;i>=0;i--) {
		for(j=sj;j>=0;j--) {
			if((mask>>j) & tw[cur_w].tab[i]) {
				return (i<<3)+j;
			}
		}
		sj=0;
	}
	return cur_loc;
}

static void near vTabKey(BYTE flag)
{
	int nt;

	if(flag) nt=iGetTabLoc(tw[cur_w].cc-1);
	else {
		vLeftKey();
		nt=iGetPrevTapLoc(tw[cur_w].cc-1);
	}
	if(flag && (tw[cur_w].ins==1 || pTB[wTL]=='\r')) {
		if(!iInsertSpaceChar(nt-tw[cur_w].cc+1, wTL, CNode)) return;
		_COL=nt+1;
		vAnalText(CNode);
		iSyncDC();
	}
	else {
		_COL=nt+1;
		iSyncDC();
		if(dc>tw[cur_w].cc) vRightKey();
	}
	key_flag=TON;
	vDispColumeStatus();
	vDispRowStatus();
}

static void near vEnterKey(void)
{
	if(tw[cur_w].ins==1	|| (CNode->next==0 && tw[cur_w].ins==0)) {
		if(!iSplitNode()) {
			vErrorMsgfDlg(1, 0, 0);
			return;
		}
		vAnalText(CNode);
	//	if(!tw[cur_w].scr_c) _PUTCURLINE;
		CNode=CNode->next;
		wTL=0;
		vAnalText(CNode);
		vSetCurrentNode();
	}
	else {
		iDownKey(1);
		vHomeKey();
	}
	iSyncDC();
	vSyncCR(1);
	if(tw[cur_w].scr_c) {
		iCY+=tw[cur_w].scr_c;
		_WINSCRR(tw[cur_w].scr_c);
		tw[cur_w].scr_c=0;
//		key_flag=ON;
	}
//else if(!key_flag) key_flag=TON;
	key_flag=ON;
}

void vSetCurrent(void)
{
	WORD i;
	CNode=SNode->next;
	cur_lnum=1;
	for(i=1;i<tw[cur_w].cr;i++) {
		if(CNode->lc[5]==cur_lnum) {
			CNode=CNode->next;
			cur_lnum=1;
		}
		else cur_lnum++;
	}
}

void vGotoLine(void)
{
	tw[cur_w].cr=wGetNumber(tw[cur_w].cr, 1, tw[cur_w].total, "Ð—", "Ñe¸Ð—·¡•·");
	vSetCurrent();
	iSetCurrentLine();
	key_flag=ON;
}

void vEndCombine(void)
{
	if(!start) {
		wTL+=5;
		iCY+=(hc[3] & 0x0f)<<1;
		tw[cur_w].cc+=(hc[3] & 0x0f)<<1;
		_SETSTART;
		iSyncScroll(tw[cur_w].ly>>2);
	}
	_SETSTART;
}

void vRestoreEdit(void)
{
	int i=0;

	_CHECKNODE(CNode);
	vSetTopVar();
	vSetCurrentNode();
	vBatchSync();
	vEndCombine();
	tw[cur_w].total=wGetTotalLineNum();
	_OUTDBLINK;
	if(E_flag) {
		while(Ext_buf[i]==32) i++;
		vInsertStr(Ext_buf+i);
		set_font_flag=E_flag=0;
	}
	vGetCursorSize();
	key_flag=ON;
	if(block_task) vDispExtraStatus(0);
	else vDispKeyInputStatus();
	vDispInsertStatus();
}

void vResetDesktop(void)
{
	vRedrawAllWindow();
	if(!tw[cur_w].active) return;
	vDrawTextWindow(cur_w);
	vEndCombine();
	vRestoreEdit();
	vEndKeyTask();
}

void vSetMargin(int i)
{
	int t1, t2, id;

	id=(int)((char)CNode->lc[1]);
	t1=CNode->lc[2];
	t2=CNode->lc[3];
	if(i<8) {
		if(id>0) {
			switch(i%4) {
				case 0 : if(t1>1)        CNode->lc[2]=t1-1; break; // ALT_F5
				case 1 : if(t1<t2-10-id) CNode->lc[2]=t1+1; break;
				case 2 : if(t2>t1+10+id) CNode->lc[3]=t2-1; break;
				case 3 : if(t2<250)      CNode->lc[3]=t2+1; break; // ALT_F8
			}
		}
		else {
			switch(i%4) {
				case 0 : if(t1>1-id)  CNode->lc[2]=t1-1; break; // ALT_F5
				case 1 : if(t1<t2-10) CNode->lc[2]=t1+1; break;
				case 2 : if(t2>t1+10) CNode->lc[3]=t2-1; break;
				case 3 : if(t2<250)   CNode->lc[3]=t2+1; break; // ALT_F8
			}
		}
	}
	if(i==8) {
		if(id>1-t1  && id>-120) CNode->lc[1]=(char)(id-1);
		else if(id <= 1-t1) {
			CNode->lc[2]=t1+1;
			CNode->lc[1]=(char)(id-1);
		}
	}
	if(i==9 && id<t2-t1-8 && id< 120) CNode->lc[1]=(char)(id+1);
	if(i<4 || i>7) {
		vAnalText(CNode);
		key_flag=ON;
	}
	vRestoreEdit();
	vPutRod(cur_w, 1);
}

void vAdjParagraph(int i)  // 0 enlarge, 1 narrowly
{
	if(i==0) {
		vSetMargin(0); vSetMargin(3);
	}
	else {
		vSetMargin(1); vSetMargin(2);
	}
	vAnalText(CNode);
	key_flag=ON;
}

void vInsertSpacialNode(BYTE kind)
{
	BYTE *str;
	BYTE *ths;
	BYTE lc[6];

	memcpy(lc, CNode->lc, 6);
	lc[1]=2;
	lc[2]=1;
	lc[5]=1;
	str=_GETLINE(CNode->bn-wTL);
	if(!str) goto ERR;
	ths=_GETHS(lc[5]);
	if(!str) {
		free(str);
ERR:
		vErrorMsgfDlg(1, 0, 0);
		return;
	}
	str[0]='\r';
	ths[0]=0x10;
	if(!iInsertNode(CNode->prev, str,	lc, ths, 1, kind)) {
		free(str);
		free(ths);
		goto ERR;
	}
	tw[cur_w].cr++;
	iUpKey(0);
	tw[cur_w].modify=key_flag=ON;
}

void vInsertHeaderNode(int kd)
{
	int i;

	if(kd) { // FOOTER
		strcpy(panel[HF_PAN].head, " ¡Ÿ¡ i");
	}
	else { // HEADER
		strcpy(panel[HF_PAN].head, "  áŸ¡ i");
	}
	i=iPullDownMenu(HF_PAN);
	vRestoreMenuBk(HF_PAN);
	if(i<0) return;
	vInsertSpacialNode(i+kd*3);
}

void vInsertSpecialCode(int i)
{
	BYTE spec[5]={0xff, 0xff, 0, 0x11, 0x80};

	if(i==0) iIntoBuf(WCODE, 1, spec);
}

static int near iCtrlNKey(void)
{
	INKEY c;
	int rtn=0;

	if(!tw[cur_w].active) return 0;
	vDispExtraStatus(6);
	while(!wCheckKeyHit()) {
		if(lbp || rbp) goto OUT;
	}
	c.i=bioskey(0);
	if(c.ch[0]) {
		switch(c.ch[0]) {
			case 'H' : case 'h' : case CTRL_H :
				rtn=350; break;

			case 'M' : case 'm' : case CTRL_M :
				rtn=360; break;
			default :
				vPutKey(c.ch[1], c.ch[0]);
		}
	}
	else vPutKey(c.ch[1], c.ch[0]);
OUT:
	vDispKeyInputStatus();
	return rtn;
}

static int near iCtrlQKey(void)
{
	INKEY c;
	int rtn=0;

	if(!tw[cur_w].active) return 0;
	vDispExtraStatus(6);
	while(!wCheckKeyHit()) {
		if(lbp || rbp) goto OUT;
	}
	c.i=bioskey(0);
	if(c.ch[0]) {
		switch(c.ch[0]) {
			case 'F' : case 'f' : case CTRL_F :
				rtn=410; break;
			case 'A' : case 'a' : case CTRL_A :
				rtn=420; break;
			case 'L' : case 'l' : case CTRL_L :
				rtn=450; break;
			default :
				vPutKey(c.ch[1], c.ch[0]);
		}
	}
	else vPutKey(c.ch[1], c.ch[0]);
OUT:
	vDispKeyInputStatus();
	return rtn;
}

void vF9Key(void)
{
	BYTE tbl[20];
	BYTE str[40];
	WORD loc;
	int count=0;
	int i;

	if(pTB[wTL]<128) {
		vRightKey();
		return;
	}
	setmem(str, 40, 0);
	memcpy(str, pTB+wTL, 2);
	if(pTB[wTL+4] & 0x40) {
		loc=wTL;
		while(count<6) {
			if(loc==0) break;
			if(pTB[loc-1] & 0x40) {
				loc-=5;
				movmem(str, str+2, 38);
				memcpy(str, pTB+loc, 2);
				count++;
			}
			else break;
		}
		if(count==0) {
ONECHAR:
			iSelectHanja(pTB+wTL);
		}
		else {
			i=iSelectHanjaWord(str, tbl);
			if(i==0) goto ONECHAR;
			if(i>0) {
				loc+=(count-i)*5;
				count=i;
				for(i=0;i<count+1;i++, loc+=5) {
					pTB[loc+4]=0x80 | tbl[i]-1;
				}
				key_flag=ON;
				tw[cur_w].modify=ON;
			}
		}
	}
	else pTB[wTL+4]=0xe0;
	vPFFfar(iCX+cur_hsize-(cursor_hsize<<4), iCY, pTB+wTL);
	vRightKey();
}


int iCheckHotKey(INKEY ch)
{
	if(ch.ch[1]==F9) return 540;
	if(!ch.ch[0]) {
SCANCODE:
		vEndCombine();
		if(!block_task) {  
			switch(ch.ch[1]) {
				case INSERT :
					if(_SHIFT_PRESSED) return 250;
					else return 0;
				case ALT_N : return 110;
				case F3 : case ALT_O : return 120;
				case F2 : case ALT_S : return 130;
				case ALT_V : return 150;
				case CTRL_F10 : return 160;
				case ALT_F10 : return 170;
				case ALT_X : return 180;
				case F5 : return 610;
				case F6 : return 620;
				case F7 : return 630;
				case ALT_F1 : return 670;
				case CTRL_F4 : return 660;
				case ALT_F4 : return 650;
				case ALT_M : return 341;
				case ALT_R : return 900;
				case ALT_D : return 320;
				case ALT_U : return 530;
				case ALT_I : return 710;
				case ALT_J : return 720;
				case ALT_P : return 730;
				case ALT_E : return 1009;
			}
		}
		else {  
			switch(ch.ch[1]) {
				case CTRL_INSERT : return 230;
				case CTRL_DELETE : return 220;
				case DELETE :
					if(_SHIFT_PRESSED) return 240;
					else return 220;
				case CTRL_F9 : return 550;
			}
		}
		switch(ch.ch[1]) { 
			case F10 : return -2;
			case ALT_T : return 310;
			case ALT_L : return 599;
			case ALT_B : return 331;
			case ALT_Z : return 334;
			case ALT_F9: return 560;
		}
	}
	else {
		if(_SHIFT_PRESSED && ch.ch[1]>70) goto SCANCODE;
		switch(ch.ch[0]) {   
			case CTRL_Q : return iCtrlQKey();
			case CTRL_N : return iCtrlNKey();
			case CTRL_G : return 440;
			case CTRL_E : return 950;
		}
		if(block_task) {
			switch(ch.ch[0]) {
				case CTRL_K : return 210;
				case CTRL_W :	return 280;
				case CTRL_C : return 230;
				case CTRL_X : return 240;
			}
		}
		else {
			switch(ch.ch[0]) { 
				case CTRL_L : return 430;
				case CTRL_K : return 210;
				case CTRL_R :	return 270;
				case CTRL_P : return 740;
				case CTRL_J : return 1020;
				case CTRL_V :
					if(_SHIFT_PRESSED) return 260;
					else return 250;
				case CTRL_B	: return 260;
			}
		}
	}
	return 0;
}

static void near vStartDrawLine(void)
{
	if(!line_drawing) {
		line_drawing=ON;
		dc=tw[cur_w].cc;
		if(CNode->lc[3]<tw[cur_w].cc) {
			dc=CNode->lc[3];
			iSyncDC();
			return;
		}
		_CHECKNODE(CNode);
		if(pTB[wTL]=='\r') iInsertSpaceChar(1, wTL, CNode);
		key_flag=ON;
	}
}

BYTE lc_tbl[11]={
	0x0c, 0x0d, 0x09, 0x0e, 0x0f, 0x0b, 0x06, 0x07, 0x03, 0x05, 0x0a
};

BYTE cl_tbl[16]={
	0, 9, 10, 8, 9, 9, 6, 7, 10, 2, 10, 5, 0, 1, 3, 4
};

BYTE dl_tbl[16]={
	32, 32, 32, 8, 32, 9, 6, 7, 32, 2, 10, 5, 0, 1, 3, 4
};

static BYTE near bCheckLeftChar(WORD loc)
{
	if(loc!=0) {
		while(pTB[loc-1]==0) loc--;
		if(pTB[loc-1]<128) {
			loc-=4;
			if(pTB[loc+3]==0x23) {
				if(pTB[loc+4]>127 && pTB[loc+6]>127) return 0;
				if(pTB[loc+4]<128 && pTB[loc+5]>127) return 0;
				if(lc_tbl[(pTB[loc]-33)%11] & 4) return 1;
			}
		}
	}
	return 0;
}

static BYTE near bCheckRightChar(WORD loc)
{
	if(pTB[loc]>127) loc+=5;
	else loc+=4;
	while(pTB[loc]==0) loc++;
	if(pTB[loc]<128 && pTB[loc]!='\r') {
		if(pTB[loc+3]==0x23 && pTB[loc+1] <128) {
			if(lc_tbl[(pTB[loc]-33)%11] & 1) return 1;
		}
	}
	return 0;
}

static BYTE near bCheckColume(int cc, int sum, WORD loc, NODE *temp)
{
	while(temp->line[loc]!='\r') {
		if(sum==cc) {
			if(temp->line[loc]<128) {
				if(temp->line[loc+3]==0x23) {
					return lc_tbl[(temp->line[loc]-33)%11];
				}
			}
			return 0;
		}
		if(sum>cc) return 0;
		if(temp->line[loc]>127) {
			sum+=(temp->line[loc+3] & 0x0f)<<1;
			loc+=5;
		}
		else if(temp->line[loc]==0) {
			sum++;
			loc++;
		}
		else {
			sum+=temp->line[loc+2] & 0x0f;
			loc+=4;
		}
	}
	return 0;
}

static BYTE near bCheckUpChar(int cc)
{
	WORD loc;
	int sum;
	BYTE tlnum;
	NODE *temp;

	if(CNode->prev->prev==0) return 0;
	temp=CNode;
	if(cur_lnum==1) {
		temp=temp->prev;
		tlnum=temp->lc[5]-1;
	}
	else tlnum=cur_lnum-2;
	_CHECKNODE(temp);
	sum=temp->lc[2];
	if(tlnum==0) sum+=(int)((char)temp->lc[1]);
	loc=wLine2Loc(tlnum, temp);
	return bCheckColume(cc, sum, loc, temp) & 8;
}

static BYTE near bCheckDownChar(int cc)
{
	WORD loc;
	int sum;
	BYTE tlnum;
	NODE *temp;

	if(CNode->next==0) return 0;
	temp=CNode;
	if(cur_lnum==temp->lc[5]) {
		temp=temp->next;
		tlnum=0;
	}
	else tlnum=cur_lnum;
	_CHECKNODE(temp);
	sum=temp->lc[2];
	if(tlnum==0) sum+=(int)((char)temp->lc[1]);
	loc=wLine2Loc(tlnum, temp);
	return bCheckColume(cc, sum, loc, temp) & 2;
}

static void near vPutLineChar(BYTE v_h)
{
	BYTE tins;
	BYTE t;
	BYTE dest=0;

	if(line_state==2) return;
	tins=tw[cur_w].ins;
	tw[cur_w].ins=0;
	sc[1]=0;
	sc[2]=cur_hsize+1;
	sc[3]=0x23;
	if(pTB[wTL+3]==0x23) {
		t=lc_tbl[(pTB[wTL]-33)%11];
		if(t & 1) { // to vLeftKey
			if(bCheckLeftChar(wTL)) dest|=1;
		}
		if(t & 2) { // to up
			if(bCheckUpChar(tw[cur_w].cc)) dest|=2;
		}
		if(t & 4) { // to vRightKey
			if(bCheckRightChar(wTL)) dest|=4;
		}
		if(t & 8) { // to down
			if(bCheckDownChar(tw[cur_w].cc)) dest|=8;
		}
		if(line_state==1)	sc[0]=33+line_style*11+cl_tbl[dest | v_h];
		else {
			sc[0]=dl_tbl[dest & (~v_h)];
			if(sc[0]==32) {
				sc[2]=0x11;
				sc[3]=0x40;
			}
			else sc[0]+=33+line_style*11;
		}
	}
	else {
		if(line_state==1) {
			if(v_h & 5) sc[0]=9+33+line_style*11;
			else if(v_h & 0x0a) sc[0]=10+33+line_style*11;
		}
		else {
			sc[0]=32;
			sc[2]=0x11;
			sc[3]=0x40;
		}
	}
	if(line_state==1 && v_h & 0x05) memcpy(ic, sc, 4);

	if(CNode->line[wTL]>127) t=CNode->line[wTL+2] & 0x80;
	else t=CNode->line[wTL+1] & 0x80;
	iIntoBuf(SCODE, 0, sc);
	CNode->line[wTL+1]=t;

	tw[cur_w].modify=ON;
	ASV_MODIF=ON;

	ic[0]=32;
	ic[2]=0x11;
	ic[3]=0x40;
	tw[cur_w].ins=tins;
}

static int near iDrawLineRight(BYTE flag)
{
	if(tw[cur_w].cc>=CNode->lc[3]) return 0;
	vPutLineChar(4);
	if(flag) _PUTCURLINE;
	vRightKey();
	if(pTB[wTL]=='\r') {
		if(line_state!=2)	iInsertSpaceChar(1, wTL, CNode);
		else return 0;
	}
	vPutLineChar(1);
	if(flag) _PUTCURLINE;
	return 1;
}

static int near iDrawLineLeft(BYTE flag)
{
	if(cur_lnum==1) {
		if(tw[cur_w].cc<=(int)((char)CNode->lc[1])+CNode->lc[2]) return 0;
	}
	else {
		if(tw[cur_w].cc<=CNode->lc[2]) return 0;
	}
	vPutLineChar(1);
	if(flag) _PUTCURLINE;
	vLeftKey();
	vPutLineChar(4);
	if(flag) _PUTCURLINE;
	if(wTL==0) return 0;
	return 1;
}

static void near vDrawLineUp(void)
{
	if(!CNode->prev->prev && CNode->lc[5]==1) return;
	vPutLineChar(2);
	_PUTCURLINE;
	line_scr=1;
	iUpKey(1);
	if(line_state!=2) {
		if(pTB[wTL]=='\r') {
			iInsertSpaceChar(dc-tw[cur_w].cc+1, wTL, CNode);
			iSyncDC();
		}
		else if(dc!=tw[cur_w].cc) {
			iDownKey(1);
			line_scr=0;
			return;
		}
	}
	line_scr=0;
	vPutLineChar(8);
	_PUTCURLINE;
}

static void near vDrawLineDown(void)
{
	BYTE lc[6];
	BYTE *tln;
	BYTE *ths;

	if(line_state!=2 && !CNode->next && CNode->lc[5]==cur_lnum) {
		memcpy(lc, CNode->lc, 6);
		lc[5]=1;
		tln=_GETLINE(1);
		tln[0]='\r';
		ths=_GETHS(1);
		ths[0]=0x10;
		iInsertNode(CNode, tln, lc, ths, 1, 0);
	}
	vPutLineChar(8);
	_PUTCURLINE;
	line_scr=1;
	iDownKey(1);
	if(line_state!=2) {
		if(pTB[wTL]=='\r') {
			iInsertSpaceChar(dc-tw[cur_w].cc+1, wTL, CNode);
			iSyncDC();
		}
		else if(dc!=tw[cur_w].cc) {
			iUpKey(1);
			line_scr=0;
			return;
		}
	}
	line_scr=0;
	vPutLineChar(2);
	_PUTCURLINE;
}

static void near vDrawLineAutoRight(void)
{
	while(1) {
//		if(line_state!=2) {
			if(!iDrawLineRight(0)) break;
//		}
		if(CNode->line[wTL]!='\r'
			&& CNode->line[wTL]<128
			&& CNode->line[wTL+3]==0x23
			&& (lc_tbl[(CNode->line[wTL]-33)%11] & 0x0a)) break;
	}
}

static void near vDrawLineAutoLeft(void)
{
	while(1) {
//		if(line_state!=2) {
			if(!iDrawLineLeft(0)) break;
//		}
		if(CNode->line[wTL]!='\r'
			&& CNode->line[wTL]<128
			&& CNode->line[wTL+3]==0x23
			&& (lc_tbl[(CNode->line[wTL]-33)%11] & 0x0a)) break;
	}
	vSetCurrentNode();
}

static void near vDrawLineToStart(void)
{
	if(pTB[wTL]>127) {
		if(pTB[wTL+2]>127) return;
	}
	else {
		if(pTB[wTL+1]>127) return;
	}
	if(line_state==2) vHomeKey();
	else {
		while(iDrawLineLeft(0));
		_PUTCURLINE;
	}
	vSetCurrentNode();
}

static void near vDrawLineToEnd(void)
{
	if(line_state==2) vEndLine();
	else {
		while(iDrawLineRight(0));
		_PUTCURLINE;
	}
}

void vLineDraw(void)
{
	INKEY ch;
	int t;

	line_state=1;
	vDispExtraStatus(line_state);
	line_drawing=OFF;
	vStartDrawLine();
	for(;;) {
		if(key_flag) vDispText();
		vMCON(); vTCON();
		while(!wCheckKeyHit()) vTextCursorOperation(1, tw[cur_w].h_e);
		vStartDrawLine();
		ch.i=bioskey(0);
		vTCOFF(); vMCOFF();
		if(!ch.ch[0]) {
			switch(ch.ch[1]) {
				case RIGHT : iDrawLineRight(1); break;
				case LEFT : iDrawLineLeft(1); break;
				case DOWN : vDrawLineDown(); break;
				case UP : vDrawLineUp(); break;
				case HOME : vDrawLineToStart(); break;
				case END : vDrawLineToEnd(); break;
				case CTRL_RARROW : vDrawLineAutoRight(); break;
				case CTRL_LARROW : vDrawLineAutoLeft(); break;
				case ALT_D :
					t=iPullDownMenu(LINE_PAN);
					if(t>0) line_style=t-1;
					vRestoreMenuBk(LINE_PAN);
					break;
				case F1 :
					line_state=1;
					vDispExtraStatus(line_state);
					break;
				case F2 :
					line_drawing=OFF;
					line_state=2;
					vDispExtraStatus(line_state);
					break;
				case F3 :
					line_drawing=OFF;
					line_state=3;
					vDispExtraStatus(line_state);
					break;
			}
		}
		else {
			switch(ch.ch[0]) {
				case ESC : line_state=0; vGetCursorSize(); return;
				case SPACE :
					line_drawing=OFF;
					line_state++;
					if(line_state>3) line_state=1;
					vDispExtraStatus(line_state);
					break;
			}
		}
	}
}

int Old_tot=0, Old_cur;

void vSaveCR(int tot, int cur)
{
	Old_tot=tot; Old_cur=cur;
}

void vTextScrollBar(void)
{
	if(tw[cur_w].total==Old_tot && tw[cur_w].cr==Old_cur) return;
	vDelVThumbBit(tw[cur_w].x+36, tw[cur_w].x+(tw[cur_w].lx<<3)-16, (tw[cur_w].y+tw[cur_w].ly-2)<<3,
					 Old_tot-1, Old_cur-1);
	vDrawVThumbBit(tw[cur_w].x+36, tw[cur_w].x+(tw[cur_w].lx<<3)-16, (tw[cur_w].y+tw[cur_w].ly-2)<<3,
					 tw[cur_w].total-1, tw[cur_w].cr-1, 0);
	vSaveCR(tw[cur_w].total, tw[cur_w].cr);
}

void vLineUpDown(BYTE flag)
{
	vMCOFF();
	switch(flag) {
		case 0 : iUpKey(1);   break;
		case 1 : iDownKey(1); break;
	}
	vGetCursorSize();
	_PUTCURLINE;
	vMCON();
}

extern BYTE mc_color;
void vTest()
{
	iPrintf(0, 0, 0, "%d %d %d %d %d", CNode->lc[0], CNode->lc[1],
					CNode->lc[2], CNode->lc[3], CNode->lc[4]);
}

BYTE engstr[]={"#$@[\\]^`{|}~\""};

BYTE bInAscii(BYTE h_e, BYTE ch)
{
	BYTE *p;
	BYTE ac=0;

	p=strchr(engstr, ch);
	if(p) {
		ac=eng_tbl[h_e-ENGLOC][(int)(p-engstr)];
		if(ac) ac+=31;
	}
	return ac;
}

static void near vKeyTask(void)
{
	if(block_task) {
		if(tw[cur_w].cr<setb.row) {
			sb.row=tw[cur_w].cr; sb.y=iCY; sb.c=tw[cur_w].cc;
			sb.loc=wTL; sb.blk=CNode;
			memcpy(&eb, &setb, sizeof(eb));
		}
		else if(tw[cur_w].cr>setb.row) {
			eb.row=tw[cur_w].cr; eb.y=iCY; eb.c=tw[cur_w].cc;
			eb.loc=wTL; eb.blk=CNode;
			memcpy(&sb, &setb, sizeof(sb));
		}
		else {
			eb.row=sb.row=setb.row; eb.blk=sb.blk=setb.blk;
			if(iCY<setb.y) {
				sb.y=iCY; sb.c=tw[cur_w].cc; sb.loc=wTL;
				eb.y=setb.y; eb.c=setb.c; eb.loc=setb.loc;
			}
			else {
				eb.y=iCY; eb.c=tw[cur_w].cc; eb.loc=wTL;
				sb.y=setb.y; sb.c=setb.c; sb.loc=setb.loc;
			}
		}
		if(block_go==DOWN && !wCheckKeyHit()) {
			vDispPrevRow(); block_go=OFF;
		}
		else if(block_go==UP && !wCheckKeyHit()) {
			vDispNextRow(); block_go=OFF;
		}
		btasking=ON;
	}
	else if(btasking) {
		vDispKeyInputStatus();
		btasking=OFF;
	}
	if(key_flag) vDispText();
	vTextScrollBar();
}

void vSetBlock(void)
{
	int ox=0, oy=0;
	BYTE t, flag;

	getpos(ox, oy);
	iSetCurrentNodeXY(ox, oy, 0);
	if(lbp && block_task) vCtrlKKey(ON);
	vKeyTask();
	vMCON();
	vTCON();
	while(lbp) {
		if(mousex<bminx) t=1;
		else if(mousex>bmaxx) t=2;
		else if(mousey<bminy || mousey>bmaxy) {
			flag=0;
			goto CHECKING;
		}
		else t=0;
		flag=1;
		if(ox!=mousex || oy!=mousey || t) {
			vMCOFF();
			vTCOFF();
			if(t) {
				vLineUpDown(t-1);
				vKeyTask();
			}
			else {
CHECKING:
				ox=mousex;
				oy=mousey;
				if(iSetCurrentNodeXY(ox, oy, flag)) {
					if(!block_task) vCtrlKKey(ON);
					vKeyTask();
				}
			}
			vTCON();
			vMCON();
		}
		else vTextCursorOperation(1, tw[cur_w].h_e);
	}
	if(block_task) {
		if(sb.y==eb.y && sb.row==eb.row) {
			vCtrlKKey(ON);
			vDispKeyInputStatus();
		}
	}
}

int rbp_func[2][8]={
	{250, 270, 290, 430, 440, 599, 540},
	{220, 230, 240, 280, 310, 599, 550},
};

int iRightMouseButton(void)
{
	if(block_task) {
		if(MRfunc2)	return rbp_func[1][MRfunc2-1];
	}
	else {
		if(MRfunc1) {
			if(MRfunc1==7) iSetCurrentNodeXY(mousex, mousey, 0);
			return rbp_func[0][MRfunc1-1];
		}
	}
	vPutKey(F1, 0);
	return 0;
}

int iEditor(void)
{
	INKEY ch;
	int new_flag;
	CODE2 fst;
	BYTE start_flag;
	int rtn_num;
	BYTE ac;
	BYTE j1, j2;

	if(!tw[cur_w].active) return -2;
	vMCOFF();
	if(disp_japan) vDrawKeyMap();
	vRestoreEdit();
	vEndKeyTask();
	vDispPageDivide();
	if(find_flag) {
		vMCOFF();
		vShowFound(1);
		vTCON();
		while(!wCheckKeyHit()) {
			if(lbp) break;
			vTextCursorOperation(1, tw[cur_w].h_e);
		}
		vTCOFF();
		vShowFound(0);
		find_flag=OFF;
	}
	for(;;) {
LOOP:
		page_div_flag=key_flag;
		vKeyTask();
		if(page_div_flag) {
			vDividePage(0);
			if(!page_div_flag) vDispPageDivide();
		}
		vMCON(); vTCON();
		while(!wCheckKeyHit()) {
			if(rbp) {
				if(lbp) goto TOMENU;
				while(rbp);
				if((rtn_num=iRightMouseButton())!=0)	goto OUT;
			}
			else if(lbp) {
				vTCOFF();
				if(rbp) {
TOMENU:
					while(rbp || lbp);
					rtn_num=-2;
					goto OUT;
				}
				rtn_num=iMouseInTextWindow();
				if(rtn_num==2) vSetBlock();
				if(rtn_num==1) {
					rtn_num=-1; goto OUT;
				}
				if(mousex<16) {
					if(mousey<16) last_top=0;
					else {
						if(mousey<592) last_top=(mousey-16)/64+1;
						else continue;
					}
					rtn_num=-2;
					goto OUT;
				}
				vTCON();
			}
			else vTextCursorOperation(1, tw[cur_w].h_e);
			vDispTime(0);
			vAutoSave();
		}
		ch.i=bioskey(0);
		vTCOFF(); vMCOFF();
//		vTest();
		if(ch.ch[0]==32) {
			if(_ALT_PRESSED) {
CHARSET:
				vTCOFF();
				vMCON();
				iKeyMapMenuDrv();
				vMCOFF();
				goto CONTI2;
			}
			if(_LEFT_SHIFT) {
				if(tw[cur_w].h_e==han_kbd) tw[cur_w].h_e=eng_kbd;
				else tw[cur_w].h_e=han_kbd;
				goto CONTI;
			}
			else if(_RIGHT_SHIFT) {
				if(C_cnt==0) continue;
				if(C_h_e>=C_cnt-1) C_h_e=0;
				else C_h_e++;
				tw[cur_w].h_e=C_key[C_h_e];
				if(tw[cur_w].h_e>1) {
					if(tw[cur_w].h_e<JAPANLOC) {  // etc
						sc[3]=0x24;
					}
					else if(tw[cur_w].h_e<HALFLOC) {  // japan input automata
					}
					else if(tw[cur_w].h_e<FULLLOC) { // half input automata
						sc[3]=0x20 | (tw[cur_w].h_e-HALFLOC);
					}
					else {  // full input automata
						wc[4]=0xc0 | (tw[cur_w].h_e-FULLLOC);
					}
				}
CONTI:
				if(disp_japan) vDrawKeyMap();
CONTI2:
				vEndCombine(); vDispKeyInputStatus();	continue;
			}
		}
		rtn_num=iCheckHotKey(ch);
		if(rtn_num!=0) goto OUT;
		_CHECKNODE(CNode);
		if(ex_err==0) {
			vErrorMsgfDlg(1, 0, 0);
			continue;
		}
		if(!ch.ch[0]) {
SCANCODE:
			vEndCombine();
			if(!block_task) {  
				switch(ch.ch[1]) {
					case ALT_EQUAL : case ALT_INSERT :
						tw[cur_w].ins ^=2; vDispInsertStatus(); continue;
					case ALT_W : vChangeActiveWindow(); rtn_num=-1; goto OUT;
					case DELETE :	iDelKey(); vGetCursorSize(); continue;
					case ALT_Y  : vEndCombine(); vAltYKey(); vGetCursorSize(); continue;
					case SFT_TAB : vTabKey(0); vGetCursorSize(); continue;
					case CTRL_DELETE : vEndCombine(); vCtrlTKey(); vGetCursorSize(); continue;
					case ALT_F2: goto CHARSET;
				}
			}
			else {  
			}
			if((ch.ch[1]>70 && ch.ch[1]<82) ||
				 (ch.ch[1]>114 && ch.ch[1]<120) ||
				 ch.ch[1]==CTRL_PGUP || ch.ch[1]==ALT_PGUP || ch.ch[1]==ALT_PGDN ||
				 ch.ch[1]==CTRL_UARROW || ch.ch[1]==CTRL_DARROW ||
				 ch.ch[1]==ALT_HOME || ch.ch[1]==ALT_END) {
											
				if(_LEFT_SHIFT) {
					if(!block_task) vCtrlKKey(ON);
				}
				else if(_RIGHT_SHIFT) {
					if(!block_task) vCtrlKKey(ON+ON);
				}
				switch(ch.ch[1]) {
					case RIGHT : vRightKey();	break;
					case LEFT :	vLeftKey();	break;
					case DOWN :	iDownKey(1); break;
					case UP :	iUpKey(1);	break;
					case PGUP : vPgUpKey(); break;
					case PGDN : vPgDnKey(); break;
					case HOME :	vHomeKey(); break;
					case END : vEndLine();	break;
					case CTRL_HOME : vCtrlHomeKey();	break;
					case CTRL_END :	vCtrlEndKey();	break;
					case CTRL_PGDN : vCtrlPgDnKey();	break;
					case CTRL_PGUP : vCtrlPgUpKey();	break;
					case CTRL_RARROW : vCtrlRightKey(); break;
					case CTRL_LARROW : vCtrlLeftKey(); break;
					case CTRL_UARROW : vCtrlUpKey(); break;
					case CTRL_DARROW : vCtrlDownKey(); break;
					case ALT_HOME : vAltHomeEndKey(1); break;
					case ALT_END : vAltHomeEndKey(0); break;
					case ALT_PGUP : vGotoPageDlg(UP); vRestoreEdit(); break;
					case ALT_PGDN : vGotoPageDlg(DOWN); vRestoreEdit(); break;
				}
				set_font_flag=0;
				vGetCursorSize();
				continue;
			}
			if(ch.ch[1]<=ALT_0 && ch.ch[1]>=ALT_1) {
				vUseMacro(ch.ch[1]-ALT_1);
				vDispKeyInputStatus();
				continue;
			}
			if(ch.ch[1]<=SHIFT_F10 && ch.ch[1]>=SHIFT_F1) {
				vUseGlossary(ch.ch[1]-SHIFT_F1);
				continue;
			}
			switch(ch.ch[1]) { 
				case INSERT :
					if(!ins_lock) {tw[cur_w].ins^=1; vDispInsertStatus();}	continue;
				case F10 : rtn_num=-2; goto OUT;
				case F1 : vHelpDlg(11); break;
				case F4 : vCtrlKKey(ON+ON); break;
				case ALT_F5 :  case ALT_F6 :
				case ALT_F7 :  case ALT_F8 : vSetMargin(ch.ch[1]-ALT_F5); break;
				case CTRL_F5 :  case CTRL_F6 : vSetMargin(ch.ch[1]+8-CTRL_F5); break;
				case CTRL_F7 :  case CTRL_F8 : vAdjParagraph(ch.ch[1]-CTRL_F7); break;
				case ALT_A : vSetFontDispOpt(); break;
				case ALT_H : no_work=10000; break;
				default : vBeep(); break;
			}
		}
		else {
			if(_SHIFT_PRESSED && ch.ch[1]>70) goto SCANCODE;
			if(block_task) { 
				vEndCombine();
				switch(ch.ch[0]) {
					case ESC : case CTRL_K : block_task=OFF; key_flag=ON;	break;
					default :	vBeep();	break;
				}
				continue;
			}
			switch(ch.ch[0]) { 
				case TAB : vEndCombine(); vTabKey(1); vGetCursorSize(); break;
				case CTRL_Y :	vEndCombine(); vCtrlYKey(); vGetCursorSize(); break;
				case CTRL_T :	vEndCombine(); vCtrlTKey(); vGetCursorSize(); break;
				case CTRL_BS: vEndCombine(); vCtrlBSKey(); vGetCursorSize(); break;
				case CTRL_Z:
					tw[cur_w].ins ^=2; vDispInsertStatus(); break;
				case CTRL_A :
					if((ec[3] & 0x04) == 0) ec[3]=0x44;
					else if(ec[3]==0x44) ec[3]=0x45;
					else if(ec[3]==0x45) ec[3]=0x40;
					vDispScriptStatus();
					break;
				case BS :
					if(!start) {
						c.i=wPopHangul();
						if(c.i==HANNULL) {
							_SETSTART;
							iDelKey();
							break;
						}
						hc[0]=c.c2[1];
						hc[1]=c.c2[0];
						vCopyBytes(WCODE, hc);
						vPFFfar(iCX+cur_hsize-(sis & 0xf0), iCY, hc);
						break;
					}
					else if(CNode->prev->prev || wTL) {
						_SETSTART;
						if(tw[cur_w].ins>1 && wTL==0) break;
						vLeftKey();
						iDelKey();
					}
					break;
				case RETURN :
					vEndCombine();
					vEnterKey();
					break;
				default :
					if(ch.ch[0]<32 || ch.ch[0]>126) {
						vBeep();
						break;
					}
					if(tw[cur_w].h_e==0) {
INTOSCODE1:
						ec[0]=ch.ch[0];
						ec[1]=eff & 0x7f;
						ec[2]=sis;
						if(!iIntoBuf(SCODE, 1, ec)) {
							_SETSTART;
							break;
						}
						break;
					}
					else if(tw[cur_w].h_e==1 || tw[cur_w].h_e==2) {
						start_flag=start;
						vPushHangul(c.i, status);
						if(tw[cur_w].h_e==1) new_flag=bCombine2(ch.ch[0], &fst.i);
						else {
							if(ch.ch[1]>70) {
								vEndCombine();
								goto INTOSCODE1;
							}
							new_flag=bCombine3(ch.ch[0], &fst.i);
						}
						if(new_flag==1) {
							hc[0]=c.c2[1];
							hc[1]=c.c2[0];
							hc[2]=eff & 0x7f;
							hc[3]=sis;
							if(start_flag) {
								if(!iIntoBuf(WCODE, 0, hc)) {
									_SETSTART;
									break;
								}
							}
							else {
								vCopyBytes(WCODE, hc);
								vPFFfar(iCX+cur_hsize-(sis & 0xf0), iCY, hc);
							}
						}
						else if(new_flag==0) {
							hc[0]=fst.c2[1];
							hc[1]=fst.c2[0];
							hc[2]=eff & 0x7f;
							hc[3]=sis;
							vCopyBytes(WCODE, hc);
							wTL+=WCODE;
							vResetHangulStack();
							hc[0]=c.c2[1];
							hc[1]=c.c2[0];
							hc[2]=eff & 0x7f;
							hc[3]=sis;
							if(!iIntoBuf(WCODE, 0, hc)) {
								if(tw[cur_w].ins>1) {
									wTL-=WCODE;
									if(!key_flag) key_flag=TON;
								}
								_SETSTART;
								break;
							}
							vPFFfar(iCX+cur_hsize-(sis & 0xf0), iCY, hc);
							if(status>CHOSONG2) {
								fst.i=c.i;
								fst.c3.code2=2;
								vPushHangul(fst.i, CHOSONG1);
							}
						}
						else {
SPACEKEY:
							vEndCombine();
							ec[0]=new_flag;
							ec[1]=eff & 0x7f;
							ec[2]=sis;
							if(!iIntoBuf(SCODE, 1, ec)) break;
						}
						break;
					}
					else if(tw[cur_w].h_e==3) {
						ch.ch[0]=dvorak[ch.ch[0]-32]+32;
						goto INTOSCODE1;
					}
					else if(tw[cur_w].h_e<JAPANLOC) {  // etc
						ac=bInAscii(tw[cur_w].h_e, ch.ch[0]);
						if(ac==0) goto INTOSCODE1;
						else {
							if(ac<128) {
								sc[0]=ac;	sc[3]=0x24;
							}
							else {
								sc[0]=(ac & 0x7f)+32;	sc[3]=0x34;
							}
							goto INTOSCODE2;
						}
					}
					else if(tw[cur_w].h_e<HALFLOC) {  // japan input automata
						if(tw[cur_w].h_e<JAPANLOC+2) {
							new_flag=iJapanSoriAutomata(ch.ch[0], &j1, &j2);
							vJapanInputState();
							if(new_flag>2) goto SPACEKEY;
							if(new_flag>0) {
								if(j1) {
									jc[1]=j1;
									jc[2]=eff & 0x7f;
									jc[3]=sis;
									if(!iIntoBuf(WCODE, 1, jc)) _SETSTART;
								}
								jc[1]=j2;
							}
							else jc[1]=0;
						}
						else {
							if(tw[cur_w].h_e==JAPANLOC+2) jc[1]=hkana_tbl[ch.ch[0]-32];
							else if(tw[cur_w].h_e==JAPANLOC+3) jc[1]=kkana_tbl[ch.ch[0]-32];
							if(jc[1]==0) {
								new_flag=ch.ch[0];
								goto SPACEKEY;
							}
						}
						if(jc[1]!=0) {
							jc[2]=eff & 0x7f;
							jc[3]=sis;
							if(!iIntoBuf(WCODE, 1, jc)) _SETSTART;
						}
					}
					else if(tw[cur_w].h_e<FULLLOC) { // half input automata
						if(ch.ch[0]==32) {
							new_flag=32;
							goto SPACEKEY;
						}
						sc[0]=ch.ch[0];
						sc[3]&=0xef;
INTOSCODE2:
						sc[1]=eff & 0x7f;
						sc[2]=sis;
						if(!iIntoBuf(SCODE, 1, sc)) break;
					}
					else {  // full input automata
						if(ch.ch[0]==32) {
							new_flag=32;
							goto SPACEKEY;
						}
						wc[1]=ch.ch[0];
						wc[2]=eff & 0x7f;
						wc[3]=sis;
						if(!iIntoBuf(WCODE, 1, wc)) _SETSTART;
					}
					break;
			}
		}
	}
OUT:
	vDispTime(2);
	vTCOFF();
	vMCON();
//	vEndCombine();
	_SETSTART;
	return rtn_num;
}

void vInitSwap(void)
{
	struct ffblk ffblk;

	if(findfirst(swp_path, &ffblk, FA_DIREC)!=0) mkdir(swp_path);
}

void vSetCtrlC(BYTE c)
{
	union REGS r;

	r.h.ah=0x33;
	r.h.al=1;
	r.h.dl=c;
	int86(0x21, &r, &r);
}

BYTE far MACname[80];
extern BYTE far GLSname[];
extern BYTE far config_name[];



extern BYTE open_twin_old_fname[80];

//BYTE forsize[16];
void main(int argc, char *argv[])
{
	BYTE *cnf_path;
	int temp, t;
	BYTE j;
	BYTE i;


//	printf("\x7%d\n", sizeof(NODE));
	vSetCtrlC(0);
	if(bCheckFile("21.CNF")!=0) {
		iFnSplit(argv[0]);
		if(*g_dir) {
			cnf_path=malloc(80);
			sprintf(cnf_path, "%s%s21.CNF", g_drive, g_dir);
		}
		else cnf_path=searchpath("21.CNF");
		iReadConfig(&i, &j, cnf_path);
		strcpy(config_name, cnf_path);
	}
	else {
		getcwd(g_dir, 80);
		sprintf(MACname, "%s\\21.CNF", g_dir);
		strcpy(config_name, MACname);
		iReadConfig(&i, &j, MACname);
	}
	sprintf(g_dir, "%s\\PRNDRV.INF", exe_path);
	prn_number=iGetPrinterDriver(g_dir);
	vInitPrintSetup();
	if(prn_number==0) {
		printf("Cannot found PRNDRV.INF file!!\n");
		printf("Execute 21SET.EXE and check FONT directory setting!!\n");
		exit(-1);
	}
	vInitSwap();
	if(iInitMemorySystem()!=1) goto CANNOT;
	if(iInitHangul(set_graph_mode)!=1) goto CANNOT;

	for(t=0;t<100;t++) {
		temp=iInitMouse(i, j);
		if(temp!=100) break;
	}
	if(temp!=-1) {
		vDeinitGraphMode();
		printf("Mouse not installed!\n");
		if(temp==100) {
			printf("Your 8259 Chip is halted! You must reboot! Press reset!\n");
			printf("Unless you cannot use mouse!!\n");
			vDeinitMouse();
		}
		goto CANNOT;
	}
	vMCOFF();
	vSetMousePosition(0, 0);
	vInitWindowValue();  //Use MaxX, MaxY;
	vInitMenu();
	vInitErrorHandler();
	vMakeOurBuf();

	sprintf(g_dir, "%s\\21.MAC", exe_path);
	strcpy(MACname, g_dir);
	vReadKeyMacro(MACname);
	sprintf(g_dir, "%s\\21.GLS", exe_path);
	strcpy(GLSname, g_dir);
	vGLSFileIO(0);

	if(color_mode) vSetColors();
	vFillScreen(0xaaaa, 0x5555);
	vSetDispOpt(disp_space, disp_newline, disp_return);
	vDrawTopMenu();
	vInitINT8();
	vInitINT66();
	vCheckSpeed();
	if(argc<2) {
		vLogo();
		vNewTextWindow();
	}
	else {
		for(i=0;argv[1][i];i++) argv[1][i]=toupper(argv[1][i]);
		if(strchr(argv[1], '*') || strchr(argv[1], '?')) {
			strcpy(open_twin_old_fname, argv[1]);
			vOpenTextDlg(1);
		}
		else {
			iFnSplit(argv[1]);
			if(*g_ext==0) {
				strcpy(g_ext, ".GWP");
			}
			fnmerge(path_buf, g_drive, g_dir, g_file, g_ext);
			vOpenTextDlg(0);
		}
	}
	vPutKey(ALT_0, 0);
	vLetsGo();
CANNOT:
	vDeinitMemorySystem();
	exit(-1);
}
