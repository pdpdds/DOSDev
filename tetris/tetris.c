/* ************************************************************* */
/* TETRIS Game Source                                            */
/*                                                               */
/* 원저자 : (컴맹도 할 수 있는) PC게임 만들기, 나원균, 1996. 04. */
/*                                                               */
/* 타이핑 : 네이버 카페 도스박물관, 알라딘, 2022. 02.            */
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
	// 첫부분의 숫자는 거의 랜덤하지 않기에, 앞부분에서 랜덤한 개수로 뽑아서 버림
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
/*  벽돌을 떨구는 함수       */
/* ************************  */
void base_block(void)
{
	int nx = 0, ny = 0, bios1; /* 변수 nx를 정의, key 입력을 저장할 변수를 선언 */
	int i;
	int m = 0;  /* 회전한 모양을 나타내는 첨자 m선언 */
	int a = 0;  /* 스페이스 바가 눌러짐을 알리는 첨자 a 선언 */
	int num = 0;
	int num1 = 0;
	int value = 0;
	int* test = &value;

	back_screen(); /* 테트리스 배경 화면을 그려 준다 */
	clear_board(); /* board를 초기화한다 */
	sound3(); /* 게임 시작할 때 소리 발생 */
	randomize();
	num = random(7); /* 난수를 발생시킴 */

	while (board[1][6] != 1) { /* board의 맨 윗줄 가운데가 채워질 때가지 벽돌을 떨어뜨린다 */
		a = 0; /* a를 벽돌을 떨어뜨릴 때마다 초기화한다. */
		m = 0;
		nx = 5;
		ny = 0;

		num1 = random(7); /* while문 안에서 난수를 발생시킴 */
		/* 처음 시작할때만 두 번의 난수 발생 */

		gprintf_XY(40, 77, YELLOW, LIGHTBLUE, "<  a ·q   ¥b ©  >");

		draw(num1, 0, -5, 3); /* 다음 벽독을 예고 위치에 그려 준다 */

		do {

			draw(num, m, nx, ny); /* 벽돌을 떨어뜨린다 */
			for (i = 0; i < 50; i++) /* delay(500)을 50개로 나눠 반복해서 한다 */
			{
				if (ny >= 20) break;
				delay(10); /* delay(10)을 할 때마다 key가 입력되었는지 검사 */

				if (kbhit()) /* keyboard가 눌려졌는지를 검사 */
				{
					while (!bioskey(1));

					bios1 = bioskey(0); /* key를 입력받는다 */

					switch (bios1) { /* 어느 key가 눌려졌는지를 검사 */
						/* 왼쪽 화살표가 눌려지면 x좌표를 미리 하나 감소키니 다음에 board판과
						 * 1로 겹쳐지는지 검사하여 아무것도 하지 않고 switch 문을 탈출한다.
						 * 겹쳐지지 않으면 앞에서처럼 한다. */
					case 19200: /* left arrowkey가 눌려지면 현재 위치의 벽돌을 지우고 */
						comp_board(num, m, nx - 1, ny, test);
						if (*test == 1)
							*test = 0;
						else {
							erase(num, m, nx, ny);
							--nx; /* x좌표를 하나 감소시킨 후 */
							draw(num, m, nx, ny); /* 그곳에 벽돌을 그린다 */
						}

						break;
						/* 오른쪽 화살표가 입력되면 벽돌을 오른쪽으로 미리 옮긴 후에 board 판과
						 * 비교하여 1이 겹쳐지면 아무것도 하지 않고 switch 문 탈출 */
					case 19712: /* right arrow key가 눌려지면 현재 위치의 벽돌을 지우고 */
						comp_board(num, m, nx + 1, ny, test);
						if (*test == 1)
							*test = 0;
						else {
							erase(num, m, nx, ny);
							++nx; /* x좌표를 하나 증가 시킨 후 */
							draw(num, m, nx, ny); /* 그곳에 벽돌을 그린다 */
						}
						break;
						/* 회전키가 눌러지면 모양을 미리 화전시켜서 board 판과 겹쳐지는지
						 * 검사한다. 겹쳐지면 아무것도 하지 않고 switch문을 탈출 */
					case 18432: // up arrow key가 눌려지면 현재 모양의 벽돌을 지우고 */
						comp_board(num, (m + 1) % 4, nx, ny, test);
						if (*test == 1)
							*test = 0;
						else {
							erase(num, m, nx, ny);
							m = (++m) % 4; /* m을 증가시키고 4로 나눈 나머지 값을 m에 대입 */
							draw(num, m, nx, ny); /* 벽돌을 그린다 */
						}
						break;
						/* 스페이스 바가 눌러지면 벽돌의 y좌표를 하나씩 차례로 증가시키면서
						 * board 판과 비교한다. 떨어지는 벽돌과 board 판이 1로 겹쳐질 때까지
						 * 반복한다. 검사를 다한 후에 벽돌의 정보를 board 배열에 저장한다. */
					case 14624: // space bar key
						erase(num, m, nx, ny);

						while (*test != 1) comp_board(num, m, nx, ++ny, test);

						ny = ny - 1;
						draw(num, m, nx, ny);
						set_board(num, m, nx, ny);
						sound1(); /* 벽돌을 놓을 때 소리 발생 */
						test_board(ny); /* 한 줄이 모두 1인지 검사 */
						a = 1;
						break;

					case 283: /* esc key가 눌려지면 그래픽을 종료시킨다. */
						goto OUT_1;
#if 0
						closegraph();
						printf("\n\nGood Bye!\n");
						exit(1); /* 함수를 종료시킴 */
#endif

					default:
						break;
					} /* switch */
				} /* if( kbhit() ) */
				if (a == 1) break; /* 스페이스 바가 눌러지면 for문을 탈출한다. */
			} /* for (delay) */

			if (*test != 1) { /* 스페이스 바가 아닌 다른 키가 눌러진 경우 */
							 /* 또는 아무 키도 눌러지지 않은 경우 */
				comp_board(num, m, nx, ny + 1, test); /* 미리 한칸 내려서 비교 */

				if (*test == 1) /* 겹쳐지면 벽돌의 정보를 board 배열에 저장 */
				{
					set_board(num, m, nx, ny);
					sound1(); /* 벽돌을 놓을 때 소리 발생 */
					test_board(ny); /* 한 줄이 모두 1인지 검사 */
				}
				else { /* 그렇지 않으면 벽돌의 정보를 board 배열에 저장 */
					erase(num, m, nx, ny);
					ny++; /* y좌표를 증가시킴 */
				}
			}
		} while (*test == 0 && ny < 20); /* 겹쳐짐이 없는 동안, 아래로 20칸 떨어질 때까지 반복 */
		*test = 0; /* 겹쳐짐이 있으면 *test를 0으로 하고 새로운 벽돌을 떨군다. */
		erase(num1, 0, -5, 3); /* 예고판을 지워 주고 */
		num = num1; /* num1을 num에 대입한다 */

	}
OUT_1:
	sound2(); /* 게임이 끝났을 때 소리 발생 */
}

/* ************************* */
/*  벽돌을 그리는 함수       */
/* ************************* */
void draw(const int num, const int m, const int nx, const int ny)
{
	int row, col;
	setfillstyle(SOLID_FILL, color[num + 1]);
	for (row = 0; row < 4; row++) /* 배열의 성분검사 */
		for (col = 0; col < 4; col++)
		{
			if (block[num][m][row][col] == 1) { /* 만일 1이 있으면 벽돌을 그린다 */
				bar(X + WD * (nx + col), Y + WD * (ny + row), X + WD * (nx + col + 1), Y + WD * (ny + row + 1));
				rectangle(X + WD * (nx + col) + 1, Y + WD * (ny + row) + 1, X + WD * (nx + col + 1) - 1, Y + WD * (ny + row + 1) - 1);
				/* 흰색의 사각형 띠를 만들어준다 */
			}
		}
}

/* ************************* */
/*  벽돌을 지우는 함수       */
/* ************************* */
void erase(const int num, const int m, const int nx, const int ny)
{
	int row, col;
	setfillstyle(SOLID_FILL, BLACK);
	for (row = 0; row < 4; row++) /* 배열의 각 성분을 검사하여 */
		for (col = 0; col < 4; col++) /* 1이 있는 위치에 작은 정사각형 */
			if (block[num][m][row][col] == 1) bar(X + WD * (nx + col), Y + WD * (ny + row), X + WD * (nx + col + 1), Y + WD * (ny + row + 1));
	/* 벽돌을 배경색으로 그린다 */
}

/* *************************** */
/* 보드 배열을 초기화하는 함수 */
/* *************************** */

void clear_board(void)
{
	int i, j;

	for (i = 0; i < 20; i++)
		for (j = 1; j < 11; j++)
			board[i][j] = 0;
	for (i = 0; i < 20; i++) /* 벽에 해당하는 부분에 1을 저장 */
		board[i][0] = 1;
	for (i = 0; i < 20; i++)
		board[i][11] = 1;
	for (i = 0; i < 12; i++)
		board[20][i] = 1;
}

/* ************************************** */
/* board와 떨어지는 벽돌을 비교하는 함수  */
/* ************************************** */

void comp_board(const int num, const int m, const int nx, const int ny, int* test)
{
	int i, j;

	for (i = ny; i < ny + 4 && i <= 20; i++)
	{
		for (j = nx; j < nx + 4; j++) /* 떨어지는 벽돌과 board 배열이 겹쳐지는지 검사 */
			if (board[i][j] == 1 && (board[i][j] == block[num][m][i - ny][j - nx]))
				*test = 1; /* 만일 1로 겹쳐지면 *test를 1로 한다 */
	}
}

/* ****************************************** */
/* 떨어진 벽돌의 모양을 board에 저장하는 함수 */
/* ****************************************** */

void set_board(int num, int m, int nx, int ny) {
	int i, j;

	for (i = ny; (i < ny + 4 && i < 20); i++) /* 떨어진 벽돌의 배열을 검사하여 */
		for (j = nx; j < nx + 4; j++)
			if (block[num][m][i - ny][j - nx] == 1) /* 1이 있으면 board 배열에 저장 */
				board[i][j] = block[num][m][i - ny][j - nx];
}

/* ************************ */
/* 배경화면을 그려주는 함수 */
/* ************************ */

void back_screen(void)
{
	int i, j, k;

	setfillstyle(SOLID_FILL, LIGHTBLUE);
	for (i = 0; i < 20; i++) /* 왼쪽 벽을 그린다 */
	{
		bar(X, Y + WD * i, X + WD, Y + WD * (i + 1));
		rectangle(X + 1, Y + WD * i + 1, X + WD - 1, Y + WD * (i + 1) - 1);
	}
	setfillstyle(SOLID_FILL, LIGHTBLUE);
	for (j = 0; j < 20; j++) /* 오른쪽 벽을 그린다 */
	{
		bar(X + WD * 11, Y + WD * j, X + WD * 12, Y + WD * (j + 1));
		rectangle(X + WD * 11, Y + WD * j + 1, X + WD * 12, Y + WD * (j + 1) - 1);
	}

	setfillstyle(SOLID_FILL, LIGHTBLUE);
	for (k = 0; k < 12; k++) /* 바닥을 그린다 */
	{
		bar(X + WD * k, Y + WD * 20, X + WD * (k + 1), Y + WD * 21);
		rectangle(X + WD * k + 1, Y + WD * 20, X + WD * (k + 1), Y + WD * 21);
	}

	setfillstyle(SOLID_FILL, LIGHTBLUE);
	bar(40, 186, 193, 217); /* 점수판을 그려 주는 함수 */
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
	gprintf_XY(46, 194, YELLOW, LIGHTBLUE, "    ¡  ¸񠠮 ");

}

/* ******************************* */
/* key가 눌릴 때까지 기다리는 함수 */
/* ******************************* */

int Pause(void)
{
	int c = 0;

	gprintf_XY(46, 194, YELLOW, LIGHTBLUE, "­¢Щa¶a ? (Y/N)");
	c = getch();
	if (c == 'n' || c == 'N') return(1);

	return(0);

	//	if (c == 0)
	//		c = getch();
}

/* ******************************************* */
/* board의 한 줄이 다 채워졌는지 검사하는 함수 */
/* ******************************************* */

void test_board(const int ny)
{
	int i, j, k, test1, n = 1; /* 지워야 함을 알리는 test1 변수 선언 */
	int x, y;

	unsigned size; /* test1이 1이면 그 줄은 지워야 한다 */
	void far* image;

	if (ny >= 16) /* 떨어진 벽돌이 바닥 근처에 있을 때 */
	{
		i = 19; /* board의 19번째 줄부터 검사 */

		while (n <= (20 - ny))
		{
			x = 40, y = 328;

			n++;
			test1 = 1;
			for (j = 1; j < 11; j++) /* board를 검사해서 만일 하나라도 0이면 */
			{
				if (board[i][j] == 0) /* 빈 곳이 있으므로 test1을 0으로 한다 */
					test1 = 0;
			}
			if (test1 == 1) /* 한 줄이 다 채워졌으면 */
			{
				size = imagesize(X, Y, X + WD * 11, Y + WD * i);
				image = farmalloc(size); /* 채워진 부분의 윗 부분에 해당하는 크기만큼 */
				/* 메모리를 할당한다. */

				getimage(X + WD, Y, X + WD * 11, Y + WD * i - 1, image); /* 이미지 저장한 후 */
				setfillstyle(SOLID_FILL, BLACK); /* 지워야 할 줄을 지우고 */
				bar(X + WD, Y + WD * i - 1, X + WD * 11, Y + WD * (i + 1) + 1);

				sound4();
				newscr += 50;
				setfillstyle(SOLID_FILL, BLACK);
				bar(45, 327, 188, 345);
				settextstyle(0, 0, 2);
				gprintf(x, y, "%9lu", newscr);


				/* 한 줄 내려서 저장한 이미지를 그려준다. */
				putimage(X + WD, Y + WD, image, COPY_PUT);

				farfree(image);

				for (j = i; j > 0; j--)
					for (k = 1; k < 11; k++) /* board 판을 하나씩 내려서 덮어쓴다 */
						board[j][k] = board[j - 1][k];

				/* 맨 윗 줄을 0으로 */
				for (k = 1; k < 11; k++)
					board[0][k] = 0;
			}

			/* 한 줄을 지우지 않았으면 그 위의 줄을 검사해야 한다. */
			if (test1 != 1) i--;
		}
	}
	else /* ny가 16보다 작을 때는 무조건 4줄을 검사 */
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
			if (test1 == 1) /* 한 줄이 다 채워졌으면  */
			{
				size = imagesize(X, Y, X + WD * 11, Y + WD * i);
				image = farmalloc(size); /* 채워진 부분의 윗 부분에 해당하는 크기만큼 */
				/* 메모리를 할당한다. */

				getimage(X + WD, Y, X + WD * 11, Y + WD * i - 1, image);
				setfillstyle(SOLID_FILL, BLACK);
				bar(X + WD, Y + WD * i - 1, X + WD * 11, Y + WD * (i + 1) + 1);

				sound4();
				newscr += 50;
				setfillstyle(SOLID_FILL, BLACK);
				bar(45, 327, 188, 345);
				settextstyle(0, 0, 2);
				gprintf(x, y, "%9lu", newscr);


				/* 한 줄 내려서 저장한 이미지를 그려준다. */
				putimage(X + WD, Y + WD, image, COPY_PUT);

				farfree(image);

				for (j = i; j > 0; j--)
					for (k = 1; k < 11; k++) /* board 판을 하나씩 내려서 덮어쓴다 */
						board[j][k] = board[j - 1][k];

				/* 맨 윗 줄을 0으로 */
				for (k = 1; k < 11; k++)
					board[0][k] = 0;
			}

			/* 한 줄을 지우지 않았으면 그 위의 줄을 검사해야 한다. */
			if (test1 != 1) i--;
		}
	}
}

/* ***************************** */
/*  벽돌을 놓을때 소리 내는 함수 */
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
/*  게임이 끝났을 때 소리 내는 함수 */
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
/*  게임을 시작할 때 소리 내는 함수 */
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
/*  한줄을 지울 때 소리 내는 함수 */
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
/* 점수판의 점수와 newscr을 비교하여 정렬하는 함수 */
/* *********************************************** */

void highscr(void)
{
	static unsigned long int begin[5] = { 0,0,0,0,0 }; /* 초기값 */
	static unsigned long int score[5]; /* 저장될 값과 새로운 값 */
	static unsigned long int t[5]; /* 매개변수 */
	int i, j;

	FILE* fp;

	if ((fp = fopen("dd.dat", "r")) == NULL) { /* 저장할 file이 없으면 file을 생성하는 루틴 */
		fclose(fp);
		fp = fopen("dd.dat", "w+");
		for (i = 0; i < 5; i++)
			fprintf(fp, "%d\n", begin[i]); /* 생성된 file에 score 값을 초기화 */
	}
	for (i = 0; i < 5; i++) /* file이 이미 있으면 새로운 값과 비교하기 위해 불러옴 */
		fscanf(fp, "%lu", &score[i]);
	fclose(fp); /* 새로 정렬된 값을 넣기 위해 file을 다시 열어서 */
	fp = fopen("dd.dat", "w+"); /* 전의 데이터를 지움 */
	for (i = 0; i < 5; i++) {
		if (newscr >= score[i]) { /* 새로운 score 값을 전의 score 값과 비교하여 */
			t[0] = score[i]; /* 다시 정렬하고 배열 score에 차례로 저장 */
			score[i] = newscr;
			for (j = 0; i + j < 5; j++) {
				t[j + 1] = score[i + j + 1];
				score[i + j + 1] = t[j];
			}
			break;
		}
	}
	for (i = 0; i < 5; i++) /* 정렬된 값들을 다시 open한 file에 저장 */
		fprintf(fp, "%lu\n", score[i]);
	fclose(fp);
}

/* ***************************** */
/* 점수판에 점수를 표시하는 함수 */
/* ***************************** */

void display_score(void)
{
	unsigned long int score[5];
	int i, x, y;

	FILE* fp;

	if ((fp = fopen("dd.dat", "r")) == NULL) /* socre들을 불러오기 위해 file을 */
											/* 열고 file이 없으면 highscr() 호출 */
		highscr();
	else {
		for (i = 0; i < 5; i++)
			fscanf(fp, "%lu", &score[i]); /* file에서 score를 불러오는 함수 */
	}
	fclose(fp);

	setfillstyle(SOLID_FILL, BLACK); /* score 쓸 자리를 배경 색으로 지움 */

	for (i = 0; i < 5; i++)
	{
		bar(DX, i * 20 + 219, DX + 143, i * 20 + 237); /* score 쓸 자리 */
		x = STX; /* 문자 위치 */
		y = i * 20 + 222;
		settextstyle(2, 0, 7);
		gprintf(x, y, "%llu", score[i]); /* 위치에 score를 출력 */
	}
}

/* ***************************************** */
/* 그래픽 모드에서 printf() 기능을 하는 함수 */
/* ***************************************** */

void gprintf(int xpos, int ypos, char* format, ...) {
	char string[140];
	va_list argptr;

	va_start(argptr, format);
	(void)vsprintf(string, format, argptr);
	outtextxy(xpos, ypos, string);
	va_end(argptr);

}

