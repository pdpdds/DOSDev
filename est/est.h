//#define _21XT
//#define _21PCX
//Date 		: 1993/04/28
//Manager : J.J. Kim
//Update 	:

typedef int SINT;
typedef unsigned int WORD;
typedef unsigned char BYTE;
typedef unsigned long DWORD;
typedef enum {F=0, T} BOOL;
typedef unsigned int word;
typedef unsigned char byte;
typedef unsigned long ulong;
typedef enum {FAULT=0, TRUE} bool;

typedef struct {
	int x;
	int y;
	int ex;
	int ey;
} BOX;

typedef struct {
	byte code 	: 5;
	byte check 	: 2;
	byte flag   : 1;
} hkey;

typedef union {
	hkey key;
	byte b;
} HANKEY;

typedef struct {
	word  code3 : 5;
	word  code2 : 5;
	word  code1 : 5;
	word  flag  : 1;
} CODE;

typedef union {
	CODE c3;
	byte c2[2];
	word i;
} CODE2;

typedef union {
	byte ch[2];
	word i;
} INKEY;

#define B7 			0x80
#define B6 			0x40
#define B5 			0x20
#define B4 			0x10
#define B3 			0x08
#define B2 			0x04
#define B1 			0x02
#define B0 			0x01
#define HanNull 0x8441
#define null		0
#define Null		0
#define ByteMax	0xff
#define ByteMin	0
#define CharMax	127
#define CharMin	-128
#define IntMax	0x7fff
#define IntMin	0x8000
#define WordMax 0xffff
#define WordMin 0

#define ON   		1
#define OFF  		0
#define YES  		1
#define NO   		0

#define RETURN   '\r'
#define BS        8
#define TAB       9
#define SPACE    32
#define ESC      27
#define CTRL_BS  0x007f

#define UP       72
#define DOWN     80
#define RIGHT    77
#define LEFT     75
#define HOME     71
#define END      79
#define PGDN     81
#define PGUP     73
#define INSERT   82
#define DELETE   83
#define F1       59
#define F2       60
#define F3       61
#define F4			 62
#define F5       63
#define F6       64
#define F7       65
#define F8       66
#define F9       67
#define F10      68
#define CTRL_INSERT  	0x92
#define CTRL_DELETE  	0x93
#define CTRL_HOME			119
#define CTRL_END  		117
#define CTRL_PGUP 		132
#define CTRL_PGDN 		118
#define CTRL_LARROW   0x73
#define CTRL_RARROW   0x74
#define CTRL_UARROW   0x8d
#define CTRL_DARROW		0x91
#define ALT_INSERT  	0xa2
#define ALT_DELETE  	0xa3
#define ALT_HOME			0x97
#define ALT_END  			0x9f
#define ALT_PGUP 			0x99
#define ALT_PGDN 			0xa1
#define ALT_LARROW    0x9b
#define ALT_RARROW    0x9d
#define ALT_UARROW    0x98
#define ALT_DARROW		0xa0
#define SFT_TAB				0x0f

enum {CTRL_A=1, CTRL_B, CTRL_C, CTRL_D, CTRL_E, CTRL_F, CTRL_G,
			CTRL_H, CTRL_I, CTRL_J, CTRL_K, CTRL_L, CTRL_M, CTRL_N,
			CTRL_O,	CTRL_P, CTRL_Q, CTRL_R, CTRL_S, CTRL_T, CTRL_U,
			CTRL_V,	CTRL_W, CTRL_X, CTRL_Y, CTRL_Z};
enum {SHIFT_F1=84, SHIFT_F2, SHIFT_F3, SHIFT_F4, SHIFT_F5,
			SHIFT_F6, SHIFT_F7, SHIFT_F8, SHIFT_F9, SHIFT_F10,
			ALT_F1=104, ALT_F2, ALT_F3, ALT_F4, ALT_F5,
			ALT_F6, ALT_F7, ALT_F8, ALT_F9, ALT_F10,
			CTRL_F1=94, CTRL_F2, CTRL_F3, CTRL_F4, CTRL_F5,
			CTRL_F6, CTRL_F7, CTRL_F8, CTRL_F9, CTRL_F10};

enum {ALT_1=120, ALT_2, ALT_3, ALT_4, ALT_5,
			ALT_6, ALT_7, ALT_8, ALT_9, ALT_0, ALT_HYPHEN, ALT_EQUAL};

enum {ALT_Q=16, ALT_W, ALT_E, ALT_R, ALT_T, ALT_Y, ALT_U, ALT_I, ALT_O,	ALT_P,
			ALT_A=30, ALT_S, ALT_D, ALT_F, ALT_G, ALT_H, ALT_J, ALT_K, ALT_L,
			ALT_Z=44, ALT_X, ALT_C, ALT_V, ALT_B, ALT_N, ALT_M };

typedef struct {
	byte kind;
	byte func_num;
	int mx;
	int my;
	word vid_seg;
	byte para1;
	byte para2;
} video_init;

