#include "menu.h"
#include <dos.h>
#include <stdio.h>
#include <time.h>

  struct hyunjung{
   char  nalza[11];
   char  name[9]; /* 이름 */
   char  sex[2];
   char  nai[3];   /* 생년월일 */
   char  sdal[3];
   char  sil[3];
   char  me[15];  /* 나와의 관계 */
   char  zuf[10];  /* 주민등록 번호 */
   char  zub[10];                           /* 이런구조가 하나의 명함을 이룬다*/
   char  htel1[5]; /* 집 전화번호 */        /* 레코드 구조*/
   char  htel2[5];
   char  htel3[5];
   char  ho1[5];   /* 삐삐 번호 */
   char  ho2[5];
   char  ho3[5];
   char  hand1[5];
   char  hand2[5]; /* 헨드폰 번호 */
   char  hand3[5];
   char  cdo[19];       /* 고향 */
   char  hobby[13];      /* 취미 */
   char  hzuso1[5];    /* 집주소 */
   char  hzuso2[57];
   char  job[25]; /* 직업 ,직장명 */
   char  jobname[33];
   char  busu[27];
   char  gikw[31];
   char  jobtel1[5];   /* 직장 연락처 */
   char  jobtel2[5];
   char  jobtel3[5];
   char  jobtel4[3];
   char  change[9];      /* 교환 */
   char  fax1[5];        /* 팩스 */
   char  fax2[5];
   char  fax3[5];
   char  jobzuso1[5]; /* 직장 주소 */
   char  jobzuso2[55];
  };
     struct node {
       char  nalza[11];
       char  record[4];
       char  name[9];
       char  sex[2];
     };

  char *ju_menu[]={" 명함 "," 명함보기 "," 유용한 검색 "," 도움말 ","."};
		  /* 주메뉴 요소 */

  char *bu_menu[][16]={ {"등록...","검색...","삭제...","삽입...","명함파일빽업...","종료","."},
		       {"모든명함 ","남자만","여자만","."},
		       {"성함으로...","이름만으로...","출생년도로...","생일로...",
			"나와의 관계로...","고향으로...","취미로...","거주도시로...",
			"나머지 주소로...","직업으로...","직장명으로...","직장의 도시로...",
			"나머지 직장주소로... ","부서로...","직위로...","."},
		       {"사용법 ","참고","."}
		     };  /* 부메뉴 요소 */

  void first(int);
  int  up(void);
  void screen(void);
  int input(void);
  void read(struct hyunjung *);
  void pulldown(void);
  void del(void);
  int kbd(int);
  int kbd2(void);
  int count(void);
  void search(void);
  void insert(void);
  void copy(void);
  void today(void);
  void second(int,int);
  void all(int,int);
  void backup(void);
  void ip(void);
  void ip2(void);
  void foup(void);
  void home(void);
  void ip3(void);
  void third(int);
  void menual(void);
  void logo(void);
  void chang(void);
  void boxx(void);
  void chamgo(void);

  int bytesu;    /* 레코드의 바이트수를 저장 */
  FILE *fp;
  struct hyunjung *record; /* 할당된 메모리의 선두주소를 저장 */

  int geksu;
  int huakin=0;
  int pandan1=0;
  char getsuchar[5];
/*--------------------------------------------------------------------*/
 main(){
    textattr(0x07);
    clrscr();
    normal();

    logo();
    window(20,15,61,22);
    clrscr();
    window(1,1,80,25);
    textattr(0x30);
    xy_puts(17,14,"┏━━━━━━━━━━━━━━━━━━━━━━┓");
    xy_puts(17,15,"┃                                            ┃");
    xy_puts(17,16,"┃                                            ┃");
    xy_puts(17,17,"┃                                            ┃");
    xy_puts(17,18,"┃                                            ┃");
    xy_puts(17,19,"┃                                            ┃");
    xy_puts(17,20,"┃                                            ┃");
    xy_puts(17,21,"┗━━━━━━━━━━━━━━━━━━━━━━┛");
    textattr(0x3f);
    xy_puts(20,15,"V");
    textattr(0x30);
    cprintf("isiting-");
    textattr(0x3f);
    cprintf("C");
    textattr(0x30);
    cprintf("ard ");
    textattr(0x3f);
    xy_puts(34,15,"S");
    textattr(0x30);
    cprintf("upervisor (명함관리프로그램)");
    xy_puts(29,17,"Version 1.1  (99.4)");
    xy_puts(23,20,"(저) 박종민 (안양과학대 전산과)");
    textattr(0x1e);

    xy_puts(28,23,"Press any key to continue");

    normal();
    get_key();

   pulldown();
  }



/*-------------------------------------------------------------------*/

 void pulldown(void){

     int st_ju=0;      /* 주메뉴와 부메뉴를 움직 일때 쓰이는 변수 */
     int st_bu=0;      /* 이변수 값과 메뉴에서 선택된 항목을 맞춘다*/
     char *flag="DS";   /* 주메뉴의 선택막대를 움직이게  한다 */

     bytesu=sizeof(struct hyunjung);
     normal();

     clrscr();

     today();
     set_attrib(1,80,1,0x75);
     set_attrib(1,80,25,0x75);

     while(st_ju != -1){           /* 풀다운 메뉴를 띄운다 */
      logo();
      textattr(0x70);
      xy_puts(1,25,"                                                                             ");


       st_ju=x_menu(1,1,st_ju,flag,ju_menu);

       if(st_bu==LEFT || st_bu==RIGHT || st_bu==-1)
	 st_bu=0;

	if(pandan1 != 7)
	 switch( st_ju )  /* 위의 주메뉴에서 선택한 것에 따라 부메뉴를 띄운다.*/
	   {
	    case 0 : st_bu=y_menu(5,3,st_bu,"LC",bu_menu[0],0);
		    break;
	    case 1 : st_bu=y_menu(17,3,st_bu,"LC",bu_menu[1],1);
		    break;
	    case 2 : st_bu=y_menu(29,3,st_bu,"LC",bu_menu[2],2);
		    break;
	    case 3 : st_bu=y_menu(46,3,st_bu,"LC",bu_menu[3],3);
		    break;
	  }


      if(st_bu==-1) flag="DS";
      else          flag="XX";

      if(st_ju==0 && st_bu==LEFT) st_ju=3;
      else if(st_ju!=0 && st_bu==LEFT) st_ju--;
      else if(st_ju==3 && st_bu==RIGHT) st_ju=0;
      else if(st_ju!=3 && st_bu==RIGHT) st_ju++;
      else if(st_bu!=-1)
	switch(st_ju)  /* 각 기능을 하는 함수 호출 */
	   {
	     case 0 : first(st_bu); break;
	     case 1 :
	     case 2 : second(st_ju,st_bu); break;
	     case 3 : third(st_bu); break;
	   }

    }
   normal();
   exit(0);
}

/* --------------------------------------------------------------*/

    void first(int sun){     /* 첫번째  부메뉴기능  */

      switch(sun){

	case 0 : pandan1=up();
		       break;
	case 1 : search();
		 break;
	case 2 : del();
		 break;
	case 3 : insert();
		 break;
	case 4 : copy();
		 break;
	case 5 : normal();
		 exit(0);
	}
     }
/*-------------------------------------------------------------------*/
    void second(int st,int sun){    /* 두번째 부메뉴 기능 */

	 all(st,sun);

     }
/*-------------------------------------------------------------------*/
    void third(int sun){            /* 세번째 부메뉴 기능 */

	  switch(sun){

	    case 0 : menual();  break;
	    case 1 : chamgo();
	   }
    }
/*-------------------------------------------------------------------------*/
    int  up(void){  /* 등록을 하는 함수 */

       int pan;
       int z;
       int pandan=1;
       screen();  /* 화면을 찍는다 */
       textattr(0x9f);
       xy_puts(5,25,"                   ☞   새로운 명함을 등록하세요   ☜                   ");
       normal();
       count();   /* 레코드 갯수를 샌다*/
       geksu++;
       itoa(geksu,getsuchar,10);
       textattr(0xe0);
       xy_puts(56,23,"현재 레코드번호: ");
       xy_puts(73,23,getsuchar);
       normal();
       pandan=input();   /* 화면으로부터 입력을 받는다 */
	if(pandan == -1)
	  {
	    return(0);
	 }
       record=(struct hyunjung *) calloc(bytesu,1);
       /* 메모리를 할당 */
       read(record);		  /* 사용자가 화면에 쓴 것을 읽어 들인다 */
       pan=0;

       pan=talk(pan,0); /* 대화상자를 띄운다 */

       switch(pan){  /* 사용자가 '예' 를 눌렀다면 등록 */

	case 0 :
	       if( (fp=fopen("person2.dat","ab")) == NULL)
	       fp=fopen("person2.dat","wb");
	      fseek(fp,0,SEEK_END);
	       fwrite(record,bytesu,1,fp);
	       free(record);
	       fclose(fp);
	       return(7);
       case 1 : free(record); /* 아니오를 눌렀다면 */
	       return(7);

       case -1 : free(record);   /* Esc 를 눌렀다면 */
	       return(7);

       }



    }
/*------------------------------------------------------------------*/
     void screen(){        /*    14  18   23    30          42                  62  65   */
       textattr(0xF0);
       window(79,3,80,19);
       clrscr();
       window(1,1,80,25);
       gotoxy(1,2);
       textattr(0xF0);
       cprintf("\n\r ┌───┬────┬──┬─┬────┬────┬─────┬───────┐");
       cprintf("\n\r │ 성함 │        │성별│  │생년월일│  .  .  │나와의관계│              │");
       cprintf("\n\r ├───┼────┴──┴┬┴──┬─┴────┴┬──┬─┴───────┤");
       cprintf("\n\r │ 집 ☎│(    )     -    │호출기│    -    -    │고향│                  │");
       cprintf("\n\r ├───┴┬───────┼───┴──┬────┴──┼──┬──────┤");
       cprintf("\n\r │ 헨드폰 │    -    -    │주민등록번호│      -       │취미│            │");
       cprintf("\n\r ├────┼───────┴──────┴───────┴──┴──────┤");
       cprintf("\n\r │ 집주소 │     시                                                         │");
       cprintf("\n\r ├───┬┴───────────┬───┬────────────────┤");
       cprintf("\n\r │ 직업 │                        │직장명│                                │");
       cprintf("\n\r ├───┼────────────┴┬──┴┬───────────────┤");
       cprintf("\n\r │ 부서 │                          │ 직위 │                              │");
       cprintf("\n\r ├───┴┬───────────┬┴─┬─┴──┬──┬─────────┤");
       cprintf("\n\r │ 직장 ☎│(    )     -     ∼   │교환│        │FAX │(    )     -      │");
       cprintf("\n\r ├────┴┬──────────┴──┴────┴──┴─────────┤");
       cprintf("\n\r │ 직장주소 │     시                                                       │");
       cprintf("\n\r └─────┴───────────────────────────────┘");

       textattr(0xb0);
       xy_puts(1,20," ┌─────────────────────────────────────┐ ");
       xy_puts(1,21," │                                                                          │ ");
       xy_puts(1,22," │                                                                          │ ");
       xy_puts(1,23," │                                                                          │ ");
       xy_puts(1,24," └─────────────────────────────────────┘ ");

       xy_puts(4,21," ☞ 모든 입력은 커서가 처음 위치한 곳부터");
       xy_puts(50,21,"←→ 커서이동");
       xy_puts(67,21,"Esc 취소");
       xy_puts(4,22," ☞ ↓ 다음필드로(← )"); gotoxy(24,22); putch(4);
       xy_puts(30,22,"↑ 이전필드로");
  xy_puts(48,22,"F10 등록(직장주소필드에서 ← )"); gotoxy(76,22); putch(4);
       xy_puts(4,23," ☞ BackSpace 지우기");
       xy_puts(28,23,"성별입력시 남->1,여->2");

       normal();

}
 /*----------------------------------------------------------------*/

     int input(void){  /* 키보드로 입력 */

	int tab=0; /*커서 필드이동을 위해 이용 */
	int key;
	int munjasu=0;

	textattr(0xF0);
	gotoxy(12,4);

	while(1){

	  key=get_key();

	  switch(key){

	       case RIGHT :
			if(tab==0)
			if(munjasu<8)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==1)
			if(munjasu<1)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==2)
			 if(munjasu<2)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==3)
			 if(munjasu<2)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==4)
			 if(munjasu<2)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==5)
			 if(munjasu<14)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==6)
			 if(munjasu<4)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==7)
			 if(munjasu<4)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==8)
			 if(munjasu<4)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==9)
			 if(munjasu<4)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==10)
			 if(munjasu<4)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==11)
			 if(munjasu<4)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==12)
			 if(munjasu<18)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==13)
			 if(munjasu<4)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==14)
			 if(munjasu<4)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==15)
			 if(munjasu<4)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==16)
			 if(munjasu<6)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==17)
			 if(munjasu<7)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==18)
			 if(munjasu<12)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==19)
			 if(munjasu<4)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==20)
			 if(munjasu<56)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==21)
			 if(munjasu<24)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==22)
			 if(munjasu<32)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==23)
			 if(munjasu<26)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==24)
			 if(munjasu<30)
			  gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==25)
			 if(munjasu<4)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==26)
			 if(munjasu<4)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==27)
			 if(munjasu<4)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==28)
			 if(munjasu<2)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==29)
			 if(munjasu<8)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==30)
			 if(munjasu<4)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==31)
			 if(munjasu<4)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==32)
			 if(munjasu<4)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==33)
			 if(munjasu<4)
			 gotoxy(wherex()+1,wherey()),munjasu++;
			if(tab==34)
			 if(munjasu<54)
			  gotoxy(wherex()+1,wherey()),munjasu++;
			break;

	   case LEFT :  if(munjasu>0)
			 gotoxy(wherex()-1,wherey()),munjasu--;
			break;
	   case   8  :  if(munjasu>0){
			gotoxy(wherex()-1,wherey()),munjasu--;
			putch(' ');
			gotoxy(wherex()-1,wherey());
		       }

			break;

     } /* switch */
       if(key==ESC) {

	    normal();
	    return(-1);
	  }


	if(key==DOWN){
	  if(tab==0)
	   tab++,munjasu=0,gotoxy(28,4);    /* 첫번째 필드에         */
	   else if(tab==1)                  /* 커서가 있 다면        */
	   tab++,munjasu=0,gotoxy(42,4);    /* tab 값이 0          */
	   else if(tab==2)                  /* 두번째는 1            */
	   tab++,munjasu=0,gotoxy(45,4);    /* 이런식으로 계속       */
	   else if(tab==3)                  /* 해서 다운키로    */
	   tab++,munjasu=0,gotoxy(48,4);    /* 커서를 다음 필드로 이동 */
	   else if(tab==4)                  /* 즉 커서의 위치에 따라  */
	   tab++,munjasu=0,gotoxy(64,4);    /* tab 변수의 값이 틀리다 */
	   else if(tab==5)                  /* tab  변수의 값에 따라  */
	   tab++,munjasu=0,gotoxy(13,6);    /* 커서의 위치를 판단 */
	   else if(tab==6)
	   tab++,munjasu=0,gotoxy(19,6);
	   else if(tab==7)
	   tab++,munjasu=0,gotoxy(24,6);
	   else if(tab==8)
	   tab++,munjasu=0,gotoxy(38,6);
	   else if(tab==9)
	   tab++,munjasu=0,gotoxy(43,6);
	   else if(tab==10)
	   tab++,munjasu=0,gotoxy(48,6);
	   else if(tab==11)
	   tab++,munjasu=0,gotoxy(60,6);
	   else if(tab==12)
	   tab++,munjasu=0,gotoxy(14,8);
	   else if(tab==13)
	   tab++,munjasu=0,gotoxy(19,8);
	   else if(tab==14)
	   tab++,munjasu=0,gotoxy(24,8);
	   else if(tab==15)
	   tab++,munjasu=0,gotoxy(44,8);
	   else if(tab==16)
	   tab++,munjasu=0,gotoxy(51,8);
	   else if(tab==17)
	   tab++,munjasu=0,gotoxy(66,8);
	   else if(tab==18)
	   tab++,munjasu=0,gotoxy(14,10);
	   else if(tab==19)
	   tab++,munjasu=0,gotoxy(22,10);
	   else if(tab==20)
	   tab++,munjasu=0,gotoxy(12,12);
	   else if(tab==21)
	   tab++,munjasu=0,gotoxy(46,12);
	   else if(tab==22)
	   tab++,munjasu=0,gotoxy(12,14);
	   else if(tab==23)
	   tab++,munjasu=0,gotoxy(48,14);
	   else if(tab==24)
	   tab++,munjasu=0,gotoxy(15,16);
	   else if(tab==25)
	   tab++,munjasu=0,gotoxy(21,16);
	   else if(tab==26)
	   tab++,munjasu=0,gotoxy(26,16);
	   else if(tab==27)
	   tab++,munjasu=0,gotoxy(34,16);
	   else if(tab==28)
	   tab++,munjasu=0,gotoxy(44,16);
	   else if(tab==29)
	   tab++,munjasu=0,gotoxy(61,16);
	   else if(tab==30)
	   tab++,munjasu=0,gotoxy(67,16);
	   else if(tab==31)
	   tab++,munjasu=0,gotoxy(72,16);
	   else if(tab==32)
	   tab++,munjasu=0,gotoxy(16,18);
	   else if(tab==33)
	   tab++,munjasu=0,gotoxy(24,18);
	   else if(tab==34)
	   tab=0,munjasu=0,gotoxy(12,4);
	}

	if(key==UP){
	  if(tab==0)
	   tab=34,munjasu=0,gotoxy(24,18);    /* 첫번째 필드에         */
	   else if(tab==1)                  /* 커서가 있 다면        */
	   tab--,munjasu=0,gotoxy(12,4);    /* tab 값이 0          */
	   else if(tab==2)                  /* 두번째는 1            */
	   tab--,munjasu=0,gotoxy(28,4);    /* 이런식으로 계속       */
	   else if(tab==3)                  /* 해서 스패이스 키로    */
	   tab--,munjasu=0,gotoxy(42,4);    /* 커서를 다음 필드로 이동 */
	   else if(tab==4)                  /* 즉 커서의 위치에 따라  */
	   tab--,munjasu=0,gotoxy(45,4);    /* tab 변수의 값이 틀리다 */
	   else if(tab==5)                  /* tab  변수의 값에 따라  */
	   tab--,munjasu=0,gotoxy(48,4);    /* 커서의 위치를 판단 */
	   else if(tab==6)
	   tab--,munjasu=0,gotoxy(64,4);
	   else if(tab==7)
	   tab--,munjasu=0,gotoxy(13,6);
	   else if(tab==8)
	   tab--,munjasu=0,gotoxy(19,6);
	   else if(tab==9)
	   tab--,munjasu=0,gotoxy(24,6);
	   else if(tab==10)
	   tab--,munjasu=0,gotoxy(38,6);
	   else if(tab==11)
	   tab--,munjasu=0,gotoxy(43,6);
	   else if(tab==12)
	   tab--,munjasu=0,gotoxy(48,6);
	   else if(tab==13)
	   tab--,munjasu=0,gotoxy(60,6);
	   else if(tab==14)
	   tab--,munjasu=0,gotoxy(14,8);
	   else if(tab==15)
	   tab--,munjasu=0,gotoxy(19,8);
	   else if(tab==16)
	   tab--,munjasu=0,gotoxy(24,8);
	   else if(tab==17)
	   tab--,munjasu=0,gotoxy(44,8);
	   else if(tab==18)
	   tab--,munjasu=0,gotoxy(51,8);
	   else if(tab==19)
	   tab--,munjasu=0,gotoxy(66,8);
	   else if(tab==20)
	   tab--,munjasu=0,gotoxy(14,10);
	   else if(tab==21)
	   tab--,munjasu=0,gotoxy(22,10);
	   else if(tab==22)
	   tab--,munjasu=0,gotoxy(12,12);
	   else if(tab==23)
	   tab--,munjasu=0,gotoxy(46,12);
	   else if(tab==24)
	   tab--,munjasu=0,gotoxy(12,14);
	   else if(tab==25)
	   tab--,munjasu=0,gotoxy(48,14);
	   else if(tab==26)
	   tab--,munjasu=0,gotoxy(15,16);
	   else if(tab==27)
	   tab--,munjasu=0,gotoxy(21,16);
	   else if(tab==28)
	   tab--,munjasu=0,gotoxy(26,16);
	   else if(tab==29)
	   tab--,munjasu=0,gotoxy(34,16);
	   else if(tab==30)
	   tab--,munjasu=0,gotoxy(44,16);
	   else if(tab==31)
	   tab--,munjasu=0,gotoxy(61,16);
	   else if(tab==32)
	   tab--,munjasu=0,gotoxy(67,16);
	   else if(tab==33)
	   tab--,munjasu=0,gotoxy(72,16);
	   else if(tab==34)
	   tab--,munjasu=0,gotoxy(16,18);
	}


	if(key==F10)
	  {  normal(); return; }

	if(key==CR) {

	   if(tab==34)
	      { normal(); return;   }
	   if(tab==0)
	   tab++,munjasu=0,gotoxy(28,4);    /* 첫번째 필드에         */
	   else if(tab==1)                  /* 커서가 있 다면        */
	   tab++,munjasu=0,gotoxy(42,4);    /* tab 값이 0          */
	   else if(tab==2)                  /* 두번째는 1            */
	   tab++,munjasu=0,gotoxy(45,4);    /* 이런식으로 계속       */
	   else if(tab==3)                  /* 해서 다운키로    */
	   tab++,munjasu=0,gotoxy(48,4);    /* 커서를 다음 필드로 이동 */
	   else if(tab==4)                  /* 즉 커서의 위치에 따라  */
	   tab++,munjasu=0,gotoxy(64,4);    /* tab 변수의 값이 틀리다 */
	   else if(tab==5)                  /* tab  변수의 값에 따라  */
	   tab++,munjasu=0,gotoxy(13,6);    /* 커서의 위치를 판단 */
	   else if(tab==6)
	   tab++,munjasu=0,gotoxy(19,6);
	   else if(tab==7)
	   tab++,munjasu=0,gotoxy(24,6);
	   else if(tab==8)
	   tab++,munjasu=0,gotoxy(38,6);
	   else if(tab==9)
	   tab++,munjasu=0,gotoxy(43,6);
	   else if(tab==10)
	   tab++,munjasu=0,gotoxy(48,6);
	   else if(tab==11)
	   tab++,munjasu=0,gotoxy(60,6);
	   else if(tab==12)
	   tab++,munjasu=0,gotoxy(14,8);
	   else if(tab==13)
	   tab++,munjasu=0,gotoxy(19,8);
	   else if(tab==14)
	   tab++,munjasu=0,gotoxy(24,8);
	   else if(tab==15)
	   tab++,munjasu=0,gotoxy(44,8);
	   else if(tab==16)
	   tab++,munjasu=0,gotoxy(51,8);
	   else if(tab==17)
	   tab++,munjasu=0,gotoxy(66,8);
	   else if(tab==18)
	   tab++,munjasu=0,gotoxy(14,10);
	   else if(tab==19)
	   tab++,munjasu=0,gotoxy(22,10);
	   else if(tab==20)
	   tab++,munjasu=0,gotoxy(12,12);
	   else if(tab==21)
	   tab++,munjasu=0,gotoxy(46,12);
	   else if(tab==22)
	   tab++,munjasu=0,gotoxy(12,14);
	   else if(tab==23)
	   tab++,munjasu=0,gotoxy(48,14);
	   else if(tab==24)
	   tab++,munjasu=0,gotoxy(15,16);
	   else if(tab==25)
	   tab++,munjasu=0,gotoxy(21,16);
	   else if(tab==26)
	   tab++,munjasu=0,gotoxy(26,16);
	   else if(tab==27)
	   tab++,munjasu=0,gotoxy(34,16);
	   else if(tab==28)
	   tab++,munjasu=0,gotoxy(44,16);
	   else if(tab==29)
	   tab++,munjasu=0,gotoxy(61,16);
	   else if(tab==30)
	   tab++,munjasu=0,gotoxy(67,16);
	   else if(tab==31)
	   tab++,munjasu=0,gotoxy(72,16);
	   else if(tab==32)
	   tab++,munjasu=0,gotoxy(16,18);
	   else if(tab==33)
	   tab++,munjasu=0,gotoxy(24,18);
	 }


 if(key!=CR && key!=LEFT && key!=RIGHT && key != DOWN && key != UP && key != 8){
			if(tab==0)
			if(munjasu<8)
			 munjasu++,putch(key);
			if(tab==1)
			if(munjasu<1)
			 munjasu++,putch(key);
			if(tab==2)
			 if(munjasu<2)
			 munjasu++,putch(key);
			if(tab==3)
			 if(munjasu<2)
			 munjasu++,putch(key);
			if(tab==4)
			 if(munjasu<2)
			 munjasu++,putch(key);
			if(tab==5)
			 if(munjasu<14)
			 munjasu++,putch(key);
			if(tab==6)
			 if(munjasu<4)
			 munjasu++,putch(key);
			if(tab==7)
			 if(munjasu<4)
			 munjasu++,putch(key);
			if(tab==8)
			 if(munjasu<4)
			 munjasu++,putch(key);
			if(tab==9)
			 if(munjasu<4)
			 munjasu++,putch(key);
			if(tab==10)
			 if(munjasu<4)
			 munjasu++,putch(key);
			if(tab==11)
			 if(munjasu<4)
			 munjasu++,putch(key);
			if(tab==12)
			 if(munjasu<18)
			 munjasu++,putch(key);
			if(tab==13)
			 if(munjasu<4)
			 munjasu++,putch(key);
			if(tab==14)
			 if(munjasu<4)
			 munjasu++,putch(key);
			if(tab==15)
			 if(munjasu<4)
			 munjasu++,putch(key);
			if(tab==16)
			 if(munjasu<6)
			 munjasu++,putch(key);
			if(tab==17)
			 if(munjasu<7)
			 munjasu++,putch(key);
			if(tab==18)
			 if(munjasu<12)
			 munjasu++,putch(key);
			if(tab==19)
			 if(munjasu<4)
			 munjasu++,putch(key);
			if(tab==20)
			 if(munjasu<56)
			 munjasu++,putch(key);
			if(tab==21)
			 if(munjasu<24)
			 munjasu++,putch(key);
			if(tab==22)
			 if(munjasu<32)
			 munjasu++,putch(key);
			if(tab==23)
			 if(munjasu<26)
			 munjasu++,putch(key);
			if(tab==24)
			 if(munjasu<30)
			 munjasu++,putch(key);
			if(tab==25)
			 if(munjasu<4)
			 munjasu++,putch(key);
			if(tab==26)
			 if(munjasu<4)
			 munjasu++,putch(key);
			if(tab==27)
			 if(munjasu<4)
			 munjasu++,putch(key);
			if(tab==28)
			 if(munjasu<2)
			 munjasu++,putch(key);
			if(tab==29)
			 if(munjasu<8)
			 munjasu++,putch(key);
			if(tab==30)
			 if(munjasu<4)
			 munjasu++,putch(key);
			if(tab==31)
			 if(munjasu<4)
			 munjasu++,putch(key);
			if(tab==32)
			 if(munjasu<4)
			 munjasu++,putch(key);
			if(tab==33)
			 if(munjasu<4)
			 munjasu++,putch(key);
			if(tab==34)
			 if(munjasu<54)
			 munjasu++,putch(key);

	}

     } /* while */

  }
/*--------------------------------------------------------------*/

 int talk(int sun,int jonglu){
    char *yosu[]={"    예    ",
		  "  아니오  "};
       normal();
       window(26,11,53,16);
       clrscr();
       textattr(0x40);
       window(24,10,50,15);
       clrscr();
       window(1,1,80,25);
       single_box(24,10,50,15);
       textattr(0x1f);
       xy_puts(28,13,yosu[0]);
       textattr(0x40);
       xy_puts(38,13,yosu[1]);
	if(jonglu==0)
	  xy_puts(29,11,"등록하시겠습니까?");
	 else if(jonglu==1)
	  xy_puts(28,11,"정말 지우시겠습니까?");
	  else if(jonglu==2)
	   xy_puts(29,11,"저장하시겠습니까?");
	   else if(jonglu==3)
	    xy_puts(29,11,"삽입하시겠습니까?");
       while(1){
	switch(get_key()){

	  case  LEFT : textattr(0x40); xy_puts(28+sun*10,13,yosu[sun]);
		       if(sun==0) sun=1; else sun--;
		       textattr(0x1f); xy_puts(28+sun*10,13,yosu[sun]);
		       break;
	  case RIGHT : textattr(0x40); xy_puts(28+sun*10,13,yosu[sun]);
		       if(sun==1) sun=0; else sun++;
		       textattr(0x1f); xy_puts(28+sun*10,13,yosu[sun]);
		       break;
	  case ESC   : return(-1);
	  case CR    : return(sun);
	  default    : break;
	       }
	  }

      }
 /*------------------------------------------------------------------*/

 void read(struct hyunjung *a){

      xy_vgets(62,1,10,a->nalza);
      xy_vgets(12,4,8,a->name);
      xy_vgets(28,4,1,a->sex);
      xy_vgets(42,4,2,a->nai);
      xy_vgets(45,4,2,a->sdal);
      xy_vgets(48,4,2,a->sil);
      xy_vgets(64,4,14,a->me);
      xy_vgets(13,6,4,a->htel1);
      xy_vgets(19,6,4,a->htel2);
      xy_vgets(24,6,4,a->htel3);
      xy_vgets(38,6,4,a->ho1);
      xy_vgets(43,6,4,a->ho2);
      xy_vgets(48,6,4,a->ho3);
      xy_vgets(60,6,18,a->cdo);
      xy_vgets(14,8,4,a->hand1);
      xy_vgets(19,8,4,a->hand2);
      xy_vgets(24,8,4,a->hand3);
      xy_vgets(44,8,6,a->zuf);
      xy_vgets(51,8,7,a->zub);
      xy_vgets(66,8,12,a->hobby);
      xy_vgets(14,10,4,a->hzuso1);
      xy_vgets(22,10,56,a->hzuso2);
      xy_vgets(12,12,24,a->job);
      xy_vgets(46,12,32,a->jobname);
      xy_vgets(12,14,26,a->busu);
      xy_vgets(48,14,30,a->gikw);
      xy_vgets(15,16,4,a->jobtel1);
      xy_vgets(21,16,4,a->jobtel2);
      xy_vgets(26,16,4,a->jobtel3);
      xy_vgets(34,16,2,a->jobtel4);
      xy_vgets(44,16,8,a->change);
      xy_vgets(61,16,4,a->fax1);
      xy_vgets(67,16,4,a->fax2);
      xy_vgets(72,16,4,a->fax3);
      xy_vgets(16,18,4,a->jobzuso1);
      xy_vgets(24,18,54,a->jobzuso2);
   }

/* --------------------------------------------------------------*/

  void del(void){   /* 레코드를 지우는 함수 */

    FILE *fp2;     /* 임시 파일을 지정 */
    int rec=1;     /* 레코드 번호를 지정 */
    int panda;
    int bunho;
    int bunho2;
    int l;
    char gums[5];

       chang();

     count();    /* 레코드 갯수를 샌다 */
     if(geksu==0)    /* 갯수가 0 이라면 , 즉 명함파일이 없다면 */
     {
       textattr(0x40);
       xy_puts(29,6,"명함이 없습니다");
       normal();
       getch();
       return;
     }


     textattr(0x5e);
     xy_puts(29,6,"삭제할 레코드번호는?");
     normal();

     panda=kbd(11); /* 키보드로 입력 */

     if(panda==-1) return;   /* 키보드로 입력하다가 Esc 를 눌렀다면 */
     set_attrib(30,37,9,0x1f);
     l=0;
     l=talk(l,1);

      switch(l){
	case 0:
		xy_vgets(30,9,4,gums);  /* 쓴 것을 gums에 기억 */
		bunho=atoi(gums);       /* gums에 기억 된 것을 정수형으로 변환 */

		 fp=fopen("person2.dat","rb");  /* 임시파일을 만들어서 삭제 레코드만*/
		 fp2=fopen("temp.dat","wb");    /* 빼고 모든 레코드를 다 쓴다 */
		 fseek(fp,0,SEEK_SET);          /* 원래 화일을 지우고 임시파일을  */
				   /* 명함록파일로 한다 */
	   while(1){
	    record=(struct hyunjung *) calloc(bytesu,1);
	    fread(record,bytesu,1,fp);
	     if(feof(fp))
	      { free(record); break; }
		if(bunho!=rec)
	      fwrite(record,bytesu,1,fp2);
	      rec++;
	     free(record);
	     }
	    fcloseall();
	    unlink("person2.dat");
	     rename("temp.dat","person2.dat");

    case 1 : return;
    case 2 : return;
  }
}
/*--------------------------------------------------------------*/
   int kbd(int dir){

      int key2;
      int munsu=0;
      int b,c;

      if(dir==11)
	b=34,c=4;
      if(dir==12)
	b=38,c=8;
      if(dir==13)
	b=36,c=6;
      if(dir==14)
	b=33,c=2;
      if(dir==15 || dir==16 || dir==17)
	b=32,c=2;
      if(dir==18)
	b=34,c=4;
      if(dir==19)
	b=55,c=24;
      if(dir==20)
	b=57,c=28;
      if(dir==22)
	b=56,c=26;
      if(dir==23)
	b=57,c=28;
      if(dir==24)
	b=44,c=14;
      if(dir==25)
	b=48,c=18;
      if(dir==26)
	b=42,c=12;
      set_attrib(29,b,9,0x1f);
      textattr(0x1f);

      if(dir==16)
	xy_puts(32,9,"/");
      if(dir==17)
	gotoxy(33,9);
       else
	gotoxy(30,9);

      while(1){
	key2=get_key();
	 switch(key2){
	    case RIGHT : if(munsu<c)
			  { gotoxy(wherex()+1,9); munsu++;}
			   break;
	    case LEFT  : if(munsu>0) gotoxy(wherex()-1,9),munsu--;
			 break;
	    case CR    : normal();
			 return;

	    case ESC   : normal();
			 return(-1);
	    case   8   :  if(munsu>0){
			gotoxy(wherex()-1,wherey()),munsu--;
			putch(' ');
			gotoxy(wherex()-1,wherey());
		       }

			break;

	    default    :   if(munsu<c)
			   putch(key2),munsu++;
			     break;
	    }
	  }
    }
/*------------------------------------------------------------*/

   int count(void){

		geksu=0;

		if((fp=fopen("person2.dat","rb")) == NULL)
		  return -1;
		fseek(fp,0,SEEK_SET);
		while(1){
		  record=(struct hyunjung *) calloc(bytesu,1);
		  fread(record,bytesu,1,fp);
		if(feof(fp))
		 { free(record); break;   }
		 geksu++;
		 free(record);
		}
	       fclose(fp);

     }
/*--------------------------------------------------------------*/

   void search(void){

    char temp[10];
    struct hyunjung *jong;
    int pan=0;
    int reco;
    int sunnn;
    int pandan=1;

    chang();

    count();
     if(geksu==0)
     {
     textattr(0x40);
     xy_puts(29,6,"명함이 없습니다");
     normal();
     getch(); return;
     }

    textattr(0x5e);
    xy_puts(29,6,"찾고 싶은 이름은 ? ");
    normal();
    gotoxy(30,9);
    pan=kbd(12);
    if(pan==-1) return;
    xy_vgets(30,9,8,temp);
    fp=fopen("person2.dat","rb+");
    reco=1;

    while(1){
     record=(struct hyunjung *) calloc(bytesu,1);
     fread(record,bytesu,1,fp);
     if(feof(fp))
      {	free(record); break; }
     if(strncmp(temp,record->name,8)==0) {

	 screen();
	 textattr(0x9f);
	 xy_puts(5,25,"                 ☞   바뀐 내용이 있으면 고치세요   ☜                  ");
	 textattr(0xf0);
      xy_puts(12,4,record->name);
      xy_puts(28,4,record->sex);
      xy_puts(42,4,record->nai);
      xy_puts(45,4,record->sdal);
      xy_puts(48,4,record->sil);
      xy_puts(64,4,record->me);
      xy_puts(13,6,record->htel1);
      xy_puts(19,6,record->htel2);
      xy_puts(24,6,record->htel3);
      xy_puts(38,6,record->ho1);
      xy_puts(43,6,record->ho2);
      xy_puts(48,6,record->ho3);
      xy_puts(60,6,record->cdo);
      xy_puts(14,8,record->hand1);
      xy_puts(19,8,record->hand2);
      xy_puts(24,8,record->hand3);
      xy_puts(44,8,record->zuf);
      xy_puts(51,8,record->zub);
      xy_puts(66,8,record->hobby);
      xy_puts(14,10,record->hzuso1);
      xy_puts(22,10,record->hzuso2);
      xy_puts(12,12,record->job);
      xy_puts(46,12,record->jobname);
      xy_puts(12,14,record->busu);
      xy_puts(48,14,record->gikw);
      xy_puts(15,16,record->jobtel1);
      xy_puts(21,16,record->jobtel2);
      xy_puts(26,16,record->jobtel3);
      xy_puts(34,16,record->jobtel4);
      xy_puts(44,16,record->change);
      xy_puts(61,16,record->fax1);
      xy_puts(67,16,record->fax2);
      xy_puts(72,16,record->fax3);
      xy_puts(16,18,record->jobzuso1);
      xy_puts(24,18,record->jobzuso2);

	 free(record);
	 pandan=input();
	   if(pandan == -1)
	     { fclose(fp); return;  }

	 record=(struct hyunjung *) calloc(bytesu,1);
	 read(record);
	 sunnn=0;
	 sunnn=talk(sunnn,2);
	switch(sunnn){
	 case 0 :  fseek(fp,bytesu*(reco-1),SEEK_SET);
		   fwrite(record,bytesu,1,fp);
		   textcolor(11);
		   textcolor(15);
		   getch();
		   free(record);
		   fclose(fp);
		   normal();
		   return;
	 case 1 :  fclose(fp);
		   free(record);
		   normal();
		   return;
	 case -1 : fclose(fp);
		   free(record);
		   normal();
		   return;
	 }
       } /* if */
	 free(record);
	 reco++;
    }/* while */
    fclose(fp);
    normal();
  }
/*-----------------------------------------------------------------*/
    void chang(void){

       window(28,6,53,12);
	clrscr();
	 window(1,1,80,25);
	 textattr(0x7e);
	   xy_puts(26,5,"┏━━━━━━━━━━━┓");
	   xy_puts(26,6,"┃                      ┃");
	   xy_puts(26,7,"┃ ────────── ┃");
	   xy_puts(26,8,"┃                      ┃");
	   xy_puts(26,9,"┃                      ┃");
	  xy_puts(26,10,"┃                      ┃");
	  xy_puts(26,11,"┗━━━━━━━━━━━┛");
	  normal();
     }
/*-----------------------------------------------------------------*/

  void insert(void){

    int n,i;
    int pandan=1;
    struct hyunjung *lee;
    FILE *fp2;
    int pand=0;
    char tem[5];

      chang();

     count();
     if(geksu==0)
     { textattr(0x40);
       xy_puts(29,6,"명함이 없습니다");
       normal();
       getch();
       return;
     }

     textattr(0x5e);
     xy_puts(29,6,"몇번 다음에 삽입 ? ");
     normal();
     gotoxy(30,9);
     pand=kbd(11);

     if(pand==-1)
      {
	return;
      }
     xy_vgets(30,9,4,tem);
     n=atoi(tem);
     if(n==0)
      {
       return;
     }
     screen();
	 textattr(0x9f);
	 xy_puts(5,25,"                   ☞   삽입할 명함을 입력하세요   ☜                   ");
     pandan=input();
      if(pandan == -1)
	return;

     record=(struct hyunjung *) calloc(bytesu,1);
     read(record);

     textcolor(14);
     i=0;
     i=talk(i,3);
       switch(i){
	 case 0 :

		 fp=fopen("person2.dat","rb");
		 fp2=fopen("temp.dat","wb");
		 fseek(fp,0,SEEK_SET);
	      for(i=0;i<n;i++)
	      {
		lee=(struct hyunjung *) calloc(bytesu,1);
		fread(lee,bytesu,1,fp);
		if(feof(fp))
		 {  free(lee);break; }
		fwrite(lee,bytesu,1,fp2);
		free(lee);
	       }
		fwrite(record,bytesu,1,fp2);
		free(record);
		while(1){
		lee=(struct hyunjung *) calloc(bytesu,1);
		fread(lee,bytesu,1,fp);
		if(feof(fp))
		  { free(lee); break; }
		fwrite(lee,bytesu,1,fp2);
		free(lee);
	      }
		fcloseall();  unlink("person2.dat");
		rename("temp.dat","person2.dat");
		textcolor(11);
		return;
       case 1 :  free(record);
		 return;
       case -1 : free(record);
		 return;
      }
}

/*-------------------------------------------------------------------*/

  void copy(void){

    FILE *fp3;
    struct hyunjung *han;
    int key;
       window(24,6,61,22);
       clrscr();
       textattr(0x70);
       window(22,5,58,21);
       clrscr();
       window(1,1,80,25);
       single_box(22,5,58,21);
       textattr(0x7f);
       xy_puts(25,8,"────────────────");
       textattr(0x70);
       xy_puts(25,10,"────────────────");
	   textattr(0xcf);
	   xy_puts(35,6,"¡ 주 의 ¡"); textattr(0x70);
	   xy_puts(25,7,"A드라이브에 디스켓이 있는지 확인");
	   textattr(0xcf);
	   xy_puts(31,9," ☞  Esc : 취소  ☜ ");
	   textattr(0x70);

	   xy_puts(35,11,"※ 역 할");
	   xy_puts(24,13,"⇒ A드라이브의 디스켓에 명함파일을");
	   xy_puts(27,14,"빽업");
	   xy_puts(24,16,"⇒ 디스켓에 'person2.dat'가 있다면");
	   xy_puts(27,17,"새로운 화일로 갱신");
	   xy_puts(25,19,"》 Press any key to continue! 《");
	 normal();
	 key=get_key();

	    switch(key){

	       case ESC :  return;
	       default : break;
	    }

	   if((fp3=fopen("a:\person2.dat","rb"))==NULL)
	      fp3=fopen("a:\person2.dat","wb");
	       else {
		 unlink("a:\person2.dat");
		 fp3=fopen("a:\person2.dat","wb");
	       }
		fp=fopen("person2.dat","rb");
		fseek(fp,0,SEEK_SET);
	     while(1){
	       han=(struct hyunjung *) calloc(bytesu,1);
		fread(han,bytesu,1,fp);

		  if(feof(fp))
		     { free(han); break; }

		    fwrite(han,bytesu,1,fp3);
		     free(han);
		 }
		fclose(fp);
		fclose(fp3);
	      }

/*---------------------------------------------------------------------*/
  void today(void){

   struct dosdate_t d;
   char *yo[7]={"일","월","화","수","목","금","토"};
   _dos_getdate(&d);
   gotoxy(62,1);
   cprintf("%d.%d.%d  (%s)", d.year, d.month, d.day,yo[d.dayofweek]);
 }



/*--------------------------------------------------------------------*/
      void all(int st,int sun){

	 int w,v,fo,back,i,j,ban,kiu,recor;
	 char buf[3700];
	 char rec4[5];
	 char temp[29];
	 char temp2[7];
	 char temp3;
	 struct hyunjung *jung;
	 char ki[5];
	 struct node *lee;
	 struct node *memori;
	 FILE *fp2;
	 memori=(struct node *) calloc(sizeof(struct node),1600);
	 gotoxy(1,6);


	 fp=fopen("person2.dat","rb");
	 fseek(fp,0,SEEK_SET);
	   j=0;
	   recor=1;

	if(st==1 && sun==0){  /* 전체레코드를 선택했다면 */

		while(1){

		 jung=(struct hyunjung *) calloc(sizeof(struct hyunjung),1);
		  fread(jung,bytesu,1,fp);
		  if(feof(fp)) break;
		  itoa(recor,getsuchar,10);
		  recor++;
		  strcpy(memori[j].record,getsuchar);
		  strcpy(memori[j].nalza,jung->nalza);
		  strcpy(memori[j].name,jung->name);
		  strcpy(memori[j].sex,jung->sex);
		  j++;
		  free(jung);
		 } /* 배열에 모든 레코드를 기억 */
		 fclose(fp);
		 free(jung);
	  }  /* 전체레코드를 선택 */
	else if(st==1 && sun==1){


	     while(1){

	      jung=(struct hyunjung *) calloc(sizeof(struct hyunjung),1);
	      fread(jung,bytesu,1,fp);

	       if(feof(fp)) break;
		if(strncmp(jung->sex,"1",1)==0){
		  itoa(recor,getsuchar,10);
		  strcpy(memori[j].record,getsuchar);
		  strcpy(memori[j].nalza,jung->nalza);
		  strcpy(memori[j].name,jung->name);
		  strcpy(memori[j].sex,jung->sex);
		  j++;
	       }
		  free(jung);
		  recor++;
	      }
		 fclose(fp);
		 free(jung);
	  }

	else if(st==1 && sun==2){

	    while(1){

	      jung=(struct hyunjung *) calloc(sizeof(struct hyunjung),1);
	      fread(jung,bytesu,1,fp);
	       if(feof(fp)) break;
		if(strncmp(jung->sex,"2",1)==0){
		  itoa(recor,getsuchar,10);
		  strcpy(memori[j].record,getsuchar);
		  strcpy(memori[j].nalza,jung->nalza);
		  strcpy(memori[j].name,jung->name);
		  strcpy(memori[j].sex,jung->sex);
		  j++;
		 }
		  free(jung);
		  recor++;
	      }
		 fclose(fp);
		 free(jung);
	  }

	  else if(st==2 && sun==0){
	      boxx();
	      xy_puts(29,7,"☞  찾고 싶은 성함은 ?");
	      xy_puts(24,12,"예) 성이 '홍'인 사람을 검색한다면,");
	      xy_puts(28,13,"'홍'만 입력하면 '홍'씨 성을");
	      xy_puts(28,14,"가진 사람이 모두 검색됩니다.");

	      xy_puts(24,16,"예) 성함이 '홍길*'->'홍길'만");
	      xy_puts(28,17,"입력하세요.");
	      xy_puts(26,19,"즉, 앞에서부터 일부만 입력해도");
	      xy_puts(26,20,"된다는 예기죠.");
	     normal();

	       v=kbd(12);
		 if(v==-1)
		 { fclose(fp);
		   free(memori);
		   return;
		}
		xy_vgets(30,9,8,temp);

		for(i=0;i<8;i++)
		 if( (strncmp(&temp[i]," ",1)) == 0 )
		   {
		     temp[i]='\0';
		      break;
		     }
	    recor=1;
	    while(1){

	      jung=(struct hyunjung *) calloc(sizeof(struct hyunjung),1);
	      fread(jung,bytesu,1,fp);

	       if(feof(fp)) break;
		if(strncmp(jung->name,temp,strlen(temp))==0){
		  itoa(recor,getsuchar,10);
		  strcpy(memori[j].record,getsuchar);
		  strcpy(memori[j].nalza,jung->nalza);
		  strcpy(memori[j].name,jung->name);
		  strcpy(memori[j].sex,jung->sex);
		  j++;
	       }
		  free(jung);
		  recor++;
	      }
		 fclose(fp);
		 free(jung);
	    } /* 이름앞의 if */

	  else if(st==2 && sun==1){

		boxx();
		xy_puts(29,7,"☞  찾고 싶은 이름은 ?");
		xy_puts(24,13,"예) 성함이 '*길*'인 사람->'길'만");
		xy_puts(28,14,"입력하세요.");
		xy_puts(24,16,"예) 성함이 '*길동'->'길동'");
		normal();
		 xy_puts(29,9,"*");
	      v=kbd(13);
		 if(v==-1){
		   fclose(fp);
		   free(memori);
		   return;
		 }
	       xy_vgets(30,9,6,temp);
	       for(i=0;i<6;i++)
		 if( (strncmp(&temp[i]," ",1)) == 0 )
		   {
		     temp[i]='\0';
		      break;
		     }

	     while(1){
	      jung=(struct hyunjung *) calloc(sizeof(struct hyunjung),1);
	       fread(jung,bytesu,1,fp);
	       if(feof(fp)) break;
		v=0;
		for(i=2;i<8;i++){
		    strcpy(&temp2[v],&(jung->name[i]));
		    v++;
		  }
		temp2[v]='\0';

		if(strncmp(temp2,temp,strlen(temp))==0){
		  itoa(recor,getsuchar,10);
		  strcpy(memori[j].record,getsuchar);
		  strcpy(memori[j].nalza,jung->nalza);
		  strcpy(memori[j].name,jung->name);
		  strcpy(memori[j].sex,jung->sex);
		  j++;
	       }
		  free(jung);
		  recor++;
	      }
		 free(jung);
		 fclose(fp);
	   }

	    else if(st==2 && sun==2){
	       boxx();

	       xy_puts(24,7,"☞ 찾고 싶은 사람의 출생년도는?");
	       xy_puts(24,13,"예) 출생년도가 '7*'->'7'만 입력");
	       xy_puts(28,14,"하세요.");
	       xy_puts(24,16,"예) 출생년도가 '78'->'78'입력");
	       normal();
	      v=kbd(14);
		 if(v==-1)
		 { fclose(fp);
		   free(memori);
		   return;
		}
		xy_vgets(30,9,2,temp);
	       for(i=0;i<2;i++)
		 if( (strncmp(&temp[i]," ",1)) == 0 )
		   {
		     temp[i]='\0';
		      break;
		     }


	   while(1){
	     jung=(struct hyunjung *) calloc(sizeof(struct hyunjung),1);
	      fread(jung,bytesu,1,fp);

	       if(feof(fp)) break;
		if(strncmp(jung->nai,temp,strlen(temp))==0){
		  itoa(recor,getsuchar,10);
		  strcpy(memori[j].record,getsuchar);
		  strcpy(memori[j].nalza,jung->nalza);
		  strcpy(memori[j].name,jung->name);
		  strcpy(memori[j].sex,jung->sex);
		  j++;
	       }
		  free(jung);
		  recor++;
	      }
		 fclose(fp);
		 free(jung);
	    }

	      else if(sun==3){
		 boxx();
		 xy_puts(25,7,"☞ 찾고싶은 사람의 생일은 ?");
		 xy_puts(24,13,"예) '07/*'(7월달)인 사람 -> '07/'");
		 xy_puts(29,14,"만 입력하세요.");
		 xy_puts(24,16,"예) '07/1*' -> '07/1'만");
		 xy_puts(24,18,"예) '07/17' -> '07/17'만 ");
		 normal();
	       set_attrib(29,35,9,0x15);
		v=kbd(16);
		 if(v==-1)
		  { fclose(fp);
		    free(memori);
		    return;
		  }

		v=kbd(17);
		 if(v==-1)
		  { fclose(fp);
		   free(memori);
		   return;
		  }
	       xy_vgets(30,9,2,temp);
	       xy_vgets(33,9,2,temp2);
		for(i=0;i<2;i++)
		 if( (strncmp(&temp2[i]," ",1)) == 0 )
		   {
		     temp2[i]='\0';
		      break;
		     }

	    while(1){
	     jung=(struct hyunjung *) calloc(sizeof(struct hyunjung),1);
	      fread(jung,bytesu,1,fp);

	       if(feof(fp)) break;
		if(strncmp(jung->sdal,temp,strlen(temp))==0
		   && strncmp(jung->sil,temp2,strlen(temp2))==0){
		  itoa(recor,getsuchar,10);
		  strcpy(memori[j].record,getsuchar);
		  strcpy(memori[j].nalza,jung->nalza);
		  strcpy(memori[j].name,jung->name);
		  strcpy(memori[j].sex,jung->sex);
		  j++;
	       }
		  free(jung);
		  recor++;
	      }
		 fclose(fp);
		 free(jung);
	    }

	    else if(sun==4){
		 boxx();
		 xy_puts(24,7,"☞ 찾고싶은 사람이 나와 어떤관계?");
		 xy_puts(24,13,"예를들어, 친구,고객,친척,요주인물");
		 xy_puts(24,14,"등등 여러가지를 입력했다가 검색할");
		 xy_puts(24,15,"수 있겠죠.");
		 v=kbd(24);
		 if(v==-1)
		  { fclose(fp);
		   free(memori);
		   return;
		  }
		 xy_vgets(30,9,14,temp);
		for(i=0;i<14;i++)
		 if( (strncmp(&temp[i]," ",1)) == 0 )
		   {
		     temp[i]='\0';
		      break;
		     }

	    while(1){
	     jung=(struct hyunjung *) calloc(sizeof(struct hyunjung),1);
	      fread(jung,bytesu,1,fp);

	       if(feof(fp)) break;
		if(strncmp(jung->me,temp,strlen(temp))==0)
		{
		  itoa(recor,getsuchar,10);
		  strcpy(memori[j].record,getsuchar);
		  strcpy(memori[j].nalza,jung->nalza);
		  strcpy(memori[j].name,jung->name);
		  strcpy(memori[j].sex,jung->sex);
		  j++;
	       }
		  free(jung);
		  recor++;
	      }
		 fclose(fp);
		 free(jung);
	    }
	    else if(sun==5){
		 boxx();
		 xy_puts(24,7,"☞ 찾고싶은 사람의 고향은?");
		 xy_puts(24,13,"점(.)을 입력한 전까지 검색됩니다.");
		 xy_puts(24,14,"예) '제주도.'을 입력하면");
		 xy_puts(28,15,"'제주도∼'가 모두 검색됨. ");
		 xy_puts(24,17,"예) '제주도 을시.'을 입력하면");
		 xy_puts(28,18,"'제주도 을시∼'가 모두 검색됨.");
		 v=kbd(25);
		 if(v==-1)
		  { fclose(fp);
		   free(memori);
		   return;
		  }
		 xy_vgets(30,9,18,temp);
		for(i=0;i<18;i++)
		 if( (strncmp(&temp[i],".",1)) == 0 )
		   {
		     temp[i]='\0';
		      break;
		     }

	    while(1){
	     jung=(struct hyunjung *) calloc(sizeof(struct hyunjung),1);
	      fread(jung,bytesu,1,fp);

	       if(feof(fp)) break;
		if(strncmp(jung->cdo,temp,strlen(temp))==0)
		{
		  itoa(recor,getsuchar,10);
		  strcpy(memori[j].record,getsuchar);
		  strcpy(memori[j].nalza,jung->nalza);
		  strcpy(memori[j].name,jung->name);
		  strcpy(memori[j].sex,jung->sex);
		  j++;
	       }
		  free(jung);
		  recor++;
	      }
		 fclose(fp);
		 free(jung);
	    }
	    else if(sun==6){
		 boxx();
		 xy_puts(24,7,"☞ 찾고싶은 사람의 취미는?");
		 xy_puts(24,13,"역시, 앞에서부터 일부만 입력하든지");
		 xy_puts(24,14,"전부 다 입력하든지 알아서 하세요.");
		 v=kbd(26);
		 if(v==-1)
		  { fclose(fp);
		   free(memori);
		   return;
		  }
		 xy_vgets(30,9,12,temp);
		for(i=0;i<12;i++)
		 if( (strncmp(&temp[i]," ",1)) == 0 )
		   {
		     temp[i]='\0';
		      break;
		     }

	    while(1){
	     jung=(struct hyunjung *) calloc(sizeof(struct hyunjung),1);
	      fread(jung,bytesu,1,fp);

	       if(feof(fp)) break;
		if(strncmp(jung->hobby,temp,strlen(temp))==0)
		{
		  itoa(recor,getsuchar,10);
		  strcpy(memori[j].record,getsuchar);
		  strcpy(memori[j].nalza,jung->nalza);
		  strcpy(memori[j].name,jung->name);
		  strcpy(memori[j].sex,jung->sex);
		  j++;
	       }
		  free(jung);
		  recor++;
	      }
		 fclose(fp);
		 free(jung);
	    }

	    else if(sun==7){
		  boxx();
		 xy_puts(25,7,"☞ 찾고싶은 사람의 거주도시는 ?");
		 xy_puts(24,13,"여기서, 거주도시란 특별시,직할시만");
		 xy_puts(24,14,"해당합니다.(서울,부산 등등)");
		 normal();
		v=kbd(18);
		 if(v==-1)
		 { fclose(fp);
		   free(memori);
		   return;
		}
		xy_vgets(30,9,4,temp);

		for(i=0;i<4;i++)
		 if( (strncmp(&temp[i]," ",1)) == 0 )
		   {
		     temp[i]='\0';
		      break;
		     }

	    while(1){
	     jung=(struct hyunjung *) calloc(sizeof(struct hyunjung),1);
	      fread(jung,bytesu,1,fp);

	       if(feof(fp)) break;
		if(strncmp(jung->hzuso1,temp,strlen(temp))==0){
		  itoa(recor,getsuchar,10);
		  strcpy(memori[j].record,getsuchar);
		  strcpy(memori[j].nalza,jung->nalza);
		  strcpy(memori[j].name,jung->name);
		  strcpy(memori[j].sex,jung->sex);
		  j++;
	       }
		  free(jung);
		  recor++;
	      }
		 fclose(fp);
		 free(jung);
	    }
	     else if(sun==8){
		  boxx();
		 xy_puts(25,7,"☞ 찾고싶은 사람의 나머지주소는?");
		 xy_puts(24,13,"점(.)을 입력한 전까지 검색됩니다.");
		 xy_puts(24,15,"예) '강남구.'을 입력하면 '강남구'");
		 xy_puts(27,16,"로 시작하는 주소는 모두 검색됨.");
		 xy_puts(24,18,"예) '강남구 역삼동.'을 입력하면");
		 xy_puts(28,19,"'강남구 역삼동'으로 시작하는");
		 xy_puts(28,20,"주소가 모두 검색됨");
		 normal();
		v=kbd(23);
		 if(v==-1)
		 { fclose(fp);
		   free(memori);
		   return;
		}
		xy_vgets(30,9,28,temp);

		for(i=0;i<28;i++)
		 if( (strncmp(&temp[i],".",1)) == 0 )
		   {
		     temp[i]='\0';
		      break;
		     }

	    while(1){
	     jung=(struct hyunjung *) calloc(sizeof(struct hyunjung),1);
	      fread(jung,bytesu,1,fp);

	       if(feof(fp)) break;
		if(strncmp(jung->hzuso2,temp,strlen(temp))==0){
		  itoa(recor,getsuchar,10);
		  strcpy(memori[j].record,getsuchar);
		  strcpy(memori[j].nalza,jung->nalza);
		  strcpy(memori[j].name,jung->name);
		  strcpy(memori[j].sex,jung->sex);
		  j++;
	       }
		  free(jung);
		  recor++;
	      }
		 fclose(fp);
		 free(jung);
	    }
	    else if(sun==9){
		  boxx();
		 xy_puts(25,7,"☞ 찾고싶은 사람의 직업은 ?");
		 xy_puts(24,13,"역시, 앞에서부터 일부만 입력하든지");
		 xy_puts(24,14,"전부 다 입력하든지 알아서 하세요.");
		 normal();
		v=kbd(19);
		 if(v==-1)
		 { fclose(fp);
		   free(memori);
		   return;
		}
		xy_vgets(30,9,24,temp);

		for(i=0;i<24;i++)
		 if( (strncmp(&temp[i]," ",1)) == 0 )
		   {
		     temp[i]='\0';
		      break;
		     }

	    while(1){
	     jung=(struct hyunjung *) calloc(sizeof(struct hyunjung),1);
	      fread(jung,bytesu,1,fp);

	       if(feof(fp)) break;
		if(strncmp(jung->job,temp,strlen(temp))==0){
		  itoa(recor,getsuchar,10);
		  strcpy(memori[j].record,getsuchar);
		  strcpy(memori[j].nalza,jung->nalza);
		  strcpy(memori[j].name,jung->name);
		  strcpy(memori[j].sex,jung->sex);
		  j++;
	       }
		  free(jung);
		  recor++;
	      }
		 fclose(fp);
		 free(jung);
	    }

	   else if(sun==10){
		  boxx();
		 xy_puts(25,7,"☞ 찾고싶은 사람의 직장명은 ?");
		 xy_puts(24,13,"역시, 앞에서부터 일부만 입력하든지");
		 xy_puts(24,14,"전부 다 입력하든지 알아서 하세요.");

		 normal();
		v=kbd(20);
		 if(v==-1)
		 { fclose(fp);
		   free(memori);
		   return;
		}
		xy_vgets(30,9,28,temp);

		for(i=0;i<28;i++)
		 if( (strncmp(&temp[i]," ",1)) == 0 )
		   {
		     temp[i]='\0';
		      break;
		     }

	    while(1){
	     jung=(struct hyunjung *) calloc(sizeof(struct hyunjung),1);
	      fread(jung,bytesu,1,fp);

	       if(feof(fp)) break;
		if(strncmp(jung->jobname,temp,strlen(temp))==0){
		  itoa(recor,getsuchar,10);
		  strcpy(memori[j].record,getsuchar);
		  strcpy(memori[j].nalza,jung->nalza);
		  strcpy(memori[j].name,jung->name);
		  strcpy(memori[j].sex,jung->sex);
		  j++;
	       }
		  free(jung);
		  recor++;
	      }
		 fclose(fp);
		 free(jung);
	    }

	    else if(sun==11){
		  boxx();
		 xy_puts(24,7,"☞ 찾고싶은 사람의 직장의 도시명 ?");
		 xy_puts(24,12,"예) 서울,부산...");
		 normal();
		 v=kbd(18);
		 if(v==-1)
		 { fclose(fp);
		   free(memori);
		   return;
		}
		xy_vgets(30,9,4,temp);

		for(i=0;i<4;i++)
		 if( (strncmp(&temp[i]," ",1)) == 0 )
		   {
		     temp[i]='\0';
		      break;
		     }

	    while(1){
	     jung=(struct hyunjung *) calloc(sizeof(struct hyunjung),1);
	      fread(jung,bytesu,1,fp);

	       if(feof(fp)) break;
		if(strncmp(jung->jobzuso1,temp,strlen(temp))==0){
		  itoa(recor,getsuchar,10);
		  strcpy(memori[j].record,getsuchar);
		  strcpy(memori[j].nalza,jung->nalza);
		  strcpy(memori[j].name,jung->name);
		  strcpy(memori[j].sex,jung->sex);
		  j++;
	       }
		  free(jung);
		  recor++;
	      }
		 fclose(fp);
		 free(jung);
	    }
	    else if(sun==12){
		 boxx();
		 xy_puts(24,7,"☞ 찾고싶은 사람의 직장나머지주소?");
		 xy_puts(24,13,"점(.)을 입력한 전까지 검색됩니다.");
		 xy_puts(24,15,"예) '강남구.'을 입력하면 '강남구'");
		 xy_puts(27,16,"로 시작하는 주소는 모두 검색됨.");
		 xy_puts(24,18,"예) '강남구 역삼동.'을 입력하면");
		 xy_puts(28,19,"'강남구 역삼동'으로 시작하는");
		 xy_puts(28,20,"주소가 모두 검색됨");
		 normal();
		v=kbd(23);
		 if(v==-1)
		 { fclose(fp);
		   free(memori);
		   return;
		}
		xy_vgets(30,9,28,temp);

		for(i=0;i<28;i++)
		 if( (strncmp(&temp[i],".",1)) == 0 )
		   {
		     temp[i]='\0';
		      break;
		     }

	    while(1){
	     jung=(struct hyunjung *) calloc(sizeof(struct hyunjung),1);
	      fread(jung,bytesu,1,fp);

	       if(feof(fp)) break;
		if(strncmp(jung->jobzuso2,temp,strlen(temp))==0){
		  itoa(recor,getsuchar,10);
		  strcpy(memori[j].record,getsuchar);
		  strcpy(memori[j].nalza,jung->nalza);
		  strcpy(memori[j].name,jung->name);
		  strcpy(memori[j].sex,jung->sex);
		  j++;
	       }
		  free(jung);
		  recor++;
	      }
		 fclose(fp);
		 free(jung);
	    }


	    else if(sun==13){
		boxx();
		 xy_puts(24,7,"☞ 찾고싶은 사람이 속한 부서명은 ?");
		 xy_puts(24,13,"역시, 앞에서부터 일부만 입력하든지");
		 xy_puts(24,14,"전부 다 입력하든지 알아서 하세요.");
		 normal();
		v=kbd(22);
		 if(v==-1)
		 { fclose(fp);
		   free(memori);
		   return;
		}
		xy_vgets(30,9,26,temp);

		for(i=0;i<26;i++)
		 if( (strncmp(&temp[i]," ",1)) == 0 )
		   {
		     temp[i]='\0';
		      break;
		     }

	    while(1){
	     jung=(struct hyunjung *) calloc(sizeof(struct hyunjung),1);
	      fread(jung,bytesu,1,fp);

	       if(feof(fp)) break;
		if(strncmp(jung->busu,temp,strlen(temp))==0){
		  itoa(recor,getsuchar,10);
		  strcpy(memori[j].record,getsuchar);
		  strcpy(memori[j].nalza,jung->nalza);
		  strcpy(memori[j].name,jung->name);
		  strcpy(memori[j].sex,jung->sex);
		  j++;
	       }
		  free(jung);
		  recor++;
	      }
		 fclose(fp);
		 free(jung);
	    }

	      else if(sun==14){
		   boxx();
		    xy_puts(24,7,"☞ 찾고싶은 사람의 직장내 직위는 ?");
		    xy_puts(24,13,"역시, 앞에서부터 일부만 입력하든지");
		    xy_puts(24,14,"전부 다 입력하든지 알아서 하세요.");
		    normal();
		v=kbd(23);
		 if(v==-1)
		 { fclose(fp);
		   free(memori);
		   return;
		}
		xy_vgets(30,9,28,temp);

		for(i=0;i<28;i++)
		 if( (strncmp(&temp[i]," ",1)) == 0 )
		   {
		     temp[i]='\0';
		      break;
		     }

	    while(1){
	     jung=(struct hyunjung *) calloc(sizeof(struct hyunjung),1);
	      fread(jung,bytesu,1,fp);

	       if(feof(fp)) break;
		if(strncmp(jung->gikw,temp,strlen(temp))==0){
		  itoa(recor,getsuchar,10);
		  strcpy(memori[j].record,getsuchar);
		  strcpy(memori[j].nalza,jung->nalza);
		  strcpy(memori[j].name,jung->name);
		  strcpy(memori[j].sex,jung->sex);
		  j++;
	       }
		  free(jung);
		  recor++;
	      }
		 fclose(fp);
		 free(jung);
	    }

		  window(17,6,63,19);
		  clrscr();
		  textattr(0x30);
		  window(15,5,61,18);
		  clrscr();
		  window(1,1,80,25);
   xy_puts(17,6,"등록한 날짜     번호       성함       성별");
		  y_line(6,7,30);
		  y_line(6,7,39);
		  y_line(6,7,53);
		  xy_puts(17,7,"━━━━━━━━━━━━━━━━━━━━━━");
		  single_box(15,5,59,18);
		  textattr(0x3f);
		  y_line(8,17,30);
		  y_line(8,17,39);
		  y_line(8,17,53);
		 textattr(0x30);
	  xy_puts(1,20,"┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
	  xy_puts(1,21,"┃ ↕ 선택막대이동   Delete 해당 레코드 삭제   Insert 해당 레코드 다음에 삽입 ┃");
	  xy_puts(1,22,"┃ ←   조회,수정                                                             ┃");
	  xy_puts(1,23,"┃                                                                            ┃");
	  xy_puts(1,24,"┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");
	   gotoxy(6,22); putch(4);
	  textattr(0x4f);
	  xy_puts(20,22," ※ 검색된 레코드가 10개를 초과할 경우에는 위,아래로 스크롤");
	  xy_puts(20,23,"    시키면서 모든 레코드를 볼 수 있습니다 !                ");


	       textattr(0x3f);

	       for(i=0;i<=9;i++){
		   xy_puts(18,i+8,memori[i].nalza);
		   xy_puts(32,i+8,memori[i].record);
		   xy_puts(43,i+8,memori[i].name);
		   xy_puts(55,i+8,memori[i].sex);
	     }   /*네모 안에 10개의 레 코드를찍는다 */


    if(j <= 10){
      ban=8;
      set_attrib(17,58,8,0x9e);

      while(1){

       switch(get_key()){

	 case DOWN : if(ban < 8+(j-1)){
		      set_attrib(17,58,ban,0x3f);  ban++;
		      set_attrib(17,58,ban,0x9e);
		      break;  }
		      else if(ban==8+(j-1)){
		      set_attrib(17,58,ban,0x3f);  ban=8;
		      set_attrib(17,58,ban,0x9e);
		      break;
		    }
	 case UP : if(ban != 8){
		      set_attrib(17,58,ban,0x3f); ban--;
		      set_attrib(17,58,ban,0x9e);
		      break; }
		      else if(ban == 8){
		       set_attrib(17,58,ban,0x3f); ban=8+(j-1);
		       set_attrib(17,58,ban,0x9e);
		       break;
		    }
	 case CR :  xy_vgets(32,ban,3,ki);
		    kiu=atoi(ki);
		    gettext(1,3,80,25,buf);
		    fp=fopen("person2.dat","rb+");
		     jung=(struct hyunjung *) calloc(bytesu,1);
		     fseek(fp,bytesu*(kiu-1),SEEK_SET);
		     fread(jung,bytesu,1,fp);
		     fclose(fp);
		     screen();
		     textattr(0x9f);
	 xy_puts(5,25,"                 ☞   바뀐 내용이 있으면 고치세요   ☜                  ");

			 textattr(0xf0);
      xy_puts(12,4,jung->name);
      xy_puts(28,4,jung->sex);
      xy_puts(42,4,jung->nai);
      xy_puts(45,4,jung->sdal);
      xy_puts(48,4,jung->sil);
      xy_puts(64,4,jung->me);
      xy_puts(13,6,jung->htel1);
      xy_puts(19,6,jung->htel2);
      xy_puts(24,6,jung->htel3);
      xy_puts(38,6,jung->ho1);
      xy_puts(43,6,jung->ho2);
      xy_puts(48,6,jung->ho3);
      xy_puts(60,6,jung->cdo);
      xy_puts(14,8,jung->hand1);
      xy_puts(19,8,jung->hand2);
      xy_puts(24,8,jung->hand3);
      xy_puts(44,8,jung->zuf);
      xy_puts(51,8,jung->zub);
      xy_puts(66,8,jung->hobby);
      xy_puts(14,10,jung->hzuso1);
      xy_puts(22,10,jung->hzuso2);
      xy_puts(12,12,jung->job);
      xy_puts(46,12,jung->jobname);
      xy_puts(12,14,jung->busu);
      xy_puts(48,14,jung->gikw);
      xy_puts(15,16,jung->jobtel1);
      xy_puts(21,16,jung->jobtel2);
      xy_puts(26,16,jung->jobtel3);
      xy_puts(34,16,jung->jobtel4);
      xy_puts(44,16,jung->change);
      xy_puts(61,16,jung->fax1);
      xy_puts(67,16,jung->fax2);
      xy_puts(72,16,jung->fax3);
      xy_puts(16,18,jung->jobzuso1);
      xy_puts(24,18,jung->jobzuso2);


		      free(jung);
		      w=1;
		      w=input();

		     if(w==-1) {
			 textattr(0x3f);
			 puttext(1,3,80,25,buf);
			 break;
		      }
		     jung=(struct hyunjung *) calloc(bytesu,1);
		     read(jung);
		     v=0;

		     v=talk(v,2);

	 switch(v){
		case 0 :
		       fp=fopen("person2.dat","rb+");
		       fseek(fp,bytesu*(kiu-1),SEEK_SET);
		       fwrite(jung,bytesu,1,fp);
		       free(jung);
		       fclose(fp);
			break;
		case 1 : free(jung);
			 break;
		case -1 : free(jung);
			  break;
		}
		  textattr(0x3f);
		  puttext(1,3,80,25,buf);
		  break;     /*여기까지가 엔터키에대한 코드 */

	 case DEL :  gettext(24,10,53,16,buf);
		       xy_vgets(32,ban,3,ki);
		     i=0;
		     i=talk(i,1);
		    switch(i){

		     case 0 :
		       v=1;


		       kiu=atoi(ki);
			fp=fopen("person2.dat","rb");  /* 임시파일을 만들어서 삭제 레코드만*/
			fp2=fopen("temp.dat","wb");    /* 빼고 모든 레코드를 다 쓴다 */

		       fseek(fp,0,SEEK_SET);          /* 원래 화일을 지우고 임시파일을  */
						     /* 명함록파일로 한다 */
			while(1){
			   record=(struct hyunjung *) calloc(bytesu,1);
			   fread(record,bytesu,1,fp);
			   if(feof(fp))
			    { free(record); break;  }
			      if(kiu!=v)
				fwrite(record,bytesu,1,fp2);
			     v++;
			     free(record);
			   }
			    fcloseall();
			    unlink("person2.dat");
			    rename("temp.dat","person2.dat");
			    free(memori);
			    return;
		      case 1 :  puttext(24,10,53,16,buf);
				break;
		      case -1 : puttext(24,10,53,16,buf);
				break;
		   }
			textattr(0x3f);
		      break;
	 case INS :  gettext(1,3,80,25,buf);
		     xy_vgets(32,ban,3,ki);
		     kiu=atoi(ki);
		    screen();
		      textattr(0x9f);
		      xy_puts(5,25,"                   ☞   삽입할 명함을 입력하세요   ☜                   ");

		    i=0;
		    i=input();

		    if(i == -1){
		       puttext(1,3,80,25,buf);
			 textattr(0x3f);
		       break;
		      }

		    record=(struct hyunjung *) calloc(bytesu,1);
		    read(record);

		     v=0;
		      v=talk(v,3);
		      switch(v){

		       case 0 :

			 fp=fopen("person2.dat","rb");
			 fp2=fopen("temp.dat","wb");
			  fseek(fp,0,SEEK_SET);
			  for(i=0;i<kiu;i++)
			   {
			    lee=(struct hyunjung *) calloc(bytesu,1);
			    fread(lee,bytesu,1,fp);
			     if(feof(fp))
			       {  free(lee); break; }
			    fwrite(lee,bytesu,1,fp2);
			     free(lee);
			      }

			   fwrite(record,bytesu,1,fp2);
			   free(record);
			 while(1){
			   lee=(struct hyunjung *) calloc(bytesu,1);
			    fread(lee,bytesu,1,fp);
			   if(feof(fp))
			    break;
			   fwrite(lee,bytesu,1,fp2);
			    free(lee);
			  }
			   fcloseall();
			   unlink("person2.dat");
			   rename("temp.dat","person2.dat");
			   puttext(1,3,80,25,buf);
			   free(memori);
			   return;
		      case 1 : free(record);
			       puttext(1,3,80,25,buf);
			       break;
		      case -1 :free(record);
			       puttext(1,3,80,25,buf);
			       break;
		   }
		     textattr(0x3f);
		     break;
	 case ESC : free(memori);
		    normal();
		    return;

	 default :  break;
	   }  /* 바깥 switch 블럭 */

       }  /* while */
   }

       else{
	   fo=10; back=-1;  ban=8;
	   set_attrib(17,58,8,0x9e);
	   while(1){
	      textcolor(15);
	      switch(get_key()){
	       case DOWN :
		    if(ban==17){

			   if(fo==j){
			    xy_puts(4,23,"☞ 마지막 "); getch();
/* 아래키를*/               xy_puts(4,23,"          ");
			     break;
			   }
/* 의 처리 */		  for(i=8;i<=16;i++){
			   lee=(struct node *) calloc(sizeof(struct node),1);
			      xy_vgets(18,i+1,10,lee->nalza);
			      xy_vgets(32,i+1,3,lee->record);
			      xy_vgets(43,i+1,8,lee->name);
			      xy_vgets(55,i+1,2,lee->sex);


			      xy_puts(18,i,lee->nalza);
			      xy_puts(32,i,lee->record);
			      xy_puts(43,i,lee->name);
			      xy_puts(55,i,lee->sex);

			      free(lee);

			   }
			      xy_puts(18,17,memori[fo].nalza);
			      xy_puts(32,17,memori[fo].record);
			      xy_puts(43,17,memori[fo].name);
			      xy_puts(55,17,memori[fo].sex);

			   set_attrib(17,58,ban,0x9e);
			   fo++; back++;
			   break;
			 }
			  else {
			  set_attrib(17,58,ban,0x3f);  ban++;
			  set_attrib(17,58,ban,0x9e);
			  break;
		       }
	      case UP :  if(ban==8){
			  if( back==-1 ){

			   xy_puts(4,23,"☞ 처음 "); getch();

			   xy_puts(4,23,"        ");
			   break;
			  }
			  for(i=16;i>=8;i--){

			   lee=(struct node *) calloc(sizeof(struct node),1);
			      xy_vgets(18,i,10,lee->nalza);
			      xy_vgets(32,i,3,lee->record);
			      xy_vgets(43,i,8,lee->name);
			      xy_vgets(55,i,1,lee->sex);


			      xy_puts(18,i+1,lee->nalza);
			      xy_puts(32,i+1,lee->record);
			      xy_puts(43,i+1,lee->name);
			      xy_puts(55,i+1,lee->sex);

			      free(lee);
			   }
			      xy_puts(18,8,"          ");
			      xy_puts(32,8,"   ");
			      xy_puts(43,8,"       ");
			      xy_puts(55,8," ");
			      xy_puts(18,8,memori[back].nalza);
			      xy_puts(32,8,memori[back].record);
			      xy_puts(43,8,memori[back].name);
			      xy_puts(55,8,memori[back].sex);



			   set_attrib(17,58,ban,0x9e);
			   back--; fo--;
			   break;
			  }
			   else{
			   set_attrib(17,58,ban,0x3f); ban--;
			   set_attrib(17,58,ban,0x9e);
			   break;
			   }
	      case ESC :   free(memori);
			   normal();
			   return;


	      case CR :
			xy_vgets(32,ban,3,ki);
			kiu=atoi(ki);
			gettext(1,3,80,25,buf);

		     fp=fopen("person2.dat","rb+");
		     jung=(struct hyunjung *) calloc(bytesu,1);
		     fseek(fp,bytesu*(kiu-1),SEEK_SET);
		     fread(jung,bytesu,1,fp);
		     fclose(fp);
		     screen();
		     textattr(0x9f);
	 xy_puts(5,25,"                 ☞   바뀐 내용이 있으면 고치세요   ☜                  ");

			 textattr(0xf0);
      xy_puts(12,4,jung->name);
      xy_puts(28,4,jung->sex);
      xy_puts(42,4,jung->nai);
      xy_puts(45,4,jung->sdal);
      xy_puts(48,4,jung->sil);
      xy_puts(64,4,jung->me);
      xy_puts(13,6,jung->htel1);
      xy_puts(19,6,jung->htel2);
      xy_puts(24,6,jung->htel3);
      xy_puts(38,6,jung->ho1);
      xy_puts(43,6,jung->ho2);
      xy_puts(48,6,jung->ho3);
      xy_puts(60,6,jung->cdo);
      xy_puts(14,8,jung->hand1);
      xy_puts(19,8,jung->hand2);
      xy_puts(24,8,jung->hand3);
      xy_puts(44,8,jung->zuf);
      xy_puts(51,8,jung->zub);
      xy_puts(66,8,jung->hobby);
      xy_puts(14,10,jung->hzuso1);
      xy_puts(22,10,jung->hzuso2);
      xy_puts(12,12,jung->job);
      xy_puts(46,12,jung->jobname);
      xy_puts(12,14,jung->busu);
      xy_puts(48,14,jung->gikw);
      xy_puts(15,16,jung->jobtel1);
      xy_puts(21,16,jung->jobtel2);
      xy_puts(26,16,jung->jobtel3);
      xy_puts(34,16,jung->jobtel4);
      xy_puts(44,16,jung->change);
      xy_puts(61,16,jung->fax1);
      xy_puts(67,16,jung->fax2);
      xy_puts(72,16,jung->fax3);
      xy_puts(16,18,jung->jobzuso1);
      xy_puts(24,18,jung->jobzuso2);


		     free(jung);
		      w=1;
		      w=input();
		     if(w==-1) {
		      textattr(0x3f);
		      puttext(1,3,80,25,buf);
		      break;
		      }
		     jung=(struct hyunjung *) calloc(bytesu,1);
		     read(jung);
		     v=0;

		     v=talk(v,2);

	 switch(v){
		case 0 :
		   fp=fopen("person2.dat","rb+");
		   fseek(fp,bytesu*(kiu-1),SEEK_SET);
		   fwrite(jung,bytesu,1,fp);
		   free(jung);
		   fclose(fp);
		   break;
		case 1 : free(jung);
			 break;
		case -1 : free(jung);
			  break;
		}

			 textattr(0x3f);
			 puttext(1,3,80,25,buf);
			 break;

	 case DEL :  gettext(24,10,53,16,buf);
		      xy_vgets(32,ban,3,ki);
		     i=0;
		     i=talk(i,1);
		    switch(i){

		     case 0 :
		       v=1;

		       kiu=atoi(ki);
			fp=fopen("person2.dat","rb");  /* 임시파일을 만들어서 삭제 레코드만*/
			fp2=fopen("temp.dat","wb");    /* 빼고 모든 레코드를 다 쓴다 */
		       fseek(fp,0,SEEK_SET);          /* 원래 화일을 지우고 임시파일을  */
						     /* 명함록파일로 한다 */
			while(1){
			   record=(struct hyunjung *) calloc(bytesu,1);
			   fread(record,bytesu,1,fp);
			   if(feof(fp))
			   {  free(record); break;}
			     if(kiu!=v)
				fwrite(record,bytesu,1,fp2);
			     v++;
			     free(record);
			   }
			    fcloseall();
			    unlink("person2.dat");
			    rename("temp.dat","person2.dat");
			    free(memori);
			    return;

		      case 1 :  puttext(24,10,53,16,buf);
				break;
		      case -1 : puttext(24,10,53,16,buf);
				break;
		   }
			textattr(0x3f);
		      break;
	    case INS :
		     gettext(1,3,80,25,buf);
		     xy_vgets(32,ban,3,ki);
		     kiu=atoi(ki);
		    screen();
		     textattr(0x9f);
		     xy_puts(5,25,"                   ☞   삽입할 명함을 입력하세요   ☜                   ");

		    i=0;
		    i=input();

		    if(i == -1){
		       puttext(1,3,80,25,buf);
			 textattr(0x3f);
		       break;
		      }

		    record=(struct hyunjung *) calloc(bytesu,1);
		    read(record);

		     v=0;
		      v=talk(v,3);
		      switch(v){

		       case 0 :

			 fp=fopen("person2.dat","rb");
			 fp2=fopen("temp.dat","wb");
			  fseek(fp,0,SEEK_SET);
			  for(i=0;i<kiu;i++)
			   {
			    lee=(struct hyunjung *) calloc(bytesu,1);
			    fread(lee,bytesu,1,fp);
			     if(feof(fp))
			      break;
			    fwrite(lee,bytesu,1,fp2);
			     free(lee);
			      }

			   fwrite(record,bytesu,1,fp2);
			   free(record);
			 while(1){
			   lee=(struct hyunjung *) calloc(bytesu,1);
			    fread(lee,bytesu,1,fp);
			   if(feof(fp))
			   { free(lee); break; }
			   fwrite(lee,bytesu,1,fp2);
			    free(lee);
			  }

			   fcloseall();
			   unlink("person2.dat");
			   rename("temp.dat","person2.dat");

			   free(memori);
			   return;
		      case 1 : free(record);
			       puttext(1,3,80,25,buf);
			       break;
		      case -1 :free(record);
			       puttext(1,3,80,25,buf);
			       break;
		   }
		      textattr(0x3f);
		      break;
	    default : break;
			} /* switch */

		   } /* while */
	     }/* else */
  }




/*--------------------------------------------------------------------*/

      void menual(void){

   window(10,4,75,23);
   clrscr();
   window(1,1,80,25);

 textattr(0x70);
 xy_puts(8,3,"┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓");
 xy_puts(8,4,"┃                                                              ┃");
 xy_puts(8,5,"┃                                                              ┃");
 xy_puts(8,6,"┃                                                              ┃");
 xy_puts(8,7,"┃                                                              ┃");
 xy_puts(8,8,"┃                                                              ┃");
 xy_puts(8,9,"┃                                                              ┃");
xy_puts(8,10,"┃                                                              ┃");
xy_puts(8,11,"┃                                                              ┃");
xy_puts(8,12,"┃                                                              ┃");
xy_puts(8,13,"┃                                                              ┃");
xy_puts(8,14,"┃                                                              ┃");
xy_puts(8,15,"┃                                                              ┃");
xy_puts(8,16,"┃                                                              ┃");
xy_puts(8,17,"┃                                                              ┃");
xy_puts(8,18,"┃                                                              ┃");
xy_puts(8,19,"┃                                                              ┃");
xy_puts(8,20,"┃                                                              ┃");
xy_puts(8,21,"┃                                                              ┃");
xy_puts(8,22,"┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛");

 textattr(0x47);
 xy_puts(13,4,"  ☞    【 V-C.S (명함관리프로그램) 】  ver 1.1     ☜  ");
 textattr(0x70);
 xy_puts(10,5,"───────────────────────────────");

 textattr(0x70);
 xy_puts(10,7,"1. 입력된 명함은 person2.dat에 기록됩니다.");
 xy_puts(10,9,"2. 프로그램의 어디에서든 입력시 커서가 처음 위치한 곳부터 입력");
 xy_puts(13,10,"하세요.");
 xy_puts(10,12,"3. 검색할 대상을 입력할 때 검색하고자하는 단어의 앞에서부터");
 xy_puts(13,13,"일부만 입력해도 됩니다.");
 xy_puts(15,14,"예) '홍길동'을 검색할 때 '홍'이나 '홍길'만 입력해도 됨.");
 xy_puts(17,15,"'김'을 입력하면 '김∼'(성이 김씨인 사람)이 모두 검색됨.");
 xy_puts(13,16,"그러나, 공란을 입력하면 공란전까지만 검색됩니다.");
 xy_puts(13,17,"예외로, 주소와 고향 검색시는 점(.)을 입력한 전까지 검색됨.");
 xy_puts(15,18,"예) '강남구 역삼동.'을 입력하면 '강남구 역삼동∼'이 모두");
 xy_puts(19,19,"검색됨.");
 textattr(0x2f);
 xy_puts(10,20,"※ 즉, 앞에서부터 일부만 입력해도 입력된 문자열을 포함하는 ");
 xy_puts(10,21,"   모든 데이타가 검색된다는 예기죠. ");
 normal();
 get_key();

 }
/*--------------------------------------------------------------------*/


   void logo(void){
      int i;
      window(1,2,80,24);
      textbackground(1);
      clrscr();
      window(1,1,80,25);
      textattr(0x1f);
      gotoxy(4,5);
      cprintf("\r\n       ♥        ♥                ♠♠♠                  ◆◆◆◆  ");
      cprintf("\r\n        ♥      ♥               ♠      ♠               ◆         ");
      cprintf("\r\n         ♥    ♥      ■        ♠              ●        ◆◆◆◆  ");
      cprintf("\r\n          ♥  ♥                 ♠      ♠                       ◆ ");
      cprintf("\r\n            ♥                     ♠♠♠                  ◆◆◆◆  ");

      textattr(0x30);
      textattr(0x1f);
      single_box(1,2,79,24);
      normal();
}
/*----------------------------------------------------------------------*/
    void boxx(void){
       window(24,7,61,22);
       clrscr();
       textattr(0x70);
       window(22,6,58,21);
       clrscr();
       window(1,1,80,25);
       single_box(22,6,58,21);
       textattr(0x70);
       xy_puts(24,8,"─────────────────");
       textattr(0x70);
       xy_puts(24,10,"─────────────────");

       }
/*----------------------------------------------------------------------*/
    void chamgo(void){

      window(21,12,64,20);
       clrscr();
      window(1,1,80,25);
      textattr(0xb0);
      xy_puts(19,11,"┏━━━━━━━━━━━━━━━━━━━━┓");
      xy_puts(19,12,"┃※ 이 프로그램의 풀다운 기법은 <c프로그 ┃");
      xy_puts(19,13,"┃   래밍 실습> 이란 책을 참고했습니다.   ┃");
      xy_puts(19,14,"┃※ 이 프로그램은 버그가 있을 수 있습니다┃");
      xy_puts(19,15,"┃☎ 버그를 발견하셨을 때 저에게 메일을   ┃");
      xy_puts(19,16,"┃   주시면 고맙겠습니다.                 ┃");
      xy_puts(19,17,"┃                                        ┃");
      xy_puts(19,18,"┃                                        ┃");
      xy_puts(19,19,"┗━━━━━━━━━━━━━━━━━━━━┛");
      textattr(0x9f);
      xy_puts(24,17," ☞ 천리안ID: mixufo ☜ ");
      xy_puts(24,18," ☞ 하이텔ID: p0213  ☜ ");
      normal();
      getch();
      }
