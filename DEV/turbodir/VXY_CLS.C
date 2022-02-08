 /* -----------------------------------------------------
	프로그램 ; 지정한 영역을 지우는 함수(비디오램)
	함수인자 ; 시작열,시작행,마지막열,마지막행,속성
	작 성 자 ; 1995.01 / 미스테리
	------------------------------------------------------ */

 void xy_cls(int scol,int srow,int ecol,int erow,int attr)
 {

	register int i,j;
	char far *v=base_mem+(scol*2)+(srow*160);

	for(i=srow;i<=erow;i++)
	{
		for(j=scol;j<=ecol;j++)
		{
			*v++=' ';
			*v++=attr;
		}
		v=base_mem+(scol*2)+((i+1)*160);
	}
 }


