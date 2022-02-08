#include <stdlib.h>
#include "drandom.h"

DRandom::DRandom()
{
	randomize();
}

DRandom::DRandom(long seed)
{
	srand(seed);
}

int DRandom::nextInt()
{
	return random(32767);
}
