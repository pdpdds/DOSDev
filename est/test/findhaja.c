#define _FINDHJ

#include <stdio.h>
// #include <dir.h>
#include <string.h>
#include <stdlib.h>

#if 0
#include <alloc.h>
#include <mem.h>
#include <dos.h>
#include "est.h"
	#include "21.h"
	#include "proto.h"
#include "mouse.h"
#endif

#include "hjtable.h"

typedef unsigned char	BYTE;
typedef unsigned short  WORD;

extern BYTE *our_buf;
extern BYTE font_path[];
extern BYTE g_dir[];
extern menu_panel panel[];
extern BYTE cur_w;
extern text_window tw[];
extern BYTE dic_data[];
extern BYTE the21[];

BYTE hanja_dic_name[80]={"HANJA.DIC"};

#define NELEMS(arr) (sizeof(arr) / sizeof(arr[0]))

int iCompWord(const unsigned *p1, const unsigned *p2)
{
	 return(*p1 - *p2);
}

int iGetHanjaInfo(WORD i, WORD *loc)
{
	unsigned *temp;

	temp = (unsigned *)bsearch(&i, hanja_table, NELEMS(hanja_table),
				 sizeof(hanja_table[0]),
				 (int(*)(const void *, const void *))iCompWord);
	if(temp) {
		(*loc)=temp[1];
		return temp[3]-temp[1];
	}
	else return 0;
}

int iGetHanjaLoc(WORD i)
{
	unsigned *temp;
	int loc;

	temp = (unsigned *)bsearch(&i, hanja_table, NELEMS(hanja_table),
				 sizeof(hanja_table[0]),
				 (int(*)(const void *, const void *))iCompWord);
	if(temp) {
		loc=(int)((unsigned *)temp-(unsigned *)hanja_table);
		return loc/2;
	}
	else return 0;
}

extern popup_window pop[];
extern int pop_x, pop_y;

int iGetHanjaChoice(int num)
{
	int tx, ty, choice;
	int mx, my;

	mx=mousex;
	my=mousey;
	if(lbp==0) return -2;
	if(mx>pop[PW6].x+27   && mx<pop[PW6].x+pop[PW6].lx*8 &&
		 my>pop[PW6].y*8+8 && my<pop[PW6].y*8+pop[PW6].ly*8) {
		tx= mx-pop[PW6].x-27;
		if(tx<1) return -1;
		tx/=18;
		ty= my-pop[PW6].y*8-12;
		if((ty/=3*8)>9) ty=9;
		choice=tx*10+ty;
		if(choice>num) return -1;
		else return choice;
	}
	else return -1;
}

int iSelectHanja(BYTE *hc)
{
	int code;
	WORD count;
	WORD num;
	WORD loc;
	unsigned long floc;
	FILE *fp;
	CODE2 co;
	INKEY ch;
	BYTE h[5]={0,0,0,0x11,0xe0};
	char choice=1, old_cho;

	vMCOFF();
	if(*hc<128) return 0;
	h[1]=co.c2[0]=hc[1];
	h[0]=co.c2[1]=hc[0];
	num=iGetHanjaInfo(co.i, &loc);

	floc=((unsigned long)loc)<<5;
	if(num) {
		sprintf(g_dir, "%s\\hanja.fnt", font_path);
		if((fp=fopen(g_dir, "rb"))==NULL) {
			vErrorMsgfDlg(0, 1, "HANJA.FNT");
			return 0;
		}
		setmem(our_buf+30000, 32*70, 0);
		fseek(fp, floc, SEEK_SET);
		fread(our_buf+30032, sizeof(WORD), num<<4, fp);
		fclose(fp);


ST:	vOpenDlgWindow(PW6);
		vMCOFF();
		for(count=0;count<num/10+1;count++) {
			vPutBitmapHanjaTable(pop[PW6].x+27+count*18, pop[PW6].y+2,
							(WORD *)(our_buf+30000+count*320), 10);
		}
		vPFFfar(pop[PW6].x+27, pop[PW6].y+2, h);
		for(;;) {
			vXORBarBytex16(pop[PW6].x+27+(choice/10)*18, pop[PW6].y+1+(choice%10)*3, 4);
			vMCON();
			while(!wCheckKeyHit()){
				// mouse check
				if(lbp) {
					if(mousex < pop[PW6].x+24) {
						code=iMoveDlgWindow(PW6);
						while(lbp);
						if(code==-2) 	goto MOV;
						if(code==-3) 	goto OUT;
					}
					else {
						old_cho=choice;
						for(;;) {
							code=iGetHanjaChoice(num);
							old_cho=code;
							if(code >-1 && choice!=old_cho) {
								vMCOFF();
								vXORBarBytex16(pop[PW6].x+27+(choice/10)*18, pop[PW6].y+1+(choice%10)*3, 4);
								choice=old_cho;
								vXORBarBytex16(pop[PW6].x+27+(choice/10)*18, pop[PW6].y+1+(choice%10)*3, 4);
								vMCON();
							}
							else if(code==-2) {
								while(lbp);
								goto RN;
							}
						}
					}
				}
			}
			ch.i=bioskey(0);
			vMCOFF();
			vXORBarBytex16(pop[PW6].x+27+(choice/10)*18, pop[PW6].y+1+(choice%10)*3, 4);
			if(!ch.ch[0]) {
				switch(ch.ch[1]) {
					case RIGHT :
						if(choice==num) break;
						choice++;
						break;
					case LEFT :
						if(choice==0) break;
						choice--;
						break;
					case DOWN :
						choice+=10;
						if(choice>num) choice=num;
						break;
					case UP :
						if(choice<10) choice=0;
						else choice-=10;
						break;
					case PGUP :
					case PGDN :
					case HOME :
					case END :
						break;
					case F7 :
						if(iKeyMoveDlgWindow(PW6)) {
MOV:					vCloseDlgWindow(PW6);
							pop[PW6].x=pop_x;
							pop[PW6].y=pop_y;
							goto ST;
						}
						break;
				}
			}
			else {
				switch(ch.ch[0]) {
RN:				case RETURN :
						vMCOFF();
						vCloseDlgWindow(PW6);
						if(choice==0) {
							hc[4]&=0xe0;
						}
						else {
							hc[4]=(choice-1);
							hc[4]|=0x80;
							tw[cur_w].modify=ON;
						}
						return choice;
					case ESC :
OUT:				vMCOFF();
						vCloseDlgWindow(PW6);
						return 0;
				}
			}
		}
	}
	else return 0;
}

#define BLKSIZE 512
#define DATASIZE 506

struct hanja_words {
	int eloc;
	DWORD next;
	BYTE b[DATASIZE];
} wb;

static int near iSearchHanjaWord(BYTE flag, BYTE *str, BYTE *tbl, FILE *fp)
{
	INKEY c;
	int i, len;
	int blk_num;
	int loc=0;
	unsigned long floc;
	int ok=0;
	int found=0;

	len=strlen(str);
	c.ch[0]=str[len-1];
	c.ch[1]=str[len-2];
	len/=2;
	blk_num=iGetHanjaLoc(c.i);
	floc=(unsigned long)blk_num*BLKSIZE+512;

	do {
		fseek(fp, floc, SEEK_SET);
		fread(&wb, BLKSIZE, 1, fp);
		while(loc<DATASIZE) {
			if(wb.b[loc]==0 || loc>=wb.eloc) break;
			if(wb.b[loc]==len) {
				for(i=0;i<len*2-2;i++) {
					if(wb.b[loc+1+i]==str[i]) ok=1;
					else {
						ok=0;
						break;
					}
				}
				if(flag) {
					if(ok==1) {
						memcpy(tbl+found*len, wb.b+loc+(wb.b[loc]-1)*2+1, wb.b[loc]);
						found++;
						if(found==9) break;
					}
				}
				else {
					for(i=0;i<len;i++) {
						if(wb.b[loc+(len-1)*2+i+1]==tbl[i]) ok=1;
						else {
							ok=0;
							break;
						}
					}
					if(ok==1) {
						movmem(wb.b+loc+(len-1)*2+len+1, wb.b+loc, DATASIZE-(loc+(len-1)*2+len+1));
//						setmem(wb.b+DATASIZE-((len-1)*2+len+1), ((len-1)*2+len), 0);
						wb.eloc-=((len-1)*2+len+1);
						fseek(fp, floc, SEEK_SET);
						fwrite(&wb, BLKSIZE, 1, fp);
						return 1;
					}
				}
			}
			loc+=(wb.b[loc]-1)*2+wb.b[loc]+1;
		}
		floc=wb.next;
		loc=0;
	} while(wb.next!=0);
	return found;
}

static void near vPutHanjaWordToFile(BYTE *str, BYTE *tbl, FILE *fp)
{
	INKEY c;
	int len;
	int blk_num;
	unsigned long floc, next;

	fseek(fp, 508, SEEK_SET);
	fread(&next, 4, 1, fp);

	len=strlen(str);
	c.ch[0]=str[len-1];
	c.ch[1]=str[len-2];
	len/=2;
	blk_num=iGetHanjaLoc(c.i);
	floc=(unsigned long)blk_num*BLKSIZE+512;

	fseek(fp, floc, SEEK_SET);
	fread(&wb, BLKSIZE, 1, fp);
	while(1) {
		if(wb.eloc+len*2-2+len+1<DATASIZE) {
			wb.b[wb.eloc]=len;
			memcpy(wb.b+wb.eloc+1, str, len*2-2);
			memcpy(wb.b+wb.eloc+len*2-1, tbl, len);
			wb.eloc+=len*2-2+len+1;
			fseek(fp, floc, SEEK_SET);
			fwrite(&wb, BLKSIZE, 1, fp);
			return;
		}
		if(wb.next) {
			floc=wb.next;
			fseek(fp, floc, SEEK_SET);
			fread(&wb, BLKSIZE, 1, fp);
		}
		else {
			wb.next=next;
			fseek(fp, floc, SEEK_SET);
			fwrite(&wb, BLKSIZE, 1, fp);
			floc=next;
			next+=512;
			fseek(fp, 508, SEEK_SET);
			fwrite(&next, 4, 1, fp);
			setmem(&wb, BLKSIZE, 0);
		}
	}
}

void vUpdateHanjaWordDlg(void)
{
	BYTE str[41];
	BYTE t[21];
	BYTE ret;
	BYTE b5[5];
	BYTE ok=0;
	int loc=0, i=0;
	FILE *fp;

	if(hanja_dic_name[0]==0) {
		vErrorMsgfDlg(0, 0, "Ðe¸a”e´á¬a¸å· ‰w¡¡w·¡ ´ô¯s“¡”a.");
		return;
	}
	if((fp=fopen(hanja_dic_name, "rb+"))==NULL) {
		vErrorMsgfDlg(0, 1, hanja_dic_name);
		return;
	}
	if(!iCheckGWP(0, fp, dic_data)) {
		vErrorMsgfDlg(0, 4, "Ðe¸a ¬a¸å");
		return;
	}
	setmem(str, 40, 0);
	if(iGetStringDlg(1, str, "—w¢Ði Ðe¸a´á“e?")!=1) goto OUT;
	if(*str==0) goto OUT;
	str[13]=0;
	if(strlen(str)>3) {
		while(str[i]) {
			ok=0;
			memcpy(b5, str+i, 2);
			if(*str>127) {
				if((ret=iSelectHanja(b5)) != 0) {
					t[loc++]=ret;
					ok=1;
				}
				else break;
				i+=2;
			}
			else break;
		}
	}
	if(ok) vPutHanjaWordToFile(str, t, fp);
OUT:
	fclose(fp);
}

BYTE *ext_1;
BYTE *ext_2;
int hj_len;

int iSelectHanjaWord(BYTE *ch, BYTE *ttbl)
{
	FILE *fp;
	int choice;
	int found, len;
	BYTE buf[10];
	BYTE tbl[60];

	if(hanja_dic_name[0]==0) return 0;
	if((fp=fopen(hanja_dic_name, "rb+"))==NULL) {
		vErrorMsgfDlg(0, 1, hanja_dic_name);
		return 0;
	}
	if(!iCheckGWP(0, fp, dic_data)) {
		vErrorMsgfDlg(0, 4, "Ðe¸a ¬a¸å");
		return 0;
	}
	len=strlen(ch);
	memcpy(tbl, ttbl, 60);
	while(1) {
		found=iSearchHanjaWord(1, ch, tbl, fp);
		if(found==0 && len>2) {
			movmem(ch+2, ch, len-1);
			movmem(tbl+1, tbl, len/2);
			len=strlen(ch);
		}
		else break;
	}
REST:
	if(found) {
		ext_1=ch;
		ext_2=tbl;
		hj_len=len/2;
		vSetMenuParaAll(HJPAN, 0, 0, found+1, 2);
		choice=iPullDownMenu(HJPAN);
		vRestoreMenuBk(HJPAN);
		if(choice==-3) {
			found=iSearchHanjaWord(1, ch, tbl, fp);
			choice=panel[HJPAN].last_menu-2;
			memcpy(buf, tbl+hj_len*choice, hj_len*2);
			iSearchHanjaWord(0, ch, buf, fp);
			movmem(tbl+hj_len*(choice+1), tbl+hj_len*choice, 60-hj_len*(choice+1));
			found--;
			goto REST;
		}
		if(choice>1 && choice<found+2){
			choice-=2;
			memcpy(ttbl, tbl+choice*hj_len, hj_len);
			found=(len/2)-1;
		}
		else found=-1;
	}
	fclose(fp);
	return found;
}


//static BYTE near wb[1024];


int iMakeNullHanjaDic(void)
{
	FILE *fp;
	int i;
	BYTE ok=1;
	long next;

	if((fp=fopen(hanja_dic_name, "wb+"))==NULL) {
		printf("Cannot make %s\n", hanja_dic_name);
		return 0;
	}
	setmem(&wb, BLKSIZE, 0);
	next=512L*473L;
	fwrite(the21, 27, 1, fp);
	fwrite(dic_data, 21, 1, fp);
	if(fwrite(&wb, 1, BLKSIZE-52, fp)!=BLKSIZE-52) ok=0;
	if(fwrite(&next, 1, 4, fp)!=4) ok=0;
	for(i=0;i<472;i++) {
		if(fwrite(&wb, 1, BLKSIZE, fp)!=BLKSIZE) {
			ok=0;
			break;
		};
	}
	fclose(fp);
	return ok;
}
