/****
   STREET FIGHTER ][ KEYBOARD INPUT ROUTINE
   Borland C++ 2.0 Version Created 1992.8.23  By Jung Y.D.

   ¯aÌa2¶w Ç¡¥¡—a ·³b žË¥ 
****/

/* Ç¡¥¡—a ¯aÅE Å¡—a ¸÷· */

#define _ESC 1          // ESC key

#define _1 2            // 1 key
#define _2 3            // 2
#define _3 4            // 3
#define _4 5            // 4
#define _5 6            // 5
#define _6 7            // 6
#define _7 8            // 7
#define _8 9            // 8
#define _9 10           // 9
#define _0 11           // 0
#define _MINUS 12       // -
#define _EQUAL 13       // =
#define _BACKSP 14      // back space
#define _Q 16           //
#define _W 17           //
#define _E 18           //
#define _R 19           //
#define _T 20           //
#define _Y 21           //
#define _U 22           //
#define _I 23           //
#define _O 24           //
#define _P 25           //
#define _LPAREN 26      // [
#define _RPAREN 27      // ]
#define _A 30           //
#define _S 31           //
#define _D 32           //
#define _F 33           //
#define _G 34           //
#define _H 35           //
#define _J 36           //
#define _K 37           //
#define _L 38           //
#define _SEMI 39        //  ;
#define _QUATO 40       //  '
#define _RSLASH 43      //  \ key
#define _Z 44           //
#define _X 45           //
#define _C 46           //
#define _V 47           //
#define _B 48           //
#define _N 49           //
#define _M 50           //
#define _COMMA 51       //  ,
#define _DOT 52         //  .
#define _SLASH 53       //  / key
#define _SPACE 57       //  space bar key

#define _F1  59         //  Function 1 key
#define _F2  60         //
#define _F3  61         //
#define _F4  62         //
#define _F5  63         //
#define _F6  64         //
#define _F7  65         //
#define _F8  66         //
#define _F9  67         //
#define _F10 68         //

#define _RET 28         // Return or Enter key
#define _TAB 15         // Tab key

#define _LSHIFT 42      // Left Shift Key
#define _RSHIFT 54      // Right Shift Key

#define _LCTRL 29       // Left Control key
#define _RCTRL 29       // Right Control key

#define _LALT 56        // Left Alternate key
#define _RALT 56        // Right Alternate key

#define _NHOME 71       // XT 83 Keyborad Home key
#define _NPGUP 73       //                Page Up
#define _NEND  79       //                End
#define _NPGDN 81       //                Page Down
#define _NINS  82       //                Insert
#define _NDEL  83       //                Delete

#define _NUMLOCK 69     // Number Lock
#define _CAPLOCK 58     // Caps lock
#define _SCRLOCK 70     // scroll lock

#define _UPARR 72
#define _DWARR 80
#define _LFARR 75
#define _RGARR 77

#define TIMERINTVECT 0x1c  /* Èa·¡ á ·¥ÈáœóËa ¥BÈá */
#define KBDINTVECT 0x9     /* Ç¡·¡¥¡—a ·¥ÈáœóËa ¥BÈá */

/** Ç¡·¡ ’Ÿe ¬wÈ ¥e® **/
unsigned char _KS[128] =
{
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
} ;

/** Ç¡·¡ µA»¡ ¬wÈ ¥e® **/
unsigned char _KE[128] =
{
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
} ;

/**
  _KBHIT   Ç¡¥¡—aˆa ’‰œá¹v”a. 
  _LKBHIT  ¹ÁÃb ÏiA·¡´áµA Ði”w–E Ç¡¥¡—aˆa ’‰œá¹v”a
  _RKBHIT  ¶Ãb ÏiA·¡´áµA Ði”w–E Ç¡¥¡—aˆa ’‰œá¹v”a.
  scanHashTbl ¹Á¶Ãb ÏiA·¡´áˆa ’Ÿe Ç¡· ¯aÅEÅ¡—a hash table
**/
unsigned char _KBHIT=0,_LKBHIT,_RKBHIT,scanHashTbl[128] ;
unsigned char *scan2Asc="UDLR456123",Timer ;

/* Ç¡¥¡—aˆa ’‰œá»©˜ Œa»¡ ‹¡”aŸ¥”a. */
#define KBDWAIT  for ( _KBHIT=0 ; !_KBHIT ; ) ;

/* Ç¡¥¡—a ·³b·i ¶áÐe ¤áÌá */
typedef struct
{
   unsigned char b[17],p ;
}  KEYBUFF ;

/* 
  ÏiA·¡´á ¹¡¸÷Ç¡ ¯aÅEÅ¡—a 
   U:¸ñÏa  L:¹Á  R:¶   D:´f·q
   a,b,c:¤i‰·‰b ´¢,º—,ˆw, x,y,z:­¥‰·‰b ´¢,º—,ˆw
*/
typedef struct
{
   unsigned char U,L,R,D,a,b,c,x,y,z ;
}  CTRLKEY ;

/* ¹Á,¶Ãb ÏiA·¡´áµA‰A  Ç¡¥¡—a ¤áÌá Ði”w */
KEYBUFF  LKB,RKB ;
/* —¡Í©Ëa ¹ÁÃb ÏiA·¡´á ¹¡¸÷ Ç¡ (¯aÅE Å¡—aˆt) */
CTRLKEY  LK = { 17,30,31,44,19,20,21,33,34,35 } ;
/* —¡Í©Ëa ¶Ãb ÏiA·¡´á ¹¡¸÷ Ç¡ (¯aÅE Å¡—aˆt) */
CTRLKEY  RK = { 72,75,77,80,82,71,73,83,79,81 } ;

#define LEFTMASK 0x40
#define RGHTMASK 0x20

unsigned char *_tmp = "abcdefghij" ;
unsigned char *Rkey2asc = "ulrd123456" ;
unsigned char *Lkey2asc = "ulrd123456" ;

/* 
   ¯aÌa2 ÑÅ‰w ¥e® 
   WAITCONST : ÄñÏAÈá ­¢•¡µA ˜aŸe ¯¡ˆe »¡µe ¥e®    
   COMTYPE   : ÄñÏAÈá ¹·ŸA ( §¡—¡µ¡ Äa—a blinking timing ‰i¸÷ )
   SOUND_DEV : ¸wÀb–E ¬a¶…—a Äa—a ¹·ŸA 
*/
int WAITCONST,COMTYPE,SOUND_DEV ;

/* ¹Á,¶Ãb ÏiA·¡´á Ç¡·¡—i· ¬wÈ¥e® */
unsigned char LKBDN,RKBDN ;

void interrupt (*oldKBD)(void) ;   /* ¶¥œ Ç¡¥¡—a ·¥ÈáœóËa žË¥ */
void interrupt (*oldTimer)(void) ; /* ¶¥œ Èa·¡ á ·¥ÈáœóËa žË¥ */

/* Ç¡¥¡—a ¬wÈ ÀÇa ·¥ÈáœóËa žË¥ */
void interrupt keyStatusCheck(void)
{
   unsigned char k,scancode,push,t ;
   unsigned char c,cu ;
   int i,r ;

   asm cli ;

   k=inportb(0x60) ;

   scancode = k&(0x7f) ;

/*
   ’Ÿaˆáa ‘½·e Ç¡·¡ˆa ¹Á¶Ãb ÏiA·¡´á ¹¡¸÷ Ç¡·¥ˆaŸi ˆñ¬aÐa‰¡ 
   ˆb ÏiA·¡´áµA Ði”w–E ¤áÌáµA ‹¡¢Ðe”a.
*/   

   if (k<128 && _KS[scancode]==0)
   {
     _KS[scancode]=1 ; _KBHIT=scancode ; _KE[scancode]=1 ;
     t=scanHashTbl[scancode] ;
     if (t&LEFTMASK)
     {
       LKBDN=0 ; c=Lkey2asc[t&0xf] ;
       LKB.b[LKB.p]=c ; LKB.p ++ ; if (LKB.p>15) LKB.p=0 ;
       _LKBHIT=c ;
     }
     if (t&RGHTMASK)
     {
       RKBDN=0 ; c=Rkey2asc[t&0xf] ;
       RKB.b[RKB.p]=c ; RKB.p ++ ; if (RKB.p>15) RKB.p=0 ;
       _RKBHIT=c ;
     }
   }

   /* Ç¡¥¡—a· Ç¡·¡Ÿi ’‰œö”a */
   if (k>127) 
   {
     t=scanHashTbl[scancode] ;
     if (t&LEFTMASK)
     {
       LKBDN=1 ; c=cu=Lkey2asc[t&0xf] ;  r=LKB.p ;
       if (_KS[scancode]>20)  cu=toupper(c) ;
       for ( i=0 ; i<16 ; i++ )
       {
         r-- ;  if (r<0) r=15 ;
         if (LKB.b[r]==c) { LKB.b[r]=cu ; break ; }
       }
     }
     if (t&RGHTMASK)
     {
       RKBDN=1 ; c=cu=Rkey2asc[t&0xf] ; r=RKB.p ;
       if (_KS[scancode]>20) cu=toupper(c) ;
       for ( i=0 ; i<16 ; i++ )
       {
         r-- ;  if (r<0) r=15 ;
         if (RKB.b[r]==c) { RKB.b[r]=cu ; break ; }
       }
     }
     _KS[scancode]=0 ;
   }
   outportb(0x20,0x20) ;
   asm sti ;
}

#define MAXGAB 6

/** Ç¡¥¡—aˆa ’‰œá»¥ ¯¡ˆe·i ¸…”a. (1/18.2 Á¡ˆe‰b·a¡) */
void interrupt sync(void)
{
   int i ;

   Timer++ ;

   /* Ç¡¥¡—aˆa ’‰œá¹v·a¡e ’‰œá»¥ ¯¡ˆe »wˆa */
   for ( i=0 ; i<127 ; i++ )
   if (_KS[i]>0 && _KS[i]<255) _KS[i]++ ;

   if (LKBDN)
   {
     LKBDN++ ;
     if (LKBDN>MAXGAB)
     {  LKB.b[LKB.p]='?' ; LKB.p ++ ; LKBDN=0 ; if (LKB.p>15) LKB.p=0 ; }
   }
   if (RKBDN)
   {
     RKBDN++ ;
     if (RKBDN>MAXGAB)
     {  RKB.b[RKB.p]='?' ; RKB.p ++ ; RKBDN=0 ; if (RKB.p>15) RKB.p=0 ; }
   }

   (*oldTimer)() ;
}

/* ·¥ÈáœóËa žË¥·i ¬³‰A Ði”wÐe”a */
setNewSystemInt()
{
   oldKBD   = getvect(KBDINTVECT) ;
   oldTimer = getvect(TIMERINTVECT) ;
   setvect(KBDINTVECT,keyStatusCheck) ;
   setvect(TIMERINTVECT,sync) ;
   LKB.p = RKB.p = LKB.b[16] = RKB.b[16] = LKBDN = RKBDN = 0 ;
   return(0) ;
}

/* ¶¥œ· ·¥ÈáœóËa žË¥·a¡ ­AË·Ðe”a */
setOldSystemInt()
{
   setvect(KBDINTVECT,oldKBD) ;
   setvect(TIMERINTVECT,oldTimer) ;
   return(0) ;
}

/* ¯aÌa2 ÑÅ‰w ¬é¸÷ ÑÁ·©·i ·ª“e”a */
loadConfig(fn)
char fn[] ;
{
   FILE *fp ;
   int i,lp ;
   unsigned char far *p ;

   lp=1000 ; COMTYPE=0 ; SOUND_DEV=0 ;
   fp=fopen(fn,"r") ;
   if (fp!=NULL)
   {
     fscanf(fp,"%u %u %u %u %u %u %u %u %u %u",&LK.U,&LK.L,&LK.R,&LK.D,
                &LK.a,&LK.b,&LK.c,&LK.x,&LK.y,&LK.z) ;
     fscanf(fp,"%u %u %u %u %u %u %u %u %u %u",&RK.U,&RK.L,&RK.R,&RK.D,
                &RK.a,&RK.b,&RK.c,&RK.x,&RK.y,&RK.z) ;
     fscanf(fp,"%d %d %d",&lp,&COMTYPE,&SOUND_DEV) ;
     fclose(fp) ;
   }

   WAITCONST = (50.0 - 2800.0*lp/3275.0) ;
   p = (unsigned char far *)&LK ;
   for ( i=0 ; i<10  ; i++ ) scanHashTbl[p[i]] = (LEFTMASK|i) ;

   p = (unsigned char far *)&RK ;
   for ( i=0 ; i<10  ; i++ ) scanHashTbl[p[i]] |= (RGHTMASK|i) ;
   return(0) ;
}

/* ÏiA·¡´áµA Ði”w–E Ç¡µA»¡ ¤áÌáŸi »¡¶…”a. */
flushKey(K)
CTRLKEY *K ;
{
   unsigned char far *p ;
   int i ;

   p = (unsigned char far *)K ;
   for ( i=0 ; i<10 ; i++ ) _KE[p[i]]=0 ;
   return(0) ;
}

/** ·©¸÷ ¯¡ˆe •·´e ‹¡”aŸ¥”a **/
waitSec(int t)
{
   Timer=0 ;
   for ( ; Timer!=t ; ) ;
}

