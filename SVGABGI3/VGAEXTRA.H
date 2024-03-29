#include <dos.h>                       /* pick up definition for __emit__ */

 typedef struct {                      /* RGB structure */
    char Red;
    char Green;
    char Blue;
 } RGB;
 typedef char DACarray[256][3];        /* Array to hold DAC register values */
 enum { red=0,grn=1,blu=2 };           /* possible array indices for R, G, B */

void far flashmodeon(void)    /* blank screen and put VGA into fast mode */
{
__emit__(0xba,0xc4,0x03,0xb0,0x01,0xee,0x42,0xec,0x0c,0x20,0xee,0xba,0xda,0x03);
}

void far flashmodeoff(void)   /* display screen and put VGA into normal mode */
{
__emit__(0xba,0xc4,0x03,0xb0,0x01,0xee,0x42,0xec,0x24,0xdf,0xee,0xba,0xda,0x03);
}

void far dacpalette(DACarray far *Pal) /* load all 256 DAC registers */
{
__emit__(0x1E,0xC5,0x76,0x06,0xBA,0xC8,0x03,0xB0,0x00,0xEE,0x42,0xFA,0xFC,0x52,
         0xB9,0x80,0x01,0xBA,0xDA,0x03,0xEC,0x24,0x08,0x75,0xFB,0xEC,0x24,0x08,
         0x74,0xFB,0x5A,0x54,0x58,0x3B,0xC4,0x75,0x18,0xF3,0x6E,0x52,0xB9,0x80,
         0x01,0xBA,0xDA,0x03,0xEC,0x24,0x08,0x75,0xFB,0xEC,0x24,0x08,0x74,0xFB,
         0x5A,0xF3,0x6E,0xEB,0x0D,0x33,0xC9,0xAD,0xEE,0x8A,0xC4,0xEE,0xAC,0xEE,
         0xFE,0xC1,0x75,0xF5,0xFB,0x1F);
}
