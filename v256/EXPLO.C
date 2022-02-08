#include "v256.h"
#include <alloc.h>
#include <stdlib.h>
#include <time.h>

BYTE *image;
BYTE *sound,*beam;
unsigned soundl,beaml;

void pause(void)
{
 delay(70);
// getch();
}

void explosive(int x, int y)
{
// if(VoiceFlag!=1)
 sayVoice(sound,soundl);

 virtualscreen2(1,1,58,58,x,y,0,image);
 pause();
 virtualscreen2(60,1,117,58,x,y,0,image);
 pause();
 virtualscreen2(119,1,176,58,x,y,0,image);
 pause();
 virtualscreen2(178,1,235,58,x,y,0,image);
 pause();
 virtualscreen2(237,1,294,58,x,y,0,image);

 pause();
 virtualscreen2(1,60,58,117,x,y,0,image);
 pause();
 virtualscreen2(60,60,117,117,x,y,0,image);
 pause();
 virtualscreen2(119,60,176,117,x,y,0,image);
 pause();
 virtualscreen2(178,60,235,117,x,y,0,image);
 pause();
 virtualscreen2(237,60,294,117,x,y,0,image);
}

void main(int argc,char *argv[])
{
 int x,y,x1;
 BYTE *back,*space;

 if(argc<2){ printf("Taegu University Computer Science. '91 Jeong, Kwang Kuk.\nType explo 1 or explo 2\n1 = PC-Speaker, 2 = S-Blaster or S-Master\n"); exit(-1); }
 else if(*argv[1]=='1') SOUND_DEV=1;
 else if(*argv[1]=='2') SOUND_DEV=2;
 else { printf("Type explo 1 or explo 2\n"); exit(-1); }

 inithan("hangul0.fnt","ascii0.fnt");
 xsi__ze__=2; ysi__ze__=1;
 hancolor=32;

 sound=readvocfile("explo.voc",&soundl);
 beam=readvocfile("beam.voc",&beaml);

 set256mode();
// loadrgb("v256.rgb");
 readpcxfilebuf("space.pcx",&space);
 displaypcxbuf(space,1);
 farfree(space);
 hanxy(35,70,1,"TNT101: Í¢¤i¸w¡e");
 hanend();

 readpcxfilebuf("explo.pcx",&image);
 randomize();
// imagesize256(x-29,y-29,x-29+58,y-29+58,&back);
 imagesize256(1,1,58,58,&back);
 for(;;){
	if(kbhit()) break;
	x=random(261)+29; y=random(100)+29;
	x1=random(320);
	//if(VoiceFlag!=1)
sayVoice(beam,beaml);
	line256(x1,199,x,y,1,10);
	delay(100);
	line256(x1,199,x,y,1,10);
	getimage256(x-29,y-29,x-29+58,y-29+58,back);
	explosive(x-29,y-29);
	putimage256(x-29,y-29,0,back);
	}
 farfree(back);
 farfree(image);
 farfree(sound);
 farfree(beam);

 settextmode();
}


