/****
    Street Fighter ][ Header File
    Original Game By CapCom
    IBM PC Game By Jung Young Dug ( KETEL ID : WD40 )
    Program Skill level : 1990.1 (0.7)
    Source Code Bolarnd C++ 2.0

    ¯aÌa2 ÑA”á ÑÁ·© 
    ¶¥¸b CAPCOM 
    IBM PC ¤á¸å ¸÷µw”â
    Ïa¡‹aœ‘ ‹¡®‰ ¸÷•¡ : 1990‘e 1¶© ®º…
    ­¡¯aÅ¡—a ¥©œe—a C++ 2.0
****/

#define MAXIMG 120    // Maximum image Size
#define MAXSPR 120    // Maximum Sprite Size

/**
   define Warrior ID Number
   ÄŸ¢Èá¥i ‰¡·A ®•¸a
**/

#define RYU     0
#define HONDA   1
#define BLANK   2
#define GUILE   3
#define KEN     4
#define CHUNLI  5
#define ZANGIEF 6
#define DALSIM  7

/* ÏiA·¡´á ¹·ŸA */
#define HUMAN  0  // Player is Human  ¬aœq·¡ ¹¡¸÷    
#define COMPU  1  // Player is Computer  ÄñÏAÈáˆa ¹¡¸÷
#define REMOTE 2  // Player connected Remote Modem  ¡¡•Q·a¡ µe‰i

/* žË¥ ¹A´á ¯¥Ñ¡ ¸÷· */ 
#define EXIT      1  // Keyboard Signal is EXIT ( ESC key pressed )
#define BREAK     2  // Keyboard Signal is BREAK ( F10 Key pressed )
#define CONTINUE  3  // Keyboard Signal is CONTINUE
#define NEWPLAYER 4  // New Player Come on Signal

/* ÏiA·¡´á ¹¡¸÷ ”¬w·¡ ¬é¸÷–A»¡ ´g´v”a. */
#define NOTSELECT (-1)     // Player is not Selected ( use Select Procedure )

/**
   Game Environment Structure
   ‰A·± ÑÅ‰w ¥e® Š¹¡
**/

typedef struct
{
  int t ; // match time  ÐeÌe”w ”¸å ¯¡ˆe
  int n,mn,tr ; // match number Ñe¸ ”¸åÒU®, Á· ”¸åÒU®
  unsigned char Bk    ; // Back Ground : RYU=0,HONDA=1, ...
  unsigned char loser[12] ; // ‰A·±µA¬á »¥ ÅAŸ¢Èá ¡w”e
} GameENV ;

#define MAXASEQ  2000  // maximum Actor Sequence ÂA” •·¸b ÓaŸq®
#define MAXACT   128   // maximum Action Size ÂA” •·¸b ®
#define MAXACTOR 400   // ÂA” •·¸b ®…ˆe ®

/**
   Action Structure •·¸b Š¹¡ 
   Action is time Sequence of Actor •·¸b·e Actor· ¯¡ˆe¸â ÓaŸq·a¡ ¸÷·
   Action is Animation •·¸b·e animation‰Á ˆ{”a.
**/
typedef struct
{
  unsigned char n[MAXACT]     ; // Ðe•·¸b· actor®
  int As[MAXASEQ],Ap[MAXACT]  ; // •·¸b ÓaŸq ¤áÌá, •·¸b ÓaŸq Í¡·¥Èá
} ACTION ;

/**
   Actor Structure   Actor Š¹¡
   Actor is transition state of Sprite or Scene
   Actor“e ¯aÏaœa·¡Ëaa ‹aŸ±· Ðe®…ˆe·i aÈa¡a, ˆb¹· ¸÷¥¡ˆa Í¡Ðq–E ¬wÈ
**/
typedef struct
{
   unsigned char sp ;  // sprite pointer  ¯aÏaœa·¡Ëa ¤åÑ¡
   char dx,dy ;        // relative move position of X,Y  ¬w” ¹ÁÎa ¶‘»¢·±
   unsigned int i ;    // Actor Information •·¸b¬wÈ 
} ACTOR ;

/*
  ¹¡Ðs ‰·‰b Š¹¡ ( ¸â‰Á a· Ñe¸ ¬wÈ· ¹¡ÐsµA ·Ðe ‰·‰b )
  
*/
typedef struct
{
   unsigned char lc,kc,ann,dann,mc,vfn[13] ;
} COMBRULE ;

/*
  ®…Àa ‰·‰b Š¹¡ ( Ç¡¥¡—aŸi ®…Àa¸â·a¡ ’‰œö·i˜ aµ¡“e ‰·‰b )
*/
typedef struct
{
   char s[10],vfn[13],code ;
}  KEYRULE ;

#define MAXCR 20  /* ÂA” ¹¡Ðs‰·‰b ˆ•® */
#define MAXKR 14  /* ÂA” ®…Àa‰·‰b ˆ•® */

/**
   Player Structure   ÏiA·¡´á Š¹¡
**/

typedef struct
{
   int ctrl,pl,end ; // Player state & control & player type ( RYU,KEN .. )
   int power,resis,energe,ve ;  // Player power & resistance & energe
   int x,y,ann,ani,ar,mirr ;    // Player Current Action #, Action index, Actor
   int x2,y2,ann2,ani2,ar2,mirr2 ;   // temporary action
   unsigned int i,i2 ;      // player Actor Information 1,2
   int nr,win,scr,chicken ;   // Player Near Distance
   char hit,san,can ;
   char far *key2asc ; // Player key state
   KEYRULE far *KR ;  // Spectial Attack Key Pattern
   KEYBUFF far *KB ;  // RingBuffer for Keyboard
   COMBRULE far *CR ; // Combination Attack RULE
   CTRLKEY far *K  ;  // Player Control Key
   ACTION  far *AN ;  // Player Action
   ACTOR   far *AR ;  // Player Actor
   Sprite  far *Spr ; // Player Sprite
   Image   far *Img ; // Player Image
} PLAYER ;

