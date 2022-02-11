// SEG.H

#ifndef _SEG_H
#define _SEG_H

#include "global.h"

class seg_C
{
	byte far *ptr2;
	long seglen;
public:
	byte *ptr;
	seg_C(long len=65536l);
	~seg_C();
};

#endif

