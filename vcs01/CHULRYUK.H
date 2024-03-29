
/************************************************************/
/* chulryuk.h : ���e �b�i �e�e/�a���a�A Ё���e �q���i ����*/
/************************************************************/
#include  <conio.h>
#include  <dos.h>

#define underline()    textattr(0x01)  /* ���� ���� ���    */
#define reverse()      textattr(0x70)  /* �e�� ���� ���    */
#define normal()       textattr(0x07)  /* ���w ���� ���    */
#define normalblink()  textattr(0x87)  /* ���w, �e�b�� ���� */
#define reverseblink() textattr(0xF0)  /* �e��, �e�b�� ���� */

/************************************************************/
/* �� (x,y)�A ���a�i str�i �b                             */
/************************************************************/
void xy_puts(int x, int y, char *str)
    {
    gotoxy(x,y);
    cputs(str);
    }

/************************************************************/
/* �� (x,y)�A ���a ch�i �b                                */
/************************************************************/
void xy_putch(int x, int y, char ch)
    {
    gotoxy(x,y);
    putch(ch);
    }

/************************************************************/
/* ���� (x1,y1)-(x2,y2)�i �e���巁 �a�b�w�a�� �e�i          */
/************************************************************/
void single_box(int x1, int y1, int x2, int y2)
    {
    int     x,y;

    gotoxy(x1,y1);   putch(166); putch(163);/* �a�b�w ����   �������Ŀ */
    for (x=x1+2 ; x<x2 ; x+=2)
        putch(166), putch(161);
     putch(166); putch(164);

    for (y=y1+1 ; y<y2 ; y++)    /* �E��,���e���� �       � */
	{
        gotoxy(x1,y); putch(166); putch(162);
        gotoxy(x2,y); putch(166); putch(162);
	}

    gotoxy(x1, y2); putch(166); putch(166);  /* �a����        ��������� */
    for (x=x1+2 ; x<x2 ; x+=2)
          putch(166),putch(161);
    putch(166); putch(165);

    }




    

/************************************************************/
/* ���� (x1,y1)-(x2,y2)�i �����巁 �a�b�w�a�� �e�i */
/************************************************************/
void double_box(int x1, int y1, int x2, int y2)
    {
    int   x,y;

    gotoxy(x1,y1); putch(201);    /* �a�b�w ����  �������ͻ */
    for (x=x1+1 ; x<x2 ; x++)
	putch(205);
    putch(187);

    for (y=y1+1 ; y<y2 ; y++)    /* �E��,���e���� �       � */
	{
	gotoxy(x1,y); putch(186);
	gotoxy(x2,y); putch(186);
	}

    gotoxy(x1, y2); putch(200);  /* �a����        �������ͼ */
    for (x=x1+1 ; x<x2 ; x++)
	putch(205);
    putch(188);
    }

/************************************************************/
/* ���� (x1,y1)-(x2,y2)�i ���e�巁 �a�b�w�a�� �a�� */
/************************************************************/
void thick_box(int x1, int y1, int x2, int y2)
    {
    int   x,y;

    gotoxy(x1,y1);               /* �a�b�w ����   ��������� */
    for (x=x1 ; x<=x2 ; x++)
        putch(220);

    for (y=y1 ; y<=y2 ; y++)     /* �E��,���e���� �       � */
	{
        gotoxy(x1,y); putch(220);
        gotoxy(x2,y); putch(220);
	}

    gotoxy(x1, y2);              /* �a����        ��������� */
    for (x=x1 ; x<=x2 ; x++)
        putch(220);
    }

/************************************************************/
/* ���� (x1,y)-(x2,y)�i ���w��a�� �e�i                     */
/************************************************************/
void x_line(int x1, int x2, int y)
    {
    int i;

    for(i=x1 ; i<=x2 ; i++)
	gotoxy(i,y), putch(196);
    }

/************************************************************/
/* ���� (x,y1)-(x,y2)�i ������a�� �e�i                     */
/************************************************************/
void y_line(int y1, int y2, int x)
    {
    int i;

    for(i=y1 ; i<=y2 ; i++)
	gotoxy(x,i), putch(179);
    }

/******************************************* ����title��ͻ  */
/* ���e�A ���e���� �{�e �w�i �b            �����������ĺ  */
/* line : �w���A�� ��A�ᦁ�� �y�弁�����a   �           �  */
/******************************************* �����������ͼ  */
void form_window(int x1, int y1, int x2, int y2,  int line,
						  char *title)
    {
    int   x;

    /* �w�� �A�¬�i �a���a */
    window(1,1,80,25);
    double_box(x1,y1, x2,y2);
    x_line(x1+1, x2-1, y1+line);

    /* �w �ẗ�e�A title�i �e�� �wȁ�� �a�� */
    window(x1,y1,x2,y2);

    x = (x2-x1-strlen(title))/2 + 1;

    reverse();  xy_puts(x, 1, title);  normal();

    window(1,1,80,25);
    }


/************************************************************/
/* ��᷁ �a���i �e�w                                       */
/************************************************************/
void set_cursor(int sijak, int keut)
    {
    union  REGS r;

    r.h.ah = 1;                             /* �᧡�a ��ѡ */
    r.h.ch = sijak;                         /* ���b ���a�� */
    r.h.cl = keut;                          /* �{ ���a��   */

    int86(0x10, &r, &r);        /* �a������ 0x10/01�i ѡ */
    }

/************************************************************/
/* �� �� (x1,y)-(x2,y) �a���� �����e�i attr�� ���          */
/************************************************************/
void set_attrib(int x1, int x2, int y, int attr)
    {
    unsigned  v_s;                       /* ���A�� �A�a���a */
    int       i;

    if (peekb(0,0x449) == 7) v_s = 0xB000;
    else                     v_s = 0xB800;

    /* ���� ��� */
    for (i=x1 ; i<=x2 ; i++)
	pokeb(v_s, 160*(y-1)+2*(i-1)+1, attr);
    }

/************************************************************/
