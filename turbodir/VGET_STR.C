 /* --------------------------------------------------------------
	프로그램 : 문자열 입력 프로그램 (비디오램 이용) -3.
	함수인자 : 시작열,시작행,입력버퍼,속성
	작 성 자 : 미스테리 / 1995.01
	------------------------------------------------------------- */

 int get_str(int scol,int srow,char *buf,int len,int attr)
 {
	union key {
		char ch[2];
		int  i;
	} inkey;

	static char prompt=' ';

	char far *v;
	char far *pv;
	char *pbuf;
	int curpos=scol;
	int i;
	int ins=0;

	v=base_mem+(srow*160)+(scol*2);
	goto_xy(scol,srow);

	if(*buf=='\0') {
		pbuf=buf;
		pv=v;
		for(i=0;i<len;i++) {
			*pbuf++='\0';
			*pv++=prompt;
			*pv++=attr;
		}
		*pbuf='\0';
	}
	if(*buf!='\0') {
		pbuf=buf;
		pv=v;
		for(;*pbuf;) {
			*pv++=*pbuf++;
			*pv++=attr;
			curpos++;
		}
		v=pv;
		buf=pbuf;
		goto_xy(srow,curpos);
	}


	for(;;) {
		while(!kbhit());/* {
			put_time(48,7,7<<4|1);
			put_toggle(58,24,7<<4|4);
		} */

		inkey.i=get_key();
		if(inkey.ch[0]) {
			switch(inkey.ch[0]) {
				case  ESC:   return ESC;
				case  ENTER: return ENTER;
				case  BACKSPACE:
					if(curpos>scol) { /* 지정한 시작위치보다 커서위치가 있을 경우 */
						goto_xy(--curpos,srow); /* 좌표감소 */
						v--;
						*v=(char)attr;
						v--;
						*v=' ';
						--buf;					/* 버퍼번지를 감소 */
						*buf='\0';
						break;
					}
					break;
				default :
							if(curpos>=scol+len)   /* 지정된 길이보다 크면 */
							   break;
							if(ins) {
								pbuf=buf;
								pv=v;
								for(i=0;*pbuf;i++)
									pbuf++;
								if(curpos+i>=scol+len)
									break;

								pv=pv+i*2;        /* 문자열의 마지막 위치 */
								for(;i>=0;i--) {
									*(buf+i+1)=*pbuf;
									*(v+i*2+2)=*pbuf;
									--pbuf;
								}
								goto_xy(curpos++,srow);
								*buf++=inkey.ch[0];
								*v++=inkey.ch[0];
								*v++=attr;
							   break;
							}
							*v++=inkey.ch[0];
							*v++=attr;
							*buf++=inkey.ch[0];
							*buf='\0';
							goto_xy(++curpos,srow);
							break;
			}

		} else if(inkey.ch[1]) {
			switch(inkey.ch[1]) {
				case UP:  return UP;
				case DOWN:  return DOWN;
				case LEFT:
					if(curpos>scol) {
						goto_xy(--curpos,srow);
						v-=2;
						--buf;
						break;
					}
					break;
			   case RIGHT:
					  if(curpos>=scol+len)
						break;
					  if(curpos<scol+len) {
						goto_xy(++curpos,srow);
						v+=2;
						++buf;
					}
					break;
			   case  INS:
						if(ins) {   /* Insert  상태에서 다시 Insert키를 눌러다면 */
							ins=0;
							break;
						}
						ins=1;


			}
		}
	}
}








