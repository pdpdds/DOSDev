/* ===================================================================
   프로그램 : 위/아래 스크롤 프로그램
   함수인자 : 시작열,시작행,마지막열,마지막행,스크롤라인수,속성,스크롤방법
   작 성 자 : 이경환 / 1995.01
   ================================================================= */

void scroll(int scol,int srow,int ecol,int erow,int lines,int attr,int sw)
{
	union REGS regs;

	if(sw==UP)
		regs.h.ah=6;
	else
		regs.h.ah=7;

	regs.h.al=lines;
	regs.h.ch=srow;
	regs.h.cl=scol;
	regs.h.dh=erow;
	regs.h.dl=ecol;
	regs.h.bh=attr;
	int86(0x10,&regs,&regs);
}