#ifndef __VGA320_LIB_HEADER__
#define __VGA320_LIB_HEADER__

#define ENGFONTSIZE     16
#define HANFONTSIZE     32
#define ENGCHARNO       96
#define CHO_START        0
#define JUNG_START      18
#define JONG_START      39
#define HANCHARNO       67

#define VGA_VRAM  ((Byte far *)0xA0000000L)

enum key_values_scancode {
    F1    = 315,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    ENTER = 13,
    LEFT  = 331,
    RIGHT = 333,
    INS   = 338,
    DEL,
    ESC   = 27,
    SPC   = 32,
    HOME  = 327,
    UP    = 328,
    PGUP  = 329,
    ENDKEY= 335,
    DOWN  = 336,
    PGDN  = 337,
    ALT_X = 301
};

enum {
    FALSE, TRUE,
    OFF=0, ON,
    NO=0, YES
};

enum Graphic_mode {
    NOGRP,
    VGALOW
};

typedef unsigned Word;
typedef unsigned long Dword;
typedef unsigned char Byte;

typedef char ENGFONT[ENGFONTSIZE];
typedef char HANFONT[HANFONTSIZE];

extern ENGFONT *engfont;
extern HANFONT *hanfont;
extern OutPutMode;
extern Byte chotbl[];
extern Byte jungtbl[];
extern Byte jongtbl[];

extern int IsGrp;

#ifndef MY_SIZE_T
#define MY_SIZE_T
typedef unsigned long my_size_t;
#endif

int getkey( void );
void init320(void);
void close320(void);
void mode(int no);
void far *malloc2( my_size_t sz);
void far *free2( void far *pt );
void error( char *s );
char *loadMFEfont( char *fname, int no1 );

#define writexy(x,y,s)          VSMprintxy2p(VGA_VRAM,x,y,s,15,0)
#define printxy(x,y,s,co)       VSMprintxy2p(VGA_VRAM,x,y,s,co,0)
#define printxy2(x,y,s,co,co2)  VSMprintxy2p(VGA_VRAM,x,y,s,co,co2)
#define vline(y1,y2,x,w,co)     VSMvlinep(VGA_VRAM,y1,y2,x,w,co)
#define hline(x1,x2,y,w,co)     VSMhlinep(VGA_VRAM,x1,x2,y,w,co)
#define ClearScreen(co)         memset(VGA_VRAM,co,64000U)
#define PutImage2co(x,y,b,c,m)  VSMPutImage2cop(VGA_VRAM,x,y,b,c,m)
#define PutImage(x,y,b,m)       VSMPutImagep(VGA_VRAM,x,y,b,m)
#define GetImage(x,y,xx,yy,buf) VSMGetImagep(VGA_VRAM,x,y,xx,yy,buf)
#define ClrBox(x,y,xx,yy,co)    VSMClrBoxp(VGA_VRAM,x,y,xx,yy,co)
#define printxy2(x,y,s,fco,bco) VSMprintxy2p(VGA_VRAM,x,y,s,fco,bco)

void VSMtoVSMp(Byte *, Byte *, int , int , int , int );
void VSMvlinep(Byte *, int , int ,int , int , int );
void VSMhlinep(Byte *, int , int , int , int , int );
void VSMPutImage2cop(Byte *, int , int , void *, int , int );
void VSMPutImagep(Byte *, int , int , void *, int );
void *VSMGetImage(Byte *, int , int , int , int , void *);
void VSMClrBoxp(Byte *, int , int , int , int y2, int );
void VSMengput2p(Byte *, int , int , Byte , int , int );
void VSMhanput2p(Byte *, int , int , Byte , Byte , int , int );
void VSMprintxy2p(Byte *, int , int , char *, int , int );

#define PutPixelp(PTR,x,y,co)   (*(PTR+x+y*320)=co)
#define GetPixelp(PTR,x,y)      (*(PTR+x+y*320))
#define VSMprintxy(x,y,s,co)    VSMprintxy2(x,y,s,co,0)
#define VSMClrScr(PTR,co)       memset(PTR,co,64000U)

#define puteng(x,y,ch,co)          engput(x,y,ch,co)
#define puthan(x,y,ch1,ch2,co)     hanput(x,y,ch1,ch2,co)
#define Vline(p1,p2,p3,p4,p5)      vline(p1,p2,p3,p4,p5)
#define Hline(p1,p2,p3,p4,p5)      hline(p1,p2,p3,p4,p5)
#define small(v1,v2)               (((v1)<(v2))?(v1):(v2))
#define big(v1,v2)                 (((v1)>(v2))?(v1):(v2))

#define PutPixel(x,y,co)           (*(VGA_VRAM+x+y*320)=co)
#define GetPixel(x,y)              (*(VGA_VRAM+x+y*320))
#define ImageSize(x1,y1,x2,y2)     ((x2-x1+1)*(y2-y1+1)+4)
#define gclrscr()                  ClearScreen(0)
#define gclrbox(x1,y1,x2,y2)       ClrBox( x1, y1, x2, y2, 15 )
#define getimage2(x1,y1,x2,y2,buf) GetImage(x1,y1,x2,y2,buf)
#define putimage2(x1,y1,buf,mode)  PutImage(x1,y1,buf,mode)
#define imagesize2(x1,y1,x2,y2)    ImageSize(x1,y1,x2,y2)
#define putpixel2(x,y,co)          PutPixel(x,y,co)
#define getpixel2(x,y)             GetPixel(x,y)
#define rectangle2(x1,y1,x2,y2,co) ( Hline( x1, x2, y1, 1, co), \
				     Hline( x1, x2, y2, 1, co), \
				     Vline( y1, y2, x1, 1, co), \
				     Vline( y1, y2, x2, 1, co)  \
				    )
#define getimage256(x1,y1,x2,y2,buf) GetImage(x1,y1,x2,y2,buf)

typedef struct {
    int x, y;
    char ch;
} FONT;

int loadPCX256( char *filename, unsigned char far *buf);
void update_pallette( unsigned char *DAC );

#endif
