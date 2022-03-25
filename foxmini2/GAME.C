#include "sed.h"

sprites plane[10];
sprites juckpic[9];
sprites back[1];

void soga(void);

main()
{
    int x=90,y=100,mode=0,page=0,size=0,a=0,end=0,score=0,gasu=8,gasuch=200;

    struct { int picnum,x,y,speed;
	    }juckja[30];

    Vga_Plane();

    Load_Font();
    Set_Cycle_Alone();
    soga();

    /*Load_Spr(0,0,"back.spr",back);
    Decode_Spr(0,0,back,64);*/

    Load_Spr(0,2,"t1.spr",plane);
    Decode_Spr(0,2,plane,2);

    Load_Spr(0,8,"t2.spr",juckpic);
    Decode_Spr(0,8,juckpic,2);

    Multi_Key();
    Voice_Init(2);
    randomize();

    Work_Page(3);
    CPut_Spr(0,0,&back[0],0);
    Work_Page(page);

    for (a=0;a<30;a++){
      juckja[a].picnum=random(9);
      juckja[a].x=random(320)+320;
      juckja[a].y=random(199)+1;
      juckja[a].speed=random(2)+4;
      }

    Mus_Play("game.son",MDI);

    while(!_key[_ESC]) { Work_Page(page);

	Page_Copy(3,page);

	Rotate_Cycle(121,107,0);

	score++;
	Printf_Han(240,183,font_color[5],0,2,"¸ñ®| %04d",score);

	if(_key[_LEFT]) x-=4;
	if(_key[_RIGHT]) x+=4;
	if(_key[_UP]) { y-=3; mode = 2; }
	if(_key[_DOWN]) { y+=3; mode = 1; }

	if (mode==0) size=11;
	if (mode==1) size=16;
	if (mode==2) size=17;

	if (x<1) x=x+4;
	if (x>320-36) x=x-4;
	if (y<1) y=y+3;
	if (y>200-size) y=y-3;

    CPut_Spr(x, y, &plane[mode], 0);

    if (score==gasuch){ gasu++;
			gasuch=gasuch+200;
			if (gasu>30) gasu=30;
		      }

    for (a=0;a<gasu;a++) {
	 CPut_Spr(juckja[a].x,juckja[a].y,&juckpic[juckja[a].picnum],0);
	 if (C_Cheak(x,x+36,y,y+size,juckja[a].x,juckja[a].x+13,juckja[a].y,juckja[a].y+13)==1) end=1;
	 juckja[a].x-=juckja[a].speed;
	  if(juckja[a].x<-10) {
	      juckja[a].x=330;
	      juckja[a].y=random(199)+1;
	      juckja[a].speed=random(2)+4;
	      juckja[a].picnum=random(9);
	  }
    }
    mode = 0;

    View_Page(page);
    page=!page;
    if (end==1) break;
    }
   Mus_End();
   Voice_Say("kill.tum",VOC);
   delay(4000);

   Printf_Han(55,80,font_color[2],0,3,"”w¯¥· ¸ñ®“e %d ¸ñ ·³“¡”a.",score);
   Printf_Han(65,100,font_color[9],0,1,"¬a¶wÐº­a¬á ˆq¬aÐs“¡”a.");

   delay(10000);

   Restore_Cycle_Alone();

   Fade_Out(0,255,3);

   Free_Spr(0,2,plane);
   Free_Spr(0,8,juckpic);
   Free_Spr(0,0,back);
   Han_End();
   Voice_End();
   Restore_Key();
   Text_Mode();
printf ("Thank You.    <Your Score: %d>",score);
printf ("\nMaker is Kim Tae-suk.");
}
void soga(void)
{
 int i=0;

 Load_Spr(0,0,"back.spr",back);
 Decode_Data(&back[0]);

 Fade(0,255,0);
 Mus_Play("title.son",MDI);
 Work_Page(0);
 Work_Clear(2);

 Put_Spr (0,0,&back[0],0);
 Printf_Han (11,11,font_color[11],0,3,"Fox Mini ---------------------<‹±È¬â>");
 Printf_Han (11,31,font_color[11],0,3,"Ïa¡‹aœ‘ ¬é¡wµA —i´áˆa‹¡ ¸åµA SEDŸi ¹A");
 Printf_Han (11,46,font_color[11],0,3,"¸b Ðaµa º¯¥ sedgame“±A  å¸á ˆq¬a —a ");
 Printf_Han (11,61,font_color[11],0,3,"Ÿ³“¡”a.                               ");
 Printf_Han (11,76,font_color[11],0,3,"-------------<Ïa¡‹aœ‘ ˆ¶a>----------");
 Printf_Han (11,91,font_color[11],0,3,"·¡ Ïa¡‹aœ‘·e SED œa·¡§aœáŸ¡ 3.05¡   ");
 Printf_Han(11,106,font_color[11],0,3," e—i´á»¥ ˆõ·a¡ SED¡ ‰A·±·i ¹A¸bÐi® ");
 Printf_Han(11,121,font_color[11],0,3,"·¶”a“e ˆé »w¡wÐa“e •·¯¡µA Á¡¥¡¸a “±—i ");
 Printf_Han(11,136,font_color[11],0,3,"A“e ”Â— ·¡œåˆõ—i·¡ SED¡ –A“eŠaŸi ");
 Printf_Han(11,151,font_color[11],0,3,"´i® ·¶·a¯©ˆó“¡”a.");
 Printf_Han(11,166,font_color[11],0,3,"------------<Ïa¡‹aœ‘ ¬é¡w>-----------");
 Printf_Han(11,181,font_color[11],0,3,"·¡ Ïa¡‹aœ‘·e ¹·Ñw ¯AË· ‰A·±·a¡ FOX R");

 Printf_Han (10,10,font_color[9],0,1,"Fox Mini ---------------------<‹±È¬â>");
 Printf_Han (10,30,font_color[3],0,1,"Ïa¡‹aœ‘ ¬é¡wµA —i´áˆa‹¡ ¸åµA SEDŸi ¹A");
 Printf_Han (10,45,font_color[3],0,1,"¸b Ðaµa º¯¥ sedgame“±A  å¸á ˆq¬a —a ");
 Printf_Han (10,60,font_color[3],0,1,"Ÿ³“¡”a.                               ");
 Printf_Han (10,75,font_color[9],0,1,"-------------<Ïa¡‹aœ‘ ˆ¶a>----------");
 Printf_Han (10,90,font_color[8],0,1,"·¡ Ïa¡‹aœ‘·e SED œa·¡§aœáŸ¡ 3.05¡   ");
 Printf_Han(10,105,font_color[8],0,1," e—i´á»¥ ˆõ·a¡ SED¡ ‰A·±·i ¹A¸bÐi® ");
 Printf_Han(10,120,font_color[8],0,1,"·¶”a“e ˆé »w¡wÐa“e •·¯¡µA Á¡¥¡¸a “±—i ");
 Printf_Han(10,135,font_color[8],0,1,"A“e ”Â— ·¡œåˆõ—i·¡ SED¡ –A“eŠaŸi ");
 Printf_Han(10,150,font_color[8],0,1,"´i® ·¶·a¯©ˆó“¡”a.");
 Printf_Han(10,165,font_color[9],0,1,"------------<Ïa¡‹aœ‘ ¬é¡w>-----------");
 Printf_Han(10,180,font_color[5],0,1,"·¡ Ïa¡‹aœ‘·e ¹·Ñw ¯AË· ‰A·±·a¡ FOX R");
 Work_Page(1);
 Work_Clear(2);
 Put_Spr (0,0,&back[0],0);
 Printf_Han  (11,1,font_color[11],0,3,"ANGER· §¡Ð—‹¡ˆa ´|µA¬á i´aµ¡“e ‰·(?)");
 Printf_Han (11,16,font_color[11],0,3,"·i Ï¡Ða¡a ¸å»¥Ða“e ‰A·±·³“¡”a.");
 Printf_Han (11,31,font_color[11],0,3,"‰·· ®“e ˆa¡e ˆi®¢ “i´ás“¡”a.(¢‰¥");
 Printf_Han (11,46,font_color[11],0,3,"Ðe‰“e ·¶»¡ e ´ás»¡·~~!)");
 Printf_Han (11,61,font_color[11],0,3,"--------------<”w¦·  i>-------------");
 Printf_Han (11,76,font_color[11],0,3,"·¡‰A·±·e ¸£¡“e ´ô»¡ e ¹A´éŠ‰·i ¥Á¬áœa");
 Printf_Han (11,91,font_color[11],0,3,"•¡(´e¥¡·¡a?)¸£¡·¶‰A Ðº¯³¯¡µ¡..");
 Printf_Han (156,116,font_color[11],0,3,"ˆq¬aÐs“¡”a.");

 Printf_Han  (10,0,font_color[5],0,1,"ANGER· §¡Ð—‹¡ˆa ´|µA¬á i´aµ¡“e ‰·(?)");
 Printf_Han (10,15,font_color[5],0,1,"·i Ï¡Ða¡a ¸å»¥Ða“e ‰A·±·³“¡”a.");
 Printf_Han (10,30,font_color[5],0,1,"‰·· ®“e ˆa¡e ˆi®¢ “i´ás“¡”a.(¢‰¥");
 Printf_Han (10,45,font_color[5],0,1,"Ðe‰“e ·¶»¡ e ´ás»¡·~~!)");
 Printf_Han (10,60,font_color[9],0,1,"--------------<”w¦·  i>-------------");
 Printf_Han (10,75,font_color[6],0,1,"·¡‰A·±·e ¸£¡“e ´ô»¡ e ¹A´éŠ‰·i ¥Á¬áœa");
 Printf_Han (10,90,font_color[6],0,1,"•¡(´e¥¡·¡a?)¸£¡·¶‰A Ðº¯³¯¡µ¡..");
 Printf_Han (155,115,font_color[9],0,1,"ˆq¬aÐs“¡”a.");

 Fade_In (0,255,3);
 getch();
 for (i=1;i<=150;i++) { Hard_Scroll(0,i);
			Rotate_Cycle(121,107,0);
		      }
 getch();
 Fade_Out(0,255,3);
 for (i=0;i<=4;i++) { Work_Page(i);
		      Work_Clear(0);
		    }
 Mus_End();
Fade(0,255,225);
}
