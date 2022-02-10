#define _LAYOUT4
#include <stdio.h>
#include <string.h>
#include <alloc.h>
#include <dos.h>
#include <bios.h>
#include <mem.h>
#include <dir.h>
#include <process.h>
#include "est.h"
	#include "21.h"
	#include "proto.h"
#include "mouse.h"
#include "layout.h"

#define _LOCSP wTL+=SCODE
#define _LOCWP wTL+=WCODE
#define _LOCSM wTL-=SCODE
#define _LOCWM wTL-=WCODE
#define _PUTCURLINE iPTC(iCX, cur_lnum, CNode)
#define _COL  dc=tw[cur_w].cc
#define _WINSCRUP(sbit) vScrollUpByte(bminx+1, bmaxx+2,\
													(bminy>>3)+1, tw[cur_w].ly-3, sbit)
#define _WINSCRDN(sbit) vScrollDownByte(bminx, bmaxx-1,\
													(bminy>>3)+1, tw[cur_w].ly-3, sbit)
#define _WINSCRR(sbyte) vScrollRightByte(bminx, bmaxx,\
													(bminy>>3)+1,	tw[cur_w].ly-3, sbyte, 0)
#define _WINSCRL(sbyte) vScrollLeftByte(bminx, bmaxx,\
													(bminy>>3)+1, tw[cur_w].ly-3, sbyte, 0)
#define _CLEARWIN(x) vBarByte(x, (bminy>>3)+1, bmaxx, (bmaxy>>3), 0)
#define pTB	CNode->line
#define _OUTDBLINK cur_hsize=CNode->hs[cur_lnum-1] & 0x70
#define bGetLH(lnum, t) (t->hs[lnum-1] & 0x70)
#define iCX tw[cur_w].cx
#define iCY tw[cur_w].cy
#define wTL tw[cur_w].bl

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

extern NODE *SNode, *CNode, *ENode;

#define LINEGAB		lc[0]
#define INDENT		lc[1]
#define LEFTR			lc[2]
#define RIGHTR    lc[3]
#define ALINE			lc[4]
#define LNUM			lc[5]

#define PRNMAX    30
#define PAPMAX    10

#define JP0 0
#define JP1 1
#define JP2 2
#define JP3 3
#define JP4 20
#define JP5 5
#define JP6 6
#define JP7 7
#define JP8 8
#define JP9 9
#define JP10 10

extern BYTE *vid_addr;
extern text_window tw[];
extern popup_window pop[];
extern menu_panel panel[];
extern int bminx, bminy, bmaxx, bmaxy, MaxX, MaxY;
extern BYTE cur_w;
extern BYTE printing;
extern BYTE mouse_kind;
extern BYTE mouse_port;
extern unsigned long Scount;
extern BYTE *menu_PW5[];

extern BYTE def_lc[6];
extern BYTE load_tab;
extern BYTE exe_path[];
extern BYTE hfnum;
extern BYTE efnum;
extern BYTE jfnum;

extern BYTE load_lc[6];
extern cursor_flag;
extern BYTE on_off;
extern BYTE eff;
extern BYTE eff_filter;
extern BYTE sis;
extern BYTE sis_filter;
extern BYTE hc[];
extern BYTE ec[4];
extern BYTE jc[5];
extern BYTE sc[4];
extern BYTE wc[5];
extern BYTE cur_hsize;
extern BYTE start;
extern BYTE status;
extern BYTE cur_lnum;
extern BYTE key_flag;
extern BYTE skip_flag;
extern block_task;
extern btasking;
extern block_go;
extern BYTE set_font_flag;
extern int scr_saver_num;

extern blk sb, eb, setb;

extern int dc;
extern CODE2 c;
extern BYTE cur_w;
extern text_window tw[];
extern text_window def_tw;
extern BYTE *our_buf;
extern int graph_mode;
extern popup_window pop[];
extern BYTE full_vector;
extern int MaxX, MaxY;
extern BYTE swp_path[];
extern BYTE the21[];
extern BYTE gwp_data[];
extern BYTE swp_data[];
extern BYTE cfg_data[];
extern BYTE hwp11_data[];
extern BYTE hwp12_data[];
extern BYTE g_drive[];
extern BYTE g_dir[];
extern BYTE g_file[];
extern BYTE g_ext[];
extern BYTE nick_name[];
extern int last_top;
extern BYTE *psstr;
extern WORD work_area_size;
extern WORD tot_page;
extern BYTE rev_flag;

int jminx, jminy, jmaxx, jmaxy;

	Mem jopan_m;

NODE *s_node;
int s_page=1;
BYTE s_lnum=0;
int s_x=0;
BYTE scr_flag=0;
WORD prn_resolution=18;
WORD paper_hsize_inch=105;
WORD paper_vsize_inch=80;
WORD paper_len;  //dot in printer
WORD paper_wid;  //dot in printer
WORD work_pagex;
WORD work_pagey;
WORD up_gap, dn_gap, l_gap;

// Global variable on printing
BYTE prn_setup[4];
BYTE p_sced=OFF;
BYTE is_laser=0;
BYTE prn_win=1;
BYTE prn_number=0;
BYTE psort=6;
NODE *head_node[4]={0, 0, 0, 0};
NODE *foot_node[4]={0, 0, 0, 0};

#define MAXDRVNAME  20
BYTE drv_name[MAXDRVNAME][13]={
	"24KSSM1.DRV ",
	"24KSSM2.DRV ",
	"24KS.DRV    ",
	"8KSSM1.DRV  ",
	"8KSSM2.DRV  ",
	"PCL3.DRV    ",
	"PCL3.DRV    ",
	"8           ",
	"9           ",
	"10          ",
	"11          ",
	"12          ",
	"13          ",
	"14          ",
	"15          ",
	"16          ",
	"17          ",
	"18          ",
	"19          ",
	"20          ",
};

int drv_dns[MAXDRVNAME]={180, 180, 180, 180, 180, 150, 150, 180};
int drv_up[MAXDRVNAME]={6, 6, 6, 6, 6, 15, 9, 3};
int drv_dn[MAXDRVNAME]={6, 6, 6, 6, 6, 15, 9, 0};
extern BYTE *dataPW14[];
extern int START_PAGE_NUM;
extern int CUR_PAGE_NUM;

void vPrintOptDlg(int n);

#define PRN_NUM prn_setup[0]
#define PAP_NUM prn_setup[1]
								 //  80,   132  A4   B5  letter Regal
WORD papwids[PAPMAX]={203, 335, 210, 184, 216, 216};
WORD paplens[PAPMAX]={266, 266, 297, 266, 279, 355};

WORD wSmallSCR2PRN(WORD n)
{
	if(is_laser) return n*5/4;
	else         return n*3/2;
}

WORD wSmallPRN2SCR(WORD n)
{
	if(is_laser) return n*4/5;
	else         return n*2/3;
}

WORD wMM2DOT(WORD i)  // convert mm to 1/180 or 1/150 inch
{
	return (WORD)((unsigned long)3937L*(drv_dns[PRN_NUM]/10)*i/10000L);

}

void vSetPaperSize(void)
{
	int n;
	n=prn_setup[1];
	paper_len=wMM2DOT(paplens[n]);
	paper_wid=wMM2DOT(papwids[n]);
}

void vSetPaper(void)
{
	WORD papup, papdn;

	papup=wMM2DOT(drv_up[PRN_NUM]);
	papdn=wMM2DOT(drv_dn[PRN_NUM]);

	vSetPaperSize();
	jmaxx=wSmallPRN2SCR(paper_len);
	jmaxy=wSmallPRN2SCR(paper_wid)+16;
	if(jmaxx>MaxX<<1) jmaxx=MaxX<<1;
	if(jmaxy>61<<4) jmaxy=61<<4;

	up_gap=wMM2DOT(tw[cur_w].up_s+HEADLEN(cur_w));
	dn_gap=wMM2DOT(tw[cur_w].down_s+FOOTLEN(cur_w));
	l_gap=wMM2DOT(tw[cur_w].left_s);
	work_pagex=paper_len-papup-papdn-up_gap-dn_gap;
	work_pagey=paper_wid-l_gap;
}

static int near iSetHeaderNode(NODE *temp)
{
	int rtn=0;

	switch(temp->kd) {
		case 0 : break;
		case HEADERALL:
			head_node[0]=head_node[2]=temp;
			rtn=1;
			if(temp->next->kd==HEADERALL)	{
				head_node[1]=head_node[3]=temp->next;
				rtn=2;
			}
			else head_node[1]=head_node[3]=0;
			break;
		case HEADER1:
			head_node[0]=temp;
			rtn=1;
			if(temp->next->kd==HEADER1)	{
				head_node[1]=temp->next;
				rtn=2;
			}
			else head_node[1]=0;
			break;
		case HEADER2:
			head_node[2]=temp;
			rtn=1;
			if(temp->next->kd==HEADER2)	{
				head_node[3]=temp->next;
				rtn=2;
			}
			else head_node[3]=0;
			break;
		case FOOTERALL:
			foot_node[0]=foot_node[2]=temp;
			rtn=1;
			if(temp->next->kd==FOOTERALL)	{
				foot_node[1]=foot_node[3]=temp->next;
				rtn=2;
			}
			else foot_node[1]=foot_node[3]=0;
			break;
		case FOOTER1:
			foot_node[0]=temp;
			rtn=1;
			if(temp->next->kd==FOOTER1)	{
				foot_node[1]=temp->next;
				rtn=2;
			}
			else foot_node[1]=0;
			break;
		case FOOTER2:
			foot_node[2]=temp;
			rtn=1;
			if(temp->next->kd==FOOTER2)	{
				foot_node[3]=temp->next;
				rtn=2;
			}
			else foot_node[3]=0;
			break;
	}
	return rtn;
}

int iSetHeaderNodeFar(NODE *n)
{
	return iSetHeaderNode(n);
}

void vFindHeader(void)
{
	NODE *temp;

	temp=SNode->next;
	while(temp) {
		if(iSetHeaderNode(temp)==2) {
			if(temp->prev==s_node) break;
			temp=temp->next;
		}
		if(temp->prev==s_node) break;
		temp=temp->next;
	}
}

void vDrawPaper(void)
{
	vBarByte(0, 0, jmaxx>>1, jmaxy>>4, 1);
}

extern int ex_err;
int iDispSmallNode(int sx, int sy, BYTE sline, BYTE flag, NODE *temp)
{
	BYTE lnum;
	BYTE hsize;
	int sumh=0;
	int sumlg=0;

	_CHECKNODE(temp);
	psstr=temp->line;
	psstr+=wLine2Loc(sline-1, temp);

	for(lnum=sline;lnum<temp->lc[5]+1;lnum++) {
		if(key_flag && wCheckKeyHit()) return 0;
		hsize=bGetLH(lnum, temp);
		if((temp->hs[lnum-1] > 127 && flag)|| \
				sx+sumh+wSmallPRN2SCR(sumlg)+hsize>=jmaxx) {
			key_flag=OFF;
			return jmaxx;
		}
		if(lnum==1)
			vSPT(sx+sumh+wSmallPRN2SCR(sumlg), (sy>>3)+(int)((char)temp->lc[1])+temp->lc[2], hsize);
		else
			vSPT(sx+sumh+wSmallPRN2SCR(sumlg), (sy>>3)+temp->lc[2], hsize);
		sumh+=hsize;
		sumlg+=temp->lc[0]*LGAP_MUL(cur_w);
		flag=1;
	}
	return sumh+sumlg/3*2;
}

void vDispSmallText(int sx, int sy)
{
	NODE *temp;
	char sline;
	BYTE flag=0, t;

	vMCOFF();
	sline=s_lnum;
	temp=s_node;
	while(temp) {
		if(temp->kd==0) sx+=iDispSmallNode(sx, sy, sline, flag, temp);
		else {
			t=iSetHeaderNode(temp);
      if(t>1) temp=temp->next;
			goto NEXT;
		}
		flag=1;
		if(key_flag && wCheckKeyHit()) return;
		if(sx>jmaxx) {
			key_flag=OFF;
			return;
		}
NEXT:
		temp=temp->next;
		sline=1;
	}
	key_flag=OFF;
}

int iSkipText(WORD sbit) // unit is 1/180 inch
{
	WORD sum=0;
	BYTE i;
	int rtn=0;

	while(s_node) {
		if(s_node->kd==0) {
			for(i=s_lnum;i<s_node->lc[5]+1;i++) {
				sum+=wSmallSCR2PRN((s_node->hs[i-1] & 0x70))+s_node->lc[0]*LGAP_MUL(cur_w);
				if(rtn==0 && (s_node->hs[i-1] & 0x80)) rtn=1;
				if(sum>sbit) {
					s_lnum=i;
					return rtn;
				}
			}
		}
		s_node=s_node->next;
		s_lnum=1;
	}
	return rtn;
}

void vDispPageStatus(void)
{
	vBarByte(349-100, (MaxY>>3)-17, MaxX, (MaxY>>3)+1, 0);
	vBarByte(349-90, (MaxY>>3)-16, 349-10, (MaxY>>3)-8, 1);
	if(scr_flag) {
		vXORBoxBit(349-62, MaxY-142, 349-8, MaxY-65);
	}
	else {
		vXORBoxBit(349-92, MaxY-142, 349-38, MaxY-65);
	}
	iPrintf(349-26, (MaxY>>3)-7, 0, "%3d ¡e", s_page);
}

void vDispSmallPage(BYTE flag)
{
	WORD scr_bit;
	int odd_even;
//	int write;

	if(s_page & 1) odd_even=0;
	else odd_even=2;
	vMCOFF();
	vDrawPaper();
	vChangePutMode(0);
	s_node=npGetPageStart(s_page, &s_lnum);
	for(scr_bit=0;scr_bit<4;scr_bit++) {
		head_node[scr_bit]=foot_node[scr_bit]=0;
	}
	if(flag) {
		scr_bit=wMM2DOT(drv_up[PRN_NUM]+drv_dn[PRN_NUM])+wSmallSCR2PRN(jmaxx)+up_gap;
		if(paper_len>scr_bit) {
			scr_bit=paper_len-scr_bit;
iSkipText(scr_bit);
			vFindHeader();
 vDispSmallText(0, wSmallPRN2SCR(l_gap));
			if(foot_node[odd_even]) goto PUTFOOT;
		}
		else {
			flag=1;
			goto ONEPEACE;
		}
	}
	else {
ONEPEACE:
		vFindHeader();
		if(head_node[odd_even]) {
			_CHECKNODE(head_node[odd_even]);
			psstr=head_node[odd_even]->line;
			vSPT(wSmallPRN2SCR(up_gap-wMM2DOT(HEADLEN(cur_w))), wSmallPRN2SCR(l_gap)>>3,
									 bGetLH(1, head_node[odd_even]));
			if(head_node[odd_even+1]) {
				_CHECKNODE(head_node[odd_even+1]);
				psstr=head_node[odd_even+1]->line;
				vSPT(wSmallPRN2SCR(up_gap-wMM2DOT(HEADLEN(cur_w)))+bGetLH(1, head_node[odd_even]),
										 wSmallPRN2SCR(l_gap)>>3, bGetLH(1, head_node[odd_even+1]));
			}
		}
		vDispSmallText(wSmallPRN2SCR(up_gap), wSmallPRN2SCR(l_gap));
		if(foot_node[odd_even] && paper_len<wSmallSCR2PRN(jmaxx)+up_gap) {
PUTFOOT:
			_CHECKNODE(foot_node[odd_even]);
			psstr=foot_node[odd_even]->line;
			vSPT(jmaxx-wSmallPRN2SCR(dn_gap), wSmallPRN2SCR(l_gap)>>3, bGetLH(1, foot_node[odd_even]));
			if(foot_node[odd_even+1]) {
				_CHECKNODE(foot_node[odd_even+1]);
				psstr=foot_node[odd_even+1]->line;
				vSPT(jmaxx-wSmallPRN2SCR(dn_gap)+bGetLH(1, foot_node[odd_even]),
						wSmallPRN2SCR(l_gap)>>3, bGetLH(1, foot_node[odd_even+1]));
			}
		}
	}
	vChangePutMode(1);
	scr_flag=flag;
	vDispPageStatus();
	vMCON();
}

void vEndPreview(void)
{
	vMCOFF();
	if(!iSwapMem(MSREAD, work_area_size, &jopan_m, vid_addr)) {
		vErrorMsgfDlg(1, 2, 0);
		vResetDesktop();
	}
	vFreeMem(&jopan_m);
	vClipOn(cur_w);
	panel[JOPAN].active=0;
	vMCON();
}

void vDispPreviewMenu(void)
{
	int x, y, i;

	vMCOFF();
	vBarByte(0, (MaxY>>3)-17, 17*6+32+23*2, (MaxY>>3)+1, 1);
	vPutMenu(JOPAN);
	panel[JOPAN].active=1;

	x=17*6+25;
	y=(MaxY>>3)-16;
	vMCOFF();
	for(i=0; i<3;i++) {
		vNBoxBit(x, (y+1+i*5)<<3, x+23-2, ((y+5+i*5)<<3)-1, 0);
		vPutFreeImageByte(x+3, y+2+i*5, 16, 16, J_arrow[i*2], 1);
		vNBoxBit(x+23, (y+1+i*5)<<3, x+46-2, ((y+5+i*5)<<3)-1, 0);
		vPutFreeImageByte(x+23+3, y+2+i*5, 16, 16, J_arrow[i*2+1], 1);
	}
	vMCON();
}

void vXORPreviewIcon(int n)
{
	int x, y;

	vMCOFF();
	x=n%2;
	x=17*6+26+x*23;
	y=n/2;
	y=(MaxY>>3)-16+y*5;
	vNBarBit(x, ((y+1)<<3)+1, x+19, ((y+5)<<3)-2, 2);
	vMCON();
}

int iGetPreviewIconCheck(void)
{
	int mx, my;

	getpos(mx, my);
	my-=MaxY-128;
	mx-=17*6+25;
	if(mx>0 && my>0) {
		if(my<32) {
			if(mx<23) return 0;
			else if(mx<46) return 1;
		}
		else if(my>40 && my<72) {
			if(mx<23) return 2;
			else if(mx<46) return 3;
		}
		else if(my>80 && my<118) {
			if(mx<23) return 4;
			else if(mx<46) return 5;
		}
	}
	return -1;
}

int iPreviewMenu(void)
{
	INKEY c;
	int choice=panel[JOPAN].last_menu;
	int t_choice, flag=0, t;

	if(!panel[JOPAN].active) vDispPreviewMenu();
	else vRevMenuItem(JOPAN, choice);
	for(;;) {
		while(!wCheckKeyHit()) {
			while(lbp) {
				if((t=iGetPreviewIconCheck())!=-1 && flag==0) {
					vXORPreviewIcon(t);
					while(lbp);
					vXORPreviewIcon(t);
					vRevMenuItem(JOPAN, choice);
					panel[JOPAN].last_menu=choice;
					switch(t) {
						case 0 : return -2;
						case 1 : return -3;
						case 2 : return -4;
						case 3 : return -5;
						case 4 : return -6;
						case 5 : return -7;
					}
				}
				t_choice=iMouseInMenu(1, JOPAN);
				if(t_choice!=choice || flag==1) {
					if(t_choice!=-1) {
						if(flag==0) {
							vRevMenuItem(JOPAN, choice);
						}
						choice=t_choice;
						vRevMenuItem(JOPAN, choice);
						flag=0;
					}
					else if(flag==0) {
						vRevMenuItem(JOPAN, choice);
						flag=1;
					}
				}
				if(lbp==0) {
					if(iMouseInMenu(1, JOPAN)!=-1) {
						panel[JOPAN].last_menu=choice;
						vRevMenuItem(JOPAN, choice);
						goto RN;
					}
					else {
						vRevMenuItem(JOPAN, choice);
						flag=0;
						break;
					}
				}
			}
		}
		c.i=bioskey(0);
		vRevMenuItem(JOPAN, choice);
		if(c.ch[0]) {
			switch(c.ch[0]) {
				case ESC    :
					return -1;
				case RETURN :
RN:       if(panel[JOPAN].menu[choice-1][0]=='0') {
						return choice;
					}
					else break;
				case SPACE  :
					choice++;
					if(choice>panel[JOPAN].count) choice=1;
					break;
				case CTRL_G : return  JP8;
				default :
					if((c.ch[0]>64 && c.ch[0]< 91) || (c.ch[0]>96 && c.ch[0]<123)) {
						if(c.ch[0]>96 && c.ch[0]<123) c.ch[0]-=32;
						t=iIsIn(JOPAN, c.ch[0]);
						if(t!=-1) {
							choice=t;
							panel[JOPAN].last_menu=choice;
							return choice;
						}
					}
					break;
			}
		}
		else {
			switch(c.ch[1]) {
				case UP    :
					choice--;
					if(choice<1) choice=panel[JOPAN].count;
					break;
				case DOWN  :
					choice++;
					if(choice>panel[JOPAN].count) choice=1;
					break;
				case HOME  : return -2;
				case END   : return -3;
				case LEFT  : return -4;
				case RIGHT : return -5;
				case PGUP  : return -6;
				case PGDN  : return -7;
				case ALT_P : return  JP1;
				case ALT_G : return  JP2;
				case F1 : return JP9;
				case ALT_I : return JP10;
			}
		}
		vRevMenuItem(JOPAN, choice);
		panel[JOPAN].last_menu=choice;
	}
}

void vPreview(void)
{
	int func;

	vMCOFF();
	if(!iSwapMem(MSWRITE, work_area_size, &jopan_m, vid_addr)) {
		vErrorMsgfDlg(1, 2, 0);
		return;
	}
	vSetBlackWhite();
	vFillScreen(0, 0);
	prn_win=cur_w;
	jminx=-1;
	jminy=-1;
	jmaxx=MaxX<<1;
	jmaxy=MaxY<<1;
	s_page=tw[cur_w].cp;
	s_node=SNode->next;
	s_lnum=1;
	vSetPaper();
	vDividePage(1);
	vDispSmallPage(0);
	for(;;) {
		func=iPreviewMenu();
		switch(func) {
			case -1 :
				goto OUT;
			case -2 :
				if(scr_flag) vDispSmallPage(0);
				break;
			case -3 :
				if(!scr_flag) vDispSmallPage(1);
				break;
			case -4 : break;
			case -5 : break;
			case -6 :
				if(s_page<=1) break;
				s_page--;
				vDispSmallPage(0);
				break;
			case -7 :
				if(s_page>=tot_page) break;
				s_page++;
				vDispSmallPage(0);
				break;
			case  JP1 :
				vPrintOptDlg(cur_w);
				vDispSmallPage(0);
				break;
			case  JP4 :
				vHeaderSizeDlg();
				break;
			case  JP2 :
				s_page=wGetNumber(s_page, 1, tot_page, "¡e", "Ñe¸¡e·¡•·");
				vDispSmallPage(0);
				break; // goto page
			case  JP3 : goto OUT;
			case  JP9 : vHelpDlg(JP_HELP); break;
			case 	JP10 : vPrintStart(); break;
			default   : break;
		}
	}
OUT :
	vSetColors();
	vEndPreview();
}




BYTE Scadule=OFF;
BYTE Sstack[5];
BYTE Si=0;

extern BYTE *Span[];

int iCheckPrintScadule(void)
{
	int i;
	if(Scadule) {
		for(i=0; i<5; i++) {
			if(Sstack[i]) {
				if(!tw[i+1].active) return 0;
			}
		}
		return 1;
	}
	return 0;
}

void vPutLot(int n, BYTE flag)
{
	int x, y, i;
	BYTE attr=0x00;

	if(flag) {
		n--;
		if(Sstack[n]) {
			x=Sstack[n]; Si--;
			Sstack[n]=0;
			for(i=0; i<5; i++) if(Sstack[i]>x) Sstack[i]--;
		}
		else {
			Si++; Sstack[n]=Si;
		}
		n++;
	}
	vRevMenuItem(SCADULER, n);
	x=panel[SCADULER].x;
	y=panel[SCADULER].y;
	for(i=0; i<5; i++) {
		if(Sstack[i]) iPrintf(x+2+i*17, y+1+5+12, attr, "%d", Sstack[i]);
		else if(panel[SCADULER].menu[i][0]=='0')
			vWriteString(x+2+i*17, y+1+5+12, " ", attr);
	}
	vRevMenuItem(SCADULER, n);
}

void vDelScadule(void)
{
	Scadule=OFF;
	setmem(Sstack, 5, 0);
	Si=0;
	panel[SCADULER].last_menu=1;
}

int iSetScadule(void)
{
	int i, rtn, x, y, win=0;

	x=pop[PW5].x;  y=pop[PW5].y;
	vSetMenuPara(SCADULER, x+124, y+28);
	for(i=0; i<5; i++) {
		memcpy(Span[i]+5, "              ", 13);
		if(tw[i+1].active) {
			fnsplit(tw[i+1].headline, g_drive, g_dir, g_file, g_ext);
			setmem(g_dir, 20, ' ');
			sprintf(g_dir, "%s%s", g_file, g_ext);
			memcpy(Span[i]+5, g_dir, strlen(g_dir));
			panel[SCADULER].menu[i][0]='0';
			win++;
		}
		else panel[SCADULER].menu[i][0]='2';
	}
	if(!iCheckPrintScadule()) vDelScadule();
	vPutMenu(SCADULER);
	if(Scadule) vPutLot(panel[SCADULER].last_menu, 0);
	while(1) {
		i=iGetUserResponse(SCADULER);
		if(i==-1 || i==7) {
			rtn=0; break;
		}
		else if(i==6) {
			rtn=1; break;
		}
		else vPutLot(i, 1);
	}
	vRestoreMenuBk(SCADULER);
	return rtn;
}


#define pkind     prn_setup[0]  // the kind of user printer
#define paper     prn_setup[1]  // the way of suppling papers
#define psupply   prn_setup[2]  // '0' meaning continous paper '1'
#define pport     prn_setup[3]  // '0'=lpt1 '1'lpt2

BYTE *units[]={
	"¸a/12 ",
	"   ¤ ",
	"   mm ",
	"   mm ",
	"   mm ",
	"   mm ",
	"   mm ",
	"     ",
  "   ¤å ",
};

BYTE popt_buf[150];
BYTE Home[9]={0,1,0,0,0,0,0,1,1};
BYTE End[9]={24,5,50,50,50,30,30,99,999};
extern int START_PAGE_NUM;

void vTWopt2PRNopt(int t)
{
	BYTE *addr;
	int i;

	addr=&tw[t].word_gap;
	for(i=0; i<9; i++) {
		strcpy(popt_buf+7+i*15, units[i]);
		popt_buf[1+i*15]=Home[i];       // +1 is SNode button gets 1 BYTE
		if(i!=8) popt_buf[5+i*15]=End[i];
	}
	popt_buf[5+8*15]=(2000 & 0xff);
	popt_buf[6+8*15]=(2000 >>8);

	popt_buf[3+0*15]=addr[0];
	popt_buf[3+1*15]=LGAP_MUL(t);
	popt_buf[3+2*15]=addr[1];
	popt_buf[3+3*15]=addr[2];
	popt_buf[3+4*15]=addr[3];
	popt_buf[3+5*15]=HEADLEN(t);
	popt_buf[3+6*15]=FOOTLEN(t);
	popt_buf[3+7*15]=addr[4];

	popt_buf[3+8*15]=(START_PAGE_NUM & 0xff);
	popt_buf[4+8*15]=(START_PAGE_NUM >> 8);
}

void vPRNopt2TWopt(int t)
{
	BYTE *addr;

	addr=&tw[t].word_gap;
	addr[0] =popt_buf[3+0*15];
	LGAP_MUL(t)=popt_buf[3+1*15];
	addr[1] =popt_buf[3+2*15];
	addr[2] =popt_buf[3+3*15];
	addr[3] =popt_buf[3+4*15];
	HEADLEN(t) =popt_buf[3+5*15];
	FOOTLEN(t) =popt_buf[3+6*15];
	addr[4] =popt_buf[3+7*15];
	START_PAGE_NUM=popt_buf[3+8*15] | ((int)popt_buf[4+8*15]<<8);
}

void vPrintOptDlg(int n)
{
	int i, j, cur_win, tmp, tmp2, t;
	BYTE tbuf[10], t2[4];
	BYTE *addr;

	if(cur_w==0) return;
	setmem(popt_buf, 130, 0);
	vOpenDlgWindow(PW5);
	addr=tw[n].prn_range;

	if(addr[0]==0xff && addr[1]==0xff) {
		vPutMenuObj2(PW5, 3, 12, panel[PRANGE].menu[addr[3]]+1, 8);
	}
	else {
		sprintf(tbuf, "%3d-%4d", ((int)((int)addr[0])<<8) | addr[1], ((int)((int)addr[2])<<8) | addr[3]);
		vPutMenuObj2(PW5, 3, 12, tbuf, 8);
	}
	if(iCheckPrintScadule()) vPutMenuObj2(PW5, 4, 17, " ON", 3);
	else vPutMenuObj2(PW5, 4, 17, "OFF", 3);
	memcpy(t2, addr, 4);

	cur_win=t=n;
	vTWopt2PRNopt(t);
	iPrintf(pop[PW5].x+24+pop[PW5].block[0][0]*8,	pop[PW5].y+pop[PW5].block[0][1]+11, 0, "[%d] ¤å", cur_win);
	for(;;) {
		i=iDialogDriver(PW5, popt_buf);
		switch(i) {
			case -1 :  case  7 :
				addr=tw[cur_win].prn_range;
				memcpy(addr, t2, 4);
				vCloseDlgWindow(PW5);
				return;
			case  0 :
				vPRNopt2TWopt(cur_win);
        i=wGetNumber(cur_win, 1, 5, (BYTE *)"¤å", (BYTE *)"Íe»³Àw¤åÑ¡");
				if(tw[i].active) {
					if(i!=-1) {
						cur_win=i;
						vTWopt2PRNopt(i);
						iPrintf(pop[PW5].x+24+pop[PW5].block[0][0]*8,
										pop[PW5].y+pop[PW5].block[0][1]+11, 0, "[%d] ¤å", cur_win);
						addr=tw[n].prn_range;
						if(addr[0]==0xff && addr[1]==0xff) {
							vPutMenuObj2(PW5, 3, 12, panel[PRANGE].menu[addr[3]]+1, 8);
						}
						else {
							sprintf(tbuf, "%3d-%4d", ((int)((int)addr[0])<<8) | addr[1], ((int)((int)addr[2])<<8) | addr[3]);
							vPutMenuObj2(PW5, 3, 12, tbuf, 8);
						}
					}
				}
				else if(i!=-1) vErrorMsgfDlg(1, 3, 0);
				break;
			case  3 :
				vSetMenuPara(PRANGE, pop[PW5].x+92, pop[PW5].y+36);
				j=iPullDownMenu(PRANGE);
				vRestoreMenuBk(PRANGE);
				if(j==-1) break;
				addr=tw[cur_win].prn_range;
				if(j <5 ) {
					setmem(addr, 2, 0xff);
					addr[2]=0; addr[3]=j-1;
					vPutMenuObj2(PW5, 3, 12, panel[PRANGE].menu[j-1]+1, 8);
				}
				else {
					tmp=wGetNumber(1, 1, addr[9]%1000, "¡e", "¯¡¸b ¡e·e?");
					tmp2=wGetNumber(tmp, tmp, addr[9], "¡e", " a»¡ b ¡e·e?");
					addr[0]=tmp<<8;  addr[1]= tmp&0x00ff;
					addr[2]=tmp2<<8; addr[3]= tmp2&0x00ff;
					sprintf(tbuf, "%3d-%4d", tmp, tmp2);
					vPutMenuObj2(PW5, 3, 12, tbuf, 8);
				}
				break;
			case  4 :
				if(iSetScadule()) {
					vPutMenuObj2(PW5, 4, 17, " ON", 3);
					Scadule=ON;
				}
				else {
					vPutMenuObj2(PW5, 4, 17, "OFF", 3);
					vDelScadule();
				}
				break;
			case  5 :	case  6 :
				vPRNopt2TWopt(cur_win);
				vCloseDlgWindow(PW5);
				vSetPaper();
				vDividePage(1);
				key_flag=ON;
				if(i==5) vPutKey(ALT_I, 0);
				else tw[cur_win].modify=ON;
				return;
		}
	}
}

int iGetPrinterDriver(BYTE *fname)
{
	FILE *fp;
	BYTE buf[80];
	int i, len;

	if((fp=fopen(fname, "rt"))==NULL) return 0;
	for(i=0; i<MAXDRVNAME; i++) {
		fgets(buf, 80, fp);
		if(buf[0]==':') break;
		sscanf(buf, "%s %s %d %d %d", dataPW14[i], drv_name[i], &drv_dns[i], &drv_up[i], &drv_dn[i]);
		len=strlen(dataPW14[i]);
		if(len<12) dataPW14[i][len]=' ';
	}
	fclose(fp);
	return i;
}

void vInitPrintSetup(void)
{
	if(drv_dns[PRN_NUM]==180) {
		is_laser=OFF;
	}
	else is_laser=ON;
	vSetPaper();
}

int iAddPaper(void)
{
	BYTE *str;
	WORD wid, len;

	if(psort<PAPMAX) psort++;
	else {
		vErrorMsgfDlg(0, 0, "10ˆ·¡¬w —w¢Ði ® ´ô¯s“¡”a");
		return 0;
	}
	str=g_dir;
	setmem(str, 80, 0);
	if(iGetStringDlg(1, str, "—w¢Ði ¶w»¡¡w·e(11¸a)?")!=1) return 0;
	str[11]=0;
	wid=wGetNumber(250, 100, 350, "mm", "¹··¡ ì·¡“e(mm”e¶á)?");
	len=wGetNumber(250, 30, 450, "mm", "¹··¡ ‹©·¡“e(mm”e¶á)?");
	papwids[psort-1]=wid;
	paplens[psort-1]=len;
	setmem(dataPW14[PRNMAX+psort-1], 12, ' ');
	strcpy(dataPW14[PRNMAX+psort-1], str);
	dataPW14[PRNMAX+psort-1][strlen(str)]=' ';
	vMoveUpCTW(prn_number, PRNMAX, psort, dataPW14);
	return 1;
}

extern BYTE del_able;

int iDelPaper(int n)
{
	if(ihUserChoiceDlg("¸÷ i »¡¶‰Œa¶a?")!=0) return 0;
	vWipeStringArray(PRNMAX+PAPMAX, PRNMAX+n, dataPW14);
	vMoveUpCTW(prn_number, PRNMAX, psort, dataPW14);
	memmove(papwids+n, papwids+n+1, (psort-6)*2);
	memmove(paplens+n, paplens+n+1, (psort-6)*2);
	psort--;
	return 1;
}

void vPrinterSetupDlg(void)
{
	BYTE p_n[2], process[2], scr[2], buf[10], value[2], set[2];
	int i, flag=0;

	p_n[0]=prn_number;
	p_n[1]=psort;
	setmem(process, 2, 0);
	setmem(scr, 2, 0);
	setmem(buf, 10, 0);
	setmem(value, 2, 0);
	set[0]=PRN_NUM;
	set[1]=paper;
	buf[2]=psupply;
	buf[3]=pport;  // setting printer port

	vMoveUpCTW(prn_number, PRNMAX, psort, dataPW14);
	vMakeExtDlgWindow(PW14, (BYTE *)dataPW14, p_n, process, scr, value, set);
	vOpenDlgWindow(PW14); del_able=1;
	for(;;) {
		i=iDialogDriver(PW14, buf);
		switch(i) {
			case -1 : case 6 :
				goto OUT;
			case  5 :
				PRN_NUM=set[0];
//				for(i=0; i<4; i++) menu_PW5[9+i][0]=drv_opt[PRN_NUM][i];
				// setting printer and paper
				if(drv_dns[PRN_NUM]==180) is_laser=0;
				else is_laser=ON;
				if(paper!=buf[1]) {
					paper=set[1];
					vSetPaper();
					flag=1;
				}
				psupply=buf[2];
				pport=buf[3];
				goto OUT;
			case  4 :
				if(iAddPaper()) {
					p_n[1]=psort;
					vDrawRecordObj(PW14, 1);
				}
				break;
			case  -21 :
				if(value[1]+scr[1]>5) {
					if(iDelPaper(value[1]+scr[1])) {
						p_n[1]--; vDrawRecordObj(PW14, 1);
					}
				}
				else vErrorMsgfDlg(0, 0, "‹¡¥¥¶w»¡“e »¡¶‰® ´ô¯s“¡”a");
				break;
			default : goto OUT;
		}
	}
OUT :
	vCloseDlgWindow(PW14); del_able=0;
	if(flag==1) {
		vSetPaper();
		vDividePage(1);
	}
}

BYTE bPrintStatus(int port)
{
	BYTE cod;
	asm mov ah, 0x02;
	asm mov dx, port;
	asm int 0x17;
	asm mov cod, ah;
	return cod;
}

int iCheckPrinter(int port)
{
	int temp;
	unsigned long i;

	i=Scount;
	while(Scount<i+18) {
		temp=bPrintStatus(port);
		temp &=0xf8;
		temp ^=0x48;
		if((temp & 0x80)) return 1;  // printer exist
	}
	if((temp & 0x08)) {
		if(temp & 0x80) return -2;  // Turn on printer
		else return -1;						  // On line printer
	}
	else return 0;                // printer no exist
}

WORD poption[20]={15, 15, 20, 20, 0, 1, 0xffff, 0, 1, 0, 8*180, 105*18, 0, 0, 0,0,0,0,0};
BYTE pname[80];
BYTE drvname[80]="d:\\est\\21\\24kssm1.drv";
extern BYTE printing;
extern BYTE disp_space, disp_newline, disp_return;

BYTE old_sp, old_nl, old_re, old_fv;
void vSaveDisplayOpt(void)
{
	old_sp=disp_space;
	old_nl=disp_newline;
	old_re=disp_return;
	old_fv=full_vector;
	full_vector=1;
	vSetDispOpt(0, 0, 0);
	printing=ON;
}

void vRestoreDisplayOpt(void)
{
	printing=OFF;
	disp_space=old_sp;
	disp_newline=old_nl;
	disp_return=old_re;
	full_vector=old_fv;
	vSetDispOpt(disp_space, disp_newline, disp_return);
	vSetImageStartLocation(0);
	vSetUnit(16, 16);
	vSetImageHeight(96);
}

int iPrintTextWindow(int n)
{
//	int tbl[4]={50, 100, 150, 200};
	int i, rep;

	vSetPaper();
	vDividePage(1);
	poption[0]=tw[n].up_s;
	poption[1]=tw[n].down_s;
	poption[2]=tw[n].left_s;
	poption[4]=tw[n].word_gap;
	poption[5]=tw[n].repeat;
	if(tw[n].prn_range[0]==0 && tw[n].prn_range[1]=='0') {
		poption[6]=0xffff; poption[7]=0;
	}
	else {
		poption[6]=(((int)tw[n].prn_range[0]) << 8) | tw[n].prn_range[1];
		poption[7]=(((int)tw[n].prn_range[2]) << 8) | tw[n].prn_range[3];
		if(poption[6]==0xffff && poption[7]==1) {
			poption[6]=tw[n].cp; poption[7]=tw[n].cp;
		}
	}
	poption[8]=100;
	poption[9]=pport;
	poption[10]=papwids[PAP_NUM];  // send by 'mm'unit
	poption[11]=paplens[PAP_NUM];
	poption[12]=psupply;
	poption[13]=tw[n].prn_range[5]; // head len
	poption[14]=tw[n].prn_range[6]; // foot len
	poption[15]=tw[n].prn_range[7]; // lgap len

	if(!iInitPrint((int *)poption)) return 0;
	vInitPrintStatusWindow(poption[6], poption[7], n);
	for(rep=0; rep<tw[n].repeat; rep++) {
		i=iPrintFile((NODE *)SNode);
		if(i==0) break;
	}
	vHidePrintStatus();
	return i;
}

void vPrintScaduler(void)
{
	BYTE lot[5], i;
	int oldpn;

	vInitINT60();
	sprintf(drvname, "%s\\%s", exe_path, drv_name[PRN_NUM]);
	vSaveDisplayOpt();
	if(iInitPrinterDriver(drvname)==0) return;
	setmem(lot, 5, 0);
	iFreeFontBuffer(1);
	if(Scadule) {
		if(iCheckPrintScadule()==0) {
			iPrintTextWindow(cur_w); goto OUT;
		}
		for(i=0; i<5; i++) if(Sstack[i]) lot[Sstack[i]-1]=i+1;
		oldpn=START_PAGE_NUM;
		for(i=0; i<5; i++) {
			if(lot[i]) {
				if(cur_w !=lot[i]) {
					vRestoreDisplayOpt();
					vChangeWindow(lot[i]);
					vRestoreEdit();
					vEndKeyTask();
					vSaveDisplayOpt();
				}
				if(!iPrintTextWindow(lot[i])) break;
				START_PAGE_NUM=START_PAGE_NUM+CUR_PAGE_NUM;
			}
		}
		START_PAGE_NUM=oldpn;
	}
	else {
		iPrintTextWindow(cur_w);
	}
OUT:
	vRestoreDisplayOpt();
	vFreeDriver();
	vDeinitINT60();
	vDelScadule();
}

void vPrintStart(void)
{
	BYTE old_saver;

	switch(iCheckPrinter(pport)) {
		case 0 :
			vErrorMsgfDlg(0, 0, "ÏaŸ¥Èáˆa ´ô¯s“¡”a");
			return;
		case 1 :
			break;
		case -1 :
			vErrorMsgfDlg(0, 5, "ÏaŸ¥Èá 'ON LINE' ");
			return;
		case -2 :
			vErrorMsgfDlg(0, 0, "ÏaŸ¥ÈáŸi Åa¯³¯¡µ¡");
			return;
	}
	old_saver=scr_saver_num;
	scr_saver_num=0;
	vPrintScaduler();
	scr_saver_num=old_saver;
	vMCON();
}
