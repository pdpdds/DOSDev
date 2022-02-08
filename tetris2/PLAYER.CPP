#include "player.h"
#include "color.h"

Player::Player(Canvas * canvas, int rows, int cols) : DThread()
{
	this->canvas = canvas;
	this->rows = rows;
	this->cols = cols;
	score = 0;
	level = 1;
	lines = 0;
	block = 0;
	runInterval = 0;
	keyInterval = 0;
	key = 0;

	segments = new int * [rows];

	for (int row = 0; row < rows; row++)
	{
		*(segments + row) = new int[cols];

		for (int col = 0; col < cols; col++)
			*(*(segments + row) + col) = Color::black;
	}

	gameOver = true;
	gamePaused = false;
	blockAlive = false;
}

Player::~Player()
{
	for (int row = 0; row < rows; row++)
		delete [] *(segments + row);

	delete [] segments;

	if (block != 0)
		delete block;
}

void Player::drawRowAt(int row)
{
	for (int col = 0; col < cols; col++)
	{
		canvas->setColor(*(*(segments + row) + col));
		canvas->fillRect(col * Segment::size + 1, row * Segment::size + 1, Segment::size - 2, Segment::size - 2);
	}
}

void Player::clearRowAt(int row)
{
	for (int col = 0; col < cols; col++)
		*(*(segments + row) + col) = Color::black;

	canvas->setColor(Color::black);
	canvas->fillRect(1, row * Segment::size + 1, cols * Segment::size - 2, Segment::size - 2);
}

void Player::dropDownRowAt(int row)
{
	if (row >= 0 && row < rows - 1)
	{
		for (int col = 0; col < cols; col++)
			*(*(segments + row + 1) + col) = *(*(segments + row) + col);

		clearRowAt(row);
		drawRowAt(row + 1);
	}
}

void Player::clearRowsFilled()
{
	int row = rows - 1;
	int rowsCleared = 0;

	for (int times = 0; times < rows; times++)
	{
		if (isRowFilledAt(row))
		{
			rowsCleared++;

			clearRowAt(row);

			for (int rowDrop = row - 1; rowDrop >= 0; rowDrop--)
				dropDownRowAt(rowDrop);
		}
		else
			row--;
	}

	lines += rowsCleared;
	score += rowsCleared * 100;
}

boolean Player::isRowFilledAt(int row)
{
	boolean result = true;

	for (int col = 0; col < cols; col++)
	{
		if (*(*(segments + row) + col) == Color::black)
		{
			result = false;
			break;
		}
	}

	return result;
}

boolean Player::isBlockFit(int rowOffSet, int colOffSet)
{
	Segment segment;
	int row = 0;
	int col = 0;

	if (block == 0)
		return false;

	for (int index = 0; index < block->getSize(); index++)
	{
		segment = block->getSegmentAt(index);
		row = segment.getRow() + rowOffSet;
		col = segment.getCol() + colOffSet;

		if (segment.getColor() == Color::black)
			continue;

		if (row < 0 || col < 0 || row >= rows || col >= cols)
			return false;

		if (*(*(segments + row) + col) != Color::black)
			return false;
	}

	return true;
}

void Player::drawBlockRuler()
{
	Segment segment;

	canvas->setColor(Color::black);
	canvas->fillRect(1, rows * Segment::size + 5, cols * Segment::size - 2, Segment::size - 2);

	for (int index = 0; index < block->getSize(); index++)
	{
		segment = block->getSegmentAt(index);

		if (segment.getColor() == Color::black)
			continue;

		canvas->setColor(segment.getColor());
		canvas->fillRect(segment.getCol() * Segment::size + 1, rows * Segment::size + 5, Segment::size - 2, Segment::size - 2);
	}
}

void Player::setGameOver()
{
	int x = 20;
	int y = 100;
	int width = 170;
	int height = 22;

	gameOver = true;

	canvas->setColor(Color::black);
	canvas->fillRect(x, y, width, height);
	canvas->setColor(Color::orange);
	canvas->drawRect(x, y, width, height);

	canvas->setColor(Color::red);
	canvas->setFont(Font("DEFAULT", 0, 2));
	canvas->drawString("Game Over", x + 14, y + 4);

	for (int row = 0; row < rows; row++)
		for (int col = 0; col < cols; col++)
			*(*(segments + row) + col) = Color::black;
}

void Player::draw()
{
	canvas->clearRect(0, 0, cols * Segment::size, rows * Segment::size);

	canvas->setColor(Color::white);
	canvas->drawRect(0, 0, cols * Segment::size, rows * Segment::size); // draw a fild's border
	canvas->drawRect(0, rows * Segment::size + 4, cols * Segment::size, Segment::size); // draw a box for unit block
}

void Player::setGamePaused(boolean gamePaused)
{
	this->gamePaused = gamePaused;
}

void Player::setBlock(Block * block, int row, int col)
{
	this->block = new Block(block);
	this->block->locate(row, col);

	if (isBlockFit(0, 0) == true)
	{
		this->block->draw(canvas);
		drawBlockRuler();

		gameOver = false;
		blockAlive = true;
	}
	else
	{
		gameOver = true;
		blockAlive = false;

		score = 0;
		level = 1;
		lines = 0;

		setGameOver();

		if (this->block != 0)
		{
			delete this->block;
			this->block = 0;
		}
	}
}

void Player::locateBlock()
{
	Segment segment;

	for (int index = 0; index < block->getSize(); index++)
	{
		segment = block->getSegmentAt(index);

		if (segment.getColor() == Color::black)
			continue;

		*(*(segments + segment.getRow()) + segment.getCol()) = segment.getColor();
	}

	clearRowsFilled();

	score += block->getScore();

	if (block != 0)
	{
		delete block;
		block = 0;
	}
}

void Player::start()
{
	gameOver = false;
	gamePaused = false;

	DThread::start();

//	((DThread *)this->start();	// fatal error
//	Don't cast the 'this' pointer like above.
//	'DThread::start()' is a virtual function.
//	So, if anyone casts the 'this' pointer to a point of the DThread,
//	it will be a permanent calling of it.
}

void Player::run()
{
	if (gameOver == true || gamePaused == true || blockAlive == false)
		return;

	runInterval++;

	if (runInterval % 8 == 0 && key != 80 && isBlockFit(1, 0)) // if the key is not 'down arrow'
	{
		block->clear(canvas);
		block->moveDown();
		block->draw(canvas);
	}

	if (key == 75) // 'left arrow' key
	{
		if (isBlockFit(0, -1))
		{
			block->clear(canvas);
			block->moveLeft();
			block->draw(canvas);

			drawBlockRuler();
		}
	}
	else if (key == 77) // 'right arrow' key
	{
		if (isBlockFit(0, 1))
		{
			block->clear(canvas);
			block->moveRight();
			block->draw(canvas);

			drawBlockRuler();
		}
	}
	else if (key == 32) // 'space' key
	{
		if (isBlockFit(1, 0))
		{
			block->clear(canvas);

			while (isBlockFit(1, 0))
				block->moveDown();

			block->draw(canvas);
		}
	}
	else if (key == 80) // 'down arrow' key
	{
		if (isBlockFit(1, 0))
		{
			block->clear(canvas);
			block->moveDown();
			block->draw(canvas);
		}
	}
	else if (key == 72) // 'up arrow' key
	{
		block->rotateRight();

		if (isBlockFit(0, 0))
		{
			block->rotateLeft();
			block->clear(canvas);

			block->rotateRight();
			block->draw(canvas);

			drawBlockRuler();
		}
		else
			block->rotateLeft();
	}

	key = 0;

	if (isBlockFit(1, 0) == false)
	{
		if (keyInterval % 2 == 0) // waits the key two times more
		{
			blockAlive = false;
			runInterval = 0;
		}

		keyInterval++;
	}
}
