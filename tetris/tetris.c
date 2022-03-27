/* ************************************************************* */
/* TETRIS Game Source                                            */
/*                                                               */
/* ¿øÀúÀÚ : (ÄÄ¸Íµµ ÇÒ ¼ö ÀÖ´Â) PC°ÔÀÓ ¸¸µé±â, ³ª¿ø±Õ, 1996. 04. */
/*                                                               */
/* Å¸ÀÌÇÎ : ³×ÀÌ¹ö Ä«Æä µµ½º¹Ú¹°°ü, ¾Ë¶óµò, 2022. 02.            */
/*                                                               */
/* ************************************************************* */

#ifndef _WIN32
#include <alloc.h>
#endif
#include <conio.h>
#include <dos.h>
#ifndef _WIN32
#include <bios.h>
#endif
#include <stdio.h>
#include <graphics.h>
#include <time.h>
#include <stdlib.h>
#include <stdarg.h>

#include "tetris.h"

#ifdef _WIN32
#include <stdlib.h>
void randomize(void) {
	srand((unsigned)time(NULL));
	// Ã¹ºÎºĞÀÇ ¼ıÀÚ´Â °ÅÀÇ ·£´ıÇÏÁö ¾Ê±â¿¡, ¾ÕºÎºĞ¿¡¼­ ·£´ıÇÑ °³¼ö·Î »Ì¾Æ¼­ ¹ö¸²
	for (int i = 0; i < (rand() % RAND_MAX); i++) (rand() % RAND_MAX);
}

int bioskey(int temp)
{
	if (temp == 1)
		return 1;

	int c = getch();

	if (c == 27)
		return 283;

	if (c == 32)
		return 14624;

	if (c == 97) //A KEY
		return 19200;

	if (c == 100) //D KEY
		return 19712;

	if (c == 119) //W KEY
		return 18432;


	return 0;
}

#define farmalloc malloc
#define farfree free
#endif

int main()
{
	int i = 0;

	OpenScreen();
	InitGR();

	for (i = 0; i < 500; i++)
	{
		cleardevice();
		newscr = 0;
		display_score();
		base_block();
		highscr();
		display_score();
		if (Pause())
			goto EXIT_1;
	}
EXIT_1:
	settextstyle(0, HORIZ_DIR, 3);
	settextjustify(CENTER_TEXT, CENTER_TEXT);
	setcolor(BLUE);
	outtextxy(getmaxx() / 2 - 50, 400, "GOOD - BYE !");
	//gprintf_XY(150, 400, YELLOW, LIGHTBLUE, " GOOD - BYE !");
	delay(2500);
	closegraph();
	CloseScreen();
	return 0;
}

/* ************************* */
/*  º®µ¹À» ¶³±¸´Â ÇÔ¼ö       */
/* ************************  */
void base_block(void)
{
	int nx = 0, ny = 0, bios1; /* º¯¼ö nx¸¦ Á¤ÀÇ, key ÀÔ·ÂÀ» ÀúÀåÇÒ º¯¼ö¸¦ ¼±¾ğ */
	int i;
	int m = 0;  /* È¸ÀüÇÑ ¸ğ¾çÀ» ³ªÅ¸³»´Â Ã·ÀÚ m¼±¾ğ */
	int a = 0;  /* ½ºÆäÀÌ½º ¹Ù°¡ ´­·¯ÁüÀ» ¾Ë¸®´Â Ã·ÀÚ a ¼±¾ğ */
	int num = 0;
	int num1 = 0;
	int value = 0;
	int* test = &value;

	back_screen(); /* Å×Æ®¸®½º ¹è°æ È­¸éÀ» ±×·Á ÁØ´Ù */
	clear_board(); /* board¸¦ ÃÊ±âÈ­ÇÑ´Ù */
	sound3(); /* °ÔÀÓ ½ÃÀÛÇÒ ¶§ ¼Ò¸® ¹ß»ı */
	randomize();
	num = random(7); /* ³­¼ö¸¦ ¹ß»ı½ÃÅ´ */

	while (board[1][6] != 1) { /* boardÀÇ ¸Ç À­ÁÙ °¡¿îµ¥°¡ Ã¤¿öÁú ¶§°¡Áö º®µ¹À» ¶³¾î¶ß¸°´Ù */
		a = 0; /* a¸¦ º®µ¹À» ¶³¾î¶ß¸± ¶§¸¶´Ù ÃÊ±âÈ­ÇÑ´Ù. */
		m = 0;
		nx = 5;
		ny = 0;

		num1 = random(7); /* while¹® ¾È¿¡¼­ ³­¼ö¸¦ ¹ß»ı½ÃÅ´ */
		/* Ã³À½ ½ÃÀÛÇÒ¶§¸¸ µÎ ¹øÀÇ ³­¼ö ¹ß»ı */

		gprintf_XY(40, 77, YELLOW, LIGHTBLUE, "<  ”a ·q   ¥b •©  >");

		draw(num1, 0, -5, 3); /* ´ÙÀ½ º®µ¶À» ¿¹°í À§Ä¡¿¡ ±×·Á ÁØ´Ù */

		do {

			draw(num, m, nx, ny); /* º®µ¹À» ¶³¾î¶ß¸°´Ù */
			for (i = 0; i < 50; i++) /* delay(500)À» 50°³·Î ³ª´² ¹İº¹ÇØ¼­ ÇÑ´Ù */
			{
				if (ny >= 20) break;
				delay(10); /* delay(10)À» ÇÒ ¶§¸¶´Ù key°¡ ÀÔ·ÂµÇ¾ú´ÂÁö °Ë»ç */

				if (kbhit()) /* keyboard°¡ ´­·ÁÁ³´ÂÁö¸¦ °Ë»ç */
				{
					while (!bioskey(1));

					bios1 = bioskey(0); /* key¸¦ ÀÔ·Â¹Ş´Â´Ù */

					switch (bios1) { /* ¾î´À key°¡ ´­·ÁÁ³´ÂÁö¸¦ °Ë»ç */
						/* ¿ŞÂÊ È­»ìÇ¥°¡ ´­·ÁÁö¸é xÁÂÇ¥¸¦ ¹Ì¸® ÇÏ³ª °¨¼ÒÅ°´Ï ´ÙÀ½¿¡ boardÆÇ°ú
						 * 1·Î °ãÃÄÁö´ÂÁö °Ë»çÇÏ¿© ¾Æ¹«°Íµµ ÇÏÁö ¾Ê°í switch ¹®À» Å»ÃâÇÑ´Ù.
						 * °ãÃÄÁöÁö ¾ÊÀ¸¸é ¾Õ¿¡¼­Ã³·³ ÇÑ´Ù. */
					case 19200: /* left arrowkey°¡ ´­·ÁÁö¸é ÇöÀç À§Ä¡ÀÇ º®µ¹À» Áö¿ì°í */
						comp_board(num, m, nx - 1, ny, test);
						if (*test == 1)
							*test = 0;
						else {
							erase(num, m, nx, ny);
							--nx; /* xÁÂÇ¥¸¦ ÇÏ³ª °¨¼Ò½ÃÅ² ÈÄ */
							draw(num, m, nx, ny); /* ±×°÷¿¡ º®µ¹À» ±×¸°´Ù */
						}

						break;
						/* ¿À¸¥ÂÊ È­»ìÇ¥°¡ ÀÔ·ÂµÇ¸é º®µ¹À» ¿À¸¥ÂÊÀ¸·Î ¹Ì¸® ¿Å±ä ÈÄ¿¡ board ÆÇ°ú
						 * ºñ±³ÇÏ¿© 1ÀÌ °ãÃÄÁö¸é ¾Æ¹«°Íµµ ÇÏÁö ¾Ê°í switch ¹® Å»Ãâ */
					case 19712: /* right arrow key°¡ ´­·ÁÁö¸é ÇöÀç À§Ä¡ÀÇ º®µ¹À» Áö¿ì°í */
						comp_board(num, m, nx + 1, ny, test);
						if (*test == 1)
							*test = 0;
						else {
							erase(num, m, nx, ny);
							++nx; /* xÁÂÇ¥¸¦ ÇÏ³ª Áõ°¡ ½ÃÅ² ÈÄ */
							draw(num, m, nx, ny); /* ±×°÷¿¡ º®µ¹À» ±×¸°´Ù */
						}
						break;
						/* È¸ÀüÅ°°¡ ´­·¯Áö¸é ¸ğ¾çÀ» ¹Ì¸® È­Àü½ÃÄÑ¼­ board ÆÇ°ú °ãÃÄÁö´ÂÁö
						 * °Ë»çÇÑ´Ù. °ãÃÄÁö¸é ¾Æ¹«°Íµµ ÇÏÁö ¾Ê°í switch¹®À» Å»Ãâ */
					case 18432: // up arrow key°¡ ´­·ÁÁö¸é ÇöÀç ¸ğ¾çÀÇ º®µ¹À» Áö¿ì°í */
						comp_board(num, (m + 1) % 4, nx, ny, test);
						if (*test == 1)
							*test = 0;
						else {
							erase(num, m, nx, ny);
							m = (++m) % 4; /* mÀ» Áõ°¡½ÃÅ°°í 4·Î ³ª´« ³ª¸ÓÁö °ªÀ» m¿¡ ´ëÀÔ */
							draw(num, m, nx, ny); /* º®µ¹À» ±×¸°´Ù */
						}
						break;
						/* ½ºÆäÀÌ½º ¹Ù°¡ ´­·¯Áö¸é º®µ¹ÀÇ yÁÂÇ¥¸¦ ÇÏ³ª¾¿ Â÷·Ê·Î Áõ°¡½ÃÅ°¸é¼­
						 * board ÆÇ°ú ºñ±³ÇÑ´Ù. ¶³¾îÁö´Â º®µ¹°ú board ÆÇÀÌ 1·Î °ãÃÄÁú ¶§±îÁö
						 * ¹İº¹ÇÑ´Ù. °Ë»ç¸¦ ´ÙÇÑ ÈÄ¿¡ º®µ¹ÀÇ Á¤º¸¸¦ board ¹è¿­¿¡ ÀúÀåÇÑ´Ù. */
					case 14624: // space bar key
						erase(num, m, nx, ny);

						while (*test != 1) comp_board(num, m, nx, ++ny, test);

						ny = ny - 1;
						draw(num, m, nx, ny);
						set_board(num, m, nx, ny);
						sound1(); /* º®µ¹À» ³õÀ» ¶§ ¼Ò¸® ¹ß»ı */
						test_board(ny); /* ÇÑ ÁÙÀÌ ¸ğµÎ 1ÀÎÁö °Ë»ç */
						a = 1;
						break;

					case 283: /* esc key°¡ ´­·ÁÁö¸é ±×·¡ÇÈÀ» Á¾·á½ÃÅ²´Ù. */
						goto OUT_1;
#if 0
						closegraph();
						printf("\n\nGood Bye!\n");
						exit(1); /* ÇÔ¼ö¸¦ Á¾·á½ÃÅ´ */
#endif

					default:
						break;
					} /* switch */
				} /* if( kbhit() ) */
				if (a == 1) break; /* ½ºÆäÀÌ½º ¹Ù°¡ ´­·¯Áö¸é for¹®À» Å»ÃâÇÑ´Ù. */
			} /* for (delay) */

			if (*test != 1) { /* ½ºÆäÀÌ½º ¹Ù°¡ ¾Æ´Ñ ´Ù¸¥ Å°°¡ ´­·¯Áø °æ¿ì */
							 /* ¶Ç´Â ¾Æ¹« Å°µµ ´­·¯ÁöÁö ¾ÊÀº °æ¿ì */
				comp_board(num, m, nx, ny + 1, test); /* ¹Ì¸® ÇÑÄ­ ³»·Á¼­ ºñ±³ */

				if (*test == 1) /* °ãÃÄÁö¸é º®µ¹ÀÇ Á¤º¸¸¦ board ¹è¿­¿¡ ÀúÀå */
				{
					set_board(num, m, nx, ny);
					sound1(); /* º®µ¹À» ³õÀ» ¶§ ¼Ò¸® ¹ß»ı */
					test_board(ny); /* ÇÑ ÁÙÀÌ ¸ğµÎ 1ÀÎÁö °Ë»ç */
				}
				else { /* ±×·¸Áö ¾ÊÀ¸¸é º®µ¹ÀÇ Á¤º¸¸¦ board ¹è¿­¿¡ ÀúÀå */
					erase(num, m, nx, ny);
					ny++; /* yÁÂÇ¥¸¦ Áõ°¡½ÃÅ´ */
				}
			}
		} while (*test == 0 && ny < 20); /* °ãÃÄÁüÀÌ ¾ø´Â µ¿¾È, ¾Æ·¡·Î 20Ä­ ¶³¾îÁú ¶§±îÁö ¹İº¹ */
		*test = 0; /* °ãÃÄÁüÀÌ ÀÖÀ¸¸é *test¸¦ 0À¸·Î ÇÏ°í »õ·Î¿î º®µ¹À» ¶³±º´Ù. */
		erase(num1, 0, -5, 3); /* ¿¹°íÆÇÀ» Áö¿ö ÁÖ°í */
		num = num1; /* num1À» num¿¡ ´ëÀÔÇÑ´Ù */

	}
OUT_1:
	sound2(); /* °ÔÀÓÀÌ ³¡³µÀ» ¶§ ¼Ò¸® ¹ß»ı */
}

/* ************************* */
/*  º®µ¹À» ±×¸®´Â ÇÔ¼ö       */
/* ************************* */
void draw(const int num, const int m, const int nx, const int ny)
{
	int row, col;
	setfillstyle(SOLID_FILL, color[num + 1]);
	for (row = 0; row < 4; row++) /* ¹è¿­ÀÇ ¼ººĞ°Ë»ç */
		for (col = 0; col < 4; col++)
		{
			if (block[num][m][row][col] == 1) { /* ¸¸ÀÏ 1ÀÌ ÀÖÀ¸¸é º®µ¹À» ±×¸°´Ù */
				bar(X + WD * (nx + col), Y + WD * (ny + row), X + WD * (nx + col + 1), Y + WD * (ny + row + 1));
				rectangle(X + WD * (nx + col) + 1, Y + WD * (ny + row) + 1, X + WD * (nx + col + 1) - 1, Y + WD * (ny + row + 1) - 1);
				/* Èò»öÀÇ »ç°¢Çü ¶ì¸¦ ¸¸µé¾îÁØ´Ù */
			}
		}
}

/* ************************* */
/*  º®µ¹À» Áö¿ì´Â ÇÔ¼ö       */
/* ************************* */
void erase(const int num, const int m, const int nx, const int ny)
{
	int row, col;
	setfillstyle(SOLID_FILL, BLACK);
	for (row = 0; row < 4; row++) /* ¹è¿­ÀÇ °¢ ¼ººĞÀ» °Ë»çÇÏ¿© */
		for (col = 0; col < 4; col++) /* 1ÀÌ ÀÖ´Â À§Ä¡¿¡ ÀÛÀº Á¤»ç°¢Çü */
			if (block[num][m][row][col] == 1) bar(X + WD * (nx + col), Y + WD * (ny + row), X + WD * (nx + col + 1), Y + WD * (ny + row + 1));
	/* º®µ¹À» ¹è°æ»öÀ¸·Î ±×¸°´Ù */
}

/* *************************** */
/* º¸µå ¹è¿­À» ÃÊ±âÈ­ÇÏ´Â ÇÔ¼ö */
/* *************************** */

void clear_board(void)
{
	int i, j;

	for (i = 0; i < 20; i++)
		for (j = 1; j < 11; j++)
			board[i][j] = 0;
	for (i = 0; i < 20; i++) /* º®¿¡ ÇØ´çÇÏ´Â ºÎºĞ¿¡ 1À» ÀúÀå */
		board[i][0] = 1;
	for (i = 0; i < 20; i++)
		board[i][11] = 1;
	for (i = 0; i < 12; i++)
		board[20][i] = 1;
}

/* ************************************** */
/* board¿Í ¶³¾îÁö´Â º®µ¹À» ºñ±³ÇÏ´Â ÇÔ¼ö  */
/* ************************************** */

void comp_board(const int num, const int m, const int nx, const int ny, int* test)
{
	int i, j;

	for (i = ny; i < ny + 4 && i <= 20; i++)
	{
		for (j = nx; j < nx + 4; j++) /* ¶³¾îÁö´Â º®µ¹°ú board ¹è¿­ÀÌ °ãÃÄÁö´ÂÁö °Ë»ç */
			if (board[i][j] == 1 && (board[i][j] == block[num][m][i - ny][j - nx]))
				*test = 1; /* ¸¸ÀÏ 1·Î °ãÃÄÁö¸é *test¸¦ 1·Î ÇÑ´Ù */
	}
}

/* ****************************************** */
/* ¶³¾îÁø º®µ¹ÀÇ ¸ğ¾çÀ» board¿¡ ÀúÀåÇÏ´Â ÇÔ¼ö */
/* ****************************************** */

void set_board(int num, int m, int nx, int ny) {
	int i, j;

	for (i = ny; (i < ny + 4 && i < 20); i++) /* ¶³¾îÁø º®µ¹ÀÇ ¹è¿­À» °Ë»çÇÏ¿© */
		for (j = nx; j < nx + 4; j++)
			if (block[num][m][i - ny][j - nx] == 1) /* 1ÀÌ ÀÖÀ¸¸é board ¹è¿­¿¡ ÀúÀå */
				board[i][j] = block[num][m][i - ny][j - nx];
}

/* ************************ */
/* ¹è°æÈ­¸éÀ» ±×·ÁÁÖ´Â ÇÔ¼ö */
/* ************************ */

void back_screen(void)
{
	int i, j, k;

	setfillstyle(SOLID_FILL, LIGHTBLUE);
	for (i = 0; i < 20; i++) /* ¿ŞÂÊ º®À» ±×¸°´Ù */
	{
		bar(X, Y + WD * i, X + WD, Y + WD * (i + 1));
		rectangle(X + 1, Y + WD * i + 1, X + WD - 1, Y + WD * (i + 1) - 1);
	}
	setfillstyle(SOLID_FILL, LIGHTBLUE);
	for (j = 0; j < 20; j++) /* ¿À¸¥ÂÊ º®À» ±×¸°´Ù */
	{
		bar(X + WD * 11, Y + WD * j, X + WD * 12, Y + WD * (j + 1));
		rectangle(X + WD * 11, Y + WD * j + 1, X + WD * 12, Y + WD * (j + 1) - 1);
	}

	setfillstyle(SOLID_FILL, LIGHTBLUE);
	for (k = 0; k < 12; k++) /* ¹Ù´ÚÀ» ±×¸°´Ù */
	{
		bar(X + WD * k, Y + WD * 20, X + WD * (k + 1), Y + WD * 21);
		rectangle(X + WD * k + 1, Y + WD * 20, X + WD * (k + 1), Y + WD * 21);
	}

	setfillstyle(SOLID_FILL, LIGHTBLUE);
	bar(40, 186, 193, 217); /* Á¡¼öÆÇÀ» ±×·Á ÁÖ´Â ÇÔ¼ö */
	bar(40, 237, 190, 239);
	bar(40, 257, 190, 259);
	bar(40, 277, 190, 279);
	bar(40, 297, 190, 299);
	bar(40, 317, 190, 321);
	bar(39, 346, 190, 350);
	bar(39, 187, 43, 347);
	bar(190, 186, 194, 350);
	rectangle(41, 188, 192, 348);
	line(41, 214, 191, 214);
	line(41, 319, 191, 319);
	settextstyle(0, HORIZ_DIR, 1);
	//	outtextxy(46,194,"HIGH SCORE");
	gprintf_XY( 46, 194, YELLOW, LIGHTBLUE, "  ÂA  ‰¡  ¸ñ  ® " );

}

/* ******************************* */
/* key°¡ ´­¸± ¶§±îÁö ±â´Ù¸®´Â ÇÔ¼ö */
/* ******************************* */

int Pause(void)
{
	int c = 0;

	gprintf_XY(46, 194, YELLOW, LIGHTBLUE, "CONTINUE ? (Y/N)");
	c = getch();
	if (c == 'n' || c == 'N') return(1);

	return(0);

	//	if (c == 0)
	//		c = getch();
}

/* ******************************************* */
/* boardÀÇ ÇÑ ÁÙÀÌ ´Ù Ã¤¿öÁ³´ÂÁö °Ë»çÇÏ´Â ÇÔ¼ö */
/* ******************************************* */

void test_board(const int ny)
{
	int i, j, k, test1, n = 1; /* Áö¿ö¾ß ÇÔÀ» ¾Ë¸®´Â test1 º¯¼ö ¼±¾ğ */
	int x, y;

	unsigned size; /* test1ÀÌ 1ÀÌ¸é ±× ÁÙÀº Áö¿ö¾ß ÇÑ´Ù */
	void far* image;

	if (ny >= 16) /* ¶³¾îÁø º®µ¹ÀÌ ¹Ù´Ú ±ÙÃ³¿¡ ÀÖÀ» ¶§ */
	{
		i = 19; /* boardÀÇ 19¹øÂ° ÁÙºÎÅÍ °Ë»ç */

		while (n <= (20 - ny))
		{
			x = 40, y = 328;

			n++;
			test1 = 1;
			for (j = 1; j < 11; j++) /* board¸¦ °Ë»çÇØ¼­ ¸¸ÀÏ ÇÏ³ª¶óµµ 0ÀÌ¸é */
			{
				if (board[i][j] == 0) /* ºó °÷ÀÌ ÀÖÀ¸¹Ç·Î test1À» 0À¸·Î ÇÑ´Ù */
					test1 = 0;
			}
			if (test1 == 1) /* ÇÑ ÁÙÀÌ ´Ù Ã¤¿öÁ³À¸¸é */
			{
				size = imagesize(X, Y, X + WD * 11, Y + WD * i);
				image = farmalloc(size); /* Ã¤¿öÁø ºÎºĞÀÇ À­ ºÎºĞ¿¡ ÇØ´çÇÏ´Â Å©±â¸¸Å­ */
				/* ¸Ş¸ğ¸®¸¦ ÇÒ´çÇÑ´Ù. */

				getimage(X + WD, Y, X + WD * 11, Y + WD * i - 1, image); /* ÀÌ¹ÌÁö ÀúÀåÇÑ ÈÄ */
				setfillstyle(SOLID_FILL, BLACK); /* Áö¿ö¾ß ÇÒ ÁÙÀ» Áö¿ì°í */
				bar(X + WD, Y + WD * i - 1, X + WD * 11, Y + WD * (i + 1) + 1);

				sound4();
				newscr += 50;
				setfillstyle(SOLID_FILL, BLACK);
				bar(45, 327, 188, 345);
				settextstyle(0, 0, 2);
				gprintf(x, y, "%9lu", newscr);


				/* ÇÑ ÁÙ ³»·Á¼­ ÀúÀåÇÑ ÀÌ¹ÌÁö¸¦ ±×·ÁÁØ´Ù. */
				putimage(X + WD, Y + WD, image, COPY_PUT);

				farfree(image);

				for (j = i; j > 0; j--)
					for (k = 1; k < 11; k++) /* board ÆÇÀ» ÇÏ³ª¾¿ ³»·Á¼­ µ¤¾î¾´´Ù */
						board[j][k] = board[j - 1][k];

				/* ¸Ç À­ ÁÙÀ» 0À¸·Î */
				for (k = 1; k < 11; k++)
					board[0][k] = 0;
			}

			/* ÇÑ ÁÙÀ» Áö¿ìÁö ¾Ê¾ÒÀ¸¸é ±× À§ÀÇ ÁÙÀ» °Ë»çÇØ¾ß ÇÑ´Ù. */
			if (test1 != 1) i--;
		}
	}
	else /* ny°¡ 16º¸´Ù ÀÛÀ» ¶§´Â ¹«Á¶°Ç 4ÁÙÀ» °Ë»ç */
	{
		i = ny + 3;
		while (n <= 4)
		{
			n++;
			test1 = 1;

			x = 40, y = 328;

			for (j = 1; j < 11; j++)
			{
				if (board[i][j] == 0)
					test1 = 0;
			}
			if (test1 == 1) /* ÇÑ ÁÙÀÌ ´Ù Ã¤¿öÁ³À¸¸é  */
			{
				size = imagesize(X, Y, X + WD * 11, Y + WD * i);
				image = farmalloc(size); /* Ã¤¿öÁø ºÎºĞÀÇ À­ ºÎºĞ¿¡ ÇØ´çÇÏ´Â Å©±â¸¸Å­ */
				/* ¸Ş¸ğ¸®¸¦ ÇÒ´çÇÑ´Ù. */

				getimage(X + WD, Y, X + WD * 11, Y + WD * i - 1, image);
				setfillstyle(SOLID_FILL, BLACK);
				bar(X + WD, Y + WD * i - 1, X + WD * 11, Y + WD * (i + 1) + 1);

				sound4();
				newscr += 50;
				setfillstyle(SOLID_FILL, BLACK);
				bar(45, 327, 188, 345);
				settextstyle(0, 0, 2);
				gprintf(x, y, "%9lu", newscr);


				/* ÇÑ ÁÙ ³»·Á¼­ ÀúÀåÇÑ ÀÌ¹ÌÁö¸¦ ±×·ÁÁØ´Ù. */
				putimage(X + WD, Y + WD, image, COPY_PUT);

				farfree(image);

				for (j = i; j > 0; j--)
					for (k = 1; k < 11; k++) /* board ÆÇÀ» ÇÏ³ª¾¿ ³»·Á¼­ µ¤¾î¾´´Ù */
						board[j][k] = board[j - 1][k];

				/* ¸Ç À­ ÁÙÀ» 0À¸·Î */
				for (k = 1; k < 11; k++)
					board[0][k] = 0;
			}

			/* ÇÑ ÁÙÀ» Áö¿ìÁö ¾Ê¾ÒÀ¸¸é ±× À§ÀÇ ÁÙÀ» °Ë»çÇØ¾ß ÇÑ´Ù. */
			if (test1 != 1) i--;
		}
	}
}

/* ***************************** */
/*  º®µ¹À» ³õÀ»¶§ ¼Ò¸® ³»´Â ÇÔ¼ö */
/* ***************************** */
void sound1(void)
{
	int i;

	for (i = 0; i < 10; i++)
	{
		//sound(500 + i);
		delay(5);
	}
	//nosound();
}

/* ******************************** */
/*  °ÔÀÓÀÌ ³¡³µÀ» ¶§ ¼Ò¸® ³»´Â ÇÔ¼ö */
/* ******************************** */
void sound2(void)
{
	int h, i = 0;

	for (h = 1; i < 8; h++) {
		for (i = 0; i < 15; i++) {
			//	sound(1000 + 2 * h * h * i);
			delay(5 + i);
		}
	}
	//nosound();
}

/* ******************************** */
/*  °ÔÀÓÀ» ½ÃÀÛÇÒ ¶§ ¼Ò¸® ³»´Â ÇÔ¼ö */
/* ******************************** */
void sound3(void)
{
	int i;

	for (i = 10; i > 0; i--)
	{
		//	sound(400 + (i % 3 - 1) * i * 50);
		delay(180);
	}
	//sound(400);
	delay(400);
	//	nosound();
}

/* ****************************** */
/*  ÇÑÁÙÀ» Áö¿ï ¶§ ¼Ò¸® ³»´Â ÇÔ¼ö */
/* ****************************** */
void sound4(void)
{
	int h, i;

	for (h = 1; h < 8; h++) {
		for (i = 0; i < 10; i++) {
			//	sound(500 + 2 * h * h * i);
			delay(3);
		}
	}
	//nosound();
}

/* *********************************************** */
/* Á¡¼öÆÇÀÇ Á¡¼ö¿Í newscrÀ» ºñ±³ÇÏ¿© Á¤·ÄÇÏ´Â ÇÔ¼ö */
/* *********************************************** */

void highscr(void)
{
	static unsigned long int begin[5] = { 0,0,0,0,0 }; /* ÃÊ±â°ª */
	static unsigned long int score[5]; /* ÀúÀåµÉ °ª°ú »õ·Î¿î °ª */
	static unsigned long int t[5]; /* ¸Å°³º¯¼ö */
	int i, j;

	FILE* fp;

	if ((fp = fopen("dd.dat", "r")) == NULL) { /* ÀúÀåÇÒ fileÀÌ ¾øÀ¸¸é fileÀ» »ı¼ºÇÏ´Â ·çÆ¾ */
		fclose(fp);
		fp = fopen("dd.dat", "w+");
		for (i = 0; i < 5; i++)
			fprintf(fp, "%d\n", begin[i]); /* »ı¼ºµÈ file¿¡ score °ªÀ» ÃÊ±âÈ­ */
	}
	for (i = 0; i < 5; i++) /* fileÀÌ ÀÌ¹Ì ÀÖÀ¸¸é »õ·Î¿î °ª°ú ºñ±³ÇÏ±â À§ÇØ ºÒ·¯¿È */
		fscanf(fp, "%lu", &score[i]);
	fclose(fp); /* »õ·Î Á¤·ÄµÈ °ªÀ» ³Ö±â À§ÇØ fileÀ» ´Ù½Ã ¿­¾î¼­ */
	fp = fopen("dd.dat", "w+"); /* ÀüÀÇ µ¥ÀÌÅÍ¸¦ Áö¿ò */
	for (i = 0; i < 5; i++) {
		if (newscr >= score[i]) { /* »õ·Î¿î score °ªÀ» ÀüÀÇ score °ª°ú ºñ±³ÇÏ¿© */
			t[0] = score[i]; /* ´Ù½Ã Á¤·ÄÇÏ°í ¹è¿­ score¿¡ Â÷·Ê·Î ÀúÀå */
			score[i] = newscr;
			for (j = 0; i + j < 5; j++) {
				t[j + 1] = score[i + j + 1];
				score[i + j + 1] = t[j];
			}
			break;
		}
	}
	for (i = 0; i < 5; i++) /* Á¤·ÄµÈ °ªµéÀ» ´Ù½Ã openÇÑ file¿¡ ÀúÀå */
		fprintf(fp, "%lu\n", score[i]);
	fclose(fp);
}

/* ***************************** */
/* Á¡¼öÆÇ¿¡ Á¡¼ö¸¦ Ç¥½ÃÇÏ´Â ÇÔ¼ö */
/* ***************************** */

void display_score(void)
{
	unsigned long int score[5];
	int i, x, y;

	FILE* fp;

	if ((fp = fopen("dd.dat", "r")) == NULL) /* socreµéÀ» ºÒ·¯¿À±â À§ÇØ fileÀ» */
											/* ¿­°í fileÀÌ ¾øÀ¸¸é highscr() È£Ãâ */
		highscr();
	else {
		for (i = 0; i < 5; i++)
			fscanf(fp, "%lu", &score[i]); /* file¿¡¼­ score¸¦ ºÒ·¯¿À´Â ÇÔ¼ö */
	}
	fclose(fp);

	setfillstyle(SOLID_FILL, BLACK); /* score ¾µ ÀÚ¸®¸¦ ¹è°æ »öÀ¸·Î Áö¿ò */

	for (i = 0; i < 5; i++)
	{
		bar(DX, i * 20 + 219, DX + 143, i * 20 + 237); /* score ¾µ ÀÚ¸® */
		x = STX; /* ¹®ÀÚ À§Ä¡ */
		y = i * 20 + 222;
		settextstyle(2, 0, 7);
		gprintf(x, y, "%lu", score[i]); /* À§Ä¡¿¡ score¸¦ Ãâ·Â */
	}
}

/* ***************************************** */
/* ±×·¡ÇÈ ¸ğµå¿¡¼­ printf() ±â´ÉÀ» ÇÏ´Â ÇÔ¼ö */
/* ***************************************** */

void gprintf(int xpos, int ypos, char* format, ...) {
	char string[140];
	va_list argptr;

	va_start(argptr, format);
	(void)vsprintf(string, format, argptr);
	outtextxy(xpos, ypos, string);
	va_end(argptr);

}

