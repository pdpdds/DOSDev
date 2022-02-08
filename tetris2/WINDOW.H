// --------------------------------------------------------------------------
// Project: Tetris Game That Has A Flexible Framework For Further Extension
// Organization: None, Personal
// --------------------------------------------------------------------------
// Class   : Window
// Platform: None
// Purpose : Window Area To Draw A Main Window Of The Tetris Game
// License : GNU General Public License
// Release : Free Software
// Date    : 1999/12/28
// Author  : Yu Byoung-Hak; Sejong University Computer Engineering Department
// E-Mail  : xover0@hitel.net
// --------------------------------------------------------------------------
// Field Summary:
// - Canvas * canvas
//        A canvas object for drawing work.
// - int width
//        The width of this window.
// - int height
//        The height of this window.
// Constructor Summary:
// - Window(Canvas * canvas, int width, int height)
//        Constructs a new window with the canvas in the given size.
// Method Summary:
// - void setBlock(Block * block, int row, int col)
//        Sets a block on this window to draw the next object at the given
//        position.
// - virtual void draw()
//        Draws the hole area of this window.
// - virtual void drawScore(int score)
//        Draws the score with the given score.
// - virtual void drawLines(int lines)
//        Draws the line number with the given lines.
// - virtual void drawLevel(int level)
//        Draws the level with the given level.
// --------------------------------------------------------------------------

#ifndef _WINDOW_
#define _WINDOW_

#include "canvas.h"
#include "block.h"

class Window
{
	private:
		Block * block;
	protected:
		Canvas * canvas;
		int width;
		int height;
	public:
		Window(Canvas * canvas, int width, int height);
		virtual ~Window();
		void setBlock(Block * block, int row, int col);
		virtual void draw();
		virtual void drawScore(int score);
		virtual void drawLines(int lines);
		virtual void drawLevel(int level);
};

#endif
