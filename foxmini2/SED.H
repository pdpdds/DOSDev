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

/* ���a�� ���I�i ���w(LARGE)���I�� �a�� �g�a�e �A��A�����i ���a. */
#ifndef __LARGE__
#error SED Ver 3.05 Only supports Large Model
#endif

#include <dos.h>

/*
   VGA 320*200, 256 Color Plane mode Graphic Kernel MACRO
   �a�a 320 ���a�� 200 Ё�w�� 256�� �w�e ���a �a��Ϣ ���a�� */

/* �a�w�a ����(Users Type) */
#ifndef __USERS_TYPE
#define __USERS_TYPE
  typedef unsigned char BYTE;     /* BYTE �� ���� ���� */
  typedef unsigned int  WORD;     /* WORD �� ���� ���� */
  typedef unsigned long DWORD;    /* DWORD �� ���� ���� */
#endif

/* addimg.c */
void imgHFill(WORD vseg,WORD voff,WORD fs,BYTE color);
void setStAdd(WORD startAddress);
void Hard_Scroll(WORD x,WORD y);    /* �a�a���� �a�a�� */
void setBitMask(BYTE mask,BYTE color);
void writeMode(BYTE mode); /* �a�a �a�����a �i�� */

/* bar.c */
pImgFill_L(WORD offOfY,WORD x1,WORD vbpl,WORD x2,WORD ys,BYTE color);
void pImgFill(WORD offOfY,WORD x1,WORD vbpl,WORD x2,WORD ys,BYTE color);
void CBar(int x1,int y1,int x2,int y2,BYTE c);  /* éЁ�� �b�a */

/* box.c */
void Box(int x1,int y1,int x2,int y2,BYTE c); /* �b�a �a���� */
void CBox(int x1,int y1,int x2,int y2,BYTE c);
void L_Box(int x,int y,int xl,int yl,BYTE c);
void CL_Box(int x,int y,int xl,int yl,BYTE c);

/* circle.c */
void Circle(int x0,int y0,int r,BYTE c); /* ���a���� */
void CCircle(int x0,int y0,int r,BYTE c);
void Fill_Circle(int x0,int y0,int r,BYTE fc);
void CFill_Circle(int x0,int y0,int r,BYTE fc);
void Circle_Out(int x0,int y0,int r,BYTE fc);
void Border_Circle(int x0,int y0,int r1,int r2,int step,int c);

/* flood.c */
WORD Flood(int x,int y,BYTE fc,BYTE bc);
WORD Flood2(int x,int y,BYTE fc,BYTE ic);

/* hangul.c */
#define FONT_NUMBER 22   /* ͥ�a���� ���i ���� */
extern BYTE font_color[FONT_NUMBER];  /* ͥ�a �i�a ���i 22�� */
extern int HANGUL_DELAY; /* �e�i�� �b�A�e �����i ���� */
#define BEAU_FONT    0    /* �a�q�a�� �e�i 1 */
#define BEAU1_FONT   1    /* �a�e�a�� �e�i 2 */
#define BEAU2_FONT   2
#define NORMAL_FONT  3    /* ��ɷ �i�a */

#define YES    1       /* ��     */
#define NO     0       /* �a���� */

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
extern BYTE COPY_MODE;    /* 0���e �a�e�a��A �a�� 1���e ����a�� */

void GetImg(WORD offOfY,WORD XPos,WORD vbpl,WORD xs,WORD ys,WORD dseg,WORD doff,WORD dbpl);
void pSprCp0_7(WORD sseg,WORD soff,WORD sbpl,WORD xs,WORD ys,WORD offOfY,WORD XPos,WORD vbpl,BYTE mode,BYTE copy_mm);
void pZoomSpr0123(WORD sseg,WORD soff,WORD sbpl,WORD xs,WORD ys,WORD offOfY,WORD XPos,WORD vbpl,WORD xl,WORD yl,BYTE mode, BYTE cc_mode);
void Spr_Mode(int x,int y,BYTE far *p,int xs,int ys,int m);
void ZSpr_Mode(int x,int y,BYTE far *p,int xs,int ys,int xz,int z,BYTE m);

/* keyintr.c */
extern char _key[128];              /* ��ˡǡ ���i */
extern BYTE _key_press;
extern char MULTI_KEY_FLAG;  /* ��ˡǡ�a �A�a�A�᷶�e�� �i�a���e ��b�e�� */

void Multi_Key();    /* ��ˡǡ �A�a */
void Restore_Key();  /* ��ˡǡ �{ */
char Multi_Kbhit();  /* ��ˡǡ�A�� �a��ǡ�a ���a�v�a�e 1�� ��ᵥ�a. */
void Multi_Clear();  /* ��ˡǡ ���i�i ���u�� �����a. <�a��ǡ�� ���a�� �g�e �wȁ��>

/* line.c */
void pVRAMSprCp(WORD voff,WORD scs,WORD xs,WORD ys,WORD mseg,WORD moff,WORD doff,WORD dcs);

/* mem.c */
void asmDecode(BYTE far *s,BYTE far *d,WORD l);
void mem2DCp(WORD sseg,WORD soff,WORD sbpl,WORD xs,WORD ys,WORD dseg,WORD doff,WORD dbpl);
void memCopy(WORD sseg,WORD soff,WORD tsize,WORD dseg,WORD doff);

/* mouse.c */
#define LB 0x1    /* �E�� ���e   */
#define RB 0x2    /* ���e�� ���e */
#define MB 0x4    /* ���e  ���e  */

#define NO_CLICK 0
#define OK_CLICK 1
#define END_CLICK 2

extern int M_X, M_Y, M_But;  /* �a���a���a�� ���e�wȁ */

void Init_Mouse();    /* �a���a �a�a����i ������ �e�a. */
void Show_Mouse();    /* �a���a ͡����i ���e�A �a�a���a. */
void Hide_Mouse();    /* �a���a ͡����i ���e�A�� �b�A�e�a. */
void Move_Mouse();    /* �a���a ͡����i ���a�e ��á�� �����a. */
void Clip_Mouse(int, int, int, int);  /* �a���a�a �ɕ��i�� ���e �w�b�i ��� */
void Speed_Mouse(int, int);  /* �a���a�� �������e �����i �����e�a. */
void Read_Mouse();   /* �a���a�� X Y ���a�i �b�b mx�� my�� ��A���a. */
char M_Click(int left,int top,int right,int bottom,int button);
/* ���A�A�� �����e ���e�� �����e 1������ �a���e 0������ */

void Set_Mouse_Cursor(int *cursor );
void Set_Default_Cursor( int index );

/* music.c */
#define MDI        -1  /* MDI �������i �a�a�� */
#define RANDOM     -2

char Mus_Number(char *filename);      /* �s�a�a ���e �q�b�����i �i�a���a. */
void Mus_Play(char *fn, char number); /* �q�b�i �e���e�a. */
void Mus_End();                       /* �q�b�i �{���a. */
extern char Mus_Continue;             /* ���� �e���i�����a ���� */

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

void Save_Page(int page);    /* �A�����i ��w�e�a. */
void Restore_Page(int page); /* ��w�e �A���� ���� */
void End_Page(int page);

/* palette.c */
#ifndef __PALETTE_STRUCT
#define __PALETTE_STRUCT
typedef struct {              /* �i�A�a �����A ��� */
    BYTE red,green,blue;
} palette;
#endif

extern BYTE Img_Pal[768];  /* �s�i Ή�����e ��b �e�� */
extern Main_Pal[256];      /* MAIN �i�A�a */
extern char PALETTE_SET;   /* �i�A�a�i �a�������� ���� : DEFAULT : 1 */

void Wait_Snow();       /* �����A�A�������a�� ���a���a. */
void Set_Palette();     /* �i�A�a�i �A�a��ǥ�a. �a���i �����i�� */
void Set_NPal(BYTE start, BYTE end, BYTE *pal);
void Get_NPal(BYTE start, BYTE end, BYTE *pal);
void Set_BPal(BYTE *pal);   /* �i�A�a�i �i���a�� �A�a�e�a. */
void Get_BPal(BYTE *pal);   /* �i�A�a�i �i���a�� ���e�a.   */
void Set_RGB(BYTE palnum, BYTE red, BYTE green, BYTE blue);    /* �i�A�a�i �a�a�� �A�a�e�a. */
void Get_RGB(BYTE color, BYTE *red, BYTE *green, BYTE *blue);  /* �i�A�a�i �a�a�� ���e�a. */
void Mix(BYTE *source, BYTE *target, BYTE mix);
void Mix_In(BYTE *target, BYTE speed);
void Mix_Out(BYTE *target, BYTE speed);
/* ��q��ѡ ���� �{ ��ѡ�a�� �j���i ��E�a�� ����  factor : 255  ���� �j�� */
void Fade(BYTE start, BYTE end, BYTE factor);
void Fade_In(BYTE start, BYTE end, BYTE speed);  /* ���e�i ���ӡ �j�a���A �e�a. */
void Fade_Out(BYTE start, BYTE end, BYTE speed); /* ���e�i ���ӡ �ᖁ�ỡ�A �e�a. */
void Set_Gray(BYTE start, BYTE end, char mode);
void Load_Pal(char *file);     /* �i�A�a�i �����A�� ���ᵥ�a. */
void Save_Pal(char *file);     /* �����A �e�a. */
void Load_APal(char *file, BYTE *pal);     /* �i�A�a�i �����A�� ���ᵥ�a. */
void Save_APal(char *file, BYTE *pal);     /* �����A �e�a. */
void Set_Cycle_Alone();
void Restore_Cycle_Alone();
void Set_Cycle(BYTE start, BYTE end, BYTE delay_time);
void Rotate_Cycle(BYTE start, BYTE end, BYTE delay_time);
void Restore_Cycle();

#define Read_Pal() Get_BPal((void *)Main_Pal);

/* pixel.c */
void Pix_Put(WORD offOfY,WORD XPos,BYTE color);  /* �񿢋� */
void CPut_Pixel(int x,int y,BYTE c);
BYTE Get_Pix(WORD offOfY,WORD XPos);             /* �� ���ᵡ�� */

/* plane.c */
extern WORD far *_PPOFF[4] ;   /* 4 �A���� �w�e ���a �A���� ���b ��a�A�a ���a�[ */
extern WORD far *_POFF ;       /* 4 �A���� �w�e ���a �b���A���� ���a�[ ͡���� */
extern WORD far  _PYOFF[800] ; /* 4 �A���� �w�e ���a �a��Ϣ ���a ���a�[ */
extern int  _XL,_YL,_XR,_YR ;  /* �a�a�� �w�i ���� ���a�t */
extern WORD  _VBPL ;           /* �a�a �������� �e���w �a���a �� */
extern WORD  _VBPP ;           /* �a�a �������� �e���e�w �a���a�� */
extern BYTE LogicFlag ;        /* VGA Internal Logical Fn Flag */
extern BYTE _tmpRGB[768] ;     /* termporary Palette Buffer */
extern WORD _VRAMSEG ;         /* Video RAM Segment */
extern BYTE _ET3000  ;         /* hardware panning Format */

void setMapMask(BYTE mask);
void Plane_Mode(void);       /* �A�a ̂ ���a�A�� �w�e ���a�� ���� */
void Pack_Mode(void);        /* �A�a �w�e���a�A�� ̂ ���a�� ���� */
void Set_Mode(char mode);    /* ���a ��� */
void Vga_Plane();            /* �i�A�����a��... */
void Text_Mode();            /* TEXT MODE ��... */
int Set_Xy(int width,int height);  /* Ё�w�� ���� */

/* poly_tri.c */
void Tri(int x1,int y1,int x2,int y2,int x3,int y3,BYTE color);  /* �q�b�w */
void CTri(int x1,int y1,int x2,int y2,int x3,int y3,BYTE color); /* �q�b�w(�i��Ϸ) */
void Fill_Tri(int x1,int y1,int x2,int y2,int x3,int y3,BYTE color);
void CFill_Tri(int x1,int y1,int x2,int y2,int x3,int y3,BYTE color);
void Poly(int x[],int y[],int n,BYTE color);  /* �a�b�� �a���� */
void CPoly(int x[],int y[],int n,BYTE color); /* �a�b�� �a����(�i��Ϸ) */

/* rot_spr.c */
void Rot_Mode(int x,int y,BYTE *spr,int xs,int ys,int angle);

/* spr_pcx.c */
/* �a�a�a���a �A�� ��� */
#define SPR_HEADER "Sprites Data File Ver 3.05\x1a"
#define OVL_HEADER "Over-Lap Data File Ver 3.05\x1a"

/* �A��a �����A�e �� �b����Ё */
#ifndef __SPRITE_STRUCT
#define __SPRITE_STRUCT
typedef struct {  /* �a�a�a���a 1���A ���e �����A �e�� ��� */
    WORD size;   /* size - 4  ���A �A���a ���� */
    int x, y;    /* X �a�� Y �a�� */
    BYTE *image; /* �a�a�a���a ������ ͡���� �e�� */
}sprites;
#endif

/* <�q��> PCX �����w���e �a�� �s���a. �y��� �a �a�q�� ��q�t�� �{�a�� �s�i
   �a�� �g�e�a. �a���e SED �e �a��A �􋡘����A �s�a�I�� �� ���a. */

extern BYTE *DECODE; /* �s�i Ή�����e ͡���� �e�� */
char GActive_Page(); /* �b�� �A�����i �i�a���a. */

char Decode_Init(BYTE msize); /* �s�i Ή�����e ���i�i �s�e�a. */
/* msize * 1000  ��:Decode_Init(64);  64000�a���a�e�q ���i�i �s�e�a. */
/* Pcx �����i �i�� 64  ��ɷ�i 64*64 = 4096  Decode_Init(5) */
/* ��́�a�e -1�� ��ᵥ�a. */

void Decode_End();   /* DECODE �A �i�w�E �A���� ���� */

void Decode_Data(sprites *data); /* �s�i ΁�e �q�� */
void Incode_Data(sprites *data); /* �s�i �a�e �q�� */
void Incode_Pcx(sprites *Buff);  /* PCX ������ �s�i�� �a���a. */

void Decode_Spr(int start, int end, sprites *Spr, char msize); /* ��ѡ�� �s�i ΅�a. */
void Incode_Spr(int start, int end, sprites *Spr, char msize); /* ��ѡ�� �s�i �e�a. */

char Get_Rle(sprites *Spr); /* �s�� �A�᷶�e�� �i�a���e �q�� */
                            /* �s : 1, �s�e�Q : 0 ����ᵥ�a. */

/* �s�E �a���� �a�a�a���a�i ���e�a. */
char Load_Spr(int Start, int End, char *sprfile, sprites *Spr);
void Save_Spr(int Start, int End, char *sprfile, sprites *Spr);
int Spr_Number(char *sprfile);   /* �a�a�a���a�� �����i ��A���a. */

void Free_Spr(int Start, int End, sprites *Spr); /* �A���� ���� */
void Free(sprites *Spr);   /* �{���� �����e�a. */

/* ���i Ё�� ���e�A ���e�a. */
/* ���� : �s�i Ή��� ���ᴡ �e�a. */
/* �� : Decode_Data(sprites *Buff); */
/* ZPut_Spr(x,y,*Buff,xs,ys,mode); */
void ZPut_Spr(int x, int y, sprites *Buff, int xs, int ys, BYTE mode);
void ZCPut_Spr(int x, int y, sprites *Buff, int xs, int ys, BYTE mode);

/* �a�� ���e�A ���e�a. */
void Put_Spr(int x, int y, sprites *Buff, BYTE mode);
void CPut_Spr(int x, int y, sprites *Buff, BYTE mode);

/* x,y xs,ys �� �a���A�� ���i �e�a. */
void CZPut_Spr(int x, int y, sprites *Buff,int xs,int ys,unsigned char mode);
void DZPut_Spr(int x, int y, sprites *Buff,int xs,int ys,unsigned char mode);
void Rot_Spr(int x, int y, sprites *Buff, int angle);

/* �s�i Ή��� ���e�A ���e�a. */
/* ���� �������e �a���� ���a. �s�E �wȁ�� */
void RZPut_Spr(int x, int y, sprites *Buff, int xs, int ys, unsigned char mode);
void RPut_Spr(int x, int y, sprites *Buff, unsigned char mode);

void Get_Spr(int x, int y, sprites *Buff, int xs, int ys);
void RGet_Spr(int x, int y, sprites *Buff, int xs, int ys);

char Load_Pcx(char *pcxfile, sprites *Buff);  /* PCX �����i ���e�a. */
void Save_Pcx(char *pcxfile, sprites *Buff);
/* �� : Load_Pcx("�������q",&Pcx[0]); */
/* RPut_Spr(x,y,&Pcx[0],mode); */

char Load_Cel(char *celfile, sprites *Buff);  /* CEL �����i ���e�a. */
void Save_Pcx(char *celfile, sprites *Buff);

char Load_Sed(char *sedfile, sprites *Buff);  /* SED �����i ���e�a. */
void Save_Pcx(char *sedfile, sprites *Buff);

char Load_Ovl(char *ovlfile, sprites *ovb, BYTE *pal, BYTE *pal1);
void Save_Ovl(char *ovlfile, sprites *ovb, BYTE *pal, BYTE *pal1);

/* ���a���a ���� */
void Mojaik(int x,int y,sprites *mojaik, BYTE zoom);
void CMojaik(int x,int y,sprites *mojaik, BYTE zoom);

/* text.c : �B�a�a �q�� */
void Text_Open(int delay);
void Text_Close();

/* util.c : �i�e�e �A˩��ˡ */
#define ON  1             /* ������ �wȁ �q */
#define OFF 0             /* ������ �wȁ �q */

void Delay(WORD milli_sec);  /* 1/1000 ���e�ᝡ ���a���a. */
void Monitor(char onoff);    /* ������i �a�� �e�a. */
int Get_Key();     /* ǡ�i ���b �h�e�a. */
int Action_Key();  /* ǡ�i �����e�a�� ���b �h�e�a. */
void Char_To_Int( char char_high, char char_low, int *integer );
void Int_To_Char( int integer, char *char_high, char *char_low );
void Get_Path( char *path );
char *str( int number );
char File_Exist(char *filename);   /* ������ ���e�� �i�a���e �q�� */

/* voice.c : �q�� �b */
#define Sample setSampleRate     /* �q�� ���i�� */
#define Volume setPCMVolume

#define VOC        -1            /* VOC �������i �a�a�� */

extern int VoiceFlag  ;   /* �q�� �b �w�A �e�� */
extern int SBLBaseIO ;     /* default sound blaster Base IO Address */
extern int SBLIRQ ;        /* default sound blaster hardware interrupt */
extern int SBLDMAChannel ; /* default sound blaster DMA channel */
extern int CovoxIO ;       /*  š���a ͡�a��ѡ �i�a�ᔓ�� �w�i ͡�a 1 */
extern WORD SampleRate ;
extern BYTE MultiVoice ;

/* �q�� �b �a�a���� ���� */

#define NO_VOICE      0 /* �q�� �b�i �a�� �g�e�a. */
#define SOUNDBLASTER  2 /* �a���a �i���a�� */
#define PC_SPEAKER    1 /* ϡ�� �aϡ�� */
#define COVOX         3 /* š���a */

#define VOICE_IDLE 0    /* �q�� �b���� �a���a. */
#define VOICE_BUSY 1    /* �q�� �b�����a. */

/* �q���i �b�e�a. */
#define Voice_Say(F, S) sayPCM((BYTE *)(F),(char)S,0)

/* �q�� �b�� �{�i�� �a�� ���a���a. */
#define Wait_Voc()    while(VoiceFlag==VOICE_BUSY)
#define Wait_Voc_Key()  while(!kbhit()) while(VoiceFlag==VOICE_BUSY) break;

void Voice_End(void) ;          
int Voice_Init(int SndDev);  /* ���e�wá ������ */
int Voice_Silent(int flag);  /* �q���i �w�A�� ���e. */
void setSoundBlasterENV(int BaseIO,int IRQ,int DMAChannel);  /* �a���a �i���a�� ������ */
int SB_Check(void);          /* �a�i�� ���e�� �A�a�e�a. */

/* win_box.c : ������ �b�a */
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

#define _SCAN400LINE 0x1 /* 400 �� �a�E �a�� */
#define _SCAN350LINE 0x2 /* 350 �� �a�E �a�� */
#define _SCAN480LINE 0x3 /* 480 �� �a�E �a�� */

#define _CLOCK25MHZ  0x0 /* 25 MHZ DOT CLOCK */
#define _CLOCK28MHZ  0x1 /* 28 MHZ DOT CLOCK */

#define _SCREEN_ON   0x0 /* ���e �a�� ON */
#define _SCREEN_OFF  0x1 /* ���e �a�� OFF */

#define setLogic()   (LogicFlag=1)
#define resetLogic() (LogicFlag=0)

#define _PSET  0x00      /* �a�a ���� �����q���a ���� �a�� */
#define _PAND  0x08      /* �a�a ���� �����q���a AND  �a�� */
#define _POR   0x10      /* �a�a ���� �����q���a OR   �a�� */
#define _PXOR  0x18      /* �a�a ���� �����q���a XOR  �a�� */

/* �a�q�A ��巡 �A�� ���e ���a���e 4 �A���� �w�e���a  ��w�� */

/* ���e�A �����e �A���� ��� */
#define View_Page(P)          setStAdd(*_PPOFF[(P)])

/* �b�� �A���� ��� ������ �q���a ���{�q, �a�w�a�� ��A */
#define Work_Page(P)         _POFF = _PPOFF[(P)]
#define Active_Page(P)       _POFF = _PPOFF[(P)]

/* �a�� �A�����i ���w�e �A���A���e ���a�� */
#define Animate(P)          { View_Page(P) ; P = (P+1)%2 ; Active_Page(P) ; }

/* �e�� �b��  ���e ������ */
#define Work_Clear(C)           { setMapMask(0xf) ; imgHFill(_VRAMSEG,_POFF[0],_VBPP,(C)) ; }
#define Active_Clear(C)         { setMapMask(0xf) ; imgHFill(_VRAMSEG,_POFF[0],_VBPP,(C)) ; }

/* �A���� ���e ������ */
#define Page_Clear(P,C)       { setMapMask(0xf) ; imgHFill(_VRAMSEG,(*_PPOFF[(P)]),_VBPP,(C)) ; }
/* �i��Ϸ ���e ������ */
#define CPage_Clear(C)           CBar(_XL,_YL,_XR,_YR,(C))

/* �A�����A��  �A������ ���e ���a�a�� */
#define Page_Copy(S,D)     { setMapMask(0xf) ; writeMode(1) ; memCopy(_VRAMSEG,(*_PPOFF[S]),_VBPP,_VRAMSEG,(*_PPOFF[D])) ; writeMode(0) ; }

/* ���a�A �e�� ���� */
#define Put_Pixel(X,Y,C)         Pix_Put(_POFF[(Y)],(X),(C))

/* ���a�A ��ӥ �� ���i ��ᵡ�� */
#define Get_Pixel(X,Y)           Get_Pix(_POFF[(Y)],(X))

/* �a�b�w éЁ�� �a���� ( �a���e ���� ���a�� ���� ) */
#define L_Bar(X,Y,XL,YL,C)   pImgFill(_POFF[(Y)],(X),_VBPL,(XL)+(X)-1,(YL),(C))
/* �a�b�w éЁ�� �a���� ( �a���e �� ���ⷁ ���a�� ) */
#define Bar(X1,Y1,X2,Y2,C) pImgFill(_POFF[(Y1)],(X1),_VBPL,(X2),(Y2)-(Y1)+1,(C))

/* �w�a �a���� é�a�� */
#define CL_Bar(X,Y,XL,YL,C)   CBar((X),(Y),(X)+(XL)-1,(Y)+(YL)-1,(C))

/* ���w�� �a���� ( ���a�� ������ ) */
#define HL_Line(X,Y,L,C)     pImgHFill(_POFF[(Y)],(X),(X)+(L)-1,(C))
/* ���w�� �a���� ( �e��� ������ ) */
#define H_Line(X1,Y,X2,C)   pImgHFill(_POFF[(Y)],(X1),(X2),(C))

/* �e��� ������ ���w�� �a���� */
#define CHL_Line(X,Y,L,C)     CH_Line((X),(Y),(X)+(L)-1,(C))

/* ������ �a���� ( ���a�� ������ ) */
#define VL_Line(X,Y,L,C)     pImgVFill(_POFF[(Y)],(X),_VBPL,(L),(C))
/* ������ �a���� ( ���{ ���a�� ) */
#define V_Line(X,Y1,Y2,C)   pImgVFill(_POFF[(Y1)],(X),_VBPL,(Y2)-(Y1)+1,(C))

/* ������ �a���� é�a�� */
#define CVL_Line(X,Y,L,C)     CV_Line((X),(Y),(Y)+(L)-1,(C))

/* ���a�� ���� 
#define  Put_Bitmap(X,Y,P,XL,YL,C) pBitImgCp(FP_SEG(P),FP_OFF(P),XL,YL,_POFF[Y],X,_VBPL,C)
#define  ZPut_Bitmap(X,Y,P,XL,YL,XZ,YZ,C) \
         pZoomOutBitImgCp(FP_SEG(P),FP_OFF(P),XL,YL,_POFF[Y],X,_VBPL,C,XZ,YZ)
#define  FPut_Bitmap(X,Y,P,XL,YL,C) pFBitImgCp(FP_SEG(P),FP_OFF(P),XL,YL,_POFF[Y],(X)/4,_VBPL,C)
*/

/* ���e�w�� �a�� �s�a���� */
#define Get_Image(X,Y,P,C,R)   GetImg(_POFF[(Y)],X,_VBPL,C,R,FP_SEG(P),FP_OFF(P),C)

/*
   �a�a�a���a�i �a���� ���a��
   X,Y ���a
   P �a�a�a���a ͡����
   C,R �a�a�a���a �a���A�� �a��
   M �a�� ���a
    0 : �a���a����        4 : 90�� �A��
    1 : ���� ��÷         5 : 90�� �A�� ������÷
    2 : �w�a ��÷         6 : 90�� �A�� �w�a��÷
    3 : �w�a���� ��÷     7 : 90�� �A�� �w�a���� ��÷
*/

#define Put_Image(X,Y,P,C,R,M)  pSprCp0_7(FP_SEG(P),FP_OFF(P),C,C,R,_POFF[Y],X,_VBPL,(M),(COPY_MODE))

/*
   �a�a�a���a�i �� ���a�a �a���� ���a��
   X,Y ���a
   P �a�a�a���a ͡����
   C,R �a�a�a���a �a���A�� �a��
   XZ,YZ �� ���I ( 32�a ���A ���I 1 ��)
   M �a�� ���a
    0 : �a���a����
    1 : ���� ��÷
    2 : �w�a ��÷
    3 : �w�a���� ��÷
*/
#define ZPut_Image(X,Y,P,C,R,XL,YL,M) pZoomSpr0123(FP_SEG(P),FP_OFF(P),C,C,R,_POFF[Y],X,_VBPL,XL,YL,(M),(COPY_MODE))

/* �i��Ϸ �w�i ��� */
#define Set_Clip(X1,Y1,X2,Y2)  { _XL=(X1) ; _YL=(Y1) ; _XR=(X2) ; _YR=(Y2) ; }
#define View_Clip(C)  Box(_XL-1,_YL-1,_XR+1,_YR+1,C)
#define Box_Fill Bar    /* éЁ�� �b�a�i �a���e �q�� */
#define CBox_Fill CBar  /* éЁ�� �b�a�i �a���e �q�� */

#define GRAY  1  /* �b�� ���a */
#define COLOR 0  /* �i�a ���a */

/* ���eǡ�� �¸wǡ ���� */
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

/* ��ˡǡ ���� ���� */

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