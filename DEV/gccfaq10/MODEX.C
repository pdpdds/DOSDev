#include "modex.h"
#include <dos.h>

void XInit()
{
 union REGS in, out;

 /* Call BIOS for mode 0x13 */

 in.x.ax = 0x13;
 int86(0x10, &in, &out);

 /* Unchain */

 outportw(0x3c4, 0x0604);
 outportw(0x3d4, 0xe317);
 outportw(0x3d4, 0x0014);
}

void XText()
{
 union REGS in, out;

 in.x.ax = 0x3;
 int86(0x10, &in, &out);
}

void XPlot(int x, int y, char c)
{
 outportb(0x3c4, 0x2);
 outportb(0x3c5, 1 << (x & 3));
 dosmemput(&c, 1, (X_BYTES * y) + (x / 4) + VIDEO_MEM);
}

int main()
{
 int x, y;

 XInit();

 for (x = 0; x < 256; x++) {
  for (y = 0; y < 100; y++) XPlot(x, y, x);
 }

 while (!kbhit());
 getkey();

 XText();
}
  

