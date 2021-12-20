#include "color.h"

int Color::black = 0;
int Color::blue = 1;
int Color::green = 2;
int Color::cyan = 3;
int Color::red = 4;
int Color::magenta = 5;
int Color::brown = 6;
int Color::lightGray = 7;
int Color::darkGray = 8;
int Color::gray = 8;
int Color::lightBlue = 9;
int Color::lightGreen = 10;
int Color::lightCyan = 11;
int Color::lightRed = 12;
int Color::orange = 12;
int Color::lightMagenta = 13;
int Color::pink = 13;
int Color::yellow = 14;
int Color::white = 15;

Color::Color(int r, int g, int b, int alpha)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->alpha = alpha;
}
