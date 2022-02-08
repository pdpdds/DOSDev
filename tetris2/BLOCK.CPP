#include "block.h"

int Block::shapes = 23;

Block::Block(int type)
{
	switch (type)
	{
		case 0:
			size = 1;
			score = 5;
			canRotate = false;
			segments = new Segment[size];
			(segments + 0)->set(0, 0, Color::yellow); // #
			break;
		case 1:
			size = 3;
			score = 10;
			canRotate = true;
			segments = new Segment[size];
			(segments + 0)->set(0, 0, Color::blue); //   #
			(segments + 1)->set(0,-1, Color::blue); // # #
			(segments + 2)->set(-1,0, Color::blue);
			break;
		case 2:
			size = 4;
			score = 10;
			canRotate = false;
			segments = new Segment[size];
			(segments + 0)->set(0, 0, Color::green); // # #
			(segments + 1)->set(0, 1, Color::green); // # #
			(segments + 2)->set(1, 0, Color::green);
			(segments + 3)->set(1, 1, Color::green);
			break;
		case 3:
			size = 4;
			score = 15;
			canRotate = true;
			segments = new Segment[size];
			(segments + 0)->set(0, 0, Color::cyan); // # # # #
			(segments + 1)->set(0,-1, Color::cyan);
			(segments + 2)->set(0, 1, Color::cyan);
			(segments + 3)->set(0, 2, Color::cyan);
			break;
		case 4:
			size = 4;
			score = 15;
			canRotate = true;
			segments = new Segment[size];
			(segments + 0)->set(0, 0, Color::red); // # # #
			(segments + 1)->set(0,-1, Color::red); //     #
			(segments + 2)->set(0, 1, Color::red);
			(segments + 3)->set(1, 1, Color::red);
			break;
		case 5:
			size = 4;
			score = 15;
			canRotate = true;
			segments = new Segment[size];
			(segments + 0)->set(0, 0, Color::magenta); //     #
			(segments + 1)->set(0,-1, Color::magenta); // # # #
			(segments + 2)->set(0, 1, Color::magenta);
			(segments + 3)->set(-1,1, Color::magenta);
			break;
		case 6:
			size = 4;
			score = 20;
			canRotate = true;
			segments = new Segment[size];
			(segments + 0)->set(0, 0, Color::brown); //   # #
			(segments + 1)->set(-1,0, Color::brown); // # #
			(segments + 2)->set(-1,1, Color::brown);
			(segments + 3)->set(0,-1, Color::brown);
			break;
		case 7:
			size = 4;
			score = 20;
			canRotate = true;
			segments = new Segment[size];
			(segments + 0)->set(0, 0, Color::lightGray); // # #
			(segments + 1)->set(-1,0, Color::lightGray); //   # #
			(segments + 2)->set(-1,-1,Color::lightGray);
			(segments + 3)->set(0, 1, Color::lightGray);
			break;
		case 8:
			size = 4;
			score = 20;
			canRotate = true;
			segments = new Segment[size];
			(segments + 0)->set(0, 0, Color::yellow); //   #
			(segments + 1)->set(0,-1, Color::yellow); // # # #
			(segments + 2)->set(0, 1, Color::yellow);
			(segments + 3)->set(-1,0, Color::yellow);
			break;
		case 9:
			size = 5;
			score = 30;
			canRotate = false;
			segments = new Segment[size];
			(segments + 0)->set(0, 0, Color::lightBlue); //   #
			(segments + 1)->set(0,-1, Color::lightBlue); // # # #
			(segments + 2)->set(0, 1, Color::lightBlue); //   #
			(segments + 3)->set(-1,0, Color::lightBlue);
			(segments + 4)->set(1, 0, Color::lightBlue);
			break;
		case 10:
			size = 5;
			score = 40;
			canRotate = true;
			segments = new Segment[size];
			(segments + 0)->set(0, 0, Color::lightGreen); //     #
			(segments + 1)->set(0,-1, Color::lightGreen); // # # #
			(segments + 2)->set(0, 1, Color::lightGreen); // #
			(segments + 3)->set(1,-1, Color::lightGreen);
			(segments + 4)->set(-1,1, Color::lightGreen);
			break;
		case 11:
			size = 5;
			score = 40;
			canRotate = true;
			segments = new Segment[size];
			(segments + 0)->set(0, 0, Color::lightCyan); // #
			(segments + 1)->set(0,-1, Color::lightCyan); // # # #
			(segments + 2)->set(0, 1, Color::lightCyan); //     #
			(segments + 3)->set(-1,-1,Color::lightCyan);
			(segments + 4)->set(1, 1, Color::lightCyan);
			break;
		case 12:
			size = 5;
			score = 35;
			canRotate = true;
			segments = new Segment[size];
			(segments + 0)->set(0, 0, Color::black); // #   #
			(segments + 1)->set(0,-1, Color::pink);  // # #
			(segments + 2)->set(0, 1, Color::pink);
			(segments + 3)->set(1,-1, Color::pink);
			(segments + 4)->set(1, 0, Color::pink);
			break;
		case 13:
			size = 5;
			score = 35;
			canRotate = true;
			segments = new Segment[size];
			(segments + 0)->set(0, 0, Color::black); // #   #
			(segments + 1)->set(0,-1, Color::red);   //   # #
			(segments + 2)->set(0, 1, Color::red);
			(segments + 3)->set(1, 1, Color::red);
			(segments + 4)->set(1, 0, Color::red);
			break;
		case 14:
			size = 4;
			score = 30;
			canRotate = true;
			segments = new Segment[size];
			(segments + 0)->set(0, 0, Color::black); // #   #
			(segments + 1)->set(0,-1, Color::white); //   #
			(segments + 2)->set(0, 1, Color::white);
			(segments + 3)->set(1, 0, Color::white);
			break;
		case 15:
			size = 3;
			score = 35;
			canRotate = true;
			segments = new Segment[size];
			(segments + 0)->set(0,  0, Color::blue); // #
			(segments + 1)->set(-1,-1, Color::blue); //   #
			(segments + 2)->set(1,  1, Color::blue); //     #
			break;
		case 16:
			size = 3;
			score = 10;
			canRotate = true;
			segments = new Segment[size];
			(segments + 0)->set(0, 0, Color::black); //   #
			(segments + 1)->set(0,-1, Color::green); // #
			(segments + 2)->set(-1,0, Color::green);
			break;
		case 17:
			size = 3;
			score = 10;
			canRotate = true;
			segments = new Segment[size];
			(segments + 0)->set(0, 0, Color::magenta); // #
			(segments + 1)->set(-1,0, Color::magenta); // #
			(segments + 2)->set(1, 0, Color::magenta); // #
			break;
		case 18:
			size = 4;
			score = 35;
			canRotate = false;
			segments = new Segment[size];
			(segments + 0)->set(0,-1, Color::brown); //   #
			(segments + 1)->set(0, 1, Color::brown); // #   #
			(segments + 2)->set(-1,0, Color::brown); //   #
			(segments + 3)->set(1, 0, Color::brown);
			break;
		case 19:
			size = 3;
			score = 25;
			canRotate = true;
			segments = new Segment[size];
			(segments + 0)->set(0, 0, Color::black);  //     #
			(segments + 1)->set(-1,1, Color::yellow); //
			(segments + 2)->set(1,-1, Color::yellow); // #
			break;
		case 20:
			size = 5;
			score = 45;
			canRotate = false;
			segments = new Segment[size];
			(segments + 0)->set(0, 0, Color::lightBlue); // #   #
			(segments + 1)->set(-1,-1,Color::lightBlue); //   #
			(segments + 2)->set(1, 1, Color::lightBlue); // #   #
			(segments + 3)->set(-1,1, Color::lightBlue);
			(segments + 4)->set(1,-1, Color::lightBlue);
			break;
/*		case 21:
			size = 5;
			score = 50;
			canRotate = true;
			segments = new Segment[size];
			(segments + 0)->set(0, 0, Color::black);       //      #
			(segments + 1)->set(0,-1, Color::lightGreen);   // #
			(segments + 2)->set(1,-1, Color::lightGreen);   // # #
			(segments + 3)->set(1, 0, Color::lightGreen);
			(segments + 4)->set(-1,1, Color::lightGreen);
			break;
		case 22:
			size = 5;
			score = 50;
			canRotate = true;
			segments = new Segment[size];
			(segments + 0)->set(0, 0, Color::black);       // #
			(segments + 1)->set(0, 1, Color::lightCyan);   //     #
			(segments + 2)->set(1, 1, Color::lightCyan);   //   # #
			(segments + 3)->set(1, 0, Color::lightCyan);
			(segments + 4)->set(-1,-1,Color::lightCyan);
			break;*/
		default:
			size = 1;
			score = 5;
			canRotate = false;
			segments = new Segment[size];
			(segments + 0)->set(0, 0, Color::red); //   #
			break;
	}

	adjust();
}

Block::Block(Block * block)
{
	size = block->size;
	score = block->score;
	canRotate = block->canRotate;
	segments = new Segment[size];

	for (int index = 0; index < size; index++)
		*(segments + index) = *(block->segments + index);
}

Block::~Block()
{
	if (segments != 0)
		delete [] segments;
}

void Block::adjust()
{
	int row = 0;
	int col = 0;
	int rowMinimum = 0;
	int colMinimum = 0;

	for (Segment * next = segments; next < segments + size; next++)
	{
		row = next->getRow();
		col = next->getCol();

		if (row < rowMinimum)
			rowMinimum = row;
		if (col < colMinimum)
			colMinimum = col;
	}

	locate(-rowMinimum, -colMinimum);
}

void Block::locate(int rowOffSet, int colOffSet)
{
	for (Segment * next = segments; next < segments + size; next++)
		next->locate(next->getRow() + rowOffSet, next->getCol() + colOffSet);
}

void Block::moveLeft()
{
	for (Segment * next = segments; next < segments + size; next++)
		next->moveLeft();
}

void Block::moveRight()
{
	for (Segment * next = segments; next < segments + size; next++)
		next->moveRight();
}

void Block::moveDown()
{
	for (Segment * next = segments; next < segments + size; next++)
		next->moveDown();
}

void Block::moveUp()
{
	for (Segment * next = segments; next < segments + size; next++)
		next->moveUp();
}

void Block::rotateLeft()
{
	if (canRotate)
	{
		int baseRow = segments->getRow();
		int baseCol = segments->getCol();
		int rowOffSet = 0;
		int colOffSet = 0;

		for (Segment * next = segments; next < segments + size; next++)
		{
			rowOffSet = next->getRow() - baseRow;
			colOffSet = next->getCol() - baseCol;
			next->locate(baseRow - colOffSet, baseCol + rowOffSet); // rowOffSet = -colOffSet, colOffSet = rowOffSet
		}
	}
}

void Block::rotateRight()
{
	if (canRotate)
	{
		int baseRow = segments->getRow();
		int baseCol = segments->getCol();
		int rowOffSet = 0;
		int colOffSet = 0;

		for (Segment * next = segments; next < segments + size; next++)
		{
			rowOffSet = next->getRow() - baseRow;
			colOffSet = next->getCol() - baseCol;
			next->locate(baseRow + colOffSet, baseCol - rowOffSet); // rowOffSet = colOffSet, colOffSet = -rowOffSet
		}
	}
}

void Block::draw(Canvas * canvas)
{
	for (Segment * next = segments; next < segments + size; next++)
		next->draw(canvas);
}

void Block::clear(Canvas * canvas)
{
	for (Segment * next = segments; next < segments + size; next++)
		next->clear(canvas);
}
