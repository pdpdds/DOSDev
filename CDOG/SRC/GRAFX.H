#ifndef __grafx
#define __grafx


struct RGB
{
  unsigned char red, green, blue;
};
typedef struct RGB color;
typedef color TPalette[256];
typedef unsigned char TranslationTable[256];


void SetColorZero( int r, int g, int b );
#pragma aux SetColorZero = \
        "mov dx,03c8h" \
        "mov ah,al"    \
        "mov al,0"     \
        "out dx,al"    \
        "inc dx"       \
        "mov al,ah"    \
        "out dx,al"    \
        "mov al,bl"    \
        "out dx,al"    \
        "mov al,cl"    \
        "out dx,al"    \
        parm [eax][ebx][ecx] \
        modify [dx];

void Mode13h( void );
void TextMode( void );
int ReadPics( const char *filename, void **pics, int maxPics, color *palette );
int AppendPics( const char *filename, void **pics, int startIndex, int maxPics );
int CompilePics( int picCount, void **pics, void **compiledPics );
int RLEncodePics( int picCount, void **pics, void **rlePics );
void vsync( void );
int PicWidth( void *pic );
int PicHeight( void *pic );

#endif
