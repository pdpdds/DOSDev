/* #############################################################
		 ***** 도스쉘 : W d i r  beta. 1.01
	프로그램 만든이 : 백 종 옥
	컴파일 언어     : Borlandc 3.1++
	천리안 아이디   : kim1417
############################################################## */

#include <dos.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <process.h>
#include <alloc.h>
#include <mem.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <io.h>
#include <dir.h>
#include <ctype.h>

/* 키보드의 입력 변수 */
union scan {
	int c;
	char ch[2];
}sc;

char far* base_mem;
/* 비디오램의 시작번지 저장 */

#include "wdir2.h"

int Fcount;
/* 화일의 갯수 저장 */

/* 화일 정보 저장 */
struct file_info {
	char name[13];
	char attr;
	char at[5];
	long size;
	struct file_info* next;
};

struct file_info* file_list, * file_first;

/* 함수 선언 */
void Cursor(int s, int e);
/* 커서 모양 결정 */
void File(int no);
/* 화일 포인터 이동 */
void Free_file(void);
/* 화일 정보 메모리에서 제거 */
void Displayfile(int start, int select);
/* 화일 정보 화면에 표시 */
void Readingfile(void);
/* 화일 정보 일기 */
void Delete(void);
/* 화일 삭제 */
void Copy(void);
/* 화일 복사 */
void Rename(void);
/* 화일 이름 바꾸기 or 이동 */
void Attribute(void);
/* 화일 속성 변경 */
void End(void);
void Cdrive(void);
/* 드라이브 변경 */
void Re_name(char far* old_name, char far* new_name);
void View(void);
/* 화일 내요 보기 */
void Mdir(void);
/* 디렉토리 만들기 */



void main(void)
{
	int i, oldi, j, sel;
	char* menu[7] = { "Copy","Move","Dele","FAttr","View","Mdir","Rdir" };

	Video_mode();
	Cursor(6, 0);
	Clear(0, 0, 79, 24);
	j = 0; sel = 0;

	Write_string(20, 2, " W d i r  &  File Manager ", 0x12);
	Write_string(14, 4, "Name", 15);
	Write_string(36, 4, "Attr", 15);
	Write_string(47, 4, "Size", 15);

	Write_char(11, 5, 201, 12);
	Write_char(55, 5, 187, 12);
	Write_char(11, 18, 200, 12);
	Write_char(55, 18, 188, 12);
	for (i = 6; i < 18; i++) {
		Write_char(11, i, 186, 12);
		Write_char(55, i, 186, 12);
	}
	for (i = 12; i < 55; i++) {
		Write_char(i, 5, 205, 12);
		Write_char(i, 18, 205, 12);
	}

	Readingfile();          /* 현 디렉토리의 파일을 읽는다 */

	Write_string(13, 19, "Copy  Move  Dele  Attr  View  Mdir  Rdir", 14);

	Write_string(13, 23, "<<< Wdir beta 1.01    Make by Baek Jong ok >>>", 0x30);

	Displayfile(j, sel);     /* 읽어온 파일을 화면에 출력한다 */

	i = 0; oldi = 1;
	do {
		Write_string(13 + oldi * 6, 19, menu[oldi], 14);
		Write_string(13 + i * 6, 19, menu[i], 0x30);
		sc.c = Get_key();

		if (sc.ch[0] == RETURN) {
			if (file_list->attr & FA_DIREC) {     /* 커서가 디렉토리에 있을때 */
				if (i == 6) rmdir(file_list->name);   /* 디렉토리 지우기 */
				else {
					chdir(file_list->name);         /* 디렉토리 변경 */
					j = 0; sel = 0;
				}
			}
			else {
				/*			switch(i) {
								case 0: Copy();		break;
								case 1: Rename();	break;
								case 2: Delete();	break;
								case 3: Attribute();break;
								case 4: View();		break;
								case 5: Mdir();		break;
								case 6: Rename();   break;
							}
						}
				*/
				Free_file();
				Readingfile();
				if (sel > Fcount - 1) sel = Fcount - 1;
				Displayfile(j, sel);
			}                           /* 엔터키 누렸을 경우 */
		}
		else if (sc.ch[0] == ESC) End();

		switch ((int)sc.ch[1]) {
		case RIGHT:                /* 오른쪽 화살표 */
			oldi = i;
			i++;
			if (i > 6) i = 0;
			break;
		case LEFT:                /* 왼쪽 화살표 */
			oldi = i;
			i--;
			if (i < 0) i = 6;
			break;
		case UP:                /* 위 화살표 */
			sel--;
			if (sel < 0) sel = 0;
			if (j > sel) j--;
			if (j < 0) j = 0;
			Displayfile(j, sel);
			break;
		case DOWN:                /* 아래 화살표 */
			sel++;
			if (sel > Fcount - 1) sel = Fcount - 1;
			if (j + 10 < sel) j++;
			if (j + 10 > Fcount) j = Fcount - 11;
			if (Fcount < 11) j = 0;
			Displayfile(j, sel);
			break;
		case HOME:                /* HOME 키 */
			sel = 0; j = 0;
			Displayfile(j, sel);
			break;
		case END:                /* END 키 */
			sel = Fcount - 1;
			j = Fcount - 11;
			if (Fcount < 11) j = 0;
			Displayfile(j, sel);
			break;
		case F1:                /* F1 키 */
			Copy();             /* 화일 복사 */
			break;
		case F2:                /* F2 키 */
			Rename();           /* 이름 변경 또는 이동 */
			break;
		case F3:                /* F3 키 */
			Cdrive();           /* 드라이브 변경 */
			sel = 0; j = 0;
			Free_file();
			Readingfile();
			Displayfile(j, sel);
			break;
		case F4:                /* F4 키 */
			Mdir();             /* 디렉토리 만들기 */
			break;
		case F5:                /* F5 키 */
		case F6:
		case F7:
		case F8:
		case F9:
		case F10:
			break;

		}
		Write_string(13, 18, "                              ", 0x79);
		Write_string(14, 18, " Selected File = ", 0x79);
		Write_string(31, 18, file_list->name, 0x79);
	} while (1);

}

/* 커서 위치 설정 */
void Cursor(int e, int s)
{
	union REGS r;
	r.h.ah = 1;
	r.h.ch = s;
	r.h.cl = e;
	int86(0x10, &r, &r);
}

/* 현 디렉토리에서 파일 읽기 */
void Readingfile(void)
{
	struct ffblk file;
	int done;
	char dir[5] = { "SDIR" };
	char buf[50];
	long Fsize = 0;

	file_first = file_list = malloc(sizeof(struct file_info));

	strcpy(file_list->name, "No Files");
	strcpy(file_list->at, "!!!!");       /* No file */
	file_list->size = 0;

	done = findfirst("*.*", &file, FA_RDONLY | FA_HIDDEN | FA_SYSTEM | FA_ARCH |
		FA_DIREC);
	Fcount = 0;
	while (!done) {
		if (strcmp(file.ff_name, ".") != 0) {
			strcpy(file_list->name, file.ff_name);
			file_list->attr = file.ff_attrib;
			file_list->size = file.ff_fsize;

			/* 파일의 속성 읽기 */
			strcpy(file_list->at, "....");
			if (file.ff_attrib & FA_RDONLY) file_list->at[0] = 'R';
			if (file.ff_attrib & FA_HIDDEN) file_list->at[1] = 'H';
			if (file.ff_attrib & FA_SYSTEM) file_list->at[2] = 'S';
			if (file.ff_attrib & FA_ARCH) file_list->at[3] = 'A';
			if (file.ff_attrib & FA_DIREC) strcpy(file_list->at, dir);

			Fsize += file.ff_fsize;

			file_list = file_list->next = malloc(sizeof(struct file_info));
			Fcount++;
		}
		done = findnext(&file);
	}
	file_list->next = NULL;         /* 파일 저장 포인터의 마지막 부분 */
	sprintf(buf, "%4d files listed = %6ld Bytes ", Fcount, Fsize);
	Write_string(14, 17, buf, 15);
	memset(buf, 0x00, 50);
	Clear(16, 20, 66, 20);
	getcwd(buf, 50);                 /* 현 디렉토리 얻기 */
	Write_string(16, 20, buf, 12);
}

/* 파일 정보의 포인터를 얻는다 */
void File(int no)
{
	int i;

	file_list = file_first;
	for (i = 0; i < no; i++)
		file_list = file_list->next;
}

/* 파일 정보 버퍼 초기화 */
void Free_file(void)
{
	file_list = file_first;
	do {
		free(file_list);
		file_list = file_list->next;
	} while (file_list != NULL);
}

/* 파일을 화면에 표시 한다 */
void Displayfile(int start, int select)
{
	int i, j, col;
	char name[42];

	Clear(12, 6, 54, 16);

	j = 0;
	File(start);
	for (i = start; i < start + 11; i++) {
		col = 11;
		if (file_list->attr & FA_DIREC) col = 13;
		Write_string(14, 6 + j, file_list->name, col);
		Write_string(36, 6 + j, file_list->at, 10);
		sprintf(name, "%6ld", file_list->size);
		Write_string(47, 6 + j, name, 14);
		j++;
		file_list = file_list->next;
		if (i >= Fcount - 1) break;
	}
	File(select);
	sprintf(name, " %-13s         %4s       %6ld", file_list->name, file_list->at,
		file_list->size);
	Write_string(13, 6 + select - start, name, 0x29);
}

/* 선택된 파일 지우기 */
void Delete(void)
{
	char* menu[2] = { "N","Y" };
	int i = 0;

	Make_window("DELETE", 7, 6, 30, 9, 1);
	Active_window(0, 15, 9);
	do {
		Write_string(15, 7, file_list->name, 12);
		Write_string(10, 8, "Delete ? N  Y", 0xe);
		Write_string(19 + i * 3, 8, menu[i], 0x30);
		sc.c = Get_key();
		i = 1 - i;
		if (sc.ch[0] == 13) {
			if (i == 0) {
				remove(file_list->name);
			}
			break;
		}
	} while (1);
	Deact_window();
}

/* 현 파일을 다른 이름으로 변경 */
void Copy(void)
{
	char name[50], buf[1024];
	int handle1, handle2, i, lmt, cnt;

	memset(name, 0x00, 50);
	Make_window("COPY", 0, 0, 66, 3, 1);
	Active_window(0, 14, 9);
	Write_string(1, 1, "Source File : ", 15);
	Write_string(15, 1, file_list->name, 15);
	Write_string(1, 2, "Target File : ", 12);
	Cursor(7, 0);
	Get_word(2, 15, 50, name, 0x1b);
	Cursor(0, 6);
	while (name[0] != '\0') {
		handle1 = open(file_list->name, O_RDONLY | O_BINARY);
		handle2 = open(name, O_CREAT | O_TRUNC | O_RDWR | O_BINARY, S_IREAD | S_IWRITE);

		if (handle1 == -1 || handle2 == -1) {
			close(handle1);
			close(handle2);
			Write_string(1, 1, "     File Erroe !!!    ", 0x42);
			Deact_window();
			return;
		}
		lmt = filelength(handle1) / 1024L;
		for (i = 0; i <= lmt; i++) {
			cnt = read(handle1, buf, 1024);
			if (cnt <= 0) break;
			if (write(handle2, buf, cnt) == -1) {
				Write_string(1, 1, "   Disk Error !!!    ", 0x4e);
				break;
			}
		}
		memset(name, 0x00, 50);
	}
	close(handle2);
	close(handle1);
	Deact_window();
}

/* 이름 바꾸기 또는 옮기기 */
void Rename(void)
{
	char name[50];

	memset(name, 0x00, 50);
	Make_window("RENAME", 0, 0, 66, 3, 1);
	Active_window(0, 14, 9);
	Write_string(1, 1, "Source File :", 15);
	Write_string(15, 1, file_list->name, 15);
	Write_string(1, 2, "Target File :", 12);
	Cursor(7, 0);
	Get_word(2, 15, 50, name, 0x1b);
	Cursor(0, 6);
	if (name[0] != '\0') {
		Re_name(file_list->name, name);
	}
	Deact_window();
}

/* 파일 속성 바꾸기 */
void Attribute(void)
{
	unsigned char attrcode[4] = { FA_RDONLY,FA_HIDDEN,FA_SYSTEM,FA_ARCH };
	char* attrstr[4] = {
		"A. Read only -",
		"B. Hidden    -",
		"C. System    -",
		"D. Archive   -"
	};
	unsigned char dp1;
	union REGS r;
	struct SREGS sg;
	int i, k;

	Make_window("Attribute", 0, 0, 23, 6, 1);
	Active_window(0, 15, 9);
	dp1 = file_list->attr;
	Write_string(4, 1, file_list->name, 15);

	for (i = 0; i < 4; i++) {
		Write_string(1, i + 2, attrstr[i], 3);
		if ((dp1 & attrcode[i]) > 0)
			Write_string(17, i + 2, "ON ", 11);
		else
			Write_string(17, i + 2, "OFF", 11);
	}
	while (1) {
		sc.ch[0] = Get_key();
		if (sc.ch[0] == 0x1b) break;
		else if (sc.ch[0] == 0x0d) {
			segread(&sg);
			sg.ds = sg.ss;
			r.x.ax = 0x4301;
			r.x.cx = (unsigned)dp1;
			r.x.dx = (unsigned)file_list->name;
			intdos(&r, &r);
			file_list->attr = dp1;
			break;
		}

		sc.ch[0] = toupper(sc.ch[0]);

		if (('@' < sc.ch[0]) && (sc.ch[0] < 'E')) {
			sc.ch[0] -= 'A';
			if ((dp1 & attrcode[sc.ch[0]]) > 0) {
				dp1 &= ~attrcode[sc.ch[0]];
				Write_string(17, (int)sc.ch[0] + 2, "OFF", 11);
			}
			else {
				dp1 |= attrcode[sc.ch[0]];
				Write_string(17, (int)sc.ch[0] + 2, "ON ", 11);
			}
		}
	}
	Deact_window();
}

/* 드라이브 변경 */
void Cdrive(void)
{
	union REGS r;
	int driv, od;
	char menu[6] = { "ABCDE" };

	Make_window("DRIVE", 0, 0, 11, 2, 1);
	Active_window(0, 12, 10);
	driv = od = 2;
	Write_string(1, 1, "A B C D E", 15);
	do {
		Write_char(od * 2 + 1, 1, menu[od], 15);
		Write_char(driv * 2 + 1, 1, menu[driv], 0x19);
		sc.c = Get_key();
		switch ((int)sc.ch[1]) {
		case 77:
			od = driv;
			driv++;
			if (driv > 4) driv = 0;
		case 75:
			od = driv;
			driv--;
			if (driv < 0) driv = 4;
			break;
		case 28:
			r.h.ah = 0x0e;
			r.h.dl = driv;
			int86(0x21, &r, &r);
			break;
		}
	} while (sc.ch[1] != 28 && sc.ch[0] != 27);
	Deact_window();
}

/* C 언어의 rename() 함수를 개량한 것 */
void Re_name(char far* old_name, char far* new_name)
{
	union REGS r;
	struct SREGS s;

	r.h.ah = 0x56;
	r.x.dx = FP_OFF(old_name);
	r.x.di = FP_OFF(new_name);
	s.ds = FP_SEG(old_name);
	s.es = FP_SEG(new_name);

	intdosx(&r, &r, &s);
}

/* 파일 내용 보기 */
void View(void)
{
	char blank[80], * ptr, ch;
	FILE* fp;

	Make_window("", 0, 0, 79, 24, 0);
	Active_window(0, 15, 14);
	fp = fopen(file_list->name, "rt");
	blank[77] = '\0'; ch = 0;
	Write_string(25, 0, "VIEW => ", 14);
	Write_string(33, 0, file_list->name, 11);
	Go_xy(0, 2);
	while (1) {
		memset(blank, 0, 80);
		fgets(blank, 78, fp);
		if (feof(fp)) {
			cprintf("<*** End of File ***>");
			Get_key();
			break;
		}
		if (blank[0] == '\0') continue;
		ptr = blank;
		while (*ptr != '\0') {
			if (*ptr == '\t') *ptr = ' ';
			ptr++;
		}
		ptr = strchr(blank, '\n');
		if (ptr) cprintf("%s \r", blank);
		else cprintf("%s\n\r", blank);
		ch++;
		if (ch == 22) {
			sc.c = Get_key();
			if (sc.ch[0] == 27) break;
			Clear(0, 0, 79, 24);
			Write_string(25, 0, "VIEW => ", 14);
			Write_string(33, 0, file_list->name, 11);
			Go_xy(0, 2);
			ch = 0;
		}
	}
	fclose(fp);
	Deact_window();
}

/* 디렉토리 만들기 */
void Mdir(void)
{
	char name[10];

	memset(name, 0x00, 10);
	Make_window("Make Directory", 0, 0, 26, 2, 1);
	Active_window(0, 14, 9);
	Write_string(2, 1, "Creat Name : ", 12);
	Cursor(7, 0);
	Get_word(1, 15, 9, name, 0x1b);
	Cursor(0, 6);
	if (name[0] != '\0') {
		mkdir(name);
	}
	Deact_window();
}

void End(void)
{
	int i;

	Clear(0, 0, 79, 24);
	for (i = 0; i < 80; i++)
		Write_char(i, 0, ' ', 0x30);
	Write_string(10, 0, "File Manager ver 1.0 program by Baek Jong Ok", 0x3f);
	Go_xy(0, 1);
	Cursor(7, 6);
	Free_file();
	exit(-1);

}






