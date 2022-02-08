/* ----------------------------------------------------------------------
	프로그램 : 지정한 영역을 읽은 후 저장한다 (비디오램 이용)
	함수인자 : 시작열,시작행,마지막열,마지막행,속성
	작 성 자 : 1995.01 / 미스테리
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