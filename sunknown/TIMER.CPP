// TIMER_C //////////////////////////////////////////////////////////////////

// Thomas H.

// INCLUDES /////////////////////////////////////////////////////////////////

#include <dos.h>
#include "timer.h"
#include "global.h"

#define INTR 0x1c
#define CLOCK_DIVISOR 9944U
#define TIMERFAC 8.35708687636071

// EXTERNALS ////////////////////////////////////////////////////////////////

// FUNCTIONS

extern "C" void smidigo(void);

// VARS

extern long ticks;
extern byte callmidi;
extern byte guson;

// VARS /////////////////////////////////////////////////////////////////////

void interrupt (*oldhandler)(...);

// FUNCTIONS ////////////////////////////////////////////////////////////////

// CONSTRUCTOR

timer_C::timer_C()
{
	outportb(0x43, 0x36);
	outportb(0x40, CLOCK_DIVISOR & 0xff);
	outportb(0x40, (CLOCK_DIVISOR & 0xff00) >> 8);

	oldhandler = getvect(INTR);
}

// DESTRUCTOR

timer_C::~timer_C()
{
	outportb(0x43,0x36);
	outportb(0x40,0x00);
	outportb(0x40,0x00);
  if (oldhandler)
	  setvect(INTR,oldhandler);
}

// READTIMER

long timer_C::readtimer(void)
{
	return ticks;
}

// ELAPSED

long timer_C::elapsed(unsigned long start,unsigned long stop)
{
	return ((float)stop-start)*TIMERFAC;
}
