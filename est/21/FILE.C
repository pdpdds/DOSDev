#define  _FILE
#include <dir.h>
#include <dos.h>
#include <io.h>
#include <fcntl.h>
#include <mem.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <alloc.h>
#include <ctype.h>
#include <direct.h>
#include <time.h>
#include "est.h"
#include "21.h"
#include "proto.h"

extern WORD SYSPEED;
extern unsigned long Scount;
extern text_window tw[];
extern text_window def_tw;
extern BYTE def_lc[];
extern BYTE load_lc[];
extern BYTE cur_w;
extern NODE *SNode, *CNode, *ENode;
extern BYTE load_tab;
extern int graph_mode;
extern int set_graph_mode;
extern BYTE color_mode;

extern BYTE *vid_addr;
extern BYTE *our_buf;
extern int MaxX, MaxY;
extern int bminx, bminy, bmaxy, bmaxx;

extern Mem tm[];
extern BYTE font_path[];
extern BYTE exe_path[];
extern BYTE hanja_dic_name[];
extern text_window def_tw;
extern BYTE sb5[];
extern BYTE sb4[];
extern BYTE curc[];
extern BYTE fontc[];
extern BYTE prn_setup[];
extern BYTE mouse_kind;
extern BYTE mouse_port;
extern BYTE path_buf[];
extern BYTE g_drive[];
extern BYTE g_dir[];
extern BYTE g_file[];
extern BYTE g_ext[];
extern WORD our_buf_size;
extern WORD work_area_size;
extern BYTE full_vector;
extern BYTE disp_space;
extern BYTE disp_newline;
extern BYTE disp_return;
extern BYTE saving;
extern BYTE mouse_on_off;
extern int m_rate;
extern BYTE	show_rod;
extern BYTE scr_saver_num;
extern WORD no_work_time;
extern BYTE nicks[6][30];
extern BYTE rev_flag;
extern BYTE han_kbd;
extern BYTE eng_kbd;
extern BYTE mc_color;
extern WORD key_macro[10][101];
extern unsigned char key_macro_state[10][101];
extern WORD far no_work;
extern BYTE View_time;
extern BYTE mk_bak;
extern BYTE ins_lock;
extern BYTE asave;
extern BYTE as_time;
extern BYTE psort;
extern WORD papwids[];
extern WORD paplens[];
extern BYTE *dataPW14[];
extern BYTE C_cnt;
extern BYTE C_key[];
extern BYTE *key_name[];
extern BYTE MRfunc1;
extern BYTE MRfunc2;

BYTE aflag=0;  		 // auto save flag;
BYTE save_null=1;  // null space save or no;

BYTE dosing=0;
BYTE password[20];
BYTE nick_name[30];

#define MAXMEM 4096

BYTE far msb_kd[MAXMEM*2];
BYTE far msb_sz[MAXMEM*2];

BYTE swp_path[80];
BYTE vms_name[80];
void (*xms_func)();
WORD xms;
int vms;
BYTE xms_state, ems_state, vms_state;
WORD vms_start=0;

BYTE *FILE_PBUF=0;

struct XMSBLK {
	unsigned long len;
	WORD sh;
	BYTE *soff;
	WORD dh;
	BYTE *doff;
} xmsblk;

BYTE config_name[80]={"21.CNF"};

BYTE the21[]={
	"Word Processor 21st Century"
};

BYTE tthe21[]={
	"Word Processor 21th Century"
};

BYTE gwp_data[]={
	" Data File V1.0\x1a\x5\x4\x3\x2\x1"
};

BYTE help_data[]={
	" Help File V1.0\x1a\x5\x4\x3\x2\x1"
};
BYTE swp_data[]={
	" Swap File V1.0\x1a\x5\x4\x3\x2\x1"
};

BYTE cfg_data[]={
	" Config File V1.3\x1a\x3\x2\x1"
};
BYTE mac_data[]={
	" Macro File V1.0\x1a\x4\x3\x2\x1"
};

BYTE gls_data[]={
	" Glossary File V1.0\x1a\x1"
};


BYTE sta_data[]={
	" State File V1.0\x1a\x4\x3\x2\x1"
};

BYTE dic_data[]={
	" Hanja Dictionary\x1a\x3\x2\x1"
};

BYTE hwp12_data[]={
	"HWP Document File V1.20 \x1a\x1\x2\x3\x4\x5"
};

WORD wUseXMS(WORD *a, WORD *d)
{
	WORD ta, td, rtn;
	ta=*a;
	td=*d;
	asm mov ax, ta;
	asm mov dx, td;
	asm call [xms_func];
	asm mov ta, ax;
	asm mov td, dx;
	asm mov rtn, bx;
	(*a)=ta;
	(*d)=td;
	return rtn;
}

WORD wMoveXMS(WORD s, WORD d)
{
	WORD rtn;

	asm push ds;
	asm mov ax, 0b00h;
	asm mov si, s;
	asm mov ds, d;
	asm call [xms_func];
	asm mov rtn, ax;
	asm pop ds;

	return rtn;
}

void *pAllocUMB(WORD size)
{
	WORD a, b, d;

	if(xms_state) {
		a=0x1000;
		d=size/16+1;
		b=wUseXMS(&a, &d);
		if(a==1) return MK_FP(b, 0);
	}
	return (BYTE *)pGetMem(size);
}

void vFreeUMB(void *buf)
{
	WORD a, seg;

	if(!buf) return;
	seg=FP_SEG(buf);
	if(seg>0xa000) {
		a=0x1100;
		wUseXMS(&a, &seg);
	}
	else free(buf);
}

WORD wInitXMS(void)
{
	union REGS r;
	struct SREGS sr;
	WORD a, d;
	WORD maxxms=0;

	r.h.ah=0x43;
	r.h.al=0;
	int86(0x2f, &r, &r);
	if(r.h.al==0x80) xms_state=ON;
	else xms_state=OFF;
	if(xms_state) {
		r.h.ah=0x43;
		r.h.al=0x10;
		int86x(0x2f, &r, &r, &sr);
		xms_func=(void *)MK_FP(sr.es, r.x.bx);
		a=0x0800;
		wUseXMS(&a, &d);
		if(a>MAXMEM) maxxms=MAXMEM;
		else maxxms=a;
		a=0x0900;
		d=maxxms;
		wUseXMS(&a, &d);
		if(a==1) xms=d;
		else maxxms=0;
	}
	return maxxms;
}

void vInitEMS(void)
{
	union REGS r;
	struct SREGS sr;
	char emm_name[]={"EMMXXXX0"};
	char *check_name;

	r.h.ah=0x35;
	r.h.al=0x67;
	int86x(0x21, &r, &r, &sr);
	check_name=MK_FP(sr.es, 0x0a);
	if(memcmp(emm_name, check_name, 8)==0) ems_state=ON;
	else ems_state=OFF;
}

int iMake21VMS(void)
{
	char *t;

	sprintf(vms_name, "%s\\21VMS.SWP", swp_path);
	unlink(vms_name);
	if((vms=creatnew(vms_name, FA_ARCH))==-1) {
		printf("Cannot make swap file!\n");
		return 0;
	}
	t=(char *)pGetMem(0x7fff);
	if(!t) {
		close(vms);
		return 0;
	}
	setmem(t, 4096, 0);
	if(write(vms, t, 4096)!=4096) {
		close(vms);
		free(t);
		return 0;
	}
	free(t);
	close(vms);
	return 1;
}

void vInitVMS(void)
{

	sprintf(vms_name, "%s\\21VMS.SWP", swp_path);
	iMake21VMS();
	if((vms=open(vms_name, O_BINARY | O_RDWR))==-1) {
		vms_state=OFF;
		return;
	}
//	close(vms);
	vms_state=ON;
	return;
}

void vDeinitVMS(void)
{
	struct ffblk ffblk;
	int done;

	close(vms);
	sprintf(g_dir, "%s\\*.swp", swp_path);
	done=findfirst(g_dir, &ffblk, FA_ARCH);
	while(done==0) {
		sprintf(g_dir, "%s\\%s", swp_path, ffblk.ff_name);
		unlink(g_dir);
		done=findnext(&ffblk);
	}
}

WORD wMemoryLeft(void)
{
	WORD size=0;
	int i;
	for(i=0;i<MAXMEM*2;i++) {
		if(msb_sz[i]==0) size++;
	}
	return size>>1;
}

int iInitMemorySystem(void)
{
	int xms_size;
	int i;

//	file_pbuf_flag(ON);
	xms_size=wInitXMS();
	if(xms_state)	{
		printf("XMS installed at %04X:%04X ", FP_SEG(xms_func), FP_OFF(xms_func));
		printf("and %uK bytes allocated by 21st Century!\n", xms_size);
	}else   printf("XMS not installed\n");
	if(xms_size<MAXMEM) {

		vInitVMS();
		if(vms_state)	printf("VMS installed\n");
		else {
			printf("VMS not installed!\n");
			printf("Check you HDD's free space or swap path!\n");
			printf("Cannot Execute 21st Century!!\n");
			return 0;
		}
	}
	else printf("XMS is enough to execute 21st Century! Not need VMS!\n");
	for(i=0;i<MAXMEM*2;i++) {
		if(i<xms_size*2) msb_kd[i]=1;
		else msb_kd[i]=3;
	}
	vms_start=xms_size;
	return 1;
}

void vDeinitMemorySystem(void)
{
	WORD a, d;
	if(xms_state) {
		a=0x0a00;
		d=xms;
		wUseXMS(&a, &d);
	}
	if(vms_state) unlink(vms_name);
}

static WORD wAllocMem(WORD size)
{
	WORD i, j;
	BYTE ok=0, kind;

	size=(size>>9)+1;
	for(i=0;i<MAXMEM*2;i++) {
		if(msb_sz[i]==0) {
			kind=msb_kd[i];
			for(j=i;j<size+i;j++) {
				if(msb_sz[j]==0 && msb_kd[j]==kind) ok=1;
				else {
					ok=0;
					break;
				}
			}
			if(ok==1) {
				setmem(msb_sz+i, size, size);
				return i+1;
			}
		}
	}
	return 0;
}

void vFreeMem(Mem *h)
{
	BYTE size;

	if(*h<1) {
//    vErrorMsgfDlg(0, 0, "¡A¡¡Ÿ¡ Ði”wÐ¹AµAœá·³“¡”a.");
    return;
	}
	size=msb_sz[*h-1];
	if(size) setmem(msb_sz+*h-1, size, 0);
	*h=0;
}

static BYTE bSwapVMS(BYTE r_w, WORD handle, BYTE *buf)
{
	unsigned long loc;

	handle--;
//	if((vms=open(vms_name, O_BINARY | O_RDWR))==-1) return 0;
	loc=handle-vms_start*2;
	loc<<=9;
	if(r_w) {
		if(lseek(vms, loc, SEEK_SET)==-1L) return 0;
		write(vms, buf, (WORD)512*msb_sz[handle]);
	}
	else {
		if(lseek(vms, loc, SEEK_SET)==-1L) return 0;
		read(vms, buf, (WORD)512*msb_sz[handle]);
	}
//	close(vms);
	return 1;
}

static BYTE bSwapXMS(BYTE r_w, WORD handle, BYTE *buf)
{
	WORD s, d;
	unsigned long loc;

	handle--;
	xmsblk.len=(unsigned long)512*msb_sz[handle];
	loc=handle;
	loc<<=9;
	if(r_w) {
		xmsblk.sh=0;
		xmsblk.soff=buf;
		xmsblk.dh=xms;
		xmsblk.doff=(BYTE *)loc;
	}
	else {
		xmsblk.sh=xms;
		xmsblk.soff=(BYTE *)loc;
		xmsblk.dh=0;
		xmsblk.doff=buf;
	}
	s=FP_OFF(&xmsblk);
	d=FP_SEG(&xmsblk);
	return wMoveXMS(s, d);
}

int iSwapMem(BYTE flag, WORD size, Mem *m, BYTE *buf)
{
	WORD handle=0xffff;

	if(flag) { // write to ms
		if(*m) {
			if(size<msb_sz[*m]) {
				vFreeMem(m);
				vErrorMsgfDlg(1, INTERNAL_ERR, 0);
			}
		}
		handle=wAllocMem(size);
		if(handle) {
			*m=handle;
			if(msb_kd[handle-1]==1) {
				if(bSwapXMS(1, handle, buf)==1) return 1;
			}
			else {
				if (bSwapVMS(1, handle, buf)==1) return 1;
			}
			vFreeMem(m);
			return 0;
		}
		else return -1;
	}
	else { // read from ms
		handle=*m;
		if(msb_kd[handle-1]==1) return bSwapXMS(0, handle, buf);
		else return bSwapVMS(0, handle, buf);
	}
}

int iCheckGWP(BYTE flag, FILE *fp, BYTE *data)
{
	BYTE buf1[50];
	BYTE buf2[30];

	fread(buf1, 48, 1, fp);
	if(flag) {
		setmem(nick_name, 25, 0);
		fread(nick_name, 20, 1, fp);
		fread(password, 10, 1, fp);
		fread(buf2, 30, 1, fp);
	}
	buf1[48]=0;
	sprintf(g_dir, "%s%s", the21, data);
	if(strcmp(buf1, g_dir)==0) return 1;
	sprintf(g_dir, "%s%s", tthe21, data);
	if(strcmp(buf1, g_dir)==0) return 1;
	return 0;
}


int iCheckHWP(FILE *fp)
{
	BYTE rtn_num=0;

	fseek(fp, 0L, SEEK_SET);
	setmem(nick_name, 25, 0);
	fread(our_buf, 97, 1, fp);
	our_buf[30]=0;
	if(strcmp(our_buf, hwp12_data)==0) {
		fread(nick_name, 20, 1, fp);
		rtn_num=2;
	}
	return rtn_num;
}

void load_info(FILE *fp)
{
	int i;

	psort=fgetc(fp);
	for(i=6; i<10; i++) papwids[i]=getw(fp);
	for(i=6; i<10; i++) paplens[i]=getw(fp);
	for(i=36; i<40; i++) fread(dataPW14[i], 12, 1, fp);

	full_vector =fgetc(fp);
	disp_return =fgetc(fp);
	disp_newline =fgetc(fp);
	disp_space =fgetc(fp);
	scr_saver_num =fgetc(fp);
	no_work_time =fgetc(fp);
	m_rate = fgetc(fp);
	show_rod = fgetc(fp);
	View_time= fgetc(fp);
	mk_bak=fgetc(fp);
	ins_lock=fgetc(fp);
	asave=fgetc(fp);
	as_time=fgetc(fp);
	MRfunc1=fgetc(fp);
	MRfunc2=fgetc(fp);

	C_cnt=fgetc(fp);
	fread(C_key, 10, 1, fp);
	for(i=KEYMAX; i<KEYMAX+10; i++) fread(key_name[i], 11, 1, fp);
}


int iReadConfig(BYTE *mk, BYTE *mp, BYTE *fname)
{
	FILE *fp;
//	int i;

	if((fp=fopen(fname, "rb"))==NULL) {
		printf("\nCannot open %s file!\n", fname);
		exit(-1);
	}
	if(!iCheckGWP(0, fp, cfg_data)) {
		printf("\nConfig file is unusable!! Excute 21SET.EXE!!");
		unlink(fname);
		exit(-1);
	}
	set_graph_mode=fgetc(fp);
	*mk=fgetc(fp);
	*mp=fgetc(fp);
	sb5[4]|=fgetc(fp);
	sb4[3]|=fgetc(fp);  // 8
	fread(exe_path, 80, 1, fp);
	fread(font_path, 80, 1, fp);
	fread(swp_path, 80, 1, fp);  // 240
	strcpy(hanja_dic_name, exe_path);
	strcat(hanja_dic_name, "\\HANJA.DIC");

	def_lc[0]		=fgetc(fp);
	def_lc[1]		=fgetc(fp);
	def_lc[2]		=fgetc(fp);
	def_lc[3]		=fgetc(fp);
	def_lc[4]		=fgetc(fp);

	def_tw.word_gap		=fgetc(fp);
	def_tw.left_s			=fgetc(fp);
	def_tw.up_s				=fgetc(fp);
	def_tw.down_s			=fgetc(fp);

	prn_setup[0]		=fgetc(fp);
	prn_setup[1] 		=fgetc(fp);
	prn_setup[2]		=fgetc(fp);
	prn_setup[3]		=fgetc(fp);
	rev_flag				=fgetc(fp);
	han_kbd					=fgetc(fp);
	eng_kbd					=fgetc(fp);
	mc_color				=fgetc(fp);   // 17
	fread(fontc, 3, 1, fp);
	fread(curc, 3, 1, fp);

	load_info(fp);
	fclose(fp);
	return 1;
}

void vWriteConfig(void)
{
	FILE *fp;
	int i;

	if((fp=fopen(config_name, "rb+"))==NULL) {
		vErrorMsgfDlg(0, 2, config_name);
		return;
	}
	fseek(fp, 48+5+240+17+6, SEEK_SET);
	fputc(psort, fp);
	for(i=6; i<10; i++) putw(papwids[i], fp);
	for(i=6; i<10; i++) putw(paplens[i], fp);
	for(i=36; i<40; i++) fwrite(dataPW14[i], 12, 1, fp);
	fputc(full_vector, fp);
	fputc(disp_return, fp);
	fputc(disp_newline, fp);
	fputc(disp_space, fp );
	fputc(scr_saver_num, fp);
	fputc(no_work_time, fp);
	fputc(m_rate, fp);
	fputc(show_rod, fp);
	fputc(View_time, fp);
	fputc(mk_bak, fp);
	fputc(ins_lock, fp);
	fputc(asave, fp);
	fputc(as_time, fp);
	fputc(MRfunc1, fp);
	fputc(MRfunc2, fp);

	fputc(C_cnt, fp);
	fwrite(C_key, 10, 1, fp);
	for(i=KEYMAX; i<KEYMAX+10; i++) fwrite(key_name[i], 11, 1, fp);

	fclose(fp);
}

void vReadKeyMacro(BYTE *fname)
{
	FILE *fp;

	if((fp=fopen(fname, "rb"))==NULL) {
		setmem(key_macro, 101*20, 0);
		setmem(key_macro_state, 101*10, 0);
		return;
	}
	if(iCheckGWP(0, fp, mac_data)) {
		fread(key_macro, 101*10, 2, fp);
		fread(key_macro_state, 101*10, 1, fp);
	}
	fclose(fp);
}

void vWriteKeyMacro(BYTE *fname)
{
	FILE *fp;

	if((fp=fopen(fname, "wb+"))==NULL) return;
	sprintf(g_dir, "%s%s", the21, mac_data);
	fwrite(g_dir, 48, 1, fp);
	fwrite(key_macro, 101*10, 2, fp);
	fwrite(key_macro_state, 101*10, 1, fp);
	fclose(fp);
}

int iReadTextData(BYTE flag, BYTE wnum, FILE *fp)
{
	BYTE lc[6];
	BYTE kd;
	WORD len;
	BYTE *str;
	BYTE *ths;
	BYTE starting=1;

	for(;;) {
		kd=fgetc(fp);
		len=getw(fp);
		if(!len || len==0xffff) break;
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
		if(feof(fp)) {
			free(str);
			vFreeLHB(ths, lc[5]);
			vErrorMsgfDlg(10, 11, 0);
			if(starting) return -1;
			return 0;
		}
		if(!iAppendNode(str, lc, ths, len, kd)) {
			vErrorMsgfDlg(1, 0, 0);
			return 0;
		}
		starting=0;
		if(flag & 1) vAnalText(ENode);
	}
	if(flag & 2) {
		fread(&(tw[wnum].x), sizeof(text_window)-80, 1, fp);
		if(tw[wnum].x+tw[wnum].lx*8>MaxX+1) {
			tw[wnum].x=16;
			tw[wnum].lx=(MaxX-tw[cur_w].x+1)/8;
		}
		if((tw[wnum].y+tw[wnum].ly)<<3>MaxY+1) {
			tw[wnum].y=0;
			tw[wnum].ly=(MaxY+1)/8-tw[cur_w].y;
		}
		if(tw[wnum].prn_range[0]==0 && tw[wnum].prn_range[1]=='0') {
			setmem(tw[wnum].prn_range,   2, 0xff);
			setmem(tw[wnum].prn_range+2, 2, 0);
		}
		if(HEADLEN(wnum)=='0') HEADLEN(wnum)=20;
		if(FOOTLEN(wnum)=='0') FOOTLEN(wnum)=20;
		if(LGAP_MUL(wnum)=='0') LGAP_MUL(wnum)=1;
	}
	return 1;
}

void vWriteTextData(BYTE wnum, FILE *fp)
{
	NODE *temp;

	temp=SNode;
	while(temp->next!=0) {
		temp=temp->next;
		fputc(temp->kd, fp);
		putw(temp->bn, fp);
		fwrite(temp->lc, 1, 6, fp);
		fwrite(temp->hs, 1, temp->lc[5], fp);
		if(temp->mh==0) {
			fwrite(temp->line, 1, temp->bn, fp);
		}
		else {
			if(iSwapMem(0, temp->bn, &(temp->mh), our_buf)!=1) {
				vErrorMsgfDlg(1, 2, 0);
				fseek(fp, (long)(temp->lc[5]+6+2+1), SEEK_CUR);
				break;
			}
			fwrite(our_buf, 1, temp->bn, fp);
		}
	}
	fputc(0, fp);
	putw(0, fp);
	fwrite(&(tw[wnum].x), sizeof(text_window)-80, 1, fp);
}

void vChangeExt(BYTE *str, BYTE *dest, BYTE *ext)
{
	fnsplit(str, g_drive, g_dir, g_file, g_ext);
	fnmerge(dest, g_drive, g_dir, g_file, ext);
}

int iRenameFile(BYTE flag, BYTE *fname, BYTE *ext)
{
	BYTE tname[80];

	vChangeExt(fname, tname, ext);
	if(flag==1)	{
		unlink(fname);
		if(rename(tname, fname)!=0) return 0;  // autosave
	}
	else {
		unlink(tname);
		if(rename(fname, tname)!=0) return 0;  // back up
	}
	return 1;
}

static int iWriteText(BYTE *fname)
{
	FILE *fp;

	iMakeMem();
	if((fp = fopen(fname,"wb+")) == NULL) {
		vErrorMsgfDlg(0, 2, fname);
		return 0;
	}
	sprintf(g_dir, "%s%s", the21, gwp_data);
	fwrite(g_dir, 48, 1, fp);
	fwrite(nicks[cur_w], 20, 1, fp);
	fwrite(password, 10, 1, fp);
	fwrite(our_buf, 30, 1, fp);
	vWriteTextData(cur_w, fp);
	if(ferror(fp)) {
		vErrorMsgfDlg(0, 5, "—¡¯aÇa ¶wœ· ¦¹¢");
		fclose(fp);
		return 0;
	}
	fclose(fp);
	tw[cur_w].modify=ASV_MODIF=OFF;
	return 1;
}

static int iSave(BYTE *fname)
{
	if(asave) {
		if(ASV_MODIF==0 && tw[cur_w].modify) {
			if(iRenameFile(1, fname, ".ASV")) {
				tw[cur_w].modify=OFF;
				ASV_MODIF=OFF;
				return 1;
			}
		}
	}
	return iWriteText(fname);
}

extern unsigned KStbl[2351];

#include "spetbl.h"
extern unsigned hanja_table[472][2];

#define ENG		0
#define KSC		1
#define SBC 	2

WORD wK2C (WORD in_code)
{
	WORD	npos;
	BYTE 	hi, lo, ks_base, cb_base;
	int 	_CH, _CL;

	hi = (BYTE)(in_code>>8);
	lo = (BYTE)(in_code);

	if (in_code > 0xcaa0) {	
		ks_base = 0xca;
		cb_base = 0xe0;
	}
	else { 
		ks_base = 0xa1;
		cb_base = 0xd9;
	}
	npos = ((hi - ks_base) * (0x60-2)) + (lo - 0xa1);
	_CH = cb_base + npos / (0xc0-4);
	_CL = 0x31 + npos % (0xc0-4);
	if (_CL >= 0x7f) _CL += 0x12;
	return (_CH<<8) | _CL;
}

WORD wC2K (WORD in_code)
{
	WORD npos;
	BYTE hi, lo, ks_base, cb_base;

		hi = (BYTE)(in_code>>8);
		lo = (BYTE)(in_code);
		if (lo > 0x90)
			lo -= 0x12;

		if (hi == 0xdf)
			return (lo > 0x8e) ? (0xfea1 + lo - 0x81) : (0xc9a1 + lo - 0x31);

		if (in_code > 0xe030) {
			cb_base = 0xe0;
			ks_base = 0xca;
		}
		else {
			cb_base = 0xd9;
			ks_base = 0xa1;
		}
		npos = ((hi - cb_base) * (0xc0-4)) + (lo - 0x31);
		return ((ks_base + npos / (0x60-2)) << 8)
			+ (0xa1 + npos % (0x60-2));
}

static int iCodeCmp(const unsigned *p, const unsigned *q)
{
	 return (*p - *q);
}

static unsigned wTG2KS(WORD code, BYTE code2)
{
	 WORD		index;
	 unsigned	*p;
	 INKEY hc;

	 hc.i=code;
	 if(code2&B6){
		 if(code2&B5){
//			 if(hc.ch[1]&0x78 && (hc.ch[0]&0x02 && hc.ch[0]&0x50)){
				 p = (unsigned *)bsearch(&hc.i, KStbl,	sizeof(KStbl) /	sizeof(unsigned),
						sizeof(unsigned),(int(*)(const void *, const void *))iCodeCmp) ;
				 if(p==NULL) return 0;
				 index =(WORD)(p - KStbl)-1;
				 return (((index / 94) + 0xb0) << 8) + (index % 94) + 0xa1;

		 }
		 else{
			 if(code2&B4){   //japan
				 if(hc.ch[0]>83){
					 hc.ch[0]-=83;
					 hc.ch[1]=0xab;
				 }
				 else hc.ch[1]=0xaa;
				 hc.ch[0]+=0xa0;
				 return hc.i;
			 }
			 else{  //specode!!
				 hc.ch[1]=code2;
				 code2=code2&0x0f;
				 if(code2==4 || code2==7){
					 if(code2==4) hc.ch[1]=0xa4;
					 else hc.ch[1]=0xa6;
					 hc.ch[0]=hc.ch[0]+0xa0-0x20;
					 return hc.i;
				 }
				 else{
					 hc.ch[0]-=32;
//					 p = (unsigned *)bsearch(&hc.i, spetbl,	sizeof(spetbl) /	sizeof(unsigned),
//									sizeof(unsigned),(int(*)(const void *, const void *))iCodeCmp) ;
					 for(index=0; index<570; index++){
						 if(spetbl[index]==hc.i)break;
					 }
					 if(index>569) return 0;
//					 if(p==NULL) return 0;
//					 index = (WORD)(p - spetbl)-1;
					 if(index>(96*5)-1)index+=(96*6);
					 else if(index>(96*3)-1)index+=(96*4);
					 else if(index>(96*2)-1)index+=(96*2);
					 return (((index / 96) + 0xa1) << 8) + (index % 96) + 0xa0;
				 }
			 }
		 }
	 }
	 else{ // hanja;
		 code2=code2&0x7f;
		 for(index=0; index<472; index++){
			 if(hanja_table[index][0]==hc.i)break;
		 }
		 if(index<472){
			 index=hanja_table[index][1]+code2;
			 hc.ch[1]=index/94+0xca;
			 hc.ch[0]=index%94+0xa0+1;
			 return hc.i;
		 }
	 }
	 return 0;
}

static BYTE line_conv_table[11]={"+-+|+|+-+-|"};

int iWriteTXT(BYTE *fname, BYTE code, BYTE way)
{
	FILE *fp;
	NODE *temp;
	WORD i, t;
	BYTE lnum, c, flag , code2;
	INKEY kc;
	int ex_err;

	if((fp = fopen(fname,"wb+")) == NULL) {
		vErrorMsgfDlg(0, 1, fname);
		return 0;
	}
	temp=SNode;

	while(temp->next!=0) {
		i=0;
		temp=temp->next;
		_CHECKNODE(temp);
		if(ex_err==0) {
			vErrorMsgfDlg(1, 0, 0);
			return 0;
		}
		for(lnum=0;lnum<temp->lc[5];lnum++) {
			t=0;
			flag=0;
			if(lnum==0) {
				for(c=0;c<(int)((char)temp->lc[1]);c++) *(our_buf+t++)=32;
			}
			for(c=1;c<temp->lc[2] && !way;c++) *(our_buf+t++)=32;
			while(1) {
				if(temp->line[i]>127) {
					if(temp->line[i+2]>127 && flag && !way) {
						*(our_buf+t++)='\r';
						*(our_buf+t++)='\n';
						break;
					}
					flag=ON;
					kc.ch[1]=temp->line[i];
					kc.ch[0]=temp->line[i+1];
					code2=temp->line[i+4];
					if(code==1) {
						kc.i=wTG2KS(kc.i, code2);
						if(kc.i==0){
UNKNOWN:
							*(our_buf+t++)='[';
							*(our_buf+t++)=']';
						}
						else{
							*(our_buf+t++)=kc.ch[1];
							*(our_buf+t++)=kc.ch[0];
						}
					}
					else {
						if(code2<0xe0){
							kc.i=wTG2KS(kc.i, code2);
							if(kc.i==0) goto UNKNOWN;
							kc.i=wK2C(kc.i);
						}
						*(our_buf+t++)=kc.ch[1];
						*(our_buf+t++)=kc.ch[0];
					}
					i+=5;
				}
				else {
					if(temp->line[i]==0) {
						if(save_null) *(our_buf+t++)=32;
						i++;
						flag=ON;
						continue;
					}
					if(temp->line[i]=='\r') {
						*(our_buf+t++)='\r';
						*(our_buf+t++)='\n';
						i=0;
						lnum=temp->lc[5];
						break;
					}
					if(temp->line[i+1]>127 && flag && !way) {
						*(our_buf+t++)='\r';
						*(our_buf+t++)='\n';
						break;
					}
					flag=ON;
					if(temp->line[i+3]==0x23) {
						*(our_buf+t++)=line_conv_table[((temp->line[i]-33)%11)];
					}
					else *(our_buf+t++)=temp->line[i];
					i+=4;
				}
			}
			fwrite(our_buf, t, 1, fp);
		}
	}
	fclose(fp);
	tw[cur_w].modify=ASV_MODIF=OFF;
	return 1;
}
int iSaveScreen(BYTE *fname)
{
	FILE *out;

	if((out=fopen(fname,"wb+"))==0) {
		vErrorMsgfDlg(0, 2, fname);
		return 0;
	}
	fwrite(vid_addr, work_area_size,1,out);
	fclose(out);
	return 1;
}

int iRestScreen(BYTE *fname)
{
	FILE *in;

	if((in=fopen(fname,"rb"))==0) {
		vErrorMsgfDlg(0, 1, fname);
		return 0;
	}
	fread(our_buf, work_area_size,1,in);
	fclose(in);
	remove(fname);
	return 1;
}

void vSwapTextWindowArea(BYTE save_load, BYTE n)
{
	if(save_load) {
		vGetImage(tw[n].x, tw[n].y, tw[n].x+(tw[n].lx<<3), tw[n].y+tw[n].ly-1,
							our_buf);
		if(!iSwapMem(MSWRITE, (tw[n].lx*(tw[n].ly))<<3, &(tm[n]), our_buf))
			vErrorMsgfDlg(1, 2, 0);
	}
	else {
		if(!iSwapMem(MSREAD, (tw[n].lx*(tw[n].ly))<<3, &(tm[n]), our_buf))
			vErrorMsgfDlg(1, 2, 0);
		vPutImage(tw[n].x, tw[n].y, tw[n].x+(tw[n].lx<<3), tw[n].y+tw[n].ly-1,
							our_buf);
	}
}


int iSwapDocTofile(BYTE wnum)
{
	FILE *fp;

	sprintf(g_dir, "%s\\editswp%d.swp", swp_path, wnum);
	if((fp = fopen(g_dir,"wb+")) == NULL) {
		vErrorMsgfDlg(0, 1, "¯aµÓ");
		return 0;
	}
	sprintf(g_dir, "%s%s", the21, swp_data);
	fwrite(g_dir, 48, 1, fp);
	fwrite(nicks[wnum], 20, 1, fp);
	fwrite(password, 10, 1, fp);
	fwrite(our_buf, 30, 1, fp);
	vWriteTextData(wnum, fp);
	fclose(fp);
//	file_pbuf_flag(ON);
	return 1;
}

int iSwapFileToDoc(BYTE wnum)
{
	FILE *fp;
	int rtn;

	sprintf(g_dir, "%s\\editswp%d.swp", swp_path, wnum);
	if((fp = fopen(g_dir,"rb")) == NULL) {
    vErrorMsgfDlg(0, 1, "¯aµÓ");
		return 0;
	}
	if(!iCheckGWP(1, fp, swp_data)) {
		fclose(fp);
		vErrorMsgfDlg(0, 5, "¯aµÓ");
		vExit();
	}
	vInitLoad();
	rtn=iReadTextData(2, wnum, fp);
	fclose(fp);
	return rtn;
}

void vSetNewTextWindow(void)
{
	if(tw[cur_w].active) {
		if(!iSwapDocTofile(cur_w)) return;
		vDarkenTextWindowTitle(cur_w);
		vSwapTextWindowArea(1, cur_w);	// deference to XT version
	}
	vResetNodeList();
}

void vNewTextWindow(void)
{
	BYTE wnum;

	if(!tw[cur_w].active || SNode->next->next || SNode->next->bn>1) {
		wnum=iGetWindowHandle();
		if(wnum==0xff) {
			vErrorMsgfDlg(10, 1, 0);
			return;
		}
		vSetNewTextWindow();
		cur_w=wnum;
		vPushWindow(cur_w);
		vResetTextWindowValue(cur_w, 1);
		vDrawTextWindow(cur_w);
	}
	vSetCurrent();
	tw[cur_w].active=ON;
	tw[cur_w].bl=0;
}

int iReadGWP(BYTE flag, BYTE *fname)
{
	FILE *fp;
	int rtn;

	if((fp = fopen(fname,"rb")) == NULL) {
		vErrorMsgfDlg(0, 1, fname);
		return 0;
	}
	if(!iCheckGWP(1, fp, gwp_data)) {
		fclose(fp);
		return 2;
	}
	vInitLoad();
	strcpy(nicks[cur_w], nick_name);
	rtn=iReadTextData(flag, cur_w, fp);
	fclose(fp);
	return rtn;
}

int iReadLineTXT(BYTE *vExit, int code, BYTE *line_buf, FILE *in)
{
	int i, loc=0, index;
	INKEY hc;
	int ch;

	(*vExit)=0;
	setmem(line_buf, 25001, 0);

	do {
		ch=fgetc(in);
		if(ch=='\r' || ch==10 || loc>25000) {
NEWTEXT:
			line_buf[loc++]='\r';
			ch=fgetc(in);
			if(ch!=10) ungetc(ch, in); // when character is '\n'
			break;
		}
		if(ch==0xff) ch=32;
		if(ch>127 && code) { // code is HANGUL
			hc.ch[1]=ch;
			hc.ch[0]=fgetc(in);
			if(code==KSC) {
KSCCODE:
				if(hc.ch[1]>0xa0 && hc.ch[1]<0xad) { //spacial code!!
					if(hc.ch[1]==0xa3){
						line_buf[loc++]=hc.ch[0]-0xa0+32;
						line_buf[loc++]=0;
						line_buf[loc++]=0x12;
						line_buf[loc++]=0x40;
					}
					else if(hc.ch[1]==0xa7){
SPECODE:
						line_buf[loc++]=0xff;
						line_buf[loc++]=0x46;
						line_buf[loc++]=0;
						line_buf[loc++]=0x11;
						line_buf[loc++]=0xc6;
					}
					else {
						if(hc.ch[1]==0xa4 || hc.ch[1]==0xa6){
							if(hc.ch[1]==0xa4)hc.ch[1]=0xc4;
							else hc.ch[1]=0xc7;
							hc.ch[0]=hc.ch[0]-0xa0+32;
						}
						else if(hc.ch[1]==0xaa || hc.ch[1]== 0xab){
							if(hc.ch[1]==0xaa){
								if(hc.ch[0]>0xa0 && hc.ch[0]<0xf4)
									hc.ch[0] = hc.ch[0]-0xa0;
								else goto SPECODE;
							}
							else{
								if(hc.ch[0]>0xa0 && hc.ch[0]<0xf7)
									hc.ch[0] = hc.ch[0]-0xa0+83;
								else goto SPECODE;
							}
							hc.ch[1]=0xd0;
						}
						else {
							index = (WORD)(hc.ch[1]-0xa1)*96+hc.ch[0]-0xa0;
							if(hc.ch[1]>0xa4){
								index=index-(96*2);
								if(hc.ch[1]>0xa7){
									index=index-(96*2);
									if(hc.ch[1]>0xab)index=index-(96*2);
								}
							}
							hc.i=spetbl[index]+32;
							if(index > 576 || hc.ch[1] == 0) goto SPECODE;
						}
						line_buf[loc++]=0xff;
						line_buf[loc++]=hc.ch[0];
						line_buf[loc++]=0;
						line_buf[loc++]=0x11;
						line_buf[loc++]=hc.ch[1];
					}
				}
				else if(hc.ch[1]>=0xca && hc.ch[0]>0xa0) { // KS hanja
					index = (WORD)(hc.ch[1] - 0xca) * 94 + hc.ch[0] - 0xa1;
					for(i=0;i<472;i++) {
						if(hanja_table[i][1]>index) break;
					}
					if(i<472) {
						index=index-hanja_table[i-1][1];
						line_buf[loc++]=hanja_table[i-1][0]>>8;
						line_buf[loc++]=hanja_table[i-1][0] & 0xff;
						line_buf[loc++]=0;
						line_buf[loc++]=0x11;
						line_buf[loc++]=0x80 + index;
					}
					else goto SPECODE;
				}
				else { // hangul
					if(hc.ch[1]<0xb0 || hc.ch[0]<0xa1) goto SPECODE;
					index = (WORD)(hc.ch[1] - 0xb0) * 94 + hc.ch[0] - 0xa1;
					if(index<2350)	hc.i=KStbl[index+1];
					else goto SPECODE;
HANINTO:
					line_buf[loc++]=hc.ch[1];
					line_buf[loc++]=hc.ch[0];
					line_buf[loc++]=0;
					line_buf[loc++]=0x11;
					line_buf[loc++]=0xe0;
				}
			}
			else {
				if(hc.i<0xd930) goto HANINTO;
				else{
				  hc.i=wC2K(hc.i);
					goto KSCCODE;
				}
			}
		}
		else {	// code is ASCII
			if(ch==-1) {
				(*vExit)=1;
				if(loc) goto NEWTEXT;
				else break;
			}
			if(ch==9) {	// tab character
				for(i=0;i<load_tab;i++) {
					line_buf[loc++]=32;
					line_buf[loc++]=0;
					line_buf[loc++]=0x11;
					line_buf[loc++]=0x40;
				}
			}
			else {		//
				if(ch==0x1a) {	// character is CTRL_Z
					if(!loc) line_buf[loc++]='\r';
					(*vExit)=1;
					break;
				}
				if(ch<32) ch=32;
				if(ch>127) { // if code ASCII and...
					ch=ch+32-128;
					if(ch<128) {
            line_buf[loc++]=ch;
						line_buf[loc++]=0;
						line_buf[loc++]=0x11;
						line_buf[loc++]=0x24;
					}
					else {
						line_buf[loc++]=ch-128+32;
						line_buf[loc++]=0;
						line_buf[loc++]=0x11;
						line_buf[loc++]=0x34;
					}
				}
				else {
					line_buf[loc++]=ch;
					line_buf[loc++]=0;
					line_buf[loc++]=0x11;
					line_buf[loc++]=0x40;
				}
			}
		}
	} while(ch!=-1);
	return loc;
}

BYTE bGetHanjaIndex(WORD i, WORD *t)
{
	register WORD j;

	for(j=0;j<472;j++) {
		if(hanja_table[j][1]>i) {
			*t=hanja_table[j-1][0];
			return i-hanja_table[j-1][1];
		}
	}
	return 0;
}

int iReadLineHWP(BYTE *vExit, BYTE *lc, BYTE *line, BYTE *effline, FILE *in)
{
	WORD tword;
	WORD bn=0;

	int SNode, second;
	BYTE code2, effect, teffect, sys;
	WORD count=0, count1=0, setcount=0;
	int num;

	(*vExit)=0;
	tword=getw(in);
	if(!tword) return bn;
	tword=fgetc(in);
	lc[0]=(BYTE)getw(in);
	if(lc[0]>20) lc[0]=20;
	lc[1]=(char)((int)getw(in));
	lc[2]=(BYTE)getw(in)+1;
	if(lc[2]>20) lc[2]=20;
	lc[3]=(BYTE)getw(in)+1;
	if(lc[3]>240) lc[3]=240;
	lc[5]=1;
	fread(line, 9, 1, in);
	setmem(effline, our_buf_size-10000, 0);
	setmem(line, 10000, 0);
	while(1) {
		num=fgetc(in);
		if(num==-1) {
			(*vExit)=1;
			break;
		}
		if(!num) break;
		teffect=fgetc(in);
		effect=teffect & 0xc0;
		if(teffect & 0x20) effect|=0x01;
		if(teffect & 0x10) effect|=0x02;
		if(teffect & 0x08) effect|=0x04;
		if(teffect & 0x04) effect|=0x08;
		if(teffect & 0x02) effect|=0x10;
		if(teffect & 0x01) effect|=0x20;

		code2=fgetc(in) & 0x7f;
		for(count=0;count<num*2;) {
			effline[setcount+count++]=effect;
			if(code2>5) code2=0;
			effline[setcount+count++]=code2;
		}
		setcount+=num*2;
	}
	while(1) {
		second=getc(in);
		SNode=getc(in);
		if(!SNode && !second) {

			count1+=2;
			continue;
		}
		if(SNode==-1 || second==-1) {
			(*vExit)=1;
			break;
		}
		if(SNode>127) {
			line[bn++]=SNode;
			line[bn++]=second;
			line[bn++]=effline[count1] & 0x3f;
			if(effline[count1] & 0x40) sys=0x20;
			else sys=0x10;
			if(effline[count1] & 0x80) sys|=0x02;
			else sys|=0x01;
			line[bn++]=sys;
			if(effline[count1+1]>3) effline[count1+1]=0;
			line[bn++]=0xe0 | effline[count1+1];
		}
		else if(SNode & 0x40) {
			SNode=bGetHanjaIndex((((SNode & 0x3f)<<8) | second), &tword);
			line[bn++]=tword >> 8;
			line[bn++]=tword & 0x00ff;
			line[bn++]=effline[count1] & 0x3f;
			if(effline[count1] & 0x40) sys=0x20;
			else sys=0x10;
			if(effline[count1] & 0x80) sys|=0x02;
			else sys|=0x01;
			line[bn++]=sys;
			line[bn++]=0x80 | SNode;
		}

		else {
			if(second>127) {
				if(second>0xb2) second=(second-0xb2)+32;
				else second=32;
				line[bn++]=second;
				line[bn++]=effline[count1] & 0x3f;
				if(effline[count1] & 0x40) sys=0x20;
				else sys=0x10;
				if(effline[count1] & 0x80) sys|=0x02;
				else sys|=0x01;
				line[bn++]=sys;
				line[bn++]=0x23;
			}
			else {
				if(SNode==1 && (second==7 || second==8)) second='\'';
				else if(second==2 || second==3) second=34;
				else if(second!='\r' && second<32) second=32;
				line[bn++]=second;
				if(second=='\r') break;
				line[bn++]=effline[count1] & 0x3f;
				if(effline[count1] & 0x40) sys=0x20;
				else sys=0x10;
				if(effline[count1] & 0x80) sys|=0x02;
				else sys|=0x01;
				line[bn++]=sys;
				if(effline[count1+1]>5) effline[count1+1]=0;
				line[bn++]=0x40 | effline[count1+1];
			}
		}
		count1+=2;
	}
	return bn;
}

int iReadLoop(BYTE func, BYTE flag, BYTE code, FILE *in)
{
	int len;
	BYTE *str;
	BYTE *ths;
	BYTE lc[6];
	NODE *temp;
	BYTE starting=1;
	BYTE vExit;

	if(func) {
		vResetTextWindowValue(cur_w, 1);
		vInitLoad();
	}
	memcpy(lc, load_lc, 6);
	temp=CNode;
	if(!flag) {
		fread(our_buf, 158, 1, in); //flag == 0 is hwp else txt
		if(our_buf[122]!=0 || our_buf[123]!=0) {
			vErrorMsgfDlg(0, 0, "´qÑ¡ˆa ˆéŸ¥ ÑÁ·©·³“¡”a.");
			return -1;
		}
	}
	while(1) {
		if(flag) len=iReadLineTXT(&vExit, code, our_buf, in);
		else len=iReadLineHWP(&vExit, lc, our_buf, our_buf+10000, in);
		if(!len) {
			if(starting) return -1;
			return 1;
		}
		else {
			if(our_buf[len-1]!='\r') our_buf[len++]='\r';
		}
		str=_GETLINE(len);
		if(!str) {
			vErrorMsgfDlg(1, 0, 0);
			if(starting) return -1;
			return 0;
		}
		memcpy(str, our_buf, len);
		ths=_GETHS(1);
		if(!ths) {
			free(str);
			vErrorMsgfDlg(1, 0, 0);
			if(starting) return -1;
			return 0;
		}
		ths[0]=0x10;
		if(func) {
			if(!iAppendNode(str, lc, ths, len, 0)) {
				vErrorMsgfDlg(1, 0, 0);
				if(starting) return -1;
				return 0;
			}
			vAnalText(ENode);
		}
		else {
			if(!iInsertNode(temp, str, lc, ths, len, 0)) {
				vErrorMsgfDlg(1, 0, 0);
				if(starting) return -1;
				return 0;
			}
			temp=temp->next;
			vAnalText(temp);
			tw[cur_w].modify=ON;
		}
		if(vExit) return 1;
		starting=0;
	}
}

int iReadTXT(BYTE flag, BYTE code, BYTE way, BYTE *fname)
{
	int load_ok;
	FILE *fp;

	memcpy(load_lc, def_lc, 6);
	if(!way) {
		load_lc[1]=0;
		load_lc[2]=1;
		load_lc[3]=240;
	}
	if((fp = fopen(fname,"rb")) == NULL) {
		vErrorMsgfDlg(0, 1, fname);
		return 0;
	}
	load_ok=iReadLoop(flag, 1, code, fp);
	fclose(fp);
	return load_ok;
}

int iReadHWP(BYTE flag, BYTE *fname)
{
	int load_ok;
	FILE *fp;

	if((fp = fopen(fname,"rb")) == NULL) {
		vErrorMsgfDlg(0, 1, fname);
		return 0;
	}
	load_ok=iReadLoop(flag, 0, 0, fp);
	fclose(fp);
	return load_ok;
}

int iGetFileType(BYTE *code, BYTE *way, BYTE *fname)
{
	FILE *fp;
	BYTE type;

	if((fp = fopen(fname,"rb")) != NULL) {
		type=iCheckGWP(1, fp, gwp_data);
		if(type==0) {
			type=iCheckHWP(fp);
			if(type==0) {
				if(!iLoadOptDlg(code, way)) {
					fclose(fp);
					return -1;
				}
			}
		}
		else type=3;
		fclose(fp);
		return type;
	}
	return -1;
}

BYTE open_twin_old_fname[80]={"*.GWP"};

void vOpenTextDlg(BYTE flag)
{
	BYTE fname[80];
	int result=1;
	BYTE wnum;
	int type;
	BYTE code=0, way=0;
	int ok;

	vSetExt(".GWP");
	if(flag) result=iFileListDlg(open_twin_old_fname, fname, 0);
	else strcpy(fname, path_buf);
	if(result>0) {
		type=iGetFileType(&code, &way, fname);
		if(type==-1) return;
		if(result==1) {
			if(!tw[cur_w].active || SNode->next->next || SNode->next->bn>1) {
				wnum=iGetWindowHandle();
				if(wnum==0xff) {
					vErrorMsgfDlg(10, 1, 0);
					return;
				}
				vSetNewTextWindow();
				cur_w=wnum;
			}
			else {
				vRedrawAllWindow();
				iPopWindow();
			}
			vResetTextWindowValue(cur_w, 1);
		}
		else {
			iCloseTextWindow(0);
//			vSetNewTextWindow();
		}
		switch(type) {
			case 0: ok=iReadTXT(1, code, way, fname); break;
			case 1: case 2: ok=iReadHWP(1, fname); break;
			case 3: ok=iReadGWP(2, fname); break;
			default : ok=-1;
		}
		if(ok==-1) {
			vResetNodeList();
			vResetTextWindowValue(cur_w, 1);
		}
		tw[cur_w].modify=ASV_MODIF=OFF;
		strcpy(tw[cur_w].headline, fname);
		vPushWindow(cur_w);
		vDrawTextWindow(cur_w);
		vSetCurrent();
		tw[cur_w].active=ON;
		vSetPaper();
		vDividePage(1);
	}
	return;
}

void vOpenAgain(BYTE *name)
{
	strcpy(path_buf, name);
	vOpenTextDlg(0);
}

extern BYTE Save_buf[];

int iIsOverWrite(BYTE *fname)
{
	if(bCheckFile(fname)==0) {
		if(ihUserChoiceDlg("Ìa·©·¡ ·¶¯s“¡”a. ”ü´á³iŒa¶a?")==0) {
			iRenameFile(0, fname, ".BAK");
			return 1;
		}
		else return 0;
	}
	else return 1;
}

int iSaveAsDlg(void)
{
	BYTE fname[80], fname2[80];
	BYTE code, way;

	if(!tw[cur_w].active || cur_w==0) {
		vBeep();
		return 0;
	}
	strcpy(Save_buf+3, nicks[cur_w]);
	if(iSaveOptDlg(&code, &way)!=1) return 0;
	strcpy(fname2, tw[cur_w].headline);
	if(code==0) {
		vSetExt(".GWP");
		if(iFileListDlg(fname2, fname, 1)==1) {
			Save_buf[23]=0;
			strcpy(nicks[cur_w], Save_buf+3);
			Save_buf[80+10+3]=0;
			strcpy(password, Save_buf+80+3);
			if(iIsOverWrite(fname)==0) return 0;
			iSave(fname);
			strcpy(tw[cur_w].headline, fname);
			vEraseOldFileName();
		}
		else return 0;
	}
	else {
		if(code==1) vSetExt(".KSC");
		else if(code==2) vSetExt(".TXT");
		else return 0;
		if(iFileListDlg(fname2, fname, 1)==1) {
			if(iIsOverWrite(fname)==0) return 0;
			iWriteTXT(fname, code, way);
			strcpy(tw[cur_w].headline, fname);
			vEraseOldFileName();
		}
		else return 0;
	}
	vResetTextWindowTitle();
	return 1;
}

int iSaveTextWindow(void)
{
	if(!tw[cur_w].active || cur_w==0) {
		vBeep();
		return 0;
	}
	iFnSplit(tw[cur_w].headline);
	if(strcmp(g_ext, ".GWP")!=0) {
		switch(iUserChoiceDlg("ÑÂ¸w¸aŸi GWP¡ ¤aŽ‰Œa¶a?")) {
			case -1: return 0;
			case  0:
				strcpy(g_ext, ".GWP");
				fnmerge(tw[cur_w].headline, g_drive, g_dir, g_file, g_ext);
				vResetTextWindowTitle();
				break;
		}
	}
	strcpy(g_dir, tw[cur_w].headline);
	g_dir[7]=0;
	if(strcmp(g_dir, "NONAME0")==0) {
		return iSaveAsDlg();
	}
	else {
		if(mk_bak) iRenameFile(0, tw[cur_w].headline, ".BAK");
		return iSave(tw[cur_w].headline);
	}
}

void vAutoSave(void)
{
	static unsigned long old;
	BYTE tmp[80];

	if(asave==0 || cur_w==0) return;
	if(aflag==0) {
		old=Scount;	aflag=1; return;
	}
	if(tw[cur_w].active==0 || ASV_MODIF==0) {
		aflag=0;	return;
	}
	if((Scount-old)/18+(Scount-old)/90<as_time) return;
	saving=1; no_work=0;
	vChangeExt(tw[cur_w].headline, tmp, ".ASV");
	ASV_MODIF=aflag=OFF;
	iWriteText(tmp);
	tw[cur_w].modify=ON;
	saving=0;
}

void vNewText(void)
{
	int i;

	if(!tw[cur_w].active) {
		vNewTextWindow();
		return;
	}
	if(tw[cur_w].modify && cur_w!=0) {
		i=iUserChoiceDlg("¸á¸w·i Ða¯¡‰V·s“¡Œa?");
		if(i==0) {
			if(iSaveTextWindow()!=1) return;
		}
		else if(i==-1) return;
	}
	vResetTextWindowValue(cur_w, 0);
	vResetTextWindowTitle();
	vResetNodeList();
	vDrawTopMenu();
//	vRedrawAllWindow();
	vDrawTextWindow(cur_w);
	tw[cur_w].active=ON;
}
void vKeyWait(void)
{
	while(!wCheckKeyHit());
	vResetKeyBuffer();
}

void vDosShell(void)
{
	BYTE err=0;
	char *comspec;
	char new_prompt[60]={"type 'EXIT' to return to 21st Century...$_$_"};
	char *old_prompt;
	char *temp;

	vMCOFF();
	if(tw[cur_w].active) {
		if(iSwapDocTofile(cur_w)==0) return;
	}
	vClearNodeList();
	iFreeFontBuffer(0);
//	copy_screen_to_mem();
	vFreeOurBuf();
	sprintf(g_dir, "%s\\SCREEN.SWP", swp_path);
	iSaveScreen(g_dir);

	vDeinitMouse();
	vDeinitINT8();
	vDeinitGraphMode();
	temp=(char *)malloc(320);
	if(!temp) {
		err=ON;
		goto OUT;
	}
	comspec=(char *)getenv("COMSPEC");
	old_prompt=(char *)getenv("PROMPT");
	strcpy(temp, "PROMPT=");
	strcat(temp, new_prompt);
	strcat(temp, old_prompt);
	putenv(temp);
	dosing=ON;
//	printf("Free memory is %lu\n", farcoreleft());
	if(spawnlp(P_WAIT, comspec, comspec, NULL) == -1) {
		printf(" Cannot found 'COMMAND.COM' or Not enough memory!!!\n");
		printf(" Press any key!!!\n");
		vKeyWait();
	}
	strcpy(temp, "PROMPT=");
	strcat(temp, old_prompt);
	putenv(temp);
OUT:
	dosing=OFF;
	iInitGraphMode(graph_mode, 1);
	if(iInitMouse(mouse_kind, mouse_port)==100) vExit();
	vInitINT8();
	iInitDefaultFont();

	vMakeOurBuf();
	sprintf(g_dir, "%s\\SCREEN.SWP", swp_path);
	iRestScreen(g_dir);
	if(color_mode) vSetColors();
	memcpy(vid_addr, our_buf, work_area_size);
	vInitNodeList();
	if(tw[cur_w].active) {
		iSwapFileToDoc(cur_w);
		vSetCurrent();
	}
	if(err) vErrorMsgfDlg(1, 0, 0);
	vEraseOldFileName();
}

#define MAXSTAR 40

void vScreenSaver1(void)
{
	int x[MAXSTAR];
	int y[MAXSTAR];
	int flag[MAXSTAR];
	int sx[MAXSTAR];
	int sy[MAXSTAR];
	int i;
	int centerx, centery;
	long dd;
	BYTE spflag=1, faster=0;
	int speedson=20;

	vChangeColor(15, 63);
	centerx=MaxX/2;
	centery=MaxY/2;
	setmem(flag, MAXSTAR*2, 0);
	while(no_work && !wCheckKeyHit()) {
		if(spflag) {
			if(random(2000)>1998-faster*2) {
				faster^=1;
				spflag=0;
			}
		}
		for(i=0;i<MAXSTAR;i++) {
			if(flag[i]==0) {
				if(i<MAXSTAR/5) {
					do {
						sx[i]=random(MaxX/7)+(MaxX*3)/7-centerx;
					} while(abs(sx[i])<2);
					do {
						sy[i]=random(MaxY/7)+(MaxY*3)/7-centery;
					} while(abs(sy[i])<2);
					if(i==0) {
						if(faster) {
							speedson--;
							if(speedson<2) {
								speedson=2;
								spflag=1;
							}
						}
						else {

							speedson++;
							if(speedson>20) {
								speedson=20;
								spflag=1;
							}
						}
					}
				}
				else {
					do {
						sx[i]=random((MaxX*2)/3)+MaxX/6-centerx;
					} while(abs(sx[i])<2);
					do {
						sy[i]=random((MaxY*2)/3)+MaxY/6-centery;
					} while(abs(sy[i])<2);
				}
				flag[i]=1;
			}
		}
		for(i=0;i<MAXSTAR;i++) {
			vPointNormal(centerx+x[i], centery+y[i], 4);
			if(abs(sx[i])>abs(sy[i])) {
				if(sx[i]>0) {
					x[i]=sx[i]+(int)(((long)flag[i]*flag[i]*flag[i]/(abs(sx[i])/4+1))/10)+(flag[i]+1)*3;
				}
				else {
					x[i]=sx[i]-(int)(((long)flag[i]*flag[i]*flag[i]/(abs(sx[i])/4+1))/10)-(flag[i]+1)*3;
				}
				y[i]=(int)(((long)sy[i]*x[i])/sx[i]);
			}
			else {
				if(sy[i]>0) {
					y[i]=sy[i]+(int)(((long)flag[i]*flag[i]*flag[i]/(abs(sy[i])/4+1))/10)+(flag[i]+1)*3;
				}
				else {
					y[i]=sy[i]-(int)(((long)flag[i]*flag[i]*flag[i]/(abs(sy[i])/4+1))/10)-(flag[i]+1)*3;
				}
				x[i]=(int)(((long)sx[i]*y[i])/sy[i]);
			}
			if(x[i]+centerx>MaxX-5 || x[i]+centerx<5
				 || y[i]+centery>MaxY-5 || y[i]+centery<5) {
				flag[i]=0;
			}
			else {
				if(abs(sx[i])>abs(sy[i])) {
					vPointThick(centerx+x[i], centery+y[i], flag[i]/(abs(sx[i])/15+3));
				}
				else {
					vPointThick(centerx+x[i], centery+y[i], flag[i]/(abs(sy[i])/15+3));
				}
				flag[i]++;
			}
		}
		for(dd=0;dd<(unsigned long)SYSPEED*speedson;dd++);
	}
}

BYTE est2[]={"2\0\x33\x40\x31\0\x33\x40­A\0\x33\xe0‹¡\0\x33\xe0\r"};
BYTE est3[]={"2\0\x66\x40\x31\0\x66\x40­A\0\x66\xe0‹¡\0\x66\xe0\r"};

extern BYTE *psstr;
#define pnum 5


static char near cRandomPoint(char vnum)
{
	char num;

	num = random(8)+1;
	if(vnum>0) return -num;
	else return num;
}

void vScreenSaver2(void)
{
	int lppx[6][pnum];
	char lppvx[pnum];
	int lppy[6][pnum];
	char lppvy[pnum];
	BYTE i, j;


	setmem(lppx, 5*pnum*2, 0);
	setmem(lppy, 5*pnum*2, 0);

	for(j=0; j<pnum; j++){
		lppx[0][j]=random(MaxX);
		lppy[0][j]=random(MaxY);
		lppvx[j]=random(50)+1;
		lppvy[j]=-(random(50)+1);
	}
	i=0;
	while(no_work && !wCheckKeyHit()){
		if(random(1000)==5) {
			j=random(8)+8;
			if(!(j/16)) j++;
			vChangeColor(15, j);
			i=0;
		}
		for(i=0; i<pnum; i++){
			if(0 > (lppx[0][i] + lppvx[i]) || MaxX < (lppx[0][i] + lppvx[i]))
				lppvx[i]=cRandomPoint(lppvx[i]);
			lppx[0][i]=	lppx[0][i] + lppvx[i];

			if(0 > (lppy[0][i] + lppvy[i]) ||  MaxY < (lppy[0][i] + lppvy[i]))
				lppvy[i]=cRandomPoint(lppvy[i]);
			lppy[0][i]=	lppy[0][i] + lppvy[i];
		}

//	scr_box(0, 1);
//  scr_box(5, 0);
		for(j=0; j<pnum-1; j++){
			vLineBit(lppx[0][j], lppy[0][j], lppx[0][j+1], lppy[0][j+1], 1);
		}
		vLineBit(lppx[0][pnum-1], lppy[0][pnum-1], lppx[0][0], lppy[0][0], 1);

		for(j=0; j<pnum-1; j++){
			vLineBit(lppx[5][j], lppy[5][j], lppx[5][j+1], lppy[5][j+1], 0);
		}
		vLineBit(lppx[5][pnum-1], lppy[5][pnum-1], lppx[5][0], lppy[5][0], 0);

		for(i=5; i>0; i--){
			memcpy(lppx[i], lppx[i-1], pnum*2);
			memcpy(lppy[i], lppy[i-1], pnum*2);
		}

	}
}


void vScreenSaver3(void)
{
	int x=0;
	int flag=0;
	BYTE old_full;
	BYTE old_return;
	long dd;
	int count=0;

	bminx=0;
	bminy=0;
	bmaxx=MaxX;
	bmaxy=MaxY;
	old_return=disp_return;
	old_full=full_vector;
	disp_return=0;
	full_vector=0;
	vSetDispOpt(disp_space, disp_newline, disp_return);
	while(no_work && !wCheckKeyHit()) {
		if(flag==0) {
			x=MaxX-10;
			flag=1;
			vChangeColor(15, random(63)+1);
			count=0;
		}
		if(count>8) {
			vBarByte(x-24, 22, x+72, MaxY/8, 0);
			x-=5;
		}
		else if(!count) vBarByte(x+43, 31, x+55, MaxY/8, 0);
		if(x>MaxX/2-2 && x<MaxX/2+2 && count<8) count++;
		else {
			x-=5;
			count=0;
		}
		if(x<-48) flag=0;
		else {
			if(x>MaxX/2-2 && x<MaxX/2+2) {
				psstr=est3;
				vPT(x-24, 22, 0x60);
				count++;
			}
			else if(!count) {
				psstr=est2;
				vPT(x, 31, 0x30);
			}
			flag++;
		}
		dd=Scount;
		while(Scount==dd);
	}
	full_vector=old_full;
	disp_return=old_return;
	vSetDispOpt(disp_space, disp_newline, disp_return);
	vClipOn(cur_w);
}

void vScreenSaver(int num)
{
	BYTE flag;
	Mem m=0;

	flag=mouse_on_off;
	vMCOFF();
	enable();
	saving=ON;
	if(!iSwapMem(MSWRITE, work_area_size, &m, vid_addr)) {
		vErrorMsgfDlg(1, 0, 0);
		return;
	}
	vFillScreen(0, 0);
	vSetBlackWhite();
	switch(num) {
		case 1 :
			while(no_work && !wCheckKeyHit());
			break;
		case 2 :
DEFAULT:
			vScreenSaver1();
			break;
		case 3 :
			vScreenSaver2();
			break;
		case 4 :
			vScreenSaver3();
			break;
		default : goto DEFAULT;
	}
	vSetColors();
	if(!iSwapMem(MSREAD, work_area_size, &m, vid_addr)) {
		vErrorMsgfDlg(1, 2, 0);
		vResetDesktop();
	}
	vFreeMem(&m);
	vResetKeyBuffer();
	if(flag) vMCON();
	saving=OFF;
}

#include "harderr.c"
