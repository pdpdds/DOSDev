The following files are included:

GCCFAQ10.TXT
____________

FAQ of how to access PC hardware for games under DJGPP (GNU C Compiler).

DMA.C/H
_______

Example WAV player using DMA, mono and DMA channel 1 only. Currently
set for IRQ 5, Port 0x220. These are set at the top of the program.
No timeout included in Read/WriteDSP routines, program assumes that
if DSP can be reset, it will not hang.

Interesting bug - if the interrupts are chained after a malloc > 30K
program crashes on first interrupt (OS/2 + Win only). Interrupts are 
now chained prior to malloc, and no crash. Can anyone explain??

gcc dma.c -lpc
go32 a.out ni.wav

MODEX.C/H
_________

Draws a box in Mode-X 320x200x256. Not very exciting, but you get
the idea.

gcc modex.c -lpc
go32 a.out

MEMORY.C
________

Prints availible physical/virtual memory.

gcc memory.c
go32 a.out

VIDEO.C
_______

Writes 'Hello' to the video buffer using asm.

gcc video.c
go32 a.out

NI.WAV
______

Sample .WAV file which can be used with the above example.
