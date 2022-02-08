/****
   STREET FIGHTER ][ KEYBOARD INPUT ROUTINE
   Borland C++ 2.0 Version Created 1992.8.23  By Jung Y.D.

   ¯aÌa2¶w Ç¡¥¡—a ·³b žË¥ 
****/
#ifndef __SF2KBD0_HEADER__
#define __SF2KBD0_HEADER__

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


#endif

