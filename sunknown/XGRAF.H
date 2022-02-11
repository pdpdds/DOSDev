// BASIC MODE X (320x240x256) FUNCTIONS

#ifndef _XGRAF_H
#define _XGRAF_H

#include "global.h"

void modex(int not13=0);
void textmode(void);
void frame(void);
void yoffset(int yoff);
void putpixel(int x,int y,byte color);
void putabspixel(word adr,byte plan, byte color, word sseg=0xa000);
void setrgb(byte color,byte red,byte green, byte blue);
unsigned char getpixel(int x,int y);
void setpalette(char *filename);
void updatepalette(byte *pal);
void switchpage(word from,word to,byte method);

#endif

