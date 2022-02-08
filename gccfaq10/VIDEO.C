#include <go32.h>
   
static int dos_seg, length;
static char *string;

main()
{
 dos_seg = _go32_conventional_mem_selector();
 string  = "H.e.l.l.o.";
 length  = 10;

 asm ("
  pushw %es
  
  movw  _dos_seg, %es
  
  movl  _string, %esi
  movl  $0xb8000, %edi
   
  movw  _length, %cx
  
  rep ; movsb 
  
  popw  %es
 ");
}

