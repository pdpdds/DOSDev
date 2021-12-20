void end(void);
void game(void);
void initg(void);
void initT(void);
void screen(void);
void Error(int i);
void Tcheck(void);
void buff(int &a,int &b,int c);
void rect(int x,int y);
void AddLib( char *dol, int value,int i);
void InitLibDefault(void);
void dol(int x,int y,int mode);
void ButtonRect(int x1,int y1,int x2,int y2,int State);
void ScrollUp(int x1,int y1,int x2,int y2,int num);
void ScrollDown(int x1,int y1,int x2,int y2,int num);
int  change(int mode);
int  check(int x,int y);
int  check2(int x,int y);
int dolvalue(int x,int y,int mode);
int dolcount(int x,int y,int dx,int dy,int mode);
int  decision(int &x,int &y,int mode);

#define TRUE        1
#define FALSE       0

#define MAN         1
#define COMPUTER    0

#define WHITE      15
#define GREEN       2

#define OK      30000
#define XLEN       15
#define YLEN       15
#define SAM2       -1
#define LIBLEN    300

#define SPACE  0x0020
#define LEFT   0x4b00
#define LEFT   0x4b00
#define RIGHT  0x4d00
#define UP     0x4800
#define DOWN   0x5000
#define ESC    0x001b

#define MIN        20
#define MAX       440
#define STEP       30

int TABLE[XLEN][YLEN];
int xvalue,yvalue;
int pox,poy;
int count,libCount[4]={0,0,0,0};

struct
{
   int value;
   char dol[15];
} lib[4][LIBLEN];





