#include "v256.h"
#include <alloc.h>
#include <stdlib.h>
#include <time.h>

BYTE *image;
BYTE *sound,*beam;
unsigned soundl,beaml;

void pause(void)
{
   delay(90);
}

void explosive(int x, int y)
{
// if(VoiceFlag!=1)
 sayVoice(sound,soundl);

 virtualscreen2(1,1,50,45,x,y,0,image);
 pause();
 virtualscreen2(52,1,101,45,x,y,0,image);
 pause();
 virtualscreen2(103,1,152,45,x,y,0,image);
 pause();
 virtualscreen2(154,1,203,45,x,y,0,image);
 pause();
 virtualscreen2(205,1,254,45,x,y,0,image);
 pause();
 virtualscreen2(256,1,305,45,x,y,0,image);
}

void main(int argc,char *argv[])
{
 int x,y,x1;
 BYTE *back,*space;

 if(argc<2){ printf("Taegu University Computer Science. '91 Jeong, Kwang Kuk.\nType explo 1 or explo 2\n1 = PC-Speaker, 2 = S-Blaster or S-Master\n"); exit(-1); }
 else if(*argv[1]=='1') SOUND_DEV=1;
 else if(*argv[1]=='2') SOUND_DEV=2;
 else { printf("Type explo 1 or explo 2\n"); exit(-1); }

 sound=readvocfile("explo.voc",&soundl);
 beam=readvocfile("beam.voc",&beaml);

 set256mode();
 readpcxfilebuf("computer.pcx",&space);
 displaypcxbuf(space,1);
 farfree(space);

 readpcxfilebuf("font.pcx",&image);
 randomize();
 imagesize256(1,1,50,45,&back);
 for(;;){
	if(kbhit()) break;
	x=random(270)+25; y=random(100)+22;
	x1=random(320);
	//if(VoiceFlag!=1)
sayVoice(beam,beaml);
	line256(x1,199,x,y,1,15);
	delay(100);
	line256(x1,199,x,y,1,15);
	getimage256(x-25,y-22,x-25+50,y-22+45,back);
	explosive(x-25,y-22);
        vertical_retrace();
	putimage256(x-25,y-22,0,back);
	}
 farfree(back);
 farfree(image);
 farfree(sound);
 farfree(beam);

 settextmode();
}


