/*
 *  SyNc's Mouse-library V 1.0 for Watcom
 *  By Q - 1995!
 *
 *  14/12-95
 */

#include "standard.h"

// Funksjons prototyper :

int check_mouse();
void show_mouse();
void hide_mouse();
void get_mouse_status(); // MOUSE_X, MOUSE_Y, BUTTON_STATE
int get_mouse_info(); // MOUSE_TYPE, MOUSE_IRQ
void limit_mouse(long x, long y, long x2, long y2);
void xlimit_mouse(long X1, long X2);
void ylimit_mouse(long Y1, long Y2);
void reset_mouse();
void set_mouse_sens(int x_mickeys, int y_mickeys);
byte get_colour(long X, long Y);

short MOUSE_X, MOUSE_Y, BUTTON_STATE, MOUSE_TYPE, MOUSE_IRQ = 0;

/*
	Funksjon: check_mouse
	Parametre: ingen
	Returnerer: 1 hvis mus finnes
                0 hvis mus ikke finnes
	Brief:
		Funksjonen sjekker om musa finnes.
*/

#pragma aux check_mouse =   \
    "mov    eax,0x0000"     /* Funksjons-nummer                     */ \
    "int    0x33"           /* Interrupt 33..                       */ \
    "or     eax,eax"        /* Sjekk om AX er 0..                   */ \
    "jz     no_mouse"       /* Hvis den er det, er ingen mus inne.. */ \
    "mov    ebx,1"          /* Mus finnes - returner 1 i EBX        */ \
    "jmp    end"            \
    "no_mouse:"             \
    "mov    ebx,0"          /* Mus finnes ikke - returner 0 i EBX   */ \
    "end:"                  \
    modify[EAX]             \
    value[EBX];

/*
	Funksjon: show_mouse
	Parametre: ingen
	Returnerer: ingenting
	Brief:
		Funksjonen viser musa på skjermen.
*/

#pragma aux show_mouse =    \
    "mov    eax,0x0001"     \
    "int    0x33"           \
    modify[EAX];

/*
	Funksjon: hide_mouse
	Parametre: ingen
	Returnerer: ingenting
	Brief:
		Funksjonen tar muse-pekeren bort fra skjermen.
*/

#pragma aux hide_mouse =    \
    "mov    eax,0x0002"     \
    "int    0x33"           \
    modify[EAX];

/*
	Funksjon: get_mouse_status
    Parametre: mouse_x oppdateres til å inneholde nytt X-koordinat
               mouse_y oppdateres til å inneholde nytt Y-koordinat
               button_state forteller om status'en til museknappene:
                    0 - ingen knapper
                    1 - venstre museknapp
                    2 - høyre museknapp
    Returnerer: ingenting
	Brief:
		Funksjonen oppdaterer muse-koordinatene (MOUSE_X, MOUSE_Y), og sjekker
		om noen av knappene har blitt nedtrykket.
        NBULF! Denne funksjonen sjekker ikke om begge knappene har blitt
		nedtrykket.
*/

#pragma aux get_mouse_status =  \
    "mov    eax,0x0003"         /* Funksjon: Få muse-status */ \
    "int    0x33"               \
    "mov    [MOUSE_X],cx"        \
    "mov    [MOUSE_Y],dx"        \
    "mov    [BUTTON_STATE],bx"   \
    modify [EAX CX DX BX];

/*
	Funksjon: get_mouse_info
    Parametre:  mouse_irq       -       IRQ nummer blir returnert her
                mouse_type      -       Musetypen blir returnet her

    Returnerer: 0 hvis muse-driver ikke støtter denne funksjonen
                1 hvis muse-driver støtter funksjonen
    Brief:
		Funksjonen sjekker først om muse-driveren støtter denne funksjonen.
        Hvis den gjør det legger den informasjonen i variabler.
*/

#pragma aux get_mouse_info  =   \
    "mov    eax,0x0024"         \
    "mov    ebx,0xffff"       /* Sjekk om driver støtter denne funksjonen */ \
    "int    0x33"               \
    "cmp    ebx,0xffff"       /* Hvis ikke, gå ut av funksjonen */ \
    "je     no_support"         \
    "jmp    support"            \
    "mov    BYTE PTR [MOUSE_IRQ],cl"     \
    "mov    BYTE PTR [MOUSE_TYPE],ch"    \
    "no_support:"               \
    "mov    edx,0"              \
    "jmp    end"                \
    "support:"                  \
    "mov    edx,1"              \
    "end:"                      \
    modify [EAX EBX CL CH]      \
    value [EDX];

/*
	Funksjon: reset_mouse
	Parametre: ingen
	Returnerer: ingenting
	Brief:
		Funksjonen reset'er musa slik som den er før programmet starter.
		Denne funksjonen må kalles på slutten av et program, ellers vil musa
		slutte å virke.
*/

#pragma aux reset_mouse =   \
    "mov    eax,0x0000"     \
    "int    0x33"           \
    modify [EAX];

/*
    Funksjoner: xlimit_mouse og ylimit_mouse
	Parametre: int x  - X koordinat
               int y  - Y koordinat
               int x2 - Andre X koordinat
               int y2 - Andre Y koordinat
	Returnerer: ingenting
	Brief:
        Funksjonene gjør at musa ikke kan flyttes over et område definert av
		x,y - x2,y2.
        Før var dette en funksjon men fordi den ble laget i in-line Watcom
        ASM ble den delt opp i to funksjoner for å unngå parm-overflow..
*/

#pragma aux xlimit_mouse   =                              \
    "mov     eax,0x0007"    /* Funksjon: Sett X-limit */    \
    "int     0x33"                                          \
    modify [EAX]                                            \
    parm [ECX] [EDX];
    
#pragma aux ylimit_mouse   =                              \
    "mov     eax,0x0008"    /* Funksjon: Sett Y-limit */    \
    "int     0x33"                                          \
    modify [EAX]                                            \
    parm [ECX EDX];

/*
    Funksjon: limit_mouse
    Parametre: x, y, x2, y2
    Returnerer: ingenting
    Brief:
        Denne funksjonen gjør limit-funksjonene enklere å bruke ved å kombinere
        dem i en funksjon.
*/

void limit_mouse(long x, long y, long x2, long y2)
{
    xlimit_mouse(x,x2);
    ylimit_mouse(y,y2);
}

/*
	Funksjon: set_mouse_sens
	Parametre: int x_mickeys  - Antall mickeys som må til for å røre mus 8
                                pixels horisontalt.
               int y_mickeys  - Samme i vertikalt.
	Returnerer: ingenting
	Brief:
		Funksjonen setter sensitiviteten på musa i mickeys.
*/

#pragma aux set_mouse_sens  =   \
    "mov     eax,0x000f"        \
    "int     0x33"              \
    modify [EAX]                \
    parm [ECX] [EDX];

#pragma aux get_colour      =                                       \
    "mov     edi,0A0000h"       /* Video-mem.. */                   \
    "mov     ecx,eax"           /* Calculate pixel-position.. */    \
    "shl     eax,8"                                                 \
    "shl     ecx,6"                                                 \
    "add     eax,ecx"                                               \
    "add     eax,ebx"                                               \
    "add     edi,eax"                                               \
    "mov     cl,[edi]"         /* put colour-value into CL */       \
    modify [EDI]                                                    \
    parm [EBX] [EAX]                                                \
    value [CL];

