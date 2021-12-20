//#include <io.h>
//#include <mem.h>
#include <dos.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
//#include <alloc.h>
#include <conio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <graphics.h>

#include "omok.h"
#include "read16.h"

int main(void)
{
    int i,ch,loop = TRUE;

    initg();
    readpcx16p("madang2.pcx",0,0);

    InitLibDefault();

    while(loop) {

       screen();
       initT();
       sound(800);delay(50);nosound();

       game();

       setwritemode(COPY_PUT);
       setfillstyle(SOLID_FILL,BLUE);
       bar(100,200,400,230);
       ButtonRect(100,200,400,230,1);
       setcolor(YELLOW);
       outtextxy(130,210,"DO YOU WANT TO PLAY AGAIN(Y/N)?");

       for( ; ; ) {
	  ch = getch();
	  if(ch == 'Y' || ch == 'y' || ch == 'N' || ch == 'n') {
	     for(i=0; i<=15; i++) {
		   ScrollUp(0,0,80,14,1);
		   ScrollDown(0,15,80,29,1);
		   delay(30);
		}
	     if(ch == 'N' || ch == 'n') {
		loop = FALSE;
		break;
	     } else {
		readpcx16p("madang2.pcx",0,0);
		break;
	     }
	  }
       }
    }
    end();
    return(0);
}

void initg(void)
{
    int gd=DETECT,gm,errorcode;
    initgraph(&gd,&gm,"");

     errorcode = graphresult();

    if(errorcode != grOk) {
       printf("Graphics error: %s\n", grapherrormsg(errorcode));
       printf("Press any key to halt:");
       getch();
       exit(1);
    }
}

void screen(void)
{
    int i;

    setfillstyle(SOLID_FILL,LIGHTBLUE);
    bar(463,18,632,112);
    bar(463,128,632,182);
    ButtonRect(463,18,632,112,1);
    ButtonRect(463,128,632,182,1);
    ButtonRect(463,198,632,292,1);
    ButtonRect(463,318,632,442,1);

    setcolor(WHITE);

    for(i=0;i<15;i++) {
	line(MIN,MIN+STEP*i,MAX,MIN+STEP*i);
	line(MIN+STEP*i,MIN,MIN+STEP*i,MAX);
    }

    line(465,65,630,65);

    dol(485,42,MAN);
    settextstyle(DEFAULT_FONT, HORIZ_DIR,1);
    outtextxy(510,40,": MAN");
    dol(485,87,COMPUTER);
    outtextxy(510,85,": COM");
    outtextxy(475,142,"TIME>");
    settextstyle(DEFAULT_FONT, HORIZ_DIR,1);
    outtextxy(493,212,"ESC   : quit   ");
    outtextxy(493,225,"SPACE : put dol");
    outtextxy(493,238,"UP    : up     ");
    outtextxy(493,251,"DOWN  : down   ");
    outtextxy(493,264,"RIGHT : right  ");
    outtextxy(493,277,"LEFT  : left ");
    line(465,350,630,350);
    outtextxy(493,330,"<< MESSAGE >>");
}

void initT(void)
{
    int i,j;

    for(i=0;i<15;i++) {
	for(j=0;j<15;j++) {
	   TABLE[i][j] = -1;
	}
    }

    count = 0;
    xvalue=230;
    yvalue=230;
}

void game(void)
{
    int i,c,mode=MAN,loop=TRUE,flag,newsec=0,oldsec,timer=30;
    struct  time t;
    char buf[3];

    setcolor(WHITE);
    setwritemode(XOR_PUT);
    rect(xvalue,yvalue);

    gettime(&t);
    oldsec = t.ti_sec;

    while(loop) {
	 if(mode == MAN) {
	    gettime(&t);
	    newsec = t.ti_sec;
	    if(abs(newsec-oldsec)==0) {
	       setwritemode(COPY_PUT);
	       setfillstyle(SOLID_FILL,LIGHTBLUE);
	       bar(530,142,620,170);
	       sprintf(buf,"%d", timer--);
	       outtextxy(540,142,buf);
	       outtextxy(500,160,"SECOND REMAINS");
	       setwritemode(XOR_PUT);
	       if(newsec == 59) newsec = -1;
	       oldsec = newsec + 1;
	       if(timer<10) { sound(100);delay(100);nosound(); }
	    }

	    if(timer==0) {
	       Error(2);
	       mode= change(mode);
	    }

	    if(kbhit()) {

	       c=getch();
	       if(c==0) c |= getch()<<8;

	       switch( c ) {
		   case ESC  : loop = FALSE;
			       break;
		   case SPACE: pox=(xvalue-20)/30;
			       poy=(yvalue-20)/30;
			       if(TABLE[pox][poy] == -1 ) {
				   if(dolvalue(pox,poy,mode) != SAM2) {
				      dol(xvalue,yvalue,mode);
				      TABLE[pox][poy] = mode;
				      if(dolvalue(pox,poy,mode)==OK) {
					 setwritemode(COPY_PUT);
					 setfillstyle(SOLID_FILL,BLUE);
					 bar(200,160,300,190);
					 ButtonRect(200,160,300,190,1);
					 sound(1000);delay(80);sound(800);delay(50);
					 sound(900);delay(100);sound(1000);delay(80); nosound();
					 for(i=0; i<5; i++) {
					    setcolor(BLUE);
					    outtextxy(210,170,"YOU WIN!!");
					    delay(100);
					    setcolor(YELLOW);
					    outtextxy(210,170,"YOU WIN!!");
					    delay(100);
					 }
					 loop = FALSE;
				      }
				      mode = change(mode);
				   }
				   else Error(1);
			       }
			       gettime(&t);
			       oldsec = t.ti_sec;
			       timer = 30;
			       sound(500);delay(100);nosound();
			       break;
		   case LEFT : if(check(xvalue-30,yvalue)) {
				    rect(xvalue,yvalue);
				    xvalue -= 30;
				    rect(xvalue,yvalue);
			       }
			       break;
		   case RIGHT: if(check(xvalue+30,yvalue)) {
				    rect(xvalue,yvalue);
				    xvalue += 30;
				    rect(xvalue,yvalue);
			       }
			       break;
		   case UP   : if(check(xvalue,yvalue-30)) {
				    rect(xvalue,yvalue);
				    yvalue -= 30;
				    rect(xvalue,yvalue);
			       }
			       break;
		   case DOWN : if(check(xvalue,yvalue+30)) {
				    rect(xvalue,yvalue);
				    yvalue += 30;
				    rect(xvalue,yvalue);
			       }
			       break;

	       }
	    }
	 }
	 else {
	      flag = TRUE;
	      while(flag) {
		 Tcheck();
		 if(TABLE[pox][poy] == -1) flag=FALSE;
	      }
	      dol(pox*30+20,poy*30+20,COMPUTER);
	      TABLE[pox][poy] = mode;
	      if(dolvalue(pox,poy,mode)==OK) {
		 setwritemode(COPY_PUT);
		 setfillstyle(SOLID_FILL,BLUE);
		 bar(200,160,300,190);
		 ButtonRect(200,160,300,190,1);
		 sound(1000);delay(50);sound(500);delay(20);
		 sound(800);delay(50);sound(1000);delay(80); nosound();
		 for(i=0; i<5; i++) {
		    setcolor(BLUE);
		    outtextxy(210,170,"YOU LOSE!!");
		    delay(100);
		    setcolor(YELLOW);
		    outtextxy(210,170,"YOU LOSE!!");
		    delay(100);
		 }
		 loop = FALSE;
	      }
	      mode = change(mode);
	      gettime(&t);
	      oldsec = t.ti_sec;
	      timer =30;
	      sound(1000);delay(100);nosound();
	 }

	 if(count == 225) loop = FALSE;

    }
}

void AddLib( char *dol, int value,int i)
{
	int k;
	k= i -1;

	if(libCount[k] >= LIBLEN) return;

	lib[k][(libCount[k])].value = value;
	strcpy(lib[k][(libCount[k])].dol, dol);
	libCount[k]++;
}

void InitLibDefault(void)
{
	// ONE DOL
	AddLib("   o o     ",    5,1);  AddLib("o oo o     ",   28,1);
	AddLib("  oo o     ",   85,1);	AddLib(" ooo o     ", 1000,1);
	AddLib(" o o o     ",   27,1); 	AddLib(" oo  o     ",   27,1);
	AddLib("     o oo  ",   85,1);	AddLib("     o ooo ", 1000,1);
	AddLib("     o oo o",   28,1);	AddLib("     o o o ",   28,1);
	AddLib("     o  o  ",    4,1);	AddLib("     o  oo ",   27,1);
	AddLib("   o o o   ",   27,1);	AddLib("   o o oo  ",   27,1);
	AddLib("   o o ooo ", 1000,1);	AddLib("   o o o oo",   27,1);
	AddLib("  oo o o   ",   28,1);	AddLib("  oo o oo  ",   28,1);
	AddLib("  oo o ooo ", 1000,1);	AddLib("  oo o o oo",   28,1);
	AddLib("  oo o oo o",   28,1);	AddLib("  oo o o  o",   28,1);
	AddLib("  oo o o o ",   28,1);	AddLib("  oo o  o  ",   85,1);
	AddLib("  oo o  oo ",   85,1);	AddLib("  oo o  ooo",   85,1);
	AddLib("  oo o  o o",   85,1);	AddLib("  oo o   o ",   85,1);
	AddLib("  oo o   oo",   85,1);	AddLib("  oo o    o",   85,1);
	AddLib(" ooo o o   ", 1000,1);	AddLib(" ooo o oo  ", 1000,1);
	AddLib(" ooo o ooo ", 5000,1);	AddLib(" ooo o oo o", 1000,1);
	AddLib(" ooo o o oo", 1000,1);	AddLib(" ooo o o o ", 1000,1);
	AddLib(" ooo o o  o", 1000,1);	AddLib(" ooo o  o  ", 1000,1);
	AddLib(" ooo o  oo ", 1000,1);	AddLib(" ooo o  ooo", 1000,1);
	AddLib(" ooo o  o o", 1000,1);	AddLib(" ooo o   o ", 1000,1);
	AddLib(" ooo o   oo", 1000,1);	AddLib(" ooo o    o", 1000,1);
	AddLib("o oo o oo  ",   28,1);	AddLib("o oo o ooo ", 1000,1);
	AddLib("oo o o ooo ", 1000,1);	AddLib(" o o o ooo ", 1000,1);
	AddLib("o  o o ooo ", 1000,1);	AddLib("  o  o o   ",    6,1);
	AddLib("  o  o oo  ",   85,1);	AddLib("  o  o ooo ", 1000,1);
	AddLib(" oo  o o   ",    6,1);	AddLib(" oo  o oo  ",   85,1);
	AddLib(" oo  o ooo ", 1000,1);	AddLib("ooo  o o   ",    6,1);
	AddLib("ooo  o oo  ",   85,1);	AddLib("ooo  o ooo ", 1000,1);
	AddLib("o o  o o   ",    6,1);	AddLib("o o  o oo  ",   85,1);
	AddLib("o o  o ooo ", 1000,1);	AddLib(" o   o o   ",    6,1);
	AddLib(" o   o oo  ",   85,1);	AddLib(" o   o ooo ", 1000,1);
	AddLib("oo   o o   ",    6,1);	AddLib("oo   o oo  ",   85,1);
	AddLib("oo   o ooo ", 1000,1);	AddLib("o    o o   ",    6,1);
	AddLib("o    o oo  ",   85,1);	AddLib("o    o ooo ", 1000,1);
	AddLib(" oo  o    x",   27,1);	AddLib(" ooo o    x", 1000,1);
	AddLib(" o o o    x",   27,1);	AddLib("o oo o    x",   28,1);
	AddLib("  oo o    x",   85,1);	AddLib("   o o    x",    6,1);
	AddLib("     o o  x",    6,1);	AddLib("     o oo x",   85,1);
	AddLib("     o ooox", 1000,1);	AddLib("     o o ox",   27,1);
	AddLib("   o o o  x",   27,1);	AddLib("   o o oo x",   28,1);
	AddLib("   o o ooox", 1000,1);	AddLib("  oo o o  x",   28,1);
	AddLib("  oo o oo x",   28,1);	AddLib("  oo o ooox", 1000,1);
	AddLib(" ooo o o  x", 1000,1);	AddLib(" ooo o oo x", 1000,1);
	AddLib(" ooo o ooox", 5000,1);	AddLib(" ooo o o ox", 1000,1);
	AddLib(" o o o ooox", 1000,1);	AddLib(" oo  o ooox", 1000,1);
	AddLib(" oo  o oo x",   85,1);	AddLib("   o o   xx",    6,1);
	AddLib("  oo o   xx",   85,1);	AddLib(" ooo o   xx", 1000,1);
	AddLib(" oo  o   xx",   27,1);	AddLib(" o o o   xx",   27,1);
	AddLib("     o o xx",    6,1);	AddLib("     o ooxx",   28,1);
	AddLib("   o o o xx",   27,1);	AddLib("  oo o o xx",   28,1);
	AddLib(" ooo o   xx", 1000,1);	AddLib(" o o o o xx",   27,1);
	AddLib("  oo o ooxx",   28,1);	AddLib(" ooo o ooxx", 1000,1);
	AddLib(" o o o ooxx",   27,1);	AddLib("   o o  xxx",    6,1);
	AddLib("  oo o  xxx",   85,1);	AddLib(" ooo o  xxx", 1000,1);
	AddLib(" o o o  xxx",   27,1);	AddLib(" oo  o  xxx",   27,1);
	AddLib("   o o oxxx",   27,1);	AddLib("  oo o oxxx",   28,1);
	AddLib(" ooo o  xxx", 1000,1);	AddLib(" o o o oxxx",   27,1);
	AddLib(" oo  o oxxx",   27,1);	AddLib("   o o xxxx",    6,1);
	AddLib("  oo o xxxx",   28,1);	AddLib("o oo o xxxx",   28,1);
	AddLib(" ooo o xxxx", 1000,1);	AddLib(" o o o xxxx",   27,1);
	AddLib(" oo  o xxxx",   27,1);	AddLib("x  o o     ",    6,1);
	AddLib("x oo o     ",   85,1);	AddLib("xooo o     ", 1000,1);
	AddLib("xo o o     ",   27,1);	AddLib("xoo  o     ",   27,1);
	AddLib("x    o o   ",    6,1);	AddLib("x    o oo  ",   85,1);
	AddLib("x    o ooo ", 1000,1);	AddLib("x    o oo o",   28,1);
	AddLib("x    o o o ",   27,1);	AddLib("x    o  oo ",   27,1);
	AddLib("x  o o o   ",   27,1);	AddLib("x  o o oo  ",   28,1);
	AddLib("x  o o ooo ", 1000,1);	AddLib("x  o o o o ",   27,1);
	AddLib("x  o o  oo ",   27,1);	AddLib("x oo o o   ",   28,1);
	AddLib("x oo o oo  ",   28,1);	AddLib("x oo o ooo ", 1000,1);
	AddLib("x oo o oo o",   28,1);	AddLib("x oo o o  o",   28,1);
	AddLib("x oo o  oo ",   85,1);	AddLib("xooo o o   ", 1000,1);
	AddLib("xooo o oo  ", 1000,1);	AddLib("xooo o ooo ", 5000,1);
	AddLib("xooo o oo o", 1000,1);	AddLib("xooo o o  o", 1000,1);
	AddLib("xooo o  oo ", 1000,1);	AddLib("xo o o oo  ",   28,1);
	AddLib("xo o o ooo ", 1000,1);	AddLib("xo o o oo o",   27,1);
	AddLib("xoo  o oo  ",   85,1);	AddLib("xoo  o ooo ", 1000,1);
	AddLib("x  o o    x",    6,1);	AddLib("x oo o    x",   85,1);
	AddLib("xooo o    x", 1000,1);	AddLib("xoo  o    x",   27,1);
	AddLib("xo o o    x",   27,1);	AddLib("x    o o  x",    6,1);
	AddLib("x    o oo x",   85,1);	AddLib("x    o ooox", 1000,1);
	AddLib("x    o o ox",   27,1);	AddLib("x    o  oox",   27,1);
	AddLib("x  o o o  x",   27,1);	AddLib("x  o o oo x",   28,1);
	AddLib("x  o o ooox", 1000,1);	AddLib("x  o o  oox",   27,1);
	AddLib("x oo o o  x",   28,1);	AddLib("x oo o oo x",   28,1);
	AddLib("x oo o ooox", 1000,1);	AddLib("x oo o  oox",   85,1);
	AddLib("xooo o o  x", 1000,1);	AddLib("xooo o oo x", 1000,1);
	AddLib("xooo o ooox", 5000,1);	AddLib("xooo o  oox", 1000,1);
	AddLib("xoo  o o  x",   27,1);	AddLib("xoo  o ooox", 1000,1);
	AddLib("xoo  o oo x",   85,1);	AddLib("xoo  o  oox",   27,1);
	AddLib("x  o o   xx",    6,1);	AddLib("x oo o   xx",   85,1);
	AddLib("xooo o   xx", 1000,1);	AddLib("xo o o   xx",   27,1);
	AddLib("xoo  o   xx",   27,1);	AddLib("x    o o xx",    6,1);
	AddLib("x    o ooxx",   28,1);	AddLib("x  o o o xx",   27,1);
	AddLib("x oo o o xx",   28,1);	AddLib("xooo o o xx", 1000,1);
	AddLib("xooo o ooxx", 1000,1);	AddLib("xooo o  oxx", 1000,1);
	AddLib("xo o o ooxx",   27,1);	AddLib("xoo  o ooxx",   27,1);
	AddLib("xoo  o o xx",   27,1);	AddLib("x  o o  xxx",    6,1);
	AddLib("x oo o  xxx",   85,1);	AddLib("xooo o  xxx", 1000,1);

	AddLib("xoo  o  xxx",   27,1);	AddLib("xo o o  xxx",   27,1);
	AddLib("x  o o oxxx",   27,1);	AddLib("x oo o oxxx",   28,1);
	AddLib("xooo o oxxx", 1000,1);	AddLib("xo o o oxxx",   27,1);
	AddLib("xoo  o oxxx",   27,1);	AddLib("x  o o xxxx",    7,1);
	AddLib("x oo o xxxx",   85,1);	AddLib("xooo o xxxx", 1000,1);
	AddLib("xo o o xxxx",   27,1);	AddLib("xoo  o xxxx",   27,1);
	AddLib("x oo oxxxxx",   28,1);	AddLib("xooo oxxxxx", 1000,1);
	AddLib("xo o oxxxxx",   27,1);	AddLib("xoo  oxxxxx",   27,1);
	AddLib("xx   o oo  ",   85,1);	AddLib("xxoo o     ",   28,1);
	AddLib("xx o o     ",    7,1);	AddLib("xx   o o   ",    7,1);
	AddLib("xx   o ooo ", 1000,1);	AddLib("xx   o oo o",   28,1);
	AddLib("xx   o o o ",   27,1);	AddLib("xx   o  oo ",   27,1);
	AddLib("xxo  o o   ",    7,1);	AddLib("xxo  o oo  ",   85,1);
	AddLib("xxo  o ooo ", 1000,1);	AddLib("xxo  o oo o",   28,1);
	AddLib("xxoo o oo  ",   28,1);	AddLib("xxoo o ooo ", 1000,1);
	AddLib("xxoo o  o o",   28,1);	AddLib("xxoo o    o",   28,1);
	AddLib("xxoo o   oo",   28,1);	AddLib("xxoo o   o ",   28,1);
	AddLib("xx o o o   ",   27,1);	AddLib("xx o o oo  ",   28,1);
	AddLib("xx o o ooo ", 1000,1);	AddLib("xx o o o oo",   27,1);
	AddLib("xx   o oo x",   85,1);	AddLib("xxoo o    x",   28,1);
	AddLib("xx o o    x",    7,1);	AddLib("xx   o ooox", 1000,1);
	AddLib("xx   o o  x",    7,1);	AddLib("xx   o o ox",   27,1);
	AddLib("xxo  o ooox", 1000,1);	AddLib("xxo  o oo x",   85,1);
	AddLib("xxo  o o ox",   27,1);	AddLib("xxoo o ooox", 1000,1);
	AddLib("xxoo o oo x",   28,1);	AddLib("xxoo o o ox",   27,1);
	AddLib("xx o o ooox", 1000,1);	AddLib("xx o o oo x",   28,1);
	AddLib("xx o o o ox",   27,1);	AddLib("xx   o o xx",    5,1);
	AddLib("xx   o ooxx",   28,1);	AddLib("xx o o   xx",    5,1);
	AddLib("xxoo o   xx",   28,1);	AddLib("xx o o o xx",   27,1);
	AddLib("xxoo o  xxx",   28,1);	AddLib("xx o o oxxx",   27,1);
	AddLib("xxoo o xxxx",   28,1);	AddLib("xxxo o     ",    5,1);
	AddLib("xxx  o o   ",    5,1);	AddLib("xxx  o oo  ",   28,1);
	AddLib("xxx  o ooo ", 1000,1);	AddLib("xxx  o oo o",   28,1);
	AddLib("xxx  o o o ",   27,1);	AddLib("xxx  o  oo ",   27,1);
	AddLib("xxxo o o   ",   27,1);	AddLib("xxxo o ooo ", 1000,1);
	AddLib("xxxo o  oo ",   27,1);	AddLib("xxx  o o  x",    5,1);
	AddLib("xxx  o oo x",   85,1);	AddLib("xxx  o ooox", 1000,1);
	AddLib("xxx  o o ox",   27,1);	AddLib("xxx  o  oox",   27,1);
	AddLib("xxxo o o  x",   27,1);	AddLib("xxxo o oo x",   28,1);
	AddLib("xxxo o ooox", 1000,1);	AddLib("xxxo o o ox",   50,1);
	AddLib("xxxo o  oox",   27,1);	AddLib("xxx  o o xx",    5,1);
	AddLib("xxx  o ooxx",   28,1);	AddLib("xxxo o o xx",   27,1);
	AddLib("xxxo o oxxx",   27,1);	AddLib("xxxx o oo  ",   28,1);
	AddLib("xxxx o ooo ", 1000,1);	AddLib("xxxx o o o ",   27,1);
	AddLib("xxxx o  oo ",   27,1);	AddLib("xxxx o oo x",   28,1);
	AddLib("xxxx o ooox", 1000,1);	AddLib("xxxx o o ox",   27,1);
	AddLib("xxxx o  oox",   27,1);	AddLib("xxxx o oox ",   28,1);
	AddLib("xxxxxo oo  ",   28,1);	AddLib("xxxxxo ooo ", 1000,1);
	AddLib("xxxxxo oo x",   28,1);	AddLib("xxxxxo ooox", 1000,1);

	//TWO DOL
	AddLib("    oo     ",   10,2);	AddLib("  o oo     ",   85,2);
	AddLib(" oo oo     ", 1005,2);  AddLib("o o oo     ",   28,2);
	AddLib("o   oo     ",   10,2);	AddLib("    oo o   ",   85,2);
	AddLib("    oo oo  ", 1005,2);	AddLib("    oo o o ",   28,2);
	AddLib("oo  oo     ",   10,2);  AddLib("    oo  oo ",   10,2);
	AddLib("    oo  o  ",   14,2); 	AddLib("  o oo oo  ", 1005,2);
	AddLib("  o oo  o  ",   85,2);	AddLib("  o oo  oo ",   85,2);
	AddLib(" o  oo     ",   14,2);	AddLib("  o oo o o ",   28,2);
	AddLib("  o oo   o ",   85,2);	AddLib(" oo oo o   ", 1005,2);
	AddLib(" oo oo oo  ", 5000,2);	AddLib(" oo oo o o ", 1005,2);
	AddLib(" oo oo  o  ", 1005,2);	AddLib(" oo oo  oo ", 1005,2);
	AddLib(" oo oo   o ", 1005,2);	AddLib("ooo oo o   ",   28,2);
	AddLib("ooo oo oo  ", 1005,2);	AddLib("ooo oo  o  ",   14,2);
	AddLib("o o oo oo  ", 1005,2);	AddLib("o o oo  o  ",   28,2);
	AddLib("o o oo   o ",   28,2);	AddLib(" o  oo o   ",   85,2);
	AddLib(" o  oo oo  ", 1005,2);	AddLib(" o  oo  o  ",   14,2);
	AddLib(" o  oo  o  ",   14,2);	AddLib("oo  oo o   ",   85,2);
	AddLib("oo  oo oo  ", 1005,2);	AddLib("oo  oo  o  ",   14,2);
	AddLib("o   oo o   ",   85,2);	AddLib("o   oo oo  ", 1005,2);
	AddLib("o   oo  o  ",   14,2);	AddLib("    oo   x ",   10,2);
	AddLib("  o oo   x ",   85,2);	AddLib(" oo oo   x ", 1005,2);
	AddLib("o o oo   x ",   28,2);	AddLib(" o  oo   x ",   14,2);
	AddLib("o   oo   x ",   10,2);	AddLib("    oo o x ",   85,2);
	AddLib("    oo oox ", 1005,2);	AddLib("    oo  ox ",   14,2);
	AddLib("  o oo o x ",   28,2);	AddLib("  o oo oox ", 1005,2);
	AddLib("  o oo  ox ",   85,2);	AddLib(" oo oo o x ", 1005,2);
	AddLib(" oo oo oox ", 5000,2);	AddLib(" oo oo  ox ", 1005,2);
	AddLib("o o oo oox ", 1005,2);	AddLib("o o oo  ox ",   28,2);
	AddLib(" o  oo o x ",   85,2);	AddLib(" o  oo oox ", 1005,2);
	AddLib(" o  oo  ox ",   14,2);	AddLib("oo  oo oox ", 1005,2);
	AddLib("oo  oo o x ",   85,2);	AddLib("oo  oo  ox ",   14,2);
	AddLib("o   oo o x ",   85,2);	AddLib("o   oo oox ", 1005,2);
	AddLib("o   oo  ox ",   14,2);	AddLib("  o oo  xx ",   85,2);
	AddLib(" oo oo  xx ", 1005,2);	AddLib(" o  oo  xx ",   14,2);
	AddLib("o o oo  xx ",   28,2);	AddLib("oo  oo  xx ",   10,2);
	AddLib("o   oo  xx ",   10,2);	AddLib("    oo oxx ",   28,2);
	AddLib("  o oo oxx ",   28,2);	AddLib(" oo oo oxx ", 1005,2);
	AddLib(" o  oo oxx ",   28,2);	AddLib("oo  oo oxx ",   28,2);
	AddLib("o   oo oxx ",   28,2);	AddLib("    oo  xx ",   10,2);
	AddLib("    oo xxx ",   10,2);	AddLib("  o oo xxx ",   85,2);
	AddLib(" oo oo xxx ", 1005,2);	AddLib("o o oo xxx ",   28,2);
	AddLib("  o ooxxxx ",   28,2);	AddLib(" oo ooxxxx ", 1005,2);
	AddLib(" o  ooxxxx ",   14,2);	AddLib("x   oo     ",   10,2);
	AddLib("xoo oo     ", 1005,2);	AddLib("xo  oo     ",   14,2);
	AddLib("x o oo     ",   85,2);	AddLib("x   oo o   ",   10,2);
	AddLib("x   oo oo  ", 1005,2);	AddLib("x   oo  o  ",   14,2);
	AddLib("x   oo o o ",   28,2);	AddLib("x   oo   o ",   10,2);
	AddLib("x o oo o   ",   28,2);	AddLib("x o oo oo  ",   10,2);
	AddLib("x o oo  o  ",   85,2);	AddLib("x o oo o o ",   28,2);
	AddLib("x o oo  oo ",   85,2);	AddLib("x o oo   o ",   85,2);
	AddLib("xoo oo o   ", 1005,2);	AddLib("xoo oo oo  ", 5000,2);
	AddLib("xoo oo o o ", 1005,2);	AddLib("xoo oo  o  ", 1005,2);
	AddLib("xoo oo  oo ", 1005,2);	AddLib("xoo oo   o ", 1005,2);
	AddLib("xoo oo ooo ", 1005,2);	AddLib("xo  oo o   ",   85,2);
	AddLib("xo  oo oo  ", 1005,2);	AddLib("xo  oo o o ",   28,2);
	AddLib("xo  oo  o  ",   10,2);	AddLib("xo  oo  oo ",   14,2);
	AddLib("xo  oo   o ",   14,2);	AddLib("x   oo   x ",   10,2);
	AddLib("x o oo   x ",   85,2);	AddLib("xoo oo   x ", 1005,2);
	AddLib("xo  oo   x ",   14,2);	AddLib("x   oo o x ",   85,2);
	AddLib("x   oo oox ", 1005,2);	AddLib("x   oo  ox ",   14,2);
	AddLib("x o oo o x ",   28,2);	AddLib("x o oo oox ", 1005,2);
	AddLib("x o oo  ox ",   85,2);	AddLib("xoo oo o x ", 1005,2);
	AddLib("xoo oo oox ", 5000,2);	AddLib("xoo oo  ox ", 1005,2);
	AddLib("xo  oo oox ", 1005,2);	AddLib("xo  oo o x ",   85,2);
	AddLib("xo  oo  ox ",   14,2);  AddLib("x   oo  xx ",   10,2);
	AddLib("x o oo  xx ",   85,2);	AddLib("xoo oo  xx ", 1005,2);
	AddLib("xo  oo  xx ",   14,2);	AddLib("x   oo oxx ",   28,2);
	AddLib("x o oo oxx ",   14,2);	AddLib("xoo oo oxx ", 1005,2);
	AddLib("xo  oo oxx ",   14,2);	AddLib("x   oo xxx ",   10,2);
	AddLib("x o oo xxx ",   85,2);	AddLib("xoo oo xxx ", 1005,2);
	AddLib("xo  oo xxx ",   14,2);	AddLib("x   ooxxxx ",    3,2);
	AddLib("x o ooxxxx ",   14,2);	AddLib("xoo ooxxxx ", 1005,2);
	AddLib("xo  ooxxxx ",   14,2);	AddLib("xx  oo     ",   10,2);
	AddLib("xxo oo     ",   14,2);	AddLib("xx  oo o   ",   85,2);
	AddLib("xx  oo oo  ", 1005,2);	AddLib("xx  oo  o  ",   14,2);
	AddLib("xx  oo o o ",   14,2);	AddLib("xx  oo   o ",    7,2);
	AddLib("xxo oo o   ",   14,2);	AddLib("xxo oo oo  ", 1005,2);
	AddLib("xxo oo  o  ",   14,2);	AddLib("xx  oo   x ",   10,2);
	AddLib("xxo oo   x ",   14,2);	AddLib("xx  oo o x ",   85,2);
	AddLib("xx  oo oox ", 1005,2);	AddLib("xx  oo  ox ",   14,2);
	AddLib("xxo oo o x ",   14,2);	AddLib("xxo oo oox ", 1005,2);
	AddLib("xxo oo  ox ",   14,2);	AddLib("xx  oo  xx ",   10,2);
	AddLib("xxo oo  xx ",   14,2);	AddLib("xx  oo oxx ",   28,2);
	AddLib("xx  oo xxx ",    3,2);	AddLib("xxo oo xxx ",   28,2);
	AddLib("xxx oo     ",   10,2);	AddLib("xxx oo o   ",   85,2);
	AddLib("xxx oo oo  ", 1005,2);	AddLib("xxx oo  o  ",   14,2);
	AddLib("xxx oo o o ",   28,2);	AddLib("xxx oo   x ",   10,2);
	AddLib("xxx oo  ox ",   14,2);	AddLib("xxx oo oox ", 1005,2);
	AddLib("xxx oo o x ",   85,2);	AddLib("xxx oo  xx ",    3,2);
	AddLib("xxx oo oxx ",   14,2);	AddLib("xxxxoo     ",    3,2);
	AddLib("xxxxoo oo  ", 1005,2);	AddLib("xxxxoo  o  ",   14,2);
	AddLib("xxxxoo o   ",   14,2);	AddLib("xxxxoo   x ",    3,2);
	AddLib("xxxxoo oox ", 1005,2);	AddLib("xxxxoo  ox ",   14,2);
	AddLib("xxxxoo o x ",   14,2);

	//THREE DOL
	AddLib("   ooo     ",  100,3); 	AddLib(" o ooo     ", 1010,3);
	AddLib("oo ooo     ",   30,3); 	AddLib("o  ooo     ",  100,3);
	AddLib("   ooo o   ", 1010,3); 	AddLib("   ooo oo  ",   30,3);
	AddLib("   ooo  o  ",  100,3); 	AddLib(" o ooo o   ", 5000,3);
	AddLib(" o ooo oo  ", 1010,3); 	AddLib(" o ooo  o  ", 1010,3);
	AddLib("oo ooo o   ", 1010,3); 	AddLib("o  ooo o   ", 1010,3);
	AddLib("o  ooo  o  ",   30,3);  AddLib("   ooo  x  ",  100,3);
	AddLib(" o ooo  x  ", 1010,3);  AddLib("oo ooo  x  ",   30,3);
	AddLib("o  ooo  x  ",  100,3); 	AddLib("   ooo ox  ", 1010,3);
	AddLib(" o ooo ox  ", 5000,3);  AddLib("oo ooo ox  ", 1010,3);
	AddLib("o  ooo ox  ", 1010,3); 	AddLib("   ooo xx  ",  100,3);
	AddLib("o  ooo xx  ",   30,3);  AddLib(" o ooo xx  ", 1010,3);
	AddLib("   oooxxx  ",   30,3); 	AddLib(" o oooxxx  ", 1010,3);
	AddLib("x  ooo     ",  100,3); 	AddLib("xo ooo     ", 1010,3);
	AddLib("x  ooo o   ", 1010,3); 	AddLib("x  ooo oo  ",   30,3);
	AddLib("x  ooo  o  ",  100,3); 	AddLib("xo ooo oo  ", 1010,3);
	AddLib("xo ooo  o  ", 1010,3); 	AddLib("xo ooo o   ", 5000,3);
	AddLib("xx ooo  o  ",   30,3); 	AddLib("xx ooo o   ", 1010,3);
	AddLib("xx ooo  x  ",  100,3); 	AddLib("xx ooo ox  ", 1010,3);
	AddLib("xx ooo xx  ",   30,3);  AddLib("xxxooo     ",   30,3);
	AddLib("xxxooo o   ", 1010,3);  AddLib("xxxooo  x  ",   30,3);
	AddLib("xxxooo ox  ", 1010,3);

	// FOUR DOL
	AddLib("  oooo     ", 5000,4);	AddLib("o oooo     ", 1030,4);
	AddLib("  oooo o   ", 1030,4);	AddLib("x oooo     ", 5000,4);
	AddLib("  oooo x   ", 5000,4);	AddLib("o oooo x   ", 1030,4);
	AddLib("  ooooxx   ", 1030,4);	AddLib("x oooo x  ",  5000,4);
	AddLib("x oooo o   ", 1030,4);	AddLib("x ooooxx   ", 1030,4);
	AddLib("xxoooo     ", 1030,4);	AddLib("xxoooo x   ", 1030,4);

}

void rect(int x,int y)
{
    setcolor(BLUE);
    rectangle(x-15,y-15,x+15,y+15);
    rectangle(x-16,y-16,x+16,y+16);
    setcolor(WHITE);
}

void Tcheck(void)
{
    int cx,cy,mx,my;
    int com,man;

    cx = 0; cy=0; mx =0; my = 0;

    com = decision(cx,cy,COMPUTER);
    man = decision(mx,my,MAN);

    if(com == OK) { pox = cx; poy = cy; }
    else if(man == OK) { pox = mx; poy = my; }
	 else if(com >= 1100) { pox = cx; poy = cy; }
	      else if(man >= 1100) { pox = mx; poy = my; }
		   else if(com >= 100) { pox = cx;poy = cy; }
			else if(man >= 100 && man < 1030) { pox=mx;poy=my; }
			     else if(com >4) { pox = cx;poy = cy; }
				  else { pox = mx; poy = my;  }
}

int decision(int &x,int &y,int mode)
{
   int i,j,value,oldvalue,loop,mode2;
   char *buf;

   oldvalue = 0;
   if(mode == MAN) mode2 =COMPUTER;
   else mode2 = MAN;

   for(i=0;i<XLEN;i++) {
       for(j=0;j<YLEN;j++) {
	   if(TABLE[i][j] == -1) {
	      value = dolvalue(i,j,mode);
	      if(oldvalue <= (value -3)) {
		  x = i; y = j;
		  if(value == OK) return OK;
		  oldvalue = value;
	      }
	      else if((oldvalue > (value-3)) && (oldvalue < (value + 3)) && (value !=0)) {
		      if(dolvalue(x,y,mode2) < dolvalue(i,j,mode2)) {
			  x = i; y=j;
		      }
		   }
	   }
       }
   }

   loop = TRUE;

   if(oldvalue == 0) {
       while(loop) {
	 randomize();
	 x =random(15);
	 randomize();
	 y =random(15);

	 if(TABLE[x][y] == -1) loop =FALSE;
       }
   }

   return oldvalue;
}

int dolvalue(int x,int y,int mode)
{
   int ch1,ch2,ch3,ch4;
   int value,samsam = 0,bsam =0;

   ch1=dolcount(x,y,1,0,mode);
   ch2=dolcount(x,y,0,1,mode);
   ch3=dolcount(x,y,1,1,mode);
   ch4=dolcount(x,y,1,-1,mode);

   value = ch1+ch2+ch3+ch4;

   if(ch1 == 100) samsam++;
   else if(ch1 == 85) {
	     ch1 += 20;
	     bsam ++;
	}

   if(ch2 == 100) samsam++;
   else if(ch2 == 85) {
	   ch2 += 20;
	   bsam ++;
	}

   if(ch3 == 100) samsam++;
   else if(ch3 == 85) {
	    bsam ++;
	    ch3 += 20;
	}

   if(ch4 == 100) samsam++;
   else if(ch4 == 85) {
	    bsam ++;
	    ch4 += 20;
	}

   if(bsam >= 2) {
	if(value < 1030) value = 500;
    }

   if(!(ch1 >100 || ch2 >100 || ch3 >100 || ch4 >100)) {
      if(samsam >= 2) value = SAM2;
   }

   if((bsam > 0) && (samsam >0)) {
	if(value < 1030) value = 500;
   }

   if(ch1==OK|| ch2== OK ||ch3 == OK|| ch4 ==OK) return OK;

   return value;
}

int dolcount(int x,int y,int dx,int dy,int mode)
{
   int i,j,k,chk;
   char dolBuf[11];
   int first,end,oldDol;
   int value,sum,total;
   int mode2 ,loop,Flag;

   oldDol = TABLE[x][y];
   TABLE[x][y] = mode;

   loop = TRUE; sum =0;total=0;
   i = 0; j = 0,k=5,value =0; chk =TRUE;

   mode2 =COMPUTER;

   if(mode == COMPUTER)  mode2 =MAN;

// setmem(dolBuf,11,'x');
   memset(dolBuf,'x', 11);
   while(loop) {

	  if(check2(x+i,y+j)) {
	     if(chk) first=k;
	     loop = FALSE;
	  }
	  else {
	    if(TABLE[x+i][y+j] == -1) {
		if(chk) first =k;
		chk =FALSE;
		dolBuf[k]=' ';
	    }
	    else if(TABLE[x+i][y+j]==mode) {
		    if(chk) sum += 1;
		    total += 1;
		    dolBuf[k]='o';
		 }
		 else if(TABLE[x+i][y+j]==mode2) {
			 if(chk) first =k;
			 dolBuf[k]='x';
			 loop = FALSE;
		      }
	  }

	  if(k == 0) loop = FALSE;

	  k --;
	  i += dx; j += dy;
   }

   loop = TRUE;
   i = 0; j =0; k =5; chk = TRUE;

   while(loop) {
	  k ++;
	  i -= dx; j -= dy;

	  if(check2(x+i,y+j)) {
	     if(chk) end=k;
	     loop = FALSE;
	  }
	  else  {
		if(TABLE[x+i][y+j]== -1) {
		       if(chk) end = k;
		       chk = FALSE;
		       dolBuf[k]=' ';
		}
		else if(TABLE[x+i][y+j]==mode) {
		       if(chk) sum += 1;
		       total += 1;
		       dolBuf[k]='o';
		     }
		     else if(TABLE[x+i][y+j]==mode2) {
			     if(chk) end =k;
			     dolBuf[k]='x';
			     loop =FALSE;
			  }
	  }

	  if(k == 10) loop = FALSE;
   }


   if(total == 1) {
       TABLE[x][y] = oldDol;
       return 0;
   }

   if(sum > 5) {
      TABLE[x][y] =oldDol;
      return 0;
   }

   if(sum == 5) {
      TABLE[x][y] = oldDol;
      return OK;
   }
   else {
      buff(first,end,sum);

      for(i=0; i<libCount[sum-1]; i++) {

	  k = 0;
	  loop = TRUE;
	  Flag=TRUE;

	  while(loop) {
	      if(dolBuf[first+k] != lib[sum-1][i].dol[k]) {
		    Flag=FALSE;
		    loop = FALSE;
	      }

	      if( (first+k) == end) loop = FALSE;
	      k++;
	  }

	  if(Flag==TRUE) {
	      value = lib[sum-1][i].value;
	      break;
	  }
      }
   }

   TABLE[x][y] =oldDol;
   return value;
}

void buff(int &a,int &b,int c)
{
      if(c ==1) { a=a-4; b=b+4; }
      else if(c == 2) { a=a-3;b=b+3;}
	   else if(c==3) {a -= 2; b+=2; }
		else if(c==4) {a--; b++; }
}

void Error(int i)
{
    switch(i) {
       case 1 : settextstyle(DEFAULT_FONT, HORIZ_DIR,1);
		outtextxy(493,370,"SAM-SAM ERROR");
		delay(1030);
		setwritemode(COPY_PUT);
		setfillstyle(SOLID_FILL,LIGHTBLUE);
		bar(490,368,620,400);
		setwritemode(XOR_PUT);
		break;
       case 2 : settextstyle(DEFAULT_FONT, HORIZ_DIR,1);
		outtextxy(493,370,"TIME ERROR");
		delay(1030);
		setwritemode(COPY_PUT);
		setfillstyle(SOLID_FILL,LIGHTBLUE);
		bar(490,368,620,400);
		setwritemode(XOR_PUT);
		break;
       default: break;
    }
}

int change(int mode)
{
   if(mode == MAN) {
	mode =COMPUTER;
   } else mode =MAN;

   return mode;
}

void dol(int x,int y,int mode)
{
    int color;

    if(mode == COMPUTER) { setcolor(YELLOW); color = YELLOW; }
    else {setcolor(WHITE); color = WHITE; }

    setfillstyle(SOLID_FILL,color);
    fillellipse(x,y,14,14);

    setcolor(WHITE);

    count ++;
}

int check(int x,int y)
{
   if((x<MIN)||(y<MIN)||(x>MAX)||(y>MAX)) return(FALSE);
   else return(TRUE);
}

int check2(int x,int y)
{
   if(x<0||y<0||x>=XLEN||y>=YLEN) return(TRUE);
   else return(FALSE);
}

void end(void)
{
    closegraph();
}

void ButtonRect(int x1,int y1,int x2,int y2,int State)
{
   setcolor(WHITE);
   rectangle(x1,y1,x2,y2);
   if(State == 1) {
      setcolor(BLACK);
      line(x2,y1,x2,y2);
      line(x1,y2,x2,y2);
   }
   else {
      setcolor(BLACK);
      line(x1,y1,x2,y1);
      line(x1,y1,x1,y2);
   }
   return;
}

void ScrollUp(int x1,int y1,int x2,int y2,int num)
{
   union REGS reg;

   reg.h.ah = 0x06;
   reg.h.al = num;
   reg.h.ch = y1;
   reg.h.cl = x1;
   reg.h.dh = y2;
   reg.h.dl = x2;
   reg.h.bh = BLACK;
   int86(0x10, &reg, &reg);
   return;
}

void ScrollDown(int x1,int y1,int x2,int y2,int num)
{
   union REGS reg;

   reg.h.ah = 0x07;
   reg.h.al = num;
   reg.h.ch = y1;
   reg.h.cl = x1;
   reg.h.dh = y2;
   reg.h.dl = x2;
   reg.h.bh = BLACK;
   int86(0x10, &reg, &reg);
   return;
}



