extern int get_faddr(char c);

void huge main()
{
	int sg, of;

	sg=get_faddr(0);
	of=get_faddr(1);
	asm mov ax, 0;
	asm mov cx, sg;
	asm mov dx, of;
	asm int 0x60;
}
