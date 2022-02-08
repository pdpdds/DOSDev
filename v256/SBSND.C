#include <stdio.h>
#include <dos.h>
#include <io.h>
#include <alloc.h>
#include <stdlib.h>
#include <fcntl.h>
#include <conio.h>

#define CMOS_WRITE(I,X) (outportb(0x70,(I)),outportb(0x71,(X)))
#define CMOS_READ(I) (outportb(0x70,(I)),inportb(0x71))
#define CMOSINTVECT 0x70

void interrupt (*oldRTC)(void);
unsigned char *VoiceData,VoiceFlag=0;
unsigned VoiceDataSize,VoiceDataBytePoint;
void silentVoice(void);

int SOUND_DEV;

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
   disable();
   if(SOUND_DEV==PC_SPEAKER) if(VoiceData[VoiceDataBytePoint++]>127) outportb(97,(inportb(97) | 0x2));
			     else outportb(97,(inportb(97) & 0xfc));
   if(SOUND_DEV==SOUND_BLASTER) write_data(VoiceData[VoiceDataBytePoint++]);
   if(SOUND_DEV==COVOX1) outportb(COLOR_LPT1_PORT,VoiceData[VoiceDataBytePoint++]);
   if(SOUND_DEV==COVOX2) outportb(COLOR_LPT2_PORT,VoiceData[VoiceDataBytePoint++]);

   if(VoiceDataBytePoint>=VoiceDataSize) silentVoice() ;

   CMOS_READ(0x0c);
   outportb(0xa0,0x20);
   outportb(0x20,0x20);
   enable();
}

int sayVoice(unsigned char far *voc,unsigned length)
{
   if(SOUND_DEV==0) return -1;
   if(VoiceFlag) silentVoice() ;

   VoiceDataSize=length;
   VoiceData=voc;
   VoiceDataBytePoint=0 ;
   if(SOUND_DEV==2) speaker_on();

   oldRTC = getvect(CMOSINTVECT);
   setvect(CMOSINTVECT, onePulsGen);

   outportb(0xa1,inportb(0xa1) & 0xfe);
   CMOS_WRITE(0xa,0xa3);
   CMOS_WRITE(0xb,0x42);
   VoiceFlag=1;
   return 0;
}

void silentVoice(void)
{
   CMOS_WRITE(0xb,0x2);
   CMOS_WRITE(0xa,0xa6);
   outportb(0xa1,inportb(0xa1) | 1);

   setvect(CMOSINTVECT,oldRTC);
   if(SOUND_DEV==2) speaker_off();
   VoiceFlag=0;
}

unsigned char far *readvocfile(char *vocname,unsigned *length)
{
   FILE *fp;
   unsigned size;
   char far *block,*buffer;

   fp=fopen(vocname,"rb");
   fseek(fp,0L,SEEK_END);
   size=ftell(fp)-0x1aL;
   *length=size;
   buffer=block=farmalloc(size);
   if(buffer==NULL){ setvmode(0x03); printf("Not enough memory!\n"); exit(-1); }
   fseek(fp,0x1aL,SEEK_SET);
   while(size--) *block++=(char)getc(fp);
   return(void far *)buffer;
}

