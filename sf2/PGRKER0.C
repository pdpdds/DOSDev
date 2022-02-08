/*********

     VGA 320 * 200 , 256 Äéœá Íw¡e¡¡—a ‹aœÏ¢ œa·¡§aœáŸ¡

     ­¡¯aÅ¡—a : Borland C++ 2.0
     ¹A¸b·©¸a : 1992.9.17
     ¹A¸b¸a   : ¸÷ µw ”â

     Íw¡e¡¡—a : 4 ÍA·¡»¡ ¬a¶wˆa“w ( 256K Îaº… §aˆaµA¬á )
     packed mode : 1 ÍA·¡»¡ ¬a¶wˆa“w ( ¥¡É·· ¤a·¡µ¡¯aÅ© )
*********/

//#include <alloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

#pragma inline

#if 0
void setStAdd() ;        // §¡—¡µ¡œ‘ ¯aÅE ¯¡¸b ´á—aA¯a ­AË·
void actPage() ;         // ¸b´ó ÍA·¡»¡ Ði”w
void planeMode() ;       // ¡yˆ· A»¡¯aÈáŸi ¹¡¸b, ÏaE¡¡—a¡ ¤aŽ…”a
void writeMode() ;       // §aˆa· ³a‹¡ ¡¡—a ¥e‰w
void pBox2() ;           // Íw¡e¡¡—aµA¬á ¸é”¹ÁÎa¡ ¬w¸a‹aŸ¡‹¡
void pBoxL() ;           // Íw¡e¡¡—aµA¬á ¸é”¹ÁÎaµÁ ‹©·¡¡ ¬w¸a‹aŸ¡‹¡
void pPutImgMode() ;     // Íw¡e¡¡—aµA¬á ¯aÏaœa·¡ËaŸi µaœá ÑwÈ¡ ‹aŸ¡‹¡
void pCTrpRImg0() ;
void pCTrpRImg1() ;
void pCTrpRImg2() ;
void pCTrpRImg3() ;
void pImgCpLU() ;
void imgHFill() ;
void pSprCp() ;
void pSprCpUD() ;
void pSprCpMir() ;
void pSprCpMirUD() ;
void pImgVFill() ;
void pBytePut() ;
void pImgHFill() ;
void pImgFill() ;
void pBitImgCp() ;
void fPBitImgCp() ;
void get1DACR() ;
void getNDACR() ;
void set1DACR() ;
void setNDACR() ;
void brightCtrl() ;
void greyCtrl() ;
void setRGBPalette() ;
void asmDecode() ;
#endif

#define pLineHL(X,Y,L,C)     pImgHFill(_PSEG[(Y)],(X),(X)+(L)-1,(C))
#define pLineH2(X1,Y,X2,C)   pImgHFill(_PSEG[(Y)],(X1),(X2),(C))
#define pLineVL(X,Y,L,C)     pImgVFill(_PSEG[(Y)],(X),80,(L),(C))
#define pLineV2(X,Y1,Y2,C)   pImgVFill(_PSEG[(Y1)],(X),80,(Y2)-(Y1)+1,(C))

unsigned char far *_ADD[200]  ; // Packed ModeµA¬á y ¹ÁÎaµA Ð”wÐa“e §¡—¡µ¡œ‘ ´á—aA¯a
unsigned int  far _SEG[200]   ; // Packed ModeµA¬á y ¹ÁÎaµA Ð”wÐa“e §¡—¡µ¡œ‘ ­A‹a åËa
unsigned int  far _PSE[4][200]; // Íw¡e¡¡—aµA¬á y ¹ÁÎaµA Ð”wÐa“e §¡—¡µ¡œ‘ ­A‹a åËa
				// [0][y] : ÍA·¡»¡ 0 , [1][y] : ÍA·¡»¡ 1 , ...
unsigned int  far *_PSEG ;      // Ñe¸ ¸b´óÍA·¡»¡· ´á—aA¯a Í¡·¥Èá
				// _PSEG[y] ˆt·¡ Ñe¸ ¸b´ó ÍA·¡»¡µA¬á y ¹ÁÎaµA
				// Ð”wÐa“e §¡—¡µ¡œ‘ ­A‹a åËa·±

unsigned char *_EBF ; // ˆe·¡ ´sÂ‚–E ˆõ·i Î‰ ·±¯¡ ¤áÌá

int _XL,_YL,_XR,_YR ;           // ¥¡·¡“e ÀwËi· Š¬â ¹ÁÎaˆt

// (_XL,_YL) *------------
//           !           !
//           !           !
//           !-----------* (_XR,_YR)

/* PGRKER0.C */
//¥¡·¡“e Àw Ëi ·Š¬â¹ÁÎa ˆt //( int _XL , int _YL );
int vgaTextMode ( void );
void setStAdd ( unsigned int st );
void actPage ( int p );
void planeMode ( void );
void writeMode ( unsigned char mode );
void pBox2 ( int x1 , int y1 , int x2 , int y2 , int c );
void pBoxL ( int x , int y , int xl , int yl , int c );
void pPutImgMode ( int x , int y , unsigned char far *p , int xs , int ys , int m );
void pCTrpRImg0 ( int x , int y , unsigned char far *p , int xs , int ys );
void pCTrpRImg1 ( int x , int y , unsigned char far *p , int xs , int ys );
void pCTrpRImg2 ( int x , int y , unsigned char far *p , int xs , int ys );
void pCTrpRImg3 ( int x , int y , unsigned char far *p , int xs , int ys );
void pImgCpLU ( unsigned int sseg , unsigned int soff , unsigned int scs , unsigned int xs , unsigned int ys , unsigned int dseg , unsigned int doff , unsigned int dcs );
void pSprCp ( unsigned int sseg , unsigned int soff , unsigned int scs , unsigned int xs , unsigned int ys , unsigned int dseg , unsigned int doff , unsigned int dcs );
void pSprCpUD ( unsigned int sseg , unsigned int soff , unsigned int scs , unsigned int xs , unsigned int ys , unsigned int dseg , unsigned int doff , unsigned int dcs );
void pSprCpMir ( unsigned int sseg , unsigned int soff , unsigned int scs , unsigned int xs , unsigned int ys , unsigned int dseg , unsigned int doff , unsigned int dcs );
void pSprCpMirUD ( unsigned int sseg , unsigned int soff , unsigned int scs , unsigned int xs , unsigned int ys , unsigned int dseg , unsigned int doff , unsigned int dcs );
void imgHFill ( unsigned int seg , unsigned int off , unsigned int xs , unsigned int color );
void pImgVFill ( unsigned int seg , unsigned int off , unsigned int scs , unsigned int ys , unsigned int color );
void pBytePut ( unsigned int seg , unsigned int off , unsigned int color );
void pImgHFill ( unsigned int seg , unsigned int x1 , unsigned int x2 , unsigned int color );
void pImgFill ( unsigned int seg , unsigned int x1 , int scs , unsigned int x2 , int ys , unsigned int color );
void pBitImgCp ( unsigned int sseg , unsigned int soff , unsigned int scs , unsigned int ys , unsigned int dseg , unsigned int doff , unsigned int dcs , unsigned int color );
void fPBitImgCp ( unsigned int sseg , unsigned int soff , unsigned int scs , unsigned int ys , unsigned int dseg , unsigned int doff , unsigned int dcs , unsigned int color );
void get1DACR ( unsigned int i , unsigned char *r , unsigned char *g , unsigned char *b );
void getNDACR ( unsigned int s , int n , unsigned char *r , unsigned char *g , unsigned char *b );
void set1DACR ( unsigned int i , unsigned int r , unsigned int g , unsigned int b );
void setNDACR ( unsigned int s , int n , unsigned char *r , unsigned char *g , unsigned char *b );
void brightCtrl ( unsigned int s , int n , unsigned char *r , unsigned char *g , unsigned char *b , int factor );
void greyCtrl ( unsigned int s , int n , unsigned char *r , unsigned char *g , unsigned char *b , int factor );
void setRGBPalette ( void );
void asmDecode ( unsigned char far *s , unsigned char far *d , unsigned int l );
int vga320Mode(void);



/*
   vga320Mode()“e VGAŸi 320*200 256 Äéœá¡¡—a¡ ­AË·Ðe”a.
     µaœáˆa»¡ ¥e®—i•¡ Á¡‹¡ÑÁ
   º· ! §aˆa ÁAÇaŸi Ða»¡´g“e”a.
*/
int vga320Mode(void)
{
   unsigned int i ;
   // ¤a·¡µ¡¯a Å©¡ ­AË·Ðe”a.
   asm {
     mov ax,013h
     int 10h
   }

   // y ¹ÁÎaµA Ð”wÐa“e §¡—¡µ¡œ‘ ´á—aA¯a Í¡·¥Èáˆt·i ­AË·Ðe”a.
   // §aˆa· ¯¡¸b ´á—aA¯a“e A000:0000 ·¡”a.

   for ( i=0 ; i<200 ; i++ )
   {
     _ADD[i]=(unsigned char far *)(0xa0000000 + i*320) ;
     _SEG[i]=0xa000+i*20 ;
     _PSE[0][i]=0xa000 + i*5 ;
     _PSE[1][i]=0xa000 + i*5 + 0x400 ;
     _PSE[2][i]=0xa000 + i*5 + 0x800 ;
     _PSE[3][i]=0xa000 + i*5 + 0xc00 ;
   }

   _XL=_YL=0 ; _XR=319 ; _YR=199 ; // ÀwËiˆt·i ¬é¸÷Ðe”a.
   _PSEG = _PSE[0] ; // ¸b´óÍA·¡»¡Ÿi 0 ÍA·¡»¡¡ ¬é¸÷Ðe”a.
   _EBF = malloc(13000) ; // ´sÂ‚Ð¹A ¤áÌáŸi Ði”wÐe”a.
   return(0) ;
}

/*
   vgaTextMode()“e §aˆaŸi 80*25 Äéœá ÉB¯aËa ¡¡—a¡ ­AË·Ðe”a.
   º· ! §aˆaÁAÇaŸi Ða»¡´g“e”a.
*/
int vgaTextMode(void)
{
   asm {
     mov ax,03h
     int 10h
   }
   return(0) ;
}

/*
   setStAdd()“e §aˆa §a—¡µ¡œ‘ ¯aÅE ´á—aA¯aŸi ¬é¸÷Ðe”a.

   Íw¡e ¡¡—aµA¬á“e 4 ÍA·¡»¡ˆa ˆa“wÐa¡a
      ÍA·¡»¡ 0 ·e setStadd(0x0000)
      ÍA·¡»¡ 1 ·e setStadd(0x4000)
      ÍA·¡»¡ 2 ·e setStadd(0x8000)
      ÍA·¡»¡ 3 ·e setStadd(0xc000)

   ·¡ žË¥·e Ða—a¶Á´á ¬wÐa ¹Á¶ ¯aÇa©·¡a ”aº— ÍA·¡»¡ ´“¡¡A·¡­eµA ³a·¥”a.

   º· ! ¹Á¶ ¯aÇa©·e ÂA­¡ ”e¶áˆa 4 Ï¢­I·¡¡a,  e´¢ 1 Ï¢­I³¢ Ða‰¡ ¯¼·a¡e
          §aˆa· ”aŸe ¦A»¡¯aÈáŸi ¤aŽ´á´¡Ðe”a.        
*/
void setStAdd(unsigned int stv)
{
   asm mov dx,03dah
lp:
   asm {
     in  al,dx
     test al,08h
     jz lp

     mov bx,stv

     mov dx,03d4h
     mov al,0dh
     mov ah,bl
     out dx,ax

     mov dx,03d4h
     mov al,0ch
     mov ah,bh
     out dx,ax
   }
}

/*
   actPage() “e Ñe¸ ¸b´ó ÍA·¡»¡Ÿi ­AË·Ðe”a.
   º· ! ¬é¸÷Ði ÍA·¡»¡ˆa Èa”wÐe»¡ ˆñ¬aŸi Ða»¡´g“e”a.
	   Îaº… §aˆa“e 0-3 Œa»¡ 4 ÍA·¡»¡ˆa ˆa“wÐa”a.
*/
void actPage(int p)
{
   _PSEG = _PSE[p] ;
}

/*
   planeMode() “e 1 ÍA·¡»¡ §aˆa packed¡¡—aŸi 4 ÍA·¡»¡ Íw¡e¡¡—a¡ ¤aŽ…”a.
   ”aº—ÍA·¡»¡ ‹¡¤ó·i ¬a¶wÐaa¡e Ï©®¸â·a¡ Íw¡e ¡¡—a¡ ¬é¸÷Ð´¡Ðe”a.
*/
void planeMode(void)
{
   outport(0x3c4,0x0604) ;
   outport(0x3d4,0x0014) ;
   outport(0x3d4,0xe317) ;
}

/*
   writeMode(mode) “e §aˆa ³a‹¡ ¡¡—aŸi ¬é¸÷Ðe”a.

   ·³b ¥e® 0 : ¥¡É·· ³a‹¡¡¡—a¡¬á host memoryµA¬á §¡—¡µ¡œ‘µA ¸å­· 
             1 : ¦ §¡—¡µ¡œ‘¬a·¡· ³a‹¡¡¡—a¡¬á 4¤ ¨aŸa”a. 
                 ·¡³a‹¡¡¡—aŸi ·¡¶w, ¦ §¡—¡µ¡œ‘µA ¤‰w·¡a, ¯aÏaœa·¡ËaŸi
                 ¸á¸wÐaµa, ‰¡­¢ ¯aÏaœa·¡Ëa ÀáŸ¡Ÿi Ðe”a.
             2,3 ·e §aˆa ¡A“A´é·i ¥¡¯¡µ¡
*/
void writeMode(unsigned char mode)
{
   unsigned char mask ;

   outportb(0x3ce,0x5) ;
   mask=inportb(0x3cf) ;
   outportb(0x3cf,(mask&0xfc)|mode) ;
}

/**
   pBox2(para,..) “e Íw¡e¡¡—aµA¬á ¬w¸aŸi ‹aŸ¥”a.

   input parameter
       x1,y1 ·e ¹ÁÃb¬w”e· ¹ÁÎa·¡”a.
       x2,y2 ·e ¶Ãb¬w”e· ¹ÁÎa·¡”a.
       c “e ‹aŸ© ¬‚·¡”a.
**/
void pBox2(int x1, int y1, int x2, int y2, int c)
{
   pLineH2(x1,y1,x2,c) ;
   pLineH2(x1,y2,x2,c) ;
   pLineV2(x1,y1,y2,c) ;
   pLineV2(x2,y1,y2,c) ;
}

/**
   pBoxL(para,..) “e Íw¡e¡¡—aµA¬á ¬w¸aŸi ‹aŸ¥”a.

   input parameter
       x,y   ¹ÁÃb ¬w”e· ¹ÁÎa·¡”a.
       xl,yl ¤b¯a· ®Íw,®»¢ ¤wÐ··a¡· ‹©·¡
       c ‹aŸ© ¬‚
**/
void pBoxL(int x, int y, int xl, int yl, int c)
{
   pLineHL(x,y,xl,c) ;
   pLineHL(x,yl+y-1,xl,c) ;
   pLineVL(x,y,yl,c) ;
   pLineVL(xl+x-1,y,yl,c) ;
}

/*
   ÏiE ¡¡—aµA¬á ‹aŸ± ¿¢“e ÑwÈ

   ------      0        ------
   !#$  !               !#$  !  ‹a”¡ ‹aŸ¥”a.
   !    !  -----------> !    !
   ------               ------
   ------      1        ------
   !#$  !               !  $#!  ˆá¶‰µA §¡Ã¥ˆõ Àáœñ ‹aŸ¥”a.
   !    !  -----------> !    !
   ------               ------
   ------      2        ------
   !#$  !               !    !  ¶á´aœŸi –á»³´á¬á ‹aŸ¥”a.
   !    !  -----------> !#$  !
   ------               ------
   ------      3        ------
   !#$  !               !    !  ˆá¶‰µA §¡Ã¡‰¡, –á»³´á¬á ‹aŸ¥”a.
   !    !  -----------> !  $#!
   ------               ------
*/

/**
  pPutImgMode(para,,) ¯aÏaœa·¡ËaŸi ‹aŸ± ¿¢“e ÑwÈµA ˜aœa ‹aŸ¥”a.
  ¯aÏaœa·¡Ëa“e 2 Àa¶¥ unsigned charÑwÈ· ¤µi¡¬á ‹a ˆt·¡ 0 ·¥‰w¶µA“e
  ‹aŸ¡»¡ ´g´a¬á ¤‰w·i ¥¡Ñ¡Ðe”a.
  ·³b ¥e®
      ¯aÏaœa·¡ËaŸi ‹aŸ© ¹ÁÎa (x,y)
      ¯aÏaœa·¡Ëaˆa ¸á¸w–A´á·¶“e ¤µi Í¡·¥Èá p
      ¯aÏaœa·¡Ëa· ˆa¡,­A¡ Ça‹¡ xs,ys
      ‹aŸ± ¿¢“e ÑwÈ 0 : ‹a· ‹aŸ¥”a.
		     1 : ˆá¶‰µA §¡Ã¥ ÑwÈ¡ ‹aŸ¥”a.
		     2 : –á»³´á ‹aŸ¥”a.
		     3 : ˆá¶‰µA §¡Ã¥ÑwÈŸi –á»³´á ‹aŸ¥”a.

  º· ! Íw¡e¡¡—aµA¬á ¬a¶wÐ´¡Ðe”a. ÇiŸ¡Ï· ÁAÇaÐe”a.
**/
void pPutImgMode(int x, int y, unsigned char far*p, int xs, int ys, int m)
{
   switch (m)
   {
     case 1 : pCTrpRImg1(x,y,p,xs,ys) ; break ;
     case 2 : pCTrpRImg2(x,y,p,xs,ys) ; break ;
     case 3 : pCTrpRImg3(x,y,p,xs,ys) ; break ;
     default: pCTrpRImg0(x,y,p,xs,ys) ; break ;
   }
}

/**
  pCTrpRImg0(para,,)
  transparent means Sprite Put

  Input parameter
      display point (x,y)
      image data pointer p
      image x,y size xs,ys

  Warnning !! this procedure use plane Mode
  Íw¡e¡¡—aµA¬á ¯aÏaœa·¡ËaŸi ‹aŸ¥”a.
**/
void pCTrpRImg0(int x, int y, unsigned char far*p, int xs, int ys)
{
   int x1,y1,xs1,ys1 ;

   x1 = max(x,_XL) ; xs1 = min(x+xs-1,_XR) - x1 + 1 ;
   y1 = max(y,_YL) ; ys1 = min(y+ys-1,_YR) - y1 + 1 ;

   if (xs1>0 && ys1>0)
   pSprCp(FP_SEG(p),FP_OFF(p)+xs*(y1-y)+x1-x,xs,xs1,ys1,_PSEG[y1],x1,80) ;
}

/**
  pCTrpRImg1(para,,) Clipping Transparent Rectangular Image Mirror Put Procedure
  transparent means Sprite Put

  Input parameter
      display point (x,y)
      image data pointer p
      image x,y size xs,ys

  Warnning !! this procedure use plane Mode
  Íw¡e¡¡—aµA¬á ¯aÏaœa·¡ËaŸi ˆá¶‰µA §¡Ã¥ ÑwÈ¡ ‹aŸ¥”a.
**/
void pCTrpRImg1(int x, int y, unsigned char far*p, int xs, int ys)
{
   int x1,y1,x2,ys1 ;

   x1 = max(x,_XL) ; x2 = min(x+xs-1,_XR) ;
   y1 = max(y,_YL) ; ys1 = min(y+ys-1,_YR) - y1 + 1 ;

   if (x2>=x1 && ys1>0)
   pSprCpMir(FP_SEG(p),FP_OFF(p)+xs*(y1-y)+xs+x-1-x2,xs,x2-x1+1,ys1,_PSEG[y1],x1,80) ;
}

/**
  pCTrpRImg2(para,,) Clipping Transparent Rectangular Image UD Put Procedure
  transparent means Sprite Put

  Input parameter
      display point (x,y)
      image data pointer p
      image x,y size xs,ys

  Warnning !! this procedure use plane Mode
  Íw¡e¡¡—aµA¬á ¯aÏaœa·¡ËaŸi –á»³´á ‹aŸ¥”a.
**/
void pCTrpRImg2(int x, int y, unsigned char far*p, int xs, int ys)
{
   int x1,y1,xs1,y2 ;

   x1 = max(x,_XL) ; xs1 = min(x+xs-1,_XR) - x1 + 1 ;
   y1 = max(y,_YL) ; y2 = min(y+ys-1,_YR)  ;

   if (xs1>0 && y2>=y1)
   pSprCpUD(FP_SEG(p),FP_OFF(p)+xs*(ys+y-y2-1)+x1-x,xs,xs1,y2-y1+1,_PSEG[y1],x1,80) ;
}

/**
  pCTrpRImg3(para,,) Clipping Transparent Rectangular Image Mir UD Put Procedure
  transparent means Sprite Put

  Input parameter
      display point (x,y)
      image data pointer p
      image x,y size xs,ys

  Warnning !! this procedure use plane Mode
  Íw¡e¡¡—aµA¬á ¯aÏaœa·¡ËaŸi ˆá¶‰µA §¡Ã¥ÑwÈŸi –á»³´á ‹aŸ¥”a.
**/
void pCTrpRImg3(int x, int y, unsigned char far*p, int xs, int ys)
{
   int x1,y1,x2,y2 ;

   x1 = max(x,_XL) ; x2 = min(x+xs-1,_XR) ;
   y1 = max(y,_YL) ; y2 = min(y+ys-1,_YR) ;

   if (x2>=x1 && y2>=y1)
   pSprCpMirUD(FP_SEG(p),FP_OFF(p)+xs*(ys+y-1-y2)+xs+x-1-x2,xs,x2-x1+1,y2-y1+1,_PSEG[y1],x1,80) ;
}


/**
   Image to Image Copy procedure
   image is 2D structure ( column size , row size )
   computer image is virtually mapping of 1D memory
   simply C language 2D array

   source image             destination image
  +------------+              +------------+
  !            !              ! seg,off    !
  ! seg,off    !   partial    !  +###      !
  !   +###  ===================> ####      !
  !   #### ys  !     copy     !  ####      !
  !   ####     !              !            !
  !    xs      !              !            !
  +------------+              +------------+
       scs                          dcs

  sseg,soff : source image start address ( segment , offset )
  scs       : source image column size
  xs,ys     : source image x,y size for copy

  dseg,doff : destination image start address ( segment , offset )
  dcs       : destination image column size

  ‹aŸ±ˆe· ¦¦…¸â •A·¡Èa ¸å­· ‹¡¥¥ ÑwÈ·¡”a.
  µa‹¡¬á ‹aŸ±·¡œe 2Àa¶¥ ¤µi·i  iÐa¡a, §aˆa· §¡—¡µ¡œ‘,¯¡¯aÉQ ¡A¡¡Ÿ¡—w
  ˆa¬w¸â·a¡ ¬é¸÷–E 1 Àa¶¥ ¡A¡¡Ÿ¡Ÿi  iÐe”a.

  µŸi —i¡e §aˆa· 320*200 ¡¡—a“e ˆa¡Ça‹¡ˆa 320, ­A¡Ça‹¡ˆa 200·¥ 2 Àa¶¥
  ¤µi •A·¡ÈaµÁ •··©Ða‰A Âá‹sÐi ® ·¶”a.

  ¸å­· ­¡¯aˆa ¯¡¯aÉQ ¡A¡¡Ÿ¡¡e ¯aÏaœa·¡ËaŸi ¸å­·Ða“e ˆõ‰Á ˆ{‰¡, ¸å­· ­¡¯aˆa
  §aˆa §¡—¡µ¡œ‘·¡¡e ‹aŸ±·i ´è´á µ¡“eˆõ‰Á ˆ{”a.

   ** Íw¡e ¡¡—aµA¬á Ò¸a“e ¬÷Ÿ³Ða»¡ ´g“e”a.
**/

/**
  pImgCpLU()   source      image : host memory or VGA video RAM
               destination image : host memory or VGA video RAM

  Use Write Mode 0,1,2,3
  specially Write Mode 1 -> high speed scrolling or panning

  packed ¡¡—aµA¬á ‹aŸ±ˆe· ¸å­··i Ðe”a.
  Íw¡e¡¡—aµA¬á §aˆa ³a‹¡¡¡—a 1¡ ¬é¸÷ÐeÒ ¤‰wÑÁ¡e ‰¡­¢ ¸å­··a¡ ¬a¶w–E”a.
**/
void pImgCpLU(sseg,soff,scs,xs,ys,dseg,doff,dcs)
unsigned int sseg,soff,scs,xs,ys,dseg,doff,dcs ;
{
   asm {
     push ds
     push si

     mov ax,sseg
     mov ds,ax
     mov ax,dseg
     mov es,ax

     cld

     mov si,soff
     mov di,doff

     mov bx,scs
     xor ax,ax
     mov dx,xs
   }

lp0:              // for ( y=0 to ys )
   asm {
     mov cx,dx
     rep movsb

     inc ax
     sub si,dx
     sub di,dx
     add si,bx
     add di,dcs
     cmp ax,ys
     jne lp0

     pop si
     pop ds
   }
}

/**
   pSprCp is Plane Mode Sprite Normal Put Procedure
      source      image : host Memory Only
      destination image : VGA video RAM Only
   Íw¡e¡¡—aµA¬á ¯aÏaœa·¡ËaŸi ‹aŸ¥”a.
**/
void pSprCp(sseg,soff,scs,xs,ys,dseg,doff,dcs)
unsigned int sseg,soff,scs,xs,ys,dseg,doff,dcs ;
{
   unsigned int sis,dis ;

   sis = ys*scs ; dis = ys*dcs ;

   asm {
     push ds
     push si
     mov ax,sseg
     mov ds,ax
     mov ax,dseg
     mov es,ax

     mov si,soff

     mov dx,3c4h
     mov al,2
     out dx,al
     mov al,11h
     mov cx,doff
     mov di,cx
     shr di,1
     shr di,1
     and cx,3
     rol al,cl
     mov dx,3c5h
     out dx,al

     mov bx,xs
   }
lp1:
   asm mov cx,ys
lp0:
   asm {
     mov ah,ds:[si]
     cmp ah,0
     je skip
     mov es:[di],ah
   }
skip:
   asm {
     add si,scs
     add di,dcs
     loop lp0
     rol al,1
     out dx,al
     adc di,0
     sub si,sis
     sub di,dis
     inc si
     dec bx
     jg lp1

     pop si
     pop ds
   }

   asm mov dx,03c4h
   asm mov ax,0f02h
   asm out dx,ax

}

/**
   pSprCpUD is Plane Mode Sprite Upside down Put Procedure
      source      image : host Memory Only
      destination image : VGA video RAM Only
   Íw¡e¡¡—aµA¬á ¯aÏaœa·¡ËaŸi –á»³´á ‹aŸ¥”a.
**/
void pSprCpUD(sseg,soff,scs,xs,ys,dseg,doff,dcs)
unsigned int sseg,soff,scs,xs,ys,dseg,doff,dcs ;
{
   unsigned int sis,dis ;

   sis = ys*scs ; dis = ys*dcs ;
   soff += (scs*ys-scs) ;

   asm {
     push ds
     push si
     mov ax,sseg
     mov ds,ax
     mov ax,dseg
     mov es,ax

     mov si,soff

     mov dx,3c4h
     mov al,2
     out dx,al
     mov al,11h
     mov cx,doff
     mov di,cx
     shr di,1
     shr di,1
     and cx,3
     rol al,cl
     mov dx,3c5h
     out dx,al

     mov bx,xs
   }
lp1:
   asm mov cx,ys
lp0:
   asm {
     mov ah,ds:[si]
     cmp ah,0
     je skip
     mov es:[di],ah
   }
skip:
   asm {
     sub si,scs
     add di,dcs
     loop lp0
     rol al,1
     out dx,al
     adc di,0
     add si,sis
     sub di,dis
     inc si
     dec bx
     jg lp1

     pop si
     pop ds
   }

   asm mov dx,03c4h
   asm mov ax,0f02h
   asm out dx,ax
}

/**
   pSprCpMir is Plane Mode Sprite Mirror Put Procedure
      source      image : host Memory Only
      destination image : VGA video RAM Only
   Íw¡e¡¡—aµA¬á ¯aÏaœa·¡ËaŸi ˆá¶‰µA §¡Ã¥ ÑwÈ¡ ‹aŸ¥”a.
**/
void pSprCpMir(sseg,soff,scs,xs,ys,dseg,doff,dcs)
unsigned int sseg,soff,scs,xs,ys,dseg,doff,dcs ;
{
   unsigned int sis,dis ;

   sis = ys*scs ; dis = ys*dcs ;
   soff += (xs-1) ;

   asm {
     push ds
     push si
     mov ax,sseg
     mov ds,ax
     mov ax,dseg
     mov es,ax

     mov si,soff

     mov dx,3c4h
     mov al,2
     out dx,al
     mov al,11h
     mov cx,doff
     mov di,cx
     shr di,1
     shr di,1
     and cx,3
     rol al,cl
     mov dx,3c5h
     out dx,al

     mov bx,xs
   }
lp1:
   asm mov cx,ys
lp0:
   asm {
     mov ah,ds:[si]
     cmp ah,0
     je skip
     mov es:[di],ah
   }
skip:
   asm {
     add si,scs
     add di,dcs
     loop lp0
     rol al,1
     out dx,al
     adc di,0
     sub si,sis
     sub di,dis
     dec si
     dec bx
     jg lp1

     pop si
     pop ds
   }
   asm mov dx,03c4h
   asm mov ax,0f02h
   asm out dx,ax
}

/**
   pSprCpMirUD is Plane Mode Sprite Mirror Upside down Put Procedure
      source      image : host Memory Only
      destination image : VGA video RAM Only
   Íw¡e¡¡—aµA¬á ¯aÏaœa·¡ËaŸi ˆá¶‰µA §¡Ã¥ ÑwÈ¡ –á»³´á ‹aŸ¥”a.
**/
void pSprCpMirUD(sseg,soff,scs,xs,ys,dseg,doff,dcs)
unsigned int sseg,soff,scs,xs,ys,dseg,doff,dcs ;
{
   unsigned int sis,dis ;

   sis = ys*scs ; dis = ys*dcs ;
   soff += (xs+scs*ys-scs-1) ;

   asm {
     push ds
     push si
     mov ax,sseg
     mov ds,ax
     mov ax,dseg
     mov es,ax

     mov si,soff

     mov dx,3c4h
     mov al,2
     out dx,al
     mov al,11h
     mov cx,doff
     mov di,cx
     shr di,1
     shr di,1
     and cx,3
     rol al,cl
     mov dx,3c5h
     out dx,al

     mov bx,xs
   }
lp1:
   asm mov cx,ys
lp0:
   asm {
     mov ah,ds:[si]
     cmp ah,0
     je skip
     mov es:[di],ah
   }
skip:
   asm {
     sub si,scs
     add di,dcs
     loop lp0
     rol al,1
     out dx,al
     adc di,0
     add si,sis
     sub di,dis
     dec si
     dec bx
     jg lp1

     pop si
     pop ds
   }
   asm mov dx,03c4h
   asm mov ax,0f02h
   asm out dx,ax
}


/**
   imgHFill(para,..) is image Horizontal Fill with color

   !--------------!
   ! seg,off      !
   !   +#######   !
   !      xs      !
   !              !
   !              !
   !              !
   !--------------!

   input parameter
       seg,off : segment,offset of image
       xs      : X size
       color   : color

   this procedure use Draw horizontal Line for packed mode

   Warnning !! this procedure used packed mode
   specially high speed Clear Screen Procedure for Plane Mode

   packed¡¡—aµA¬á ®Íw¬å·i ‹aŸ¥”a.
   Íw¡e¡¡—aµA¬á“e ‰¡­¢ ÑÁ¡e ¬‚Ã©žË¥·a¡ ¬a¶w–I ® ·¶”a.
**/
void imgHFill(segv,off,xs,color)
unsigned int segv,off,xs ;
unsigned char color ;
{
   asm {
     push ds
     push si

     mov ax,segv
     mov es,ax
     mov di,off

     cld

     mov al,color
     mov ah,al

     mov cx,xs

     shr cx,1
     jnb lp1
     stosb
   }
lp1:
   asm {
     rep stosw
     pop si
     pop ds
   }
}

/**
   plane Mode Image Veritcal Fill Procedure

   input parameter
       seg,off : segment,offset of image
       scs     : image column size
       ys      : Y size
       color   : color

   this procedure use Draw vertical Line for plane Mode
   ÏiE ¡¡—aµA¬á ®»¢¬å·i ‹aŸ¥”a.
**/
void pImgVFill(segv,off,scs,ys,color)
unsigned int segv,off,scs,ys ;
unsigned char color ;
{
   asm {
     push ds
     push si

     mov dx,3c4h
     mov al,2
     out dx,al

     mov ax,segv
     mov es,ax

     mov cx,off
     mov di,cx
     shr di,1
     shr di,1
     and cx,3
     mov al,1
     shl al,cl
     mov dx,3c5h
     out dx,al
     mov dx,scs
     mov ah,color
     mov cx,ys
   }
lp0:
   asm {
     mov es:[di],ah
     add di,dx
     loop lp0

     pop si
     pop ds
   }
   asm mov dx,03c4h
   asm mov ax,0f02h
   asm out dx,ax
}

/**
   plane Mode put 1 pixel

   ÏiE ¡¡—aµA¬á Ðe ¸ñ·i ¿¢“e”a.
**/
void pBytePut(segv,off,color)
unsigned int segv,off ;
unsigned char color ;
{
   asm {
     push ds
     push si

     mov dx,3c4h
     mov al,2
     out dx,al

     mov ax,segv
     mov es,ax

     mov cx,off
     mov di,cx
     shr di,1
     shr di,1
     and cx,3
     mov al,1
     shl al,cl
     mov dx,3c5h
     out dx,al
     mov ah,color
     mov es:[di],ah

     pop si
     pop ds
   }
   asm mov dx,03c4h
   asm mov ax,0f02h
   asm out dx,ax
}

/**
   plane Mode Image Horizontal Fill procedure

   !--------------!
   ! seg          !
   !   +######+   !
   !  x1     x2   !
   !              !
   !              !
   !              !
   !--------------!

   input parameter
       seg     : segment of image
       x1,x2   : horizontal fill left,right point
       color   : color

   this procedure use Draw horizontal Line for plane mode
   Íw¡e¡¡—aµA¬á ®Íw¬å·i ‹aŸ¥”a.
**/
void pImgHFill(segv,x1,x2,color)
unsigned int segv,x1,x2,color ;
{
   unsigned int off2 ;

   asm {
     push ds
     push si

     mov ax,segv
     mov es,ax
     mov dx,3c4h
     mov al,2
     out dx,al

     mov cx,x1
     mov bx,cx
     and cx,3
     mov al,0fh
     shl al,cl

     mov cx,x2
     mov off2,cx
     and cx,3
     mov ah,0e1h
     rol ah,cl

     mov cl,2
     shr bx,cl
     shr word ptr off2,cl
     mov dx,03c5h

     cmp bx,off2
     je skip

     mov di,bx
     out dx,al
     mov al,ah
     mov ah,color
     mov es:[di],ah
     mov di,off2
     out dx,al
     mov es:[di],ah

     mov al,0fh
     out dx,al

     mov di,bx
     mov cx,off2
   }
lp0:
   asm {
     inc di
     cmp di,cx
     je end
     mov es:[di],ah
     jmp lp0
   }
skip:
   asm {
     and al,ah
     out dx,al
     mov di,bx
     mov ah,color
     mov es:[di],ah
  }
end:
   asm pop si
   asm pop ds
   asm mov dx,03c4h
   asm mov ax,0f02h
   asm out dx,ax
}


/**
   planeMode Image Fill procedure

   !--------------!
   ! seg,x1   x2  !
   !   ########   !
   !   ########ys !
   !   ########   !
   !              !
   !              !
   !--------------!
	 scs

   input parameter
       seg     : segment of image
       x1,x2   : left,right value of x point
       scs     : image column size
       ys      : fill y size
       color   : fill color

   this procedure use Draw bar for plane mode
   ÏiE ¡¡—aµA¬á ¬aˆbÑw ¦¦…·i ·©¸÷Ðe ¬‚·a¡ Ã©Ðe”a.
**/
void pImgFill(segv,x1,scs,x2,ys,color)
unsigned int segv,x1,x2,color ;
{
   unsigned int off2 ;

   asm {
     push ds
     push si

     mov ax,segv
     mov es,ax
     mov dx,3c4h
     mov al,2
     out dx,al

     mov cx,x1
     mov bx,cx
     and cx,3
     mov al,0fh
     shl al,cl

     mov cx,x2
     mov off2,cx
     and cx,3
     mov ah,0e1h
     rol ah,cl

     mov cl,2
     shr bx,cl
     shr word ptr off2,cl
     mov dx,03c5h

     cmp bx,off2
     je skip

     mov di,bx
     out dx,al
     mov al,ah
     mov ah,color
     mov cx,ys
   }
lp0:
   asm {
     mov es:[di],ah
     add di,scs
     loop lp0

     out dx,al
     mov di,off2
     mov cx,ys
  }
lp1:
   asm {
     mov es:[di],ah
     add di,scs
     loop lp1

     mov al,0fh
     out dx,al

     mov dx,off2
     inc bx
     sub dx,bx
     mov cx,ys
     mov di,bx
   }
lp3: asm mov bx,0
lp2: asm cmp bx,dx
   asm {
    je next0
    mov es:[di],ah
    inc di
    inc bx
    jmp lp2
  }
next0:
   asm {
     sub di,dx
     add di,scs
     loop lp3
     jmp end
   }
skip:
   asm {
     and al,ah
     out dx,al
     mov di,bx
     mov cx,ys
     mov ah,color
   }
lp5:
   asm {
     mov es:[di],ah
     add di,scs
     loop lp5
   }
end:
   asm pop si
   asm pop ds
   asm mov dx,03c4h
   asm mov ax,0f02h
   asm out dx,ax
}

/**
   plane Mode Bitmapped image copy

   input parameter see previous information
   note !! scs is source column size -> byte order not bit order
   example ) 16 x 16 bitmap font : source column size is 2 , not 16

   ÏiE ¡¡—aµA¬á §¡Ëa “·i ‹aŸ¥”a.
**/
void pBitImgCp(sseg,soff,scs,ys,dseg,doff,dcs,color)
unsigned int sseg,soff,scs,ys,dseg,doff,dcs ;
unsigned char color ;
{
   unsigned int sis,dis ;

   sis = ys*scs ; dis = ys*dcs ;

   asm {
     push ds
     push si
     mov ax,sseg
     mov ds,ax
     mov ax,dseg
     mov es,ax

     mov si,soff

     mov dx,3c4h
     mov al,2
     out dx,al
     mov al,11h
     mov cx,doff
     mov di,cx
     shr di,1
     shr di,1
     and cx,3
     rol al,cl
     mov dx,3c5h
     out dx,al

     mov bl,color
     mov bh,scs
   }
lp2:
   asm mov cl,1
lp1:
   asm mov ch,ys
lp0:
   asm {
     mov ah,ds:[si]
     shl ah,cl
     jnb skip
     mov es:[di],bl
   }
skip:
   asm {
     add si,scs
     add di,dcs
     dec ch
     jg lp0
     rol al,1
     out dx,al
     adc di,0
     sub si,sis
     sub di,dis
     inc cl
     cmp cl,9
     jne lp1
     inc si
     dec bh
     jg lp2

     pop si
     pop ds
   }

   asm mov dx,03c4h
   asm mov ax,0f02h
   asm out dx,ax
}

/**
   fast plane Mode bitmapped image Copy

   input parameter see previous information
   note !! scs is source column size -> byte order, not bit order
   example ) 16 x 16 bitmap font : source column size is 2 , not 16

   warnnig !!
     because this procedure direct controlled write mode register with bitmap,
     bitmap data 4 bit nibble is swapped.

   ÏiE ¡¡—aµA¬á ‰¡­¢·a¡ §¡Ëa “·i ‹aŸ¥”a. 
   §¡Ëa “·a¡ Ïiœ… mask registerŸi ¹¡¸÷Ða£a¡ §¡Ëa “ •A·¡Èaˆa 4bit³¢ –á»³´á»¥
   ÑwÈµa´¡ Ðe”a. 
**/
void fPBitImgCp(sseg,soff,scs,ys,dseg,doff,dcs,color)
unsigned int sseg,soff,ys,dseg,doff,dcs ;
unsigned char color,scs ;
{
   asm {
     push ds
     push si

     mov ax,sseg
     mov ds,ax
     mov ax,dseg
     mov es,ax

     mov si,soff
     mov di,doff

     mov dx,3c4h
     mov al,2
     out dx,al
     mov dx,3c5h
     mov cl,04
     mov ch,color
    }

    asm mov bh,ys
lp2:
    asm mov bl,scs
lp1:
   asm {
     mov ah,ds:[si]
     mov al,ah
     and al,0f0h
     shr al,cl
     out dx,al
     mov es:[di],ch
     inc di
     mov al,ah
     and al,0fh
     out dx,al
     mov es:[di],ch
     inc di
     inc si
     dec bl
     jg lp1
     add di,dcs
     sub di,scs
     sub di,scs
     dec bh
     jg lp2

     pop si
     pop ds
   }

   asm mov dx,03c4h
   asm mov ax,0f02h
   asm out dx,ax
}

/*
  get one Digital Analog Conver Register value
  Digital Analog Convert Register means VGA 256 R,G,B palette

  input  i : index value of palette register
  output r,g,b : r,g,b value of palette registers

  ÌiA¯a A»¡¯aÈá ˆt—i·i ·ª´áµ¥”a.
**/
void get1DACR(i,r,g,b)
unsigned char i,*r,*g,*b ;
{
   outportb(0x3c7,i) ;
   *r = inportb(0x3c9)   ;
   *g = inportb(0x3c9)   ;
   *b = inportb(0x3c9)   ;
}

/*
  get N Digital Analog Conver Register value
  Digital Analog Convert Register means VGA 256 R,G,B palette

  input  s : start index value of palette register
         n : number of index
  output r[],g[],b[] : r,g,b array value of palette registers

  Nˆ· ÌiAËa A»¡Èáˆt—i·i ·ª´áµ¥”a.
**/
void getNDACR(s,n,r,g,b)
unsigned char s,*r,*g,*b ;
int n ;
{
   register int i ;

   outportb(0x3c7,s) ;

   for ( i=0 ; i<n ; i++ )
   {
     r[i] = inportb(0x3c9)   ;
     g[i] = inportb(0x3c9)   ;
     b[i] = inportb(0x3c9)   ;
   }
}

/*
  set one Digital Analog Conver Register value
  Digital Analog Convert Register means VGA 256 R,G,B palette

  input      i : index value of palette register
	 r,g,b : r,g,b value of palette registers
  Ðeˆ· ÌiAËaµA ˆt·i ¬é¸÷Ðe”a.
**/
void set1DACR(i,r,g,b)
unsigned char i,r,g,b ;
{
   outportb(0x3c8,i) ;
   outportb(0x3c9,r) ;
   outportb(0x3c9,g) ;
   outportb(0x3c9,b) ;
}

/*
  set N Digital Analog Conver Register value
  Digital Analog Convert Register means VGA 256 R,G,B palette

  input  s : start index value of palette register
         n : number of index
         r[],g[],b[] : r,g,b array value of palette registers
  Nˆ· ÌiAËa ˆt·i ¬é¸÷Ðe”a.
**/
void setNDACR(s,n,r,g,b)
unsigned char s,*r,*g,*b ;
int n ;
{
   register int i ;

   outportb(0x3c8,s) ;

   for ( i=0 ; i<n ; i++ )
   {
     outportb(0x3c9,r[i]) ;
     outportb(0x3c9,g[i]) ;
     outportb(0x3c9,b[i]) ;
   }
}

/*
  bright control of VGA graphic screen
  input s : start index of palette register
        n : number of index
        r[],g[],b[] : r,g,b value of palette
        factor : birghtness percent(%)
  ¸åÁA· ¤j‹¡Ÿi % ¡ ¹¡¸÷Ðe”a.
*/
void brightCtrl(s,n,r,g,b,factor)
unsigned char s,*r,*g,*b ;
int n,factor ;
{
   register int i ;
   unsigned char r1,g1,b1 ;

   outportb(0x3c8,s) ;

   for ( i=0 ; i<n ; i++ )
   {
     r1 = (r[i] * factor)/100 ; outportb(0x3c9,r1) ;
     g1 = (g[i] * factor)/100 ; outportb(0x3c9,g1) ;
     b1 = (b[i] * factor)/100 ; outportb(0x3c9,b1) ;
   }
}

/*
  grey control of VGA graphic screen
  input s : start index of palette register
        n : number of index
        r[],g[],b[] : r,g,b value of palette
        factor : grey level percent(%)
  ¸åÁA· ¡w´q·i %¡ ¹¡¸÷Ðe”a
*/
void greyCtrl(s,n,r,g,b,factor)
unsigned char s,*r,*g,*b ;
int n,factor ;
{
   register int i ;
   unsigned char grey ;

   outportb(0x3c8,s) ;

   for ( i=0 ; i<n ; i++ )
   {
     grey = ((r[i]*3 + g[i]*5 + b[i]*2)/10)*factor/100  ;
     outportb(0x3c9,grey) ;
     outportb(0x3c9,grey) ;
     outportb(0x3c9,grey) ;
   }
}

/*
  set RGB palette

  index is 8 bit

  +--+--+--+--+--+--+--+--+
  |R2|R1|R0|G2|G1|G0|B1|B0|
  +--+--+--+--+--+--+--+--+
     RED      GREEN  BLUE

  ¯aÌa2 ¸å¶w ÌiAËa ¬é¸÷
*/
void setRGBPalette(void)
{
   int i,r,g,b ;

   for ( r=0,i=0 ; r<8 ; r++ )
     for ( g=0 ; g<8 ; g++ )
      for ( b=0 ; b<4 ; b++,i++ )
      set1DACR(i,r*8+4,g*8+4,b*16+8) ;
}

/*
 fast decoding of Modified Run length encoding

 input  s : source encoding data buffer pointer
	l : source encoding data size
 output d : destination decoding data buffer pointer

 Modified Run Length Encoding Format

 d1 d2 d3 d4 d5 d6 .....  dn

 first  data d1 : length of 0
 second data d2,d3 : length of data ( integer )
 d4,d5,d6 .. dk : data value ( length is d2 + d3*256 )
      .
      .
      .

 example) source encoding data is
            4 6 0 1 2 3 7 9 3 4 2 0 2 3

          decoding data is
            0 0 0 0 1 2 3 7 9 3 0 0 0 0 2 3
	    !-----! !---------! !-----! !-!
               4         6         4     2

  ˆe·¡ Run length ´sÂ‚·i ‰¡­¢·a¡ Î…”a
*/

void asmDecode(s,d,l)
unsigned char far *s ;
unsigned char far *d ;
unsigned int l ;
{
   asm {

     push ds
     push si

     lds si,s
     les di,d
     mov bx,l
     mov dx,di

     cld
   }
lp0:
   asm {
     xor ch,ch
     mov cl,ds:[si]
     inc si
     dec bx
     jl  end
     xor ax,ax
     shr cx,1
     jnb lp1
     stosb
   }
lp1: asm rep stosw
   asm {
     mov cx,ds:[si]
     inc si
     inc si
     dec bx
     dec bx
     jl  end
     sub bx,cx
     shr cx,1
     jnb lp2
     movsb
   }
lp2: asm rep movsw
   asm jmp lp0
end:
   asm {
     mov cx,100
     xor ax,ax
     rep stosw
     mov ax,di
     sub ax,dx

     pop si
     pop ds
   }
}


