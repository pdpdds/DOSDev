/* Sample program that initializes the SuperVGA driver */

#include <graphics.h>
#include <stdio.h>
#include <conio.h>

extern int far Svga16_fdriver[];
extern int far Svga256_fdriver[];
extern int far Twk16_fdriver[];
extern int far Twk256_fdriver[];

int huge DetectVGA256()
{
  int Vid;

  printf("Which video mode would you like to use? \n");
  printf("  0) 320x200x256\n");
  printf("  1) 640x400x256\n");
  printf("  2) 640x480x256\n");
  printf("  3) 800x600x256\n");
  printf("  4) 1024x768x256\n\n>");
  scanf("%d",&Vid);
  return Vid;
}

int huge DetectVGA16()
{
  int Vid;

  printf("Which video mode would you like to use? \n");
  printf("  0) 320x200x16\n");
  printf("  1) 640x200x16\n");
  printf("  2) 640x350x16\n");
  printf("  3) 640x480x256\n");
  printf("  4) 800x600x16\n");
  printf("  5) 1024x768x16\n\n>");
  scanf("%d",&Vid);
  return Vid;
}

int huge DetectTwk16()
{
  int Vid;

  printf("Which video mode would you like to use? \n");
  printf("  0) 704x528x16\n");
  printf("  1) 720x540x16\n");
  printf("  2) 736x552x16\n");
  printf("  3) 752x564x16\n");
  printf("  4) 768x576x16\n");
  printf("  5) 784x588x16\n");
  printf("  6) 800x600x16\n\n>");
  scanf("%d",&Vid);
  return Vid;
};

int huge DetectTwk256()
{
  int Vid;

  printf("Which video mode would you like to use? \n");
  printf("  0) 320x400x256\n");
  printf("  1) 320x480x256\n");
  printf("  2) 360x480x256\n");
  printf("  3) 376x564x256\n");
  printf("  4) 400x564x256\n");
  printf("  5) 400x600x256\n");
  scanf("%d",&Vid);
  return Vid;
};


int main(void)
{
  int Gd = DETECT, Gm;
  int Drv;

  printf("Which driver would you like to use?\n");
  printf("  0) Svga256\n");
  printf("  1) Svga16\n");
  printf("  2) Tweak256\n");
  printf("  3) Tweak16\n");
  printf(">");
  scanf("%d",&Drv);
  switch(Drv)
  {
    case 0: installuserdriver("Svga256",DetectVGA256);
/*  If driver is linked with file, remove comments */
/*          registerfarbgidriver(Svga256_fdriver); */
	    break;
    case 1: installuserdriver("Svga16",DetectVGA16);
/*  If driver is linked with file, remove comments */
/*          registerfarbgidriver(Svga16_fdriver);  */
	    break;
    case 2: installuserdriver("Twk16",DetectTwk16);
/*  If driver is linked with file, remove comments */
/*          registerfarbgidriver(Twk16_fdriver);  */
	    break;
    case 3: installuserdriver("Twk256",DetectTwk256);
/*  If driver is linked with file, remove comments */
/*          registerfarbgidriver(Twk256_fdriver);  */
  }
  initgraph(&Gd,&Gm,"");
  setcolor(15);
  line(0,0,getmaxx(),getmaxy());
  line(0,getmaxy(),getmaxx(),0);
  getch();
  closegraph();
  return(0);
}
