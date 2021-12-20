/**
  ‹aŸ±•A·¡Èá Š¹¡
**/
typedef struct 
{
   int xs,ys,ls ; /* ‹aŸ±· ˆa¡,­A¡Ça‹¡ . ´sÂ‚–E‰w¶ ´sÂ‚–E Ça‹¡ */
   unsigned char far *p ; /* ‹aŸ± •A·¡Èa Í¡·¥Èá */
} Image ;

/*
   ¯aÏaœa·¡Ëa Š¹¡
*/
typedef struct
{
   int xo[2],yo[2],i[2],n,xs,ys,xfs ; /* ‹aŸ±·i ‹aŸ© x,y ¶¥¸ñ, ˆ•® 
                                         ¯aÏaœa·¡Ëa x,y ¬a·¡»a */
   char xh1,xh2,yh1,yh2 ; /* ‰·‰b ¤ñ¶á ¬å */
} Sprite ;
