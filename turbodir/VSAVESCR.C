/* ----------------------------------------------------------------------
	���α׷� : ������ ������ ���� �� �����Ѵ� (������ �̿�)
	�Լ����� : ���ۿ�,������,��������,��������,�Ӽ�
	�� �� �� : 1995.01 / �̽��׸�
	-------------------------------------------------------------------- */


 unsigned char *save_screen(int scol,int srow,int ecol,int erow,int attr)
 {
	char far *v;
	unsigned char *sbuf;
	unsigned char *buf;
	register int i,j;

	v=base_mem+(scol*2)+(srow*160);

	buf=(unsigned char *) malloc((ecol-scol+1)*2+(erow-srow+1)*160);

	if(!buf)
		exit(1);

	sbuf=buf;

	for(i=srow;i<=erow;i++) {
		for(j=scol;j<=ecol;j++) {
			*buf++=*v++;
			*buf++=*v++;
		}
		v=base_mem+(scol*2)+((i+1)*160);
	}
	xy_cls(scol,srow,ecol,erow,attr);

	return(sbuf);
 }
