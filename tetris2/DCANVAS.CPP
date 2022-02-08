#include "dcanvas.h"

DCanvas::DCanvas(DGraphicDevice * graphicDriver, int x, int y, int width, int height) : Canvas()
{
	this->graphicDriver = graphicDriver;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
}

DCanvas::~DCanvas()
{

}

void DCanvas::clearRect(int x, int y, int width, int height)
{
	setfillstyle(1, Color::black);
	bar(this->x + x, this->y + y, this->x + x + width, this->y + y + height);
}

void DCanvas::drawRect(int x, int y, int width, int height)
{
	setcolor(color);
	rectangle(this->x + x, this->y + y, this->x + x + width, this->y + y + height);
}

void DCanvas::fillRect(int x, int y, int width, int height)
{
	setfillstyle(1, color);
	bar(this->x + x, this->y + y, this->x + x + width, this->y + y + height);
}

void DCanvas::drawString(const String & string, int x, int y)
{
	String fontName = font.getName();
	int fontValue = 0;

	if (fontName.equals("DEFAULT"))
		fontValue = 0;
	else if (fontName.equals("TRIPLEX"))
		fontValue = 1;
	else if (fontName.equals("SMALL"))
		fontValue = 2;
	else if (fontName.equals("SANS_SERIF"))
		fontValue = 3;
	else if (fontName.equals("GOTHIC"))
		fontValue = 4;

	settextstyle(fontValue, 0, font.getSize());
	setcolor(color);
	outtextxy(this->x + x, this->y + y, string.getChars());
}
