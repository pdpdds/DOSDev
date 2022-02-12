
// #################### Jo Jae Goo Standard Game Library ####################
/*

   VGA320.LIB
      VGA320   size = 3427
      SUBHAN   size = 20176
      ENGLISH  size = 4096
      HANGUL   size = 11520
      VIEW     size = 3581

   ADLIB.LIB
      ADLIB    size = 11296
      OUTCHIP  size = 60
      SETFREQ  size = 784
      TIMER    size = 851

   VOICE.LIB
      DMAIO    size = 351
      VOICE    size = 9463

   XMS.LIB
      XMS_ASM  size = 158
      XMS_C    size = 254

   MULTIKEY.LIB
      MULTIKEY size = 439

*/


// **************************************************************************
// ********************************* vga320.c *******************************
// **************************************************************************
// ##### Jo Jae Goo VGA 13H Standard Mode 320x200 256 Color Library #####

#include <alloc.h>
#include <dos.h>

typedef unsigned char byte ;
typedef unsigned int  word ;

// VGA320.C
extern unsigned int Adress[] ;  // Video Memory Adress Table
extern int ScreenXStart ;       // Default 0
extern int ScreenYStart ;       // Default 0
extern int ScreenXEnd   ;       // Default 319
extern int ScreenYEnd   ;       // Default 199

extern void SetVga( void ) ;
extern void EndVga( void ) ;
extern void SetRgbPalette( byte *plt, int start, int len ) ;
extern void SetRgbPaletteFast( byte *plt, int start, int len ) ;
extern void SetRgbPaletteSnow( byte *pal ) ;
extern void SetPalette( int color_no, char red, char green, char blue ) ;
extern int  BufGetPixel( int x, int y, byte *tarbuff ) ;
extern void BufPutPixel( int x, int y, int color, byte *tarbuff ) ;
extern void BufXLine( int sx, int sy, word value, byte color, byte *tarbuff ) ;
extern void BufYLine( int sx, int sy, int value, int color, byte *tarbuff ) ;
extern void BufBoxLine( int sx, int sy, int ex, int ey, int color, byte *tarbuff ) ;
extern void BufBoxFill( int sx, int sy, int ex, int ey, int color, byte *tarbuff ) ;
extern void BufLine( int startx, int starty, int endx, int endy, int color, byte *tarbuff ) ;
extern void BufGetImage( word x, word y, word x1, word y1, byte *buff, byte *tarbuff ) ;
extern void BufPutImage( int x, int y, byte *buff, byte *tarbuff ) ;
extern void BufSpritePutImage( int x, int y, byte *buff, byte *tarbuff ) ;
extern void MoveData( byte *_data1, byte *_data2, word data_size ) ;
extern void SpriteMoveData( byte *_data1, byte *_data2, word data_size ) ;
extern void MoveImage( int sx, int sy, int ex, int ey, int newx, int newy, byte *tarbuff ) ;
extern void BufMoveImage( int sx, int sy, int ex, int ey, byte *sour, byte *dest ) ;
extern int  Getkey( int mode ) ;
extern long GetTime( void ) ;
extern void DelayTime( word data ) ;

// SUBHAN.C
extern void MHprint( word x, word y, byte *mess, int color ) ;
extern void BufMHprint( word x, word y, byte *mess, int color, byte *buff ) ;
extern void FillHprint( word x, word y, word value, byte *mess, int color ) ;
extern void BufFillHprint( word x, word y, word value, byte *mess, int color, byte *buff ) ;
extern void Hprint( word x, word y, byte *mess, int color ) ;
extern void BufHprint( word x, word y, byte *mess, int color, byte *buff ) ;
extern void Hprint12( word x, word y, byte *mess, int color ) ;
extern void BufHprint12( word x, word y, byte *mess, int color, byte *buff ) ;
extern void Hprint21( word x, word y, byte *mess, int color ) ;
extern void BufHprint21( word x, word y, byte *mess, int color, byte *buff ) ;
extern void Hprint22( word x, word y, byte *mess, int color ) ;
extern void BufHprint22( word x, word y, byte *mess, int color, byte *buff ) ;

// VIEW.C
extern unsigned char Pal[256][3] ;
extern int  ViewGIF( char *gif_name, byte *tarbuff ) ;
extern int  ViewPCX( char *pcx_name, byte *tarbuff ) ;

extern void ViewIMG( char *pcx_name, byte *tarbuff ) ;
extern void Open_Image_File( char *img_file_name ) ;
extern void Close_Image_File( void ) ;


// **************************************************************************
// ********************************* adlib.c ********************************
// **************************************************************************
extern int  InitAdLib( void ) ;
extern void AdLib_Start( char *mdi_name ) ;
extern int  CheckPlay( void ) ;
extern void AdLib_End( void ) ;
extern char adlib_card ;                   /* != 0 if AdLib Card Install */


// **************************************************************************
// ********************************* voice.c ********************************
// **************************************************************************
#ifndef _VOICE_MAIN_SOURCE_
extern int  VoiceFlag  ;   /* ·q¬÷ Â‰b ¬wÉA ¥e® */
extern int SBLBaseIO ;     /* default sound blaster Base IO Address */
extern int SBLIRQ ;        /* default sound blaster hardware interrupt */
extern int SBLDMAChannel ; /* default sound blaster DMA channel */
extern int CovoxIO ;       /*  Å¡¥¢¯a Í¡Ëa¤åÑ¡ Äiœa´á”“Èá ¥wi Í¡Ëa 1 */
extern unsigned int SampleRate ;
extern unsigned char MultiVoice ;
#endif

#ifndef _VOICE_HEADER_
#define _VOICE_HEADER_

/* ·q¬÷ Â‰b Ða—a¶Á´á ¸÷· */

#define NO_VOICE      0 /* ·q¬÷ Â‰b·i Ða»¡ ´g“e”a. */
#define SOUNDBLASTER  2 /* ¬a¶…—a §iœ¯aÈá */
#define PC_SPEAKER    1 /* Ï¡¯¡ ¯aÏ¡Äá */
#define COVOX         3 /* Å¡¥¢¯a */

#define VOICE_IDLE 0    /* ·q¬÷ Â‰bº—·¡ ´a“¡”a. */
#define VOICE_BUSY 1    /* ·q¬÷ Â‰bº—·¡”a. */

#define CMOS_INT_VECT 0x70 /* ¯©¯¡ˆe ·¥ÈáœóËa ¤å»¡ */

#define CMOS_WRITE(I,X) { outportb(0x70,(I)) ;  outportb(0x71,(X)) ; }
#define CMOS_READ(I)     (outportb(0x70,(I)) , inportb(0x71))

#define DIRECT_8BIT_DAC 0x10  /* ¬a§i »¢¸ó 8 §¡Ëa DAC */
#define DMA_8BIT_DAC    0x14  /* DMAŸi ·¡¶wÐe 8 §¡Ëa DAC */
#define HALT_DMA        0xd0  /* DMA ¸å­· º—»¡ */
#define FREQ_DIVISER    0x40  /* ¬‘ÏiŸ· ºÌa® ¬é¸÷ */
#define SBL_SPEAKER_ON  0xD1  /* ¬a§i ¯aÏ¡ÄáŸi Åe”a. */
#define SBL_SPEAKER_OFF 0xD3  /* ¬a§i ¯aÏ¡ÄáŸi e”a. */
#define SBL_RESET       0x6   /* ¬a§i Ÿ¡­[ */
#define SBL_RESET_CMD   0x1
#define SBL_DATA_AVAIL  0x0e  /* ¬a§i •A·¡Èa ¦…¬âº— */
#define SBL_READ_DATA   0xa   /* ¬a§i •A·¡Èa ·ª‹¡ */
#define SBL_READ_STATUS 0xc   /* ¬a§i ¬wÈ ·ª‹¡ */
#define SBL_WRITE_DATA  0xc   /* ¬a§i •A·¡Èa ³a‹¡ */

#define SBLPRO_MIXER_A  0x4   /* ¬a§i Ïa¡ £¢¬á ´á—aA¯a */
#define SBLPRO_MIXER_D  0x5   /* ¬a§i Ïa¡ £¢¬á •A·¡Èa ´á—aA¯a */

#define SBLPRO_VOICE_VOLUME   0x04 /* ¬a§i Ïa¡ ¥¡·¡¯a ¥©ŸQ */
#define SBLPRO_MASTER_VOLUME  0x22
#define SBLPRO_LINE_VOLUME    0x2e
#define SBLPRO_FM_VOLUME      0x26
#define SBLPRO_CD_VOLUME      0x28

#define setSBLProMixer(A,R,L) \
  { outportb(SBLBaseIO+SBLPRO_MIXER_A,(A)) ; \
    outportb(SBLBaseIO+SBLPRO_MIXER_D,((R)<<4)|(L)) ; }

#define sayVoice(F)    sayPCM((F),0)

/* ·q¬÷ Â‰b·¡ {i˜ Œa»¡ ‹¡”aŸ¥”a. */
#define waitVoiceEnd()    while(VoiceFlag==VOICE_BUSY)

#define setCovoxBaseIO(D) (CovoxIO = (D))  /* Å¡¥¢¯a ¤å»¡ ¬é¸÷ */

/* Å¡¥¢¯a ¤å»¡Ÿi ÏaŸ¥Èá Í¡Ëa ¤åÑ¡¡ ¬é¸÷ */
#define setCovoxLPT(LPT)    (CovoxIO = *((unsigned int *)(0x406+(LPT)+(LPT))))


/* VOICE.C procedure */
int SBLCommand(unsigned char cmd) ;
int SBLSendCommand(unsigned char cmd) ;
int SBLReset(int BaseIO) ;
void SBLSpeaker(int flag) ;
void SBLOnePulseDA(unsigned char PCMData) ;
void endVoice(void) ;
int initVoice(int SndDev) ;
void setPCMVolume(unsigned int vl,unsigned int vr) ;
void setSampleRate(unsigned int sampleRate) ;
int silentVoice(int flag) ;
int sayPCM(unsigned char *PCMData,unsigned int l) ;
void sayDMAVoice(long physical,unsigned int l) ;
unsigned int getVoiceRemainSample(void) ;
void setSoundBlasterENV(int BaseIO,int IRQ,int DMAChannel) ;
int SBLAutoDetect(void) ;
void waitmSec(unsigned int mSec) ;

#endif


// **************************************************************************
// ********************************* xms.c **********************************
// **************************************************************************
typedef unsigned char byte ;
typedef unsigned int  word ;

typedef struct {
   unsigned long length  ;
   unsigned int  sourH   ;
   unsigned long sourOff ;
   unsigned int  destH   ;
   unsigned long destOff ;
} XMOVE ;

extern XMOVE xmove ;
extern long ptr2long() ;

extern int  init_xms( void ) ;
extern int  move_xms() ;
extern int  xmsfree() ;
extern int  xmsmalloc() ;

extern void xmstomem( int Handle, long _off_set, byte *dest, word length ) ;
extern void memtoxms( byte *sour, int Handle, long _off_set, word length ) ;


// **************************************************************************
// ****************************** multikey.c ********************************
// **************************************************************************
/******* Multi Key Define *******/

#define M_ESC       1
#define M_1         2
#define M_2         3
#define M_3         4
#define M_4         5
#define M_5         6
#define M_6         7
#define M_7         8
#define M_8         9
#define M_9         10
#define M_0         11
#define M_-         12
#define M_BACKSPACE 14
#define M_TAB       15
#define M_Q         16
#define M_W         17
#define M_E         18
#define M_R         19
#define M_T         20
#define M_Y         21
#define M_U         22
#define M_I         23
#define M_O         24
#define M_P         25
#define M_ENTER     28
#define M_CTRL      29
#define M_A         30
#define M_S         31
#define M_D         32
#define M_F         33
#define M_G         34
#define M_H         35
#define M_J         36
#define M_K         37
#define M_L         38
#define M_LSHIFT    42
#define M_Z         44
#define M_X         45
#define M_C         46
#define M_V         47
#define M_B         48
#define M_N         49
#define M_M         50
#define M_RSHIFT    54
#define M_ALT       56
#define M_SPACEBAR  57
#define M_CAPSLOCK  58
#define M_F1        59
#define M_F2        60
#define M_F3        61
#define M_F4        62
#define M_F5        63
#define M_F6        64
#define M_F7        65
#define M_F8        66
#define M_F9        67
#define M_F10       68
#define M_NUMLOCK   69
#define M_SCRLOCK   70
#define M_HOME      71
#define M_UP        72
#define M_PAGEUP    73
#define M_LEFT      75
#define M_RIGHT     77
#define M_END       79
#define M_DOWN      80
#define M_PAGEDOWN  81
#define M_INSERT    82
#define M_DELETE    83

extern unsigned char keycode[128] ;

extern void SetInt09( void ) ;
extern void EndInt09( void ) ;
