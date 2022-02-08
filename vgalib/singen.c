#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif // !M_PI


void main(void)
{
	double sinma;
	double cosma;
	int angle;

	printf("#ifndef SINCOS_H\n#define SINCOS_H\n");
	printf("int sindeg[] = {\n");
	for (angle=0;angle<361;angle++) {
		sinma = sin(-angle*M_PI/180);

		printf("%ld%s // %d %f\n", (long)(sinma*16384),angle<360?",":"",angle,sinma);
	}

	printf("};\n");
	printf("int cosdeg[] = {\n");
	for (angle=0;angle<361;angle++) {
		cosma = cos(-angle*M_PI/180);

		printf("%ld%s // %d %f\n",(long)(cosma*16384),angle<360?",":"",angle,cosma);
	}
	printf("};\n#endif\n");
}
