

 int get_key(void)
 {
	int ret;
	union REGS regs;

	regs.h.ah=0;
	ret=int86(0x16,&regs,&regs);
	return (ret);
 }

