/*========================================*/
/*                                        */
/*      THIS IS C   Total Library         */
/*                                        */
/*           "Initial Header"             */
/*                Module                  */
/*                                        */
/*     Programmed by Lee Kwang Chul       */
/*                                        */
/*     HITEL ID : fox8169 (Priend)        */
/*     TEL      : (062) 263-6669          */
/*                                        */
/*========================================*/

/* ÄñÌa·© ¡¡•I·i ”Ñw(LARGE)¡¡•I¡ Ða»¡ ´g·a¡e µAœá¡A¯¡»¡Ÿi …”a. */
#ifndef __LARGE__
#error SED Ver 3.05 Only supports Large Model
#endif

#include <dos.h>

/*
   VGA 320*200, 256 Color Plane mode Graphic Kernel MACRO
   §aˆa 320 ‰³Ða‹¡ 200 Ð¬w•¡ 256¬‚ Íw¡e ¡¡—a ‹aœÏ¢  Ça¡ */

/* ¬a¶w¸a ¸÷·(Users Type) */
#ifndef __USERS_TYPE
#define __USERS_TYPE
  typedef unsigned char BYTE;     /* BYTE ¡ ¬¡ ¸÷· */
  typedef unsigned int  WORD;     /* WORD ¡ ¬¡ ¸÷· */
  typedef unsigned long DWORD;    /* DWORD ¡ ¬¡ ¸÷· */
#endif

/* addimg.c */
void imgHFill(WORD vseg,WORD voff,WORD fs,BYTE color);
void setStAdd(WORD startAddress);
void Hard_Scroll(WORD x,WORD y);    /* Ða—a¶Á´á ¯aÇa© */
void setBitMask(BYTE mask,BYTE color);
void writeMode(BYTE mode); /* §aˆa ³a‹¡¡¡—a ‰i¸÷ */

/* bar.c */
pImgFill_L(WORD offOfY,WORD x1,WORD vbpl,WORD x2,WORD ys,BYTE color);
void pImgFill(WORD offOfY,WORD x1,WORD vbpl,WORD x2,WORD ys,BYTE color);
void CBar(int x1,int y1,int x2,int y2,BYTE c);  /* Ã©Ð»¥ ¤b¯a */

/* box.c */
void Box(int x1,int y1,int x2,int y2,BYTE c); /* ¤b¯a ‹aŸ¡‹¡ */
void CBox(int x1,int y1,int x2,int y2,BYTE c);
void L_Box(int x,int y,int xl,int yl,BYTE c);
void CL_Box(int x,int y,int xl,int yl,BYTE c);

/* circle.c */
void Circle(int x0,int y0,int r,BYTE c); /* ¶¥‹aŸ¡‹¡ */
void CCircle(int x0,int y0,int r,BYTE c);
void Fill_Circle(int x0,int y0,int r,BYTE fc);
void CFill_Circle(int x0,int y0,int r,BYTE fc);
void Circle_Out(int x0,int y0,int r,BYTE fc);
void Border_Circle(int x0,int y0,int r1,int r2,int step,int c);

/* flood.c */
WORD Flood(int x,int y,BYTE fc,BYTE bc);
WORD Flood2(int x,int y,BYTE fc,BYTE ic);

/* hangul.c */
#define FONT_NUMBER 22   /* Í¥Ëa¸÷· ¬‚Œi ˆ•® */
extern BYTE font_color[FONT_NUMBER];  /* Í¥Ëa Äiœa ¤µi 22ˆ */
extern int HANGUL_DELAY; /* Ðe‹i·¡ Â‰b–A“e ­¢•¡Ÿi ¹¡¸÷ */
#define BEAU_FONT    0    /* ´aŸq”a¶… Ðe‹i 1 */
#define BEAU1_FONT   1    /* ´aŸe”a¶… Ðe‹i 2 */
#define BEAU2_FONT   2
#define NORMAL_FONT  3    /* ¥¡É· ‹i¸a */

#define YES    1       /* µ     */
#define NO     0       /* ´a“¡µ¡ */

void puts_lee(int x, int y, char *s, int color, int flag, int choose);
void Printf_Han(int x, int y, int color, int ss, int font, char *format,...);
void Put_Han(int x, int y, int color, int ss, int font, int si, char *format,...);
char Input_String( int x, int y, char *data, BYTE color, BYTE back, char attr, char type, int number);

/* hvline.c */
void pImgHFill(WORD offOfY,WORD x1,WORD x2,BYTE color);
pImgHFill_L(WORD offOfY,WORD x1,WORD x2,BYTE color);
void CH_Line(int x1,int y1,int x2,BYTE c);
void pImgVFill(WORD offOfY,WORD XPos,WORD vbpl,WORD ys,BYTE color);
void CV_Line(int x1,int y1,int y2,BYTE c);

/* image.c */
extern BYTE COPY_MODE;    /* 0·¡¡e ¸aµe¯aœó‰A ³a‹¡ 1·¡¡e ”ü´á³a‹¡ */

void GetImg(WORD offOfY,WORD XPos,WORD vbpl,WORD xs,WORD ys,WORD dseg,WORD doff,WORD dbpl);
void pSprCp0_7(WORD sseg,WORD soff,WORD sbpl,WORD xs,WORD ys,WORD offOfY,WORD XPos,WORD vbpl,BYTE mode,BYTE copy_mm);
void pZoomSpr0123(WORD sseg,WORD soff,WORD sbpl,WORD xs,WORD ys,WORD offOfY,WORD XPos,WORD vbpl,WORD xl,WORD yl,BYTE mode, BYTE cc_mode);
void Spr_Mode(int x,int y,BYTE far *p,int xs,int ys,int m);
void ZSpr_Mode(int x,int y,BYTE far *p,int xs,int ys,int xz,int z,BYTE m);

/* keyintr.c */
extern char _key[128];              /*  éË¡Ç¡ ¤µi */
extern BYTE _key_press;
extern char MULTI_KEY_FLAG;  /*  éË¡Ç¡ˆa ­AËa–A´á·¶“e»¡ ´i´a“e ¸åµb¥e® */

void Multi_Key();    /*  éË¡Ç¡ ­AËa */
void Restore_Key();  /*  éË¡Ç¡ { */
char Multi_Kbhit();  /*  éË¡Ç¡µA¬á ´a¢Ç¡a ’‰a¹v·a¡e 1·¡ ñ´áµ¥”a. */
void Multi_Clear();  /*  éË¡Ç¡ ¤µi·i Œu·¡ »¡¶…”a. <´a¢Ç¡•¡ ’Ÿa»¡ ´g“e ¬wÈ¡>

/* line.c */
void pVRAMSprCp(WORD voff,WORD scs,WORD xs,WORD ys,WORD mseg,WORD moff,WORD doff,WORD dcs);

/* mem.c */
void asmDecode(BYTE far *s,BYTE far *d,WORD l);
void mem2DCp(WORD sseg,WORD soff,WORD sbpl,WORD xs,WORD ys,WORD dseg,WORD doff,WORD dbpl);
void memCopy(WORD sseg,WORD soff,WORD tsize,WORD dseg,WORD doff);

/* mouse.c */
#define LB 0x1    /* ¶E½¢ ¤áËe   */
#define RB 0x2    /* µ¡Ÿe½¢ ¤áËe */
#define MB 0x4    /* º—ˆe  ¤áËe  */

#define NO_CLICK 0
#define OK_CLICK 1
#define END_CLICK 2

extern int M_X, M_Y, M_But;  /*  a¶¯a¹ÁÎaµÁ ¤áËe¬wÈ */

void Init_Mouse();    /*  a¶¯a —aœa·¡¤áŸi Á¡‹¡ÑÁ Ðe”a. */
void Show_Mouse();    /*  a¶¯a Í¡·¥ÈáŸi ÑÁ¡eµA aÈa…”a. */
void Hide_Mouse();    /*  a¶¯a Í¡·¥ÈáŸi ÑÁ¡eµA¬á ¬b¹AÐe”a. */
void Move_Mouse();    /*  a¶¯a Í¡·¥ÈáŸi ¶¥Ða“e ¶áÃ¡¡ µ«‹¥”a. */
void Clip_Mouse(int, int, int, int);  /*  a¶¯aˆa ÑÉ•·Ði® ·¶“e µwµb·i ¬é¸÷ */
void Speed_Mouse(int, int);  /*  a¶¯a· ¶‘»¢·¡“e ­¢•¡Ÿi ¹¡¹·Ðe”a. */
void Read_Mouse();   /*  a¶¯a· X Y ¹ÁÎaŸi ˆbˆb mxµÁ my¡ ˆå‘Aº…”a. */
char M_Click(int left,int top,int right,int bottom,int button);
/* ¤ñ¶á‘AµA¬á »¡¸÷Ðe ¤áËe·¡ ’‰Ÿ¡¡e 1·¡Ÿ¡Èå ´a“¡¡e 0·¡Ÿ¡Èå */

void Set_Mouse_Cursor(int *cursor );
void Set_Default_Cursor( int index );

/* music.c */
#define MDI        -1  /* MDI ÑÁ·©·±·i aÈa‘ */
#define RANDOM     -2

char Mus_Number(char *filename);      /* ÐsÁa¹a ·¶“e ·q´bˆ•®Ÿi ´i´a…”a. */
void Mus_Play(char *fn, char number); /* ·q´b·i µeºÐe”a. */
void Mus_End();                       /* ·q´b·i {…”a. */
extern char Mus_Continue;             /* ‰­¢ µeºÐiˆõ·¥ˆa ¸÷· */

/* page.c */
void Page_Line(int x1,int y1,int x2,int y2,int SourcePage,int TargetPage);
void CPage_Line(int x1,int y1,int x2,int y2,int SourcePage,int TargetPage);

#define Page_HLine(X1, Y1, X2, P, P1) Page_Line((X1),(Y1),(X2),(Y1),(P),(P1))
#define CPage_HLine(X1, Y1, X2, P, P1) CPage_Line((X1),(Y1),(X2),(Y1),(P),(P1))
#define Page_VLine(X1, Y1, Y2, P, P1) Page_Line((X1),(Y1),(X1),(Y2),(P),(P1))
#define CPage_VLine(X1, Y1, Y2, P, P1) CPage_Line((X1),(Y1),(X1),(Y2),(P),(P1))

void Page_Box(int x, int y, int x1, int y1, char page, char page1);
void CPage_Box(int x, int y, int x1, int y1, char page, char page1);
void Page_Circle(int x0,int y0,int r,char page, char page1);
void CPage_Circle(int x0,int y0,int r,char page, char page1);
void Page_FCircle(int x0,int y0,int r,char page, char page1);
void CPage_FCircle(int x0,int y0,int r,char page, char page1);
void Page_Tri(int x1,int y1,int x2,int y2,int x3,int y3,char page,char page1);
void CPage_Tri(int x1,int y1,int x2,int y2,int x3,int y3,char page,char page1);
void Page_FTri(int x1,int y1,int x2,int y2,int x3,int y3,char page,char page1);
void CPage_FTri(int x1,int y1,int x2,int y2,int x3,int y3,char page,char page1);

void Save_Page(int page);    /* ÍA·¡»¡Ÿi ¸á¸wÐe”a. */
void Restore_Page(int page); /* ¸á¸wÐe ÍA·¡»¡ ¥¢Š */
void End_Page(int page);

/* palette.c */
#ifndef __PALETTE_STRUCT
#define __PALETTE_STRUCT
typedef struct {              /* ÌiAËa Š¹¡ÁA ¬å´å */
    BYTE red,green,blue;
} palette;
#endif

extern BYTE Img_Pal[768];  /* ´sÂ‚·i Î‰‹¡¶áÐe ¸åµb ¥e® */
extern Main_Pal[256];      /* MAIN ÌiAËa */
extern char PALETTE_SET;   /* ÌiAËaŸi ¤aŽ‰ˆõ·¥»¡ ¸÷· : DEFAULT : 1 */

void Wait_Snow();       /* ®»¢ÒAŠAº—·©˜Œa»¡ ‹¡”aŸ¥”a. */
void Set_Palette();     /* ÌiAËaŸi ­AËa¯¡Ç¥”a. ‹aŸ±·i ·ª´ö·i˜ */
void Set_NPal(BYTE start, BYTE end, BYTE *pal);
void Get_NPal(BYTE start, BYTE end, BYTE *pal);
void Set_BPal(BYTE *pal);   /* ÌiAËaŸi §i¢·a¡ ­AËaÐe”a. */
void Get_BPal(BYTE *pal);   /* ÌiAËaŸi §i¢·a¡ ·ª“e”a.   */
void Set_RGB(BYTE palnum, BYTE red, BYTE green, BYTE blue);    /* ÌiAËaŸi Ðaa³¢ ­AËaÐe”a. */
void Get_RGB(BYTE color, BYTE *red, BYTE *green, BYTE *blue);  /* ÌiAËaŸi Ðaa³¢ ·ª“e”a. */
void Mix(BYTE *source, BYTE *target, BYTE mix);
void Mix_In(BYTE *target, BYTE speed);
void Mix_Out(BYTE *target, BYTE speed);
/* Àá·q¤åÑ¡ ¦Èá { ¤åÑ¡Œa»¡ ¤j‹¡Ÿi Ìá­EËa¡ ¹¡¸÷  factor : 255  ¶¥œ ¤j‹¡ */
void Fade(BYTE start, BYTE end, BYTE factor);
void Fade_In(BYTE start, BYTE end, BYTE speed);  /* ÑÁ¡e·i ¬á¬áÓ¡ ¤j´a»¡‰A Ðe”a. */
void Fade_Out(BYTE start, BYTE end, BYTE speed); /* ÑÁ¡e·i ¬á¬áÓ¡ ´á–´á»¡‰A Ðe”a. */
void Set_Gray(BYTE start, BYTE end, char mode);
void Load_Pal(char *file);     /* ÌiAËaŸi ÑÁ·©µA¬á ·ª´áµ¥”a. */
void Save_Pal(char *file);     /* ÑÁ·©µA ³e”a. */
void Load_APal(char *file, BYTE *pal);     /* ÌiAËaŸi ÑÁ·©µA¬á ·ª´áµ¥”a. */
void Save_APal(char *file, BYTE *pal);     /* ÑÁ·©µA ³e”a. */
void Set_Cycle_Alone();
void Restore_Cycle_Alone();
void Set_Cycle(BYTE start, BYTE end, BYTE delay_time);
void Rotate_Cycle(BYTE start, BYTE end, BYTE delay_time);
void Restore_Cycle();

#define Read_Pal() Get_BPal((void *)Main_Pal);

/* pixel.c */
void Pix_Put(WORD offOfY,WORD XPos,BYTE color);  /* ¸ñ¿¢‹¡ */
void CPut_Pixel(int x,int y,BYTE c);
BYTE Get_Pix(WORD offOfY,WORD XPos);             /* ¸ñ ·ª´áµ¡‹¡ */

/* plane.c */
extern WORD far *_PPOFF[4] ;   /* 4 ÍA·¡»¡ Íw¡e ¡¡—a ÍA·¡»¡ ¯¡¸b ´á—aA¯a µ¡Ïa­[ */
extern WORD far *_POFF ;       /* 4 ÍA·¡»¡ Íw¡e ¡¡—a ¸b´óÍA·¡»¡ µ¡Ïa­[ Í¡·¥Èá */
extern WORD far  _PYOFF[800] ; /* 4 ÍA·¡»¡ Íw¡e ¡¡—a ‹aœÏ¢ ¹ÁÎa µ¡Ïa­[ */
extern int  _XL,_YL,_XR,_YR ;  /* ¸aŸa‹¡ ÀwËi Š¬â ¹ÁÎaˆt */
extern WORD  _VBPL ;           /* §aˆa §¡—¡µ¡œ‘ Ðeº‰”w ¤a·¡Ëa ® */
extern WORD  _VBPP ;           /* §aˆa §¡—¡µ¡œ‘ ÐeÑÁ¡e”w ¤a·¡Ëa® */
extern BYTE LogicFlag ;        /* VGA Internal Logical Fn Flag */
extern BYTE _tmpRGB[768] ;     /* termporary Palette Buffer */
extern WORD _VRAMSEG ;         /* Video RAM Segment */
extern BYTE _ET3000  ;         /* hardware panning Format */

void setMapMask(BYTE mask);
void Plane_Mode(void);       /* ·A¬a Ì‚ ¡¡—aµA¬á Íw¡e ¡¡—a¡ ¸åÑÅ */
void Pack_Mode(void);        /* ·A¬a Íw¡e¡¡—aµA¬á Ì‚ ¡¡—a¡ ¸åÑÅ */
void Set_Mode(char mode);    /* ¡¡—a ¬é¸÷ */
void Vga_Plane();            /* ÏiA·¥¡¡—a¡... */
void Text_Mode();            /* TEXT MODE ¡... */
int Set_Xy(int width,int height);  /* Ð¬w•¡ »¡¸÷ */

/* poly_tri.c */
void Tri(int x1,int y1,int x2,int y2,int x3,int y3,BYTE color);  /* ¬qˆbÑw */
void CTri(int x1,int y1,int x2,int y2,int x3,int y3,BYTE color); /* ¬qˆbÑw(ÇiŸ¡Ï·) */
void Fill_Tri(int x1,int y1,int x2,int y2,int x3,int y3,BYTE color);
void CFill_Tri(int x1,int y1,int x2,int y2,int x3,int y3,BYTE color);
void Poly(int x[],int y[],int n,BYTE color);  /* ”aˆb¬å ‹aŸ¡‹¡ */
void CPoly(int x[],int y[],int n,BYTE color); /* ”aˆb¬å ‹aŸ¡‹¡(ÇiŸ¡Ï·) */

/* rot_spr.c */
void Rot_Mode(int x,int y,BYTE *spr,int xs,int ys,int angle);

/* spr_pcx.c */
/* ¯aÏaœa·¡Ëa ÑA”á ¬å´å */
#define SPR_HEADER "Sprites Data File Ver 3.05\x1a"
#define OVL_HEADER "Over-Lap Data File Ver 3.05\x1a"

/* ÑA”áˆa º—¥¢–A“e ˆé  b‹¡¶áÐ */
#ifndef __SPRITE_STRUCT
#define __SPRITE_STRUCT
typedef struct {  /* ¯aÏaœa·¡Ëa 1ˆµA ”Ðe Š¹¡ÁA ¥e® ¬å´å */
    WORD size;   /* size - 4  ¯©¹A •A·¡Èa ‹©·¡ */
    int x, y;    /* X Ça‹¡ Y Ça‹¡ */
    BYTE *image; /* ¯aÏaœa·¡Ëa ·¡£¡»¡ Í¡·¥Èá ¥e® */
}sprites;
#endif

/* <Àq‰¡> PCX ÑÁ·©Ñw¯¢·e œa·¥ ´sÂ‚·¡”a. ¡y¤å‰Á ‹a ”a·qº‰ Àá·qˆt·¡ ˆ{´a•¡ ´sÂ‚·i
   Ða»¡ ´g“e”a. Ða»¡ e SED “e ‹aœå‰A ´ô‹¡˜¢…µA ´sÂ‚Òa·I·¡ ”á ¹½”a. */

extern BYTE *DECODE; /* ´sÂ‚·i Î‰‹¡¶áÐe Í¡·¥Èá ¥e® */
char GActive_Page(); /* ¸b´ó ÍA·¡»¡Ÿi ´i´a…”a. */

char Decode_Init(BYTE msize); /* ´sÂ‚·i Î‰‹¡¶áÐe ¤µi·i ¸s“e”a. */
/* msize * 1000  µ:Decode_Init(64);  64000¤a·¡Ëa eÇq ¤µi·i ¸s“e”a. */
/* Pcx ÑÁ·©·i µi˜ 64  ¥¡É··i 64*64 = 4096  Decode_Init(5) */
/* ¯©ÌÐa¡e -1·¡ ñ´áµ¥”a. */

void Decode_End();   /* DECODE µA Ði”w–E ¡A¡¡Ÿ¡ ¥¢Š */

void Decode_Data(sprites *data); /* ´sÂ‚·i Î“e Ðq® */
void Incode_Data(sprites *data); /* ´sÂ‚·i Ða“e Ðq® */
void Incode_Pcx(sprites *Buff);  /* PCX ÑÁ·©¡ ´sÂ‚Ði˜ ³a·¥”a. */

void Decode_Spr(int start, int end, sprites *Spr, char msize); /* ¤åÑ¡¡ ´sÂ‚·i Î…”a. */
void Incode_Spr(int start, int end, sprites *Spr, char msize); /* ¤åÑ¡¡ ´sÂ‚·i Ðe”a. */

char Get_Rle(sprites *Spr); /* ´sÂ‚·¡ –A´á·¶“e»¡ ´i´a“e Ðq® */
                            /* ´sÂ‚ : 1, ´sÂ‚´e–Q : 0 ·¡ñ´áµ¥”a. */

/* ´sÂ‚–E ‹a”¡ ¯aÏaœa·¡ËaŸi ·ª“e”a. */
char Load_Spr(int Start, int End, char *sprfile, sprites *Spr);
void Save_Spr(int Start, int End, char *sprfile, sprites *Spr);
int Spr_Number(char *sprfile);   /* ¯aÏaœa·¡Ëa· ˆ•®Ÿi ˆå‘Aº…”a. */

void Free_Spr(int Start, int End, sprites *Spr); /* ¡A¡¡Ÿ¡ ¥¢Š */
void Free(sprites *Spr);   /* {ˆ¡ ¥¢ŠÐe”a. */

/* ÑÂ”Ÿi Ð¬á ÑÁ¡eµA ¿¢“e”a. */
/* º· : ´sÂ‚·i Î‰´á¬á ¿¢´á´¡ Ðe”a. */
/* µ : Decode_Data(sprites *Buff); */
/* ZPut_Spr(x,y,*Buff,xs,ys,mode); */
void ZPut_Spr(int x, int y, sprites *Buff, int xs, int ys, BYTE mode);
void ZCPut_Spr(int x, int y, sprites *Buff, int xs, int ys, BYTE mode);

/* ‹a· ÑÁ¡eµA ¿¢“e”a. */
void Put_Spr(int x, int y, sprites *Buff, BYTE mode);
void CPut_Spr(int x, int y, sprites *Buff, BYTE mode);

/* x,y xs,ys · ˆa¶…•A¬á ÑÂ”Ÿi Ðe”a. */
void CZPut_Spr(int x, int y, sprites *Buff,int xs,int ys,unsigned char mode);
void DZPut_Spr(int x, int y, sprites *Buff,int xs,int ys,unsigned char mode);
void Rot_Spr(int x, int y, sprites *Buff, int angle);

/* ´sÂ‚·i Î‰´á¬á ÑÁ¡eµA ¿¢“e”a. */
/* ¶¥œ ·¡£¡»¡“e ‹a”¡ ·¶”a. ´sÂ‚–E ¬wÈ¡ */
void RZPut_Spr(int x, int y, sprites *Buff, int xs, int ys, unsigned char mode);
void RPut_Spr(int x, int y, sprites *Buff, unsigned char mode);

void Get_Spr(int x, int y, sprites *Buff, int xs, int ys);
void RGet_Spr(int x, int y, sprites *Buff, int xs, int ys);

char Load_Pcx(char *pcxfile, sprites *Buff);  /* PCX ÑÁ·©·i ·ª“e”a. */
void Save_Pcx(char *pcxfile, sprites *Buff);
/* µ : Load_Pcx("ÑÁ·©·¡Ÿq",&Pcx[0]); */
/* RPut_Spr(x,y,&Pcx[0],mode); */

char Load_Cel(char *celfile, sprites *Buff);  /* CEL ÑÁ·©·i ·ª“e”a. */
void Save_Pcx(char *celfile, sprites *Buff);

char Load_Sed(char *sedfile, sprites *Buff);  /* SED ÑÁ·©·i ·ª“e”a. */
void Save_Pcx(char *sedfile, sprites *Buff);

char Load_Ovl(char *ovlfile, sprites *ovb, BYTE *pal, BYTE *pal1);
void Save_Ovl(char *ovlfile, sprites *ovb, BYTE *pal, BYTE *pal1);

/* ¡¡¸a·¡Ça ‹¡¤ó */
void Mojaik(int x,int y,sprites *mojaik, BYTE zoom);
void CMojaik(int x,int y,sprites *mojaik, BYTE zoom);

/* text.c : ÉB¯aËa Ðq® */
void Text_Open(int delay);
void Text_Close();

/* util.c : ³i eÐe ·AË©Ÿ¡Ë¡ */
#define ON  1             /* ¡¡“¡Èá ¬wÈ Åq */
#define OFF 0             /* ¡¡“¡Èá ¬wÈ q */

void Delay(WORD milli_sec);  /* 1/1000 Á¡”e¶á¡ ‹¡”aŸ¥”a. */
void Monitor(char onoff);    /* ¡¡“¡ÈáŸi Åa‰¡ e”a. */
int Get_Key();     /* Ç¡Ÿi ·³b ¤h“e”a. */
int Action_Key();  /* Ç¡Ÿi ¯©¯¡ˆe·a¡ ·³b ¤h“e”a. */
void Char_To_Int( char char_high, char char_low, int *integer );
void Int_To_Char( int integer, char *char_high, char *char_low );
void Get_Path( char *path );
char *str( int number );
char File_Exist(char *filename);   /* ÑÁ·©·¡ ·¶“e»¡ ´i´a“e Ðq® */

/* voice.c : ·q¬÷ Â‰b */
#define Sample setSampleRate     /* ·q¬÷ ¬‘ÏiŸ· */
#define Volume setPCMVolume

#define VOC        -1            /* VOC ÑÁ·©·±·i aÈa‘ */

extern int VoiceFlag  ;   /* ·q¬÷ Â‰b ¬wÉA ¥e® */
extern int SBLBaseIO ;     /* default sound blaster Base IO Address */
extern int SBLIRQ ;        /* default sound blaster hardware interrupt */
extern int SBLDMAChannel ; /* default sound blaster DMA channel */
extern int CovoxIO ;       /*  Å¡¥¢¯a Í¡Ëa¤åÑ¡ Äiœa´á”“Èá ¥wi Í¡Ëa 1 */
extern WORD SampleRate ;
extern BYTE MultiVoice ;

/* ·q¬÷ Â‰b Ða—a¶Á´á ¸÷· */

#define NO_VOICE      0 /* ·q¬÷ Â‰b·i Ða»¡ ´g“e”a. */
#define SOUNDBLASTER  2 /* ¬a¶…—a §iœ¯aÈá */
#define PC_SPEAKER    1 /* Ï¡¯¡ ¯aÏ¡Äá */
#define COVOX         3 /* Å¡¥¢¯a */

#define VOICE_IDLE 0    /* ·q¬÷ Â‰bº—·¡ ´a“¡”a. */
#define VOICE_BUSY 1    /* ·q¬÷ Â‰bº—·¡”a. */

/* ·q¬÷·i Â‰bÐe”a. */
#define Voice_Say(F, S) sayPCM((BYTE *)(F),(char)S,0)

/* ·q¬÷ Â‰b·¡ {i˜ Œa»¡ ‹¡”aŸ¥”a. */
#define Wait_Voc()    while(VoiceFlag==VOICE_BUSY)
#define Wait_Voc_Key()  while(!kbhit()) while(VoiceFlag==VOICE_BUSY) break;

void Voice_End(void) ;          
int Voice_Init(int SndDev);  /* º¥e¸wÃ¡ Á¡‹¡ÑÁ */
int Voice_Silent(int flag);  /* ·q¬÷·i ˆw¹A¡ º—”e. */
void setSoundBlasterENV(int BaseIO,int IRQ,int DMAChannel);  /* ¬a¶…—a §iœ¯aÈá Á¡‹¡ÑÁ */
int SB_Check(void);          /* ¬a§i·¡ ·¶“e»¡ ÁAÇaÐe”a. */

/* win_box.c : ¶¥•¡¶ ¤b¯a */
void Box_3d(int x1, int y1, int x2, int y2, BYTE color, BYTE color1);
void CBox_3d(int x1, int y1, int x2, int y2, BYTE color, BYTE color1);
void Win_3d( int x1, int y1, int x2, int y2, BYTE s, BYTE s1,BYTE s3, BYTE q);
void Box_3dB(int x1, int y1, int x2, int y2, BYTE s, BYTE s1, BYTE s2);
void CBox_3dB(int x1, int y1, int x2, int y2, BYTE s, BYTE s1, BYTE s2);
void Win_3dB( int x1, int y1, int x2, int y2, BYTE s, BYTE s1,BYTE color, BYTE q, BYTE q1);
void CWin_3dB( int x1, int y1, int x2, int y2, BYTE s, BYTE s1,BYTE color, BYTE q, BYTE q1);
void Box_Beau(int x, int y, int x1, int y1, int mm, BYTE color);
void Xor_Pixel(WORD x, WORD y, BYTE color);
void Xor_CPixel(WORD x, WORD y, BYTE color);
void Xor_HLine(int x, int y, int x1, BYTE color);
void Xor_CHLine(int x, int y, int x1, BYTE color);
void Xor_VLine(int x, int y, int y1, BYTE color);
void Xor_CVLine(int x, int y, int y1, BYTE color);
void Xor_Box(int x, int y, int x1, int y1,BYTE color);
void Xor_CBox(int x, int y, int x1, int y1,BYTE color);
void Put_Msg(BYTE y, char *string, BYTE win_color, BYTE color, BYTE wait_time);
BYTE Yes_No(BYTE y, char *string, BYTE win_color, BYTE color, char sub,char yes_no);

#define _SCAN400LINE 0x1 /* 400 º‰ ¯aÅE œa·¥ */
#define _SCAN350LINE 0x2 /* 350 º‰ ¯aÅE œa·¥ */
#define _SCAN480LINE 0x3 /* 480 º‰ ¯aÅE œa·¥ */

#define _CLOCK25MHZ  0x0 /* 25 MHZ DOT CLOCK */
#define _CLOCK28MHZ  0x1 /* 28 MHZ DOT CLOCK */

#define _SCREEN_ON   0x0 /* ÑÁ¡e Îa¯¡ ON */
#define _SCREEN_OFF  0x1 /* ÑÁ¡e Îa¯¡ OFF */

#define setLogic()   (LogicFlag=1)
#define resetLogic() (LogicFlag=0)

#define _PSET  0x00      /* §aˆa ¦ ‘¥Ÿ¡Ðq®ˆa »¢¸ó ³a‹¡ */
#define _PAND  0x08      /* §aˆa ¦ ‘¥Ÿ¡Ðq®ˆa AND  ³a‹¡ */
#define _POR   0x10      /* §aˆa ¦ ‘¥Ÿ¡Ðq®ˆa OR   ³a‹¡ */
#define _PXOR  0x18      /* §aˆa ¦ ‘¥Ÿ¡Ðq®ˆa XOR  ³a‹¡ */

/* ”a·qµA ¬å´å·¡ –A´á ·¶“e  Ça¡“e 4 ÍA·¡»¡ Íw¡e¡¡—a  ¸å¶w·± */

/* ÑÁ¡eµA ¥¡·¡“e ÍA·¡»¡ ¬é¸÷ */
#define View_Page(P)          setStAdd(*_PPOFF[(P)])

/* ¸b´ó ÍA·¡»¡ ¬é¸÷ –ˆ· Ðq®ˆa ™¢ˆ{·q, ¬a¶wÐa‹¡ ®ó‰A */
#define Work_Page(P)         _POFF = _PPOFF[(P)]
#define Active_Page(P)       _POFF = _PPOFF[(P)]

/* ”aº— ÍA·¡»¡Ÿi ·¡¶wÐe µA“¡¡A·¡­e  Ça¡ */
#define Animate(P)          { View_Page(P) ; P = (P+1)%2 ; Active_Page(P) ; }

/* Ñe¸ ¸b´ó  ÑÁ¡e »¡¶‹¡ */
#define Work_Clear(C)           { setMapMask(0xf) ; imgHFill(_VRAMSEG,_POFF[0],_VBPP,(C)) ; }
#define Active_Clear(C)         { setMapMask(0xf) ; imgHFill(_VRAMSEG,_POFF[0],_VBPP,(C)) ; }

/* ÍA·¡»¡ ÑÁ¡e »¡¶‹¡ */
#define Page_Clear(P,C)       { setMapMask(0xf) ; imgHFill(_VRAMSEG,(*_PPOFF[(P)]),_VBPP,(C)) ; }
/* ÇiŸ¡Ï· ÑÁ¡e »¡¶‹¡ */
#define CPage_Clear(C)           CBar(_XL,_YL,_XR,_YR,(C))

/* ÍA·¡»¡µA¬á  ÍA·¡»¡¡ ÑÁ¡e ¥¢¬aÐa‹¡ */
#define Page_Copy(S,D)     { setMapMask(0xf) ; writeMode(1) ; memCopy(_VRAMSEG,(*_PPOFF[S]),_VBPP,_VRAMSEG,(*_PPOFF[D])) ; writeMode(0) ; }

/* ¹ÁÎaµA Ðe¸ñ ¿¢‹¡ */
#define Put_Pixel(X,Y,C)         Pix_Put(_POFF[(Y)],(X),(C))

/* ¹ÁÎaµA ¿¢Ó¥ ¸ñ ¬‚Œi ´è´áµ¡‹¡ */
#define Get_Pixel(X,Y)           Get_Pix(_POFF[(Y)],(X))

/* ¬aˆbÑw Ã©Ð¬á ‹aŸ¡‹¡ ( ‹aŸ¡“e Š¬â ¹ÁÎaµÁ ‹©·¡ ) */
#define L_Bar(X,Y,XL,YL,C)   pImgFill(_POFF[(Y)],(X),_VBPL,(XL)+(X)-1,(YL),(C))
/* ¬aˆbÑw Ã©Ð¬á ‹aŸ¡‹¡ ( ‹aŸ¡“e ´· Š¬â· ¹ÁÎa¡ ) */
#define Bar(X1,Y1,X2,Y2,C) pImgFill(_POFF[(Y1)],(X1),_VBPL,(X2),(Y2)-(Y1)+1,(C))

/* ¬w¸a ‹aŸ¡‰¡ Ã©Ða‹¡ */
#define CL_Bar(X,Y,XL,YL,C)   CBar((X),(Y),(X)+(XL)-1,(Y)+(YL)-1,(C))

/* ®Íw¬å ‹aŸ¡‹¡ ( ¹ÁÎaµÁ ‹©·¡¡ ) */
#define HL_Line(X,Y,L,C)     pImgHFill(_POFF[(Y)],(X),(X)+(L)-1,(C))
/* ®Íw¬å ‹aŸ¡‹¡ ( Ðe¸ñ‰Á ‹©·¡¡ ) */
#define H_Line(X1,Y,X2,C)   pImgHFill(_POFF[(Y)],(X1),(X2),(C))

/* Ðe¸ñ‰Á ‹©·¡¡ ®Íw¬å ‹aŸ¡‹¡ */
#define CHL_Line(X,Y,L,C)     CH_Line((X),(Y),(X)+(L)-1,(C))

/* ®»¢¬å ‹aŸ¡‹¡ ( ¹ÁÎaµÁ ‹©·¡¡ ) */
#define VL_Line(X,Y,L,C)     pImgVFill(_POFF[(Y)],(X),_VBPL,(L),(C))
/* ®»¢¬å ‹aŸ¡‹¡ ( ´·{ ¹ÁÎa¡ ) */
#define V_Line(X,Y1,Y2,C)   pImgVFill(_POFF[(Y1)],(X),_VBPL,(Y2)-(Y1)+1,(C))

/* ®»¢¬å ‹aŸ¡‰¡ Ã©Ða‹¡ */
#define CVL_Line(X,Y,L,C)     CV_Line((X),(Y),(Y)+(L)-1,(C))

/* §¡Ëa “ ¿¢‹¡ 
#define  Put_Bitmap(X,Y,P,XL,YL,C) pBitImgCp(FP_SEG(P),FP_OFF(P),XL,YL,_POFF[Y],X,_VBPL,C)
#define  ZPut_Bitmap(X,Y,P,XL,YL,XZ,YZ,C) \
         pZoomOutBitImgCp(FP_SEG(P),FP_OFF(P),XL,YL,_POFF[Y],X,_VBPL,C,XZ,YZ)
#define  FPut_Bitmap(X,Y,P,XL,YL,C) pFBitImgCp(FP_SEG(P),FP_OFF(P),XL,YL,_POFF[Y],(X)/4,_VBPL,C)
*/

/* ÑÁ¡e¬w· ‹aŸ± ¸s´aµ¡‹¡ */
#define Get_Image(X,Y,P,C,R)   GetImg(_POFF[(Y)],X,_VBPL,C,R,FP_SEG(P),FP_OFF(P),C)

/*
   ¯aÏaœa·¡ËaŸi ‹aŸ¡‹¡  Ça¡
   X,Y ¹ÁÎa
   P ¯aÏaœa·¡Ëa Í¡·¥Èá
   C,R ¯aÏaœa·¡Ëa ˆa¡­A¡ Ça‹¡
   M ‹aŸ© ¡¡—a
    0 : ¤a¡‹aŸ¡‹¡        4 : 90•¡ ÒA¸å
    1 : ¹Á¶ ”Ã·         5 : 90•¡ ÒA¸å ¹Á¶”Ã·
    2 : ¬wÐa ”Ã·         6 : 90•¡ ÒA¸å ¬wÐa”Ã·
    3 : ¬wÐa¹Á¶ ”Ã·     7 : 90•¡ ÒA¸å ¬wÐa¹Á¶ ”Ã·
*/

#define Put_Image(X,Y,P,C,R,M)  pSprCp0_7(FP_SEG(P),FP_OFF(P),C,C,R,_POFF[Y],X,_VBPL,(M),(COPY_MODE))

/*
   ¯aÏaœa·¡ËaŸi ÑÂ” Â‚­¡Ðaµa ‹aŸ¡‹¡  Ça¡
   X,Y ¹ÁÎa
   P ¯aÏaœa·¡Ëa Í¡·¥Èá
   C,R ¯aÏaœa·¡Ëa ˆa¡­A¡ Ça‹¡
   XZ,YZ ÑÂ” ¤·I ( 32ˆa ¯©¹A ¤·I 1 ·±)
   M ‹aŸ© ¡¡—a
    0 : ¤a¡‹aŸ¡‹¡
    1 : ¹Á¶ ”Ã·
    2 : ¬wÐa ”Ã·
    3 : ¬wÐa¹Á¶ ”Ã·
*/
#define ZPut_Image(X,Y,P,C,R,XL,YL,M) pZoomSpr0123(FP_SEG(P),FP_OFF(P),C,C,R,_POFF[Y],X,_VBPL,XL,YL,(M),(COPY_MODE))

/* ÇiŸ¡Ï· ÀwËi ¬é¸÷ */
#define Set_Clip(X1,Y1,X2,Y2)  { _XL=(X1) ; _YL=(Y1) ; _XR=(X2) ; _YR=(Y2) ; }
#define View_Clip(C)  Box(_XL-1,_YL-1,_XR+1,_YR+1,C)
#define Box_Fill Bar    /* Ã©Ð»¥ ¤b¯aŸi ‹aŸ¡“e Ðq® */
#define CBox_Fill CBar  /* Ã©Ð»¥ ¤b¯aŸi ‹aŸ¡“e Ðq® */

#define GRAY  1  /* Ób¤‚ ¡¡—a */
#define COLOR 0  /* Äiœa ¡¡—a */

/* ·©¤eÇ¡µÁ ÑÂ¸wÇ¡ ¸÷· */
#define ESC    0x1b
#define ENTER  0x0d
#define SPACE  0x20

#define LEFT   0x4b00
#define RIGHT  0x4d00
#define UP     0x4800
#define DOWN   0x5000
#define HOME   0x4700
#define END    0x4f00
#define PgUp   0x4900
#define PgDn   0x5100
#define INS    0x5200
#define DEL    0x5300
#define ALT    0x0800

#define ALT_X  0x2d00

/*  éË¡Ç¡ ¸÷· ¦¦… */

#define _ESC       1
#define _F1        59
#define _F2        60
#define _F3        61
#define _F4        62
#define _F5        63
#define _F6        64
#define _F7        65
#define _F8        66
#define _F9        67
#define _F10       68
#define _F11       87
#define _F12       88

#define _ALT       56
#define _CTRL      29
#define _TAB       15
#define _BP        14
#define _SPACE     57
#define _ENTER     28
#define _UP        72
#define _DOWN      80
#define _LEFT      75
#define _RIGHT     77
#define _HOME      71
#define _END       79
#define _PGUP      73
#define _PGDN      81
#define _INS       82
#define _DEL       83
#define _LSHIFT    42
#define _RSHIFT    54
#define _SLOCK     70
#define _PRTSCR    55

#define _A  30        
#define _B  48
#define _C  46
#define _D  32
#define _E  18
#define _F  33
#define _G  34
#define _H  35
#define _I  23
#define _J  36
#define _K  37
#define _L  38
#define _M  50
#define _N  49
#define _O  24
#define _P  25
#define _Q  16
#define _R  19
#define _S  31
#define _T  20
#define _U  22
#define _V  47
#define _W  17
#define _X  45
#define _Y  21
#define _Z  44 













