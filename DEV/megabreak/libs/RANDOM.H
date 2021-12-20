/*
    Random-funksjoner

    12/2-97
*/

#include <stdlib.h>
#include <time.h>

short random(short start, short length);
double frandom(double start, double end);
void randomize();

short random(short start, short length)
{
    double value;

    value = ((rand()/32767.0)*length)+start;

    return value;
}

double frandom(double start, double end)
{
    double value, length;

    length = end - start;
    value = ((rand()/32767.0)*length)+start;

    return value;
}

void randomize()
{
    auto struct tm rndtime;
    time_t currtime;
    unsigned short seed;

    currtime = time(NULL);
    _gmtime(&currtime, &rndtime);

    seed = rndtime.tm_sec * rndtime.tm_min;

    srand(seed);
}

