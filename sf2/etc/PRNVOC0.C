/*******************************************************
   Å¡¥¢¯a ·q¬÷ Â‰b Ïa¡‹aœ‘ µ¹A

   Borland C++ 2.0
********************************************************/

#include <dos.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>

#define COLOR_LPT1 0x378  /* Å¡¥¢¯aˆa ÏaŸ¥Èá Í¡Ëa 1 µA µe‰i */
#define COLOR_LPT2 0x278  /* Å¡¥¢¯aˆa ÏaŸ¥Èá Í¡Ëa 2 µA µe‰i */
#define MONO_LPT1  0x3bc  /* Å¡¥¢¯aˆa ÏaŸ¥Èá Í¡Ëa 1 µA µe‰i (ÐáÇIA¯aÄa—a) */

/** Èa·¡ á Äa¶…Èá 0 µA ˆt·i °áý“e”a **/
#define Timer0_SET(X) (outp(0x43,0x34),outp(0x40,((X)&0xff)),outp(0x40,((X)>>8)))

#define BUSY 1  /** ·q¬÷·¡ aµ¡“e º—·¡”a. **/
#define IDLE 0  /** ·q¬÷·¡ Â‰b·¡ {v”a. **/

unsigned char *VoiceData,VoiceFlag=IDLE ;
unsigned int VoiceDataSize,VoiceDataBytePoint,TimerTick ;
int silentVoice(void);
int sayVoice(unsigned char fn[]);

#define TIMERINTVECT0 0x8  /* Èa·¡ á ·¥ÈáœóËa ¥BÈá **/
void interrupt (*TimerInt0x8)(void) ; /* Èa·¡ á ·¥Èé¤áËa Ðq® Í¡·¥Èá */

void interrupt onePulsGen(void) /* º‹¡¸â·a¡ ÐeÌé¯a “e žË¥ */
{
   disable() ; /* ·¥ÈáœóËa ‹q»¡ */

   outportb(COLOR_LPT1,VoiceData[VoiceDataBytePoint]) ;

   VoiceDataBytePoint++ ; /* ·q¬÷ ÉA·¡Èa Í¡·¥ÈáŸi Ðaa »wˆa */

   if (TimerTick==0) (*TimerInt0x8)() ; /* ¯¡‰Ÿi »wˆa¯¡Ç¡‹¡¶áÐ 
                                           ¶¥œ· ·¥ÈáœóËažË¥ ®Ð— */ 
   /* Èa·¡ á º‹¡ˆa ¶¥œ¥¡”a 512¤ ¨iœa¹v·a£a¡ 512¤å  a”a Ðe¤å³¢ ®Ð— */    
   TimerTick++ ; if (TimerTick>511) TimerTick=0 ;

   /* ·q¬÷ ¸ažaˆa ”a {v·a¡e ·q¬÷ Â‰b ¸÷»¡ žË¥·i ¦Ÿe”a. */
   if (VoiceDataBytePoint>=VoiceDataSize) silentVoice() ;
   outp(0x20,0x20) ; /* ·¥ÈáœóËaˆa {v·q·i ´iaº…”a. */
   enable() ; /* ·¥ÈáœóËa ˆa“w */
}

/* ·q¬÷ÑÁ·© fn·i Â‰bÐa“e žË¥ */
int sayVoice(unsigned char fn[])
{
   FILE *fp ;

  
   if (VoiceFlag!=IDLE) return(0) ; /* ·q¬÷·¡ a‰¡·¶“eº—·¡¡e ¯©Ð—´eÐq */

   /* ·q¬÷ÑÁ·©·i µi‰¡, ¡A¡¡Ÿ¡ Ði”w¤h‰¡, ·ª´áµ¥”a */
   fp=fopen(fn,"rb") ; if (fp==NULL) return(0) ;
   VoiceDataSize=filelength(fileno(fp)) ;
   VoiceData=malloc(VoiceDataSize) ;
   if (VoiceData==NULL) { fclose(fp) ; return(0) ; }
   fread(VoiceData,1,VoiceDataSize,fp) ; fclose(fp) ;

   VoiceDataBytePoint=0 ; /* ·q¬÷•A·¡Èa Í¡·¥ÈáŸi Àá·q·a¡ Ði”w */

   disable() ; /* ·¥ÈáœóËa ‹q»¡ */
   Timer0_SET(0x80) ; /* Èa·¡ á ·¥ÈáœóËaŸi 9.318Khz¡ ­AË· */
   TimerInt0x8 = getvect(TIMERINTVECT0) ; /* ¶¥œ· ·¥ÈáœóËa žË¥·i ´è´áµ± */
   setvect(TIMERINTVECT0,onePulsGen) ; /* ¬¡¶… ·¥ÈáœóËa žË¥·a¡ Ði”w */
   enable() ; /* ·¥ÈáœóËa ˆa“w */
   TimerTick=0 ; /* ¯¡‰ ¥e® */
   VoiceFlag=BUSY ; /* ·q¬÷ Â‰bº— */
}

int silentVoice(void)
{
   if (VoiceFlag==IDLE) return(0) ; /* ·q¬÷·¡ µeºº—·¡ ´a“¡¡e {‘ */
   disable() ;
   Timer0_SET(0x00) ;  /* Èa·¡ á º‹¡Ÿi ¶¥œ”¡ 18.2Hz¡  e—e”a. */
   setvect(TIMERINTVECT0,TimerInt0x8) ; /* Èa·¡ á ·¥ÈáœóËaŸi ¶¥œ”¡  e—q */
   enable() ;
   /* ·q¬÷µA Ði”w–E ¡A¡¡Ÿ¡ ¤eÑÅ */
   if (VoiceData!=NULL) { free(VoiceData) ; VoiceData=NULL ; }
   VoiceFlag=IDLE ; /* ·q¬÷ Â‰b·i ´eÐe”a. */
}

void main(argc,argv)
int argc ;
char **argv ;
{
   sayVoice(argv[1]) ;
   for ( ; VoiceFlag==BUSY ; ) ;
}



