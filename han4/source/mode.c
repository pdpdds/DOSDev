/****************************************************************************/
/*   TITLE              Hangul Library <HAN> 4.1                            */
/*   SUB-TITLE          set_to_han_mode(), set_to_eng_mode(),               */
/*                      enable_han_change(), disable_han_change(),          */
/*                      get_keyboard_type(), set_keyboard_type(),           */
/*                      get_video_seg(), is_han_mode(), is_vga_mode()       */
/*   FILENAME           mode.c                                              */
/*   DATE & TIME        03/11/92(WED) 18:52                                 */
/*   PROGRAMMER         Lee Hyun-Ho (ID:easyride)                           */
/****************************************************************************/

#include "hanio.h"
#include "extendio.h"

/****************************************************************************/
/*                 Implementation of the library functions                  */
/****************************************************************************/

void set_to_han_mode(void)
/* �e�i ���b �wȁ�� �����e�a. */
{
   han_mode = 1;
}

void set_to_eng_mode(void)
/*  �w�� ���b �wȁ�� �����e�a. */
{
   han_mode = 0;
}

void enable_han_change(void)
/* �e/�w ����ǡ�a �b���A���� �e�a. */
{
   able_to_change_han = 1;
}

void disable_han_change(void)
/* �e/�w ����ǡ�a �b���A�� �g���� �e�a. */
{
   able_to_change_han = 0;
}

int get_keyboard_type(void)
/* �a�e�� ���A�i �����e�a. */
{
   return(bul3_mode);
}

void set_keyboard_type(int type)
/* �a�e�� ���A�i ����e�a. */
{
   bul3_mode = (type == BUL3_TYPE) ? 1 : 0;
}

unsigned get_video_seg(void)
/* ���a�a �a��Ϣ�� �b�A�e ���������� �A�a�E�a�t�i �����e�a. */
{
   return(video_seg);
}

int is_han_mode(void)
/* �e�i ���b �wȁ���e 1 �i �����e�a. */
{
   return(han_mode);
}

int is_vga_mode(void)
/* EGA/VGA ���a�� �������A���a�e 1 �i �����e�a. */
{
   return(in_vga_system);
}

