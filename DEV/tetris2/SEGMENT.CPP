#include "segment.h"

int Segment::size = 16;

void Segment::draw(Canvas * canvas)
{
	if (color != Color::black)
	{
		canvas->setColor(color);
		canvas->fillRect(col * size + 1, row * size + 1, size - 2, size - 2);
	}
}

void Segment::clear(Canvas * canvas)
{
	if (color != Color::black)
	{
		canvas->setColor(Color::black);
		canvas->fillRect(col * size + 1, row * size + 1, size - 2, size - 2);
	}
}
