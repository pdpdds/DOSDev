#include <stdio.h>
#include <conio.h>
#include <dos.h>

int sy,sd,sm,ly,lm,ld,ky;
long ch,cy;
char *stri;
static int eum[4][80]={
		{ 0,0,5,0,0,4, 0,0,2,0,6, 0,0,5,0,0, 3,0,7,0,0, 6,0,0,4,0, 0,2,0,7,0, 0,5,0,0,3, 0,8,0,0,6,
			0,0,4,0,0, 3,0,7,0,0, 5,0,0,4,0, 8,0,0,6,0, 0,4,0,10,0, 0,6,0,0,5, 0,0,3,0,8, 0,0,5,0 },
		{ 0,0,29,0,0,29, 0,0,29,0,29, 0,0,30,0,0, 30,0,30,0,0, 30,0,0,30,0, 0,29,0,29,0, 0,30,0,0,30, 0,29,0,0,29,
			0,0,29,0,0, 29,0,29,0,0, 29,0,0,29,0, 29,0,0,30,0, 0,29,0,29,0, 0,29,0,0,29, 0,0,29/*30*/,0,29, 0,0,29,0 },
		{ 0,354,384,354,354,385, 354,355,384,354,383, 354,355,384,355,354, 384,354,384,354,354, 384,355,355,384,354,
			354,384,354,384,354, 355,384,355,354,384, 354,384,354,354,384, 355,354,384,355,353, 384,355,384,354,355,
			384,354,354,384,354, 384,354,355,384,355, 354,384,354,384,354, 354,385,354,355,384, 354,354,383/*384*/,355,384, /*354*/355,354/*355*/,383/*385*/,354 },
		{ 0,2635,2891,1705,2772,2997, 694,2395,1335,1175,1622, 3658,3749,1705,1461,694, 2222,2350,3213,3221,3402,
			3466,2921,1389,603,605, 2349,3371,2709,3413,2890, 2901,1365,1243,603,2135, 1323,2715,1685,1706,2794,
			2741,1206,2734,2647,1318, 3878,3477,1717,1386,2477, 1245,1198,2638,3405,3365, 3413,2900,3434,2394,2395,
			1179,2715,2635,2855,1701, 1748,2933,694,2395,1207,	1175,1611,1738,3749,1753, 1452,2742,2541,2350}};
int md[14]={0,0,31,28,31,30,31,30,31,31,30,31,30,31};

void s2s_1(void)
{
	int i;
	long k;
	k=sy-1;
	ch=k*365+(k/4)-(k/100)+(k/400);
	if ((sy%4)==0 && (sy%100)!=0)
		md[3]=29;
	else if ((sy%400)!=0)
				md[3]=28;
	else
		md[3]=29;
	for(i=1;i<=sm;i++)
		ch+=md[i];
	ch+=sd;
}

void s2s_2(void)
{
	long gg;
	int na,i,j,mm,m,kt,mk,kp,chk;

	chk=0;
/*	gg=715920; */
	gg=701303;
	ly=4254;
	na=ch-gg;
	for (i=1;i<=79;i++) {
		if (na > eum[2][i]) {
			na -= eum[2][i];
			ly++;
		}
		else	break;
	}
	mm = eum[3][i];
	m = 2048;
	kt = 0;
	for (j=1;j<=12;j++) {
		mk = mm/m;
		mm = mm - mk * m;
		m = m / 2;
		kt = kt + 29 + mk;
		if (na <= kt) {
			stri = "평달";
			kp = kt - 29 - mk;
			chk = 1;
			break;
		}
		if (j == eum[0][i])
			kt += eum[1][i];
		if (na > kt)
			continue;
		else break;
	}
	if (chk == 0) {
		kp = kt - eum[1][i];
		stri = "윤달";
	}
	lm = j;
	ld = na - kp;
}


void l2s_1(void)
{
	int mm,m,mk,i;

	ch=701303;
	ky=ly-4254;
	if (ky!=0)
		for(i=1;i<=ky;i++)
			ch=ch+eum[2][i];
	if (lm!=1) {
		m=2048;
		ky++;
		mm=eum[3][ky];
		for (i=1;i<=lm-1;i++) {
			mk=mm/m;
			mm=mm-mk*m;
			m=m/2;
			ch=ch+29+mk;
			if (i==eum[0][ky])
				ch=ch+eum[1][ky];
		}
	}
	if (lm==eum[0][ky])  {
		mk=mm/m;
		cy=ch+29+mk+ld;
	}
	else
		cy=0;
	ch+=ld;
}

void l2s_2(void)
{
	int yd,ka,kk,ss,i;
	long k,na;

	k=ch/365-1;
	na=ch-k*365;
	yd=(k/4)-(k/100)+(k/400);
	ka=na-yd;
	if (ka<0) {
		k--;
		na=ch-k*365;
		yd=(k/4)-(k/100)+(k/400);
		na=na-yd;
	}
	else
		na=ka;
	ss=k+1;
	if ((ss%4)==0 && (ss%100)!=0)
	{
		md[3]=29;
		kk=366;
	}
	else if ((ss%400)!=0)
	{
		md[3]=28;
		kk=365;
	}
	else
	{
		md[3]=29;
		k=366;
	}
	sy=ss;
	if (na==0)
	{
		na=kk;
		sy--;
	}
	for(i=1;i<=13;i++)
		if (na>md[i])
			na-=md[i];
		else    break;
	sm=i-1;
	sd=na;
}

/*
void main()
{
	 int ko,sm1,sd1,mlm,mld,sm2,sd2;
	 unsigned selec;
	 ko = 1;
	 while (ko == 1) {
	 clrscr();
	 printf("\n\n\n\n\t\t 날짜 변환 프로그램\n\n\n");
	 printf("\t\tL) 음력  ==>  양력\n");
	 printf("\t\tS) 양력  ==>  음력\n");
	 printf("\t\tQ) 끝.\n\n");
	 printf("\n\t\t어떤 작업을? (L/S/Q) ");
	 selec = getche();
	 if ((selec == 'L') || (selec == 'l')) {
		 clrscr();
		 ly = 1921;
		 while ((ly > 1920) && (ly < 2000) || (ly != 0)) {
			 printf("\n\n\t 음력 날짜를 입력.(YYYY/MM/DD) ==>  ");
			 scanf("%d/%d/%d",&ly,&lm,&ld);
			 ly+=2333;
			 if (ly==2333) break;
			 if (ly<=4332 && ly>=4254) {
				 l2s_1();
				 l2s_2();
				 mlm = lm;
				 mld = ld;
				 sm2 = 1;
				 sd2 = 1;
				 sm1 = 0;
				 sd1 = 0;
				 if (lm != eum[0][ky] || ld > eum[1][ky]) {
					 if (ld > 29) {
						 sm1 = sm;
						 sd1 = sd;
						 lm++;
						 ld = 1;
						 l2s_1();
						 l2s_2();
						 sm2 = sm;
						 sd2 = sd;
						 if (--lm == eum[0][ky] || (sm1<=sm2) && (sd1<=sd2)) {
							 sm=sm1;
							 sd=sd1;
						 }
						 if ((eum[0][ky] != 0) && ((eum[3][ky] >> 12-eum[0][ky] & 01 ) == 0) && (eum[1][ky] < mld)) {
							 sm1 = sd1 = 1;
							 sm2 = sd2 = 0;
						 }
					 }
				 }
				 if ((sm1 >= sm2) && (sd1 >= sd2) || mld >= 31) {
					 sound(200);
					 delay(350);
					 nosound();
					 printf("\n\t 음력 %d년 %d월에는 %d일이 없습니다.\n",ly-2333,mlm,mld);
				 }
				 else {
					 printf("\n\t 양력으로는  (평달) ==>  %d년  %d월  %d일\n",sy,sm,sd);
					 if ((cy != 0) && (ld <= eum[1][ky]))  {
						  ch=cy;
						  l2s_2();
						  printf("\t 양력으로는  (윤달) ==>  %d년  %d월  %d일\n",sy,sm,sd);
					 }
				 }
			 }
			 else {
				 sound(300);
				 delay(70);
				 nosound();
				 printf("\n\t%d년은 데이타가 없어 처리를 못합니다. 죄송합니다.\n",ly-2333);
				 ly = 1921+2333;
			 }
			 getch();
			 ly -=2333;
		 }
	 }
	 if (selec == 'S' || selec == 's') {
		 clrscr();
		 sy = 1921;
		 while ((sy > 1920) && (sy < 2000)) {
			 printf("\n\n\n\n\t 양력 날짜를 입력.(YYYY/MM/DD) ==>  ");
			 scanf("%d/%d/%d",&sy,&sm,&sd);
			 if (sy==0) break;
			 if (sy < 2000 && sy > 1920) {
				 s2s_1();
				 if (sd > md[sm+1]) {
					 sound(200);
					 delay(350);
					 nosound();
					 printf("\n\t 양력 %d년 %d월에는 %d일이 없습니다.\n",sy,sm,sd);
				 }
				 else {
					 s2s_2();
					 printf("\n\t 음력으로는  (%s) ==>  %d년  %d월  %d일\n",stri,ly-2333,lm,ld);
				 }
			 }
			 else {
				 sound(300);
				 delay(70);
				 nosound();
				 printf("\n\t%d년은 데이타가 없어 처리를 못합니다. 죄송합니다.\n",sy);
				 sy = 1921;
			 }
			 getch();
		 }
	 }
	 if (selec == 'Q' || selec == 'q')
		 ko = 0;
	}
	clrscr();
	sound(48);
	delay(250);
	sound(100);
	delay(300);
	nosound();
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t 이용해 주셔서 대단히 감사합니다.\n");
	printf("\n\t\t\t\t 안녕히 가십시요.\n");
	getch();
	clrscr();
}
*/


void calc_luner(int x, int y,  int syear,  int smon, int sday)
{
	sy = syear;
	sm = smon;
	sd = sday;
	if (sy < 2000 && sy > 1920) {
		s2s_1();
    if(sd > md[sm+1]) {
      return;
		}
		else {
			s2s_2();
			if(lm > 12 || lm <1 || ld <1 || ld>31) return;
			put_luner(x, y, ly-2333, lm, ld);
		}
	}
	else {
		return;
	}
}
