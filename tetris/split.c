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

    // ������ ȭ�� ������ ���� ������ �����صд�. 
    for(i=0; i<80; i++)
        gettext(i+1, 1, i+1, 25, &screen[i]);

    // Step 1 
    // ȭ���� �߾ӿ� �� ������ ����� ���� �κ� 
    movetext(2,1,40,25,1,1);
    movetext(41,1,79,25,42,1);
    window(40,1,41,25);
    clrscr();

    // Step 2 
    // ȭ���� ������ �����ϴ� �Լ� 
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
    // ȭ���� �ٽ� ���̱� �����ϴ� �κ�
    window(1,1,80,25);
    for(i=1; i<40; i++) {
	puttext(1,1,1,25,&screen[40-i]);
	movetext(1,1,i,25,2,1);
	puttext(80,1,80,25,&screen[39+i]);
	movetext(81-i,1,80,25,80-i,1);
	delay(10);
    }
    // Step 4 
    // ȭ���� ���� ���ʰ� �������� ȸ���ϴ� �κ� 
    puttext(1,1,1,25,&screen[40-40]);
    puttext(80,1,80,25,&screen[39+40]);
    gotoxy(current_x, current_y);
    exit(0);
#endif
}

#ifdef TEST
void main() {
    printf("ȭ���� ������ ���α׷�\n");
    printf("�ƹ� Ű�� �����ÿ�\n");

    getch();
    OpenScreen();

    printf("ȭ���� ������?\n");
    printf("�ƹ� Ű�� �����ÿ�\n");
    getch();
    CloseScreen();
}
#endif