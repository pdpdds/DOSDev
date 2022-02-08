// --------------------------------------------------------------------------
// Project: Tetris Game That Has A Flexible Framework For Further Extension
// Organization: None, Personal
// --------------------------------------------------------------------------
// Class   : Font
// Platform: None
// Purpose : Represented Font Class Like The Java's
// License : GNU General Public License
// Release : Free Software
// Date    : 1999/12/28
// Author  : Yu Byoung-Hak; Sejong University Computer Engineering Department
// E-Mail  : xover0@hitel.net
// --------------------------------------------------------------------------
// Constructor Summary:
// - Font(const String & name, int style, int size)
//        Creates a new Font from the specified name, style and point size.
// Method Summary:
// - String getName()
//        Returns the name of this Font.
// - int getStyle()
//        Returns the style of this Font.
// - int getSize()
//        Returns the point size of this Font.
// --------------------------------------------------------------------------

#ifndef _FONT_
#define _FONT_

#include "string.h"

class Font
{
	private:
		String name;
		int style;
		int size;
	public:
		Font(const String & name, int style, int size);
		String getName();
		int getStyle();
		int getSize();
};

inline Font::Font(const String & name, int style, int size)
{
	this->name = name;
	this->style = style;
	this->size = size;
}

inline String Font::getName()
{
	return name;
}

inline int Font::getStyle()
{
	return style;
}

inline int Font::getSize()
{
	return size;
}

#endif
