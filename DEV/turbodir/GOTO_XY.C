 void goto_xy(int col, int row)
 {
	union REGS regs;

	regs.h.ah=2;

	regs.h.bh=0;
	regs.h.dl=col;
	regs.h.dh=row;

	int86(0x10,&regs,&regs);
 }
