/****
   Plane Mode font kernel Program
   Borland C++ 2.0 compiler Only
   1992.9.17 By Jung y.d.

   ÏiE¡¡—a §¡Ëa “ Í¥Ëa Â‰b Ïa¡‹aœ‘
   1992.9.17 ¸÷µw”â
****/

#include <dos.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>

#include "grker0.h"
#include "animate0.h"

/* ·±¯¡ 16 by 16 dot §¡Ëa “ ¤áÌá */
unsigned int far _bm[16]     ; // temporary font bitmap buffer
/* Ðe‹i Á¡¬÷ ‹i¸a §¡Ëa “ •A·¡Èa Í¡·¥Èá */
unsigned int far *_FCBITMAP  ; // hangul first  character bitmap buffer pointer
/* Ðe‹i º—¬÷ ‹i¸a §¡Ëa “ •A·¡Èa Í¡·¥Èá */
unsigned int far *_MCBITMAP  ; // hangul middle character bitmap buffer pointer
/* Ðe‹i ¹·¬÷ ‹i¸a §¡Ëa “ •A·¡Èa Í¡·¥Èá */
unsigned int far *_LCBITMAP  ; // hangul last   character bitmap buffer pointer
/* µw¢… ‹i¸a §¡Ëa “ •A·¡Èa Í¡·¥Èá */
unsigned char far *_ECBITMAP ; // english bitmap buffer pointer

/* Ðe‹i Á¡¬÷ ‹i¸a ˆ•® */
#define NOFC 19  // Number Of First  Hangul Character
/* Ðe‹i º—¬÷ ‹i¸a ˆ•® */
#define NOMC 21  // Number Of Middle Hangul Character
/* Ðe‹i ¹·¬÷ ‹i¸a ˆ•® */
#define NOLC 27  // Number Of Last   Hanugl Character

/***
   Hangul code to Bitmap Convertion
   input  f,m,l  : code of first,middle,last hangul character
   output bitmap : 16 x 16 bitmap of hangul character

   return value  : offset of horizontal position

   Hangul code is special format.
               is not conventional format ( KS, ... )

   Ðe‹i Á¡,º—,¹·¬÷ Å¡—aŸi 16 by 16 dot §¡Ëa “ Í¥Ëa¡ ¥eÑÅ
    f : Á¡¬÷ Å¡—a 
    m : º—¬÷ Å¡—a
    l : ¹·¬÷ Å¡—a
    bitmap : Ðe‹i ‹i¸a §¡Ëa “
    Ðe‹i Í¡ •·e Ëb® Í¡ •·a¡¬á ‹¡¹¥· µÅ¬÷Ñw,¹¡ÐsÑw‰Á“e ”aŸa”a.
***/
HCode2Bitmap(f,m,l,bitmap)
unsigned char f,m,l ;
int bitmap[16] ;
{
   int f0,m0,l0,fo,mo,lo,i ;
   unsigned int fm,mm,lm ;
   static char fo2[22] = { 0,0,0,0,0,0,0,0,0,1,3,3,3,1,2,4,4,4,2,1,3,0 } ;
   static char lo2[22] = { 0,0,0,0,0,0,0,0,0,3,0,0,0,3,3,2,2,2,3,3,0,0 } ;

   if (f>19) f=0 ;
   if (m>21) m=0 ;
   if (l>27) l=0 ;

   fo = fo2[m] ; mo=1 ; lo = lo2[m] ;
   if (f==1||f==16) mo = 0 ;
   if (l>0) { fo += 5 ; mo += 2 ; }

   f0 = f+fo*NOFC-1 ; m0 = m+mo*NOMC-1 ; l0 = l+lo*NOLC-1 ;

   for ( i=0 ; i<16 ; i++ )
   bitmap[i]=0 ;

   fm = f ? 0xffff : 0 ;
   mm = m ? 0xffff : 0 ;
   lm = l ? 0xffff : 0 ;

   for ( i=0 ; i<16 ; i++ )
   bitmap[i]
     = (_FCBITMAP[f0*16+i]&fm) | (_MCBITMAP[m0*16+i]&mm) | (_LCBITMAP[l0*16+i]&lm) ;

   if (f==0&&m==0&&l==0) return(8) ;
   return(16) ;
}

/***
   Load Font File
   input fn : font file name
   return value  0 : normal
                -1 : font file not exist

   Í¥Ëa ÑÁ·©·i ·ª“e”a.
***/
loadFont(char *fn)
{
   FILE *fp ;

   fp=fopen(fn,"rb") ;
   if (fp==NULL) return(-1) ;
   _FCBITMAP=malloc(190*32) ; if (_FCBITMAP!=NULL) fread(_FCBITMAP,190,32,fp) ;
   _MCBITMAP=malloc( 84*32) ; if (_MCBITMAP!=NULL) fread(_MCBITMAP, 84,32,fp) ;
   _LCBITMAP=malloc(108*32) ; if (_LCBITMAP!=NULL) fread(_LCBITMAP,108,32,fp) ;
   _ECBITMAP=malloc(1504)   ; if (_ECBITMAP!=NULL) fread(_ECBITMAP,1,1504,fp) ;
   fclose(fp) ;
   return(0) ;
}

/**
  fast plane mode put text bitmap
  input x,y  : locate of x,y
        str  : print string
        mode : not used
        fc   : character foreground color
        bc   : not used

  str is special format of Street Fighter ][

  $str : english
  #str : hangul

  ‰¡­¢ §¡Ëa “ ÏaE¡¡—a Â‰b žË¥ ( §¡Ëa “ •A·¡Èa—i· 4§¡Ëaˆa ŒáŽ¡ –A´á´¡ Ðq )
  ˆa¡ ¹ÁÎa“e 4· ¤®·¡´á´¡ Ðe”a. ( ÏiE¡¡—a Ëb¬÷·i ‹a”¡ ·¡¶wÐa‹¡ ˜¢… )
**/
void fPText(x,y,str,mode,fc,bc)
unsigned int x,y ;
unsigned char str[],mode,fc,bc ;
{
   unsigned int i,p ;
   unsigned char c,f,m,l,t ;

   t=0 ;  x = x/4 ;

   for ( i=0 ; str[i]!='\0' ; i++ )
   {
     c=str[i] ;
     if (c=='$') { t=1 ; continue ; }
     if (c=='#') { t=0 ; continue ; }
     if (c==' '||c=='_') { x += 2 ; continue ; }
     if (t==0)
     {
       f=str[i++]-'@' ; m=str[i++]-'@' ; l=str[i]-'@' ;
       HCode2Bitmap(f,m,l,_bm) ;
       fPBitImgCp(FP_SEG(_bm),FP_OFF(_bm),2,16,_PSEG[y],x,80,fc) ;
       x += 4 ;
     }
     if (t==1)
     {
       p=str[i]-'!' ; p <<= 4 ;
       fPBitImgCp(FP_SEG(_ECBITMAP+p),FP_OFF(_ECBITMAP+p),1,16,_PSEG[y],x,80,fc) ;
       x += 2 ;
     }
   }
}

/**
  fast plane mode put Sprite from text bitmap
  input x,y   : locate of x,y
        ch    : ascii code
        FC,BC : foreground , background Sprite
        IMG   : Image Data Structure

  ‰¡­¢ Íw¡e ¡¡—a §¡Ëa “ Â‰b žË¥ ( ¸ñ ”¯¥ ¯aÏaœa·¡Ëa¡ ‹aŸ¥”a. )
  Çe‹i¸aŸi ‹aŸ©˜ ¬a¶w–E”a.
**/
void fPLargeECh(x,y,xt,yt,ch,FC,BC,IMG)
unsigned int x,y ;
unsigned char ch ;
Sprite *FC,*BC ;
Image *IMG ;
{
   unsigned int i,p,j ;
   unsigned char f ;
   int dx[8] ;

   dx[0]=3 ; dx[1]=2 ; dx[2]=1 ; dx[3]=0 ;
   dx[4]=7 ; dx[5]=6 ; dx[6]=5 ; dx[7]=4 ;

   p=ch-'!' ; p <<= 4 ;
   for ( i=3 ; i<12 ; i++,y+=yt )
   {
     f = *(_ECBITMAP+i+p) ;
     for ( j=0 ; j<8 ; j++ )
     {
       if (j==4) continue ;
       if (f&(0x80>>j)) pCTrpSpr0(x+dx[j]*xt,y,FC,IMG) ;
                else    pCTrpSpr0(x+dx[j]*xt,y,BC,IMG) ;
     }
   }
}
