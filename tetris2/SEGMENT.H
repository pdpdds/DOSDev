// --------------------------------------------------------------------------
// Project: Tetris Game That Has A Flexible Framework For Further Extension
// Organization: None, Personal
// --------------------------------------------------------------------------
// Class   : Segment
// Platform: None
// Purpose : Implemented The Segment Class For This Tetris Game
// License : GNU General Public License
// Release : Free Software
// Date    : 1999/12/28
// Author  : Yu Byoung-Hak; Sejong University Computer Engineering Department
// E-Mail  : xover0@hitel.net
// --------------------------------------------------------------------------
// Constructor Summary:
// - Segment()
//        Constructs an empty segment.
// - Segment(int row, int col, Color color)
//        Constructs a new segment using the row and column as position,
//        the color for the segment's surface color.
// - Segment(Segment & segment)
//        Copy Constructor of this class.
// Method Summary:
// - void set(int row, int col, Color color)
//        Sets the information for this segment.
// - int getRow()
//        Returns the current row position of this segment.
// - int getCol()
//        Returns the current column position of this segment.
// - Color getColor()
//        Returns the color of this segment.
// - void locate(int row, int col)
//        Sets the row and column with the given parameters.
// - void moveLeft()
//        Moves this segment to the left direction once. (col--)
// - void moveRight()
//        Moves this segment to the right direction once. (col++)
// - void moveDown()
//        Moves this segment to the down direction once. (row++)
// - void moveUp()
//        Moves this segment to the up direction once. (row--)
// - void draw(Canvas * canvas)
//        Draws this segment on the canvas given at the current position.
// - void clear(Canvas * canvas)
//        Clears this segment from the canvas given at the current position.
// --------------------------------------------------------------------------

#ifndef _SEGMENT_
#define _SEGMENT_

#include "canvas.h"

class Segment
{
	private:
		int row;
		int col;
		Color color;
	public:
		static int size;
		Segment();
		Segment(int row, int col, Color color);
		Segment(Segment & segment);
		void set(int row, int col, Color color);
		int getRow();
		int getCol();
		Color getColor();
		void locate(int row, int col);
		void moveLeft();
		void moveRight();
		void moveDown();
		void moveUp();
		void draw(Canvas * canvas);
		void clear(Canvas * canvas);
		Segment & operator=(Segment & segment);
};

inline Segment::Segment() : color(Color::black)
{
	row = 0;
	col = 0;
}

inline Segment::Segment(int row, int col, Color color) : color(color)
{
	this->row = row;
	this->col = col;
}

inline Segment::Segment(Segment & segment) : color(segment.color)
{
	this->row = segment.row;
	this->col = segment.col;
}

inline void Segment::set(int row, int col, Color color)
{
	this->row = row;
	this->col = col;
	this->color = color;
}

inline int Segment::getRow()
{
	return row;
}

inline int Segment::getCol()
{
	return col;
}

inline Color Segment::getColor()
{
	return color;
}

inline void Segment::locate(int row, int col)
{
	this->row = row;
	this->col = col;
}

inline void Segment::moveLeft()
{
	col--;
}

inline void Segment::moveRight()
{
	col++;
}

inline void Segment::moveDown()
{
	row++;
}

inline void Segment::moveUp()
{
	row--;
}

inline Segment & Segment::operator=(Segment & segment)
{
	row = segment.row;
	col = segment.col;
	color = segment.color;

	return *this;
}

#endif
