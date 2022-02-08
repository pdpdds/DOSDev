#include <io.h>
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <alloc.h>
#include <process.h>
#include <fcntl.h>
#include "v256.h"

typedef struct
{
 BYTE Red,Green,Blue;
} ColorRegister;

struct PCXFileHeader
{
 BYTE Header,Version,Encode,BitPerPix;
 unsigned X1,Y1,X2,Y2,Hres,Vres;
};

struct PCXInfo
{
 BYTE Vmode,NumOfPlanes;
 unsigned BytesPerLine;
 BYTE     unused[60];
};

struct PCX_File
{
   struct PCXFileHeader PCXHeader;
   ColorRegister        Palette[16];
   struct PCXInfo       Info;
};

struct ExtendedPalette
{
   BYTE ExtendedPalette;
   ColorRegister Palette[256];
};

struct PCX_File PCXData;
unsigned ImageWidth=320,ImageHeight=200;
int rgb_check;

static FILE    *PCXFile;
static BYTE     ScanLine[320];
static BYTE     PixelColorNum[320];
static struct   ExtendedPalette Color256Palette;
BYTE R_[256],G_[256],B_[256];

void setvgapalette256(DacPalette256 *PalBuf)
{
	asm{ mov ax,1012h
	     mov bx,0
	     mov cx,256
	     les dx,PalBuf
	     int 10h       }
}

void getvgapalette256(DacPalette256 *PalBuf)
{
	asm{ mov ax,1017h
	     mov bx,0
	     mov cx,256
	     les dx,PalBuf
	     int 10h      }
}

void savergb(char *filename)
{
	DacPalette256 *rgb;
	int handle;

	if((rgb=malloc(768))==NULL){
	    settextmode(); printf("Not enough memory!\n"); exit(-1);
				   }
	else{
	     getvgapalette256(rgb);
	     if((handle=open(filename,O_WRONLY|O_CREAT|O_BINARY,0x0100|0x0080))==-1){
		 settextmode(); printf("File open error!\n"); exit(-1);
								   }
	else{
	     write(handle,rgb,768);
	     close(handle);
	    }
	    free(rgb);
	    }
}

void savergb2(char *filename)
{
 int handle;
 if((handle=open(filename,O_WRONLY|O_CREAT|O_BINARY,0x0100|0x0080))==-1)
   { settextmode(); printf("File open error!\n"); exit(-1); }
 else{ write(handle,&Color256Palette,sizeof(struct ExtendedPalette)); close(handle); }
}

void loadrgb2(char *filename)
{
 int handle;
 unsigned Index;

 if((handle=open(filename,O_RDONLY | O_BINARY))==-1)
   { settextmode(); printf("File not found!\n"); exit(-1); }
 else{read(handle,&Color256Palette,sizeof(struct ExtendedPalette));
      close(handle);
      for(Index=0; Index < 256; Index++){
	  R_[Index]=Color256Palette.Palette[Index].Red;
	  G_[Index]=Color256Palette.Palette[Index].Green;
	  B_[Index]=Color256Palette.Palette[Index].Blue;
					}
     }
}

void loadrgb(char *filename)
{
  DacPalette256 *rgb;
  int handle;

  if((handle=open(filename,O_RDONLY | O_BINARY))==-1){
      settextmode(); printf("File not found!\n"); exit(-1);
						     }
  else{if((rgb=malloc(768))==NULL){
	  settextmode(); printf("Not enough memory!\n"); exit(-1);
				  }
       read(handle,rgb,768);
       close(handle);
       setvgapalette256(rgb);
       free(rgb);
      }
}

int ReadPCXFileHdr (char *FileName,int pcc)
{
   char String[80];

   if (!strchr(FileName,'.')) {
      strcpy(String,FileName);
      FileName = String;
      if(pcc) strcat(FileName,".pcx");
      else strcat(FileName,".pcc");
			      }

   if ((PCXFile = fopen(FileName,"rb")) == NULL) return(-1);
   if (fread(&PCXData,sizeof(struct PCX_File),1,PCXFile) != 1) return(-1);
   return(0);
}

static int ExpandScanLine (FILE *InFile)
{
   register short    BitNum;
   register unsigned ByteNum;
   register short    CharRead;
   unsigned InPtr=0,RepCount,PixelsData;
   unsigned BytesToRead,PlaneNum;
   unsigned ByteOffset,BitOffset;

   BytesToRead = PCXData.Info.NumOfPlanes * PCXData.Info.BytesPerLine;

   do{
      CharRead = getc(InFile);
      if (CharRead == EOF) return(0);

      if ((CharRead & 0xC0) == 0xC0)
      {
	 RepCount = CharRead & ~0xC0;
	 CharRead = getc(InFile);
	 if (CharRead == EOF) return(0);

	 while (RepCount--) ScanLine[InPtr++] = CharRead;
      }
      else ScanLine[InPtr++] = CharRead;
      } while (InPtr < BytesToRead);

   memtomem(FP_SEG(PixelColorNum),FP_OFF(PixelColorNum)
	   ,FP_SEG(ScanLine),FP_OFF(ScanLine),ImageWidth,1);
   return(1);
}

void setvgapalette2562(ColorRegister *PalBuf)
{
	asm{ mov ax,1012h
	     mov bx,0
	     mov cx,256
	     les dx,PalBuf
	     int 10h       }
}

void InstallPCXFilePalette(void)
{
 ColorRegister *rgb;
 unsigned Index;

 if(rgb_check){ for(Index=0; Index < 256; Index++){
		       Color256Palette.Palette[Index].Red   >>= 2;
		       Color256Palette.Palette[Index].Green >>= 2;
		       Color256Palette.Palette[Index].Blue  >>= 2;
						      }
		   rgb=Color256Palette.Palette;
		   setvgapalette2562(rgb);
		 }
 else if(rgb_check==0){ for(Index=0; Index < 256; Index++){
			    R_[Index]=Color256Palette.Palette[Index].Red   >>= 2;
			    G_[Index]=Color256Palette.Palette[Index].Green >>= 2;
			    B_[Index]=Color256Palette.Palette[Index].Blue  >>= 2;
							   }
			rgb=Color256Palette.Palette;
			setvgapalette2562(rgb);
			rgb_check=1;
		       }
}

int readpcxfilebuf (char *FileName, BYTE **BufferPtr)
{
   register unsigned ScanNum;
   BYTE *ImageMemory;
   unsigned long PixelBufOffset;

   ReadPCXFileHdr(FileName,0);

   ImageMemory = (BYTE *) farcalloc((long) ImageWidth * ImageHeight,sizeof(BYTE));

   if (ImageMemory == NULL) {
      settextmode();
      printf("Error Not enough memory for PCX buffer!\n");
      exit(-1);             }

   for (ScanNum=0; ScanNum < ImageHeight; ScanNum++) {
	ExpandScanLine(PCXFile);
	PixelBufOffset = (long) ScanNum * ImageWidth;
	smemtosmem(ImageMemory+PixelBufOffset,PixelColorNum,ImageWidth);
						     }
   fread(&Color256Palette,sizeof(struct ExtendedPalette),1,PCXFile);
   fclose(PCXFile);
   InstallPCXFilePalette();
   *BufferPtr = ImageMemory;
   return(0);
}

void displaypcxbuf(BYTE *image, int mode)
{
   memtomem(0x0a000,0,FP_SEG(image),FP_OFF(image),64000,mode);
}

void displaypcxfile (char *FileName, int mode)
{
   register unsigned ScanNum;
   unsigned video_offset;

   if (ReadPCXFileHdr(FileName,0) != 0) exit(-1);

   for (ScanNum=0; ScanNum < ImageHeight; ScanNum++)
   {
	 ExpandScanLine(PCXFile);
	 video_offset=320*ScanNum;
	 memtomem(0x0a000,video_offset,FP_SEG(PixelColorNum),FP_OFF(PixelColorNum),ImageWidth,mode);
   }

   fread(&Color256Palette,sizeof(struct ExtendedPalette),1,PCXFile);
   InstallPCXFilePalette();

   fclose(PCXFile);
}

void displaypccfile (char *FileName,int mode,unsigned XPOS,unsigned YPOS)
{
   register unsigned ScanNum;
   unsigned video_offset;

   ReadPCXFileHdr(FileName,1);

   for(ScanNum=0; ScanNum < PCXData.PCXHeader.Y2+1; ScanNum++){
       ExpandScanLine(PCXFile);
       video_offset=320*(ScanNum+YPOS)+XPOS;
       memtomem(0x0a000,video_offset,FP_SEG(PixelColorNum),FP_OFF(PixelColorNum),PCXData.PCXHeader.X2+1,mode);
							    }
   fclose(PCXFile);
}

#define W unsigned

void brightCtrl(s,n,r,g,b,factor)
BYTE s,*r,*g,*b ;
int n,factor ;
{
   int i ;

   outportb(0x3c8,s) ;

   for ( i=0 ; i<n ; i++ )
   {
      outportb(0x3c9,(r[i] * factor)/100) ;
      outportb(0x3c9,(g[i] * factor)/100) ;
      outportb(0x3c9,(b[i] * factor)/100) ;
   }
}

void Fade_In()
{
   int i ;
   for ( i=0 ; i<=100 ; i++ )
   {
     brightCtrl(0,256,R_,G_,B_,i) ;
     //delay(i/3);
     vertical_retrace();
   }
   brightCtrl(0,256,R_,G_,B_,100) ;
}

void Fade_Out()
{
   int i ;

   for ( i=100 ; i>=0 ; i-- )
   {
     brightCtrl(0,256,R_,G_,B_,i) ;
     //delay(i/3);
     vertical_retrace();
   }
   brightCtrl(0,256,R_,G_,B_,0) ;
}

void displaypccfile2 (char *FileName,int mode,unsigned XPOS,unsigned YPOS)
{
   register unsigned ScanNum;
   unsigned video_offset;
   W Index;

   ReadPCXFileHdr(FileName,1);

   brightCtrl(0,256,R_,G_,B_,0);
   for(ScanNum=0; ScanNum < PCXData.PCXHeader.Y2+1; ScanNum++){
       ExpandScanLine(PCXFile);
       video_offset=320*(ScanNum+YPOS)+XPOS;
       memtomem(0x0a000,video_offset,FP_SEG(PixelColorNum),FP_OFF(PixelColorNum),PCXData.PCXHeader.X2+1,mode);
							    }
   fread(&Color256Palette,sizeof(struct ExtendedPalette),1,PCXFile);
   fclose(PCXFile);

   for (Index=0; Index < 256; Index++){
	R_[Index] = Color256Palette.Palette[Index].Red >>=2;
	G_[Index] = Color256Palette.Palette[Index].Green >>=2;
	B_[Index] = Color256Palette.Palette[Index].Blue >>=2;
				      }
   Fade_In();
}

/*
  get one Digital Analog Conver Register value
  Digital Analog Convert Register means VGA 256 R,G,B palette

  input  i : index value of palette register
  output r,g,b : r,g,b value of palette registers

  ÌiAËa A»¡¯aÈá ˆt—i·i ·ª´áµ¥”a.
**/

void get1DACR(i,r,g,b)
BYTE i,*r,*g,*b ;
{
   outportb(0x3c7,i) ;
   *r = inportb(0x3c9)   ;
   *g = inportb(0x3c9)   ;
   *b = inportb(0x3c9)   ;
}

/*
  get N Digital Analog Conver Register value
  Digital Analog Convert Register means VGA 256 R,G,B palette

  input  s : start index value of palette register
         n : number of index
  output r[],g[],b[] : r,g,b array value of palette registers

  Nˆ· ÌiAËa A»¡Èáˆt—i·i ·ª´áµ¥”a.
**/
void getNDACR(s,n,r,g,b)
BYTE s,*r,*g,*b ;
int n ;
{
   int i ;

   outportb(0x3c7,s) ;
   for ( i=0 ; i<n ; i++ )
   {
     r[i] = inportb(0x3c9)   ;
     g[i] = inportb(0x3c9)   ;
     b[i] = inportb(0x3c9)   ;
   }
}

/*
  set one Digital Analog Conver Register value
  Digital Analog Convert Register means VGA 256 R,G,B palette

  input      i : index value of palette register
         r,g,b : r,g,b value of palette registers
  Ðeˆ· ÌiAËaµA ˆt·i ¬é¸÷Ðe”a.
**/
void set1DACR(i,r,g,b)
BYTE i,r,g,b ;
{
   outportb(0x3c8,i) ;
   outportb(0x3c9,r) ;
   outportb(0x3c9,g) ;
   outportb(0x3c9,b) ;
}

/*
  set N Digital Analog Conver Register value
  Digital Analog Convert Register means VGA 256 R,G,B palette

  input  s : start index value of palette register
         n : number of index
         r[],g[],b[] : r,g,b array value of palette registers
  Nˆ· ÌiAËa ˆt·i ¬é¸÷Ðe”a.
**/
void setNDACR(s,n,r,g,b)
BYTE s,*r,*g,*b ;
int n ;
{
   int i ;

   outportb(0x3c8,s) ;

   for ( i=0 ; i<n ; i++ )
   {
     outportb(0x3c9,r[i]) ;
     outportb(0x3c9,g[i]) ;
     outportb(0x3c9,b[i]) ;
   }
}

/*
  grey control of VGA graphic screen
  input s : start index of palette register
        n : number of index
        r[],g[],b[] : r,g,b value of palette
        factor : grey level percent(%)
  ¸åÁA· ¡w´q·i %¡ ¹¡¸÷Ðe”a
*/
void greyCtrl(s,n,r,g,b,factor)
BYTE s,*r,*g,*b ;
int n,factor ;
{
   int i ;
   BYTE grey ;

   outportb(0x3c8,s) ;

   for ( i=0 ; i<n ; i++ )
   {
     grey = ((r[i]*3 + g[i]*5 + b[i]*2)/10)*factor/100  ;
     outportb(0x3c9,grey) ;
     outportb(0x3c9,grey) ;
     outportb(0x3c9,grey) ;
   }
}

/*
  set RGB palette

  index is 8 bit

  +--+--+--+--+--+--+--+--+
  |R2|R1|R0|G2|G1|G0|B1|B0|
  +--+--+--+--+--+--+--+--+
     RED      GREEN  BLUE

  ¯aÌa2 ¸å¶w ÌiAËa ¬é¸÷
*/
void setRGBPalette()
{
   int i,r,g,b ;

   for ( r=0,i=0 ; r<8 ; r++ )
     for ( g=0 ; g<8 ; g++ )
      for ( b=0 ; b<4 ; b++,i++ )
      set1DACR(i,r*8+4,g*8+4,b*16+8) ;
}
