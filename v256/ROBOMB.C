#include "v256.h"
#include <alloc.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INTR 0x1C

extern unsigned _stklen = 10000U;

//stage1 variable initialize
BYTE *title,*explo,*beam,*font,*etemp,*htemp,*st_im,*door,*get;
unsigned explol,beaml,doorl,getl;
int HX=3,HY=106,HMODE,GUN,JUM,EX=286,HLIFE=2,ELIFE=10,INED0=1,INED1=1,INED2=1,
    INED3=1,BEAM_COUNT1,PASS_STAGE;

void init_gv(void);
void interrupt (*oldhandler)(void);

void interrupt logo_handler(void);
void interrupt move_camera(void);
void interrupt stage2_task(void);

void h_explosive(int,int);
void e_explosive(void);

void hero_move(int,BYTE *);
void life_display(void);
void beam_display(void);

void stage1_init(int *);
void stage2_init(void);
int stage1(void);
int stage2(void);

void line_fade_out(int,int,int,int);
void left_right_sac(BYTE *);

void main(int argc,char *argv[])
{
  unsigned logosl;
  BYTE *logos,*message="Taegu University Computer Science. '91 Jeong, Kwang Kuk.\n"
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
  if(SOUND_DEV >0) for(;;) if(!VoiceFlag) break;
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
  imagesize256(1,170,34,198,&htemp);

  hancolor=15;
  hanxy(110,0,   1,"˜“e 2017‘e ¡¥¡ËaÍ¢Èe·"); delay(50);
  hanxy(110,16*1,1,"‰·‰b·a¡ Ðe •¡¯¡ˆa ‹a—i·"); delay(50);
  hanxy(110,16*2,1,"®º—µA ·¶·a“¡... ¹Aˆa ‹a"); delay(50);
  hanxy(110,16*3,1,"•¡¯¡µA¬á ‹a—i·i ¡©´a´¡"); delay(50);
  hanxy(110,16*4,1,"Ðs“¡”a. ¹A¤i ¸áŸi •¡µÁ¬á"); delay(50);
  hanxy(110,16*5,1,"‹a—i·i ¡©´a“¯¡”a!");
  delay(100);
//------------- reset interrupt num 8
  setvect(INTR, oldhandler); farfree(htemp); Fade_Out();
  farfree(title);

//font read routine
  clearscreen(0);
  brightCtrl(0,256,R_,G_,B_,100);
  readpcxfilebuf("font.pcx",&font);

//load sound effect file
  explo=readvocfile("explo.voc",&explol);
  beam=readvocfile("beam.voc",&beaml);
  door=readvocfile("door.voc",&doorl);
  get=readvocfile("get.voc",&getl);

  imagesize256(63,47,92,76,&htemp);
//start game routine
  if(stage1()!=-1) stage2();

  resetkeyint();
  clearscreen(0);
  hanxy(0,0,1,"ºA­·Ðs“¡”a. ´a»¢·e STAGE-1  e·¡ ˆa“w!");
  hanxy(0,16,1,"‹aœñ ´e‘wÓ¡...");
  hanend(); sleep(2);
//-------------- the end game routine
  farfree(beam); farfree(explo);
  farfree(get); farfree(door);
  farfree(htemp); farfree(font);
  settextmode();
}

//-------------- robot mouth multitask routine
void interrupt logo_handler(void)
{
   BYTE *temp;
   static int toggle=0,key=1,x,y,xl;

   if(SPACE) key=0;
   if(key){ delay(80);
   disable();
   //mouth proceeding routine
   switch(toggle++){
	  case 0: virtualscreen2(102,90,139,99,12,52,1,title); break;
	  case 1: //city & bomb proceeding routine
		  xl=random(320);
		  x=random(181); y=random(39)+115;
		  getimage256(x,y,x+33,y+28,htemp);
		  line256(xl,199,x+16,y+14,1,15);
                  delay(10);
		  line256(xl,199,x+16,y+14,1,15);
		  virtualscreen2(1,170,34,198,x,y,0,title); break;
	  case 2: virtualscreen2(36,170,69,198,x,y,0,title); break;
	  case 3: virtualscreen2(102,79,139,88,12,52,1,title); break;
	  case 4: virtualscreen2(71,170,104,198,x,y,0,title); break;
	  case 5: virtualscreen2(106,170,139,198,x,y,0,title); break;
	  case 6: virtualscreen2(141,170,174,198,x,y,0,title);
		  putimage256(x,y,0,htemp); toggle=0;} }
  enable();
  (*oldhandler)();
}

void line_fade_out(xx1,yy1,xx2,yy2)
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
 if(INED1){
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
 (*oldhandler)();
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

void e_explosive(void)
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
 static int tx=0,ty=0,lx=0,rx=0,uy=0,dy=0,thmode=0;

    //right key check routine
    if(RIGHT && !LEFT && !DOWN && !UP){ HX++; HMODE=2; lx=1; rx=0; if(HX>=290+1){ HX=290; lx=rx=0;}}

    //left key check routine
    if(LEFT && !RIGHT && !DOWN && !UP){ HX--; HMODE=3; lx=0; rx=1; if(HX<=0-1){ HX=0; lx=rx=0;}}

    if(move_form){ if(DOWN || HY<ty) {HY++; HMODE=0; uy=1; dy=0; if(HY>=170+1){ HY=170; uy=dy=0;}}
		   if(UP && HY==106) {HMODE=1; uy=0; dy=1; if(HY<=0-1){ HY=0; uy=dy=0;}}}
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

void life_display(void)
{
 switch(HLIFE){
	case 2: virtualscreen2(311,61,318,66,136,187,1,font); break;
	case 1: virtualscreen2(311,54,318,59,136,187,1,font); break;
	case 0: virtualscreen2(311,47,318,52,136,187,1,font);
	      }
}

void beam_display(void)
{
 if(HMODE!=0) sayVoice(beam,beaml);
 line256(BEAM_COUNT1,179,BEAM_COUNT1,181,0,21);
 line256(BEAM_COUNT1-1,179,BEAM_COUNT1-1,181,0,21);
 BEAM_COUNT1-=2;
 if(BEAM_COUNT1<136){ virtualscreen2(11,176,22,197,11,176,1,font);
		      sayVoice(get,getl); GUN=0; }
}

void stage1_init(int *exit)
{
 HX=3; HY=106; HMODE=0; GUN=0; EX=286; ELIFE=10; *exit=0;
 INED1=0;

 line_fade_out(0,0,319,174);
 virtualscreen2(0,175,319,199,0,175,1,font);
//in open door routine
 virtualscreen2(0,0,319,174,0,0,1,st_im);
 life_display();
 sayVoice(door,doorl);
 fill_box(2,103,33,134,0); fill_box(285,103,317,134,0);
 delay(300);
 virtualscreen2(2,103,33,134,2,103,1,st_im);
 virtualscreen2(1,47,30,76,HX,HY,0,font);
 virtualscreen2(285,103,317,134,285,103,1,st_im);
 virtualscreen2(156,47,185,76,EX,HY,0,font);
 delay(100);
 INED1=1;
}

//-------------- game stage1 routine
int stage1(void)
{
 BYTE *stage1i,*temp;
 int sp=1,out_door=0,exit=0,rev=-1;

 imagesize256(63,47,92,76,&temp);
 etemp=temp;
 readpcxfilebuf("stage1.pcx",&stage1i);
 st_im=stage1i;

 stage1_init(&exit);

 //set multi enemy proceeding
 oldhandler = getvect(INTR);
 setvect(INTR, move_camera);

 do{hero_move(0,st_im);
    //get gun routine
    if(!GUN){ virtualscreen2(307,1,318,22,156,114,0,font);
	      if(HX+29>=156){ virtualscreen2(157,114,167,135,157,114,1,st_im);
			      sayVoice(get,getl);
			      virtualscreen2(307,1,318,22,11,176,0,font);
			      fill_box(136,179,315,181,4);
			      BEAM_COUNT1=315; GUN=1;
			     }
	    }
    //gun proceeding routine
    if(SPACE && GUN && sp && HMODE!=0){ beam_display();
       switch(HMODE){ case 1: line256(HX+24,HY+7,HX+24,39,1,15);
			      delay(50);
			      line256(HX+24,HY+7,HX+24,39,1,15);
			      if(HX+24>=158 && HX+24<=166 && !out_door){
			      brightCtrl(0,256,R_,G_,B_,70); INED1=0;
			      virtualscreen2(EX,HY,EX+30,HY+29,EX,HY,1,st_im);
			      EX=286; ELIFE=10; INED1=1;
			      sayVoice(door,doorl);
			      delay(500);
			      brightCtrl(0,256,R_,G_,B_,100);       }
			      if(HX+24>=230 && HX+24<=235 && !out_door){
			      h_explosive(HX,39);
			      virtualscreen2(HX,39,EX+30,39+45,HX,39,1,st_im);
			      if(exit++ >=4){ virtualscreen2(1,176,40,183,214,56,1,st_im);
			      ELIFE=0; INED1=0;
			      virtualscreen2(EX,HY,EX+30,HY+29,EX,HY,1,st_im);
			      EX=319; ELIFE=0; out_door=1;} } break;
		      case 2: line256(HX+18,HY+14,EX,HY+14,1,15);
			      delay(50);
			      line256(HX+18,HY+14,EX,HY+14,1,15);
			      if(!out_door){ e_explosive();
			      if(ELIFE-- <=1){ sayVoice(explo,explol);
			      virtualscreen2(EX,HY,EX+30,HY+29,EX,HY,1,st_im);
			      virtualscreen2(280,47,309,76,EX,HY,0,font);
			      delay(300);
			      virtualscreen2(EX,HY,EX+30,HY+29,EX,HY,1,st_im);
			      EX=286; ELIFE=10;
			      sayVoice(door,doorl); } } break;
		      case 3: line256(HX+11,HY+14,0,HY+14,1,15);
			      delay(50);
			      line256(HX+11,HY+14,0,HY+14,1,15); }
	     } if(SPACE) sp=0; else sp=1;
    //hero & enemy match proceeding routine
    if(ELIFE>0 && HX+29>=EX){  INED1=0; h_explosive(HX,HY);
			       virtualscreen2(HX,HY,EX+30,HY+45,HX,HY,1,st_im);
			       virtualscreen2(125,47,154,76,HX,HY,0,font);
			       virtualscreen2(280,47,309,76,EX,HY,0,font);
			       delay(500); if(HLIFE-- ==0){ rev=-1; break; }
			       stage1_init(&exit);
			     }
    //out open door routine
    if(HX>=286 && UP && out_door){ sayVoice(door,doorl);
				   virtualscreen2(286,103,319,136,286,103,1,st_im);
				   virtualscreen2(32,47,61,76,287,106,0,font);
				   delay(500); fill_box(286,103,317,134,0);
				   delay(500); virtualscreen2(286,103,317,134,286,103,1,st_im);
				   delay(500); rev=0; break;
				  }
   }while(ESC!=1);

   setvect(INTR, oldhandler);
   line_fade_out(0,0,319,174);
   if(VoiceFlag) silentVoice();
   farfree(stage1i); farfree(temp);
   return(rev);
}

void init_gv(void)
{
 HX=3; HY=106; HMODE=0; EX=289; ELIFE=10; INED0=INED1=INED2=INED3=1;
}

void interrupt stage2_task(void)
{
   static int run1=0,run2=0,run3=0,run4=0;

   disable();
   if(INED0) switch(run4++){
		    case 0: virtualscreen2(114,191,141,193,87,148,1,st_im); break;
		    case 1: virtualscreen2(143,188,155,196,32,157,1,st_im); break;
		    case 2: virtualscreen2(157,188,169,196,32,157,1,st_im); break;
		    case 3: virtualscreen2(171,188,183,196,32,157,1,st_im); break;
		    case 4: virtualscreen2(87,148,114,150,87,148,1,st_im); run4=0;
			   }

   if(INED1) switch(run1++){
		    case 0: virtualscreen2(64,176,125,186,131,136,1,st_im); break;
		    case 1: virtualscreen2(127,176,188,186,131,136,1,st_im); break;
		    case 2: virtualscreen2(190,176,251,186,131,136,1,st_im); break;
		    case 3: virtualscreen2(253,176,314,186,131,136,1,st_im); run1=0;
			   }
   if(INED2) switch(run2++){
		    case 0: virtualscreen2(1,191,18,193,204,27,1,st_im); break;
		    case 1: virtualscreen2(20,191,37,193,204,27,1,st_im); break;
		    case 2: virtualscreen2(39,191,56,193,204,27,1,st_im); break;
		    case 3: virtualscreen2(58,191,75,193,204,27,1,st_im); break;
		    case 4: virtualscreen2(77,191,94,193,204,27,1,st_im); break;
		    case 5: virtualscreen2(96,191,112,193,204,27,1,st_im); run2=0;
			   }
   if(INED3) switch(run3++){
		    case 0: virtualscreen2(1,191,18,193,294,78,1,st_im); break;
		    case 1: virtualscreen2(20,191,37,193,294,78,1,st_im); break;
		    case 2: virtualscreen2(39,191,56,193,294,78,1,st_im); break;
		    case 3: virtualscreen2(58,191,75,193,294,78,1,st_im); break;
		    case 4: virtualscreen2(77,191,94,193,294,78,1,st_im); break;
		    case 5: virtualscreen2(96,191,112,193,294,78,1,st_im); run3=0;
			   }
   if(PASS_STAGE!=1){ getimagevirtual(EX,115,EX+29,115+13,etemp,st_im);
		      composition(1,32,30,45,font,EX,115,st_im,0);
		      virtualscreen2(EX,115,EX+30,115+13,EX,115,1,st_im);
		      getcomposition(etemp,EX,115,st_im,1);
		      if(EX--<=0){virtualscreen2(0,115,30,115+13,0,115,1,st_im); EX=290;} }
   enable();
   (*oldhandler)();
}

void stage2_init(void)
{
  INED0=INED1=INED2=INED3=0;
  PASS_STAGE=1;
  line_fade_out(0,0,319,174);
  init_gv();
  JUM=0; virtualscreen2(44,176,55,197,44,176,1,font);
//in open door routine
  virtualscreen2(0,0,319,174,0,0,1,st_im);
  PASS_STAGE=0;
  life_display();
  sayVoice(door,doorl);
  fill_box(2,103,33,134,0);
  delay(300);
  virtualscreen2(2,103,33,134,2,103,1,st_im);
  virtualscreen2(1,47,30,76,HX,HY,0,font);
}

int stage2(void)
{
  BYTE *stage2i,*temp;
  int sp=1,jp=1;
  readpcxfilebuf("stage2.pcx",&stage2i);
  st_im=stage2i;
  imagesize256(1,32,30,45,&temp);
  etemp=temp;

  stage2_init();
  oldhandler = getvect(INTR);
  setvect(INTR,stage2_task);

  do{
     if(HY<106) hero_move(1,st_im); else hero_move(0,st_im);
     if(!JUM){ virtualscreen2(307,24,318,45,109,114,0,font);
	       if(HX+29>=109){ virtualscreen2(109,114,109+11,114+21,109,114,1,st_im);
			       sayVoice(get,getl);
			       virtualscreen2(307,24,318,45,44,176,0,font);
			       //fill_box(136,179,315,181,4);
			       JUM=1; }
	     }

     //hero die proceeding routine
     if(HX>=132){ sayVoice(explo,explol); virtualscreen2(HX,HY,HX+29,HY+29,HX,HY,1,st_im);
		  virtualscreen2(1,78,30,107,132,136,0,font); if(HLIFE-- ==0) break;
		  delay(200); stage2_init();
		}
     //space bar proceeding routine
     if(SPACE && GUN && sp && HMODE!=0) beam_display();
     if(SPACE) sp=0; else sp=1;

     //jump proceeding routine
     if(ALT && JUM && jp && HY==106){virtualscreen2(HX,HY,HX+29,HY+29,HX,HY,1,st_im);
				     HY=39;} else if( HY++ >=106) HY=106;
     if(ALT) jp=0; else jp=1;

    }while(ESC!=1);

  INED0=INED1=INED2=INED3=0; PASS_STAGE=1;
  line_fade_out(0,0,319,199);
  setvect(INTR, oldhandler);
  farfree(stage2i); farfree(temp);
  return(0);
}

