/* ===================================================================
   ���α׷� : ��/�Ʒ� ��ũ�� ���α׷�
   �Լ����� : ���ۿ�,������,��������,��������,��ũ�Ѷ��μ�,�Ӽ�,��ũ�ѹ��
   �� �� �� : �̰�ȯ / 1995.01
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