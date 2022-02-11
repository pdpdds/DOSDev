// VX_C

// A class implementing a virtual 320x240x16/160x240x256 screen in mode x.

#include "global.h"

// DEFINES:

#define VRAMSIZE     	19200

// VGA-registers:

#define AC_INDEX        0x03c0
#define MISC_OUTPUT     0x03c2
#define SC_INDEX        0x03c4
#define GC_INDEX	0x03ce
#define CRTC_INDEX      0x03d4
#define INPUT_STATUS_0  0x03da
#define MAP_MASK	0x02
#define READ_MAP        0x04
#define BIT_MASK	0x08

// MACROS:

#define FROMLATCHES 	asm mov dx,GC_INDEX; asm mov ax,BIT_MASK;asm out dx,ax
#define FROMREGISTER 	asm mov dx,GC_INDEX+1;asm mov al,0xff;asm out dx,al

// CLASS DECLARATION:

class vx_C
{
  word offset;
public:
	void setoffset(word newoffset);
  void frame(void);
  void writepixel(word pos, byte plane, byte color);
  void putpixel(word x,word y,byte color)
    {writepixel(offset+x/4+80*y,x&3,color);};
  byte getpixel(word x,word y);
  void setrgb(byte color, byte r, byte g, byte b);
  void blockfill(word start,word end, byte data);
};