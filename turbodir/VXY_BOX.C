#define Up         196
#define Down       196
#define Right      179
#define Left       179

#define UpRight    191
#define UpLeft     218
#define DownRight  217
#define DownLeft   192

/* -----------------------------------------------------------
	프로그램 ; 메뉴길이에 맞게 박스 그리기
	함수인자 ; 열,행,마지막열,마지막행,속성
	작 성 자 ; 1995.01. 미스테리
    --------------------------------------------------------- */

void xy_box(int scol,int srow,int ecol,int erow,int attr)
{
	register int i;
#if 0
	put_char(scol,srow,1,attr);
	put_char(ecol,srow,2,attr);
	put_char(scol,erow,3,attr);
	put_char(ecol,erow,4,attr);

		for(i=scol+1;i<ecol;i++) {
		put_char(i,srow,6,attr);
		put_char(i,erow,6,attr);
	}

		for(i=srow+1;i<erow;i++) {
		put_char(scol,i,5,attr);
		put_char(ecol,i,5,attr);
	}
#endif
	put_char(scol,srow,UpLeft,attr);
	put_char(ecol,srow,UpRight,attr);
	put_char(scol,erow,DownLeft,attr);
	put_char(ecol,erow,DownRight,attr);

	for(i=scol+1;i<ecol;i++) {
		put_char(i,srow,Up,attr);
		put_char(i,erow,Down,attr);
	}

	for(i=srow+1;i<erow;i++) {
		put_char(scol,i,Left,attr);
		put_char(ecol,i,Right,attr);
	}

}

