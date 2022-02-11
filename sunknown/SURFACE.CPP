// SURFACE.CPP //////////////////////////////////////////////////////////////

// Thomas H.

// INCLUDES /////////////////////////////////////////////////////////////////

#include <math.h>
#include <dos.h>
#include <stdio.h>

#include "global.h"
#include "rcglobal.h"
#include "xgraf.h"
#include "surface.h"
#include "model.h"
#include "timer.h"
#include "template.h"

// INCLUDES /////////////////////////////////////////////////////////////////

#define MAXX 440000
#define MINX   7000
#define CENTY 78500

// EXTERNALS ////////////////////////////////////////////////////////////////

extern timer_C timer;
extern byte *pressed;
extern word modelx,modely,modelv;
extern ulong musicstart;

// FUNCTIONS ////////////////////////////////////////////////////////////////

// SURFACE

void surface(void)
{
	model_C model;
	model.camangle(OVERVIEW);
	model.load(SURFACEFILE);

	float x,y,v,dv=0,dx=0,dy=0;

	x=70000;
	y=78500;
	v=-2048;

	dv=0;

	model.setpos(0,x,y);

	setpalette(SURFACEPAL);

	long frames=0;
	int done=0;

	ulong estart=timer.readtimer();
	ulong left=END_SURFA-timer.elapsed(musicstart,timer.readtimer());
  float ddv=0;

	do
	{
		long now=timer.elapsed(estart,timer.readtimer());
		float pro=pow((float)now/left,1.4);
		pro=0.17+pro*0.83;
		float vpro=pro*4;
		v=-2048+3048*(vpro)+dv;
		float maxf=pro*3000;
		x=MINX+(MAXX-MINX)*pro+cos(M_PI*2*vpro)*maxf;
		y=CENTY+sin(M_PI*2*vpro)*maxf;

		frames++;
		modelx=x;
		modely=y;
		modelv=v;

		model.player[0].rc->x=x;
		model.player[1].rc->x=x;
		model.player[0].rc->y=y;
		model.player[1].rc->y=y;
		model.cam[0].angle=v;
		model.cam[1].angle=v+ANGLES/2;

		model.draw();
		model.newpage();

		if (pressed[75])
			ddv-=0.5;
		if (pressed[77])
			ddv+=0.5;

    dv+=ddv;

		x+=dx;
    y+=dy;

		fit((float)32768.0,x,(float)((2048*256.0)-32768.0));
		fit((float)32768.0,y,(float)((1536*256.0)-32768.0));

		ulong gaatt=timer.elapsed(musicstart,timer.readtimer());
		done|=(gaatt>=END_SURFA);
	}while (!done);
	long p1=timer.readtimer();
	while (timer.elapsed(p1,timer.readtimer())<1000);
}