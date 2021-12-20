/**
   Plane Mode Sprite Kernel program
   Borland C++ 2.0 Compiler Only
   by Jung y.d.

   Image & Sprite Data Structure is not conventional format.
   Special Format of Street Fighter ][

   §aˆa ÏiE ¡¡—aµA¬á ¯aÏaœa·¡Ëa ‹aŸ¡“e žË¥
   ¸÷ µw ”â
   ¯aÏaœa·¡ËaµÁ ‹aŸ± Í¡ •·e ·©¤e¸â·¥ Í¡ •·¡ ´a“±.
**/

#include <alloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

#include "animate0.h"
#include "grker0.h"

extern unsigned char *_EBF ; //  encoding data temporary save buffer

/**
  initialize image data structure
  input img : Image Data structure
          n : initialized image size

  ‹aŸ± •A·¡Èa Š¹¡µA¬á ‹aŸ± Í¡·¥Èá ¦¦…·i Á¡‹¡ÑÁ Ðe”a.
**/
void initImg(img,n)
Image img[] ;
int n ;
{
   int i ;

   for ( i=0 ; i<n ; i++ ) img[i].p=NULL ;
}

/**
  free image buffer if image buffer allocated
  input img : Image Data structure
          n : free image size

  Ði”w–E ‹aŸ± •A·¡Èa µwµb·i ¤eÑÅÐe”a.
**/
void freeImg(img,n)
Image img[] ;
int n ;
{
   int i ;

   for ( i=0 ; i<n ; i++ )
   if (img[i].p!=NULL) { free(img[i].p) ; img[i].p=NULL ; }
}


/**
  load Image & Sprite Data From identification File
  input IdFn : Identification File Name
        img  : Input Image  data structure
        spr  : Input Sprite data structure
        flag : 0 -> image data is raw format ( not encoding )
               1 -> image data is modified RLE
               RLE is Run length Encoding

  IdFn File is special format of Street Fighter ][

  .ID ÑÁ·©·¡a .IDE ÑÁ·©¡ ¦Èá ‹aŸ±, ¯aÏaœa·¡Ëa ¸÷¥¡Ÿi ·ª“e”a.

**/
loadImgSpr(IdFn,img,spr,flag)
char IdFn[] ;
Image img[] ;
Sprite spr[] ;
int flag ;
{
   FILE *fp1,*fp2 ;
   char imgFn[64],sprFn[64] ;
   unsigned int n,in ;
   int i,x1,y1,x2,y2,xo,yo ;

   fp1=fopen(IdFn,"r") ;
   if (fp1==NULL) cError(IdFn," Not Found !",1) ;

   fscanf(fp1,"%s",imgFn) ;
   fscanf(fp1,"%s",imgFn) ;
   fp2=fopen(imgFn,"rb") ;
   if (fp2==NULL) cError(imgFn," Not Found !",1) ;

   for ( n=0 ; ; n++ )
   {
     fscanf(fp1,"%s",imgFn) ;
     if (imgFn[0]=='_') break ;
     if (flag) fscanf(fp1,"%u %u %u",&img[n].ls,&img[n].xs,&img[n].ys) ;
         else  fscanf(fp1,"%u %u",&img[n].xs,&img[n].ys) ;
   }

   for ( i=0 ; i<n ; i++ )
   {
     if (img[i].p!=NULL) free(img[i].p) ;
     if (flag) img[i].p = malloc(img[i].ls) ;
        else   img[i].p = malloc(img[i].xs * img[i].ys) ;
     if (img[i].p==NULL) cError("","Insuff Mem",1) ;
     if (flag) fread(img[i].p,1,img[i].ls,fp2) ;
         else  fread(img[i].p,img[i].xs,img[i].ys,fp2) ;
   }

   fscanf(fp1,"%s",sprFn) ;

   for ( n=0 ; ; n++ )
   {
     fscanf(fp1,"%s",sprFn) ;
     if (sprFn[0]=='_') break ;
     x1=y1= (1000) ; x2=y2=(-1000) ;
     for ( in=0 ; ; in++ )
     {
       fscanf(fp1,"%d",&i) ; if (i<0) break ;
       spr[n].i[in] = i ;
       fscanf(fp1,"%d %d",&xo,&yo) ;
       spr[n].xo[in]=xo ; spr[n].yo[in]=yo ;
       if (xo<x1) x1=xo ; if (yo<y1) y1=yo ;
       if (xo+img[i].xs>x2) x2=xo+img[i].xs ;
       if (yo+img[i].ys>y2) y2=yo+img[i].ys ;
     }
     spr[n].n=in ; spr[n].xs=x2-x1 ; spr[n].ys=y2-y1 ; spr[n].xfs=x2-1 ;
     spr[n].xh1 = spr[n].yh1 = 0 ;
     spr[n].xh2 = x2-x1 ; spr[n].yh2 = y2-y1 ;
   }

   if (fscanf(fp1,"%s",sprFn)==1)
      for ( i=0 ; ; i++ )
      {
        if (fscanf(fp1,"%d %d %d %d",&x1,&y1,&x2,&y2)!=4) break ;
        if (x1==0 && y1==0 && x2==0 && y2==0) continue ;
        spr[i].xh1 = x1 ; spr[i].yh1 = y1 ;
        spr[i].xh2 = x2 ; spr[i].yh2 = y2 ;
      }

   fclose(fp1) ; fclose(fp2) ;
   return(n) ;
}

/**
  plane mode put RLE Sprite in Current active page
  input x,y : put locate of x,y
        spr : Sprite Data Structure
        img : Image Data Structure
          m : put mode
              0 -> normal put
              1 -> mirror put
              2 -> upside down put
              3 -> upside down & mirror put

  Íw¡e ¡¡—aµA¬á ˆe·¡ RLE ÑwÈ· spriteŸi ‹aŸ¥”a.
**/
void pPutRLESprMode(x,y,spr,img,m)
int x,y ;
Sprite *spr ;
Image *img  ;
int m ;
{
   int i,ip,xs,ys ;

   for ( i=0 ; i<spr->n ; i++ )
   {
     ip=spr->i[i] ;  xs=img[ip].xs ; ys=img[ip].ys ;
     asmDecode(img[ip].p,_EBF,img[ip].ls) ;
     switch (m)
     {
       case 1 : pCTrpRImg1(x-spr->xo[i]-xs,y+spr->yo[i],_EBF,xs,ys) ; break ;
       case 2 : pCTrpRImg2(x+spr->xo[i],y+spr->yo[i],_EBF,xs,ys)    ; break ;
       case 3 : pCTrpRImg3(x-spr->xo[i]-xs,y+spr->yo[i],_EBF,xs,ys) ; break ;
       default: pCTrpRImg0(x+spr->xo[i],y+spr->yo[i],_EBF,xs,ys)    ; break ;
     }
   }
}

/**
  plane mode put Sprite in Current active page
  input x,y : put locate of x,y
        spr : Sprite Data Structure
        img : Image Data Structure
          m : put mode
              0 -> normal put
              1 -> mirror put
              2 -> upside down put
              3 -> upside down & mirror put
  Íw¡e ¡¡—aµA¬á ¯aÏaœa·¡ËaŸi ‹aŸ¥”a.
**/
void pPutSprMode(x,y,spr,img,m)
int x,y ;
Sprite *spr ;
Image *img  ;
int m ;
{
   int i,ip,xs,ys ;

   for ( i=0 ; i<spr->n ; i++ )
   {
     ip=spr->i[i] ;  xs=img[ip].xs ; ys=img[ip].ys ;
     switch (m)
     {
       case 1 : pCTrpRImg1(x-spr->xo[i]-xs,y+spr->yo[i],img[ip].p,xs,ys) ; break ;
       case 2 : pCTrpRImg2(x+spr->xo[i],y+spr->yo[i],img[ip].p,xs,ys)    ; break ;
       case 3 : pCTrpRImg3(x-spr->xo[i]-xs,y+spr->yo[i],img[ip].p,xs,ys) ; break ;
       default: pCTrpRImg0(x+spr->xo[i],y+spr->yo[i],img[ip].p,xs,ys)    ; break ;
     }
   }
}


/**
  plane mode Normal put Sprite in Current active page
  input x,y : put locate of x,y
        spr : Sprite Data Structure
        img : Image Data Structure
**/
void pCTrpSpr0(x,y,spr,img)
int x,y ;
Sprite *spr ;
Image *img  ;
{
   int i,ip ;

   for ( i=0 ; i<spr->n ; i++ )
   {
     ip=spr->i[i] ;
     pCTrpRImg0(x+spr->xo[i],y+spr->yo[i],img[ip].p,img[ip].xs,img[ip].ys) ;
   }
}


/**
  plane mode mirror put Sprite in Current active page
  input x,y : put locate of x,y
        spr : Sprite Data Structure
        img : Image Data Structure
**/
void pCTrpSpr1(x,y,spr,img)
int x,y ;
Sprite *spr ;
Image *img  ;
{
   int i,ip,xs ;

   for ( i=0 ; i<spr->n ; i++ )
   {
     ip=spr->i[i] ; xs=img[ip].xs ;
     pCTrpRImg1(x-spr->xo[i]-xs,y+spr->yo[i],img[ip].p,xs,img[ip].ys) ;
   }
}

/**
  plane mode upside down put Sprite in Current active page
  input x,y : put locate of x,y
        spr : Sprite Data Structure
        img : Image Data Structure
**/
void pCTrpSpr2(x,y,spr,img)
int x,y ;
Sprite *spr ;
Image *img  ;
{
   int i,ip,xs,ys ;

   for ( i=0 ; i<spr->n ; i++ )
   {
     ip=spr->i[i] ; xs=img[ip].xs ; ys=img[i].ys ;
     pCTrpRImg2(x+spr->xo[i],y-spr->yo[i]-ys,img[ip].p,xs,ys) ;
   }
}

/**
  plane mode upside down & mirror put Sprite in Current active page
  input x,y : put locate of x,y
        spr : Sprite Data Structure
        img : Image Data Structure
**/
void pCTrpSpr3(x,y,spr,img)
int x,y ;
Sprite *spr ;
Image *img  ;
{
   int i,ip,xs,ys ;

   for ( i=0 ; i<spr->n ; i++ )
   {
     ip=spr->i[i] ; xs=img[ip].xs ; ys=img[i].ys ;
     pCTrpRImg3(x-spr->xo[i]-xs,y-spr->yo[i]-ys,img[ip].p,xs,ys) ;
   }
}

