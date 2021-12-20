/*  SuperVGA demo program for Turbo C */
/*      By: Thomas Design             */
/*      August 3, 1989                */

/*  Compile with alignment "BYTE"     */
/*   for palette routine              */

#include <math.h>
#include <graphics.h>
#include <alloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#include "SVGA256.H"
#include "VGAEXTRA.H"

void sinepas(void);
void initialize(void);

DACarray Palette_Array;              /* create array to hold DAC values */


/*--------------- Hue Saturation and Value to RGB ----------------*/
void hsv2rgb(float h,float s,float v,RGB *Color)
{
  float h1,f,a[7];
  int   i;
    h1 = h / 60;
    i  = h1;
    f  = h1 - i;
    a[1] = v;
    a[2] = v;
    a[3] = v * (1 - (s*f));
    a[4] = v * (1 - s);
    a[5] = a[4];
    a[6] = v * (1-(s*(1-f)));
    if (i>4) i = i - 4; else i = i + 2;
    Color->Red = a[i];
    if (i>4) i = i - 4; else i = i + 2;
    Color->Green = a[i];
    if (i>4) i = i - 4; else i = i + 2;
    Color->Blue  = a[i];
}

void Build_Chroma(void)
{
	int i;
	int jump=2, min=12, max=60;
	int Cindex = 0;
	int holdr=0, holdg=0, holdb = 0;
	for (i=0; i<9; i++)
	{
		switch (i)
		{
			case 0:		/* set up starting color */
				++Cindex;
				holdr = holdg = holdb = min;
				Palette_Array[Cindex][grn]=min;
				Palette_Array[Cindex][blu]=min;
				Palette_Array[Cindex][red]=min;
				break;
			case 1:
				do
				{	/* add in purple to max */
					++Cindex;
					holdb += jump;
					holdr += jump;
					Palette_Array[Cindex][blu] = holdb;
					Palette_Array[Cindex][grn] = min;
					Palette_Array[Cindex][red] = holdr;
				} while (holdb < max);
				break;
			case 2:
				do
				{	/* remove red] to leave blu] */
					++Cindex;
					holdr -= jump;
					Palette_Array[Cindex][blu] = holdb;
					Palette_Array[Cindex][grn] = holdg;
					Palette_Array[Cindex][red] = holdr;
				} while (holdr > min);
				break;
			case 3:
				do
				{	/* add in grn to get cyan */
					++Cindex;
					holdg += jump;
					Palette_Array[Cindex][grn] = holdg;
					Palette_Array[Cindex][red] = holdr;
					Palette_Array[Cindex][blu] = holdb;
				} while (holdg < max);
				break;
			case 4:
				do
				{	/* remove blu] to leave grn] */
					++Cindex;
					holdb -= jump;
					Palette_Array[Cindex][grn] = holdg;
					Palette_Array[Cindex][red] = holdr;
					Palette_Array[Cindex][blu] = holdb;
				} while (holdb > min);
				break;
			case 5:
				do
				{	/* add in blu] and red] to get white */
					++Cindex;
					holdb += jump;
					holdr += jump;
					Palette_Array[Cindex][grn] = holdg;
					Palette_Array[Cindex][blu] = holdb;
					Palette_Array[Cindex][red] = holdr;
				} while (holdb < max);
				break;
			case 6:
				do
				{	/* remove blu] to leave yellow */
					++Cindex;
					holdb -= jump;
					Palette_Array[Cindex][blu] = holdb;
					Palette_Array[Cindex][grn] = holdg;
					Palette_Array[Cindex][red] = holdr;
				} while (holdb > min);
				break;
			case 7:
				do
				{	/* remove grn] to leave red] */
					++Cindex;
					holdg -= jump;
					Palette_Array[Cindex][blu] = holdb;
					Palette_Array[Cindex][red] = holdr;
					Palette_Array[Cindex][grn] = holdg;
				} while (holdg > min);
			case 8:
				do
				{	/*  remove red] to fade to black */
					++Cindex;
					holdr -= jump;
					Palette_Array[Cindex][blu] = holdb;
					Palette_Array[Cindex][grn] = holdg;
					Palette_Array[Cindex][red] = holdr;
				} while (holdr > min);
		}
	}
}

/* detects EGA or VGA cards */
int huge detectSVGA(void);

/*---------------------- Setup graphics ------------------------*/
void initialize(void)
{
  int    g_driver,  g_mode = 4, g_error;

  int x;
  RGB ColorValue;
  float hue,sat,val;
  installuserdriver("SVGA256", detectSVGA);
#if 0
//g_error = registerbgidriver(svga_fdriver);
/* report any registration errors */
if (g_error < 0)
{
   printf("Graphics error: %s\n", grapherrormsg(g_error));
   exit(1); /* terminate with an error code */
}
#endif
    g_driver = DETECT;                 /* must have this line to use User Driver */
    initgraph(&g_driver,&g_mode,"");
    g_error = graphresult();
    if (g_error != 0) {
       printf("%s \n",grapherrormsg(g_error));
       exit(1);
    }
    Build_Chroma();
    Palette_Array[0][red] = 0;
    Palette_Array[0][blu] = 0;
    Palette_Array[0][grn] = 0;
    dacpalette(&Palette_Array);      /* load DAC registers with new colors */
}

void sinepas(void)
{
    int          x1, x2, y2, y1x2, indx1, indx, istep, iq;
    double       angle;
//    int          color;
    int          xcon,xmax,ymax;

    initialize();
	x1   = getmaxy()/2;
	y1x2 = getmaxy()/2;
	x2   = getmaxx();
	y2   = getmaxy();
	xcon = getmaxx()/2;
//	xmax = getmaxx();
	ymax = getmaxy();
       istep = 1; iq = 1; angle = 0;
//       color = 1;
       indx  = indx1 = 0;
       angle = -3.1415;

do {
    x1 = (cos(angle) * xcon) + xcon;
    y2 = y2 + istep;
    angle = angle + 0.03;
    if (angle > 3.1415) angle = - 3.1415;
    y1x2 = y1x2 + iq;
    if ((y1x2 < 10))  iq =  1;
    if ((y1x2 > ymax)) iq = -1;
    if (y2 < 10)  istep =  1;
    if (y2 > ymax) istep = -1;
    x2 = y1x2 + (sin(angle) * 45);
    setcolor(indx);
    line(x1, y1x2, x2, y2);
    indx  = ++indx;
    indx1 = ++indx1;
    if (indx1 == 2000)
    {
       indx1 = indx = 0;
       cleardevice();
    }
    if (indx > 186) indx = 1;
  } while (!kbhit()) ;
}

/* detects EGA or VGA cards */
int huge detectSVGA(void)
{
   int driver, mode, sugmode = SVGA1024x768x256;
#if 0
   detectgraph(&driver, &mode);
   if ((driver == EGA) || (driver == VGA))
      /* return suggested video mode number */
      return sugmode;
   else
      /* return an error code */
      return grError;
#endif
      /* return suggested video mode number */
      return sugmode;

}

void main(void)
{
    sinepas();
    getch();
    restorecrtmode();
}
