 /* ----------------------------------------------------------------------
	프로그램 : 저장하였던 화면을 복구한다(비디오램 이용)
	함수인자 : 시작열,시작행,마지막열,마지막행,메모리확보 시작번지
	작 성 자 : 1995.01 / 미스테리
	-------------------------------------------------------------------- */


 void restore_screen(int scol,int srow,int ecol,int erow,unsigned char *buf)
 {
	char far *v;
	register int i,j;

	v=base_mem+(scol*2)+(srow*160);

	for(i=srow;i<=erow;i++) {
		for(j=scol;j<=ecol;j++) {
			*v++=*buf++;
			*v++=*buf++;
		}
		v=base_mem+(scol*2)+((i+1)*160);
	}
/*	free(buf); */
 }


