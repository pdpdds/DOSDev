// --------------------------------------------------------------------------
// Project: Tetris Game That Has A Flexible Framework For Further Extension
// Organization: None, Personal
// --------------------------------------------------------------------------
// Language    : C++
// Tool        : Borland C++ For DOS Version 3.1
// Memory Model: Compact Model
// Required Lib: Graphics Library For DOS EGA/VGA; 'EGAVGA.BGI'
// --------------------------------------------------------------------------
// Class   : Main Function
// Platform: DOS System
// Purpose : Implemented Tetris Game On DOS System
// License : GNU General Public License
// Release : Free Software
// Date    : 1999/12/28
// Author  : Yu Byoung-Hak; Sejong University Computer Engineering Department
// E-Mail  : xover0@hitel.net
// --------------------------------------------------------------------------
// Note 1  : This program was designed with a flexible framework.
//           So, maybe someone can do porting this to the other platform like
//           Java, Unix or Window System.
// Note 2  : This program is free software; you can redistribute it and/or
//           modify it under the terms of the GNU General Public License as
//           published by the Free Software Foundation.
// Note 3  : This program is distributed in the hope that it will be useful.
// --------------------------------------------------------------------------

#include <stdlib.h>
#include <conio.h>
#include <new.h>
#include "dgdevice.h"
#include "dcanvas.h"
#include "window.h"
#include "player.h"
#include "drandom.h"

void newHandler();

int main()
{
	set_new_handler(newHandler);

	DGraphicDevice * graphicDriver = new DGraphicDevice();
	Canvas * canvasWindow = new DCanvas(graphicDriver, 160, 15, 314, 448);
	Canvas * canvasPlayer = new DCanvas(graphicDriver, 260, 21, 210, 440);
	Window * window = new Window(canvasWindow, 314, 448);
	Player * player = new Player(canvasPlayer, 26, 13);
	Random * random = new DRandom();
	Block * blockNext = 0;
	boolean isQuited = false;
	int key = 0;

	window->draw();
	player->draw();

	while (isQuited == false)
	{
		if (kbhit())
		{
			key = getch();

			if (key == 0)
				key = getch();

			if (key == 27) // 'ESC' key
				isQuited = true;
			else if (key == 63 && player->isGameOver() == true) // 'F5' key
			{
				window->drawScore(0);
				window->drawLevel(1);
				window->drawLines(0);
				player->draw();

				window->setBlock(blockNext = new Block(random->nextInt() % Block::shapes), 4, 2);
				player->setBlock(&Block(random->nextInt() % Block::shapes), 0, 5);

				player->start();
			}
			else if (key == 66 && player->isGameOver() == false) // 'F8' key
				player->setGamePaused(boolean (!player->isGamePaused()));
			else if (player->isGameOver() == false && player->isGamePaused() == false && player->isBlockAlive() == true)
				player->key = key;
		}

		if (player->isGameOver() == false && player->isGamePaused() == false && player->isBlockAlive() == false)
		{
			player->locateBlock();

			window->drawScore(player->getScore());
			window->drawLevel(player->getLevel());
			window->drawLines(player->getLines());

			player->setBlock(blockNext, 0, 5);

			if (blockNext != 0)
			{
				delete blockNext;
				blockNext = 0;
			}

			if (player->isBlockAlive() == true)
			{
				blockNext = new Block(random->nextInt() % Block::shapes);
				window->setBlock(blockNext, 4, 2);
			}
		}
	}

	delete random;
	delete player;
	delete window;
	delete canvasPlayer;
	delete canvasWindow;
	delete graphicDriver;
	return 0;
}

void newHandler()
{
	exit(1);
}
