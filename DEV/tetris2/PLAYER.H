// --------------------------------------------------------------------------
// Project: Tetris Game That Has A Flexible Framework For Further Extension
// Organization: None, Personal
// --------------------------------------------------------------------------
// Class   : Player Inherited From DThread
// Platform: None
// Purpose : One Player That Can Play The Tetris Game
// License : GNU General Public License
// Release : Free Software
// Date    : 1999/12/28
// Author  : Yu Byoung-Hak; Sejong University Computer Engineering Department
// E-Mail  : xover0@hitel.net
// --------------------------------------------------------------------------
// Field Summary:
// - protected Canvas * canvas
//        A canvas object for drawing work.
// - protected int * * segments
//        This is a array used for the playing field. In this, the color
//        information of segments for this player's playing field is stored.
// - protected int rows
//        The number of rows for this player's playing field.
// - protected int cols
//        The number of columns for this player's playing field.
// - protected int runInterval
//        This is used in the run method to delay the block dropping into the
//        playing field.
// - protected int keyInterval
//        This is used in the run method to delay the block locating into the
//        playing field. This work is needed because a player want to move
//        the block before locating into the playing field.
// - public int key
//        A key of the keystroke that will be processed in the run method.
//        Keys should be pushed from the outside of this class.
// Constructor Summary:
// - Player(Canvas * canvas, int rows, int cols)
//        Constructs a new player can play the game making the player's field
//        area by using the given rows and columns.
// Method Summary:
// - protected virtual void drawBlockRuler()
//        Draws the block ruler at the bottom of the playing field to
//        indicate where the block is in a column.
// - protected virtual void setGameOver()
//        Sets the game is over and draws a window for game over.
// - boolean isGameOver()
//        Tests whether the game is over.
// - boolean isGamePaused()
//        Tests whether the game is paused.
// - boolean isBlockAlive()
//        Tests whether the block on this player's field is alive.
// - int getScore()
//        Returns the total score of this player.
// - int getLines()
//        Returns the line number deleted of this player.
// - int getLevel()
//        Returns the current level of this player. (Not Implemented)
// - virtual void draw()
//        Draws the hole area of this player after clearing the canvas area.
// - virtual void setGamePaused(boolean gamePaused)
//        Sets the game state with the parameter.
// - void setBlock(Block * block, int row, int col)
//        Sets the block on this player's field using the row and column.
// - void locateBlock()
//        Locates the block into this player's field using the position of
//        the current block.
// - virtual void start()
//        Starts this thread to begin execution. The run method of this
//        thread is called without hesitation.
//        Overrode from the class DThread.
// - virtual void run()
//        Thread's execution body. Overrode from the class DThread.
// Methods inherited from class DThread:
// - boolean isAlive()
// - static void sleep(long millis)
// --------------------------------------------------------------------------

#ifndef _PLAYER_
#define _PLAYER_

#include "dthread.h"
#include "canvas.h"
#include "block.h"

class Player : public DThread
{
	private:
		Block * block;
		boolean gameOver;
		boolean gamePaused;
		boolean blockAlive;
		int score;
		int level;
		int lines;
		void drawRowAt(int row);
		void clearRowAt(int row);
		void dropDownRowAt(int row);
		void clearRowsFilled();
		boolean isRowFilledAt(int row);
		boolean isBlockFit(int rowOffSet, int colOffSet);
	protected:
		Canvas * canvas;
		int * * segments;
		int rows;
		int cols;
		int runInterval;
		int keyInterval;
		virtual void drawBlockRuler();
		virtual void setGameOver();
	public:
		int key;
		Player(Canvas * canvas, int rows, int cols);
		virtual ~Player();
		boolean isGameOver();
		boolean isGamePaused();
		boolean isBlockAlive();
		int getScore();
		int getLines();
		int getLevel();
		virtual void draw();
		virtual void setGamePaused(boolean gamePaused);
		void setBlock(Block * block, int row, int col);
		void locateBlock();
		virtual void start();
		virtual void run();
};

inline boolean Player::isGameOver()
{
	return gameOver;
}

inline boolean Player::isGamePaused()
{
	return gamePaused;
}

inline boolean Player::isBlockAlive()
{
	return blockAlive;
}

inline int Player::getScore()
{
	return score;
}

inline int Player::getLevel()
{
	return level;
}

inline int Player::getLines()
{
	return lines;
}

#endif
