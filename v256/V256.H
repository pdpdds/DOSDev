#define set256mode() setvmode(0x13);
#define settextmode() setvmode(0x03);
#define clearscreen(color) fill_box(0,0,319,199,(color));

typedef unsigned char BYTE;
typedef BYTE DacPalette256[256][3];

void savergb(char *);
void loadrgb(char *);
void savergb2(char *);
void loadrgb2(char *);
void setvgapalette256 (DacPalette256 *);
void getvgapalette256 (DacPalette256 *);

void fill_box (unsigned,unsigned,unsigned,unsigned,BYTE);
void monitor_onoff (char);

BYTE getpixel256 (unsigned,unsigned);
void putpixel256 (unsigned, unsigned, BYTE);

void uiswap (unsigned *,unsigned *);

void line256 (unsigned,unsigned,unsigned,unsigned,int,BYTE);
void box256 (unsigned,unsigned,unsigned,unsigned,int,BYTE);

void memtomem (unsigned,unsigned,unsigned,unsigned,unsigned,BYTE);
void smemtosmem (BYTE *,BYTE *,unsigned);

int  ReadPCXFileHdr (char *,int);
void InstallPCXFilePalette (void);
void displaypcxfile (char *,int);
void displaypccfile (char *,int,unsigned,unsigned);
int  readpcxfilebuf (char *, BYTE **);
void displaypcxbuf (BYTE *,int);

void imagesize256 (unsigned,unsigned,unsigned,unsigned,BYTE **);
void getimage256 (unsigned,unsigned,unsigned,unsigned,BYTE *);
void putimage256 (unsigned,unsigned,int,BYTE *);
void setscanaddress (unsigned);
void virtualscreen (unsigned,unsigned,unsigned,unsigned,unsigned,unsigned,int,BYTE *);

#define W unsigned
void composition(W,W,W,W,BYTE *,W,W,BYTE *,int);
void getcomposition(BYTE *,W,W,BYTE *,int);
void brightCtrl(BYTE,int,BYTE *,BYTE *,BYTE *,int);
void Fade_In(void);
void Fade_Out(void);
void displaypccfile2(char *,int,W,W);
void synthetic(W,W,W,W,BYTE *);

void inithan(BYTE *,BYTE *);
void putimg(int,int,int,void *);
void hanxy(int,int,int,BYTE *);
void hanend(void);
void interrupt handler(void);
void setkeyint(void);
void resetkeyint(void);
void vertical_retrace(void);
void getimagevirtual(W,W,W,W,BYTE *,BYTE *);
void virtualscreen2(W,W,W,W,W,W,int,BYTE *);
void xorputpixel256(W,W,BYTE);

void get1DACR(BYTE,BYTE *,BYTE *,BYTE *);
void getNDACR(BYTE,int,BYTE *,BYTE *,BYTE *);
void set1DACR(BYTE,BYTE,BYTE,BYTE);
void setNDACR(BYTE,int,BYTE *,BYTE *,BYTE *);
void greyCtrl(BYTE,int,BYTE *, BYTE *,BYTE *,int);

extern int SOUND_DEV,hancolor,xsi__ze__,ysi__ze__,rgb_check,
           ESC,CTRL,ALT,SPACE,UP,DOWN,LEFT,RIGHT,LSHIFT;
extern BYTE VoiceFlag,R_[256],G_[256],B_[256];
extern silentVoice(void);
extern BYTE far *readvocfile(char *,unsigned *);

