// --------------------------------------------------------------------------
// Project: Tetris Game That Has A Flexible Framework For Further Extension
// Organization: None, Personal
// --------------------------------------------------------------------------
// Class   : DGraphicDevice
// Platform: DOS System
// Purpose : Graphic Device For DOS System
// License : GNU General Public License
// Release : Free Software
// Date    : 1999/12/28
// Author  : Yu Byoung-Hak; Sejong University Computer Engineering Department
// E-Mail  : xover0@hitel.net
// --------------------------------------------------------------------------
// Constructor Summary:
// - DGraphicDevice()
//        Constructs a dos graphic device with the highest resolution
//        available for the detected driver.
// Method Summary:
// - void clear()
//        Clears the graphics screen.
// - int getErrorCode()
//        Returns an error code for the last unsuccessful graphics operation.
// --------------------------------------------------------------------------

#ifndef _DGRAPHICDEVICE_
#define _DGRAPHICDEVICE_

#include <graphics.h>

class DGraphicDevice
{
	private:
		int driver;
		int mode;
		int error;
	public:
		DGraphicDevice();
		~DGraphicDevice();
		void clear();
		int getErrorCode();
};

#endif
