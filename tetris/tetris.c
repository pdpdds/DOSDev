/* ************************************************************* */
/* TETRIS Game Source                                            */
/*                                                               */
/* ������ : (�ĸ͵� �� �� �ִ�) PC���� �����, ������, 1996. 04. */
/*                                                               */
/* Ÿ���� : ���̹� ī�� �����ڹ���, �˶��, 2022. 02.            */
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
	// ù�κ��� ���ڴ� ���� �������� �ʱ⿡, �պκп��� ������ ������ �̾Ƽ� ����
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
/*  ������ ������ �Լ�       */
/* ************************  */
void base_block(void)
{
	int nx = 0, ny = 0, bios1; /* ���� nx�� ����, key �Է��� ������ ������ ���� */
	int i;
	int m = 0;  /* ȸ���� ����� ��Ÿ���� ÷�� m���� */
	int a = 0;  /* �����̽� �ٰ� �������� �˸��� ÷�� a ���� */
	int num = 0;
	int num1 = 0;
	int value = 0;
	int* test = &value;

	back_screen(); /* ��Ʈ���� ��� ȭ���� �׷� �ش� */
	clear_board(); /* board�� �ʱ�ȭ�Ѵ� */
	sound3(); /* ���� ������ �� �Ҹ� �߻� */
	randomize();
	num = random(7); /* ������ �߻���Ŵ */

	while (board[1][6] != 1) { /* board�� �� ���� ����� ä���� ������ ������ ����߸��� */
		a = 0; /* a�� ������ ����߸� ������ �ʱ�ȭ�Ѵ�. */
		m = 0;
		nx = 5;
		ny = 0;

		num1 = random(7); /* while�� �ȿ��� ������ �߻���Ŵ */
		/* ó�� �����Ҷ��� �� ���� ���� �߻� */

		gprintf_XY(40, 77, YELLOW, LIGHTBLUE, "<  ?a ��q   ��b ?��  >");

		draw(num1, 0, -5, 3); /* ���� ������ ���� ��ġ�� �׷� �ش� */

		do {

			draw(num, m, nx, ny); /* ������ ����߸��� */
			for (i = 0; i < 50; i++) /* delay(500)�� 50���� ���� �ݺ��ؼ� �Ѵ� */
			{
				if (ny >= 20) break;
				delay(10); /* delay(10)�� �� ������ key�� �ԷµǾ����� �˻� */

				if (kbhit()) /* keyboard�� ������������ �˻� */
				{
					while (!bioskey(1));

					bios1 = bioskey(0); /* key�� �Է¹޴´� */

					switch (bios1) { /* ��� key�� ������������ �˻� */
						/* ���� ȭ��ǥ�� �������� x��ǥ�� �̸� �ϳ� ����Ű�� ������ board�ǰ�
						 * 1�� ���������� �˻��Ͽ� �ƹ��͵� ���� �ʰ� switch ���� Ż���Ѵ�.
						 * �������� ������ �տ���ó�� �Ѵ�. */
					case 19200: /* left arrowkey�� �������� ���� ��ġ�� ������ ����� */
						comp_board(num, m, nx - 1, ny, test);
						if (*test == 1)
							*test = 0;
						else {
							erase(num, m, nx, ny);
							--nx; /* x��ǥ�� �ϳ� ���ҽ�Ų �� */
							draw(num, m, nx, ny); /* �װ��� ������ �׸��� */
						}

						break;
						/* ������ ȭ��ǥ�� �ԷµǸ� ������ ���������� �̸� �ű� �Ŀ� board �ǰ�
						 * ���Ͽ� 1�� �������� �ƹ��͵� ���� �ʰ� switch �� Ż�� */
					case 19712: /* right arrow key�� �������� ���� ��ġ�� ������ ����� */
						comp_board(num, m, nx + 1, ny, test);
						if (*test == 1)
							*test = 0;
						else {
							erase(num, m, nx, ny);
							++nx; /* x��ǥ�� �ϳ� ���� ��Ų �� */
							draw(num, m, nx, ny); /* �װ��� ������ �׸��� */
						}
						break;
						/* ȸ��Ű�� �������� ����� �̸� ȭ�����Ѽ� board �ǰ� ����������
						 * �˻��Ѵ�. �������� �ƹ��͵� ���� �ʰ� switch���� Ż�� */
					case 18432: // up arrow key�� �������� ���� ����� ������ ����� */
						comp_board(num, (m + 1) % 4, nx, ny, test);
						if (*test == 1)
							*test = 0;
						else {
							erase(num, m, nx, ny);
							m = (++m) % 4; /* m�� ������Ű�� 4�� ���� ������ ���� m�� ���� */
							draw(num, m, nx, ny); /* ������ �׸��� */
						}
						break;
						/* �����̽� �ٰ� �������� ������ y��ǥ�� �ϳ��� ���ʷ� ������Ű�鼭
						 * board �ǰ� ���Ѵ�. �������� ������ board ���� 1�� ������ ������
						 * �ݺ��Ѵ�. �˻縦 ���� �Ŀ� ������ ������ board �迭�� �����Ѵ�. */
					case 14624: // space bar key
						erase(num, m, nx, ny);

						while (*test != 1) comp_board(num, m, nx, ++ny, test);

						ny = ny - 1;
						draw(num, m, nx, ny);
						set_board(num, m, nx, ny);
						sound1(); /* ������ ���� �� �Ҹ� �߻� */
						test_board(ny); /* �� ���� ��� 1���� �˻� */
						a = 1;
						break;

					case 283: /* esc key�� �������� �׷����� �����Ų��. */
						goto OUT_1;
#if 0
						closegraph();
						printf("\n\nGood Bye!\n");
						exit(1); /* �Լ��� �����Ŵ */
#endif

					default:
						break;
					} /* switch */
				} /* if( kbhit() ) */
				if (a == 1) break; /* �����̽� �ٰ� �������� for���� Ż���Ѵ�. */
			} /* for (delay) */

			if (*test != 1) { /* �����̽� �ٰ� �ƴ� �ٸ� Ű�� ������ ��� */
							 /* �Ǵ� �ƹ� Ű�� �������� ���� ��� */
				comp_board(num, m, nx, ny + 1, test); /* �̸� ��ĭ ������ �� */

				if (*test == 1) /* �������� ������ ������ board �迭�� ���� */
				{
					set_board(num, m, nx, ny);
					sound1(); /* ������ ���� �� �Ҹ� �߻� */
					test_board(ny); /* �� ���� ��� 1���� �˻� */
				}
				else { /* �׷��� ������ ������ ������ board �迭�� ���� */
					erase(num, m, nx, ny);
					ny++; /* y��ǥ�� ������Ŵ */
				}
			}
		} while (*test == 0 && ny < 20); /* �������� ���� ����, �Ʒ��� 20ĭ ������ ������ �ݺ� */
		*test = 0; /* �������� ������ *test�� 0���� �ϰ� ���ο� ������ ������. */
		erase(num1, 0, -5, 3); /* �������� ���� �ְ� */
		num = num1; /* num1�� num�� �����Ѵ� */

	}
OUT_1:
	sound2(); /* ������ ������ �� �Ҹ� �߻� */
}

/* ************************* */
/*  ������ �׸��� �Լ�       */
/* ************************* */
void draw(const int num, const int m, const int nx, const int ny)
{
	int row, col;
	setfillstyle(SOLID_FILL, color[num + 1]);
	for (row = 0; row < 4; row++) /* �迭�� ���а˻� */
		for (col = 0; col < 4; col++)
		{
			if (block[num][m][row][col] == 1) { /* ���� 1�� ������ ������ �׸��� */
				bar(X + WD * (nx + col), Y + WD * (ny + row), X + WD * (nx + col + 1), Y + WD * (ny + row + 1));
				rectangle(X + WD * (nx + col) + 1, Y + WD * (ny + row) + 1, X + WD * (nx + col + 1) - 1, Y + WD * (ny + row + 1) - 1);
				/* ����� �簢�� �츦 ������ش� */
			}
		}
}

/* ************************* */
/*  ������ ����� �Լ�       */
/* ************************* */
void erase(const int num, const int m, const int nx, const int ny)
{
	int row, col;
	setfillstyle(SOLID_FILL, BLACK);
	for (row = 0; row < 4; row++) /* �迭�� �� ������ �˻��Ͽ� */
		for (col = 0; col < 4; col++) /* 1�� �ִ� ��ġ�� ���� ���簢�� */
			if (block[num][m][row][col] == 1) bar(X + WD * (nx + col), Y + WD * (ny + row), X + WD * (nx + col + 1), Y + WD * (ny + row + 1));
	/* ������ �������� �׸��� */
}

/* *************************** */
/* ���� �迭�� �ʱ�ȭ�ϴ� �Լ� */
/* *************************** */

void clear_board(void)
{
	int i, j;

	for (i = 0; i < 20; i++)
		for (j = 1; j < 11; j++)
			board[i][j] = 0;
	for (i = 0; i < 20; i++) /* ���� �ش��ϴ� �κп� 1�� ���� */
		board[i][0] = 1;
	for (i = 0; i < 20; i++)
		board[i][11] = 1;
	for (i = 0; i < 12; i++)
		board[20][i] = 1;
}

/* ************************************** */
/* board�� �������� ������ ���ϴ� �Լ�  */
/* ************************************** */

void comp_board(const int num, const int m, const int nx, const int ny, int* test)
{
	int i, j;

	for (i = ny; i < ny + 4 && i <= 20; i++)
	{
		for (j = nx; j < nx + 4; j++) /* �������� ������ board �迭�� ���������� �˻� */
			if (board[i][j] == 1 && (board[i][j] == block[num][m][i - ny][j - nx]))
				*test = 1; /* ���� 1�� �������� *test�� 1�� �Ѵ� */
	}
}

/* ****************************************** */
/* ������ ������ ����� board�� �����ϴ� �Լ� */
/* ****************************************** */

void set_board(int num, int m, int nx, int ny) {
	int i, j;

	for (i = ny; (i < ny + 4 && i < 20); i++) /* ������ ������ �迭�� �˻��Ͽ� */
		for (j = nx; j < nx + 4; j++)
			if (block[num][m][i - ny][j - nx] == 1) /* 1�� ������ board �迭�� ���� */
				board[i][j] = block[num][m][i - ny][j - nx];
}

/* ************************ */
/* ���ȭ���� �׷��ִ� �Լ� */
/* ************************ */

void back_screen(void)
{
	int i, j, k;

	setfillstyle(SOLID_FILL, LIGHTBLUE);
	for (i = 0; i < 20; i++) /* ���� ���� �׸��� */
	{
		bar(X, Y + WD * i, X + WD, Y + WD * (i + 1));
		rectangle(X + 1, Y + WD * i + 1, X + WD - 1, Y + WD * (i + 1) - 1);
	}
	setfillstyle(SOLID_FILL, LIGHTBLUE);
	for (j = 0; j < 20; j++) /* ������ ���� �׸��� */
	{
		bar(X + WD * 11, Y + WD * j, X + WD * 12, Y + WD * (j + 1));
		rectangle(X + WD * 11, Y + WD * j + 1, X + WD * 12, Y + WD * (j + 1) - 1);
	}

	setfillstyle(SOLID_FILL, LIGHTBLUE);
	for (k = 0; k < 12; k++) /* �ٴ��� �׸��� */
	{
		bar(X + WD * k, Y + WD * 20, X + WD * (k + 1), Y + WD * 21);
		rectangle(X + WD * k + 1, Y + WD * 20, X + WD * (k + 1), Y + WD * 21);
	}

	setfillstyle(SOLID_FILL, LIGHTBLUE);
	bar(40, 186, 193, 217); /* �������� �׷� �ִ� �Լ� */
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
	gprintf_XY(46, 194, YELLOW, LIGHTBLUE, "  ?  ?��  ��??? ");

}

/* ******************************* */
/* key�� ���� ������ ��ٸ��� �Լ� */
/* ******************************* */

int Pause(void)
{
	int c = 0;

	gprintf_XY(46, 194, YELLOW, LIGHTBLUE, "??���ˬ�?a��a ? (Y/N)");
	c = getch();
	if (c == 'n' || c == 'N') return(1);

	return(0);

	//	if (c == 0)
	//		c = getch();
}

/* ******************************************* */
/* board�� �� ���� �� ä�������� �˻��ϴ� �Լ� */
/* ******************************************* */

void test_board(const int ny)
{
	int i, j, k, test1, n = 1; /* ������ ���� �˸��� test1 ���� ���� */
	int x, y;

	unsigned size; /* test1�� 1�̸� �� ���� ������ �Ѵ� */
	void far* image;

	if (ny >= 16) /* ������ ������ �ٴ� ��ó�� ���� �� */
	{
		i = 19; /* board�� 19��° �ٺ��� �˻� */

		while (n <= (20 - ny))
		{
			x = 40, y = 328;

			n++;
			test1 = 1;
			for (j = 1; j < 11; j++) /* board�� �˻��ؼ� ���� �ϳ��� 0�̸� */
			{
				if (board[i][j] == 0) /* �� ���� �����Ƿ� test1�� 0���� �Ѵ� */
					test1 = 0;
			}
			if (test1 == 1) /* �� ���� �� ä�������� */
			{
				size = imagesize(X, Y, X + WD * 11, Y + WD * i);
				image = farmalloc(size); /* ä���� �κ��� �� �κп� �ش��ϴ� ũ�⸸ŭ */
				/* �޸𸮸� �Ҵ��Ѵ�. */

				getimage(X + WD, Y, X + WD * 11, Y + WD * i - 1, image); /* �̹��� ������ �� */
				setfillstyle(SOLID_FILL, BLACK); /* ������ �� ���� ����� */
				bar(X + WD, Y + WD * i - 1, X + WD * 11, Y + WD * (i + 1) + 1);

				sound4();
				newscr += 50;
				setfillstyle(SOLID_FILL, BLACK);
				bar(45, 327, 188, 345);
				settextstyle(0, 0, 2);
				gprintf(x, y, "%9lu", newscr);


				/* �� �� ������ ������ �̹����� �׷��ش�. */
				putimage(X + WD, Y + WD, image, COPY_PUT);

				farfree(image);

				for (j = i; j > 0; j--)
					for (k = 1; k < 11; k++) /* board ���� �ϳ��� ������ ����� */
						board[j][k] = board[j - 1][k];

				/* �� �� ���� 0���� */
				for (k = 1; k < 11; k++)
					board[0][k] = 0;
			}

			/* �� ���� ������ �ʾ����� �� ���� ���� �˻��ؾ� �Ѵ�. */
			if (test1 != 1) i--;
		}
	}
	else /* ny�� 16���� ���� ���� ������ 4���� �˻� */
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
			if (test1 == 1) /* �� ���� �� ä��������  */
			{
				size = imagesize(X, Y, X + WD * 11, Y + WD * i);
				image = farmalloc(size); /* ä���� �κ��� �� �κп� �ش��ϴ� ũ�⸸ŭ */
				/* �޸𸮸� �Ҵ��Ѵ�. */

				getimage(X + WD, Y, X + WD * 11, Y + WD * i - 1, image);
				setfillstyle(SOLID_FILL, BLACK);
				bar(X + WD, Y + WD * i - 1, X + WD * 11, Y + WD * (i + 1) + 1);

				sound4();
				newscr += 50;
				setfillstyle(SOLID_FILL, BLACK);
				bar(45, 327, 188, 345);
				settextstyle(0, 0, 2);
				gprintf(x, y, "%9lu", newscr);


				/* �� �� ������ ������ �̹����� �׷��ش�. */
				putimage(X + WD, Y + WD, image, COPY_PUT);

				farfree(image);

				for (j = i; j > 0; j--)
					for (k = 1; k < 11; k++) /* board ���� �ϳ��� ������ ����� */
						board[j][k] = board[j - 1][k];

				/* �� �� ���� 0���� */
				for (k = 1; k < 11; k++)
					board[0][k] = 0;
			}

			/* �� ���� ������ �ʾ����� �� ���� ���� �˻��ؾ� �Ѵ�. */
			if (test1 != 1) i--;
		}
	}
}

/* ***************************** */
/*  ������ ������ �Ҹ� ���� �Լ� */
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
/*  ������ ������ �� �Ҹ� ���� �Լ� */
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
/*  ������ ������ �� �Ҹ� ���� �Լ� */
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
/*  ������ ���� �� �Ҹ� ���� �Լ� */
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
/* �������� ������ newscr�� ���Ͽ� �����ϴ� �Լ� */
/* *********************************************** */

void highscr(void)
{
	static unsigned long int begin[5] = { 0,0,0,0,0 }; /* �ʱⰪ */
	static unsigned long int score[5]; /* ����� ���� ���ο� �� */
	static unsigned long int t[5]; /* �Ű����� */
	int i, j;

	FILE* fp;

	if ((fp = fopen("dd.dat", "r")) == NULL) { /* ������ file�� ������ file�� �����ϴ� ��ƾ */
		fclose(fp);
		fp = fopen("dd.dat", "w+");
		for (i = 0; i < 5; i++)
			fprintf(fp, "%d\n", begin[i]); /* ������ file�� score ���� �ʱ�ȭ */
	}
	for (i = 0; i < 5; i++) /* file�� �̹� ������ ���ο� ���� ���ϱ� ���� �ҷ��� */
		fscanf(fp, "%lu", &score[i]);
	fclose(fp); /* ���� ���ĵ� ���� �ֱ� ���� file�� �ٽ� ��� */
	fp = fopen("dd.dat", "w+"); /* ���� �����͸� ���� */
	for (i = 0; i < 5; i++) {
		if (newscr >= score[i]) { /* ���ο� score ���� ���� score ���� ���Ͽ� */
			t[0] = score[i]; /* �ٽ� �����ϰ� �迭 score�� ���ʷ� ���� */
			score[i] = newscr;
			for (j = 0; i + j < 5; j++) {
				t[j + 1] = score[i + j + 1];
				score[i + j + 1] = t[j];
			}
			break;
		}
	}
	for (i = 0; i < 5; i++) /* ���ĵ� ������ �ٽ� open�� file�� ���� */
		fprintf(fp, "%lu\n", score[i]);
	fclose(fp);
}

/* ***************************** */
/* �����ǿ� ������ ǥ���ϴ� �Լ� */
/* ***************************** */

void display_score(void)
{
	unsigned long int score[5];
	int i, x, y;

	FILE* fp;

	if ((fp = fopen("dd.dat", "r")) == NULL) /* socre���� �ҷ����� ���� file�� */
											/* ���� file�� ������ highscr() ȣ�� */
		highscr();
	else {
		for (i = 0; i < 5; i++)
			fscanf(fp, "%lu", &score[i]); /* file���� score�� �ҷ����� �Լ� */
	}
	fclose(fp);

	setfillstyle(SOLID_FILL, BLACK); /* score �� �ڸ��� ��� ������ ���� */

	for (i = 0; i < 5; i++)
	{
		bar(DX, i * 20 + 219, DX + 143, i * 20 + 237); /* score �� �ڸ� */
		x = STX; /* ���� ��ġ */
		y = i * 20 + 222;
		settextstyle(2, 0, 7);
		gprintf(x, y, "%llu", score[i]); /* ��ġ�� score�� ��� */
	}
}

/* ***************************************** */
/* �׷��� ��忡�� printf() ����� �ϴ� �Լ� */
/* ***************************************** */

void gprintf(int xpos, int ypos, char* format, ...) {
	char string[140];
	va_list argptr;

	va_start(argptr, format);
	(void)vsprintf(string, format, argptr);
	outtextxy(xpos, ypos, string);
	va_end(argptr);

}

