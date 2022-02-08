
#define SPACE       32
/* 키보드 스캔 코드 값 */
#define RETURN      13
#define ESC         27
#define HOME        71
#define UP          72
#define PGUP        73
#define LEFT        75
#define RIGHT       77
#define END         79
#define DOWN        80
#define PGDN        81
#define DEL         83
#define Alt_R       19
#define Alt_D       32
#define Alt_C       46
#define Alt_X       45
#define Alt_V       47
#define Alt_M       50
#define F1          69
#define F2          60
#define F3          61
#define F4          62
#define F5          63
#define F6          64
#define F7          65
#define F8          66
#define F9          67
#define F10         68


int Get_key(void);
void Go_xy(int col,int row);
void Clear(int scol,int srow,int ecol,int erow);
void Write_char(int x,int y,int ch,int attrib);
void Write_string(int col,int row,char *p,int attrib);
void Save_screen(unsigned char *screen,int scol,int srow,int ecol,int erow,int color);
void Restore_screen(unsigned char *screen,int scol,int srow,int ecol,int erow);
void Video_mode(void);
void Make_window(char *header,int startx,int starty,int endx,int endy,int box);
void Draw_box(int color);
void Display_header(int color);
void Save_window(int color);
void Restore_window(void);
void Active_window(int b_color,int x_color,int h_color);
void Deact_window(void);
int  Get_word(int r,int c,int count,char *buffer,int color);


/* 키보드의 스캔코드를 읽는다 */
int Get_key(void)
{
	union REGS regs;
	regs.h.ah = 0;
	return int86(0x16,&regs,&regs);
}

/* 커서의 위치를 선정한다 */
void Go_xy(int row,int col)
{
	union REGS regs;
	regs.h.ah = 2;
	regs.h.dh = row;
	regs.h.dl = col;
	regs.h.bh = 0;
	int86(0x10,&regs,&regs);
}

/* 화면의 일부를 지운다 */
void Clear(int scol,int srow,int ecol,int erow)
{
	int i,j;
	char far *v;
	for (i=srow;i<=erow;i++) {
		for (j=scol;j<=ecol;j++) {
			v = base_mem + (i*160) + j*2;
			*v++ = ' ';
			*v = 0x7;
		}
	}
}

/* 원하는 위치에 한 문자 출력 */
void Write_char(int x,int y,int ch,int attrib)
{
	char far *v;
	v = base_mem + (y*160) + x*2;
	*v++ = ch;
	*v = attrib;
}

/* 원하는 위치에 문자열 출력 */
void Write_string(int col,int row,char *p,int attrib)
{
	char far *v;
	v = base_mem + (row*160) + col*2;
	while(*p) {
		*v++ = *p++;
		*v++ = attrib;
	}
}

/* 화면의 일부분 저장 */
void Save_screen(unsigned char *screen,int scol,int srow,int ecol,
					int erow,int color)

{
	char far *v;
	int i,j;
	for(i=srow;i<=erow;i++) {
		for(j=scol;j<=ecol;j++) {
			v = base_mem + (i*160) + j*2;
			*screen++ = *v++;
			*screen++ = *v;
			*v = color;
			*(v-1) = ' ';
		}
	}
}

/* 저장된 화면 복구 */
void Restore_screen(unsigned char *screen,int scol,int srow,int ecol,
						int erow)
{
	char far *v;
	int i,j;
	for (i=srow;i<=erow;i++) {
		for (j=scol;j<=ecol;j++) {
			v = base_mem + (i*160) + j*2;
			*v++ = *screen++;
			*v = *screen;
		}
	}
}

/* 비디오 램의 시작번지 설정 */
void Video_mode(void)
{
	union REGS regs;
	int vmode;

	regs.h.ah = 0xf;
	vmode = int86(0x10,&regs,&regs) & 0xff;
	if (vmode != 2 && vmode != 3 && vmode !=7) {
		printf("Display mode must be 80 column TEXT.\n");
		exit(1);
	}

	if(vmode == 7)
		base_mem = (char far *)0xB0000000;
	else
		base_mem = (char far *)0xB8000000;
}

/* 화면에서 문자를 읽어 들인다. */
int Get_word(int r,int c,int count,char *buffer,int color)
{
	int max;
	int dcount,k,cond;

	for (k=0;k<count;k++)
		Write_char(c+k,r,' ',color);
	if ((max = strlen(buffer)) > 0)
		Write_string(c,r,buffer,color);
	Go_xy(c,r);
	dcount = 0;
	cond = 0;
	do {
		sc.c = Get_key();
		switch((int)sc.ch[1]) {
			case 1:
			case 68:
			case 59:
			case 60:
			case 61:
			case 62:
			case 63:
			case 72:
			case 80:
				buffer[max] = '\0';
				return (sc.ch[1]);
			case 28:
				if (dcount != 0) {
					buffer[dcount] = '\0';
					for (k=c+dcount;k<c+count;k++)
						Write_char(k,r,' ',color);
				}
				return (sc.ch[1]);
			}
			if (sc.ch[0] != 0 && sc.ch[1] == 14) {
				if (dcount == 0) putchar(0x07);
				else {
					dcount--;
					buffer[max] = '\0';
					max--;
					Go_xy(c+dcount,r);
					for(k=dcount;k<=max;k++) {
						buffer[k] = buffer[k+1];
						if (buffer[k] == '\0')
							Write_char(c+k,r,' ',color);
						else
							Write_char(c+k,r,buffer[k],color);
					}
				}
			}
			else if ((sc.ch[0] >= 48 && sc.ch[0] <= 57) ||
					 (sc.ch[0] >= 65 && sc.ch[0] <= 90) ||
					 (sc.ch[0] >= 97 && sc.ch[0] <=122) ||
					 (sc.ch[0] == 32) || (sc.ch[0] == 58) ||
					 (sc.ch[0] == 59) || (sc.ch[0] >= 45 && sc.ch[0] <=47) ||
					 (sc.ch[0] >= 64 && (int)sc.ch[0] <= 253) ||
					 ((int)sc.ch[0] >=136 && (int)sc.ch[0] <= 211) ||
					 ((int)sc.ch[0] >= -200 && (int)sc.ch[0] <= -1)) {

					 if (dcount == 0 && ((sc.ch[0] > 47) ||
							((int)sc.ch[0] >= -200 && (int)sc.ch[0] <= -1))) {
							memset(buffer,0x00,sizeof(buffer));
							for (k=0;k<count;k++)
								Write_char(c+k,r,' ',color);
							max = 0;
							Go_xy(c,r);
					 }
					 Write_char(c+dcount,r,sc.c,color);
					 buffer[dcount] = sc.c;
					 dcount++;
					 if (dcount > max) max++;
					 if (dcount >= count) cond = 1;
			}
			else if (sc.ch[1] == 75) {
				if (dcount == 0) {
					buffer[max] = '\0';
					return(sc.ch[1]);
				}
				else dcount --;
			}
			else if (sc.ch[1] == 77) {
				if (dcount < max) dcount++;
				else {
					buffer[max] = '\0';
					return(sc.ch[1]);
				}
			}
			else putchar(0x07);
			Go_xy(c+dcount,r);
	}while(dcount < count);
	buffer[max] = '\0';
	if (cond == 1) {
		while(1) {
			sc.c = Get_key();
			if ((int)sc.ch[1] == 28) break;
		}
	}
	return (sc.ch[1]);
}

/****** 윈도우 제어 부분 ********/

struct window_frame {
	int startx,endx,starty,endy;
	int curx,cury;
	unsigned char *p;
	char *header;
	int box;
	int active;
} frame;


/* 윈도우 프레임을 만든다 */
void Make_window(char *header,int startx,int starty,int endx,
					int endy,int box)
{
	unsigned char *p;
	p = (unsigned char *)malloc(2*(endx-startx+1)*(endy-starty+1));
	if (!p) {
		Write_string(0,0,"Out over memory......",7);
		exit(-1);
	}
	frame.startx = startx;
	frame.starty = starty;
	frame.endx = endx;
	frame.endy = endy;
	frame.p = p;
	frame.header = header;
	frame.box = box;
	frame.active = 0;
	frame.curx = 0;
	frame.cury = 0;
}

/* 윈도우의 경계선을 그린다 */
void Draw_box(int color)
{
	int i;
	char far *v;
	for (i = frame.startx+1;i<frame.endx;i++) {
		v = base_mem + frame.starty*160 + i*2;
		*v++ = 196;
		*v = color;
		v = base_mem + frame.endy*160 + i*2;
		*v++ = 196;
		*v = color;
	}
	for (i = frame.starty+1;i<frame.endy;i++) {
		v = base_mem + i*160 + frame.startx*2;
		*v++ = 179;
		*v = color;
		v = base_mem + i*160 + frame.endx*2;
		*v++ = 179;
		*v = color;
	}
	Write_char(frame.startx,frame.starty,218,color);
	Write_char(frame.startx,frame.endy,191,color);
	Write_char(frame.endx,frame.starty,192,color);
	Write_char(frame.endx,frame.endy,217,color);
}

/* 윈도우의 헤더 문자열을 출력 */
void Display_header(int color)
{
	int y,len;

	y = frame.starty;
	len = strlen(frame.header);
	len = (frame.endy - y - len)/2;
	if (len < 0) return;
	y += len;
	Write_string(frame.startx,y,frame.header,color);
}

/* 윈도우로 지정된 화면을 저장한다 */
void Save_window(int color)
{
	int i,j;
	char *buf_ptr;
	char far *v;

	buf_ptr = frame.p;
	for (i=frame.starty;i<=frame.endy;i++) {
		for (j=frame.startx;j<=frame.endx;j++) {
			v = base_mem + (i*160) + j*2;
			*buf_ptr++ = *v++;
			*buf_ptr++ = *v;
			*v = color;
			*(v-1) = ' ';
		}
	}
}

/* 윈도우로 지정된 화면 복구 */
void Restore_window(void)
{
	int i,j;
	char *buf_ptr;
	char far *v;

	buf_ptr = frame.p;
	for (i=frame.starty;i<=frame.endy;i++) {
		for (j=frame.startx;j<=frame.endx;j++) {
			v = base_mem + (i*160) + j*2;
			*v++ = *buf_ptr++;
			*v = *buf_ptr++;
		}
	}
}

/* 윈도우를 활성화시킨다 */
void Active_window(int b_color,int x_color,int h_color)
{
	int x,y;
	if (!frame.active) {
		Save_window(b_color);
		frame.active = 1;
	}

	if (frame.box)
		Draw_box(x_color);
	Display_header(h_color);
	x = frame.startx + frame.curx + 1;
	y = frame.starty + frame.cury + 1;
	Go_xy(x,y);
}

/* 윈도우를 비활성화시킨다 */
void Deact_window(void)
{
	frame.curx = 0;
	frame.cury = 0;
	Restore_window();
	frame.active = 0;
	free(frame.p);
}

