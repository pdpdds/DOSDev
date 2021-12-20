// --------------------------------------------------------------------------
// Project: Tetris Game That Has A Flexible Framework For Further Extension
// Organization: None, Personal
// --------------------------------------------------------------------------
// Class   : DCanvas Inherited From Canvas
// Platform: DOS System
// Purpose : Implemented A Graphics Class Like The java.awt.Graphics For DOS
// License : GNU General Public License
// Release : Free Software
// Date    : 1999/12/28
// Author  : Yu Byoung-Hak; Sejong University Computer Engineering Department
// E-Mail  : xover0@hitel.net
// --------------------------------------------------------------------------
// Constructor Summary:
// - DCanvas(DGraphicDevice * graphicDriver, int x, int y, int width, int height)
//        Constructs a new canvas for DOS with the DGraphicDevice in given area.
// Method Summary:
// - void clearRect(int x, int y, int width, int height)
//        Clears the specified rectangle by filling it with the color black.
//        Overrode from the class Canvas.
// - void drawRect(int x, int y, int width, int height)
//        Draws the outline of the specified rectangle.
//        Overrode from the class Canvas.
// - void fillRect(int x, int y, int width, int height)
//        Fills the specified rectangle. Overrode from the class Canvas.
// - void drawString(const String & string, int x, int y)
//        Draws the text given by the specified string, using this graphics
//        context's current font and color. Overrode from the class Canvas.
// Methods inherited from class Canvas:
// - void setColor(const Color & color)
// - Color getColor()
// - void setFont(const Font & font)
// - Font getFont()
// --------------------------------------------------------------------------

#ifndef _DCANVAS_
#define _DCANVAS_

#include "canvas.h"
#include "dgdevice.h"

class DCanvas : public Canvas
{
	private:
		DGraphicDevice * graphicDriver;
		int x;
		int y;
		int width;
		int height;
	public:
		DCanvas(DGraphicDevice * graphicDriver, int x, int y, int width, int height);
		~DCanvas();
		void clearRect(int x, int y, int width, int height);
		void drawRect(int x, int y, int width, int height);
		void fillRect(int x, int y, int width, int height);
		void drawString(const String & string, int x, int y);
};

#endif
