#include <dos.h>
#include <stdlib.h>
#include <conio.h>

#include "hanio.h"
#include "extendio.h"
#include "misc.h"
#include "keydef.h"
#include "mylib.h"

#define default_color() set_vbcolor_han(BLACK, LIGHTGRAY)

static void interrupt (*old_isr09)(void);
static void interrupt new_isr09(void);
void setintr_new(void);
void setintr_old(void);

void init(void);
void status_line(void);
void logo_display(void);
void logo2_display(void);
void draw_game_screen(void);
void f_display(void);
void disp_game_quiz(int num);
void result_dis_page0(void);
void result_dis_page1(void);
void result_dis_page2(void);
void result_dis_page3(void);
void delete(int del_top, int del_left, int del_right);

int item_num[10];
int sex=0;
char name[10];
char pi[3];


char ss1[8][65+1] = {{"�a�A���A �e�wЁ ��������."},
		     {"�e ������ ���s���a."},
		     {"�����a�� �i��a�e ���e 3�a���a ���� �ᗡ���e �i��i �� ���s���a."},
		     {"�����e�e ������� �A�� ���a�a �e, �w, ѡ��, ���i, ����, ����,"},
		     {"�A���� �a���� �a��a�� �����i�� �g�� ���s���a."},
		     {"���� �a�A �a�e �������e �����a �w�׷i �w�wЁ�� ���s���a."},
		     {"�a�q �����A �a�a�� �sЁ �a�e�� �����a�� �i��a�e �S���a."},
		     {"�a, �a�� �i�ᶅ �a�q�a�� ���� �a�a�� �a �����a."}};

char ss_quest[10][3][45] ={{{"1. �����a�� �i��a�e �����e �A���� ���s���a."},
			  {"   �w���e ��a ������ �i��a�� ���s���a?"}},
			 {{"2. �����A�� ���w�a�i ���� �s���a."},
			  {"   ��a���a�a?"}},
			 {{"3. �����A�� ���� �A ���a�� �a ���s���a."},
			  {"   ��a ���i Ȃ�a�V�s���a?"}},
			 {{"4. ���� ϡ�� ���s���a."},
			  {"   ��� �� �����a�a?"}},
			 {{"5. ���� ���s���a."},
			  {"   ��� �����a�a?"}},
			 {{"6. �����A ������ ���s���a."},
			  {"   ��� ���� ���a�a?"}},
                         {{"7. �|�A ����a �a�� �� �{�s���a."},
                          {"   ��� �����a�a?"}},
                         {{"8. �x�e �e�A�� �a�q�� ��ᵡ�� ���s���a."},
                          {"   ��� �a�q���a�a?"}},
                         {{"9. �w���e �a �a�q�� ����A �a���V�s���a?"}},
                         {{"10. �韡 �|�A �墉�� ������ ���b�s���a."},
                          {"   ��� �����a�a?"}}};

char ss_item[10][3][40]={{{" ���� ����"},
                          {" �q�� ����"},
                          {" �ὢ ����"}},
						 {{" 300��"},
                          {" 1,000��"},
                          {" 2,000��"}},
                         {{" �E���� ���������e ���e ��"},
                          {" ���w�� ���a�� ��e ͡�w�E ��"},
                          {" ���e���a�� з�a�e ��w�� ���e��"}},
                         {{" �a�e��"},
                          {" �i�e��"},
			  {" ���e��"}},
                         {{" ñ�s���� �E �ᖁ�� ��"},
                          {" �i���� ���i"},
                          {" �j�e �s����"}},
                         {{" �eɡ��"},
			  {" �b��"},
                          {" ̅�ቱ"}},
                         {{" �׸w"},
                          {" đ�a��"},
			  {" �a���� ���e ���w�e ��"}},
                         {{" �a�a"},
                          {" �q�a"},
                          {" ������ �a��"}},
                         {{" �a �a�q�� �q�A ����i �����e�a."},
                          {" �a�s�A ���a�a�� �a���� ���i �e�a."},
                          {" ���e�A �a�� ���a�e�a."}},
                         {{" ��́�e ����"},
                          {" ���w�e ��"},
			  {" ���a�e ����"}}};

char ss_dis0[][50] = {{"���A �w���� Ȃ�e �墉 ���A�� �q�e �᯳����."},
		      {"�����e�e �a�� ���a���a �a���� ѡ������ �w�e �a�q"},
		      {"�i�i ���a�a �a���a�� �����A �s�e ���w�i ɡ���� �w"},
		      {"���� ���b�i �sЁ �a���V�s���a."}};

char ss_dis1[][3][3][75] ={{{{"  �w���e ���ⷥ �a���a�� ѥ�a�� �a���A �q����a ���w�i �i���e ���� �g��"},
							 {"  �g�s���a?"}},
							{{"  �w���e �j�� �a�a�ⷥ ���b�a�� ���a�e �a���i Η�����e ���e �A���� �a�e"},
							 {"  ���� �g�� ���s���a."}},
							{{"  �w���e �a�q�� �a�u�a�� �a���ⷥ �a�q�����a. �a��a �����A�A�a �ᢁ �a"},
							 {"  �u�a�A ��Ё ���a ���e ��Ё���e ���� �g�� �g�s���a?"}}},
						   {{{"  �ᢁ �����a���a. ��з�ⷡ�a �����A �e �a���i ���� �ᶅ �w���e ������"},
							 {"  �a �a�ᡡ�� ���e ���w�i �������a."}},
							{{"  �w���e ���b�i �a�� �a�q�����a. ӱ�e �A�w�i �i�a�a�e �A �w���� ����e"},
							 {"  �e�e�b�� ���� ���a�i �������a. �i �ɶwЁ ��������. "}},
							{{"  ���b �A�a�a�i ��Ё �a���A �g�e ���i ���e �w�����a�e �񮉷i �ᢁ ����"},
							 {"  �g���� Ё�� �i �������a."}}},
						   {{{"  ����ˢ�a�� �w�w�b�� Η��Ё ���i �i ���V���a. �a��a �e���e �����s��"},
							 {"  �a. ���� ӱ�i ��a�� �� �a���� �w�w�b�i ��ӡ �ɶw�i �� ���e ���i �x"},
							 {"  �a ��������."}},
							{{"  �a���e �aϡ�a�i �� �� ���e ���� �a�����a. �ᢁ �|�i зЁ �����a�e ��"},
							 {"  �e ���� �s���a. ��b�ⷥ �w���� ���b�e �����e �I �� ���e�e ����ӡ З��"},
							 {"  Ё ��������."}},
							{{"  �e���i �����i �� �a�e �a�q�����a. �����a��, �����i ���bЁ�� �e���i �� "},
							 {"  ���e �a�q�����a. �a��a �q�A ���e �A�e �������e �a�q�����a. �a�q�e"},
							 {"  �a�q�� ���� Ё ��������."}}},
						   {{{"  ���a�a�� ʁ�w�e �a�q�a���i ���a�a�e �a�q, ��a������ �a�e, ѥ�a�����i"},
							 {"  ���a�s���a. �i�����a?"}},
							{{"  ���i�ⷡ�� �������a �q���i �����a�e �a�q, �g�e å���i�� �i��A �ᶉ��"},
							 {"  �e ���i ���a�s���a. �a���a?"}},
							{{"  ��b���e ���q�b�i �a�� �a�q. �a��a ���� ���e �a�q�� �a���� �ᢁ ��"},
							 {"  �u�a�A ���� �A�� ���e �� �{���a."}}},
						   {{{"  �w���e �a�q���A �a��a�� �a�w���i �q�� ���s���a. �a�� �a���A ��Ё"},
							 {"  �� ���bЁ ��������. ������ �a���i �i�e�i �������a."}},
							{{"  ���w�ⷡ�� ��b�ⷥ �w���e �a�a�� �����i �����a�e �a�������a. ӱ�e "},
							 {"  ���i �󐁻� �i�� ���� �|�a�� �a�a ��������. �e ���i�A�e �����a ���a"},
							 {"  �� ���a���a ����Ё ��������."}},
							{{"  �w���e ���e���� ���s�a�A ���b�a�� �g�a�a �a���� �a�q�i �����a �a�� �g"},
							 {"  �e �����e �a�q�����a. �s�����e �a���� �A�e ���� ���a Е�{�� �i ��"},
							 {"  á�e �A �j�� �b��ⷥ �w���e �e���A�� �i ��wЁ �a�s���a."}}},
						   {{{"  �� �� �g�� ���� ���e �e�����a. ��q �e�a�e �a�q�A�A�e �a�a�� ���e �a"},
							 {"  ���i ���� ���a���e, �a��� ������ å���a�� ��A �e�i���� �g�e �a�q��"},
							 {"  �i �i �� ���s���a."}},
							{{"  �����q�� �w�a�� �w�w�b�� Η���a�a ���Eˢ�e �e�����a. ñ�b���� �� ��"},
							 {"  �� ���A �����e �e�� �����e �a�q�e �w�w�s���a."}},
							{{"  ���� ���A�� ̅�ቱ�� ���i�� ��V���a. �w�w�b�� �����A�� �e������ ��"},
							 {"  �a �a��a 10�A ���a �៥���� ���s���a�e �����e ���s���a �i �� ���i ��"},
							 {"  �����a."}}}};

char ss_dis2[][4][75] ={{{"  �w���e �� �i �A�����a�a �១�ו� �������e �����a ҉�W�e �a�q�i �����a"},
						 {"  �� �a������ �ɕ��a���i ���a�s���a. �����g�� ������ �g�a�a �����a�a "},
						 {"  �夁�i �i �a�a�e �e�� ���s���a. �a��a �ⷡ �g�� ���a�a�e �a���� ����"},
						 {"  �a�e �a���� �¯��a�A �����A�� ���s���a."}},
						{{"  ���w�ⷡ�a �a���� �Aâ�i �i ��ǡ�e ���� �a�e �a�q�����a. ѥ�a�� ���a"},
						 {"  ���i �a�����a �a�����A�� ������a�� З���a���i ���a�s���a. �w���w��"},
						 {"  ���w�i �����a�� �� ���e�a�e �a�q�� ���� �i��A �a�鮁 ���V���a."}},
						{{"  �w���� ѥ�a�� ����ӡ ���b�A �q���e ���b�a �����a. �w�w�A ���� �q�a ��"},
						 {"  �e �������a ���{�e �Ŭw�A �q�����i ���a�s���a. �a���� ���b���a �a���i"},
						 {"  �q�A�A �i�a�e ���� �����e �a�q�A �q�� ���e ���� �� �g�s���a."}}};

char ss_dis_f[][4][75] = {{{"  �a���� �w���� �a�q�� �{�e �a���i �e�v�i ���A�e �i������ �i�w���a ϩ�a"},
			   {"  �a�V���a. �a �a�q�� �������a �i ɷ�a�e �a �a�q�i ɷЁ �����i �e�a�A"},
			   {"  �I���� ���s���a. �󐁻� �i�� ���� ��b��a�� ��eЁ ���e ���a�a!"}},
			  {{"  �a���� �w���� �q���i ���eӡ �e�v�i ���A�e ����Ё ��������. �q���a��"},
			   {"  ���a���� ���� �b�������� ���s���a. �� �A�w�A�e �q�a�a �g�� ���s���a."},
			   {"  �� �a�q �e �a�q�e �q�a�e �a�����a. �἖�e �ᢁ ����Ж�a�a �a���A ���w"},
			   {"  �a�� �g���� �����a������."}},
			  {{"  �q���� �w���� �a���i �e�v�i ���e �ᶂ �� ���i �������a. �a��a �����A"},
			   {"  �� �e�e �a���A�A �s�a�� åЁ�����i �a����e �e�S���a. �a�a�e �w���i"},
			   {"  �a�i�� ���a�e �a�a������ ���s���a. �ᖁ�a�� �i�� �w���w�i ���iЁ ����"},
                           {"  ����."}},
                          {{"  �q���� �w���� �q���i �e�v�i �w���A�e ���i �a�e�A �ᝡ ���e �a���a �I "},
                           {"  ���� ���s���a. ���e �a�q�e �e�a�� ��s�s���a ��a�� �� �����a �I ��"},
                           {"  ���e �a�q���e �ᶂ ���V���a."}},
                          {{"  �a�����a ���b�a�e �w���e �q ���e���ⷥ �a�q�� �� �{�s���a. �a��a �w"},
                           {"  ���e �w��ӡ �៥�� �a�q�i �x�e �a�q�a�� �a�q���a�� ���b�s���a. �iš"},
			   {"  �A�a�ⷥ �a�q�� �a��, �e�� �w���� �Eá�e �b�庁���a�a �a���a�e ������"},
                           {"  �I ���� ���i �������a."}}};

char ss_dis4[][3][5][75] = {{{{"  ��b�ⷡ���a. ���s���a. �a��a �� ������e �� �{���a �|�a�� ����A�e "},
			      {"  ���� �����a�A, ���a�q�� ����ᕡ ���a ñ�b�a�A ���b�a�� �a�� З���a�e "},
                              {"  ���� ���i �������a."}},
                             {{"  ���� �a�e �a�q�����a. �����a�� �������A З���a�e ���e �a�a�� �g���e �a"},
                              {"  �q �ɕ��ⷥ �a�q�� �A�� ���e ���� ���i �������a. �ᢁ ���b�ⷥ З���i "},
                              {"  �����e ���w�� �a���A �e �� ������ ���a�� �a���A ҁ�A�i ���� ���s���a."}},
                             {{"  �����e �a�q�����a �a��a �a �����e ��Ё �i �� ���s���a. �w���e �� �a"},
                              {"  �a���� �ᣡ�i ���g�a �a�e �a�q�����a. �a���� �ᶂ �� �A���� �a���� �a"},
                              {"  ���e �a���e �i�i �鉡 ���ᕡ �廡 ���A�i ��á�e �a���� �a�����a? �A�a"},
                              {"  �a ����� ѥ�a�� ����i ����Ё �֓e�A ���w���ⷥ �w���� �i�i���� ��"},
			      {"  �a�e �a�q�� �q�A �i�� �� ���i �� ���a�V���a."}}},
                            {{{"  ѡ������ �g���a. �w�q�a�� ��b�ⷡ�a ���� ���e �w���{�e �a���e ����"},
                              {"  �a�a�A�e �e�� �a�����e ѡ������ �ᢁ �w�s���a. �ᢁ �a�� �e�eЁ �a�� "},
                              {"  �i����, �a���� �����A�� �����a�e ���巳���a."}},
                             {{"  �w�w�ⷥ �a�q�����a. �ኁ�a ���Eˢ�e �a�q�����a. �A�e ���A ѩ�� �� ��"},
                              {"  �e �� �a ���A ���a �i�� ���i �a�a. �w���� �����ⷥ �����e �i���e �q�e "},
                              {"  �ቡ ����i ����Ё ��������."}},
                             {{"  ���� ���A�� ���a�e �����i �i�e�a�a��, �ᢁ �����ⷡ���a. �a���� �g�a"},
                              {"  �e �a�a �e�e �a�a�� �q�A ���� ���b�����a? �a���e �u���� �A�e ���� �a"},
			      {"  �����a. �i�� �i ɷ�e�a�e ���� ��ӡ�a ���w�s���a. ���� �g���� ����Ё"},
			      {"  ��������."}}}};
int main(void)
{
   int i, j;

   if(init_han() != OK_HAN)	   exit(2);
   if(get_abs_maxx_han() != 79)
	   if(!getch())	getch();
   harderr(error_handler);

   setintr_new();

   logo_display();   if(!getch()) getch();
   logo2_display();   delete(5,20,60);
   init();            delete(10,30,52);
   status_line();
   f_display();
   if(!getch_han())
	getch_han(); delete(3,6,72);
   for(i=0; i<10; i++)
   {
	  set_vbcolor_han(BLUE,LIGHTGRAY);
	  draw_game_screen();
	  disp_game_quiz(i);
	  delete(7,17,66);
   }
   result_dis_page0();   if(!getch_han()) getch_han();   delete(10,12,67);
   result_dis_page1();   if(!getch_han()) getch_han();   delete(6,1,79);
   result_dis_page2();   if(!getch_han()) getch_han();   delete(4,1,79);
   result_dis_page3();   if(!getch_han()) getch_han();   delete(6,1,79);

   close_han();
   setintr_old();
//   exit(1);
   return(0);
}

static void interrupt new_isr09(void)
{
   char far *ptr1;
   char far *ptr2;
   __emit__(0xFA);
   ptr1 = MK_FP(0x40,0x1C);
   ptr2 = MK_FP(0x40,0x1A);
   (*ptr2) = (*ptr1);
   (*old_isr09)();
   __emit__(0xFB);
}

void setintr_new(void)
{
   old_isr09 = getvect(0x9);
   setvect(0x9,new_isr09);
}

void setintr_old(void)
{
   setvect(0x9,old_isr09);
}


void draw_game_screen(void)
{
   int i, j=1;

   draw_3dbox(17, 7, 66, 21, LIGHTGRAY);
   draw_horiz_line(17*8+5, 13*16+8, 49*8-3, BLUE);
   draw_vert_line( 20*8+5, 7*16+9, 14*16-3, BLUE);
   vputs_han(18,  9, "��", BLUE);
   vputs_han(18, 11, "��", BLUE);
   for(i = 0; i < 3; i++, ++j)
	  vprintf_han(18, 14+i+j, BLUE,"%c%c", SPC_ID, i+'1');
}

void f_display(void)
{
   int i=0, j=2;

   draw_3dbox(6, 3, 72, 23, LIGHTBLUE);
   set_vbcolor_han(LIGHTCYAN,LIGHTBLUE);
   aputs_han(12, 5, ss1[0], FONT_HORIZ_DBL|FONT_VERT_DBL);
   for(i=1; i < 8; ++i, j+=2)
	  vputs_han(8, 6+j, ss1[i], LIGHTCYAN);
}

void disp_game_quiz(int num)
{
   int i, j=1,key, guess;

   for(i = 0; i < 3;i++,++j) {
	  if (i <= 1)
		  nputs_han(21,  8+i+j, ss_quest[num][i], 45);
	  nputs_han(23, 14+i+j, ss_item[num][i], 40);
   }
   while (1)
   {
	  if (kbhit())
	  {
		 if ((key = getch_han()) == 0) getch_han();

		 item_num[num] = (key - '1') + 1;
		 if(item_num[num] == 1) break;
		 else if(item_num[num] == 2) break;
		 else if(item_num[num] == 3) break;
	  }
   }
}

void result_dis_page1(void)
{
   int i=0,j=0,temp=0, line=0;

   set_vbcolor_han(BLUE,LIGHTGRAY);
   draw_3dbox(1,6,79,22,LIGHTGRAY);
   for(i=0; i<4; i++,temp++){
	  put_han_font(3,7+line+temp,212,239);
	  for(j=0; j<3; j++, line++)
		 nputs_han(5, 7+line+temp,ss_dis1[i][item_num[i]-1][j],73);
   }
}

void result_dis_page2(void)
{
   int i=0, j=0, temp=0, line=0, tt;

   set_vbcolor_han(BLUE, LIGHTGRAY);
   draw_3dbox(1,4,79, 24, LIGHTGRAY);
   for(i=4; i<6; i++, temp++){
	  put_han_font(3,5+line+temp, 212,239);
	  for(j=0; j<3; j++, line++)
		  nputs_han(5,5+line+temp,ss_dis1[i][item_num[i]-1][j],73);
   }
   temp++;
   put_han_font(3,5+line+temp, 212,239);
   for(j=0; j<4; j++, line++)
	  nputs_han(5,5+line+temp,ss_dis2[item_num[6]-1][j],73);

   if (item_num[7]==3) tt = 4;
   else
   if(sex==0) {
	if(item_num[7] == 1) tt= 0;
	else		     tt= 1;
   } else {
	if(item_num[7] == 1) tt= 2;
	else  		     tt= 3;
   }
   put_han_font(3,6+line+temp, 212, 239);         line++;
   nputs_han(5,5+line+temp,ss_dis_f[tt][0],73);   line++;
   nputs_han(5,5+line+temp,ss_dis_f[tt][1],73);   line++;
   nputs_han(5,5+line+temp,ss_dis_f[tt][2],73);   line++;
   nputs_han(5,5+line+temp,ss_dis_f[tt][3],73);
}

void result_dis_page3(void)
{
   int i,j,temp=0, line=0;

   set_vbcolor_han(BLUE,LIGHTGRAY);
   draw_3dbox(1,6,79,22,LIGHTGRAY);
   for(i=8;i<10; i++, temp++){
	  put_han_font(3,7+line+temp,212,239);
	  for(j=0; j<5; ++j, line++)
		  nputs_han(5,7+line+temp,ss_dis4[temp][item_num[i]-1][j],73);
   }
}


void logo_display(void)
{
   vclrscr_han(BLACK);
   draw_3dbox(20,5,60,18,LIGHTBLUE);   set_vbcolor_han(BLUE,LIGHTBLUE);

   aputs_han(24,7,"���b�A�a�a C.S.C.",FONT_HORIZ_DBL|FONT_VERT_DBL);
   vputs_han(31,10,"��e ���� �墅 ���b",BLUE);
   vputs_han(31,12,"��a���e�� 94�b��",BLUE);
   vputs_han(31,14,"�e �e �� : �w �� �A",BLUE);
   vputs_han(31,16,"��a ���a�A�A...",YELLOW);

   draw_3dbox(20,18,60,22,LIGHTGRAY);   set_vbcolor_han(BLUE,LIGHTGRAY);
   draw_horiz_line(46*8,19*16+12,10*8-1,WHITE);
   draw_horiz_line(46*8+1,21*16+2,10*8,DARKGRAY);
   draw_vert_line(46*8,19*16+12,1*16+6,WHITE);
   draw_vert_line(56*8,19*16+12,1*16+6,DARKGRAY);

   put_han_font(47,20,212,78);
   put_han_font(49,20,212,69);
   put_han_font(51,20,212,88);
   put_han_font(53,20,212,84);
}

void delete(int del_top, int del_left, int del_right)
{
   set_vbcolor_han(BLACK,BLACK);
   delay(500);
   del_top--;
   while((++del_top) <= 24){
	  vclear_text_han(del_left, 25, del_right, 25, BLACK);
	  xscrolldn_han(del_left,del_top,del_right,24);
	  delay(10);
   }
   delay(300);
}

void init(void)
{
   char key[2];

   draw_3dbox(30,10,52,16,LIGHTGRAY);
   set_vbcolor_han(BLACK,LIGHTGRAY);
   draw_horiz_line(30*8+5,12*16+8,22*8-3,BLUE);
   draw_horiz_line(30*8+5,14*16+8,22*8-3,BLUE);
   vputs_han(32,11,"��  �q : ",BLUE);

   set_to_han_mode();
   set_max_strlen_han(9);
   disable_han_change();
   gotoxy_han(41,11);
   gets_han(name);
   vputs_han(32,13,"�i���w : ",BLUE);

   set_keyflag_han(NUM_LOCK|CAPS_LOCK);
   set_max_strlen_han(2);
   set_to_eng_mode();
   gotoxy_han(41,13);
   gets_han(pi);
   vputs_han(32,15,"��  �i : ",BLUE);
   draw_3dbox(33,17,50,19,LIGHTGRAY);
   vputs_han(34,18,"1. �a�a  2. �q�a",BLUE);
   set_max_strlen_han(1);
   while(1){
	   gotoxy_han(41,15);	   gets_han(key);
	   sex = (int)key[0] - '1';
	   if(sex == 0) break;
	   else
	   if(sex == 1) break;
  }
}

void status_line(void)
{
   vclear_text_han(0,29,79,29,LIGHTGRAY);
   set_vbcolor_han(BLUE,LIGHTGRAY);
   vputs_han(1,29, "��  �q : ",BLUE);   vputs_han(10,29,name,BLUE);
   vputs_han(20,29,"�i���w :", BLUE);   vputs_han(29,29,pi,  BLUE);
   vputs_han(31,29,"�w",BLUE);
   vputs_han(40,29,"��a ���e�� �剷���i   C.  S.  C.",BLUE);
}

void result_dis_page0(void)
{
   draw_3dbox(12,10,67,20,LIGHTGRAY);
   vputs_han(17,12,ss_dis0[0],YELLOW);
   set_vbcolor_han(BLUE,LIGHTGRAY);
   vputs_han(15,14,ss_dis0[1],BLUE);
   vputs_han(15,16,ss_dis0[2],BLUE);
   vputs_han(15,18,ss_dis0[3],BLUE);
}

void logo2_display(void)
{
   draw_horiz_line(46*8, 19*16+12,10*8-1,DARKGRAY);
   draw_horiz_line(46*8+1,21*16+2,10*8,  WHITE);
    draw_vert_line(46*8, 19*16+12,1*16+6,DARKGRAY);
    draw_vert_line(56*8, 19*16+12,1*16+6,WHITE);
}
