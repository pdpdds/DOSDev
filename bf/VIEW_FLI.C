/*

   This program is animation player for AutoDesk Animater

   Before source made sohn dongik 1992.11

   Source rebuild MG. ( MulGeal ) Software Production

   rebuild ver 0.4

   date : 93.10.01

   rebuild : YoungJin Ju

   PS : next version 93.10.15

*/


#pragma inline
#include <sys\stat.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <alloc.h>
#include <fcntl.h>
#include <mem.h>
#include <dos.h>

#include "bf.h"

/*
 * 한 FLIC file의 최대 frame수 (이 program에서는 상관 없음).
 */
#define FLI_MAXFRAMES		4000

#define FLIH_MAGIC		0xaf11		/* FLIC file 식별 code	*/
#define FLIF_MAGIC		0xf1fa		/* FLIC frame 식별 code	*/

/*
 * FLIC file header 구조.
 */
typedef struct {
	long		Size;		/* FLIC file의 size		*/
	unsigned	Type;		/* FLIC file의 식별 code	*/
	unsigned	FrameCount;	/* frame 갯수			*/
	unsigned	Width;		/* 화면 폭			*/
	unsigned	Height;		/* 화면 높이			*/
	unsigned	BPP;		/* pixel당 bit수(Bits Per Pixel)*/
	int 		Flags;
	int 		Speed;		/* playing 속도			*/
	long		NextHead;	/* 다음 FLIC file header 위치?  */
	long		FramesInTable;
	int 		File;
	long		Frame1Off;
	long		Strokes;
	long		Session;
	char		Reserved[88];
	} FLIHEAD;

/*
 * 한 frame header의 구조.
 */
typedef struct {
	long		Size;		/* 한 frame의 size		*/
	unsigned	Type;		/* frame 식별 code		*/
	int		Chunks;		/* chunk의 갯수			*/
	char		Pad[8];		/* 미사용			*/
	} FLIFRAME;

/*
 * 한 chunk header의 구조.
 */
typedef struct {
	long		Size;		/* 한 chunk의 size		*/
	unsigned	Type;		/* chunk type(아래에 정의)	*/
	} FLICHUNK;

typedef struct{
	unsigned re;
	char Skip;
	char Change;
	} PAL_BUF;

/*
 * chunk type의 정의.
 */
#define FLI_COLOR	11	/* palette update에 관계된 chunk type	*/
#define FLI_LC		12	/* line compress image data chunk type	*/
#define FLI_BLACK	13	/* 화면을 black으로 set하는 chunk type	*/
#define FLI_BRUN	15	/* RLE compress image data chunk type	*/
#define FLI_COPY	16	/* non-compress image data chunk type	*/

/*
 * program error 정의.
 */
#define SUCCESS			0
#define ERR_READ		1
#define ERR_WRITE		2
#define ERR_ALLOC		3
#define ERR_INV_FLIFRAME	4
#define ERR_INV_CHUNK		5
#define ERR_INV_COMP		6

char far 	*FirstScreen;		/* 첫 frame image data buffer	*/
int 		FirstFlag = 0;		/* 첫 frame decompress flag	*/
unsigned long f = (unsigned long)sizeof(FLIHEAD);
unsigned long fCount ;                  /* Fli Data Off Set Point       */
//long time1;


int hopen ( const char *name , const char mode )
{
   asm push ds
   asm lds dx,name
   asm mov ah,3dh
   asm mov al,mode
   asm int 21h
   asm pop ds
   asm jc l00
   asm jmp l01
l00:
   return -1;
l01:
   return _AX;
}

int hclose ( int handle )
{
   asm mov ah,3eh
   asm mov bx,handle
   asm jc  l00
   asm jmp l01

l00:
   return EOF;
l01:
   return NULL;
}

unsigned hread ( char *buf , unsigned size , int handle )
{
   asm push ds
   asm lds  dx,buf
   asm mov  ah,3fh
   asm mov  bx,handle
   asm mov  cx,size
   asm int  21h
   asm pop  ds
   asm jc  l00
   asm jmp l01
l00:
   return NULL ;
l01:
   return _AX ;
}


int hseek ( int handle , long len , char whence )
{
   asm push ds
   asm lds  dx,len
   asm mov  cx,ds
   asm pop  ds
   asm mov  ah,42h
   asm mov  al,whence
   asm mov  bx,handle
   asm int  21h
}

extern unsigned char Pal[256][3] ;
int DoPalette(int fp, unsigned size)
{

// char         Palette[768] ;
  PAL_BUF       pa;
  unsigned	rSize,buf;

  rSize = hread((char *)&pa,sizeof(PAL_BUF),fp) ;
  if(rSize != sizeof(PAL_BUF)) return ERR_READ ;
  if(!pa.Skip && !pa.Change) buf = 256 ;
  else buf = pa.Change ;
  if(hread((unsigned char *)&Pal,size - 4,fp) != size - 4) return ERR_READ ;
  SetRgbPalette((unsigned char *)&Pal,pa.Skip,buf) ;
  return SUCCESS ;
}

int DoRLE(int fp, unsigned size )
{
  char far   *buff;
  unsigned   Count,iCount = 0,k = 0;
  char	     c;
  int	     Packets;

  if(FirstFlag) {
    fCount += (unsigned long)sizeof(FLIHEAD);
    movedata(FP_SEG(FirstScreen),FP_OFF(FirstScreen),0xA000,0,64000);
    return SUCCESS;
  }
//  if((buff = farmalloc(size)) == NULL) return ERR_ALLOC;
  buff = (char far *)mcga2 ;
  if(hread(buff,size,fp) != size) return ERR_READ;
  while(iCount < 64000) {
    Packets = (int)*(buff + k++) & 0x00ff;
    while(Packets--) {
      Count = (unsigned)*(buff + k++) & 0x00ff;
      if(Count == 0) goto END;
      else if(Count > 127) {
	Count = 256 - Count;
	while(Count--)
	  *(FirstScreen + iCount++) = *(buff + k++);
      }else{
	c = *(buff + k++);
	while(Count--) *(FirstScreen + iCount++) = c;
      }
    }
  }

END:
  movedata(FP_SEG(FirstScreen),FP_OFF(FirstScreen),0xA000,0,64000);
//  farfree(buff);
  FirstFlag = 1;
  return SUCCESS;
}


/*
int DoRLE(fp,size)
FILE *fp;
unsigned size;
{
  char far	*buff;

  asm{
   cmp FirstFlag,0
   je l0
   push ds
   lds si,FirstScreen
   mov ax,0a000h
   mov es,ax
   xor di,di
   mov cx,32000
   rep movsw
   pop ds
  }

  fCount = f;
  return SUCCESS;

  l0:
  if((buff = farmalloc(size)) == NULL) return ERR_ALLOC;
  if(hread(buff,1,size,fp) != size) return ERR_READ;

  asm{
   xor dx,dx
   xor bx,bx
   xor cx,cx
   push ds
   lds si,buff
  }

  l00:
  asm{
    cmp dx,64000
    jge l10
    mov cl,ds:[bx+si]
    inc bx
  }

  l01:
  asm{
    cmp cl,0
    je l00
    push cx
    mov cl,ds:[bx+si]
    inc bx
    cmp cl,0
    je l09
    cmp cl,127
    jle l03
  }

  l02:
  asm{
    les di,FirstScreen
    mov al,ds:[bx+si]
    inc bx
    add di,dx
    add dx,cx
    rep stosb
    jmp l04
  }

  l03:
  asm{
    not cl
    inc cx
    push si
    les di,FirstScreen
    add di,dx
    add si,bx
    add bx,cx
    add dx,cx
    rep movsb
    pop si
  }

  l04:
  asm{
    pop cx
    loop l01
  }

  l09:
  asm{
    pop cx
  }

  l10:
  asm{
   lds si,FirstScreen
   mov ax,0a000h
   mov es,ax
   xor di,di
   mov cx,32000
   rep movsw
   pop ds
  }

  farfree(buff);
  FirstFlag = 1;
  return SUCCESS;
}*/

void DoBlackScreen(void)
{
  asm{
    mov ax,0a000h
    mov es,ax
    xor di,di
    mov ax,0
    mov cx,32000
    rep stosw
  }
}

int DoCopy(int fp, unsigned size)
{
  char far *buff;

//  if((buff = farmalloc(size)) == NULL) return ERR_ALLOC;
  buff = (char far *)mcga2 ;
  if(hread(buff,size,fp) != size) return ERR_READ;

  asm{
    push ds
    lds si,buff
    mov ax,0a000h
    mov es,ax
    xor di,di
    mov cx,size
    rep movsb
    pop ds
  }

  return SUCCESS;
}

int DoLC(int fp, unsigned size)
{
//  long          time1;
  char far	*buff;

//  if((buff = farmalloc(size)) == NULL) return ERR_ALLOC;
  buff = mcga2 ;
  if(hread(buff,size,fp) != size) return ERR_READ;

  asm{
   push ds
   lds si,buff
   mov ax,0a000h
   mov es,ax
   lodsw
   mov dx,ax
   lodsw
   mov cx,ax
   xchg dh,dl
   mov bx,dx
   shr bx,1
   shr bx,1
   add dx,bx
   mov di,dx
  }

  l00:
  asm{
    cmp cx,0
    je l06
    push di
    push cx
    xor ch,ch
    lodsb
    mov cl,al
  }

  l01:
  asm{
    cmp cl,0
    je l05
    push cx
    xor ah,ah
    lodsb
    add di,ax
    xor ch,ch
    lodsb
    mov cl,al
    cmp cl,7fh
    jbe l02
    jmp l07
  }

  l02:
  asm{
    cmp cx,0
    je l03
    jmp l09
  }

  l03:
  asm{
    pop cx
    pop cx
    pop ds
  }
    return ERR_INV_COMP;

  l04:
  asm{
    pop cx
    loop l01
  }

  l05:
  asm{
    pop cx
    pop di
    add di,320
    loop l00
  }

  l06:
    asm{
     pop ds
     jmp l11
    }


  l07:
  asm{
    not cl
    inc cx
    lodsb
    cld
    rep stosb
    jmp l04
  }

  l08:
  asm{
    not cl
    inc cx
    lodsb
    mov ah,al
    shr cx,1
    cld
    rep stosw
    jmp l04
  }

  l09:
  asm{
    cld
    rep movsb
    jmp l04
  }

  l10:
  asm{
    shr cx,1
    cld
    rep movsw
    jmp l04
  }

  l11:

//  farfree(buff);
  return SUCCESS;
}

int DoChunk(int fp)
{
  FLICHUNK	fliChunk;
  unsigned	rSize;
  int		rtn;

  rSize = hread((char *)&fliChunk,sizeof(FLICHUNK),fp);

  if(rSize != sizeof(FLICHUNK)) return ERR_READ;

  switch(fliChunk.Type) {
    case FLI_COLOR:
      rtn = DoPalette(fp,fliChunk.Size - sizeof(FLICHUNK));
      if(rtn != SUCCESS) return rtn;
      break;

    case FLI_LC:
      rtn = DoLC(fp,fliChunk.Size - sizeof(FLICHUNK));
      if(rtn != SUCCESS) return rtn;
      break;

    case FLI_BLACK:
      DoBlackScreen();
      break;

    case FLI_BRUN:
      rtn = DoRLE(fp,fliChunk.Size - sizeof(FLICHUNK));
      if(rtn != SUCCESS) return rtn;
      break;

    case FLI_COPY:
      rtn = DoCopy(fp,fliChunk.Size - sizeof(FLICHUNK));
      if(rtn != SUCCESS) return rtn;
      break;

    default:
      return ERR_INV_CHUNK;
  }

  return SUCCESS;
}

int DoOneFrame(int fp)
{
  FLIFRAME	fliFrame;
  unsigned	rSize;
  int		i,rtn;


  rSize = hread((char *)&fliFrame,sizeof(FLIFRAME),fp);

  if(rSize != sizeof(FLIFRAME)) return ERR_READ;
  if(fliFrame.Type != FLIF_MAGIC) return ERR_INV_FLIFRAME;
    for(i = 0; i < fliFrame.Chunks; i++) {
      rtn = DoChunk(fp);
      if(rtn != SUCCESS) return rtn;
    }

  fCount += fliFrame.Size;
  hseek(fp,fCount,SEEK_SET);
  return SUCCESS;
}

#define  MAXCOUNT 50

char AniHeader[16] = "ANI Version 1.0" ;
int  AniCount = 0 ;
char AniName[MAXCOUNT][9] ;
long AniSize[MAXCOUNT] ;

int  ani_datafile ;

int CheckFLI( char *fli_name )
{
   int ani_number ;

   for( ani_number=0; ani_number<AniCount; ani_number++ )
   {
      if( !strcmpi( fli_name, AniName[ani_number] ) ) break ;
   }
   if( ani_number == AniCount ) return 0 ;
   else                         return 1 ;
}

void ViewFLI( char *fli_name, int play_mode )
{
   FLIHEAD   fliHead ;
   unsigned  rSize ;
   int       Frames ;
   int       rtn ;
   int ani_number, i ;

   Frames = 0 ;
   FirstFlag = 0 ;
//   time1 = biostime ( 1 , 0l ) ;
   fCount = f ;

//   if((ani_datafile = hopen("1star.ani",0)) == -1) {
//      puts("can't find file !");
//      exit(1);
//   }

   for( ani_number=0; ani_number<AniCount; ani_number++ )
   {
      if( !strcmpi( fli_name, AniName[ani_number] ) ) break ;
   }
   lseek( ani_datafile, 16L+2L, SEEK_SET ) ;
   lseek( ani_datafile, 12L*AniCount, SEEK_CUR ) ;
   fCount += 16L+2L+(12L*AniCount) ;
   for( i=0; i<ani_number; i++ )
   {
      lseek( ani_datafile, AniSize[i], SEEK_CUR ) ;
      fCount += AniSize[i] ;
   }

   rSize = hread((char *)&fliHead,sizeof(FLIHEAD),ani_datafile);

   if(rSize != sizeof(FLIHEAD)) {
     puts("Read Error !");
     hclose(ani_datafile);
     exit(1);
   }

   if(fliHead.Type != FLIH_MAGIC) {
     puts("Not flic animation file !");
     hclose(ani_datafile);
     exit(1);
   }

   /*
   if((FirstScreen = farmalloc(64000)) == NULL) {
     puts("memory allocation error !");
     hclose(ani_datafile);
     exit(1);
   } */
   FirstScreen = (char far *)mcga1 ;

   _fmemset( FirstScreen, 0, 64000 ) ;

   while( 1 )
   {
     if((rtn = DoOneFrame(ani_datafile)) == ERR_INV_COMP) {
       SetMode(0x03);
//       farfree(FirstScreen);
       printf("Error Code: %d",rtn);
       hclose(ani_datafile);
       exit(1);
     }

     if( kbhit() && play_mode ) { if(!getch()) getch(); break; }

     if(Frames++ >= (fliHead.FrameCount-1))
     {
//        time1 = biostime ( 0 , 0l );
//        printf ( "%f" , time1 / CLK_TCK );
	if( !play_mode ) break ;

	Frames = 0 ;
	FirstFlag = 0 ;
//	time1 = biostime ( 1 , 0l ) ;
	fCount = f ;

	for( ani_number=0; ani_number<AniCount; ani_number++ )
	{
	   if( !strcmpi( fli_name, AniName[ani_number] ) ) break ;
	}
	lseek( ani_datafile, 16L+2L, SEEK_SET ) ;
	lseek( ani_datafile, 12L*AniCount, SEEK_CUR ) ;
	fCount += 16L+2L+(12L*AniCount) ;
	for( i=0; i<ani_number; i++ )
	{
	   lseek( ani_datafile, AniSize[i], SEEK_CUR ) ;
	   fCount += AniSize[i] ;
	}

	hseek(ani_datafile,(long)sizeof(FLIHEAD),SEEK_CUR);
     }
     else
	waitmSec( 50 ) ;
   }

//   farfree(FirstScreen) ;
//   hclose(ani_datafile) ;
}

void Open_Anima_File( char *ani_file_name )
{
   int i ;

   ani_datafile = open( ani_file_name, O_RDONLY | O_BINARY, S_IWRITE | S_IREAD ) ;
   if( ani_datafile == -1 )
   {
      printf( "\a File Not Found !\n\a" ) ;
      return ;
   }
   read( ani_datafile, AniHeader, 16 ) ;
   read( ani_datafile, &AniCount, 2 ) ;

   for( i=0; i<AniCount; i++ )
   {
      AniName[i][8] = 0 ;
      read( ani_datafile, AniName[i], 8 ) ;
      read( ani_datafile, &AniSize[i], 4 ) ;
   }
}

void Close_Anima_File( void )
{
   close( ani_datafile ) ;
}
