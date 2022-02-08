/*
    Funksjoner for bruk av standard tekst i alle VGA-moduser

    Sync 7/6-97
*/

#include "standard.h"
#include <string.h>
#include <conio.h>

void charprint(long NUM);
void numprint(long NUM, short x, short y);
void textprint(char *text, short x, short y);
void ctextprint(char *text, short x, short y, short length); // Skriver ut sentrert tekst
void fnumprint(long NUM, short x, short y, short flength); // Skriver ut formaterte tall
void set_cursor(long col, long row);
void cls();
void print_character(byte col, byte row, char bokstav, byte bg_colour, byte fg_colour);
void print_line(byte row, char *line, byte bg_colour, byte fg_colour);
void textpixel(int column, int row, byte col);
void set_textpal(signed char *palette);
void set_tp_entry(byte r, byte g, byte b, int entrynum);
void set_palreg(byte colval, byte palregnum);
void print_char(byte *adr, char bokstav, byte bg_colour, byte fg_colour);
void set_redpalette();



byte *text_screen;



#pragma aux print_line =                                            \
    "mov    edi, 0xb8000h"  /* Video-memory */                      \
    "shl    ecx, 4"         /* Shift bg-farge 4 til venstre */      \
    "add    edx, ecx"       /* EDX inneholder nå bg + fg farge */   \
    "mov    ah, dl"         /* Flytt bg + fg til øvre del av AX */  \
    "mov    ecx, 0"         /* Clear ECX */                         \
    "mov    cl, bh"         /* ROW til nedre CX... */               \
    "mov    edx, ecx"       /* ROW til nedre DX... */               \
    "shl    ecx, 7"         /* Ganger ROW med 128 */                \
    "shl    edx, 5"         /* Ganger ROW med 32 */                 \
    "add    ecx, edx"       /* Ganger tilsammen ROW med 160 */      \
    "add    edi, ecx"       /* Legg offset til video-mem adresse */ \
    "next_char:"                                                    \
    "mov    al, BYTE PTR [esi]"                                     \
    "or     al, al"         /* Er linjen slutt? */                  \
    "jz     end"            /* Ja - hopp ut */                      \
    "mov    [edi], ax"      /* Nei - tegn til skjermen */           \
    "add    BYTE PTR edi, 2"                                        \
    "inc    esi"            /* ESI++ */                             \
    "jmp    next_char"                                              \
    "end:"                                                          \
modify [eax edi]                                                    \
parm [bh] [esi] [ecx] [edx];

#pragma aux print_character =               \
"mov    edi, 0xb8000h"                                     \
"shl    ecx, 4"                 /* Kalkuler farge-verdi */ \
"add    edx, ecx"               /* legg bgcol til fgcol */ \
"mov    ah, dl"                 /* fargeverdi ligger i ah */ \
"mov    ecx, 0"                 /* clear bgcol */ \
"mov    cl, bh"                 /* flytt row til cl */ \
"shl    ecx, 7"                 /* gange... */ \
"add    edi, ecx"               /* adresse i edi */ \
"mov    edx, 0"\
"mov    dl, bh" \
"shl    edx, 5"  \
"add    edi, edx" \
"and    ebx, 0x000000FF"\
"shl    ebx, 1"\
"add    edi, ebx"\
"mov    [edi], ax"              /* tegn bokstav og farge-attributt */ \
modify [eax edi]                \
parm [bl] [bh] [al] [ecx] [edx];

#pragma aux print_char =               \
"shl    ecx, 4"                 /* Kalkuler farge-verdi */ \
"add    edx, ecx"               /* legg bgcol til fgcol */ \
"mov    ah, dl"                 /* fargeverdi ligger i ah */ \
"mov    ecx, 0"                 /* clear bgcol */ \
"mov    cl, bh"                 /* flytt row til cl */ \
"mov    edx, ecx"               /* kopier row i edx */ \
"mov    [edi], ax"              /* tegn pixel og farge-atributt */ \
modify [eax]                \
parm [edi] [al] [ecx] [edx];

/*
"mov    bh, 0"
*/
#pragma aux text_pixel =               \
"mov    edi, 0xb8000"                                \
"mov    ecx, ebx"   \
"shl    ecx, 7"     \
"shl    ebx, 5"     \
"add    ecx, ebx"   \
"shl    edx, 1"      \
"mov    dh, 0"                  /* clear row */ \
"mov    al, 0xdb"   \
"add    ecx, edx"      \
"add    edi, ecx"   \
"mov    [edi], ax"                                         \
modify [eax ecx edi]                \
parm [edx] [ebx] [eax];

#pragma aux charprint =              /* eax = NUM */ \
"mov    ah, 09h"                                     \
"mov    bl, 14"                /* forgrunns-farge */ \
"mov    bh, 0"                 /* bakgrunns-farge */ \
"mov    cx, 1"                       /* 1 bokstav */ \
"int    10h"                                         \
modify [ebx ecx]    \
parm [eax];

#pragma aux set_cursor =                            \
"mov    ah, 02h"                                    \
"mov    dh, bl"               /* row-number */      \
"mov    dl, al"              /* column-number */    \
"mov    bh, 0"              /* Use page-number 0 */ \
"int    10h"                                        \
modify [edx]                                        \
parm [eax] [ebx];

#pragma aux cls =               \
    "mov    edi, 0xb8000"       \
    "add    edi, 160"           \
    "mov    ecx, 1840"           \
    "mov    edx, 1"         \
    "shl    edx, 12"         \
    "cls_next:"                 \
    "mov    [edi], dx"  \
    "dec    ecx"             \
    "add    BYTE PTR edi, 2"            \
    "cmp    ecx, 0"         \
    "jnz cls_next"      \
    modify [edi ecx];

/*
    Funksjon for å vise et tall på skjermen i hvilken som helst
    grafikk modus.

    Tallet må være max 9 siffer!
*/

void numprint(long NUM, short x, short y)
{
    if (NUM > 0) {

    int numlist[10];

    int i;

    set_cursor(x, y);

    for (i=0; i<10; i++)
    {
        numlist[i]=0;
    }
    
    int numindex = 0;

    if (NUM < 10)
    {
        charprint(48+NUM);
    }
    else {
        numlist[numindex] = NUM%10;
        NUM = NUM/10;
        numindex++;

        while (NUM > 0 || numlist[numindex] > 0)
        {
            numlist[numindex] = NUM%10;
            NUM = NUM/10;
            numindex++;
        }

    for (i=(numindex-1); i>=0; i--)
    {
        charprint(48+numlist[i]);
        set_cursor(++x, y);
    }

    } // else
} // IF
}

void fnumprint(long NUM, short x, short y, short flength)
{
    if (NUM > 0) {

    short numlist[10];

    short i;
    short offset;

    set_cursor(x, y);

    for (i=0; i<10; i++)
    {
        numlist[i]=0;
    }

    int numindex = 0;

    if (NUM < 10)
    {
        set_cursor(x+(flength-2), y);
        charprint(32);                  // En blank først
        set_cursor(x+(flength-1), y);
        charprint(48+NUM);              // Deretter det enkle tallet
    }
    else {
        numlist[numindex] = NUM%10;
        NUM = NUM/10;
        numindex++;

        while (NUM > 0 || numlist[numindex] > 0)
        {
            numlist[numindex] = NUM%10;
            NUM = NUM/10;
            numindex++;
        }

    // Formater tekst
    offset = flength - numindex;
    x += offset;
    
    set_cursor(x,y);

    for (i=(numindex-1); i>=0; i--)
    {
        charprint(48+numlist[i]);
        set_cursor(++x, y);
    }

    } // else
} // IF
}

void textprint(char *text, short x, short y)
{
    int i = 0;

    set_cursor(x, y);

    while (text[i] != 0) {
        charprint((long)text[i++]);
        set_cursor(++x, y);
    }
}

void ctextprint(char *text, short x, short y, short length)
{
    short textlen = strlen(text);
    short offset = (length/2) - (textlen)/2;

    int i=0;
    x+=offset;
    set_cursor(x, y);

    while (text[i] != 0) {
        charprint((long)text[i++]);
        set_cursor(++x, y);
    }
}

#pragma aux set_textpal =                                               \
    "mov    al, 0"                                                      \
    "mov    edx, 3C8h"                                                  \
    "cli"                                                               \
    "out    dx, al"         /* Start på farge nr. 0                  */ \
    "mov    ecx, 16"        /* 16 farger skal settes...              */ \
    "mov    dx, 3c9h"       /* Color-data register i dx, for outsb.. */ \
    "aloop:"                                                            \
    "outsb"                 /* R                                     */ \
    "outsb"                 /* G                                     */ \
    "outsb"                 /* B                                     */ \
    "dec    ecx"                                                        \
    "jnz    aloop"          /* Nei - neste farge                     */ \
    "sti"                                                               \
    modify [EAX EDX ECX]                                                \
    parm [ESI];


void set_tp_entry(byte r, byte g, byte b, int entrynum)
{
    outp(0x3c8, entrynum);
    outp(0x3c9, r);
    outp(0x3c9, g);
    outp(0x3c9, b);
}

#pragma aux set_palreg =    \
    "mov    ax, 0x1000"          \
    "int    0x10"           \
    modify [EAX]            \
    parm [bh] [bl];

void text_colourbar(byte x, byte offset)
{
    byte i=0, ii=0;
    text_screen = (byte *)0x000b8000;
    text_screen += (i * 160) + (x * 2);

    for (i=0; i<25; i++) {
        if ((ii+offset) > 15) {
            ii = 0;
            offset = 0;
        }
        print_char(text_screen, 219, 0, (ii+offset));
        text_screen += 160;
        ii++;
    }
}

void set_redpalette()
{
    int i;
    byte r, g, b, o;
    signed char ri, gi, bi;

    r = 63;
    g = 0;
    b = 0;

    ri = -4;
    gi = 0;
    bi = 0;

    // Ordne pal-registre
    for (i = 0; i < 16; i ++)
        set_palreg(i, i);

    for (i = 15; i >= 0; i --) {
        set_tp_entry(r, g, b, i);
        r += ri;
        g += gi;
        b += bi;
    }
}


