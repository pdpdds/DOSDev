/**
   VGA 320*200, 256 Color Graphic Macro
   §aˆa 320*200 256 Äiœa ‹aœÏ¢ ¸÷· 
 
   1992. 9.17 ¸÷ µw ”â 
**/

extern  unsigned char far *_ADD[200] ; /* packed ¡¡—a ´á—aA¯a Í¡·¥Èá */ 
extern  unsigned int  far  _SEG[200],*_PSEG ; /* packed ¡¡—a ´á—aA¯a ­A‹a åËa
                                                 plane ¡¡—a ¸b´óÍA·¡»¡ Í¡·¥Èá */
extern  unsigned int  far  _PSE[][200] ; /* plane ¡¡—a 4 page ´á—aA¯a ­A‹a åËa */

/**
   Packed Mode Graphic Macro
   packed ¡¡—a ‹aœÏ¢  aÇa¡
**/

/* x,y ¹ÁÎaµA ¬‚ c ¡ ‹aŸ¥”a. */
#define setP(X,Y,C)          (*(_ADD[(Y)]+(X)) = (C))
/* x,y µA¬á xl,yl ‹©·¡· barŸi ¬‚ c¡ ‹aŸ¥”a. */
#define barL(X,Y,XL,YL,C)    imgFill(_SEG[(Y)],(X),320,(XL),(YL),(C))
/* x1,y1 µA¬á x2,y2 Œa»¡ ¬‚ c¡ barŸi ‹aŸ¥”a. */
#define bar2(X1,Y1,X2,Y2,C)  imgFill(_SEG[(Y1)],(X1),320,(X2)-(X1)+1,(Y2)-(Y1)+1,(C))
/* ÐeÑÁ¡e·i ¬‚ c¡ Ã©Ðe”a. */
#define ClrScr(C)            imgHFill(0xa000,0,0xfa00,(C))
/* x,yµA¬á ®Íw‹©·¡ l¡ ®Íw¬å·i ¬‚ c¡ ‹aŸ¥”a. */
#define lineHL(X,Y,L,C)      imgHFill(_SEG[(Y)],(X),(L),(C))
/* x,yµA¬á x2,yŒa»¡ ®Íw¬å·i ¬‚ c¡ ‹aŸ¥”a. */
#define lineH2(X1,Y,X2,C)    imgHFill(_SEG[(Y)],(X),(X2)-(X1)+1,(C))
/* x,yµA¬á ®»¢‹©·¡ l¡ ®»¢¬å·i ¬‚ c¡ ‹aŸ¥”a. */
#define lineVL(X,Y,L,C)      imgVFill(_SEG[(Y)],(X),320,(L),(C))
/* x,yµA¬á x,y2Œa»¡ ®»¢¬å·i ¬‚ c¡ ‹aŸ¥”a. */
#define lineV2(X,Y1,Y2,C)    imgVFill(_SEG[(Y1)],(X),320,(Y2)-(Y1)+1,(C))
/* ¹ÁÎa x,yµA ¬a·¡»aˆa C,R·¡‰¡ ¸aža Í¡·¥Èáˆa P·¥ ¯aÏaœa·¡ËaŸi ‹aŸ¥”a. */
#define putSpr0(X,Y,P,C,R)   sprCp(FP_SEG(P),FP_OFF(P),C,C,R,_SEG[Y],X,320)
/* ¹ÁÎa x,yµA ¬a·¡»aˆa C,R·¡‰¡ ¸aža Í¡·¥Èáˆa P·¥ ¯aÏaœa·¡ËaŸi ˆá¶‰Ñw¬w·a¡ ‹aŸ¥”a. */
#define putSpr1(X,Y,P,C,R)   sprCpMir(FP_SEG(P),FP_OFF(P),C,C,R,_SEG[Y],X,320)
/* ¹ÁÎa x,yµA ¬a·¡»aˆa C,R·¡‰¡ ¸aža Í¡·¥Èáˆa P·¥ ¯aÏaœa·¡ËaŸi –á»³´á ‹aŸ¥”a. */
#define putSpr2(X,Y,P,C,R)   sprCpUD(FP_SEG(P),FP_OFF(P),C,C,R,_SEG[Y],X,320)
/* ¹ÁÎa x,yµA ¬a·¡»aˆa C,R·¡‰¡ ¸aža Í¡·¥Èáˆa P·¥ ¯aÏaœa·¡ËaŸi –á»³‰¡ ˆá¶‰Ñw¬w·a¡ ‹aŸ¥”a. */
#define putSpr3(X,Y,P,C,R)   sprCpMirUD(FP_SEG(P),FP_OFF(P),C,C,R,_SEG[Y],X,320)

/**
   Plane Mode Graphic Marco
   4 ÍA·¡»¡ Íw¡e ¡¡—a ‹aœÏ¢  aÇa¡
**/

/* x,y ¹ÁÎaµA ¬‚ c ¡ ‹aŸ¥”a. */
#define psetP(X,Y,C)         pBytePut(_PSEG[(Y)],(X),(C))
/* x,y µA¬á xl,yl ‹©·¡· barŸi ¬‚ c¡ ‹aŸ¥”a. */
#define pBarL(X,Y,XL,YL,C)   pImgFill(_PSEG[(Y)],(X),80,(XL)+(X)-1,(YL),(C))
/* x1,y1 µA¬á x2,y2 Œa»¡ ¬‚ c¡ barŸi ‹aŸ¥”a. */
#define pBar2(X1,Y1,X2,Y2,C) pImgFill(_PSEG[(Y1)],(X1),80,(X2),(Y2)-(Y1)+1,(C))
/* x,yµA¬á ®Íw‹©·¡ l¡ ®Íw¬å·i ¬‚ c¡ ‹aŸ¥”a. */
#define pLineHL(X,Y,L,C)     pImgHFill(_PSEG[(Y)],(X),(X)+(L)-1,(C))
/* x,yµA¬á x2,yŒa»¡ ®Íw¬å·i ¬‚ c¡ ‹aŸ¥”a. */
#define pLineH2(X1,Y,X2,C)   pImgHFill(_PSEG[(Y)],(X1),(X2),(C))
/* x,yµA¬á ®»¢‹©·¡ l¡ ®»¢¬å·i ¬‚ c¡ ‹aŸ¥”a. */
#define pLineVL(X,Y,L,C)     pImgVFill(_PSEG[(Y)],(X),80,(L),(C))
/* x,yµA¬á x,y2Œa»¡ ®»¢¬å·i ¬‚ c¡ ‹aŸ¥”a. */
#define pLineV2(X,Y1,Y2,C)   pImgVFill(_PSEG[(Y1)],(X),80,(Y2)-(Y1)+1,(C))
/* ÐeÑÁ¡e·i ¬‚ c¡ Ã©Ðe”a. */
#define pClrScr(C)           imgHFill(_PSEG[0],0,0x4000,(C)) ;
/* ÍA·¡»¡ sµA¬á ÍA·¡»¡ d¡ ‰¡­¢ ¸å­·Ðe”a */
#define page2PageCp(S,D)     writeMode(1) ; pImgCpLU(_PSE[(S)][0],0,80,80,200,_PSE[(D)][0],0,80) ;
/* ¹ÁÎa x,yµA ¬a·¡»aˆa C,R·¡‰¡ ¸aža Í¡·¥Èáˆa P·¥ ¯aÏaœa·¡ËaŸi ‹aŸ¥”a. */
#define pPutSpr0(X,Y,P,C,R)  psprCp(FP_SEG(P),FP_OFF(P),C,C,R,_PSEG[Y],X,80)
/* ¹ÁÎa x,yµA ¬a·¡»aˆa C,R·¡‰¡ ¸aža Í¡·¥Èáˆa P·¥ ¯aÏaœa·¡ËaŸi ˆá¶‰Ñw¬w·a¡ ‹aŸ¥”a. */
#define pPutSpr1(X,Y,P,C,R)  psprCpMir(FP_SEG(P),FP_OFF(P),C,C,R,_PSEG[Y],X,80)
/* ¹ÁÎa x,yµA ¬a·¡»aˆa C,R·¡‰¡ ¸aža Í¡·¥Èáˆa P·¥ ¯aÏaœa·¡ËaŸi –á»³´á ‹aŸ¥”a. */
#define pPutSpr2(X,Y,P,C,R)  psprCpUD(FP_SEG(P),FP_OFF(P),C,C,R,_PSEG[Y],X,80)
/* ¹ÁÎa x,yµA ¬a·¡»aˆa C,R·¡‰¡ ¸aža Í¡·¥Èáˆa P·¥ ¯aÏaœa·¡ËaŸi –á»³‰¡ ˆá¶‰Ñw¬w·a¡ ‹aŸ¥”a. */
#define pPutSpr3(X,Y,P,C,R)  psprCpMirUD(FP_SEG(P),FP_OFF(P),C,C,R,_PSEG[Y],X,80)
/* ¥¡µaº“e ÍA·¡»¡Ÿi ¬é¸÷Ðe”a */
#define viewPage(P)          setStAdd((P)*0x4000)
/* Íw¡e ³a‹¡ ¡¡—a 1 */
#define planeWrite1          planeMode() ; writeMode(1) ;
/* Íw¡e ³a‹¡ ¡¡—a 0 */
#define planeWrite0          planeMode() ; writeMode(0) ;
/* packed ³a‹¡ ¡¡—a 1 */
#define packWrite1           packMode()  ; writeMode(1) ;
/* packed ³a‹¡ ¡¡—a 0 */
#define packWrite0           packMode()  ; writeMode(0) ;

/* ¤åÑ¡ iµA red,green,blueˆt·a¡ ÌiAËa ¬é¸÷Ðe”a */
#define setRGB(I,R,G,B)      set1DACR((I),(R),(G),(B))

#define _R7 0xe0
#define _R6 0xc0
#define _R5 0xa0
#define _R4 0x80
#define _R3 0x60
#define _R2 0x40
#define _R1 0x20

#define _G7 0x1c
#define _G6 0x18
#define _G5 0x14
#define _G4 0x10
#define _G3 0x0c
#define _G2 0x08
#define _G1 0x04

#define _B3 0x03
#define _B2 0x02
#define _B1 0x01

