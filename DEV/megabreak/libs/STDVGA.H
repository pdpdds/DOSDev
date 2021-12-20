/*
    Standard VGA functions
    Part of Knut's GFX-Lib for Watcom

    1/4-97
*/
#include <math.h>

void mode_13h();
void text_mode();
void wait_vbl();
void plot(long x, long y, long colour);
void do_plot();
void draw_box(long x, long y, long xlen, long ylen, long colour);
void do_draw_box();
void bput_block(long x, long y, long xlen, long ylen, byte *imgpnt, byte *scr);
void do_bput_block(byte *imgpnt, byte *scr);
void put_block(long x, long y, long xlen, long ylen, byte *imgpnt);
void do_put_block(byte *imgpnt);
void cput_block(long x, long y, long xlen, long ylen, byte *imgpnt);
void do_cput_block(byte *imgpnt);
void bdraw_box(long x, long y, long xlen, long ylen, long colour, byte *scr);
void do_bdraw_box(byte *scr);
void put_sprite(long x, long y, long xlen, long ylen, byte *imgpnt);
void do_put_sprite(byte *imgpnt);
void bput_sprite(long x, long y, long xlen, long ylen, byte *imgpnt, byte *buffer);
void do_bput_sprite(byte *imgpnt, byte *buffer);
void block_copy(int srcx, int srcy, int srcxlen, int srcylen, byte *buffer, int destx, int desty);
void do_block_copy(byte *buffer);
void block_copy_b(int srcx, int srcy, int srcxlen, int srcylen, byte *buffer, int destx, int desty, byte *destimg);
void do_block_copy_b(byte *buffer, byte *destimg);
short detectvga();
void clip_put_block(long x, long y, long xlen, long ylen, byte *imgpnt);

long _clipart = 0;
long _x = 0;
long _y = 0;
long _xlen = 0;
long _ylen = 0;
long _colour = 0;
long _srcx = 0;
long _srcy = 0;
long _srcxlen = 0;
long _srcylen = 0;
long _dstx = 0;
long _dsty = 0;

#pragma aux detectvga =                \
    "mov    bx, 0xFFFF"                 \
    "mov    ax, 0x101A"                 \
    "int    0x10"                       \
    modify [AX]                         \
    value [BX];

short detect_vga()
{
    short result = detectvga();
    if (result == 0xFFFF)
        return 0;
    else
        return 1;
}

#pragma aux mode_13h = \
    "mov    eax,0013h"       \
    "int    10h"             \
    modify [EAX];

#pragma aux text_mode = \
    "mov    eax,0003h"   \
    "int    10h"         \
    modify [EAX];

#pragma aux wait_vbl = \
    "mov  dx,03DAh" \
"loop1:"\
    "in   al,dx"\
    "test al,8"\
    "jnz  loop1"\
"loop2:"\
    "in   al,dx"\
    "test al,8"\
    "jz   loop2"\
    modify [DX AL];

// clipping put block impl. Note: Max 4096 pixels (64x64).
void clip_put_block(long x, long y, long xlen, long ylen, byte *imgpnt)
{
    byte dataToDraw[4096];

    if (x + xlen <= 0 || y + ylen <= 0 || x > 319 || y > 199) return;
    if (x > 0 && y > 0 && x + xlen < 320 && y + ylen < 200)
    {
        _x = x;
        _y = y;
        _xlen = xlen;
        _ylen = ylen;
        do_put_block(imgpnt);
        return;
    }
    
    long clippedXOffset = 0;
    long clippedYOffset = 0;
    long clippedXlen = xlen;
    long clippedYlen = ylen;

    if (x < 0) {
        clippedXOffset = fabs(x);
        clippedXlen = xlen - clippedXOffset;
    }
    if (y < 0) {
        clippedYOffset = fabs(y);
        clippedYlen = ylen - clippedYOffset;
    }
    if (x + xlen > 319) {
        clippedXlen = 320 - x;
    }
    if (y + ylen > 199) {
        clippedYlen = 200 - y;
    }

    long i = 0;
    long imgIx = xlen * clippedYOffset;
    for (long cy = clippedYOffset; cy < clippedYOffset + clippedYlen; cy++) {
        imgIx += clippedXOffset;
        for (long cx = clippedXOffset; cx < clippedXOffset + clippedXlen; cx++) {
            dataToDraw[i++] = *(imgpnt + imgIx++);
        }
        if (clippedXlen < xlen) imgIx += xlen - clippedXlen;
    }

    _x = x + clippedXOffset;
    _y = y + clippedYOffset;
    _xlen = clippedXlen;
    _ylen = clippedYlen;
    do_put_block(dataToDraw);
}

void plot(long x, long y, long colour)
{
    _x = x;
    _y = y;
    _colour = colour;
    do_plot();
}

#pragma aux do_plot = \
    "mov     eax,_y" \
    "mov ebx,_x" \
    "mov edx,_colour" \
    "mov     edi, 0a0000h" \
    "shl     eax, 6" \
    "add     edi, eax" \
    "shl     eax, 1" \
    "shl     eax, 1" \
    "add     edi, eax" \
    "add     edi, ebx" \
    "mov     BYTE PTR [edi], dl" \
modify [eax ebx edx edi];

void draw_box(long x, long y, long xlen, long ylen, long colour)
{
    if (x > 319 || y > 199 || x < 0 || y < 0) return;
    if (x + xlen > 319) xlen = 320 - x;
    if (y + ylen > 199) ylen = 200 - y;
    if (ylen == 0 || xlen == 0) return;

    _x = x;
    _y = y;
    _xlen = xlen;
    _ylen = ylen;
    _colour = colour;

    do_draw_box();
}

#pragma aux do_draw_box = \
    "mov     eax,_y" \
    "mov     ebx,_x" \
    "mov     edi,0A0000h" \
    "mov     ecx,eax" \
    "shl     eax,8" \
    "shl     ecx,6" \
    "add     eax,ecx" \
    "add     eax,ebx" \
    "add     edi,eax" \
    "mov     ebx,320" \
    "mov     edx,_xlen" \
    "sub     ebx,edx" \
    "mov     eax,_ylen" \
    "mov     edx,_colour" \
"next_line:" \
    "mov     ecx,_xlen" \
"next:" \
    "mov     BYTE PTR [edi],dl" \
    "dec     ecx" \
    "inc     edi" \
    "or      ecx,ecx" \
    "jnz     next" \
    "dec     eax" \
    "add     edi,ebx" \
    "or      eax,eax" \
    "jnz     next_line" \
modify [eax ebx ecx edx edi];

void bput_block(long x, long y, long xlen, long ylen, byte *imgpnt, byte *scr)
{
    _x = x;
    _y = y;
    _xlen = xlen;
    _ylen = ylen;
    do_bput_block(imgpnt, scr);
}

#pragma aux do_bput_block = \
    "mov     eax,_y" \
    "mov     ebx,_x" \
    "mov     ecx,eax" \
    "shl     eax,8" \
    "shl     ecx,6" \
    "add     eax,ecx"             /* Legg sammen begge Y-ene */ \
    "add     eax,ebx"             /* Legg til X */ \
    "add     edi,eax"             /* Ferdig koordinat ligger nå i EDI */ \
    "mov     edx,320"             /* Kalkuler mengde som må legges til EDI på */ \
    "mov     ebx,_xlen"          /* slutten av hver linje. */ \
    "sub     edx,ebx"             /* Mengde ligger i EDX */ \
    "mov     eax,_ylen"          /* Legg vertikal teller i EAX */ \
    "next_line:" \
    "mov     ecx,ebx"             /* Legg linje-teller i ECX */ \
    "rep     movsb"               /* Tegn opp en linje.. */ \
    "dec     eax"                 /* En linje mindre.. */ \
    "add     edi,edx"             /* Legg til slik at neste linje begynner under.. */ \
    "or      eax,eax"             /* Vertikal teller 0 ennå? */ \
    "jnz     next_line"          /* Nei - gå til neste linje */ \
parm [esi] [edi] \
modify [eax ebx ecx edx edi];

void put_block(long x, long y, long xlen, long ylen, byte *imgpnt)
{
    _x = x;
    _y = y;
    _xlen = xlen;
    _ylen = ylen;
    do_put_block(imgpnt);
}

#pragma aux do_put_block = \
    "mov     eax,_y" \
    "mov     ebx,_x" \
    "mov     edi,0A0000h"          /* Video-mem... */ \
    "mov     ecx,eax" \
    "shl     eax,8" \
    "shl     ecx,6" \
    "add     eax,ecx"             /* Legg sammen begge Y-ene */ \
    "add     eax,ebx"             /* Legg til X */ \
    "add     edi,eax"             /* Ferdig koordinat ligger nå i EDI */ \
    "mov     edx,320"             /* Kalkuler mengde som må legges til EDI på */ \
    "mov     ebx,_xlen"          /* slutten av hver linje. */ \
    "sub     edx,ebx"             /* Mengde ligger i EDX */ \
    "mov     eax,_ylen"          /* Legg vertikal teller i EAX */ \
    "next_line:" \
    "mov     ecx,ebx"             /* Legg linje-teller i ECX */ \
    "rep     movsb"               /* Tegn opp en linje.. */ \
    "dec     eax"                 /* En linje mindre.. */ \
    "add     edi,edx"             /* Legg til slik at neste linje begynner under.. */ \
    "or      eax,eax"             /* Vertikal teller 0 ennå? */ \
    "jnz     next_line"           /* Nei - gå til neste linje */ \
parm [esi] \
modify [eax ebx ecx edx edi esi];

void cput_block(long x, long y, long xlen, long ylen, byte *imgpnt)
{
    _x = x;
    _y = y;
    _xlen = xlen;
    _ylen = ylen;
    if (x + xlen < 320 && y + ylen < 200)
        do_put_block(imgpnt);
    else
        do_cput_block(imgpnt);
}

#pragma aux do_cput_block = \
    "mov    eax,_y" \
    "mov    ebx,_x" \
    "cmp    ebx, 319" /* Are coords outside screen? */ \
    "ja     cend" \
    "cmp    eax, 199" \
    "ja     cend" \
    "add    ebx, _xlen" /* No - should the whole block be drawn? */ \
    "add    eax, _ylen" \
    "cmp    ebx, 319" \
    "jle    normal_put" \
    "cmp    eax, 199" \
    "jle    normal_put" \
    "sub    ebx, 319" \
    "sub    eax, 199" \
    "mov    _clipart, 0" \
    "cmp    ebx, 0" \
    "ja     new_xlen" \
    "cmp    eax, 0" \
    "ja     new_ylen" \
    "jmp    normal_put" \
"new_xlen:" \
    "sub    _xlen, ebx" \
    "mov    _clipart, ebx" \
    "jmp    normal_put" \
"new_ylen:" \
    "sub    _ylen, eax" \
"normal_put:" \
    "mov    eax,_y" \
    "mov    _y, ebx" \
    "mov    ebx,_x" \
    "mov    edi,0A0000h"          /* Video-mem... */ \
    "mov    ecx,eax" \
    "shl    eax,8" \
    "shl    ecx,6" \
    "add    eax,ecx"             /* Legg sammen begge Y-ene */ \
    "add    eax,ebx"             /* Legg til X */ \
    "add    edi,eax"             /* Ferdig koordinat ligger nå i EDI */ \
    "mov    edx,320"             /* Kalkuler mengde som må legges til EDI på */ \
    "mov    ebx,_xlen"          /* slutten av hver linje. */ \
    "sub    edx,ebx"          /* Mengde ligger i EDX */ \
    "mov    eax,_ylen"          /* Legg vertikal teller i EAX */ \
"cnext_line:" \
    "mov    ecx,ebx"          /* Legg linje-teller i ECX */ \
    "rep    movsb"               /* Tegn opp en linje.. */ \
    "dec    eax"                 /* En linje mindre.. */ \
    "add    edi,edx"             /* Legg til slik at neste linje begynner under.. */ \
    "add    esi,_clipart" \
    "or     eax,eax"             /* Vertikal teller 0 ennå? */ \
    "jnz    cnext_line"          /* Nei - gå til neste linje */ \
"cend:" \
parm [esi] \
modify [eax ebx ecx edx esi edi];

void bdraw_box(long x, long y, long xlen, long ylen, long colour, byte *scr)
{
    _x = x;
    _y = y;
    _xlen = xlen;
    _ylen = ylen;
    _colour = colour;
    do_bdraw_box(scr);
}

#pragma aux do_bdraw_box = \
    "mov    eax,_y" \
    "mov    ebx,_x" \
    "mov    ecx,eax"       /* Calculate pixel-position.. */ \
    "shl    eax,8" \
    "shl    ecx,6" \
    "add    eax,ecx" \
    "add    eax,ebx" \
    "add    edi,eax" \
    "mov    ebx,320"            /* Kalkuler mengde som må legges til DI på */ \
    "mov    edx,_xlen" \
    "sub    ebx,edx" \
    "mov    eax,_ylen"         /* Legg vertikal teller i AX */ \
    "mov    edx,_colour" \
"next_line:" \
    "mov    ecx,_xlen"        /* Horisontal teller i CX */ \
"next:" \
    "mov    BYTE PTR [edi],dl"    /* Legg en byte til EDI */ \
    "dec    ecx" \
    "inc    edi" \
    "or     ecx,ecx" \
    "jnz    next" \
    "dec    eax" \
    "add    edi,ebx"     /* Legg til slik at neste linje begynner */ \
    "or     eax,eax"     /* Er vertikal teller 0 ennå? */ \
    "jnz    next_line" \
parm [edi] \
modify [eax ebx ecx edx edi];

void put_sprite(long x, long y, long xlen, long ylen, byte *imgpnt)
{
    _x = x;
    _y = y;
    _xlen = xlen;
    _ylen = ylen;
    do_put_sprite(imgpnt);
}

#pragma aux do_put_sprite = \
    "mov     eax,_y" \
    "mov     ebx,_x" \
    "mov     edi,0A0000h"             /* Video-mem... */ \
    "mov     ecx,eax" \
    "shl     eax,8" \
    "shl     ecx,6" \
    "add     eax,ecx"                 /* Legg sammen begge Y-ene */ \
    "add     eax,ebx"                 /* Legg til X */ \
    "add     edi,eax"                 /* Ferdig koordinat ligger nå i EDI */ \
    "mov     edx,320"                 /* Kalkuler mengde som må legges til EDI på */ \
    "mov     ebx,_xlen"              /* slutten av hver linje. */ \
    "sub     edx,ebx"                 /* Mengde ligger i EDX */ \
    "mov     eax,_ylen"              /* Legg vertikal teller i EAX */ \
"bloop1:" \
    "mov     ecx,_xlen"              /* Sett counter til horisontal lengde. */ \
"bloop2:" \
    "mov  bl,[esi]"                   /* Få en byte fra SI */ \
    "or   bl,bl"                    /* Er den 0? */ \
    "je   skip_pixel"                 /* OK - Skip den */ \
    "mov  BYTE PTR [edi],bl"                   /* Nei - Tegn den */ \
    "inc  edi" \
    "inc  esi" \
    "dec  ecx"                        /* En pixel tegnet... */ \
    "or   ecx,ecx"                    /* Er denne linjen ferdig ennå? */ \
    "jne  bloop2"                      /* Nei - kjør gjennom igjen... */ \
    "add  edi,edx"                    /* Legg til slik at neste linje starter under... */ \
    "dec  eax"                        /* Ja - en linje ferdig... */ \
    "or   eax,eax"                    /* Er Vertikal teller null enda? */ \
    "jne  bloop1"                      /* Nei - kjør loop igjen */ \
    "jmp  bend"                        /* Ja - avslutt */ \
"skip_pixel:" \
    "inc  edi"                        /* Hopp over dette koordinatet */ \
    "inc  esi" \
    "dec  ecx"                        /* En pixel tegnet.. */ \
    "or   ecx,ecx"                    /* Er horisontal teller 0? */ \
    "jne  bloop2"                      /* Hopp tilbake hvis linje ikke er ferdig tegnet */ \
    "add  edi,edx"                    /* Legg til slik at neste linje starter under.. */ \
    "or   eax,eax"                    /* Er vertikal teller 0? */ \
    "dec  eax"                        /* En linje mindre.. */ \
    "jnz  bloop1"                      /* Hvis den ikke er 0, initialiser teller på nytt. */ \
"bend:" \
parm [esi] \
modify [eax ebx ecx edx esi edi];

void bput_sprite(long x, long y, long xlen, long ylen, byte *imgpnt, byte *buffer)
{
    _x = x;
    _y = y;
    _xlen = xlen;
    _ylen = ylen;
    do_bput_sprite(imgpnt, buffer);
}

#pragma aux do_bput_sprite = \
    "mov     eax,_y" \
    "mov     ebx,_x" \
    "mov     ecx,eax" \
    "shl     eax,8" \
    "shl     ecx,6" \
    "add     eax,ecx"                 /* Legg sammen begge Y-ene */ \
    "add     eax,ebx"                 /* Legg til X */ \
    "add     edi,eax"                 /* Ferdig koordinat ligger nå i EDI */ \
    "mov     edx,320"                 /* Kalkuler mengde som må legges til EDI på */ \
    "mov     ebx,_xlen"              /* slutten av hver linje. */ \
    "sub     edx,ebx"                 /* Mengde ligger i EDX */ \
    "mov     eax,_ylen"              /* Legg vertikal teller i EAX */ \
"bbloop1:" \
    "mov     ecx,_xlen"              /* Sett counter til horisontal lengde.*/ \
"bbloop2:" \
    "mov  bl,[esi]"                   /* Få en byte fra SI*/ \
    "or   bl,bl"                    /* Er den 0?*/ \
    "je   bskip_pixel"                 /* OK - Skip den*/ \
    "mov  BYTE PTR [edi],bl"                   /* Nei - Tegn den*/ \
    "inc  edi" \
    "inc  esi" \
    "dec  ecx"                        /* En pixel tegnet...*/ \
    "or   ecx,ecx"                    /* Er denne linjen ferdig ennå?*/ \
    "jne  bbloop2"                      /* Nei - kjør gjennom igjen...*/ \
    "add  edi,edx"                    /* Legg til slik at neste linje starter under...*/ \
    "dec  eax"                        /* Ja - en linje ferdig...*/ \
    "or   eax,eax"                    /* Er Vertikal teller null enda?*/ \
    "jne  bbloop1"                      /* Nei - kjør loop igjen*/ \
    "jmp  bbend"                        /* Ja - avslutt*/ \
"bskip_pixel:" \
    "inc  edi"                        /* Hopp over dette koordinatet*/ \
    "inc  esi" \
    "dec  ecx"                        /* En pixel "tegnet"..*/ \
    "or   ecx,ecx"                    /* Er horisontal teller 0?*/ \
    "jne  bbloop2"                      /* Hopp tilbake hvis linje ikke er ferdig tegnet*/ \
    "add  edi,edx"                    /* Legg til slik at neste linje starter under..*/ \
    "or   eax,eax"                    /* Er vertikal teller 0?*/ \
    "dec  eax"                        /* En linje mindre..*/ \
    "jnz  bbloop1"                      /* Hvis den ikke er 0, initialiser teller på nytt*/ \
"bbend:" \
parm [esi] [edi] \
modify [eax ebx ecx edx esi edi];

void block_copy(int srcx, int srcy, int srcxlen, int srcylen, byte *buffer, int destx, int desty)
{
    _srcx = srcx;
    _srcy = srcy;
    _srcxlen = srcxlen;
    _srcylen = srcylen;
    _dstx = destx;
    _dsty = desty;
    do_block_copy(buffer);
}

#pragma aux do_block_copy = \
    "mov    ecx,_srcy" \
    "shl    ecx,6" \
    "mov    eax,_srcy" \
    "shl    eax,8" \
    "add    eax,ecx" \
    "add    eax,_srcx" \
    "add    esi,eax" \
    "mov    eax,_dsty" \
    "mov    edi,0A0000h"         /* Video-minne... */ \
    "mov    ecx,eax" \
    "shl    eax,8" \
    "shl    ecx,6" \
    "add    eax,ecx"             /* Legg sammen begge Y-ene */ \
    "add    eax,_dstx"         /* Legg til X */ \
    "add    edi,eax"             /* Ferdig koordinat ligger nå i EDI */ \
    "mov    edx,320"             /* Kalkuler mengde som må legges til hver av skjermene ved linjeskift */ \
    "mov    ebx,_srcxlen"       /* slutten av hver linje. */ \
    "sub    edx,ebx"             /* Mengde ligger i EDX */ \
    "mov    eax,_srcylen"       /* Legg vertikal teller i EAX */ \
"next_line:" \
    "mov    ecx,ebx"             /* Legg linje-teller i ECX */ \
    "rep    movsb"               /* Tegn opp en linje.. */ \
    "add    esi, edx"            /* Legg til 'linjeskift' på buffer */ \
    "dec    eax"                 /* En linje mindre.. */ \
    "add    edi,edx"             /* Legg til 'linjeskift' på skjerm */ \
    "or     eax,eax"             /* Vertikal teller 0 ennå? */ \
    "jnz    next_line"          /* Nei - gå til neste linje */ \
parm [esi] \
modify [eax ebx ecx edx esi edi];

void block_copy_b(int srcx, int srcy, int srcxlen, int srcylen, byte *buffer, int destx, int desty, byte *destimg)
{
    _srcx = srcx;
    _srcy = srcy;
    _srcxlen = srcxlen;
    _srcylen = srcylen;
    _dstx = destx;
    _dsty = desty;
    do_block_copy_b(buffer, destimg);
}

#pragma aux do_block_copy_b = \
    "mov     ecx,_srcy" \
    "shl     ecx,6" \
    "mov     eax,_srcy" \
    "shl     eax,8" \
    "add     eax,ecx" \
    "add     eax,_srcx" \
    "add     esi,eax" \
    "mov     eax,_dsty"           /* Kalkuler koordinater for skjerm-minne */ \
    "mov     ecx,eax" \
    "shl     eax,8" \
    "shl     ecx,6" \
    "add     eax,ecx"             /* Legg sammen begge Y-ene */ \
    "add     eax,_dstx"         /* Legg til X */ \
    "add     edi,eax"             /* Ferdig koordinat ligger nå i EDI */ \
    "mov     edx,320"             /* Kalkuler mengde som må legges til hver av skjermene ved linjeskift */ \
    "mov     ebx,_srcxlen"       /* slutten av hver linje. */ \
    "sub     edx,ebx"             /* Mengde ligger i EDX */ \
    "mov     eax,_srcylen"       /* Legg vertikal teller i EAX */ \
"next_line:" \
    "mov     ecx,ebx"             /* Legg linje-teller i ECX */ \
    "rep     movsb"               /* Tegn opp en linje.. */ \
    "add     esi, edx"            /* Legg til 'linjeskift' på buffer */ \
    "dec     eax"                 /* En linje mindre.. */ \
    "add     edi,edx"             /* Legg til 'linjeskift' på skjerm */ \
    "or      eax,eax"             /* Vertikal teller 0 ennå? */ \
    "jnz     next_line"          /* Nei - gå til neste linje */ \
parm [esi] [edi] \
modify [eax ebx ecx edx esi edi];
