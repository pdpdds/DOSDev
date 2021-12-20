/*
   ¯aÌa2 ¬a¶…—a,¥¡·¡¯a ¹A´á žË¥

   ´—aŸ³‰Á ¬a§i žË¥·e ­¡¯aŸi ´è´á¬á ¬s·³Ðe ˆõ ·³“¡”a.  
   
   ´—aŸ³ º—¶a žË¥‰Á ¬a§i ¹A´á žË¥·e ­¡¯aˆa ´ô·¡ libraryÑÁ·©‰Á 
  
   obj ÑÁ·© e ·¶·s“¡”a. 
*/

/* ´—aŸ³ ¤‰w·q´b Â‰b žË¥ */
#include "sf2adl.c"

/** ·q¬÷ Â‰b žË¥ **/
/***
   Motorola MC146818    CMOS Real Time Clock & RAM ¹A´á Ã³ MC146818

   index port Address 0x70  ·¥•B¯a Í¡Ëa ´á—aA¯a 
   data  port Address 0x71  •A·¡Èa Í¡Ëa ´á—aA¯a

   inportance data address º—¶aÐe •A·¡Èa ´á—aA¯a

   0xa Status Register A
       bit 7 : Update in process
               0 : read enable for time clock
               1 : updating time clock
       bit 6-4 : Stage Divider ( default Value 010 )
               000 : Timer frequency 4.194304 MHz
               001 : Timer frequency 1.048576 MHz
               010 : Timer frequency 32.768   KHz
               110 : Any
               111 : Any
       bit 3-0 : Rate Selection Bit
                      Timer freq 4.1 & 1.0 MHz   Timer freq 32.768 Khz
               0000 :          NONE                     NONE
               0001 :   32.768 KHz Interrupt       256 Hz  Interrupt
               0010 :   16.384 KHz Interrupt       128 Hz  Interrupt
               0011 :    8.192 KHz Interrupt      8192 Hz  Interrupt
               0100 :    4.096 KHz Interrupt      4096 Hz  Interrupt
               0101 :    2.048 KHz Interrupt      2048 Hz  Interrupt
               0110 :    1.024 KHz Interrupt      1024 Hz  Interrupt
               0111 :      512  Hz Interrupt       512 Hz  Interrupt
               1000 :      256  Hz Interrupt       256 Hz  Interrupt
               1001 :      128  Hz Interrupt       128 Hz  Interrupt
               1010 :       64  Hz Interrupt        64 Hz  Interrupt
               1011 :       32  Hz Interrupt        32 Hz  Interrupt
               1100 :       16  Hz Interrupt        16 Hz  Interrupt
               1101 :        8  Hz Interrupt         8 Hz  Interrupt
               1110 :        4  Hz Interrupt         4 Hz  Interrupt
               1111 :        2  Hz Interrupt         2 Hz  Interrupt

   0xb Status Register B
       bit 7 : timer data update enable
               0 : disable
               1 : enable
       bit 6 : periodic     interrupt enable ( interrupt 0x70 )
               0 : disable ( default value )
               1 : enable
       bit 5 : alram        interrupt enable ( interrupt 0x70 )
               0 : disable ( default value )
               1 : enable
       bit 4 : update ended interrupt enable ( interrupt 0x70 )
               0 : disable  ( default value )
               1 : enable
       bit 3 : Square Wave Enable
               0 : disable  ( default value )
               1 : enable
       bit 2 : Data numerical Base
               0 : BCD number   ( default )
               1 : binary number
       bit 1 : 24/12 select
               0 : 24 hour mode
               1 : 12 hour AM/PM mode
       bit 0 : Daylight Saving Enable
               0 : daylight saving disable ( default value )
               1 : daylight saving enable

   0xc Status Register C
   0xd Status Register D
***/
/* CMOS RAMµA •A·¡ÈaŸi ³e”a */
#define CMOS_WRITE(I,X) (outportb(0x70,(I)) , outportb(0x71,(X)))
/* CMOS RAMµA¬á •A·¡ÈaŸi ·ª“e”a */
#define CMOS_READ(I)    (outportb(0x70,(I)) , inportb(0x71))

#define CMOSINTVECT 0x70 /* ¯©¯¡ˆe Çiœâ ·¥ÈáœóËa ¥BÈá */
void interrupt (*oldRTC)(void) ; /* ¶¥œ· ¯©¯¡ˆe Çiœâ ·¥ÈáœóËa ¥BÈá */

#define BUSY 1 /* ·q¬÷ Â‰b º— */
#define IDLE 0 /* ·q¬÷ Â‰b·i ´eÐq */
#define MAX_VOICE_SIZE 20000 /* ·q¬÷ ¸aža· ÂA” Ça‹¡ */

unsigned char *VoiceData,VoiceFlag=IDLE ; /* ·q¬÷ •A·¡Èa Í¡·¥Èá , ·q¬÷ ¬wÈ ÏiA‹a */
int VoiceDataSize,VoiceDataBytePoint ; /* ·q¬÷•A·¡Èa Ça‹¡, Ñe¸·q¬÷ ¸aža ¤åÑ¡*/

#define SPEAKER_ON  (outp(97,(inp(97)|0x2)))  /* ¯aÏ¡Äá Åe”a */
#define SPEAKER_OFF (outp(97,(inp(97)&0xfc))) /* ¯aÏ¡Äá e”a */
#define FREQ_IN(X) (outp(67,182),outp(66,((X)&0xff)),outp(66,((X)>>8)))

extern int SOUND_DEV ; /* ¬a¶…—a Äa—a ¹·ŸA */

#define NO_VOICE      0 /* ·q¬÷ Â‰b·i Ða»¡ ´g“e”a */
#define PC_SPEAKER    1 /* Ï¡³¡ ¯aÏ¡Äá¡ ·q¬÷ Â‰b */
#define SOUND_BLASTER 2 /* ¬a¶…—a §iœ¯aÈá¡ ·q¬÷ Â‰b */
#define COVOX1        3 /* Å¡¥¢¯aˆa ÏaŸ¥Èá Í¡Ëa 1µA µe‰i */
#define COVOX2        4 /* Å¡¥¢¯aˆa ÏaŸ¥Èá Í¡Ëa 2µA µe‰i */

#define COLOR_LPT1_PORT 0x378 /* Äéœá ´á”“Èá ÏaŸ¥Èá Í¡Ëa 1 */
#define COLOR_LPT2_PORT 0x278 /* Äéœá ´á”“Èá ÏaŸ¥Èá Í¡Ëa 2 */
#define MONO_LPT1_PORT  0x3bc /* ÐáÇIA¯a ´á”“Èá ÏaŸ¥Èá Í¡Ëa 1 */

void interrupt onePulsGen(void)
{
   unsigned char data ;

   disable() ;

   data = VoiceData[VoiceDataBytePoint] ;
   switch(SOUND_DEV)
   {
     case PC_SPEAKER : if (data>127) SPEAKER_ON  ;
                              else   SPEAKER_OFF ;
                          break ;
     case SOUND_BLASTER : write_data(data) ; break ;
     case COVOX1        : outportb(COLOR_LPT1_PORT,data) ; break ;
     case COVOX2        : outportb(COLOR_LPT2_PORT,data) ; break ;
   }

   VoiceDataBytePoint++ ;

   if (VoiceDataBytePoint>=VoiceDataSize) silentVoice() ;

   CMOS_READ(0x0c) ; // reactive interrupt
   outportb(0xa0,0x20) ; outportb(0x20,0x20) ; // send EOI to 8259

   enable() ;
}

/* ·q¬÷ mdi ÑÁ·© fn· ­¡Ÿ¡Ÿi …”a */
sayVoice(fn)
unsigned char fn[] ;
{
   FILE *fp ;

   if (SOUND_DEV==NO_VOICE) return(0) ;

   fp=fopen(fn,"rb") ; if (fp==NULL) return(0) ;
   if (VoiceFlag==BUSY) silentVoice() ;
   VoiceDataSize=filelength(fileno(fp)) ;
   if (VoiceDataSize>MAX_VOICE_SIZE) VoiceDataSize=MAX_VOICE_SIZE ;
   fread(VoiceData,1,VoiceDataSize,fp) ; fclose(fp) ;

   VoiceDataBytePoint=0 ;

   if (SOUND_DEV==SOUND_BLASTER) speaker_on() ;
   oldRTC = getvect(CMOSINTVECT) ;
   setvect(CMOSINTVECT, onePulsGen) ;
   outportb(0xa1,inportb(0xa1) & 0xfe) ;  // 2nd 8259A's IMR reset for RTC
   CMOS_WRITE(0xa,0xa3) ; // Set timer interrupt 8.192 KHz
   CMOS_WRITE(0xb,0x42) ; // enable timer interrupt
   VoiceFlag=BUSY ;
}

/* ·q¬÷ ÑÁ·© •A·¡Èa ¡A¡¡Ÿ¡ Ði”w */
allocVoiceMem()
{
   if ((VoiceData=malloc(MAX_VOICE_SIZE))==NULL) SOUND_DEV=NO_VOICE ;
}

/* ·q¬÷·i a‰¡, ¶¥œ· ·¥ÈáœóËa žË¥·a¡ ¥¢Šá */
silentVoice()
{
   if (VoiceFlag==IDLE||SOUND_DEV==NO_VOICE) return(0) ;
   CMOS_WRITE(0xb,0x2) ; //disable RTC periodic int.
   CMOS_WRITE(0xa,0xa6) ; // restore timer frequency 1.024 KHz
   outportb(0xa1,inportb(0xa1) | 1) ;  // set mask for RTC
   setvect(CMOSINTVECT,oldRTC) ;
   if (SOUND_DEV==SOUND_BLASTER) speaker_off() ;
   VoiceFlag=IDLE ;
}
