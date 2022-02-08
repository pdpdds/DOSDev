// blit.h (blit.asm)

#ifndef __blit
#define __blit


void DrawPic( int x, int y, void *pic, void *code );
#pragma aux DrawPic \
        parm [ebx] [ecx] [esi] [eax] \
        modify [eax edx edi];

void DrawTPic( int x, int y, void *pic, void *code );
#pragma aux DrawTPic \
        parm [ebx] [ecx] [esi] [eax] \
        modify [eax edx edi];

void DrawTTPic( int x, int y, void *pic, void *table, void *rle );
#pragma aux DrawTTPic \
        parm [ebx] [ecx] [esi] [edi] [eax]\
        modify [eax edx];

void DrawBTPic( int x, int y, void *pic, void *table, void *rle );
#pragma aux DrawBTPic \
        parm [ebx] [ecx] [esi] [edi] [eax]\
        modify [eax edx];

void SetClip( int left, int top, int right, int bottom );
#pragma aux SetClip \
        parm [eax] [ebx] [ecx] [edx];

void SetDstScreen( void *screen );
#pragma aux SetDstScreen \
        parm [esi];

void *GetDstScreen( void );
#pragma aux GetDstScreen \
        value [eax];

void CopyToScreen( void );
#pragma aux CopyToScreen modify [ecx esi edi];

void AltScrCopy( void );
#pragma aux AltScrCopy modify [eax ecx esi edi];

void SetPalette( void *palette );
#pragma aux SetPalette \
        parm [esi] \
        modify [al cx dx];


#endif
