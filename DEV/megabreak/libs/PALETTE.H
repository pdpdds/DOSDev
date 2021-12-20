/*
    Palette-functions

    Part of Knut's GFX-Lib for Watcom

    1/4-97
*/

static signed char fadesteps[768];
static signed char black_pal[768];
static signed char white_pal[768];

void init_fades();                          /* Initialiserer fade-rutinene */
void set_pal(signed char *palette);
void calc_fadesteps(signed char *src_palette, signed char *dst_palette, long num_steps);
void do_fadestep(signed char *palette);        /* Gjør et fade-step */
void do_fade(long num_steps, signed char *palette);             /* Gjør alle fade-steps */
void do_simple_fadestep(long stepvalue, signed char *palette);
void do_simple_fade(long num_steps, long stepvalue, signed char *palette);

void init_fades()
{
    int i;

    for (i=0; i<768; i++) {
        fadesteps[i] = 0;
        black_pal[i] = 0;
        white_pal[i] = 64;
    }
}

void calc_fadesteps(signed char *src_palette, signed char *dst_palette, long num_steps)
{
    int i;

    for (i=0; i<768; i++) {
        fadesteps[i] = (dst_palette++ - src_palette++)/num_steps;
        if (fadesteps[i] < 1)
            fadesteps[i] = 1;
    }
}

void do_fadestep(signed char *palette)
{
    int i;

    for (i=0; i<768; i++) {
        palette[i] -= fadesteps[i];
        if (palette[i] <= 0)
            palette[i] = 0;
        if (palette[i] >= 63)
            palette[i] = 63;
    }
    wait_vbl();
    set_pal(palette);
}

void do_fade(long num_steps, signed char *palette)
{
    int i;

    for (i=0; i<num_steps; i++)
        do_fadestep(palette);
}

void do_simple_fadestep(long stepvalue, signed char *palette)
{
    int i;

    for (i=0; i<768; i++) {
        palette[i] += stepvalue;
        if (palette[i] < 0)
            palette[i] = 0;
        if (palette[i] >= 63)
            palette[i] = 63;
    }
    wait_vbl();
    set_pal(palette);
}

void do_simple_fade(long num_steps, long stepvalue, signed char *palette)
{
    int i;

    for (i=0; i<num_steps; i++)
        do_simple_fadestep(stepvalue, palette);
}

#pragma aux set_pal =                                                   \
    "mov    al, 0"                                                      \
    "mov    edx, 3C8h"                                                  \
    "cli"                                                               \
    "out    dx, al"         /* Start på farge nr. 0                  */ \
    "mov    ecx, 256"       /* 256 farger skal settes...             */ \
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

