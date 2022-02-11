// KEYSCAN_C ////////////////////////////////////////////////////////////////

// Ronny H./ Thomas H.

// INCLUDES /////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#include "keyscan.h"

// DEFINES //////////////////////////////////////////////////////////////////

// EXTERNALS ////////////////////////////////////////////////////////////////

extern void interrupt newkbdhandler(...);
extern void interrupt (*oldkbdhandler)(...);
extern byte *pressed;

// FUNCTIONS ////////////////////////////////////////////////////////////////

void interrupt newkbdhandler(...)
{
	char scan=inportb(0x60);
	oldkbdhandler();
	pressed[scan&0x7f]=!(scan&0x80);
	int head=peek(0x40,0x1a);
	poke(0x40,0x1c,head);
}

// METHODS //////////////////////////////////////////////////////////////////

// CONSTRUCTOR

keyscan_C::keyscan_C()
{
	pressed=new byte[128];
	for(int count=0; count<128; count++)
		pressed[count]=0;
	oldkbdhandler=getvect(0x09);
	setvect(0x09,newkbdhandler);
	pokeb(0,0x417,peekb(0,0x417)&223);
	pokeb(0,0x0500,1);
}

// DESTRUCTOR

keyscan_C::~keyscan_C()
{
  if (pressed)
	  delete pressed;
  if (oldkbdhandler)
	  setvect(0x09,oldkbdhandler);
}


