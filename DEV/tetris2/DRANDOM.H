// --------------------------------------------------------------------------
// Project: Tetris Game That Has A Flexible Framework For Further Extension
// Organization: None, Personal
// --------------------------------------------------------------------------
// Class   : DRandom Inherited From Random
// Platform: DOS System
// Purpose : Random Number Generator
// License : GNU General Public License
// Release : Free Software
// Date    : 1999/12/28
// Author  : Yu Byoung-Hak; Sejong University Computer Engineering Department
// E-Mail  : xover0@hitel.net
// --------------------------------------------------------------------------
// Constructor Summary:
// - DRandom()
//        Creates a new random number generator.
// - DRandom(long seed)
//        Creates a new random number generator using a single long seed:
// Method Summary:
// - int nextInt()
//        Returns the next random number from this random number generator's
//        sequence. Overrode from the class Random.
// --------------------------------------------------------------------------

#ifndef _DRANDOM_
#define _DRANDOM_

#include "random.h"

class DRandom : public Random
{
	private:
	public:
		DRandom();
		DRandom(long seed);
		int nextInt();
};

#endif
