/****

   AT ÑÂ¸w 101 Ç¡·¡¥¡—a ¡¶A¥I ¹A´á žË¥

   MSC Version 1991.4.30 
   Borland C++ 2.0 Version Created 1992.8.23
****/

#include <dos.h>
#include <stdio.h>

/** Ç¡·¡¥¡—a ¯aÅEÅ¡—aµÁ ‹aµA ”·wÐa“e Ç¡·¡¥¡—a ·¡Ÿq ¸÷· */
#define _ESC 1          /* ESC key                    */

#define _1 2            /* 1 key                      */
#define _2 3            /* 2                          */
#define _3 4            /* 3                          */
#define _4 5            /* 4                          */
#define _5 6            /* 5                          */
#define _6 7            /* 6                          */
#define _7 8            /* 7                          */
#define _8 9            /* 8                          */
#define _9 10           /* 9                          */
#define _0 11           /* 0                          */
#define _MINUS 12       /* -                          */
#define _EQUAL 13       /* =                          */
#define _BACKSP 14      /* back space                 */
#define _Q 16           /*                            */
#define _W 17           /*                            */
#define _E 18           /*                            */
#define _R 19           /*                            */
#define _T 20           /*                            */
#define _Y 21           /*                            */
#define _U 22           /*                            */
#define _I 23           /*                            */
#define _O 24           /*                            */
#define _P 25           /*                            */
#define _LPAREN 26      /* [                          */
#define _RPAREN 27      /* ]                          */
#define _A 30           /*                            */
#define _S 31           /*                            */
#define _D 32           /*                            */
#define _F 33           /*                            */
#define _G 34           /*                            */
#define _H 35           /*                            */
#define _J 36           /*                            */
#define _K 37           /*                            */
#define _L 38           /*                            */
#define _SEMI 39        /*  ;                         */
#define _QUATO 40       /*  '                         */
#define _RSLASH 43      /*  \ key                     */
#define _Z 44           /*                            */
#define _X 45           /*                            */
#define _C 46           /*                            */
#define _V 47           /*                            */
#define _B 48           /*                            */
#define _N 49           /*                            */
#define _M 50           /*                            */
#define _COMMA 51       /*  ,                         */
#define _DOT 52         /*  .                         */
#define _SLASH 53       /*  / key                     */
#define _SPACE 57       /*  space bar key             */

#define _F1  59         /*  Function 1 key            */
#define _F2  60         /*                            */
#define _F3  61         /*                            */
#define _F4  62         /*                            */
#define _F5  63         /*                            */
#define _F6  64         /*                            */
#define _F7  65         /*                            */
#define _F8  66         /*                            */
#define _F9  67         /*                            */
#define _F10 68         /*                            */

#define _RET 28         /* Return or Enter key        */
#define _TAB 15         /* Tab key                    */

#define _LSHIFT 42      /* Left Shift Key             */
#define _RSHIFT 54      /* Right Shift Key            */

#define _LCTRL 29       /* Left Control key           */
#define _RCTRL 29       /* Right Control key          */

#define _LALT 56        /* Left Alternate key         */
#define _RALT 56        /* Right Alternate key        */

#define _NHOME 71       /* XT 83 Keyborad Home key    */
#define _NPGUP 73       /*                Page Up     */
#define _NEND  79       /*                End         */
#define _NPGDN 81       /*                Page Down   */
#define _NINS  82       /*                Insert      */
#define _NDEL  83       /*                Delete      */

#define _NUMLOCK 69     /* Number Lock                /*
#define _CAPLOCK 58     /* Caps lock                  /*
#define _SCRLOCK 70     /* scroll lock                */

#define _UPARR 72       /* ¶á ÑÁ¬iÎa                  */
#define _DWARR 80       /* ´aœ ÑÁ¬iÎa                */
#define _LFARR 75       /* ¶E½¢ ÑÁ¬iÎa                */
#define _RGARR 77       /* µ¡Ÿe½¢ ÑÁ¬iÎa              */

#define KBDINTVECT 0x9  /* Ç¡·¡¥¡—a ·¥ÈáœóËa ¤åÑ¡     */

/** 
  Ç¡·¡¥¡—a ¯aÅEÅ¡—a ¸÷¥¡Ÿi ”q“e ¤µi
  µ) ¶áÑÁ¬iÎa Ç¡·¡ˆa ’‰œá»¥ ¬wÈ·¡¡e ¯aÅEÅ¡—aˆa 77 ·¡£a¡ _KS[77]·¡ 1·¡ –E”a.
		      ˜é´á»¥ ¬wÈ·¡¡e                      _KS[77]·¡ 0·¡ –E”a.
**/
static unsigned char _KS[128] =
{
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
} ;

/**
  Ç¡·¡¥¡—a ¯aÅEÅ¡—a µA»¡ ¸÷¥¡Ÿi ”q“e ¤µi
  µa‹¡¬á µA»¡ ¸÷¥¡œe, Ç¡·¡ˆa ’‰œá»¥ ®…ˆe·i ·£¡Ðs“¡”a. ·¡ˆõ·e ´‚­e ‰A·±·¡a
  ‹¡Èa ‰A·±µA¬á Á··i ±©˜“e ’‰œá»¥ ¬wÈµA¬á ‰­¢ ¤i¬i–A¡e ´e–A£a¡ ’‰œá»¥ 
  ®…ˆeµA ¤i¬a–A‹¡ ¶áÐaµa ¬a¶w–S“¡”a.
**/
static unsigned char _KE[128] =
{
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
} ;

/**
  ´a¢Ç¡·¡a ’‰œá»¥ ¬wÈ¡e ‹a˜ ’‰œá»¥ Ç¡·¡· ¯aÅEÅ¡—aˆt·¡ ‹¡¢–S“¡”a.
  Hit Any key to start—wµA ¬a¶w
**/
unsigned char _KBHIT=0 ;

/** ´a¢Ç¡·¡ˆa ’‰œá»©˜ Œa»¡ ”‹¡Ðe”a. ***/
#define KBDWAIT  for ( _KBHIT=0 ; !_KBHIT ; ) ; _KBHIT=0 ;

/** ‹¡¹¥· Ç¡¥¡—a ·¥ÈáœóËa žË¥ Ðq® Í¡·¥Èá **/
void interrupt far (*oldKBD)(void) ;

/** ¬¡¶… Ç¡¥¡—a ·¥ÈáœóËa žË¥ **/
void interrupt keyStatusCheck(void)
{
   unsigned char k,scancode ;

   asm cli ;  /* ·¥ÈáœóËa ‹q»¡ */

   k=inportb(0x60) ;  /* Ç¡·¡ˆt·i ·ª´áµ¥”a. */

   scancode = k&(0x7f) ;  /* ¯aÅEÅ¡—a  e·i ÂÂ‰Ðe”a. */

   if (k<128 && _KS[scancode]==0)  /* Ç¡·¡ˆa ’‰œá»¥ ®…ˆe */ 
   {
     _KS[scancode]=1 ; _KBHIT=scancode ; _KE[scancode]=1 ;
   }

   /** Ç¡·¡ˆa ˜é´á»¥ ®…ˆe **/
   if (k>127) _KS[scancode]=0 ;
   /**
   (*oldKBD)() ;
    ·¡ žË¥·i ³a¡e ¶¥œ· Ç¡¥¡—a žË¥•¡ ®Ð—Ðe”a. getch() Ðq®—w·i ³i®·¶”a.
    º·Ði¸ñ·e Ç¡·¡Ÿi µ¡œ’Ÿa‰¡ ·¶·a¡e «¢ ­¡Ÿ¡ˆa e”a.
    -> ¤a·¡µ¡¯aÅ©¡ ­¡Ÿ¡Ÿi ´ô´‰® ·¶”a.
   **/
   outportb(0x20,0x20) ;  /* ·¥ÈáœóËa ÒA¡µA {v·q·i ´iaº‘ */
                          /* (*oldKBD)()Ÿi  å¸á ¦Ÿa“e ‰w¶µA“e ·¡ žË¥·¡
                             Í¡Ðq–A´á ·¶·a£a¡ ´ô´á•¡ –E”a */
                         
   asm sti ;  /* ·¥ÈáœóËa ‹q»¡ Ð¹A */
}

/** Ç¡·¡¥¡—a ·¥ÈáœóËa žË¥·i ¬¡¶…ˆõ·a¡ ¤aŽ…”a. **/
int setNewKBDInt(void)
{
   oldKBD   = getvect(KBDINTVECT) ;
   setvect(KBDINTVECT,keyStatusCheck) ;
   return(0) ;
}

/** Ç¡·¡¥¡—a ·¥ÈáœóËa žË¥·i ¶¥œ”¡ ¤aŽ…”a. **/
int setOldKBDInt(void)
{
   setvect(KBDINTVECT,oldKBD) ;
   return(0) ;
}

int main(void)
{
   int x,y,shoot ;

   setNewKBDInt() ; /* Ç¡·¡¥¡—a ·¥ÈáœóËa žË¥·i ¤aŽ…”a */

   printf(" Hit Any Key to Test Start !!\n") ;
   KBDWAIT ; /* ´a¢Ç¡ˆa ’‰œá»©˜ Œa»¡ ”‹¡  */

   x=0 ; y=0 ; shoot=0 ;

   /** ¤wÐ· ÑÁ¬iÎaÇ¡Ÿi ¶‘»¢·± Ç¡·¡¡ ¬é¸÷
	¯aÍA·¡¯a Ç¡·¡Ÿi Á·±¡“e Ç¡·¡¡ ¬é¸÷ **/

   for ( ; ; )
   {
     if (_KS[_LFARR]) x-- ;  /* ¹ÁÃb ÑÁ¬iÎaÇ¡·¡ˆa ’‰œá¹v·a¡e ¤wÐ· ˆq­¡ */
     if (_KS[_RGARR]) x++ ;  /* ¶Ãb ÑÁ¬iÎaÇ¡·¡ˆa ’‰œá¹v·a¡e ¤wÐ· »wˆa */
     if (_KS[_UPARR]) y-- ;  /*  ¶á ÑÁ¬iÎaÇ¡·¡ˆa ’‰œá¹v·a¡e ¤wÐ· ˆq­¡  */
     if (_KS[_DWARR]) y++ ;  /* ´aœ ÑÁ¬iÎaÇ¡·¡ˆa ’‰œá¹v·a¡e ¤wÐ· »wˆa */
     /* Á·±¡“e Ç¡·¡, ’‰œá¹v·i˜ e »wˆa */
     if (_KE[_SPACE]) { _KE[_SPACE]=0 ; shoot++ ; }
     if (_KE[_ESC])   break ; /*  ESC  Ç¡·¡ˆa ’‰œá»¡¡e {…”a . */
     printf("x=%5d y=%5d  shoot=%5d\n",x,y,shoot) ;
   }

   setOldKBDInt() ;

   return(0);
}



