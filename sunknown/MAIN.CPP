// SUPERUNKNOWN
//
// By Five, And Then Some.
//
// See the file SUNKNOWN.NFO for information regarding this source code.

// INCLUDES /////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <new.h>
#include <dos.h>
#include <conio.h>

#include "seg.h"
#include "midi.h"
#include "template.h"
#include "xgraf.h"
#include "timer.h"
#include "keyscan.h"
#include "voxel.h"
#include "vx.h"
#include "zoom.h"
#include "morph.h"
#include "dune.h"
#include "ifs.h"
#include "surface.h"
#include "flamscrl.h"
#include "eye.h"

// EXTERNALS ////////////////////////////////////////////////////////////////

extern void newhandler(void);
extern "C"
{
  void smidigo(void);
  void smidistart(void);
  void smidireset(void);
  void um_init_timers(void);
  void um_cleanup_timers(void);
}

// GLOBALS //////////////////////////////////////////////////////////////////

// STRUCTS

timer_C timer;
keyscan_C keyscan;
vx_C vx;

// SCALARS

ulong musicstart;
int relspeed;
long ticks,
     modelx,
     modely,
     modelv;
byte callmidi,
     loop,
     guson,
     *pressed;
void interrupt newkbdhandler(...);
void interrupt (*oldkbdhandler)(...);

// ARRAYS

byte rgb[768];

// FUNCTIONS ////////////////////////////////////////////////////////////////

// QUIT

void quit(int val=0)
{
	if (guson)
		smidireset();
  else
    um_cleanup_timers();
  exit(val);
}

// MAIN

void main(int argc, char **argv)
{
	set_new_handler(newhandler);
  int testspeed=usepara(argc,argv,guson);

  do
  {
	  if (guson)
		  smidistart();
    else
      um_init_timers();

	  findrelspeed();

    if (testspeed)
      showspeed();

	  musicstart=timer.readtimer();
	  callmidi=guson;

	  morph();
	  modex(1);
	  eye();
	  flamscrl();
	  zoom();
	  voxel();
	  ifs();
	  dune();
	  modex(1);
	  surface();

    if (guson)
		  smidireset();
    else
      um_cleanup_timers();

    for (int black=0; black<256; black++)
      setrgb(black,0,0,0);

  } while (loop);

  bye();
  quit();
}
