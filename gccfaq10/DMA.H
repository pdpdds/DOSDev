#include <pc.h>
#include <dos.h>
#include <go32.h>
#include <dpmi.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#define DMA_SIZE 2048
#define DMA_CHANNELS 4
#define DMA_DOS 0x21

#define DSP_TIMEOUT 2048

#define DMA_MEMORY_ALLOCATED 1
#define DMA_CHAINED 2
#define SB_ACTIVE 4
#define DMA_RUNNING 8

#define TRUE 1
#define FALSE 0

/* Global Variables */

extern struct WAVHeaderStruct {
 char rid[4];
 unsigned int rlen;
 
 char wid[4];
 char fid[4];
 unsigned int flen;
 char null1[2];
 short channels;
 short frequency;
 char null2[10];
 
 char did[4];
 unsigned int dlen;
} WAVHeader;

extern struct SoundStruct {
 int port;
 int irq;
 int dma;
} Sound;

extern struct DMAStruct {
 int stat;
 int flag;
 int count;
 
 int wav_offset;
 int wav_length;
 int wav_frequency;

 unsigned int buffer_offset;

 _go32_dpmi_seginfo dos_memory;
 _go32_dpmi_seginfo old_timer_handler;
 _go32_dpmi_seginfo old_irq_handler;

} DMA;

extern struct WAVStruct {
 char *offset;
 int  length;
} WAV;

/* Function Prototypes */

int WAVLoad(char *);
int DMAInit(void);

void DMAChain(void);
void DMAPlayWAV(void);
void DMAStart(void);
void DMAStop(void);
void DMAPoll(void);
void DMAInterrupt(void);
void DMAShutDown(void);
void WriteDSP(char);

short DMACount(void);
unsigned char ReadDSP(void);
