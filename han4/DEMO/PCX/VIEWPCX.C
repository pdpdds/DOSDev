#include <stdio.h>
#include <graphics.h>
#include "hanio.h"
#include "extendio.h"
#include "keydef.h"
#include "pcx.h"

void set_rgb_palette(int color_num, int red, int green, int blue);

void show_16color(void)
/* 16 ���a�� �i�a�i �b�a�e �q�� */
{
   int i;

   for(i = 0; i < 16; i++) vputs_han(0, i + 2, "16���a�� �i�a�i �b", i);
   if(!getch()) getch();
   clrscr_han();
}

void set_default_palette(void)
{
   int de_pal[16][3] =
   {
      {  0,  0,  0 }, {  0,  0, 42 }, {  0, 42,  0 }, {  0, 42, 42 },
      { 42,  0,  0 }, { 42,  0, 42 }, { 42, 21,  0 }, { 42, 42, 42 },
      { 21, 21, 21 }, { 21, 21, 63 }, { 21, 63, 21 }, { 21, 63, 63 },
      { 63, 21, 21 }, { 63, 21, 63 }, { 63, 63, 21 }, { 63, 63, 63 }
   };
   int i;

   for(i = 0; i < 16; i++)
      set_rgb_palette(i, de_pal[i][0], de_pal[i][1], de_pal[i][2]);
}

void main(void)
{
   char file_name[128];
   int ret;

   /* �e�i �ŉw ������ */
   if(init_han() != OK_HAN)
   {
      puts("Hangul environment error.");
      exit(2);
   }

   /* �e�� �i�A�a�i ��w�e�a. */
   puts_han("������ �i�A�a�e �a�q�� �{�e �������a.");
   show_16color();

   while(1)
   {
      /* ���Bɡ�� �b�a�i ���a ���� �� �����i ��Ȃ�a���� �e�a. */
      ret = dir_box_long((get_maxx_han() - 58) / 2, 5, "*.pcx", file_name);
      /* <Esc>ǡ�i ���a�e �{���a. */
      if(ret == ESC) break;

     /* �B�a�a ���a�� (0, 0)�A PCX �����i ���a �� ҁ, ����  �t�A  �a�a */
     /* ����e �A�����i �b�e�a.		      		       */
     /* show_pcx() �����A ���� ���q�e ��a ���e PCX �����i �� �� ���a. */
     ret = show_pcx(0, 0, file_name);
     switch(ret)
     {
	case OK_PCX :
	   xputs_han(0, maxy_han, "�a�� ǡ�a ���a�A�a.");
	   break;
	case FILE_NOT_FOUND_PCX :
	   xputs_han(0, maxy_han, "������ ���s���a.");
	   break;
	case READ_HEADER_ERROR_PCX :
	   xputs_han(0, maxy_han, "�����i ���i �� ���s���a.");
	   break;
	case NOT_PCX_FILE :
	   xputs_han(0, maxy_han, "PCX �w���� ������ �a�����a.");
	   break;
	case NOT_16COLOR_PCX :
	   xputs_han(0, maxy_han, "EGA/VGA�A��e HGC�w �����i �� �� ���s���a.");
	   break;
	case NOT_2COLOR_PCX :
	   xputs_han(0, maxy_han, "HGC�A��e 16���w �����i �� �� ���s���a.");
	   break;
	case MEMORY_NOT_ENOUGH_PCX :
	   xputs_han(0, maxy_han, "�A�����a �����s���a.");
	   break;
     }
     if(!getch()) getch();
     clrscr_han();

     /* �i�A�a�i �����a�� �巁 16���i �b */
     puts_han("�e�� ����E �i�A�a�� ���e �a�q�� �{�s���a.");
     show_16color();

     set_default_palette();
     /* �i�A�a�i �����e ҁ�� 16���i �b */
     puts_han("������ �i�A�a�� �����e ҁ�� ���e �a�q�� �{�s���a.");
     show_16color();
   }
   close_han();
}

