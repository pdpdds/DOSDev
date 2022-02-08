#include <dos.h>
#include "mouse.h"

static int mouse_cursor = 0, /* 1 => show , 0 => hide */
		   mouse_init = 0;	/* 1=> initialized, 0 => not */

#pragma inline
/*-------------------------------------------------------------------------*/
int mouse_initialize(void)
{	int __ax;

	asm xor ax, ax
	asm int 0x33
	asm mov __ax, ax

	if (__ax != 0) mouse_init = 1;
	return __ax;      					/* failed initialized ==> RETURN 0 */
}
/*-------------------------------------------------------------------------*/
void mouse_show_cursor(void)
{   if (mouse_cursor == 1 || mouse_init == 0) return;
	mouse_cursor = 1;

	asm mov ax, 1
	asm int 0x33
}
/*-------------------------------------------------------------------------*/
void mouse_hide_cursor(void)
{   if (mouse_cursor == 0 || mouse_init == 0) return;
	mouse_cursor = 0;

	asm mov ax, 2
	asm int 0x33
}
/*-------------------------------------------------------------------------*/
void mouse_information(int *left,int *right,int *x,int *y)
{   int bx;

	if (mouse_init == 0)
	{	*x = *y = *left = *right = 0;
		return;
	}

	asm mov ax, 3
	asm int 0x33;              /*  RIGHT ~~~\      /~~~ LEFT               */
	bx = _BX;                  /*             \  /                         */
	*x = _CX;                  /*              ||						   */
	*y = _DX;                  /* _BX = 0000 0000  ==> NO BUTTON           */
							   /* _BX = 0000 0001  ==> left BUTTON 		   */
	*left = bx & 0x01;		   /* _BX = 0000 0010  ==> right BUTTON 	   */
	*right = (bx & 0x02)>>1;  /* _BX = 0000 0011  ==> left & right BUTTON */
}
/*-------------------------------------------------------------------------*/
void mouse_move_cursor(int x, int y)
{    if (mouse_init == 0) return;

	_AX = 4;
	_CX = x;
	_DX = y;
	asm int 0x33
}
/*-------------------------------------------------------------------------*/
int mouse_times_released(int button,int *num,int *x,int *y)
{   if (mouse_init == 0)
	{	*num = *x = *y =0;
		return 0;
	}

	_AX = 6;
	_BX = button;       /* button = 0   ===>  LEFT button  */
	asm int 0x33        /* button = 1   ===>  RIGHT button */
	asm push cx
	asm mov cx,bx
	*num = _CX;
	asm pop cx
	*x = _CX;
	*y = _DX;
	return _AX;         /* button push ==> return 1 else 0 */
}
/*-------------------------------------------------------------------------*/
void mouse_range(int left,int top,int right,int bottom)
{   if (mouse_init == 0) return;

	_AX = 7;
	_CX = left;
	_DX = right;
	asm int 0x33

	_AX = 8;
	_CX = top;
	_DX = bottom;
	asm int 0x33
}
/*-------------------------------------------------------------------------*/
int mouse_check_click(int left,int top,int right,int bottom)
{   int m_l,m_r,m_x,m_y,
		button, num,
		old_mouse_cursor=mouse_cursor,
		ret=-1;

	if (mouse_init == 0) return 0;
	if (mouse_cursor == 0) mouse_show_cursor();
	mouse_information(&m_l,&m_r,&m_x,&m_y);
	button = (m_l == 1)? 0 : 1;
	do
	{	if (mouse_times_released(button,&num,&m_x,&m_y) == 0)
		{	ret = 1;
			break;
		}
		mouse_information(&m_l,&m_r,&m_x,&m_y);
		if (m_x<left || m_x>right || m_y<top || m_y>bottom)
		{	ret = 0;
			break;
		}
	} while (ret == -1);
	if (old_mouse_cursor == 0) mouse_hide_cursor();
	return ret;
}
/*-------------------------------------------------------------------------*/
int mouse_get_cursor(void)
{	return mouse_cursor;
}
/*-------------------------------------------------------------------------*/