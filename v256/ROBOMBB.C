#include "v256.h"
#include <alloc.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INTR 0x1C

extern unsigned _stklen = 10000U;
extern int SOUND_DEV,hancolor;
extern silentVoice(void);
extern BYTE VoiceFlag,R_[256],G_[256],B_[256];
extern BYTE far *readvocfile(char *,unsigned *);
extern int far RIGHT,LEFT,DOWN,UP,SPACE,ESC;

//stage1 variable initialize
BYTE *title,*explo,*beam,*font,*etemp,*htemp,*st_im;
unsigned explol,beaml;
int HX=3,HY=106,HMODE,GUN,EX=286,HLIFE=2,ELIFE=10,INED=1;

void interrupt (*oldhandler)(void);
void interrupt logo_handler(void);

void interrupt (*oldhandle)(void);
void interrupt move_camera(void);

void h_explosive(int,int);
void e_explosive(BYTE *,unsigned);

void hero_move(int,BYTE *);

void stage1_init(BYTE *,unsigned,int *);
void stage1(void);

void line_fad_out(int,int,int,int);
void left_right_sac(BYTE *);

void main(int argc,char *argv[])
{
  extern int rgb_check;
  unsigned logosl,edy=5,esongl;
  BYTE *logos,*enemy,*esong,
       *message="Taegu University Computer Science. '91 Jeong, Kwang Kuk.\n"
		"Type robomb 0 or robomb 1 or robomb 2\n"
		"0 = No-sound, 1 = PC-Speaker, 2 = S-Blaster or S-Master\n";

//-------------- selection : pc_speaker or sound_card
  if(argc<2){ printf("%s",message); exit(-1); }
  else if(*argv[1]=='0') SOUND_DEV=0;
  else if(*argv[1]=='1') SOUND_DEV=1;
  else if(*argv[1]=='2') SOUND_DEV=2;
  else { printf("%s",message); exit(-1); }

  inithan("hangulg.fnt","ascii0.fnt");
  set256mode();
  setkeyint();

//-------------- title display routine
  readpcxfilebuf("title.pcx",&title);
  logos=readvocfile("logo.voc",&logosl);
  brightCtrl(0,256,R_,G_,B_,0);
  virtualscreen2(0,0,319,76,0,21,1,title);
  Fade_In();
  sayVoice(logos,logosl);
  if(SOUND_DEV==0) for(;;) if(SPACE) break;
  else for(;;) if(!VoiceFlag) break;
  Fade_Out();
  farfree(logos);
//-------------- game story display routine
  clearscreen(0);
  brightCtrl(0,256,R_,G_,B_,100);
  virtualscreen2(1,79,100,168,0,0,1,title);
  virtualscreen2(102,101,318,168,0,115,1,title);
  randomize();

//------------- set interrupt num 8
  oldhandler = getvect(INTR);
  setvect(INTR, logo_handler);

  hancolor=15;
  hanxy(110,0,   1,"˜“e 2017‘e ¡¥¡ËaÍ¢Èe·");
  hanxy(110,16*1,1,"‰·‰b·a¡ Ðe •¡¯¡ˆa ‹a—i·");
  hanxy(110,16*2,1,"®º—µA ·¶·a“¡... ¹Aˆa ‹a");
  hanxy(110,16*3,1,"•¡¯¡µA¬á ‹a—i·i ¡©´a´¡");
  hanxy(110,16*4,1,"Ðs“¡”a. ¹A¤i ¸áŸi •¡µÁ¬á");
  hanxy(110,16*5,1,"‹a—i·i ¡©´a“¯¡”a!");
  delay(100);
//------------- reset interrupt num 8
  setvect(INTR, oldhandler);
  Fade_Out();

//-------------- the end title & logo routine
  clearscreen(0);
  brightCtrl(0,256,R_,G_,B_,100);

//-------------- enemy display routine
  rgb_check=0;
  readpcxfilebuf("enemy.pcx",&enemy);
  esong=readvocfile("enemy.voc",&esongl);

  left_right_sac(enemy);
  imagesize256(102,101,318,168,&etemp);
  for(;;){ getimagevirtual(55,edy,55+216,edy+67,etemp,enemy);
	   composition(102,101,318,168,title,55,edy,enemy,0);
	   virtualscreen2(55,edy,55+216,edy+66,55,edy,1,enemy);
	   getcomposition(etemp,55,edy,enemy,1);
           if(kbhit()) break;
	   if(edy++ >=127){ sayVoice(esong,esongl); break; } }
  line_fad_out(0,0,319,199);

  farfree(title); farfree(enemy); farfree(etemp);
  for(;;) if(!VoiceFlag){ farfree(enemy); break;}

//-------------- start game routine
  rgb_check=0;
  readpcxfilebuf("font.pcx",&font);

  explo=readvocfile("explo.voc",&explol);
  beam=readvocfile("beam.voc",&beaml);
  imagesize256(63,47,92,76,&htemp);

//start stage1
  readpcxfilebuf("stage1.pcx",&st_im);
  imagesize256(63,47,92,76,&etemp);
  stage1();
  farfree(st_im);
  farfree(etemp);
//the end stage1

  resetkeyint();
  clearscreen(0);
  hanxy(0,0,1,"ºA­·Ðs“¡”a. ´a»¢·e STAGE-1  e·¡ ˆa“w!");
  hanxy(0,16,1,"‹aœñ ´e‘wÓ¡...");
  hanend(); sleep(2);
//-------------- the end game routine
  farfree(beam); farfree(explo);
  farfree(htemp); farfree(font);
  settextmode();
}

//-------------- robot mouth multitask routine
void interrupt logo_handler(void)
{
   BYTE *temp;
   static int toggle=0,key=1,x,y,xl;

   disable();
   //mouth proceeding routine
   switch(toggle++){
	  case 0: virtualscreen2(102,90,139,99,12,52,1,title); break;
	  case 1: //city & bomb proceeding routine
		  xl=random(320);
		  x=random(181); y=random(39)+115;
		  imagesize256(1,170,34,198,&temp);
		  getimage256(x,y,x+33,y+28,temp);
		  line256(xl,199,x+16,y+14,1,15);
		  delay(30);
		  line256(xl,199,x+16,y+14,1,15);
		  virtualscreen2(1,170,34,198,x,y,0,title); break;
	  case 2: virtualscreen2(36,170,69,198,x,y,0,title); break;
	  case 3: virtualscreen2(102,79,139,88,12,52,1,title); break;
	  case 4: virtualscreen2(71,170,104,198,x,y,0,title); break;
	  case 5: virtualscreen2(106,170,139,198,x,y,0,title); break;
	  case 6: virtualscreen2(141,170,174,198,x,y,0,title);
		  putimage256(x,y,0,temp);
		  farfree(temp); toggle=0;}
  if(SPACE) key=0;
  if(key) delay(80);
  enable();
  (*oldhandler)();
}

void line_fad_out(xx1,yy1,xx2,yy2)
int xx1,yy1,xx2,yy2;
{
  for(;;){ box256(xx1,yy1,xx2,yy2,0,0); xx1++; yy1++; xx2--; yy2--;
	   if(yy1>99) break; }
}

void left_right_sac(BYTE *image)
{
  int xx1=159,xx2=159;
  for(;;){ virtualscreen2(xx1,0,xx1,199,xx1,0,1,image);
	   virtualscreen2(xx2,0,xx2,199,xx2,0,1,image);
	   xx1--; xx2++; if(xx1<=-1) break; }
}

//-------------- multi enemy proceeding routine
void interrupt move_camera(void)
{
 static int run=0;

 disable();
 if(INED){
 switch(run++){
	     case 0://LED off
		    fill_box(230,28,235,30,0); break;
	     case 1:virtualscreen2(42,176,84,198,20,15,1,st_im); break;
	     case 2:virtualscreen2(86,176,128,198,20,15,1,st_im); break;
	     case 3:virtualscreen2(130,176,172,198,20,15,1,st_im); break;
	     case 4:virtualscreen2(174,176,216,198,20,15,1,st_im); break;
	     case 5:virtualscreen2(218,176,260,198,20,15,1,st_im); break;
	     case 6:virtualscreen2(262,176,282,198,272,149,1,st_im); break;
	     case 7:virtualscreen2(284,176,304,198,272,149,1,st_im); break;
	     case 8:virtualscreen2(272,149,272+20,149+22,272,149,1,st_im); break;
	     case 9://LED on
		    virtualscreen2(230,28,235,30,230,28,1,st_im);
		    run=0;
		}
 if(ELIFE>0){ getimagevirtual(EX,106,EX+29,106+29,etemp,st_im);
	      composition(249,47,278,76,font,EX,106,st_im,0);
	      virtualscreen2(EX,106,EX+30,106+29,EX,106,1,st_im);
	      getcomposition(etemp,EX,106,st_im,1);
	      if(EX--<=2){virtualscreen2(EX,106,EX+30,106+29,EX,106,1,st_im);
	      EX=286;}}}
 enable();
 (*oldhandle)();
}

//-------------- hero bomb explosive routine
void h_explosive(int epx,int epy)
{
 sayVoice(explo,explol);
 virtualscreen2(103,1,152,45,epx,epy,0,font);
 delay(50);
 virtualscreen2(154,1,203,45,epx,epy,0,font);
 delay(50);
 virtualscreen2(205,1,254,45,epx,epy,0,font);
 delay(50);
 virtualscreen2(256,1,305,45,epx,epy,0,font);
}

void e_explosive(BYTE *get,unsigned getl)
{
 sayVoice(get,getl);
 virtualscreen2(1,1,30,30,EX,HY,0,font);
 delay(70);
 virtualscreen2(32,1,61,30,EX,HY,0,font);
 delay(70);
 virtualscreen2(EX,HY,EX+29,HY+29,EX,HY,1,st_im);
 virtualscreen2(249,47,278,76,EX,HY,0,font);
}

void hero_move(int move_form,BYTE *stagei)
{
 int tx=0,ty=0,lx=0,rx=0,uy=0,dy=0,thmode=0;

    //right key check routine
    if(RIGHT && !LEFT && !DOWN && !UP){ HX++; HMODE=2; lx=1; rx=0; if(HX>=290+1){ HX=290; lx=rx=0;}}

    //left key check routine
    if(LEFT && !RIGHT && !DOWN && !UP){ HX--; HMODE=3; lx=0; rx=1; if(HX<=0-1){ HX=0; lx=rx=0;}}

    if(move_form){ if(DOWN) {HY++; HMODE=0; uy=1; dy=0; if(HY>=170+1){ HY=170; uy=dy=0;}}
		   if(UP) {HY--; HMODE=1; uy=0; dy=1; if(HY<=0-1){ HY=0; uy=dy=0;}}}
    else{ if(DOWN && !RIGHT && !LEFT && !UP) HMODE=0;
	  if(UP && !RIGHT && !LEFT && !DOWN) HMODE=1; }

    //hero font proceeding routine
    if(HX!=tx || HY!=ty || HMODE!=thmode){
       getimagevirtual(HX,HY,HX+29,HY+29,htemp,stagei);
       switch(HMODE){ case 0: composition(1,47,30,76,font,HX,HY,stagei,0); break;
		      case 1: composition(32,47,61,76,font,HX,HY,stagei,0); break;
		      case 2: composition(63,47,92,76,font,HX,HY,stagei,0); break;
		      case 3: composition(94,47,123,76,font,HX,HY,stagei,0);
		    }
       virtualscreen2(HX-lx,HY-uy,HX+29+rx,HY+29+dy,HX-lx,HY-uy,1,stagei);
       getcomposition(htemp,HX,HY,stagei,1);
				     }
 tx=HX; ty=HY; thmode=HMODE;
}

void stage1_init(BYTE *door,unsigned doorl,int *exit)
{
 HX=3; HY=106; HMODE=0; GUN=0; EX=286; ELIFE=10; *exit=0;
 INED=0;

 line_fad_out(0,0,319,174);
 virtualscreen2(0,175,319,199,0,175,1,font);
//in open door routine
 virtualscreen2(0,0,319,174,0,0,1,st_im);
 switch(HLIFE){
	case 2: virtualscreen2(311,61,318,66,136,187,1,font); break;
	case 1: virtualscreen2(311,54,318,59,136,187,1,font); break;
	case 0: virtualscreen2(311,47,318,52,136,187,1,font);
	      }
 sayVoice(door,doorl);
 fill_box(2,103,33,134,0); fill_box(285,103,317,134,0);
 delay(300);
 virtualscreen2(2,103,33,134,2,103,1,st_im);
 virtualscreen2(1,47,30,76,HX,HY,0,font);
 virtualscreen2(285,103,317,134,285,103,1,st_im);
 virtualscreen2(156,47,185,76,EX,HY,0,font);
 delay(100);
 INED=1;
}

//-------------- game stage1 routine
void stage1(void)
{
 BYTE *door,*get;
 int sp=1,beam_count=0,out_door=0,exit=0;
 unsigned doorl,getl;

//load sound effect file
 door=readvocfile("door.voc",&doorl);
 get=readvocfile("get.voc",&getl);

 stage1_init(door,doorl,&exit);

 //set multi enemy proceeding
 oldhandle = getvect(INTR);
 setvect(INTR, move_camera);

 do{hero_move(0,st_im);
    //get gun routine
    if(!GUN){ virtualscreen2(307,1,318,22,156,114,0,font);
	      if(HX+29>=156){ virtualscreen2(157,114,167,135,157,114,1,st_im);
			      sayVoice(get,getl);
			      virtualscreen2(307,1,318,22,11,176,0,font);
			      fill_box(136,179,315,181,4);
			      beam_count=315; GUN=1;
			     }
	    }
    //gun proceeding routine
    if(SPACE && HMODE!=0){ if(sp && GUN && beam_count>=136){ if(HMODE!=0) sayVoice(beam,beaml);
		  line256(beam_count,179,beam_count,181,0,21);
		  line256(beam_count-1,179,beam_count-1,181,0,21);
		  beam_count-=2;
		  if(beam_count<136){ virtualscreen2(11,176,22,197,11,176,1,font);
				      sayVoice(get,getl);
				    }
		  switch(HMODE){ case 1: line256(HX+24,HY+7,HX+24,39,1,15);
					 delay(50);
					 line256(HX+24,HY+7,HX+24,39,1,15);
					 if(HX+24>=158 && HX+24<=166 && !out_door){
					    brightCtrl(0,256,R_,G_,B_,70); INED=0;
					    virtualscreen2(EX,HY,EX+30,HY+29,EX,HY,1,st_im);
					    EX=286; ELIFE=10; INED=1;
					    sayVoice(door,doorl);
					    delay(500);
					    brightCtrl(0,256,R_,G_,B_,100);       }
					 if(HX+24>=230 && HX+24<=235 && !out_door){
					    h_explosive(HX,39);
					    virtualscreen2(HX,39,EX+30,39+45,HX,39,1,st_im);
					    if(exit++ >=4){ virtualscreen2(1,176,40,183,214,56,1,st_im);
							    ELIFE=0; INED=0;
							    virtualscreen2(EX,HY,EX+30,HY+29,EX,HY,1,st_im);
							    EX=319; ELIFE=0; out_door=1;}
										   }
					 break;
				 case 2: line256(HX+18,HY+14,EX,HY+14,1,15);
					 delay(50);
					 line256(HX+18,HY+14,EX,HY+14,1,15);
					 if(!out_door){ e_explosive(get,getl);
					 if(ELIFE-- <=1){ sayVoice(explo,explol);
							  virtualscreen2(EX,HY,EX+30,HY+29,EX,HY,1,st_im);
							  virtualscreen2(280,47,309,76,EX,HY,0,font);
							  delay(300);
							  virtualscreen2(EX,HY,EX+30,HY+29,EX,HY,1,st_im);
							  EX=286; ELIFE=10;
							  sayVoice(door,doorl);
							 }
						       } break;
				 case 3: line256(HX+11,HY+14,0,HY+14,1,15);
					 delay(50);
					 line256(HX+11,HY+14,0,HY+14,1,15);
			       }
					       }
	     } if(SPACE) sp=0; else sp=1;
    //hero & enemy match proceeding routine
    if(ELIFE>0 && HX+29>=EX){  INED=0; h_explosive(HX,HY);
			       virtualscreen2(HX,HY,EX+30,HY+45,HX,HY,1,st_im);
			       virtualscreen2(125,47,154,76,HX,HY,0,font);
			       virtualscreen2(280,47,309,76,EX,HY,0,font);
			       delay(500); if(HLIFE-- ==0) break; stage1_init(door,doorl,&exit);
			     }
    //out open door routine
    if(HX>=286 && UP && out_door){ sayVoice(door,doorl);
				   virtualscreen2(286,103,319,136,286,103,1,st_im);
				   virtualscreen2(32,47,61,76,287,106,0,font);
				   delay(500); fill_box(286,103,317,134,0);
				   delay(500); virtualscreen2(286,103,317,134,286,103,1,st_im);
				   delay(500); break;
				  }
   }while(ESC!=1);

   setvect(INTR, oldhandle);
   line_fad_out(0,0,319,199);
   if(VoiceFlag) silentVoice();
   farfree(get); farfree(door);
}



