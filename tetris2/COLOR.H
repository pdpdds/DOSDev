// --------------------------------------------------------------------------
// Project: Tetris Game That Has A Flexible Framework For Further Extension
// Organization: None, Personal
// --------------------------------------------------------------------------
// Class   : Color
// Platform: None Or DOS System
// Purpose : Specified The Color Information Like The Java's
// License : GNU General Public License
// Release : Free Software
// Date    : 1999/12/28
// Author  : Yu Byoung-Hak; Sejong University Computer Engineering Department
// E-Mail  : xover0@hitel.net
// --------------------------------------------------------------------------
// Field Summary:
// - static int black
//        The color black.
// - static int blue
//        The color blue.
// - static int green
//        The color green.
// - static int cyan
//        The color cyan.
// - static int red
//        The color red.
// - static int magenta
//        The color magenta.
// - static int brown
//        The color brown.
// - static int lightGray
//        The color light gray.
// - static int darkGray
//        The color dark gray.
// - static int gray
//        The color gray.
// - static int lightBlue
//        The color light blue.
// - static int lightGreen
//        The color light green.
// - static int lightCyan
//        The color light cyan.
// - static int lightRed
//        The color light red.
// - static int orange
//        The color orange.
// - static int lightMagenta
//        The color light magenta.
// - static int pink
//        The color pink.
// - static int yellow
//        The color yellow.
// - static int white
//        The color white.
// Constructor Summary:
// - Color(int r, int g, int b, int alpha = 0)
//        Creates an RGB color with the specified red, green, blue, and alpha
//        values. (Not Completed)
// - Color(int rgb)
//        Creates an opaque RGB color with the specified value.
// Method Summary:
// --------------------------------------------------------------------------

#ifndef _COLOR_
#define _COLOR_

class Color
{
	private:
		int r;
		int g;
		int b;
		int alpha;
		int rgb;
	public:
		Color(int r, int g, int b, int alpha = 0);
		Color(int rgb);
		operator int();
		static int black;
		static int blue;
		static int green;
		static int cyan;
		static int red;
		static int magenta;
		static int brown;
		static int lightGray;
		static int darkGray;
		static int gray;
		static int lightBlue;
		static int lightGreen;
		static int lightCyan;
		static int lightRed;
		static int orange;
		static int lightMagenta;
		static int pink;
		static int yellow;
		static int white;
};

inline Color::Color(int rgb)
{
	this->rgb = rgb;
}

inline Color::operator int()
{
	return rgb;
}

#endif
