// --------------------------------------------------------------------------
// Project: Tetris Game That Has A Flexible Framework For Further Extension
// Organization: None, Personal
// --------------------------------------------------------------------------
// Class   : Canvas
// Platform: None
// Purpose : Canvas Class Specification To Implement java.awt.Graphics
// License : GNU General Public License
// Release : Free Software
// Date    : 1999/12/28
// Author  : Yu Byoung-Hak; Sejong University Computer Engineering Department
// E-Mail  : xover0@hitel.net
// --------------------------------------------------------------------------
// Constructor Summary:
// - Canvas()
//        Constructs a new canvas object.
// Method Summary:
// - abstract virtual void clearRect(int x, int y, int width, int height)
//        Clears the specified rectangle by filling it with the color black.
// - abstract virtual void drawRect(int x, int y, int width, int height)
//        Draws the outline of the specified rectangle.
// - abstract virtual void fillRect(int x, int y, int width, int height)
//        Fills the specified rectangle.
// - abstract virtual void drawString(const String & string, int x, int y)
//        Draws the text given by the specified string, using this graphics
//        context's current font and color.
// - void setColor(const Color & color)
//        Sets this graphics context's current color to the specified color.
// - Color getColor()
//        Gets this graphics context's current color.
// - void setFont(const Font & font)
//        Sets this graphics context's font to the specified font.
// - Font getFont()
//        Gets the current font.
// --------------------------------------------------------------------------

#ifndef _CANVAS_
#define _CANVAS_

#include "color.h"
#include "font.h"
#include "string.h"

class Canvas
{
	protected:
		Color color;
		Font font;
	public:
		Canvas();
		virtual ~Canvas();
		virtual void clearRect(int x, int y, int width, int height) = 0;
		virtual void drawRect(int x, int y, int width, int height) = 0;
		virtual void fillRect(int x, int y, int width, int height) = 0;
		virtual void drawString(const String & string, int x, int y) = 0;
		void setColor(const Color & color);
		Color getColor();
		void setFont(const Font & font);
		Font getFont();
};

inline void Canvas::setColor(const Color & color)
{
	this->color = color;
}

inline Color Canvas::getColor()
{
	return color;
}

inline void Canvas::setFont(const Font & font)
{
	this->font = font;
}

inline Font Canvas::getFont()
{
	return font;
}

#endif
