// KEYSCAN_C

#ifndef _KEYSCAN_H
#define _KEYSCAN_H

#define CTRL        29
#define ALT         56
#define LCTRL       100
#define RCTRL       101
#define LALT        102
#define RALT        103
#define NUMON       120
#define CAPSON      121
#define SCROLLON    122
#define INSON       123
#define KBDF1       59
#define KBDF2       60
#define KBDF3       61
#define KBDF4       62
#define KBDF5       63
#define KBDF6				64
#define KBDF7				65
#define KBDLEFT     75
#define KBDRIGHT    77
#define KBDUP       72
#define KBDDOWN     80
#define KBDPGUP     73
#define KBDPGDN     81

#define _LSHIFT     0x02
#define _LCTRL      0x0100
#define _LALT       0x0200
#define _RCTRL      0x0400
#define _RALT       0x0800

#include <dos.h>
#include <bios.h>
#include "global.h"

class keyscan_C{
public:
	keyscan_C();                   // Constructor
	~keyscan_C();                  // Destructor
};


#endif