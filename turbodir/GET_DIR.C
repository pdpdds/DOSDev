/* -------------------------------------------------------------------
	Turbo-C F3 기능을 프로그램화
*/
#ifdef TEST
#include <dos.h>
#include <dir.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

static struct dirfile {
	char filename[13];
	int  attrib;
} FILE_dir[3000];

static int total, first;
char far *base_mem;

#include "key.h"
#include "vid_mode.c"
#include "vputchar.c"
#include "vxy_box.c"
#include "vxy_cls.c"
#include "vsavescr.c"
#include "vrestscr.c"
#include "vputattr.c"
#include "vput_str.c"
#include "vput_spc.c"
#include "get_key.c"
#include "scroll.c"
#include "find_dir.c"

int get_directory(const char *dirpath);
char *current_directory(char *path)
{
   strcpy(path, "X:\\");      /* fill string with form of response: X:\ */
   path[0] = 'A' + getdisk();    /* replace X with current drive letter */
   getcurdir(0, path+3);  /* fill rest of string with current directory */
   return(path);
}

void main(void)
{
	const char* old_dir;
	char curdir[MAXPATH];
//	unsigned char *buf=save_screen(19,10,62,12,7<<4|3);
//	if(!buf) exit(1);

	video_mode();	xy_cls(0,0,79,24,0<<4|7);
	old_dir =current_directory(curdir);
	strcat((char*)old_dir, "\\*.*");
	gotoxy(1,1);
	get_directory(old_dir);
	gotoxy(1,23);

}
#endif

/*----------------------------------------------------------------------- */
void put_file_list(int);

int get_directory(const char *dirpath)
{
	union key {
		char ch[2];
		int i;
	} inkey;

	char Path[80];
	char Drive[3];
	char Directory[66];
	char Filename[9];
	char Extend[3];
	int col=7,row=8;
	int LR_move=0;
	int UD_move=0;
	int ED_move=0;
	int FileCur=0;
	int j=0;

	/* 출력길이 68열 : 15+2열 */
//	unsigned char *buf=save_screen(6,6,72,18,3<<4);
//	if(!buf) exit(1);

	strcpy(Path,dirpath);  /* 인자로 넘겨준 경로.파일을 복사 */
	xy_box   (6,6,72,18,3<<4|15);
	put_attr (7,7,65,7<<4);
	put_space(7,7,65,7<<4);
	put_str(36-(strlen(Path)/2),7,Path,7<<4|1);
	find_dir(Path);
	put_file_list(0);			/* 지정한 파일을 화면에 출력 */
	put_attr(7,8,14,2<<4|0);			/* 첫번째 데이타역상 */
#if 0
gotoxy(1,1); textattr(1<<4|15);
cprintf("No  : %-13d", FileCur);
gotoxy(1,2);
if(FILE_dir[FileCur].filename[1] == '.')
	cprintf("NAME: %-13s", ".. < DIR >");
else
if(FILE_dir[FileCur].attrib==FA_DIREC) {
	cprintf("NAME: %-13s < DIR >", FILE_dir[FileCur].filename);
}
else
	cprintf("NAME: %-13s", FILE_dir[FileCur].filename);
#endif
	for(;;) {
#define NOP_OP
		while(!kbhit())
			NOP_OP;
		put_attr(col,row+UD_move,14,3<<4|15);		/* 역상해제 */
		inkey.i=get_key();

		if(inkey.ch[0]) {
			switch(inkey.ch[0]) {
				case ESC:
//					restore_screen(6,6,72,18,buf);
					return ESC;
				case ENTER:
					if(FILE_dir[FileCur].attrib==FA_DIREC) {
						fnsplit(Path,Drive,Directory,Filename,Extend);
						strcpy(Path,"");
						if(strncmp(FILE_dir[FileCur].filename,"..",2)==0)
						{
							Directory[strlen(Directory)-1]='\0';
							for(j=strlen(Directory)-2;Directory[j]!='\\';--j)
								Directory[j]='\0';
						} else {
							strcat(Directory,FILE_dir[FileCur].filename);
							strcat(Directory,"\\");
						}
						strcat(Path,Drive);
						strcat(Path,Directory);
						if(chdir(Path))
							printf("\nERROR !");

						strcat(Path,Filename);
						strcat(Path,Extend);
						find_dir(Path);
						put_space(7,7,65,7<<4);
						put_str(36-(strlen(Path)/2),7,Path,7<<4|1);
						put_file_list(0);
						/* ------- 초기화 작업 -------- */
						LR_move=0; ED_move=0; UD_move=0; FileCur=0;
						col=7; row=8;
						strcpy(Drive,"");
						strcpy(Directory,"");
						strcpy(Filename,"");
						strcpy(Extend,"");
					}
					break;

			}
		} else
		if(inkey.ch[1]) {
		switch(inkey.ch[1]) {
			case LEFT:
				if(FileCur==0) continue;
				FileCur--; /* 파일위치 카운트 */
				if(FileCur<0)
					{ FileCur++; break;}
				col=col-16;
				LR_move--;
				if(LR_move<0)
					{ LR_move=3; col=(col)+4*16; UD_move--; }
				if(UD_move<0) {
					scroll(7,8,71,17,1,3<<4|15,DOWN);
					put_file_list(FileCur-LR_move);
					UD_move++;
				}
				break;
			case RIGHT:
				if(FileCur==total) continue;
				FileCur++;
				if(FileCur>=total)
					{ FileCur--; break;}
				col=col+16;
				LR_move++;
				if(LR_move==4)
					{ LR_move=0; col=7; UD_move++; }
				if(UD_move>9){
					scroll(7,8,71,17,1,3<<4|15,UP);
					put_file_list(FileCur-36-LR_move);
					UD_move--;
				}

				break;
			case DOWN:
				if(FileCur==total) continue;
				FileCur+=4;
				if(FileCur>=total)
					{ FileCur-=4; break;}
				UD_move++;
				if(UD_move>9){
					scroll(7,8,71,17,1,3<<4|15,UP);
					put_file_list(FileCur-36-LR_move);
					UD_move--;
				}
				break;
			case UP:
				if(FileCur==0) continue;
				FileCur-=4;
				if(FileCur<0)
					{ FileCur+=4; break; }
				UD_move--;
				if(UD_move<0) {
					scroll(7,8,71,17,1,3<<4|15,DOWN);
					put_file_list(FileCur-LR_move);
					UD_move++;
				}
				break;
			case HOME:
				if(FileCur==0) continue;
				FileCur=0;
				col=7;
				LR_move=0;
				UD_move=0;
				put_file_list(0);
				break;
			case END:
				if(FileCur==total-1) continue;
				FileCur=total-1;
				ED_move=FileCur%40;
				UD_move=ED_move/4;
				LR_move=ED_move%4;
				col=7+LR_move*16;
				put_file_list(FileCur-ED_move);
				break;
			case PGDW:
				if(FileCur==total) continue;
				FileCur+=40;
				if(FileCur>=total)
					{ FileCur-=40; break; }
				put_file_list(FileCur-LR_move);
				UD_move=0;
				break;
			case PGUP:
				if(FileCur ==0) continue;
				FileCur-=40;
				if(FileCur<0)
					{ FileCur+=40; break; }
				put_file_list(FileCur-LR_move);
				UD_move=0;
				break;

			}/* switch */
		} /* if */
put_attr(col,row+UD_move,14,2<<4|0);	/* 역상 */

gotoxy(1,1); textattr(1<<4|15);
cprintf("No  : %-13d", FileCur);
gotoxy(1,2);
if(FILE_dir[FileCur].filename[1] =='.')
cprintf("NAME: %-13s", "< UP DIR >");
else
if(FILE_dir[FileCur].attrib==FA_DIREC) {
	cprintf("NAME: %-13s", "< D I R >");
}
else
	cprintf("NAME: %-13s", FILE_dir[FileCur].filename);
	} /* for */
}

void put_file_list(int i)
{
	int  col=8, row=8;
	int Lcnt=0,Rcnt=0;
	int j;
	xy_cls(7,8,71,17,3<<4|15);

	for(;i<total;i++,Rcnt++) {
		if(FILE_dir[i].attrib==FA_DIREC)
			put_str(col,row,FILE_dir[i].filename,3<<4|15);
		else
			put_str(col,row,FILE_dir[i].filename,3<<4|15);
		col=col+16;
		Lcnt++;
		if(Lcnt==4){
			Lcnt=0;
			row++;
			col=8;
		}
		if(i>=total) break;
		if(Rcnt>=39) break;

	}
}

