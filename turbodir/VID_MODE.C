/* =========================================================================
	���α׷� : ���� ��� ���ϱ�
	�Լ����� : void
	��    �� : 1995.01 / �̽��׸�
   ===================================================================== */


	void video_mode(void)
	{
		union REGS regs;
		int vmode;

		regs.h.ah=0x0f;
		int86(0x10, &regs, &regs);
		vmode= regs.h.al;

		if(vmode==0||vmode==1||vmode==2||vmode==3||vmode==4||vmode==5||vmode==6)
			base_mem=(char far*) 0xB8000000;
		if(vmode==7)
			base_mem=(char far*) 0xB0000000;
	}

