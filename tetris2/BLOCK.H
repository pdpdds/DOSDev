// --------------------------------------------------------------------------
// Project: Tetris Game That Has A Flexible Framework For Further Extension
// Organization: None, Personal
// --------------------------------------------------------------------------
// Class   : Block
// Platform: None
// Purpose : Implemented The Block Shape For This Tetris Game
// License : GNU General Public License
// Release : Free Software
// Date    : 1999/12/28
// Author  : Yu Byoung-Hak; Sejong University Computer Engineering Department
// E-Mail  : xover0@hitel.net
// --------------------------------------------------------------------------
// Constructor Summary:
// - Block(int type)
//        Makes one block by using the type number. The shape of a block
//        varies as the type number.
// - Block(Block * block)
//        The Copy constructor of this class.
// Method Summary:
// - void locate(int rowOffSet, int colOffSet)
//        Locates this block at the position given as offset to current point.
// - void moveLeft()
//        Moves this block to the left direction once.
// - void moveRight()
//        Moves this block to the right direction once.
// - void moveDown()
//        Moves this block to the down direction once.
// - void moveUp()
//        Moves this block to the up direction once.
// - void rotateLeft()
//        Rotates this block to the left direction once.
// - void rotateRight()
//        Rotates this block to the right direction once.
// - void draw(Canvas * canvas)
//        Draws this block on the canvas given at the current position.
// - void clear(Canvas * canvas)
//        Clears this block from the canvas given at the current position.
// - Segment getSegmentAt(int index)
//        Returns one segment at the index. A block consists of many segments.
// - int getSize()
//        Returns the number of the segments of this block.
// - int getScore()
//        Returns the score of this block.
// --------------------------------------------------------------------------

#ifndef _BLOCK_
#define _BLOCK_

#include "boolean.h"
#include "canvas.h"
#include "segment.h"

class Block
{
	private:
		Segment * segments;
		boolean canRotate;
		int size;
		int score;
		void adjust();
	public:
		static int shapes;
		Block(int type);
		Block(Block * block);
		~Block();
		void locate(int rowOffSet, int colOffSet);
		void moveLeft();
		void moveRight();
		void moveDown();
		void moveUp();
		void rotateLeft();
		void rotateRight();
		void draw(Canvas * canvas);
		void clear(Canvas * canvas);
		Segment getSegmentAt(int index);
		int getSize();
		int getScore();
};

inline Segment Block::getSegmentAt(int index)
{
	return *(segments + index);
}

inline int Block::getSize()
{
	return size;
}

inline int Block::getScore()
{
	return score;
}

#endif
