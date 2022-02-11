// MODEL.H

#ifndef _MODEL_H
#define _MODEL_H

#include "global.h"
#include "player.h"
#include "coor.h"
#include "obj.h"
#include "seg.h"

class cam_C
{
public:
	cam_C();
	seg_C area;

	long lastx,lasty;

	float maxl,camz,camx,maxz;

	byte bigpixels;
	byte fjellpaa,radaron;

	word fradarvin;
	word angle;
	word *ormask;
	word *wlen;
	word radarzoom;
	void setangle(void);
};

class model_C
{
	word *allormask;
	word *allwlen;
	float *maxilen,*minilen;
public:
	word screenseg,pageno;
	seg_C map;
	byte *oval_ptr,*o_ptr;
	byte colormap;
	byte view;
	byte players;
	player_C player[2];
	cam_C cam[2];
	coor_C coor[COORS];

  model_C();
  ~model_C();
  void draw(void);
	void drawover(long xpos, long ypos);
	void draw1pl(long xpos, long ypos);
	void draw2pl(long xpos, long ypos,int camno);
	void swapmap(byte *ptr,long nyx,long nyy, long& oldx,long& oldy);
	void load(char *filnavn);
	void calclen(float camz,float camx, float maxz, word camangle,word h);
	void newpage(void);
	void camangle(int nview);
	void screenoverview(int ikkedraw=0);
	void setpos(int camno,long x,long y);
};

void newhandler(void);

#endif