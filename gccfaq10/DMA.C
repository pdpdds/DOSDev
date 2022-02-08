#include "dma.h"

#define TIMER

struct WAVHeaderStruct WAVHeader;
struct SoundStruct Sound;
struct DMAStruct DMA;
struct WAVStruct WAV;

static unsigned short dos_segment;
static unsigned int source, dest, length, blank_length;

int main(int argc, char *argv[])
{
 char c;
 #if !defined TIMER
  int count;
 #endif
 
 printf("go32 a.out [filename].wav\n");

 c = 0;

 DMA.stat = 0;
 dos_segment = _go32_conventional_mem_selector();
 
 /* Sound blaster settings. Sound.dma is always 1 */

 Sound.port = 0x220;
 Sound.irq = 5;
 Sound.dma = 1;
 
 /* Chain prior to malloc */

 DMAChain();

 if (!WAVLoad(argv[1])) {
  printf("Could not load WAV file.\n");
  DMAShutDown();
  exit(-1);
 }

 if (!DMAInit()) {
  printf("Cound not init DMA/SB.\n");
  DMAShutDown();
  exit(-1);
 }

 printf("Press [RETURN] to play, any other key to exit.\n");

 /* Loop */

 while (!c) {
  #if defined TIMER
   while(!kbhit());
  #else
   while(!kbhit()) {
    if (rawclock()!=count) {
     count = rawclock();
     DMAPoll();
   }}
  #endif
  
  c = getkey();
  if (c == 13) {
   c = 0;
   DMAPlayWAV();
 }}

 /* Cleanup */

 DMAShutDown();

 free(WAV.offset);
 
 return(0);
}

int WAVLoad(char *filename)
{
 FILE *fp;

 if (!(fp=fopen(filename, "rb"))) return(FALSE);

 if (!(fread(&WAVHeader, sizeof (WAVHeader), 1, fp))) return(FALSE);

 if (strncmp(WAVHeader.did, "data", 4) ||
     WAVHeader.channels != 1) return(FALSE);

 WAV.length = WAVHeader.dlen;
 DMA.wav_frequency = WAVHeader.frequency;

 if ((WAV.offset = malloc (WAV.length)) == NULL) return(FALSE);

 if (!(fread(WAV.offset, WAV.length, 1, fp))) return(FALSE);

 fclose(fp);

 return(TRUE);
}

void DMAPlayWAV(void)
{
 asm ("cli");

 DMA.wav_offset = (int) WAV.offset;
 DMA.wav_length = WAV.length;

 asm ("sti");

 if (!(DMA.stat & DMA_RUNNING)) DMAStart();
}
 

void DMAChain()
{
 _go32_dpmi_seginfo handler;

 /* Chain sb irq */

 handler.pm_offset = (int) DMAInterrupt;
 handler.pm_selector = _go32_my_cs();
 
 _go32_dpmi_get_protected_mode_interrupt_vector(8 + Sound.irq, &DMA.old_irq_handler);
 _go32_dpmi_chain_protected_mode_interrupt_vector(8 + Sound.irq, &handler);
 
 /* Chain timer */
 
 #if defined TIMER
  handler.pm_offset = (int) DMAPoll;

  _go32_dpmi_get_protected_mode_interrupt_vector(8, &DMA.old_timer_handler);
  _go32_dpmi_chain_protected_mode_interrupt_vector(8, &handler);
 #endif

 DMA.stat |= DMA_CHAINED;
}

int DMAInit(void)
{
 int i;
 unsigned char major, minor;
 unsigned int addr, page;
 _go32_dpmi_seginfo handler;
 
 /* Init */

 DMA.wav_length = 0;

 /* Allocate some DOS memory for the DMA buffers */

 DMA.dos_memory.size = (DMA_SIZE * 2) / 16;
 if (_go32_dpmi_allocate_dos_memory(&DMA.dos_memory)) return(FALSE);
 DMA.stat |= DMA_MEMORY_ALLOCATED;
 
 /* What is the 20 bit address? */

 addr = DMA.dos_memory.rm_segment << 4;

 /* See if we cross any 64K boundarys */
  
 page = addr & 0xffff;
 if ((page + DMA_SIZE) > 0xffff) addr = (addr - page) + 0x10000;

 DMA.buffer_offset = addr;

 /* Reset SB DSP */

 inportb(Sound.port+0xa);
 outportb(Sound.port+0x6, 1);
 inportb(Sound.port+0x6);
 inportb(Sound.port+0x6);
 inportb(Sound.port+0x6);
 inportb(Sound.port+0x6);
 outportb(Sound.port+0x6, 0);                                      

 i = 0;
 for (i = 0; i < 100; i++) {
  if (inportb(Sound.port+0xe) & 0x08) {
   if (inportb(Sound.port+0xa) == 0xaa) break;
 }}

 if (i == 100) return(FALSE);

 DMA.stat |= SB_ACTIVE;
 
 /* Get DSP version, not used. */

 WriteDSP(0xe1);
 major = ReadDSP();
 minor = ReadDSP();

 /* Enable interrupts on PIC */

 major = inportb(0x21);
 minor = ~(1 << Sound.irq);
 outportb(0x21, major & minor);

 /* Set Time Constant */

 inportb(Sound.port+0xe);
 WriteDSP(0x40);
 WriteDSP(256-(1000000/DMA.wav_frequency));

 /* SpkOn */

 WriteDSP(0xd1);
  
 /* Silence in buffer */

 length = DMA_SIZE / 4;
 dest = DMA.buffer_offset;

 asm ("
  pushw %es
  
  movw _dos_segment, %es
  movl _length, %ecx
  movl _dest, %edi
  movl $0x80808080, %eax
   
  rep ; stosl
  
  popw %es
 ");

 return(TRUE);
}

void DMAShutDown(void)
{
 /* SpkOff */

 if (DMA.stat & SB_ACTIVE) WriteDSP(0xd3);

 /* Stop any DMA transfer */

 DMAStop();
 
 /* Free memory */

 if (DMA.stat & DMA_MEMORY_ALLOCATED) {
  _go32_dpmi_free_dos_memory(&DMA.dos_memory);
  DMA.stat -= DMA_MEMORY_ALLOCATED;
 }
  
 /* Unchain interrupts */

 if (DMA.stat & DMA_CHAINED) {
  #if defined TIMER
   _go32_dpmi_set_protected_mode_interrupt_vector(8, &DMA.old_timer_handler);
  #endif
  _go32_dpmi_set_protected_mode_interrupt_vector(8 + Sound.irq, &DMA.old_irq_handler);
  DMA.stat -= DMA_CHAINED;
 }
}

void WriteDSP(char data)
{
/* int i;

 for (i = 0;(inportb(Sound.port+0xc) & 0x80) && i < DSP_TIMEOUT; i++);
*/
 while(inportb(Sound.port+0xc) & 0x80);
 outportb(Sound.port+0xc, data);
}

unsigned char ReadDSP(void)
{
/* int i;

 for (i = 0;(inportb(Sound.port+0xe) & 0x80) && i < DSP_TIMEOUT; i++);
*/
 while(!(inportb(Sound.port+0xe) & 0x80));
 return(inportb(Sound.port+0xa));
}

void DMAInterrupt(void)
{
 asm ("cli;pusha");

 /* Acknowledge SB */

 inportb(Sound.port+0xe);
 
 /* Tell SB to accept next block */

 WriteDSP(0x14);
 WriteDSP(0xff);
 WriteDSP(0xff);
 
 /* Acknowledge interrupt */

 outportb(0x20, 0x20);
 asm ("popa;sti");
}

static unsigned short word;

short DMACount()
{
 asm ("
  inb  $03, %al
  movb %al, %cl
  inb  $03, %al
  movb %al, %ch
  movw %cx, _word
 ");
 return (DMA_SIZE - word);
}

void DMAStop()
{
 if (DMA.stat & DMA_RUNNING) {
  WriteDSP(0xd0);
  DMA.stat -= DMA_RUNNING;
 }
}

void DMAStart(void)
{
 unsigned int size;
 
 size = DMA_SIZE - 1;

 outportb(0x0a, 0x05);
 outportb(0x0c, 0x00);
 outportb(0x0b, 0x59);

 outportb(0x02, DMA.buffer_offset);
 outportb(0x02, DMA.buffer_offset >> 8);
 outportb(0x83, DMA.buffer_offset >> 16);

 outportb(0x03, size);
 outportb(0x03, size >> 8);
 outportb(0x0a, 0x01);

 WriteDSP(0x14);
 WriteDSP(0xff);
 WriteDSP(0xff);

 DMA.stat |= DMA_RUNNING;
 DMA.flag = TRUE;
 DMA.count = 0;
}

void DMAPoll(void)
{
 short count;
 
 if (DMA.stat & DMA_RUNNING) {
 
  count = DMACount();
  dest = 0;
 
  /* DMA should be running */

  if (DMA.count == count) {

   /* DMA has halted, restart it */

   if (DMA.flag) {
    DMAStart(); 
    count = 0;        /* Start filling 2nd 1/2 */
    DMA.flag = TRUE;
   } else {
    DMAStart();   
    count = DMA_SIZE; /* Start filling 1st 1/2 */
    DMA.flag = FALSE;
   }
  }

  DMA.count = count;

  /* What half of the buffer are we in? */

  if (DMA.flag) {
   if (count < DMA_SIZE/2) {
    dest = DMA.buffer_offset + DMA_SIZE/2;
    DMA.flag = FALSE;
   }
  } else {
   if (count >= DMA_SIZE/2) {
    dest = DMA.buffer_offset;
    DMA.flag = TRUE;
   }
  }
 
  /* Can we go ahead and fill 1/2 buffer? */

  if (dest) {
   
   /* Is there anything to fill it with? */
   
   length = DMA.wav_length;

   if (length) {

    if (length > DMA_SIZE/2) length = DMA_SIZE/2;

    source = DMA.wav_offset;

    DMA.wav_length -= length;
    DMA.wav_offset += length;
   }

   blank_length = (DMA_SIZE/2) - length;

   asm ("
    pushw %es

    movw _dos_segment, %es
    movl _dest, %edi
    movl _length, %ecx

    testl %ecx, %ecx
    jz no_sound

    movl _source, %esi

    rep ; movsb

   no_sound:

    movl _blank_length, %ecx

    testl %ecx, %ecx
    jz no_more

    movb $0x80, %al

    rep ; stosb
    
   no_more:

    popw %es
   ");
  } 
 }
}
