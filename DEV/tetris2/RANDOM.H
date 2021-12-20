// --------------------------------------------------------------------------
// Project: Tetris Game That Has A Flexible Framework For Further Extension
// Organization: None, Personal
// --------------------------------------------------------------------------
// Class   : Random
// Platform: None
// Purpose : Abstract Class To Specify A Random Number Generator
// License : GNU General Public License
// Release : Free Software
// Date    : 1999/12/28
// Author  : Yu Byoung-Hak; Sejong University Computer Engineering Department
// E-Mail  : xover0@hitel.net
// --------------------------------------------------------------------------
// Constructor Summary:
// Method Summary:
// - abstract virtual int nextInt()
//        Returns the next random number from this random number generator's
//        sequence.
// --------------------------------------------------------------------------

#ifndef _RANDOM_
#define _RANDOM_

class Random
{
	private:
	public:
		virtual int nextInt() = 0;
};

#endif
