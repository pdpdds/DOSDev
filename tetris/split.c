#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

char *screen[80][25];
int  current_x, current_y;

void OpenScreen(void) {

#ifndef _WIN32
    int i, j;

    current_x = wherex();
    current_y = wherey();

    // 현재의 화면 정보를 세로 단위로 저장해둔다. 
    for(i=0; i<80; i++)
        gettext(i+1, 1, i+1, 25, &screen[i]);

    // Step 1 
    // 화면의 중앙에 빈 공간을 만들기 위한 부분 
    movetext(2,1,40,25,1,1);
    movetext(41,1,79,25,42,1);
    window(40,1,41,25);
    clrscr();

    // Step 2 
    // 화면을 가르기 시작하는 함수 
    window(1,1,80,25);
    for(j=1; j<40; j++) {
	movetext(2,1,41-j,25,1,1);
	movetext(40+j,1,79,25,41+j,1);
	delay(10);

    }
#endif
}

void CloseScreen(void) {

#ifndef _WIN32
    int i, j;

    // Step 3
    // 화면을 다시 붙이기 시작하는 부분
    window(1,1,80,25);
    for(i=1; i<40; i++) {
	puttext(1,1,1,25,&screen[40-i]);
	movetext(1,1,i,25,2,1);
	puttext(80,1,80,25,&screen[39+i]);
	movetext(81-i,1,80,25,80-i,1);
	delay(10);
    }
    // Step 4 
    // 화면의 제일 왼쪽과 오른쪽을 회복하는 부분 
    puttext(1,1,1,25,&screen[40-40]);
    puttext(80,1,80,25,&screen[39+40]);
    gotoxy(current_x, current_y);
    exit(0);
#endif
}

#ifdef TEST
void main() {
    printf("화면이 갈리는 프로그램\n");
    printf("아무 키나 누르시요\n");

    getch();
    OpenScreen();

    printf("화면이 갈렸죠?\n");
    printf("아무 키나 누르시요\n");
    getch();
    CloseScreen();
}
#endif