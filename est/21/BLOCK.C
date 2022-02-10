#define _BLOCK
#include <stdio.h>
#include <ctype.h>
#include <alloc.h>
#include <string.h>
#include <dos.h>
#include <mem.h>
#include <dir.h>
#include "est.h"
#include "21.h"
#include "proto.h"
#include "graph.h"
#include "mouse.h"
#include "hidden.h"

#define _LOCSP wTL+=SCODE
#define _LOCWP wTL+=WCODE
#define _LOCSM wTL-=SCODE
#define _LOCWM wTL-=WCODE
#define _PUTCURLINE iPTC(iCX, cur_lnum, CNode)
#define _COL  dc=tw[cur_w].cc
#define _CLEARWIN(x) vBarByte(x, (bminy>>3)+1, bmaxx, (bmaxy>>3), 0)
#define pTB	CNode->line
#define _OUTDBLINK cur_hsize=CNode->hs[cur_lnum-1]
#define bGetLH(lnum, t) (t->hs[lnum-1])
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

extern NODE *SNode, *CNode, *ENode;
extern NODE *top;
extern int top_x;
extern BYTE top_lnum;
extern WORD top_cr;
extern int ex_err;

#define LINEGAB		lc[0]
#define INDENT		lc[1]
#define LEFTR			lc[2]
#define RIGHTR    lc[3]
#define ALINE			lc[4]
#define LNUM			lc[5]

extern BYTE def_lc[];
extern BYTE hfnum;
extern BYTE efnum;
extern BYTE jfnum;
extern BYTE eff;
extern BYTE eff_filter;
extern BYTE sis;
extern BYTE sis_filter;
extern BYTE hc[5];
extern BYTE ec[4];
extern BYTE jc[5];
extern BYTE sc[4];
extern BYTE wc[5];
extern BYTE ic[4];
extern BYTE cur_hsize;
extern BYTE cursor_hsize;
extern BYTE cursor_vsize;

extern BYTE cur_lnum;
extern BYTE key_flag;
extern BYTE skip_flag;
extern BYTE block_task;
extern BYTE set_font_flag;

extern blk sb, eb, setb;

extern int dc;
extern unsigned long int CURSORSPEED;

extern BYTE cur_w;
extern text_window tw[];
extern text_window def_tw;
extern BYTE *our_buf;
extern int graph_mode;
extern popup_window pop[];
extern BYTE full_vector;
extern BYTE current_color;
extern BYTE font_color;
extern BYTE around_color;
extern int MaxX, MaxY;
extern int bminx, bminy, bmaxx, bmaxy;
extern BYTE swp_path[];
extern BYTE font_path[];
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
extern BYTE han_kbd;
extern WORD tot_page;
extern BYTE line_state;
extern BYTE line_drawing;
extern BYTE line_style;

int iMakeNullSwapFile(BYTE *buf)
{
	FILE *fp;

	if((fp = fopen(buf,"wb+")) == NULL) {
		vErrorMsgfDlg(0, 2, buf);
		return 0;
	}
	sprintf(g_dir, "%s%s", the21, swp_data);
	fwrite(g_dir, 48, 1, fp);
	setmem(g_dir, 20, 0);
	fwrite(g_dir, 20, 1, fp);
	fwrite(our_buf, 40, 1, fp);

 	fputc(0, fp);
	putw(1, fp);
	fwrite(def_lc, 1, 6, fp);
	fputc(0x10, fp);
	fputc('\r', fp);

  fputc(0, fp);
	putw(0, fp);
	tw[0].total=1;
	fwrite(&(tw[0].x), sizeof(text_window)-80, 1, fp);
	putw(0, fp);
	putw(0, fp);
	fclose(fp);
	return 1;
}

static void near vWriteNode(NODE *temp, FILE *fp)
{
	fputc(temp->kd, fp);
	putw(temp->bn, fp);
	fwrite(temp->lc, 1, 6, fp);
	fwrite(temp->hs, 1, temp->lc[5], fp);
	fwrite(temp->line, 1, temp->bn, fp);
}

int iWriteLineBlock(FILE *fp, WORD *total)
{
	NODE *temp;

	if(sb.blk==eb.blk) {
		temp=npGetFullNode(eb.loc-sb.loc+1, 1);
		if(!temp) return 0;
		_CHECKNODE(sb.blk);
		if(ex_err==0) {
			vFreeNode(temp);
			vErrorMsgfDlg(1, 0, 0);
			return 0;
		}
		temp->bn=eb.loc-sb.loc+1;
		temp->kd=sb.blk->kd;
		memcpy(temp->lc, sb.blk->lc, 6);
		memcpy(temp->line, sb.blk->line+sb.loc, temp->bn);
		temp->lc[5]=1;
		temp->line[temp->bn-1]='\r';
		vAnalText(temp);
		(*total)+=temp->lc[5];
		vWriteNode(temp, fp);
		vFreeNode(temp);
	}
	else {
		temp=npGetFullNode(sb.blk->bn-sb.loc, 1);
		if(!temp) return 0;
		temp->bn=sb.blk->bn-sb.loc;
		_CHECKNODE(sb.blk);
		if(ex_err==0) {
			vFreeNode(temp);
			vErrorMsgfDlg(1, 0, 0);
			return 0;
		}
		temp->kd=sb.blk->kd;
		memcpy(temp->lc, sb.blk->lc, 6);
		memcpy(temp->line, sb.blk->line+sb.loc, temp->bn);
		temp->lc[5]=1;
		vAnalText(temp);
		(*total)+=temp->lc[5];
		vWriteNode(temp, fp);
		vFreeNode(temp);
		temp=sb.blk;
		while(temp->next!=eb.blk) {
			temp=temp->next;
			_CHECKNODE(temp);
			if(ex_err==0) {
				vErrorMsgfDlg(1, 0, 0);
				return 0;
			}
			(*total)+=temp->lc[5];
			vWriteNode(temp, fp);
		}
		if(eb.loc) {
			temp=npGetFullNode(eb.loc+1, 1);
			if(!temp) return 0;
			temp->bn=eb.loc+1;
			_CHECKNODE(eb.blk);
			if(ex_err==0) {
				vFreeNode(temp);
				vErrorMsgfDlg(1, 0, 0);
				return 0;
			}
			temp->kd=eb.blk->kd;
			memcpy(temp->lc, eb.blk->lc, 6);
			memcpy(temp->line, eb.blk->line, temp->bn);
			temp->line[eb.loc]='\r';
			temp->lc[5]=1;
			vAnalText(temp);
			(*total)+=temp->lc[5];
			vWriteNode(temp, fp);
			vFreeNode(temp);
		}
	}
	return 1;
}

static int near iCol2Loc(int col, NODE *n)
{
	BYTE *str;
	int c;
	WORD loc=0;

	c=(int)((char)n->lc[1])+n->lc[2];
	str=n->line;
	while(c<col) {
		if(str[loc]>127) {
			if(loc && str[loc+2] > 127) break;
			c+=(str[loc+3] & 0x0f)<<1;
			loc+=5;
		}
		else {
			if(str[loc]==0) {
				loc++;
				c++;
			}
			else if(str[loc]=='\r') break;
			else {
				if(loc && str[loc+1] > 127) break;
				c+=str[loc+2] & 0x0f;
				loc+=4;
			}
		}
	}
	return loc;
}

static int near iWriteColumeBlock(FILE *fp, WORD *total)
{
	NODE *temp, *n;
	int sc, ec;
	WORD sloc, eloc, bn;

	n=sb.blk;
	if(wLoc2Line(sb.loc, n->line)>1) {
		if(n==eb.blk)	return 0;
		n=n->next;
	}
	if(sb.c<eb.c) {
		sc=sb.c;
		ec=eb.c;
	}
	else {
		sc=eb.c;
		ec=sb.c;
	}
	while(eb.blk!=n->prev) {
		sloc=iCol2Loc(sc, n);
		eloc=iCol2Loc(ec, n);
		bn=eloc-sloc+1;
		temp=npGetFullNode(bn, 1);
		if(!temp) return 0;
		temp->bn=bn;
		_CHECKNODE(n);
		if(ex_err==0) {
			vFreeNode(temp);
			vErrorMsgfDlg(1, 0, 0);
			return 0;
		}
		temp->kd=n->kd;
		memcpy(temp->lc, n->lc, 6);
		memcpy(temp->line, n->line+sloc, temp->bn);
		temp->line[temp->bn-1]='\r';
		temp->lc[5]=1;
		vAnalText(temp);
		(*total)++;
		vWriteNode(temp, fp);
		vFreeNode(temp);
		n=n->next;
		if(n==0) break;
	}
	return 1;
}

static BYTE old_bfname[80]={"*.GWP"};

int iWriteBlockText(BYTE swp_or_save)
{
	FILE *fp;
	BYTE fname[80];
	WORD total=0;
	BYTE tmodify;
	BYTE result;

	tmodify=tw[cur_w].modify;
	if((eb.blk==sb.blk && eb.loc==sb.loc) || !block_task) {
		vErrorMsgfDlg(10, 3, 0);
		return 0;
	}
	if(swp_or_save) {
		if(cur_w==0) {
			vErrorMsgfDlg(10, 5, 0);
			return 0;
		}
		sprintf(g_dir, "%s\\editswp0.swp", swp_path);
		if((fp = fopen(g_dir,"wb+")) == NULL) {
			vErrorMsgfDlg(0, 2, "¯aµÓ");
			block_task=OFF;
			return 0;
		}
		sprintf(g_dir, "%s%s", the21, swp_data);
		fwrite(g_dir, 48, 1, fp);
		setmem(g_dir, 40, 0);
		fwrite(g_dir, 20, 1, fp);
		fwrite(g_dir, 40, 1, fp);
		if(block_task==ON) result=iWriteLineBlock(fp, &total);
		else result=iWriteColumeBlock(fp, &total);
		if(result) {
			fputc(0, fp);
			if(eb.loc) putw(0xffff, fp);
			else putw(0, fp);
			tw[0].total=total;
			fwrite(&(tw[0].x), sizeof(text_window)-80, 1, fp);
			putw(0, fp);
			putw(0, fp);
		}
		else vErrorMsgfDlg(1, 0, 0);
		fclose(fp);
	}
	else {
		vSetExt(".GWP");
		if(iFileListDlg(old_bfname, fname, 3)==1) {
			if(iIsOverWrite(fname)==0) return 0;
			if((fp = fopen(fname,"wb+")) == NULL) {
				vErrorMsgfDlg(0, 1, fname);
				block_task=OFF;
				return 0;
			}
			sprintf(g_dir, "%s%s", the21, gwp_data);
			fwrite(g_dir, 48, 1, fp);
			setmem(g_dir, 40, 0);
			fwrite(g_dir, 20, 1, fp);
			fwrite(g_dir, 40, 1, fp);
			if(block_task==ON) result=iWriteLineBlock(fp, &total);
			else result=iWriteColumeBlock(fp, &total);
			if(result) {
				def_tw.total=total;
				fputc(0, fp);
				putw(0, fp);
				fwrite(&(def_tw), sizeof(text_window)-80, 1, fp);
				def_tw.total=1;
				putw(0, fp);
				putw(0, fp);
			}
			else vErrorMsgfDlg(1, 0, 0);
			fclose(fp);
			vEraseOldFileName();
		}
	}
	tw[cur_w].modify=tmodify;
	key_flag=ON;
	block_task=OFF;
	return 1;
}

static int near iReadInsertNode(FILE *fp, NODE *over_node)
{
	BYTE lc[6];
	WORD len;
	BYTE kd;
	BYTE *str;
	BYTE *ths;

	kd=fgetc(fp);
	len=getw(fp);
	if(!len) return 0;
	if(len==0xffff) return 0xff;
	fread(lc, 6, 1, fp);
	str=_GETLINE(len);
	if(!str) {
		vErrorMsgfDlg(1, 0, 0);
		return 0;
	}
	ths=_GETHS(lc[5]);
	if(!ths) {
		free(str);
		vErrorMsgfDlg(1, 0, 0);
		return 0;
	}
	fread(ths, lc[5], 1, fp);
	fread(str, len, 1, fp);
	if(!iInsertNode(over_node, str, lc, ths, len, kd)) {
		vErrorMsgfDlg(1, 0, 0);
		return 0;
	}
	return 1;
}

void vInsertBlock(BYTE flag, BYTE *fname)
{
	FILE *fp;
	BYTE result;
	NODE *temp;

	if((fp = fopen(fname,"rb")) == NULL) {
		vErrorMsgfDlg(0, 1, fname);
		key_flag=ON;
		return;
	}
	if(!iCheckGWP(1, fp, gwp_data)) {
		fclose(fp);
		vErrorMsgfDlg(0, 7, "GWP 1.0");
		return;
	}
	temp=CNode;
	while(1) {
		result=iReadInsertNode(fp, temp);
		temp=temp->next;
		if(!result || result==0xff) break;
		if(flag) vAnalText(temp);
	}
	tw[cur_w].modify=ON;
	fclose(fp);
}

void vReadBlock(BYTE paste_or_read)
{
	FILE *fp;
	NODE *temp, *in_node;
	BYTE fname[80];
	BYTE result=0;
	BYTE type, code, way, kd;
	int in_col, in_loc;
	BYTE lc[6];
	WORD len;
	BYTE *str;

	if(block_task) {
		vErrorMsgfDlg(10, 4, 0);
		return;
	}
	if(paste_or_read) {
		if(cur_w==0) {
			vErrorMsgfDlg(10, 5, 0);
			return;
		}
		sprintf(g_dir, "%s\\editswp0.swp", swp_path);
		if((fp = fopen(g_dir,"rb")) == NULL) return;
		if(!iCheckGWP(1, fp, swp_data)) {
			fclose(fp);
			vErrorMsgfDlg(0, 1, "¯aµÓ");
			return;
		}
		if(paste_or_read==1) {
			vResetSpace(CNode);
			if(!iSplitNode()) {
				vErrorMsgfDlg(1, 0, 0);
				fclose(fp);
				return;
			}
			vAnalText(CNode);
			vAnalText(CNode->next);
			temp=CNode;
			do {
				result=iReadInsertNode(fp, temp);
				temp=temp->next;
			} while(result!=0 && result!=0xff);
			if(result==0xff) {
				temp=temp->prev;
				iMergeNode(temp);
				vAnalText(temp);
			}
			iMergeNode(CNode);
			vAnalText(CNode);
		}
		else {
			temp=CNode;
			in_col=tw[cur_w].cc;
			while(1) {
				kd=fgetc(fp);
				len=getw(fp);
				if(!len) break;
				if(len==0xffff) break;
				fread(lc, 6, 1, fp);
				while(1) {
					fgetc(fp);
					lc[5]--;
					if(lc[5]==0) break;
				}

				if(!temp) {
					if((in_node=npGetFullNode(1, 1))==0) {
ERR:
						vErrorMsgfDlg(1, 0, 0);
						fclose(fp);
						return;
					}
					in_node->bn=1;
					in_node->kd=kd;
					memcpy(in_node->lc, lc, 6);
					in_node->lc[5]=1;
					in_node->hs[0]=0x10;
					in_node->line[0]='\r';
					ENode->next=in_node;
					in_node->prev=ENode;
					in_node->next=0;
					temp=ENode=in_node;
				}
				in_loc=iCol2Loc(in_col, temp);
				if(temp->line[in_loc]=='\r') {
					iInsertSpaceChar(in_col-iLoc2Col(in_loc, temp), in_loc, temp);
					in_loc=temp->bn-1;
				}

				str=_REGETLINE(temp->bn+len-1, temp->line);
				if(!str) goto ERR;
				temp->line=str;
				movmem(temp->line+in_loc, temp->line+in_loc+len-1, temp->bn-in_loc);
				fread(temp->line+in_loc, len-1, 1, fp);
				fgetc(fp);
				temp->bn+=len-1;
				vAnalText(temp);
				temp=temp->next;
			}
		}
		fclose(fp);
	}
	else {
		vSetExt(".GWP");
		if(iFileListDlg(old_bfname, fname, 2)==1) {
			type=iGetFileType(&code, &way, fname);
			switch(type) {
				case 0: iReadTXT(0, code, way, fname); break;
				case 1: case 2: iReadHWP(0, fname); break;
				case 3: vInsertBlock(0, fname); break;
			}
		}
	}

	if(!key_flag) key_flag=TON;
	vDispColumeStatus();
	vDispRowStatus();
	return;
}

void vClearBlock(void)
{
	NODE *temp;
	WORD total;
	int sc, ec;
	WORD sloc, eloc;

	if(eb.blk==sb.blk && eb.loc==sb.loc) return;
	if(!block_task) return;
	total=tw[cur_w].total;
	if(block_task==ON) {
		block_task=OFF;
		if(sb.blk==eb.blk) {
			temp=sb.blk;
			_CHECKNODE(temp);
			if(ex_err==0) vErrorMsgfDlg(1, 0, 0);
			else {
				movmem(temp->line+eb.loc, temp->line+sb.loc, temp->bn-eb.loc);
				temp->bn-=(eb.loc-sb.loc);
				temp->line=_REGETLINE(temp->bn, temp->line);
				vAnalText(temp);
				CNode=sb.blk;
				wTL=sb.loc;
			}
		}
		else {
			temp=sb.blk;
			_CHECKNODE(temp);
			if(ex_err==0) vErrorMsgfDlg(1, 0, 0);
			else {
				temp->line[sb.loc]='\r';
				temp->bn=sb.loc+1;
				temp->line=_REGETLINE(temp->bn, temp->line);
				vAnalText(temp);
				temp=temp->next;
				vAnalText(temp);
				while(temp!=eb.blk) {
					temp=temp->next;
					vDeleteNode(temp->prev);
				}
				if(eb.loc) {
					_CHECKNODE(temp);
					if(ex_err==0) vErrorMsgfDlg(1, 0, 0);
					else {
						movmem(temp->line+eb.loc, temp->line, temp->bn-eb.loc);
						temp->bn-=eb.loc;
						temp->line=_REGETLINE(temp->bn, temp->line);
						vAnalText(temp);
					}
				}
				CNode=sb.blk;
				wTL=sb.loc;
				iMergeNode(CNode);
				vAnalText(CNode);
			}
		}
	}
	else {
		block_task=OFF;
		temp=sb.blk;
		_CHECKNODE(temp);
		if(ex_err==0) vErrorMsgfDlg(1, 0, 0);
		else {
			if(wLoc2Line(sb.loc, temp->line)>1) {
				if(temp==eb.blk) return;
				temp=temp->next;
			}
			if(sb.c<eb.c) {
				sc=sb.c;
				ec=eb.c;
			}
			else {
				sc=eb.c;
				ec=sb.c;
			}
			while(eb.blk!=temp->prev) {
				_CHECKNODE(temp);
				if(ex_err==0) {
					vErrorMsgfDlg(1, 0, 0);
					break;
				}
				sloc=iCol2Loc(sc, temp);
				eloc=iCol2Loc(ec, temp);
				movmem(temp->line+eloc, temp->line+sloc, temp->bn-eloc);
				temp->bn-=eloc-sloc;
				temp->line=_REGETLINE(temp->bn, temp->line);
				vAnalText(temp);
				temp=temp->next;
				if(temp==0) break;
			}
			wTL=0;
			cur_lnum=1;
		}
	}
	if(total>tw[cur_w].total) {
		tw[cur_w].cr-=(total-tw[cur_w].total);
		cur_lnum=wLoc2Line(wTL, pTB);
		vSetTopVar();
	}

	if(!key_flag) key_flag=TON;
	vDispColumeStatus();
	vDispRowStatus();
}

void vPasteBlock(void)
{
	BYTE t;

	t=block_task;
	if(!iWriteBlockText(1)) return;
	block_task=t;
	vClearBlock();
}

void vDelLineChar(BYTE flag)
{
	WORD sloc;
	WORD eloc;
	WORD width;
	NODE *tn;
	BYTE block_task_mode;

	if(!block_task) return;
	block_task_mode=block_task;
	block_task=OFF;
	sloc=sb.loc;
	tn=sb.blk;
	while(1) {
		if(block_task_mode==ON) {
			if(tn==eb.blk) {
				if(eb.loc==0) break;
				eloc=eb.loc;
			}
			else {
				eloc=tn->bn;
			}
		}
		else {
			if(sb.c>eb.c) {
				sloc=iCol2Loc(eb.c, tn);
				eloc=iCol2Loc(sb.c, tn);
			}
			else {
				sloc=iCol2Loc(sb.c, tn);
				eloc=iCol2Loc(eb.c, tn);
			}
		}
		_CHECKNODE(tn);
		if(ex_err==0) {
			vErrorMsgfDlg(1, 0, 0);
			break;
		}
		while(sloc<eloc) {
			if(tn->line[sloc]>127) {
				if(flag) {
					width=(tn->line[sloc+3] & 0x0f)<<1;
					iDelBytes(width, sloc, tn);
					iInsertSpaceChar(width, sloc, tn);
					sloc+=width*4;
					eloc+=width*4-5;
				}
				else sloc+=5;
			}
			else {
				if(tn->line[sloc]==0) {
					sloc++;
					continue;
				}
				if(tn->line[sloc]=='\r') break;
				if(tn->line[sloc+3]==0x23) {
					if(!flag) goto DELLINE;
				}
				else {
					if(flag) {
DELLINE:
						tn->line[sloc]=32;
						tn->line[sloc+3]=0x40;
					}
				}
				sloc+=4;
			}
		}
		vAnalText(tn);
		if(sb.blk==eb.blk) break;
		if(tn==eb.blk) break;
		sloc=0;
		tn=tn->next;
	}
	wTL=0;
	cur_lnum=1;
}

extern int maxjhan;

void vSetFontKind(void)
{
	WORD ttot=0;
	WORD sloc;
	WORD eloc;
	NODE *tn;
	BYTE block_task_mode;

	if(!block_task) {
		cursor_hsize=sis>>4;
		cursor_vsize=sis & 0x0f;
		if(hfnum) hc[4]= 0xe0 | (hfnum-1);

		if(efnum) ec[3]= 0x40 | (efnum-1);
		if(jfnum) jc[4]= 0xd0 | (jfnum-1);
		set_font_flag=ON;
	}
	else {
		block_task_mode=block_task;
		block_task=OFF;
		if(CNode==eb.blk && wTL==eb.loc) ttot=tw[cur_w].total;
		sloc=sb.loc;
		tn=sb.blk;
		while(1) {
			if(block_task_mode==ON) {
				if(tn==eb.blk) {
					if(eb.loc==0) break;
					eloc=eb.loc;
				}
				else {
					eloc=tn->bn;
				}
			}
			else {
				if(sb.c>eb.c) {
					sloc=iCol2Loc(eb.c, tn);
					eloc=iCol2Loc(sb.c, tn);
				}
				else {
					sloc=iCol2Loc(sb.c, tn);
					eloc=iCol2Loc(eb.c, tn);
				}
			}
			_CHECKNODE(tn);
			if(ex_err==0) {
				vErrorMsgfDlg(1, 0, 0);
				break;
			}
			while(sloc<eloc) {
				if(tn->line[sloc]>127) {
					if((tn->line[sloc+4] & 0xe0)==0xe0) { // HANGUL
					if(hfnum) tn->line[sloc+4]=0xe0 | (hfnum-1);

					}
					else if((tn->line[sloc+4] & 0xd0)==0xd0) { //JAPAN
						if(jfnum) tn->line[sloc+4]=0xd0 | (jfnum-1);
					}
					if(sis_filter & 2) {
						tn->line[sloc+3]&=0x0f;
						tn->line[sloc+3]|=(sis & 0xf0);
					}
					if(sis_filter & 1) {
						tn->line[sloc+3]&=0xf0;
						tn->line[sloc+3]|=(sis & 0x0f);
					}
					tn->line[sloc+2]|=eff;
					tn->line[sloc+2]&=eff_filter;
					sloc+=5;
				}
				else {
					if(tn->line[sloc]==0) {
						sloc++;
						continue;
					}
					if(tn->line[sloc]=='\r') break;
					if((tn->line[sloc+3] & 0x48)==0x40) {
						if(efnum) tn->line[sloc+3] = 0x40 | (efnum-1);
					}
					if(sis_filter & 2) {
						tn->line[sloc+2]&=0x0f;
						tn->line[sloc+2]|=(sis & 0xf0);
					}
					if(sis_filter & 1) {
						tn->line[sloc+2]&=0xf0;
						tn->line[sloc+2]|=(sis & 0x0f);
					}
					tn->line[sloc+1]|=eff;
					tn->line[sloc+1]&=eff_filter;
					sloc+=4;
				}
			}
			vAnalText(tn);
			if(sb.blk==eb.blk) break;
			if(tn==eb.blk) break;
			sloc=0;
			tn=tn->next;
		}
		if(ttot) {
			if(ttot>tw[cur_w].total) tw[cur_w].cr-=(ttot-tw[cur_w].total);
			else tw[cur_w].cr+=(tw[cur_w].total-ttot);
			cur_lnum=wLoc2Line(wTL, pTB);
		}

		key_flag=ON;
	}
	vDispColumeStatus();
	vDispRowStatus();
}

extern BYTE *H_font[];
extern BYTE *E_font[];
extern BYTE *J_font[];
void vSetFontEffect(void)
{
	int i, j, code;
	BYTE temp[18];

	if(block_task==0) {
		strcpy(pop[PW3].headline,"‹i¸a¡¡´· ¬é¸÷ ");
		for(i=0;i<7;i++) temp[i]=(eff>>i) & 1;
		temp[7]=(sis & 0x0f)-1;
		temp[8]=(sis >> 4)-1;
		temp[9]=temp[10]=temp[11]=0;
		_CHECKNODE(CNode);
		if(ex_err==0) {
			vErrorMsgfDlg(1, 0, 0);
			return;
		}
		if(pTB[wTL]>127) {

			if((pTB[wTL+4] & 0xe0)==0xe0) {
				temp[9]=(pTB[wTL+4] & 0x1f)+1;  //bCombine2 hangul
			}
			else if((pTB[wTL+4] & 0xd0)==0xd0) { // japan
				temp[11]=(pTB[wTL+4] & 0x03)+1;
			}
		}
		else {
			if(pTB[wTL]=='\r') {
				if(wTL==0) {
					temp[9]=temp[10]=temp[11]=0;
				}
				else if(pTB[wTL-1]>127) {
					if((pTB[wTL-1] & 0xe0)==0xe0) {
						temp[9]=(pTB[wTL-1] & 0x1f)+1;  //hangul
					}
					else if((pTB[wTL-1] & 0xd0)==0xd0) {
						temp[11]=(pTB[wTL-1] & 0x03)+1;  //hangul
					}
				}
				else {
					if((pTB[wTL-1] & 0x48)==0x40) {
						temp[10]=(pTB[wTL-1] & 0x0f)+1;
					}
				}
			}
			else {
				if((pTB[wTL+3] & 0x48)==0x40) {
					temp[10]=(pTB[wTL+3] & 0x0f)+1;
				}
			}
		}
		if(temp[9]>maxjhan) temp[9]-=16-maxjhan;
	}
	else if(block_task) {
		strcpy(pop[PW3].headline,"‹i¸a¡¡´· ŠaÁA");
		setmem(temp, 7, 2);
		temp[7]=10;
		temp[8]=10;
		setmem(temp+9, 3, 0);
	}
	memcpy(pop[PW3].data[19]+12, H_font[temp[9]]+1, 6);
	memcpy(pop[PW3].data[20]+12, E_font[temp[10]]+1, 6);
	memcpy(pop[PW3].data[21]+12, J_font[temp[11]]+1, 6);
	vOpenDlgWindow(PW3);
	for(;;) {
		i=iDialogDriver(PW3, (BYTE *)temp);
		for(j=0;j<3; j++) {
			vSetMenuPara(EXTPAN1+j, pop[PW3].x+100+40+j*24, pop[PW3].y+20+2);
		}
		switch(i) {
			case -1 :
				vCloseDlgWindow(PW3);
				return;
			case  3 :
				code=iPullDownMenu(EXTPAN1);
				vRestoreMenuBk(EXTPAN1);
				if(code==-1) break;
				else vPutMenuObj(PW3, 3, 12, H_font[code-1]+1);
				temp[9]=code-1;
				break;
			case  4 :
				code=iPullDownMenu(EXTPAN2);
				vRestoreMenuBk(EXTPAN2);
				if(code==-1) break;
				else vPutMenuObj(PW3, 4, 12, E_font[code-1]+1);
				temp[10]=code-1;
				break;
			case  5 :
				code=iPullDownMenu(EXTPAN3);
				vRestoreMenuBk(EXTPAN3);
				if(code==-1) break;
				else vPutMenuObj(PW3, 5,  12, J_font[code-1]+1);
				temp[11]=code-1;
				break;
			case  6 : break;  // Hanja
			case  7 :
				setmem(temp, 12, 0);
				vPutMenuObj(PW3, 3, 12, H_font[temp[9]]+1);
				vPutMenuObj(PW3, 4, 12, E_font[temp[10]]+1);
				vPutMenuObj(PW3, 5, 12, J_font[temp[11]]+1);
				break;
			case  8 :
				eff&=0x80;
				for(i=0;i<7;i++) {
					if(temp[i]==1) eff|=1<<i;
				}
				eff_filter=0xff;
				for(i=0;i<7;i++) {
					if(temp[i]==0) eff_filter&=~(1<<i);
				}
				sis_filter=3;
				if(temp[7]==10) {
					sis_filter^=1;
					temp[7]=0;
				}
				else temp[7]++;
				if(temp[8]==10) {
					sis_filter^=2;
					temp[8]=0;
				}
				else temp[8]++;
				sis=temp[7] | (temp[8]<<4);
				if(temp[9]>maxjhan) temp[9]+=16-maxjhan;
				hfnum=temp[9];
				efnum=temp[10];
				jfnum=temp[11];
				vSetFontKind();
				vCloseDlgWindow(PW3);
				return;
			case  9 :
				vCloseDlgWindow(PW3);
				return;
		}
	}
}

void vHanja2Hangul(void)
{
	WORD sloc;
	WORD eloc;
	NODE *tn;

	if(block_task==OFF) return;
	block_task=OFF;
	sloc=sb.loc;
	tn=sb.blk;
	while(1) {
		if(tn==eb.blk) {
			if(eb.loc==0) break;
			eloc=eb.loc;
		}
		else {
			eloc=tn->bn;
		}
		_CHECKNODE(tn);
		if(ex_err==0) {
			vErrorMsgfDlg(1, 0, 0);
			break;
		}
		while(sloc<eloc) {
			if(tn->line[sloc]>127) {
				if((tn->line[sloc+4] & 0x40)==0) tn->line[sloc+4]=0xe0;
				sloc+=5;
			}
			else {
				if(tn->line[sloc]==0) sloc++;
				else if(tn->line[sloc]=='\r') break;
				else sloc+=4;
			}
		}
		if(sb.blk==eb.blk) break;
		if(tn==eb.blk) break;
		sloc=0;
		tn=tn->next;
	}
	key_flag=ON;
	vDispColumeStatus();
	vDispRowStatus();
}

void vSetParagraph(int *temp, BYTE *fil)
{
	NODE *tn;
	WORD ttot=0;
	int i;

	if(CNode==eb.blk && wTL==eb.loc) ttot=tw[cur_w].total;
	if(block_task) {
		block_task=OFF;
		tn=sb.blk->prev;
		do {
			tn=tn->next;
			_CHECKNODE(tn);
			if(ex_err==0) {
				vErrorMsgfDlg(1, 0, 0);
				break;
			}
			if(fil[0]) tn->lc[0]=(BYTE)temp[0];
			if(tn->kd==0) {
				if(fil[2]) {
					if(fil[1]) {
						if(temp[1]<tn->lc[3]-10 && temp[1]> -temp[2])
							tn->lc[1]=(BYTE)temp[1];
					}
				}
				else {
					if(fil[1]) {
						if(temp[1]<tn->lc[3]-10 && temp[1]> -((int)tn->lc[2]))
							tn->lc[1]=(BYTE)temp[1];
					}
				}
			}
			if(fil[2]) {
				if(temp[2]<tn->lc[3]-9 && temp[2]+(int)(char)tn->lc[1]<tn->lc[3]-9)
					tn->lc[2]=(BYTE)temp[2];
			}
			if(fil[3]) {
				if(temp[3]>tn->lc[2]+9 && temp[3]>(int)(char)tn->lc[1]+9)
					tn->lc[3]=(BYTE)temp[3];
			}
			if(temp[4]!=10) tn->lc[4]=temp[4];
			vAnalText(tn);
		} while(tn!=eb.blk);
	}
	else {
		if(ex_err==0) vErrorMsgfDlg(1, 0, 0);
		else {
			_CHECKNODE(CNode);
			for(i=0;i<5;i++) {
				if(i==1 && CNode->kd!=0) continue;
				CNode->lc[i]=(BYTE)temp[i];
			}
			vAnalText(CNode);
		}
	}
	if(ttot) {
		if(ttot>tw[cur_w].total) tw[cur_w].cr-=(ttot-tw[cur_w].total);
		else tw[cur_w].cr+=(tw[cur_w].total-ttot);
		cur_lnum=wLoc2Line(wTL, pTB);
	}

	key_flag=ON;
	vPutRod(cur_w, 1);
	vDispColumeStatus();
	vDispRowStatus();
}

BYTE align_buf[70];
BYTE filter[4];

void vParagraphDlg(void)
{
	WORD i;
	int temp[5];
	int vHomeKey[4]={0,0,1,30};
	int vExit[4]={20,40,20,255};

	if(block_task==0) strcpy(pop[PW7].headline,"¢…”eÑw¯¢ ¬é¸÷");
	else  strcpy(pop[PW7].headline,"¢…”eÑw¯¢ ŠaÁA");

	setmem(align_buf, 70, 0);
	setmem(filter, 4, 0);
	for(i=0; i<4; i++) temp[i]=CNode->lc[i];
	temp[1]=(int)((char)CNode->lc[1]);

	if(block_task) align_buf[60]=10;
	else align_buf[60]=CNode->lc[4];

	for(i=0; i<4; i++) {
		vInt2Char(vHomeKey[i], align_buf+0+i*15);
		vInt2Char(temp[i],  align_buf+2+i*15);
		vInt2Char(vExit[i], align_buf+4+i*15);
	}
	vOpenDlgWindow(PW7);
	vMCOFF();
	switch(iDialogDriver(PW7, align_buf)) {
		case 2 :
			for(i=0; i<4; i++) temp[i]=*(align_buf+2+i*15);
			temp[1]=(int)((char)(*(align_buf+2+1*15)));
			temp[4]=align_buf[60];
			vSetParagraph(temp, filter);
			break;
		case 3 :  case -1 :
			break;
	}
	vCloseDlgWindow(PW7);
	return;
}

void vCtrlKKey(BYTE mode)
{
	if(!block_task) {
		block_task=mode;
		vDispExtraStatus((mode-1)*7);
		eb.y=sb.y=setb.y=iCY;
		eb.c=sb.c=setb.c=tw[cur_w].cc;
		eb.row=sb.row=setb.row=tw[cur_w].cr;
		eb.loc=sb.loc=setb.loc=wTL;
		eb.blk=sb.blk=setb.blk=CNode;
	}
	else {
		block_task=OFF;
		key_flag=ON;
	}
}

typedef struct {
	WORD loc;
	NODE *blk;
} found_blk;

found_blk found;
found_blk sfb;
found_blk efb;

BYTE case_sen=ON;
BYTE whole=OFF;
BYTE answer=ON;
BYTE blk_or_not=0; // 0 is all text, 1 is block, 2 CNode page
BYTE up_down=1;
BYTE st_pos=ON; //if OFF is CNode position
BYTE last_task=0;

BYTE search_buf[90]={""};
BYTE find_flag=0;

static int near iCompString(WORD tloc, BYTE *buf, BYTE *str)
{
	WORD mach=0;
	WORD ttloc=tloc;

	while(str[mach]) {
		while(buf[tloc]==0) {
			if(buf[tloc]=='\r') return 0;
			(tloc)++;
		}
		if(str[mach]>127) {
			if(buf[tloc]>127) {
				if(str[mach]==buf[tloc] && str[mach+1]==buf[(tloc)+1]) {
					mach+=2;
					(tloc)+=5;
				}
				else return 0;
			}
			else return 0;
		}
		else {
			if(buf[tloc]<128) {
				if(buf[tloc]=='\r') return 0;
				if(!case_sen) {
					if(toupper(str[mach])==toupper(buf[tloc])) {
						mach++;
						(tloc)+=4;
					}
				}
				else if(str[mach]==buf[tloc]) {
					mach++;
					(tloc)+=4;
				}
				else return 0;
			}
			else return 0;
		}
	}
	if(whole) {
		if(ttloc==0 || (buf[ttloc-1]<128 && buf[ttloc-4]==32)) {
			if(buf[tloc]==32 || buf[tloc]=='\r') return 1;
		}
		return 0;
	}
	return 1;
}

WORD wFindString(WORD loc, WORD eloc, BYTE *buf, BYTE *str)
{
	while(buf[loc]!='\r' && loc<eloc) {
		if(buf[loc]==0) {
			loc++;
			continue;
		}
		if(buf[loc]>127) {
			if(str[0]>127) {
				if(str[0]==buf[loc] && str[1]==buf[loc+1]) {
					if(iCompString(loc, buf, str)) return loc;
				}
			}
			loc+=5;
		}
		else {
			if(str[0]<127) {
				if(!case_sen) {
					if(toupper(str[0])==toupper(buf[loc])) {
						if(iCompString(loc, buf, str)) return loc;
					}
				}
				else if(str[0]==buf[loc]) {
					if(iCompString(loc, buf, str)) return loc;
				}
			}
			loc+=4;
		}
	}
	return 0xffff;
}

WORD wFindStringBack(WORD loc, WORD eloc, BYTE *buf, BYTE *str)
{
	while(loc>eloc) {
		if(buf[loc-1]==0) {
			loc--;
			continue;
		}
		if(buf[loc-1]>127) {
			loc-=5;
			if(str[0]>127) {
				if(str[0]==buf[loc] && str[1]==buf[loc+1]) {
					if(iCompString(loc, buf, str)) return loc;
				}
			}
		}
		else {
			loc-=4;
			if(str[0]<127) {
				if(!case_sen) {
					if(toupper(str[0])==toupper(buf[loc])) {
						if(iCompString(loc, buf, str)) return loc;
					}
				}
				else if(str[0]==buf[loc]) {
					if(iCompString(loc, buf, str)) return loc;
				}
			}
		}
	}
	return 0xffff;
}

// 0 ~ 79 iGetString  80  Distinguish
// 81  Range          82  Direction
// 83  Start          84  O.k       85 Cancel

void vSearchString(WORD sloc, NODE *temp)
{
	WORD eloc;

	if(up_down) {
		while(temp) {
			if(temp==efb.blk) eloc=efb.loc;
			else eloc=temp->bn-1;
			_CHECKNODE(temp);
			if(ex_err==0) {
				vErrorMsgfDlg(1, 0, 0);
				break;
			}
			found.loc=wFindString(sloc, eloc, temp->line, search_buf);
			if(found.loc!=0xffff) {
				if(found.loc==wTL && temp==CNode) {
					if(temp->line[sloc]>127) sloc+=5;
					else sloc+=4;
					continue;
				}
				break;
			}
			if(temp==efb.blk) break;
			temp=temp->next;
			sloc=0;
		}
	}
	else {
		while(temp->prev) {
			_CHECKNODE(temp);
			if(ex_err==0) {
				vErrorMsgfDlg(1, 0, 0);
				break;
			}
			if(temp==sfb.blk) eloc=sfb.loc;
			else eloc=0;
			found.loc=wFindStringBack(sloc, eloc, temp->line, search_buf);
			if(found.loc!=0xffff) break;
			if(temp==sfb.blk) break;
			temp=temp->prev;
			sloc=temp->bn-1;
		}
	}
	if(found.loc!=0xffff) {
		wTL=found.loc;
		found.blk=CNode=temp;
		while(temp) {
			if(temp==top) {
				vSetTopVar();
				break;
			}
			temp=temp->next;
		}
		vSetCurrentNode();
		if(iCX+cur_hsize+CNode->lc[0]*PRN_SCR>bmaxx || iCX<bminx+1) {
			iCX=(bminx+bmaxx)/2;
		}
		find_flag=strlen(search_buf);
	}
	else {
		found.loc=wTL;
		found.blk=CNode;
		find_flag=OFF;
	}
}

int iSetSearchOpt(void)
{
	if(blk_or_not==0) {
		sfb.blk=SNode->next;
		sfb.loc=0;
		efb.blk=ENode;
		efb.loc=ENode->bn-1;
	}
	else if(blk_or_not==1) {
		if(!block_task) {
			vErrorMsgfDlg(10, 3, 0);
			return 0;
		}
		sfb.blk=sb.blk;
		sfb.loc=sb.loc;
		efb.blk=eb.blk;
		efb.loc=eb.loc;
	}
	return 1;
}

void vSearchStringDlg(void)
{
	vOpenDlgWindow(PW8);
	setmem(search_buf+80, 10, 0);
	search_buf[80]=case_sen;
	search_buf[81]=whole;
	search_buf[82]=up_down;
	search_buf[83]=blk_or_not;
	search_buf[84]=st_pos;
	if(iDialogDriver(PW8, search_buf)==5) {
		case_sen=search_buf[80];
		whole=search_buf[81];
		up_down=search_buf[82];
		blk_or_not=search_buf[83];
		st_pos=search_buf[84];
		vCloseDlgWindow(PW8);
		if(!iSetSearchOpt()) return;
		if(!st_pos && !block_task) {
			vSearchString(wTL, CNode);
		}
		else {
			if(up_down) vSearchString(sfb.loc, sfb.blk);
			else vSearchString(efb.loc, efb.blk);
		}
		if(find_flag==OFF) vErrorMsgfDlg(10, 6, 0);
		block_task=OFF;
		last_task=0;
	}
	else vCloseDlgWindow(PW8);
	return;
}

BYTE change_buf[170]={""};

void vChangeString(BYTE flag)
{
	INKEY ch;

	while(find_flag) {
		if(flag) {
			vRestoreEdit();
			key_flag=ON;
			vEndKeyTask();
			vShowFound(1);
			vDispExtraStatus(5);
			while(1) {
				vTCON();
				while(!wCheckKeyHit()) vTextCursorOperation(1, 1);
				vTCOFF();
				ch.i=bioskey(0);
				if(ch.ch[0]) {
					switch(ch.ch[0]) {
						case RETURN: case 'y': case 'Y':
							vShowFound(0);
							goto CHANGE;
						case 'n': case'N': vShowFound(0); goto REFIND;
						case ESC: case 'Q': case 'q':
							vShowFound(0); find_flag=0; return;
						default : vBeep();
					}
				}
				else vBeep();
			}
		}
		else {
			vGetCursorSize();
CHANGE:
			vDeleteStr(find_flag);
			vInsertStr(change_buf+80);
		}
REFIND:
		vSearchString(wTL, CNode);
	}
  vErrorMsgfDlg(0, 0, "”á ·¡¬w ¤aŽ‰ ”e´áˆa ´ô¯s“¡”a.");
}

void vChangeStringDlg(void)
{
	vOpenDlgWindow(PW9);
	strcpy(change_buf, search_buf);
	setmem(change_buf+160, 10, 0);
	change_buf[160]=case_sen;
	change_buf[161]=whole;
	change_buf[162]=answer;
	change_buf[163]=up_down;
	change_buf[164]=blk_or_not;
	change_buf[165]=st_pos;

	if(iDialogDriver(PW9, change_buf)==6) {
		vCloseDlgWindow(PW9);
		case_sen=change_buf[160];
		whole=change_buf[161];
		answer=change_buf[162];
		up_down=change_buf[163];
		blk_or_not=change_buf[164];
		st_pos=change_buf[165];
		strcpy(search_buf, change_buf);
		if(!iSetSearchOpt()) return;
		if(!st_pos && !block_task) {
			vSearchString(wTL, CNode);
		}
		else {
			if(up_down) vSearchString(sfb.loc, sfb.blk);
			else vSearchString(efb.loc, efb.blk);
		}
		block_task=OFF;
		last_task=1;
		vChangeString(answer);
	}
	else vCloseDlgWindow(PW9);
	return;
}

void vSearchAgain(void)
{
	vSearchString(wTL, CNode);
	if(find_flag==OFF) vErrorMsgfDlg(10, 6, 0);
	if(last_task) vChangeString(1);
}

void vGotoPageDlg(int i)
{
	WORD page_num;

	page_num=tw[cur_w].cp;
	switch(i) {
		case 0 :
			page_num=wGetNumber(page_num, 1, tot_page, "¡e", "Ñe¸¡e·¡•·"); break;
		case UP   : if(tw[cur_w].cp>1) page_num--; break;
		case DOWN : if(tw[cur_w].cp<tot_page) page_num++; break;
	}
	CNode=npGetPageStart(page_num, &cur_lnum);
	_CHECKNODE(CNode);
	if(ex_err==0) vErrorMsgfDlg(1, 0, 0);
	else wTL=wLine2Loc(cur_lnum-1, CNode);
}

void vSetFontDispOpt(void)
{
	full_vector++;
	if(full_vector>2) full_vector=0;
	key_flag=ON;
}

void vClipBoard(void)
{
	if(tw[0].active) {
		vChangeWindow(0);
	}
	else {
		sprintf(g_dir, "%s\\editswp0.swp", swp_path);
		if(bCheckFile(g_dir)!=0) {
			if(!iMakeNullSwapFile(g_dir)) return;
		}
		vEndKeyTask();
		if(tw[cur_w].active) {
			if(!iSwapDocTofile(cur_w)) {
				vErrorMsgfDlg(0, 2, "¯aµÓ");
				return;
			}
		}
		if(!iSwapFileToDoc(0)) {
			vErrorMsgfDlg(0, 1, "¯aµÓ");
			vExit();
		}
		vMCOFF();
		vDarkenTextWindowTitle(cur_w);
		vSwapTextWindowArea(1, cur_w);
		tw[0].active=ON;
		vPushWindow(0);
		cur_w=0;
		vDrawTextWindow(cur_w);
		vSetCurrent();
		vMCON();
	}
}

#include "kana.h"

BYTE *charl[]={
	"1234567890-=\\",
	"qwertyuiop[]",
	"asdfghjkl;'",
	"zxcvbnm,./",
	"`",
	"!@#$%^&*()_+|",
	"QWERTYUIOP\{\}",
	"ASDFGHJKL:\"",
	"ZXCVBNM<>?",
	"~",
};


BYTE jf_buf[3040];
BYTE jf_flag;
BYTE disp_japan=OFF;
extern BYTE *gsf[6];
extern WORD *gwf[5];
extern WORD *gjf[4];
extern BYTE engstr[];
extern BYTE eng_tbl[][12];
extern BYTE dvorak[];
void vSetKeyMapImage(void)
{
	FILE *fp;
	WORD i;
	BYTE t3;
	BYTE *tb;
	static BYTE h_e=0xff;

	if(h_e!=tw[cur_w].h_e) {
		h_e=tw[cur_w].h_e;
		if(h_e<3) {
			if(h_e) { // HANGUL 2 BULSIC
				if(h_e==1) sprintf(g_dir, "%s\\HAN2.KBD", font_path);
				else sprintf(g_dir, "%s\\HAN3.KBD", font_path);
				if((fp=fopen(g_dir, "rb"))==NULL) {
					vErrorMsgfDlg(0, 1, g_dir);
					return;
				}
				fread(jf_buf, 95*32, 1, fp);
				fclose(fp);
				jf_flag=1;
			}
			else {
				memcpy(jf_buf, gsf[0], 95*16);
				jf_flag=0;
			}
		}
		else {
			if(h_e==3) {
				for(i=0;i<95;i++) {
					memcpy(jf_buf+i*16, gsf[0]+(int)dvorak[i]*16, 16);
				}
				jf_flag=0;
			}
			else if(h_e<JAPANLOC) {  // etc
				memcpy(jf_buf, gsf[0], 95*16);
				jf_flag=0;
				for(i=0;i<12;i++) {
					t3=eng_tbl[h_e-ENGLOC][i];
					if(t3) {
						t3--;
						if(t3<96) tb=pGetFontCacheImage(3, 0x24, t3, 0)+1;
						else tb=pGetFontCacheImage(3, 0x34, t3, 0)+1;
						memcpy(jf_buf+(WORD)(engstr[i]-32)*16, tb, 16);
					}
				}
			}
			else if(h_e<HALFLOC) {  // japan input automata
				setmem(jf_buf, 32*95, 0);
				sprintf(g_dir, "%s\\JAPAN.FNT", font_path);
				if((fp=fopen(g_dir, "rb"))==NULL) {
					vErrorMsgfDlg(0, 1, "JAPAN.FNT");
					return;
				}
				fread(our_buf+32, 10848, 1, fp);
				fclose(fp);
				setmem(our_buf, 32, 0);
				if(h_e==JAPANLOC+2) {
				 for(i=0;i<95;i++) {
						memcpy(jf_buf+i*32, our_buf+hkana_tbl[i]*32, 32);
					}
				}
				else if(h_e==JAPANLOC+3){
					for(i=0;i<95;i++) {
						memcpy(jf_buf+i*32, our_buf+kkana_tbl[i]*32, 32);
					}
				}
				jf_flag=1;
			}
			else if(h_e<FULLLOC) { // half input automata
				sprintf(g_dir, "%s\\HALF.FNT", font_path);
				if((fp=fopen(g_dir, "rb"))==NULL) {
					vErrorMsgfDlg(0, 1, "HALF.FNT");
					return;
				}
				i=(h_e-HALFLOC);
				fseek(fp, ((unsigned long)i*95)<<4, SEEK_SET);
				fread(jf_buf, 95*16, 1, fp);
				fclose(fp);
				jf_flag=0;
			}
			else {  // full input automata
				sprintf(g_dir, "%s\\FULL.FNT", font_path);
				if((fp=fopen(g_dir, "rb"))==NULL) {
					vErrorMsgfDlg(0, 1, "FULL.FNT");
					return;
				}
				i=(h_e-FULLLOC);
				fseek(fp, ((unsigned long)i*95)<<5, SEEK_SET);
				fread(jf_buf, 95*32, 1, fp);
				fclose(fp);
				jf_flag=1;
			}
		}
  }
}

void vDrawKey(int x, int y, int count, int n)
{
	WORD i;
	BYTE c[4]={0, 0x04, 0x11, 0xe0};

	for(i=0; i<count; i++) {
		vWBoxBit(x-2, y*8+i*48-1, x+16+1, y*8+(i+1)*48-7);
		c[0]=charl[n][i];
		vPHFfar(x, y+i*6+4, c);

		if(jf_flag) {
			vPutWordImageBit(x, (y+i*6)<<3, (jf_buf+((WORD)charl[n][i]-32)*32));
			vPutWordImageBit(x, (y+i*6+2)<<3, (jf_buf+((WORD)charl[n+5][i]-32)*32));
		}
		else {
			vPutFreeImageByte(x, y+i*6, 16, 8,\
				jf_buf+((WORD)charl[n][i]-32)*16, 0);
			vPutFreeImageByte(x, y+i*6+2, 16, 8,\
				jf_buf+((WORD)charl[n+5][i]-32)*16, 0);
		}
	}
}

void vDrawKeys(int x, int y)
{
	vSetKeyMapImage();
	vDrawKey(x, y+1, 13, 0);
	vDrawKey(x+21, y+4, 12, 1);
	vDrawKey(x+42, y+7, 11, 2);
	vDrawKey(x+63, y+10, 10, 3);
	vDrawKey(x+63, y+1, 1, 4);
	vWLineXBit(x+17, x+61, (y+2)*8+2);
}

void vDrawKeyMap(void)
{
	int x, y;
//	BYTE temp;

//	temp=View_time;
//	View_time=OFF;

	bminx=0;
  bminy=0;
  bmaxx=MaxX;
  bmaxy=MaxY;
	x=MaxX-90;
	y=0;
	vDrawPanelByte(x, y, MaxX, 79, 0);
	vDrawKeys(x+5, y);
	disp_japan=ON;
//	View_time=temp;
	vClipOn(cur_w);
}

void vShowKeyMap(void)
{
	disp_japan^=1;
	vResetDesktop();
}

static int near iCompNode(BYTE *astr, BYTE *bstr)
{
	while(1) {
		if(*astr==0) astr++;
		else if(*astr==32) astr+=4;
		else break;
	}
	while(1) {
		if(*bstr==0) bstr++;
		else if(*bstr==32) bstr+=4;
		else break;
	}
	while(1) {
		if(*astr==0) {
			astr++;
			continue;
		}
		if(*bstr==0) {
			bstr++;
			continue;
		}
		if(*astr==32 || *bstr==32) return 0;
		if(*astr>*bstr || *bstr=='\r') return 0;
		if(*astr<*bstr || *astr=='\r') return 1;
		if(*astr>127) {
			if(*(astr+1)>*(bstr+1)) return 0;
			if(*(astr+1)<*(bstr+1)) return 1;
			astr+=5;
			bstr+=5;
		}
		else {
			astr+=4;
			bstr+=4;
		}
	}
}

static BYTE *pGetNumberInNode(BYTE *str, long *rtn, BYTE *eloc)
{
	BYTE abuf[11];
	BYTE abufloc=0;

	*rtn=0;
	while(1) {
		if(*str==0) str++;
		else {
			if((*str>47 && *str<58 && (*(str+3) & 0x40) == 0x40 ) ||
					(*str=='-' && (*(str+3) & 0x40) ==0x40) || (*str=='\r')) break;
//			if((*str>47 && *str<58) || (*str=='-') || (*str=='\r')) break;
			else if(*str>127) str+=5;
			else str+=4;
		}
		if(str==eloc) return 0;
	}
	while(1) {
		if(*str==0) {
			str++;
			continue;
		}
		if(*str==32 || *str=='\r')  break;
		if(*str>127) break;         
		else {
			if((*str>47 && *str<58 && (*(str+3) & 0x40) == 0x40 ) ||
					(abufloc==0 && *str=='-' && (*(str+3) & 0x40) ==0x40)) {
				abuf[abufloc++]=*str;
				if(abufloc>10) break;
				str+=4;
			}
			else if(*str==',') str+=4;
			else break;
		}
	}
	abuf[abufloc]=0;
	sscanf(abuf, "%ld", rtn);
	return str;
}

static void near vSwapNode(NODE *a, NODE *b)
{
	b->prev = a->prev;
	a->next = b->next;
	a->prev->next = b;
	if(b->next)	b->next->prev = a;
	a->prev = b;
	b->next = a;
}

extern BYTE CompWay;
extern BYTE SortWay;
extern BYTE CompStart;

void vSortNodeDlg(void)
{
	int n, i, j;
	NODE *t;
	WORD aloc, bloc, cc;
	int result;
	long anum, bnum;

	if(!block_task) return;
	block_task=OFF;
	if(sb.c>eb.c) cc=eb.c;
	else cc=sb.c;
	CompStart=cc;
	if(iSortOptDlg()) {
		cc=CompStart;
		t=sb.blk;
		for(n=0;eb.blk!=t;n++) t=t->next;
		if(n<1) return;
		for(i=0;i<n;i++) {
			t=sb.blk;
			_CHECKNODE(t);
			if(ex_err==0) {
				vErrorMsgfDlg(1, 0, 0);
				break;
			}
			aloc=iCol2Loc(cc, t);
			if(CompWay) pGetNumberInNode(t->line+aloc, &anum, (BYTE *)(0xffffffffl));
			for(j=0;j<n-i;j++) {
				_CHECKNODE(t);
				if(ex_err==0) {
					vErrorMsgfDlg(1, 0, 0);
					break;
				}
				_CHECKNODE(t->next);
				if(ex_err==0) {
					vErrorMsgfDlg(1, 0, 0);
					break;
				}
				bloc=iCol2Loc(cc, t->next);
				if(CompWay) {
					pGetNumberInNode(t->next->line+bloc, &bnum, (BYTE *)(0xffffffffl));
					if(anum>bnum) result=0;
					else result=1;
				}
				else result=iCompNode(t->line+aloc, t->next->line+bloc);
				if(result==SortWay) {
					vSwapNode(t, t->next);
					if(t==sb.blk) sb.blk=sb.blk->prev;
					t=t->prev;
				}
				else {
					anum=bnum;
					aloc=bloc;
				}
				t=t->next;
			}
		}
		tw[cur_w].modify=ASV_MODIF=ON;
		CNode=sb.blk;
		wTL=0;
		cur_lnum=1;
		t=SNode;
		while(t->next) t=t->next;
		ENode=t;
	}
}

extern BYTE *menu_PW22[];

void vBlockSumDlg(void)
{
	long lnum, sum=0;
	BYTE *str, *estr;
	NODE *t;
	BYTE cc;

	if(block_task==ON) {
		block_task=OFF;
		t=sb.blk;
		str=t->line+iCol2Loc(sb.c, t);
		if(sb.blk==eb.blk) estr=t->line+eb.loc;
		else estr=t->line+t->bn-1;
		while(1) {
			str=pGetNumberInNode(str, &lnum, estr);
			sum+=lnum;
			if(str==0) break;
			if(str==estr) break;
//			if(*str>127) str+=5;
//			else str+=4;
		}
	}
	else {
		block_task=OFF;
		t=sb.blk;
		if(sb.c>eb.c)	cc=eb.c;
		else cc=sb.c;
		while(1) {
			str=t->line+iCol2Loc(cc, t);
			pGetNumberInNode(str, &lnum, (BYTE *)(0xffffffffl));
			sum+=lnum;
			if(eb.blk==t) break;
			t=t->next;
		}
	}
	sprintf(menu_PW22[9]+10, "%ld", sum);
//	vGLSFileIO(1);
}
