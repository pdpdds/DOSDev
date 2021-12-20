/***********************++********/
/* Operating System: MS-DOS 6.2  */
/*                               */
/* Compiler: Turbo C v2.0        */
/*                               */
/* by onlinestore (DOS MUSEUM)   */
/*********************************/


#include <stdlib.h>
#include <graphics.h>
#include <time.h>

#define CENTER_X  320
#define CENTER_Y  240

#define NUM_STAR  300

struct StarClass {

   float  x, y;
   float  z;
   float  velocity;

   int    pos_x, pos_y;
   int    radius;
   int    color;

} STAR[NUM_STAR];


void initstar(int i)
{

      if ( random(2) ) {
		STAR[i].x = random(500);
      } else
		STAR[i].x = random(500) * -1;

      if ( random(2) ) {
		STAR[i].y = random(500);
      } else
		STAR[i].y = random(500) * -1;

	  STAR[i].z = random(900) + 100;
	  STAR[i].velocity = (float)(random(45) + 5) / 10.0;

	  STAR[i].radius = random(2) ? random(2) : 0;
	  if( STAR[i].radius != 0 ) {
		  STAR[i].radius = random(2) ? random(2) : 0;
		  if ( STAR[i].radius != 0 ) {
			 STAR[i].radius = random(3) ? random(3) : 0;
		  }
	  }

	  STAR[i].color = random(15) + 1;

}

int main(int argc, char **argv)
{

   time_t t;
   unsigned int sval, delaytime;
   int gd=DETECT, gm, i;

   if ( argc < 2) {
	   printf("\n Usage] ST3 delaytime(0-20)\n\n");
	   printf("\tC:>ST3 5\n");
	   exit(1);
   }

   initgraph(&gd, &gm, "");

   delaytime = atoi(argv[1]);
   if(delaytime > 20) delaytime = 20;

   sval = (unsigned) time(&t);
   srand(sval);

   for (i=0; i< NUM_STAR ; i++ ) initstar(i);

   while( !kbhit() ) {

	  for(i=0; i < NUM_STAR; i++)  {
		 setcolor(BLACK);
		 if (STAR[i].radius == 0) {
			 putpixel(STAR[i].pos_x, STAR[i].pos_y, BLACK);
		 } else {
			 circle(STAR[i].pos_x, STAR[i].pos_y, STAR[i].radius);
			 setcolor(STAR[i].color);
		 }

		 STAR[i].z = STAR[i].z - STAR[i].velocity;

		 STAR[i].pos_x = STAR[i].x / STAR[i].z * 100 + CENTER_X;
		 STAR[i].pos_y = STAR[i].y / STAR[i].z * 100 + CENTER_Y;

		 if (STAR[i].pos_x < 1 || STAR[i].pos_x > 640 || \
			 STAR[i].pos_y < 1 || STAR[i].pos_y > 480 || \
			 STAR[i].z < 1 	)
		 {
			initstar(i);
		 }

		 if(STAR[i].radius == 0) {
		   putpixel(STAR[i].pos_x, STAR[i].pos_y, STAR[i].color);
		 } else {
		   circle(STAR[i].pos_x, STAR[i].pos_y, STAR[i].radius);
		 }

		 delay(delaytime);
      }
   }

   getch();
   closegraph();
   exit(0);
}