#include "window.h"
#include "integer.h"

Window::Window(Canvas * canvas, int width, int height)
{
	this->canvas = canvas;
	this->width = width;
	this->height = height;
	block = 0;
}

Window::~Window()
{
	if (block != 0)
		delete block;
}

void Window::setBlock(Block * block, int row, int col)
{
	if (this->block != 0)
	{
		this->block->clear(canvas);

		delete this->block;
		this->block = 0;
	}

	this->block = new Block(block);
	this->block->locate(row, col);
	this->block->draw(canvas);
}

void Window::draw()
{
	canvas->clearRect(0, 0, width, height);

	canvas->setColor(Color::red);
	canvas->drawRect(0, 0, width, height); // draw a window border
	canvas->drawRect(6, 6, 88, 16); // draw a box for logo

	canvas->setColor(Color::yellow); // color for tetris logo
	canvas->drawString("TETRIS 1.0", 10, 11); // draw the tetris logo
	canvas->drawString("Next Object", 8, 50); // draw a string for next block

	canvas->setColor(Color::white);
	canvas->drawString("Score:", 8, 200);
	canvas->drawString("Level:", 8, 220);
	canvas->drawString("Lines:", 8, 240);

	canvas->setColor(Color::gray);
	canvas->drawString("Start: F5", 8, 340);
	canvas->drawString("Pause: F8", 8, 355);
	canvas->drawString("Quit : ESC",8, 370);

	canvas->setColor(Color::pink);
	canvas->drawString("Designed by", 7, 424);
	canvas->drawString("YuByoungHak", 7, 434);
}

void Window::drawScore(int score)
{
	canvas->setColor(Color::black);
	canvas->fillRect(58, 200, 40, 10);

	canvas->setColor(Color::white);
	canvas->drawString(Integer::toString(score), 58, 200);
}

void Window::drawLevel(int level)
{
	canvas->setColor(Color::black);
	canvas->fillRect(58, 220, 40, 10);

	canvas->setColor(Color::white);
	canvas->drawString(Integer::toString(level), 58, 220);
}

void Window::drawLines(int lines)
{
	canvas->setColor(Color::black);
	canvas->fillRect(58, 240, 40, 10);

	canvas->setColor(Color::white);
	canvas->drawString(Integer::toString(lines), 58, 240);
}
