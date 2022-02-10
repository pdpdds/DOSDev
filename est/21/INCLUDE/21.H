typedef unsigned int Mem;

struct dblink {
	BYTE use;
//	WORD hi;
	WORD bn;
	BYTE kd;
	BYTE far lc[6];
	BYTE far *hs;
	BYTE far *line;
	Mem mh;
	WORD tt;
	struct dblink *prev;
	struct dblink *next;
};



typedef struct dblink NODE;

typedef struct {
	WORD row;
	int  y;
	int	 c;
	WORD loc;
	NODE *blk;
} blk;

typedef struct {
	int x;           
	int y;           
	BYTE lx;          
	BYTE ly;          
	int cx;
	int cy;
	int  cc;
	WORD cr;
	WORD cp;
	WORD total;
	WORD bl;
	BYTE modify;
	BYTE active;
	BYTE h_e;
	BYTE ins;
	BYTE scr_c;
	BYTE tab[30];

	BYTE word_gap; // gap between words;
	BYTE up_s;
	BYTE down_s;
	BYTE left_s;   // free space of the vLeftKey
	BYTE repeat;         // how many repeat printing
	BYTE prn_range[11];  // SNode one BYTE is index '0'= Full range '1'= Part
	BYTE prn_dens;

	char headline[80];
} text_window;

typedef struct {
	int x;            // Bit
	int y;            // Byte
	int lx;           // 8 bit
	int ly;           // 8 bit
	Mem m;
	BYTE headline[24];// window headline
	BYTE used;        // number of used block
	BYTE far **block; // contains  1.x(BYTE), 2.y(BYTE), 3.num of element(ex)
										// and  4.width(BYTE), 5.attribute  == block[8][5]
	BYTE far **data;  // popup data
	BYTE far **comment;// comment block
	BYTE far **code;  // case UP, DOWN, LEFT, RIGHT goto blk num == code[8][4]
										// and  ??
	BYTE far *p;     // extra pointer used when variable data block
	BYTE far *p_n;    // variable data number
	BYTE far *process;// write by vWriteString or vPT()
	BYTE far *vb_scr;
	BYTE far *value;
	BYTE far *set;
} popup_window;

typedef struct {
	int x, y, endx, endy;
	BYTE far **menu;
	BYTE far *head;
	int count, last_menu, active;

} menu_panel;

enum
{HEADERALL=1, HEADER1, HEADER2, FOOTERALL, FOOTER1, FOOTER2};

#define MSREAD  0
#define MSWRITE 1
#define ENGLOC   3
#define JAPANLOC 15
#define HALFLOC  19
#define FULLLOC  23
#define KEYMAX   32

#define bioskey(n) wGetKey(n)

enum {PW0=0, PW1,  PW2,  PW3,  PW4,  PW5,  PW6,  PW7,  PW8,  PW9,
			PW10,  PW11, PW12, PW13, PW14, PW15, PW16, PW17, PW18, PW19,
			PW20,  PW21, PW22, PW23, PW24, PW25, PW26, PW27, PW28, PW29,
			PW30 };

enum {
	PAN_0=0, PAN_1, PAN_2, PAN_3, PAN_4,
	PAN_5,   PAN_6, PAN_7, PAN_8, PAN_9
};

enum {
	PAN_33=10, PAN_34, PAN_51
};

enum {
	PAN_511=13, PAN_521, PAN_531, PAN_541, PAN_551
};

enum {
	LINE_PAN=18, HJPAN,   SCADULER, PRANGE,
	EXTPAN1,     EXTPAN2, EXTPAN3, HF_PAN
};

#define JOPAN 29
// Warning !!! JOPAN must be LAST

#define JP_HELP  12
#define POP_HELP 13

#define INTERNAL_ERR 4


#define TOTALHSNUM			20000
#define _CHECKNODE(n) {if(n->mh!=0) ex_err=iLockNode(n);else {n->tt=wGetLTLoc();ex_err=1;}}
#define _GETNODE (NODE *)npGetNode()

#define _GETLINE(len) (BYTE *)pGetMem(len+1)
#define _REGETLINE(len, s) (BYTE *)pRegetMem(s, len)

#define _GETHS(t) (BYTE *)pGetLHB(t)
#define _REGETHS(t, n) (BYTE *)pRegetLHB(t, n)

#define _CAPS_LOCK bGetKeyState(0)
#define _LEFT_SHIFT bGetKeyState(1)
#define _RIGHT_SHIFT bGetKeyState(2)
#define _SHIFT_PRESSED bGetKeyState(3)
#define _ALT_PRESSED bGetKeyState(4)
#define _CTRL_PRESSED bGetKeyState(5)
#define _SETSTART vResetHanin()

#define HEADLEN(i)  tw[i].prn_range[5]
#define FOOTLEN(i)  tw[i].prn_range[6]
#define LGAP_MUL(i) tw[i].prn_range[7]
//  prn_range[8] and [9] si total page number
#define ASV_MODIF tw[cur_w].prn_range[10]
#define TOT_PAGE ((tw[n].prn_range[8]<<8) | tw[n].prn_range[9])

#define TRUE      0xFF
#define INDEXREGS 0x3CE
#define VALREGS 	0x3CF
#define ENABLE  	0x0F
#define OUTPUT(index, value) outport(INDEXREGS, (((WORD)(value))<<8)|(index))
