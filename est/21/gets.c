extern BYTE clipping;
WORD han_stack[7];
BYTE s_stack[7];
BYTE stack_loc=0;

void vResetHangulStack(void)
{
	stack_loc=0;
}

void vPushHangul(WORD i, BYTE s)
{
	han_stack[stack_loc]=i;
	s_stack[stack_loc]=s;
	stack_loc++;
}

WORD wPopHangul(void)
{
	if(stack_loc==0) {
		_SETSTART;
		return HANNULL;
	}
	stack_loc--;
	status=s_stack[stack_loc];
	return han_stack[stack_loc];
}


void vPutStringLength(int x, int y, BYTE *str, BYTE attr, BYTE sloc, BYTE len)
{
	BYTE temp[0xff];
	register int i;
	BYTE flag;

	vMCOFF();
	bminy=(y<<3)-1;
	bmaxy=(y+len)<<3;
	bminx=-1;
	bmaxx=MaxX;
	clipping=ON;
	for(i=0; i<sloc;) {
		if(str[i++]<127) continue;
		i++;
	}
	flag=i-sloc;
	strcpy(temp, str+sloc-flag);
	for(i=flag; i<len;) {
		if(temp[i++]<127) continue;
		i++;
	}
	temp[len+i-len]=0;
	vWriteString(x, y-flag, temp, attr);
	if(strlen(temp)-flag<len)	vBarByte(x, y+strlen(temp)-flag, x+15, y+len, 0);
	clipping=OFF;
	vClipOn(cur_w);

}

int iChechHangul(BYTE *str, int loc)
{
	register int i;
	BYTE flag=0;

	for(i=0; i<loc ;) {
		if(str[i++]<127) {
			flag=0;
			continue;
		}
		i++;
		flag=1;
	}
	return flag;
}

int iInsertByte(BYTE *buf, BYTE loc, BYTE len, BYTE flag)
{
	if(strlen(buf)+flag>=len) {
		vBeep();
		return 0;
	}
	movmem(buf+loc, buf+loc+flag, len-loc-flag);
	return 1;
}

int tx, ty;

void vSaveTC(void)
{
	tx=iCX;
	ty=iCY;
}

void vRestTC(void)
{
	iCX=tx;
	iCY=ty;
	_SETSTART;
}


int iGetString(BYTE flag, int x, int y, BYTE len, BYTE scr_len, BYTE *buf)
{
	INKEY ch;
	BYTE h_e;
	BYTE new_flag;
	BYTE start_flag;
	BYTE scr_c=0;
	CODE2 fst;
	BYTE loc;
	int rtn_num;

	len-=2;
	cur_hsize=0x10;
	cursor_hsize=1;
	cursor_vsize=1;
	if(flag) h_e=han_kbd;
	else h_e=eng_kbd;
	c.i=HANNULL;
	loc=strlen(buf);
	vSaveTC();
	iCX=x;
	iCY=y+loc;
	_SETSTART;
	vBarByte(x, y, x+15, y+scr_len, 0);
	vPutStringLength(x, y, buf, 0, 0, scr_len);
	vMCOFF();
	if(loc>scr_len) {
		vXORBarBytex16(x, y, scr_len);
		scr_c=loc-scr_len+2;				//  Update 1993.7.22
		iCY=y+loc-scr_c;           //  By Jeon Joon Hee
	}
	else {
		vXORBarBytex16(x, y, loc);
	}
	vMCON();
	if(loc<scr_len) vTCON();
	while(!wCheckKeyHit()) {
		vTextCursorOperation(0, h_e);
		if(rbp) {
			rtn_num=-4; goto OUT;
		}
		if(lbp) {
			if(mousex>x && mousex<x+16 && mousey>y*8 && mousey<(y+scr_len)*8) {
				while(lbp) {
					if(mousex<x || mousex>x+16 || mousey<y*8 || mousey>(y+scr_len)*8) {
						rtn_num=-4;	goto OUT;
					}
				}
			}
			else {
				rtn_num=-4; goto OUT;
			}
		}
	}
	vTCOFF();
	ch.i=bioskey(0);
	vPutKey(ch.ch[1], ch.ch[0]);
	if(ch.ch[0] && ch.ch[0]!=BS) {
		if(ch.ch[0]!=RETURN && ch.ch[0]!=ESC) {
			loc=0;
			buf[loc]=0;
			iCY=y;
			setmem(buf, len, 0);
		}
	}
	for(;;) {
		vMCON();
		vTCON();
		while(!wCheckKeyHit()) {
			vTextCursorOperation(0, h_e);
			if(rbp) {
				rtn_num=-4; goto OUT;
			}
			if(lbp) {
				if(mousex>x && mousex<x+16 && mousey>y*8 && mousey<(y+scr_len)*8) {
					while(lbp) {
						if(mousex<x || mousex>x+16 || mousey<y*8 || mousey>(y+scr_len)*8) {
							rtn_num=-4; goto OUT;
						}
					}
				}
				else {
					rtn_num=-4; goto OUT;
				}
			}
		}
		ch.i=bioskey(0);
		vTCOFF();
		vMCOFF();
		if(ch.ch[0]==32 && _LEFT_SHIFT && flag) {
			if(h_e==han_kbd) h_e=eng_kbd;
			else h_e=han_kbd;
			if(start==OFF) {
				loc+=2;
				iCY+=2;
				_SETSTART;
			}
			continue;
		}
		if(!ch.ch[0]) {
			switch(ch.ch[1]) {
				case RIGHT :
					_SETSTART;
					if(loc==strlen(buf)) break;
					if(buf[loc]>127) {
						loc+=2;
						iCY+=2;
					}
					else {
						loc++;
						iCY++;
					}
					break;
				case LEFT :
					_SETSTART;
					if(loc==0) break;
					if(iChechHangul(buf, loc)) {
						loc-=2;
						iCY-=2;
						if(iCY<y) {
							if(scr_c<2) {
								scr_c=0;
								iCY=y;
							}
							else {
								scr_c-=2;
								iCY+=2;
							}
						}
					}
					else {
						loc--;
						iCY--;
						if(iCY<y) {
							scr_c--;
							iCY++;
						}
					}
					break;
				case DOWN :
DN:				rtn_num=-2; goto OUT;
				case UP :	rtn_num=-3; goto OUT;
				case F7 : rtn_num=-10; goto OUT;
				case SFT_TAB : rtn_num=-16; goto OUT;
				case PGDN :	rtn_num=-5; goto OUT;
				case HOME :
					_SETSTART;
					iCY=y;
					loc=0;
					scr_c=0;
					break;
				case END :
					_SETSTART;
					loc=strlen(buf);
					if(loc>scr_len) scr_c=loc-scr_len+2;
					iCY=y+loc-scr_c;
					break;
				case DELETE :
				 _SETSTART;
				 if(buf[loc]==0) break;
				 if(buf[loc]>127) movmem(buf+loc+2, buf+loc, len-loc-2);
				 else movmem(buf+loc+1, buf+loc, len-loc-1);
				 break;
				case F3 : rtn_num=-20; goto OUT;
				case F1 : rtn_num=-11; goto OUT;
				case ALT_M : rtn_num=1000; goto OUT;
			}
		}
		else {
			switch(ch.ch[0]) {
				case TAB : rtn_num=-15; goto OUT;
				case ESC : rtn_num=-1; goto OUT;
				case BS :
					if(!start) {
						c.i=wPopHangul();
						if(c.i==HANNULL) {
							vPutKey(DELETE, 0);
							break;
						}
						buf[loc]=c.c2[1];
						buf[loc+1]=c.c2[0];
						break;
					}
					vPutKey(LEFT, 0);
					vPutKey(DELETE, 0);
					break;
				case RETURN :
RN:				rtn_num=1;
					goto OUT;
				default :
					if(ch.ch[0]<32 || ch.ch[0]>126) break;
					if(h_e==eng_kbd) {
						if(!iInsertByte(buf, loc, len, 1)) break;
						iCY++;
						buf[loc++]=ch.ch[0];
						break;
					}
					else {
						start_flag=start;
						vPushHangul(c.i, status);
						if(h_e==1) new_flag=bCombine2(ch.ch[0], &fst.i);
						else new_flag=bCombine3(ch.ch[0], &fst.i);
						if(new_flag==1) {
							if(start_flag) {
								if(!iInsertByte(buf, loc, len, 2)) {
									_SETSTART;
									break;
								}
							}
							buf[loc]=c.c2[1];
							buf[loc+1]=c.c2[0];
						}
						else if(new_flag==0) {
							buf[loc]=fst.c2[1];
							buf[loc+1]=fst.c2[0];
							if(!iInsertByte(buf, loc, len, 2)) {
								_SETSTART;
								break;
							}
							loc+=2;
							vResetHangulStack();
							buf[loc]=c.c2[1];
							buf[loc+1]=c.c2[0];
							iCY+=2;
							if(status>CHOSONG2) {
								fst.i=c.i;
								fst.c3.code2=2;
								vPushHangul(fst.i, CHOSONG1);
							}
						}
						else {
							if(start!=ON) {
								loc+=2;
								iCY+=2;
							}
							_SETSTART;
							if(!iInsertByte(buf, loc, len, 1)) break;
							iCY++;
							buf[loc++]=new_flag;
						}
					}
					break;
			}
		}
		if(iCY>y+scr_len-3) {
			scr_c+=iCY-y-scr_len+3;
			iCY-=iCY-y-scr_len+3;
		}
		vPutStringLength(x, y, buf, 0, scr_c, scr_len);
	}
OUT:
	vRestTC();
	return rtn_num;
}
